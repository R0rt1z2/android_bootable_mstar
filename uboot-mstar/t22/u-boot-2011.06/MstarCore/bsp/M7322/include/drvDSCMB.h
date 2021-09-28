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
/// @brief  Descrambler (Dscmb) Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------
/*! \defgroup DSCMB DSCMB interface

  MSTAR DESCRAMBLE(abbr. DSCMB) engine is used to decrypt the payload of Transport stream packet (ISO 13818-1).
  The design of descramble engine must corporate with DEMUX engine to accomplish decryption process, which including TS packet filtering and TS payload descrambling.

  <b> DSCMB engine support following cipher algorithms. </b> \n
  -# CSA :  Common Scrambling Algorithm, ETSI TS 100 289
  -# DES :  Data Encryption Standard, U.S. FIPS PUB 46-2
  -# AES :  Advanced Encryption Standard, U.S. FIPS PUB 197 (FIPS 197)
  -# AES-CTR :   High-Bandwidth Digital Content Protection (HDCP2.x)
  -# SCTE52 (DVS042) :   Data Encryption Standard ¡V Cipher Block Chaining Packet Encryption Specification
  -# MULTI2 :   ISO/IEC 9979 and assigned registration number 9

  <b> API call flow: </b> \n
   \image html drvDSCMB_pic.png

  *! \defgroup DSCMB_General DSCMB general functions
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_Slot_Control DSCMB alloc/free/connect/disconnect functions.
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_Key DSCMB set type/key related functions.
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_STR DSCMB STR related functions
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_ToBeModified DSCMB api to be modified
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_ToBeModified_Slot_Ctrl DSCMB slot control functions. (To be modified)
  *   \ingroup   DSCMB_ToBeModified

  *! \defgroup DSCMB_ToBeRemove DSCMB api to be removed
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_STB DSCMB functions for STB.
  *   \ingroup   DSCMB

  *! \defgroup DSCMB_STB_General DSCMB general functions for STB.
  *   \ingroup   DSCMB_STB

  *! \defgroup DSCMB_STB_Kladder DSCMB Kladder functions for STB.
  *   \ingroup   DSCMB_STB

  */
//-------------------------------------------------


#ifndef __DRV_DSCMB_H__
#define __DRV_DSCMB_H__

#include "MsTypes.h"
#include "MsCommon.h"

#include "ULog.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_DSMB_LIB_CODE              {'D','S','M','B'}    //Lib code
#define MSIF_DSMB_LIBVER                {'0','2'}            //LIB version
#define MSIF_DSMB_BUILDNUM              {'1','8'}            //Build Number
#define MSIF_DSMB_CHANGELIST            {'0','0','5','6','5','9','8','0'} //P4 ChangeList Number

#define DSCMB_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DSMB_LIB_CODE,                 /* IP__                                             */  \
    MSIF_DSMB_LIBVER,                   /* 0.0 ~ Z.Z                                        */  \
    MSIF_DSMB_BUILDNUM,                 /* 00 ~ 99                                          */  \
    MSIF_DSMB_CHANGELIST,               /* CL#                                              */  \
    MSIF_OS


#define DSCMB_ASSERT_AND_RETURN(x)             if (!(x)){printf ("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__); return FALSE;}
#define DSCMB_ASSERT_NO_RETURN(x)             if (!(x)){printf ("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);}

#define DSCMB_LIB_TIME_MAX          16      ///< Maximum length of time
#define DSCMB_LIB_DATE_MAX          16      ///< Maximum length of date

typedef struct DLL_PACKED
{
    MS_U8   Time[DSCMB_LIB_TIME_MAX];       ///< Library Built Time
    MS_U8   Date[DSCMB_LIB_DATE_MAX];       ///< Library Built Date
} DSCMB_Info;

typedef struct DLL_PACKED
{
    MS_U32 u32Status;
} DSCMB_Status;

/// Descrambler null filter ID
#define DRV_DSCMB_FLT_NULL          0xFFFFFFFF

typedef enum
{
    /// Descrambler Slot Type :
    E_SLOT_TYPE_NORMAL = 0,  // Normal
    E_SLOT_TYPE_SHARE,       // Share
} DSCMB_SLOT_Type;

typedef enum
{
    E_DSCMB_FLT_3_KEYS        = 0x00000000, // Odd, Even, and Clear Key
    E_DSCMB_FLT_2_KEYS        = 0x00000001, // Odd and Even Key
    E_DSCMB_FLT_2_KEYS_SHARE  = 0x00000002, // Odd and Even Key (share key)
    E_DSCMB_FLT_1_KEYS        = 0x00000003, // Always use the Key
    //Particular pid slot configurations
    E_DSCMB_FLT_PRIV_KEYS_ENABLE     = 1 << 6, // Enable Privilege pid slot
    E_DSCMB_FLT_SECURE_KEYS_ENABLE   = 1 << 7, // Enable Secure pid slot
} DSCMB_Flt_Type;

/// Descrambler algorithm type
typedef enum
{
    // Meaning, Main Algo, Sub Algo, Residual block
    E_DSCMB_TYPE_CSA = 0,    // CSA
    E_DSCMB_TYPE_NSA_AS_ESA, // NSA as ESA
    E_DSCMB_TYPE_DES,        // DES
    E_DSCMB_TYPE_AES,        // AES + CBC + CLR
    E_DSCMB_TYPE_AES_ECB,    // AES + ECB
    E_DSCMB_TYPE_AES_SCTE52, // AES + CBC, DBook
    E_DSCMB_TYPE_AES_CTR,    // AES Counter mode
    E_DSCMB_TYPE_TDES_ECB,   // TDES + ECB
    E_DSCMB_TYPE_TDES_SCTE52,// TDES + CBC
    E_DSCMB_TYPE_SYN_AES,    // Synamedia AES, AES Leading CLR EBC
    E_DSCMB_TYPE_MULTI2,     // Multi2 + CBC
    E_DSCMB_TYPE_CSA3,       // CSA3
    E_DSCMB_TYPE_CSA_CONF,   // CSA conformance mode
    E_DSCMB_TYPE_OC,         // Open Cable
    // new enum for mi3
    E_DSCMB_TYPE_DES_SCTE52, // DES + CBC
    E_DSCMB_TYPE_DES_CLR,    // DES + CBC + CLR
    E_DSCMB_TYPE_TDES_CLR,   // TDES + CBC + CLR
    E_DSCMB_TYPE_MULTI2_CLR, // Multi2 + CBC + CLR
} DSCMB_Type;

/// Descrambler key type
typedef enum
{
    /// Descrambler key type : clear
    E_DSCMB_KEY_CLEAR = 0,
    /// Descrambler key type : even
    E_DSCMB_KEY_EVEN,
    /// Descrambler key type : odd
    E_DSCMB_KEY_ODD,

    /// Enable FSCB
    E_DSCMB_KEY_FSCB_ENABLE =   1 << 4,
    /// Enable KL Key
    E_DSCMB_KEY_TO_KL_ENABLE =  1 << 5,
    /// Enable Secure Key
    E_DSCMB_KEY_SECURE_KEYS_ENABLE =   1 << 7,
} DSCMB_Key_Type;

/// Scramble level
typedef enum
{
    /// Descrambler level : none
    E_DSCMB_NONE                    =   0x00000000,
    /// Descrambler level : TS level
    E_DSCMB_TS                      =   0x00000001,
    /// Descrambler level : PES level
    E_DSCMB_PES                     =   0x00000002,
    /// Descrambler level : TS and PES level
    E_DSCMB_TS_PES                  =   (E_DSCMB_TS | E_DSCMB_PES),
} SCMB_Level;

// Descrambler Engine Type //
typedef enum
{
    E_DSCMB_ENG_LSAS = 0x1,
    E_DSCMB_ENG_ESA  = 0x2,
    E_DSCMB_ENG_LSAD = 0x4,
} DSCMB_Eng_Type;

/// Key ladder input source
typedef enum
{
    /// Key ladder source from ACPU/SCPU
    E_DSCMB_KL_SRC_ACPU             =   0,
    /// Key ladder source from SECRET_1
    E_DSCMB_KL_SRC_SECRET_1         =   1,
    /// Key ladder source from SECRET_2
    E_DSCMB_KL_SRC_SECRET_2         =   2,
    /// Key ladder source from SECRET_3
    E_DSCMB_KL_SRC_SECRET_3         =   3,
    /// Key ladder source from SECRET_4
    E_DSCMB_KL_SRC_SECRET_4         =   4,
    /// Key ladder source from SECRET_5
    E_DSCMB_KL_SRC_SECRET_5         =   5,
    /// Key ladder source from SECRET_6
    E_DSCMB_KL_SRC_SECRET_6         =   6,
    /// Key ladder source from SECRET_7
    E_DSCMB_KL_SRC_SECRET_7         =   7,
    /// Key ladder source from SECRET_8
    E_DSCMB_KL_SRC_SECRET_8         =   8,
    /// Key ladder source from SECRET_9
    E_DSCMB_KL_SRC_SECRET_9         =   16,
    /// Key ladder source from SECRET_10
    E_DSCMB_KL_SRC_SECRET_10        =   10,
    /// Key ladder source from SECRET_11
    E_DSCMB_KL_SRC_SECRET_11        =   11,
    /// Key ladder source from SECRET_12
    E_DSCMB_KL_SRC_SECRET_12        =   12,
    /// Key ladder source from SECRET_13
    E_DSCMB_KL_SRC_SECRET_13        =   13,
    /// Key ladder source from SECRET_14
    E_DSCMB_KL_SRC_SECRET_14        =   14,
    /// Key ladder source from SECRET_15
    E_DSCMB_KL_SRC_SECRET_15        =   15,

    /// Key ladder source from Private_0, only for K3S, K5X, K7
    E_DSCMB_KL_SRC_PRIVATE_0        =   5,
    /// Key ladder source from Private_1, only for K3S, K5X, K7
    E_DSCMB_KL_SRC_PRIVATE_1        =   6,
    /// Key ladder source from Private_2, only for K3S, K5X, K7
    E_DSCMB_KL_SRC_PRIVATE_2        =   7,
    /// Key ladder source from Private_3, only for K3S, K5X, K7
    E_DSCMB_KL_SRC_PRIVATE_3        =   8,

    /// Key ladder source from Private_0
    E_DSCMB_KL_SRC_PRIVATE_EX_0        =   0x40,
    /// Key ladder source from Private_1
    E_DSCMB_KL_SRC_PRIVATE_EX_1        =   0x41,
    /// Key ladder source from Private_2
    E_DSCMB_KL_SRC_PRIVATE_EX_2        =   0x42,
    /// Key ladder source from Private_3
    E_DSCMB_KL_SRC_PRIVATE_EX_3        =   0x43,

    /// Key ladder source from VGK
    E_DSCMB_KL_SRC_VGK              =   9,

    /// Key ladder source go through KDF
    E_DSCMB_KL_SRC_KDF_ENABLE               =   1 << 8,
    /// Key ladder source go through KDF with Module ID
    E_DSCMB_KL_SRC_KDF_ENABLE_WITH_MODID    =   3 << 8,
} DSCMB_KLSrc;

/// Key ladder output destination
typedef enum
{
    E_DSCMB_KL_DST_ACPU             =   0x1,
    E_DSCMB_KL_DST_KT_NSA           =   0x3,
    E_DSCMB_KL_DST_DMA_AES          =   0x4,
    E_DSCMB_KL_DST_DMA_TDES         =   0x5,
    E_DSCMB_KL_DST_PRIVATE0         =   0x6,
    E_DSCMB_KL_DST_PRIVATE1         =   0x7,
    E_DSCMB_KL_DST_PRIVATE2         =   0x8,


    /// Key ladder destination to ACPU
    E_DSCMB_KL_DST_CPU_ACPU         =   0x00000080,
    /// Key ladder destination to Secure CPU
    E_DSCMB_KL_DST_CPU_SCPU         =   0x00000081,
    /// Key ladder destination to TSIO
    E_DSCMB_KL_DST_TSIO             =   0x00000800,
    /// Key ladder destination to DSCMB (KeyTable)
    E_DSCMB_KL_DST_KT_LSAD          =   0x00008000,
    E_DSCMB_KL_DST_KT_ESA           =   0x00008100,
    E_DSCMB_KL_DST_KT_LSAS          =   0x00008200,
    E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE  = (1 << 14),
    /// Key ladder destination to CFB
    E_DSCMB_KL_DST_CFB              =   0x00080000,
    E_DSCMB_KL_DST_CFB_EPK          =   0x00081000,
    E_DSCMB_KL_DST_CFB_EMK          =   0x00082000,
    E_DSCMB_KL_DST_CFB_EFUV         =   0x00083000,
    /// Key ladder destination to DMA
    E_DSCMB_KL_DST_DMA_SK0         =    0x00800000,
    E_DSCMB_KL_DST_DMA_SK1         =    0x00810000,
    E_DSCMB_KL_DST_DMA_SK2         =    0x00820000,
    E_DSCMB_KL_DST_DMA_SK3         =    0x00830000,
    E_DSCMB_KL_DST_DMA_PVRLUT0     =    0x00840000,
    E_DSCMB_KL_DST_DMA_PVRLUT1     =    0x00850000,
    E_DSCMB_KL_DST_DMA_PVRLUT2     =    0x00860000,
    E_DSCMB_KL_DST_DMA_PVRLUT3     =    0x00870000,
    E_DSCMB_KL_DST_DMA_SPS0        =    0x00880000 ,
    E_DSCMB_KL_DST_DMA_SPS1        =    0x00890000 ,
    E_DSCMB_KL_DST_DMA_SPS2        =    0x008A0000 ,
    E_DSCMB_KL_DST_DMA_SPS3        =    0x008B0000 ,
    E_DSCMB_KL_DST_DMA_SPD0        =    0x008C0000 ,
    E_DSCMB_KL_DST_DMA_SPD1        =    0x008D0000 ,
    E_DSCMB_KL_DST_DMA_SPD2        =    0x008E0000 ,
    E_DSCMB_KL_DST_DMA_SPD3        =    0x008F0000 ,
    /// Key ladder destination to PRIVATE KEY
    E_DSCMB_KL_DST_PRIVATE_0       =(int)0x81000000   ,
    E_DSCMB_KL_DST_PRIVATE_1       =(int)0x82000000   ,
    E_DSCMB_KL_DST_PRIVATE_2       =(int)0x83000000   ,
    E_DSCMB_KL_DST_PRIVATE_3       =(int)0x84000000   ,
} DSCMB_KLDst;

typedef enum
{
    // descrambler engine
    E_DSCMB_CAP_ENGINE_NUM,
    // descrambler slot
    E_DSCMB_CAP_FLT_NUM,
    // descrambler type supported
    E_DSCMB_CAP_SUPPORT_ALGORITHM,
    // descrambler key type supported
    E_DSCMB_CAP_SUPPORT_KEY_TYPE,
    // descrambler mapping, start tsp pid filter
    E_DSCMB_CAP_PIDFILTER_MAP_START,
    // descrambler mapping, end tsp pid filter
    E_DSCMB_CAP_PIDFILTER_MAP_END,
    // share key slot max number
    E_DSCMB_CAP_SHARE_KEY_SLOT_MAX_NUM,
    // share key region number
    E_DSCMB_CAP_SHARE_KEY_REGION_NUM,
    // share key region start
    E_DSCMB_CAP_SHARE_KEY_REGION_START,
    // share key region end
    E_DSCMB_CAP_SHARE_KEY_REGION_END,
} DSCMB_Query_Type;

typedef enum
{
    // HDCP2.2 Errata
    E_DSCMB_CFG_HDCP2_TX_CONTENT_CATEGORY_SUPPORT,

    // update scramble bit @ PUSI packet
    E_DSCMB_CFG_SCMB_STATUS_PUSI_ONLY,

} DSCMB_Option;

typedef enum
{
    E_DSCMB_KL_TDES                 =   0x00000000,
    E_DSCMB_KL_AES                  =   0x0000003F,
    // Transformed TDES
    E_DSCMB_KL_T3DES                =   0x00000100,
    // Transformed AES
    E_DSCMB_KL_TAES                 =   0x0000013F,
} DSCMB_KLEng;

typedef enum
{
    E_DSCMB_KL_SEL_DEFAULT = 0, //Backward compatible, actual default value is chosen in HAL driver
    E_DSCMB_KL_SEL_CW      , //CWKL
    E_DSCMB_KL_SEL_PVR     , //PVRKL
} DSCMB_KL_SelEng;

/// Key ladder configurations
typedef enum
{
    E_DSCMB_KL_TYPE_LV0 = 0,
    E_DSCMB_KL_TYPE_LV1 ,
    E_DSCMB_KL_TYPE_LV2 ,
    E_DSCMB_KL_TYPE_LV3 ,
    E_DSCMB_KL_TYPE_LV4 ,
    E_DSCMB_KL_TYPE_LV5 ,
    E_DSCMB_KL_TYPE_INF = 9 ,
    E_DSCMB_KL_TYPE_S5  = 0xD,
} DSCMB_KLType;

typedef enum
{
    E_DSCMB_KL_128_BITS   ,
    E_DSCMB_KL_64_BITS  ,
} DSCMB_KL_OutputSize;

/// Key ladder configurations
typedef struct DLL_PACKED
{
    MS_BOOL         bDecrypt;
    MS_BOOL         bInverse;
    MS_BOOL         bKeepRound;
    MS_U32          u32Round;
    MS_U32          u32EngMode;
    DSCMB_KLType         eKLType;
    DSCMB_KL_OutputSize  eOutsize;

} DSCMB_KLCfg;

typedef enum
{
    E_KDF_HASH_SHA1,
    E_KDF_HASH_SHA256,
} KDF_TYPE;


typedef enum
{
    E_KDF_HW_KEY1,
    E_KDF_HW_KEY2,
    E_KDF_HW_KEY3,
    E_KDF_HW_KEY4,
    E_KDF_HW_KEY_NUM,
} KDF_HW_KEY_SEL;

/// Key ladder KDF configuration
typedef struct DLL_PACKED
{
    MS_U16             u16AppId;
    KDF_TYPE           eKDFType;
    KDF_HW_KEY_SEL     eHWKeyId;
    MS_U8              u8ModId;
} DSCMB_KL_KDF;

typedef enum
{
    DSCMB_FSCB_UNCHG = 0,
    DSCMB_FSCB_CLEAR,
    DSCMB_FSCB_EVEN,
    DSCMB_FSCB_ODD,
} DSCMB_FSCB;

///New  Key ladder configurations for Keyladder run all.
typedef struct DLL_PACKED
{
    DSCMB_KLEng          eAlgo;
    DSCMB_KLSrc          eSrc;   // Select KL root key source //
    DSCMB_KLDst          eDst;   // Select KL output Key destination //
    DSCMB_KL_OutputSize  eOutsize;  // Select ouput Key size. ex: Key size of CSA and DES are E_DSCMB_KL_64_BITS//
    DSCMB_Key_Type       eKeyType;
    // Key ladder running level, support level 0 ~ 3 // [NOTE] Level 0 is only supported when destination is CryptoDMA
    MS_U32               u32Level;
    MS_U32               u32EngID;  // Select ESA/NSA as KL destination, Dscmb engine ID //
    MS_U32               u32DscID;  // Select ESA/NSA as KL destination, Dscmb ID. Select TSIO as KL destination, Tsio ID //
    MS_U8                *u8KeyACPU;  // Select ACPU as KL root Key, KL root key //
    // KL inputs, pointer of bytes array for each level input, every input contains 16 bytes //
    // ex: MS_U8 input[48]: input[0-15] Level1 , input[16-31] Level2, input[32-47] Level3
    MS_U8                *pu8KeyKLIn;
    MS_BOOL              bDecrypt;
    MS_BOOL              bInverse;

    // New data member
    DSCMB_KL_SelEng      eKLSel;   //Select KeyLadder
    MS_U32               u32CAVid; //Set CAVid
    DSCMB_KL_KDF         stKDF;
    DSCMB_FSCB           eFSCB; // Key-specific FSCB
} DSCMB_KLCfg_All;


typedef enum
{
    E_DSCMB_MAIN_ALGO_AES       ,
    E_DSCMB_MAIN_ALGO_CSA2      ,
    E_DSCMB_MAIN_ALGO_DES       ,
    E_DSCMB_MAIN_ALGO_TDES      ,
    E_DSCMB_MAIN_ALGO_MULTI2    ,
    E_DSCMB_MAIN_ALGO_CSA2_CONF ,
    E_DSCMB_MAIN_ALGO_CSA3      ,
    E_DSCMB_MAIN_ALGO_ASA       ,
    E_DSCMB_MAIN_ALGO_TCSA3     ,
    E_DSCMB_MAIN_ALGO_ESSA      ,
    E_DSCMB_MAIN_ALGO_DEFAULT = 0xF,
    E_DSCMB_MAIN_ALGO_NUM,
} DSCMB_MainAlgo_Type;


typedef enum
{
    E_DSCMB_SUB_ALGO_NONE = 0   ,
    E_DSCMB_SUB_ALGO_MDI_CBC = 0,
    E_DSCMB_SUB_ALGO_MDI_RCBC  ,
    E_DSCMB_SUB_ALGO_MDD_CBC   ,
    E_DSCMB_SUB_ALGO_MDD_RCBC  ,
    E_DSCMB_SUB_ALGO_LEADING_CLEAR ,
    E_DSCMB_SUB_ALGO_ECB ,
    E_DSCMB_SUB_ALGO_CBC ,
    E_DSCMB_SUB_ALGO_CTR ,
    E_DSCMB_SUB_ALGO_OFB ,
    E_DSCMB_SUB_ALGO_DEFAULT = 0xF,
    E_DSCMB_SUB_ALGO_NUM,
} DSCMB_SubAlgo_Type;


typedef enum
{
    E_DSCMB_RESSB_ALGO_NONE = 0,
    E_DSCMB_RESSB_ALGO_CLR = 0,
    E_DSCMB_RESSB_ALGO_CTS,
    E_DSCMB_RESSB_ALGO_SCTE52,
    E_DSCMB_RESSB_ALGO_XORIV1,
    E_DSCMB_RESSB_ALGO_OC_M,
    E_DSCMB_RESSB_ALGO_XORIV2,
    E_DSCMB_RESSB_ALGO_CTR,
    E_DSCMB_RESSB_ALGO_LCLR,
    E_DSCMB_RESSB_ALGO_MSC2,
    E_DSCMB_RESSB_ALGO_MSC4,
    E_DSCMB_RESSB_ALGO_MSC12,
    E_DSCMB_RESSB_ALGO_MSC18,
    E_DSCMB_RESSB_ALGO_MSC28,
    E_DSCMB_RESSB_ALGO_DEFAULT = 0xF,
    E_DSCMB_RESSB_ALGO_NUM,
} DSCMB_ResSBAlgo_Type;


typedef struct DLL_PACKED
{
    DSCMB_MainAlgo_Type   eMainAlgo;
    DSCMB_SubAlgo_Type    eSubAlgo;
    DSCMB_ResSBAlgo_Type  eResAlgo;
    DSCMB_ResSBAlgo_Type  eSBAlgo;
    MS_BOOL               bDecrypt;
} DSCMB_Algo_Cfg;

typedef enum
{
    E_DSCMB_CAPVR_PROG0 = 1 ,
    E_DSCMB_CAPVR_PROG1 = 2 ,
    E_DSCMB_CAPVR_PROG2 = 3 ,
    E_DSCMB_CAPVR_PROG3 = 4 ,
} DSCMB_CAPVR_MODE;

typedef enum
{
    //Live 0~3, Filein 0~3
    E_DSCMB_TSIF_LIVE0,
    E_DSCMB_TSIF_LIVE1,
    E_DSCMB_TSIF_LIVE2,
    E_DSCMB_TSIF_LIVE3,
    E_DSCMB_TSIF_FILE0,
    E_DSCMB_TSIF_FILE1,
    E_DSCMB_TSIF_FILE2,
    E_DSCMB_TSIF_FILE3,
    //Live 4~6, Filein 4~6
    E_DSCMB_TSIF_LIVE4,
    E_DSCMB_TSIF_LIVE5,
    E_DSCMB_TSIF_LIVE6,
    E_DSCMB_TSIF_FILE4,
    E_DSCMB_TSIF_FILE5,
    E_DSCMB_TSIF_FILE6,
    E_DSCMB_TSIF_NUM,
} DSCMB_TSIF;

typedef enum
{
    E_DSCMB_CAMOD_ACT_CA,
    E_DSCMB_CAMOD_NULL = 0xFFFF,
} DSCMB_CAMod_ActType ;

// Error Status for KL //s
#define DSCMB_KL_Status MS_U32
//Software keyladder error flag
#define    KL_STATUS_KEY_OK                            0x0000
#define    KL_STATUS_ACPU_KEY_NULL                     0x0001
#define    KL_STATUS_ACPU_OUTKEY_NULL                  0x0002
#define    KL_STATUS_INVALID_KEYROUND_PARAMETER        0x0004
#define    KL_STATUS_INVALID_INPUT_LEVEL               0x0008
#define    KL_STATUS_IVALID_DSCMB_ID                   0x0010
#define    KL_STATUS_WAIT_CW_READY_TIMEOUT             0x0020
#define    KL_STATUS_WAIT_KL_READY_TIMEOUT             0x0040
#define    KL_STATUS_KL_INPUT_NULL                     0x0080

#define    KL_STATUS_BAD_KEY                           0x0100
#define    KL_STATUS_INVALID_FUNCTION                  0x0200
#define    KL_STATUS_MULTI_FUNCTION                    0x0400
#define    KL_STATUS_ZERO_ORDER                        0x0800
#define    KL_STATUS_INTERNAL_BAD_KEY                  0x1000
#define    KL_STATUS_INVALID_KL_SEL                    0x2000
#define    KL_STATUS_INVALID_DESTINATION               0x4000
#define    KL_STATUS_TA_FAILED                         0x8000
#define    KL_STATUS_KT_FAILED                         0x10000

//Hardware keyladder error flag
#define    KL_HW_STATUS_ERROR                          0x80000000
#define    KL_HW_STATUS_INVALID_DST                    0x80000001
#define    KL_HW_STATUS_INVALID_SRC                    0x80000002
#define    KL_HW_STATUS_INVALID_ALGO                   0x80000004
#define    KL_HW_STATUS_INVALID_LEVEL                  0x80000008
#define    KL_HW_STATUS_INVALID_ENG                    0x80000010
#define    KL_HW_STATUS_INVALID_KDF                    0x80000020
#define    KL_HW_STATUS_BAD_ROOT_KEY                   0x80000040
#define    KL_HW_STATUS_BAD_OUTPUT_KEY                 0x80000080
#define    KL_HW_STATUS_NO_SLOT                        0x80000100
#define    KL_HW_STATUS_INVALID_WRITE_KEY              0x80000200
#define    KL_HW_STATUS_OTHER                          0x88000000

// Backward compatible
typedef DSCMB_KLSrc          DSCMB_KLadderSrc;
#define E_DSCMB_KLAD_SRC_ACPU               E_DSCMB_KL_SRC_ACPU
#define E_DSCMB_KLAD_SRC_SECRET_KEY1        E_DSCMB_KL_SRC_SECRET_1
#define E_DSCMB_KLAD_SRC_SECRET_KEY2        E_DSCMB_KL_SRC_SECRET_2
#define E_DSCMB_KLAD_SRC_SECRET_KEY3        E_DSCMB_KL_SRC_SECRET_3
#define E_DSCMB_KLAD_SRC_SECRET_KEY4        E_DSCMB_KL_SRC_SECRET_4
#define E_DSCMB_KLAD_SRC_VGK                E_DSCMB_KL_SRC_VGK

typedef DSCMB_KLDst          DSCMB_KLadderDst;
#define E_DSCMB_KLAD_DST_KTAB_ESA           E_DSCMB_KL_DST_KT_ESA
#define E_DSCMB_KLAD_DST_KTAB_NSA =         E_DSCMB_KL_DST_KT_NSA
#define E_DSCMB_KLAD_DST_AESDMA_AES =       E_DSCMB_KL_DST_DMA_AES
#define E_DSCMB_KLAD_DST_AESDMA_TDES =      E_DSCMB_KL_DST_DMA_TDES
#define E_DSCMB_KLAD_DST_ACPU =             E_DSCMB_KL_DST_ACPU

typedef DSCMB_KLCfg          DSCMB_KLadderConfig;

// CA module activation error flag
#define DSCMB_CAMOD_STATUS  MS_U32
#define CAMOD_STATUS_CONNECTED     0x00
#define CAMOD_STATUS_ACT_WRONGPWD  0x01
#define CAMOD_STATUS_KL_WRONGPWD   0x02
#define CAMOD_STATUS_CA_WRONGPWD   0x04

// wrapper function
#define MDrv_DSCMB_FltAlloc(void)   \
    MDrv_DSCMB2_FltAlloc(0)
#define MDrv_DSCMB_FltAlloc_Ex(eFltType)   \
    MDrv_DSCMB2_FltAlloc_Ex(0, eFltType)
#define MDrv_DSCMB_FltFree(u32DscmbId)   \
    MDrv_DSCMB2_FltFree(0, u32DscmbId)
#define MDrv_DSCMB_FltConnectPid(u32DscmbId, u32Pid)  \
    MDrv_DSCMB2_FltConnectPid(0, u32DscmbId, u32Pid)
#define MDrv_DSCMB_FltDisconnectPid(u32DscmbId, u32Pid) \
    MDrv_DSCMB2_FltDisconnectPid(0, u32DscmbId, u32Pid)
#define MDrv_DSCMB_FltConnectFltId(u32DscmbId, u32FltId)    \
    MDrv_DSCMB2_FltConnectFltId(0, u32DscmbId, u32FltId)
#define MDrv_DSCMB_FltDisconnectPid_Ex(u32DscmbId, u32Pid) \
    MDrv_DSCMB2_FltDisconnectPid_Ex(0, u32DscmbId, u32Pid)
#define MDrv_DSCMB_FltDisconnectFltId(u32DscmbId, u32FltId)    \
    MDrv_DSCMB2_FltDisconnectFltId(0, u32DscmbId, u32FltId)
#define MDrv_DSCMB_FltDscmb(u32DscmbId, bDscmb) \
    MDrv_DSCMB2_FltDscmb(0, u32DscmbId, bDscmb)
#define MDrv_DSCMB_FltTypeSet(u32DscmbId, eType)    \
    MDrv_DSCMB2_FltTypeSet(0, u32DscmbId, eType)
#define MDrv_DSCMB_Multi2_SetRound(u32Round)    \
    MDrv_DSCMB2_Multi2_SetRound(0, u32Round)
#define MDrv_DSCMB_Multi2_SetSystemKey(u8Syskey)    \
    MDrv_DSCMB2_Multi2_SetSystemKey(0, u8Syskey)
#define MDrv_DSCMB_FltKeySet(u32DscmbId, eKeyType, pu8Key)  \
    MDrv_DSCMB2_FltKeySet(0, u32DscmbId, eKeyType, pu8Key)
#define MDrv_DSCMB_FltKeyReset(u32DscmbId, eKeyType)    \
    MDrv_DSCMB2_FltKeyReset(0, u32DscmbId, eKeyType)
#define MDrv_DSCMB_FltIVSet(u32DscmbId, eKeyType, pu8IV)    \
    MDrv_DSCMB2_FltIVSet(0, u32DscmbId, eKeyType, pu8IV)
#define MDrv_DSCMB_PidFlt_ScmbStatus(u32PidFltId, pScmbLevel)    \
    MDrv_DSCMB2_PidFlt_ScmbStatus(0, u32PidFltId, pScmbLevel)
#define MDrv_DSCMB_FltKeyReset(u32DscmbId, eKeyType)    \
    MDrv_DSCMB2_FltKeyReset(0, u32DscmbId, eKeyType)

//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------

// descrambler initial function
//-----------------
// DSCMB_General
//-----------------
//-------------------------------------------------------------------------------------------------
/// Initialize DSCMB module
/// @ingroup DSCMB_General
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_Init(void);

//-------------------------------------------------------------------------------------------------
/// Finalize DSCMB module
/// @ingroup DSCMB_General
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_Exit(void);
//-----------------
// DSCMB_STB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set DSCMB CA VID
/// @ingroup DSCMB_STB_General
/// @param  u32EngId   \b IN: DSCMB engine id.
/// @param  u32CAVid  \b IN: CA VID.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_SetDefaultCAVid(MS_U32 u32EngId, MS_U32 u32CAVid);

//-----------------
// DSCMB_ToBeModified: Slot_Ctrl
//-----------------
//-------------------------------------------------------------------------------------------------
/// Allocate a dscrambler filter (Slot). (Update to support share key case)
/// @ingroup DSCMB_ToBeModified_Slot_Ctrl
/// @return TRUE  : Succeed - Descrambler filter slot
/// @return FALSE : Fail - DRV_DSCMB_FLT_NULL
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32  MDrv_DSCMB2_FltAlloc(MS_U32 u32EngId);

//-------------------------------------------------------------------------------------------------
/// Allocate a dscrambler filter (Slot). (Only this function can allocate share key slot).
/// (Just keep MDrv_DSCMB2_FltAlloc and refine it)
/// @ingroup DSCMB_ToBeRemove
/// @param u32EngId  \b IN: DSCMB engine id.
/// @param eFltType    \b IN: Type for descramber filter.
/// @return TRUE  : Succeed - Descrambler filter slot.
/// @return FALSE : Fail - DRV_DSCMB_FLT_NULL
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32  MDrv_DSCMB2_FltAlloc_Ex(MS_U32 u32EngId, DSCMB_Flt_Type eFltType);
//-----------------
// DSCMB_Slot_Control
//-----------------
//-------------------------------------------------------------------------------------------------
/// Free a dscrambler slot.
/// @ingroup DSCMB_Slot_Control
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltFree(MS_U32 u32EngId, MS_U32 u32DscmbId);

//-------------------------------------------------------------------------------------------------
/// Change descrambler filter type.
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param eFltType    \b IN: Type for descramber filter.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltSwitchType(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Flt_Type eFltType);

//-------------------------------------------------------------------------------------------------
/// Connect DSCMB slot with DMX PID filter by filter index for Enc/Dec used.
/// @ingroup DSCMB_Slot_Control
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param u32FltId          \b IN: DMX PID filter index.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltConnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId);
//-------------------------------------------------------------------------------------------------
/// Dis-connect DSCMB slot with DMX PID filter by filter index.
/// @ingroup DSCMB_Slot_Control
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param u32FltId          \b IN: DMX PID filter index.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltDisconnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId);

// descrambler filter maps to TSP pid
#if !(defined(MSOS_TYPE_OPTEE) || defined(MSOS_TYPE_NUTTX))
//-------------------------------------------------------------------------------------------------
/// Connect DSCMB slot with DMX PID filter by PID for Enc/Dec used.
/// @ingroup DSCMB_Slot_Control
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param u32Pid            \b IN: PID.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltConnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid);
//-------------------------------------------------------------------------------------------------
/// Dis-connect DSCMB slot with DMX PID filter by PID. (Refine for multi dis-connect case)
/// @ingroup DSCMB_ToBeModified_Slot_Ctrl
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param u32Pid            \b IN: PID.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltDisconnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid);
//-------------------------------------------------------------------------------------------------
/// Dis-connect DSCMB slot with DMX PID filter by PID. (It will check this PID exist on DMX side or not)
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param u32Pid            \b IN: PID.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltDisconnectPid_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid);
#endif

//--------------------------------------------------------------------------------------------------
/// Set descrambler filter as scrambling/decrambling mode
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param eEngType        \b IN: DSCMB engine type.
/// @param stConfig          \b IN: DSCMB algo.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetAlgo(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Algo_Cfg stConfig);

//--------------------------------------------------------------------------------------------------
/// Set descrambler key FSCB
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId    \b IN: DSCMB slot.
/// @param eEngType        \b IN: DSCMB engine type.
/// @param eKeyType       \b IN: Descrambler key type
/// @param eForceSCB     \b IN: FSCB type.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetKeyFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, DSCMB_FSCB eForceSCB);

//--------------------------------------------------------------------------------------------------
/// Set descrambler key
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eEngType       \b IN: DSCMB engine type.
/// @param eKeyType       \b IN: Descrambler key type
/// @param pu8Key          \b IN: pointer to the key (Array)
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8Key);

//--------------------------------------------------------------------------------------------------
/// Reset descrambler key
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eEngType       \b IN: DSCMB engine type.
/// @param eKeyType      \b IN: Descrambler key type
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngResetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType);

//--------------------------------------------------------------------------------------------------
/// Set descrambler IV key
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eKeyType      \b IN: Descrambler key type
/// @param pu8IV            \b IN: pointer to the key. (Instruction Vector)
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetIV(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV);

//--------------------------------------------------------------------------------------------------
/// Set descrambler IV key by engine type
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eEngType       \b IN: DSCMB engine type.
/// @param eKeyType      \b IN: Descrambler key type
/// @param pu8IV            \b IN: pointer to the key. (Instruction Vector)
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId,  DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV);

//--------------------------------------------------------------------------------------------------
/// Set Riv of HDCP2, which is 64-bits number generated from transmitter
/// @ingroup DSCMB_STB_General
/// @param u32EngId   \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot
/// @param pu8RIV      \b IN: pointer to 64bit Riv buffer in BIG ENDIAN order
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetRIV(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV);

//--------------------------------------------------------------------------------------------------
/// Switch descrambler engine type
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eUppSwitch    \b IN: Up: Engine type.
/// @param eLowSwitch    \b IN: Low: Engine type.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetSwitch(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eUppSwitch, DSCMB_Eng_Type eLowSwitch);

//--------------------------------------------------------------------------------------------------
/// Set descrambler engine FSCB
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eForceSCB     \b IN: FSCB type.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngSetFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_FSCB eForceSCB );

//--------------------------------------------------------------------------------------------------
/// Enable/disable descrambler key
/// @ingroup DSCMB_STB_General
/// @param u32EngId        \b IN: DSCMB engine id.
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eEngType       \b IN: DSCMB engine type.
/// @param eKeyType      \b IN: Descrambler key type
/// @param bEN      \b IN: enable or disable
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_EngEnableKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_BOOL bEn);
//--------------------------------------------------------------------------------------------------
/// Set descrambler dual path flow
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param eCaMode   \b IN: DSCMB CA mode.
/// @param ePvrSrcTsif     \b IN: PVR source.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_CAPVR_FlowSet(MS_U32 u32EngId, DSCMB_CAPVR_MODE eCaMode, DSCMB_TSIF ePvrSrcTsif);
//--------------------------------------------------------------------------------------------------
/// Set descrambler pid filter of upper path
/// @ingroup DSCMB_STB_General
/// @param eCaMode   \b IN: DSCMB CA mode.
/// @param u32Pid       \b IN: Pid of transport stream.
/// @param bEnable     \b IN: Set or clear.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_CAPVR_SetPidFLT(DSCMB_CAPVR_MODE eCaMode, MS_U32 u32Pid, MS_BOOL bEnable);
//--------------------------------------------------------------------------------------------------
/// Disable stream dual path
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32FltId          \b IN: DMX PID filter index.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_DualPath_Enable(MS_U32 u32EngId, MS_U32 u32FltId);
//--------------------------------------------------------------------------------------------------
/// Enable stream dual path
/// @ingroup DSCMB_STB_General
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32FltId          \b IN: DMX PID filter index.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_DualPath_Disable(MS_U32 u32EngId, MS_U32 u32FltId);


//--------------------------------------------------------------------------------------------------
/// Set descrambler filter as scrambling/decrambling mode.
/// Default filter mode is descrambling if this function is not specified.
/// This function should be invoked before MDrv_DSCMB_FltTypeSet.
/// @ingroup DSCMB_STB_General
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param DscmbType \b IN: The descrambler algorithm
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltDscmb(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_BOOL bDscmb);
//-----------------
// DSCMB_Key
//-----------------
//--------------------------------------------------------------------------------------------------
/// Set descrambler algorithm by common predefined descramble type.
/// @ingroup DSCMB_Key
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param DscmbType    \b IN: The descrambler algorithm
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltTypeSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Type DscmbType);
//--------------------------------------------------------------------------------------------------
/// Set descrambler key of the Dscmb ID
/// @ingroup DSCMB_Key
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eKeyType      \b IN: Descrambler key type
/// @param pu8Key         \b IN: pointer to the key (Array)
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltKeySet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8Key);
//--------------------------------------------------------------------------------------------------
/// Reset descrambler key of the Dscmb ID. (It will set key to 0)
/// @ingroup DSCMB_Key
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eKeyType      \b IN: Descrambler key type
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType);

//--------------------------------------------------------------------------------------------------
/// Set descrambler initial vector of the Dscmb ID
/// @ingroup DSCMB_Key
/// @param u32EngId       \b IN: DSCMB engine id
/// @param u32DscmbId   \b IN: DSCMB slot.
/// @param eKeyType      \b IN: Descrambler key type
/// @param pu8IV            \b IN: pointer to the key. (Instruction Vector)
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltIVSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV);

//--------------------------------------------------------------------------------------------------
/// Get / Set dscrambler miscellaneous options
/// @ingroup DSCMB_General
/// @param u32EngId      \b IN: DSCMB engine id.
/// @param Option Type  \b IN: Option type
/// @param pInput          \b IN: intput parameter
/// @param pOutput        \b OUT: result for the query type
/// @param bSet           \b IN : Set or Get
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_OptConfig(MS_U32 u32EngId, DSCMB_Option eOpt, void* pInput, void* pOutput, MS_BOOL bSet);

//--------------------------------------------------------------------------------------------------
/// Query dscrambler capability information
/// @ingroup DSCMB_General
/// @param u32EngId      \b IN: DSCMB engine id.
/// @param eQueryType  \b IN: type for query
/// @param pInput          \b IN: intput parameter
/// @param pOutput        \b OUT: result for the query type
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_GetCap(MS_U32 u32EngId, DSCMB_Query_Type eQueryType, void* pInput, void* pOutput);
//--------------------------------------------------------------------------------------------------
/// Get DSCMB driver library information
/// @ingroup DSCMB_General
/// @param ppVersion \b OUT: Descrambler library version infomation
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// Set DSCMB Debug Level.
/// @ingroup DSCMB_General
/// @param  u32Level  \b IN: debug level.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_SetDBGLevel(MS_U32 u32Level);
//--------------------------------------------------------------------------------------------------
/// Check the status between DSCMB slot and DMX PID filter.
/// @ingroup DSCMB_STB_General
/// @param  u32EngId      \b IN: DSCMB engine id.
/// @param u32DscmbId   \b IN: descramble id
/// @param u32TspFltId    \b IN: tsp filter id
/// @return TRUE  - Success (connect)
/// @return FALSE - Failure (not connect)
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_GetConnectStatus(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32TspFltId);
//--------------------------------------------------------------------------------------------------
/// Get a valid PID slot map index
/// @ingroup DSCMB_STB_General
/// @param  u32EngId      \b IN: DSCMB engine id.
/// @param u32DscmbId   \b IN: descramble id
/// @return TRUE  : Succeed - Descrambler PID slot map index.
/// @return FALSE : Fail - DRV_DSCMB_FLT_NULL
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32  MDrv_DSCMB2_GetPidSlotMapIndex(MS_U32 u32EngId, MS_U32 u32DscmbId);


//--------------------------------------------------------------------------------------------------
/// Set rounds of descrambler algorithm MULTI2
/// @ingroup DSCMB_Key
/// @param u32EngId     \b IN: DSCMB engine id
/// @param u32Round    \b IN: # rounds to apply, multiple of 4 and maximum value is 252
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round);
//--------------------------------------------------------------------------------------------------
/// Set system key of descrambler algorithm MULTI2
/// @ingroup DSCMB_Key
/// @param u32EngId     \b IN: DSCMB engine id
/// @param u32SysKey  \b IN: the array of system key (u8SysKey[32]): there are 32 group system key(8-bit should be 32 group) should be set.
/// @[0-31][32-63][64-95][96-127][128-159][160-191][192-223][224-255] and the bit order is MSB.
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8Syskey);
//--------------------------------------------------------------------------------------------------
/// Set Riv of HDCP2, which is 64-bits number generated from transmitter
/// @ingroup DSCMB_Key
/// @param u32EngId   \b IN: DSCMB engine id
/// @param pu8RIV      \b IN: pointer to 64bit Riv buffer in BIG ENDIAN order
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_HDCP2_SetRIV(MS_U32 u32EngId, MS_U8* pu8RIV);
//--------------------------------------------------------------------------------------------------
/// Set Riv of HDCP2, which is 64-bits number generated from transmitter
/// @ingroup DSCMB_Key
/// @param u32EngId   \b IN: DSCMB engine id
/// @param u32EngId   \b IN: DSCMB slot
/// @param pu8RIV      \b IN: pointer to 64bit Riv buffer in BIG ENDIAN order
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_HDCP2_SetRIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV);
//--------------------------------------------------------------------------------------------------
/// Enable Record stream after stream is descrambled, only supported when CA venders do not prohibat this feature.
/// @ingroup DSCMB_STB_General
/// @param u32EngId  \b IN: DSCMB engine id
/// @param Enable      \b IN: for enabe/disable record mechanism.
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @Note: Function can only be called before any record function is called.
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_PVR_RecCtrl(MS_U32 u32EngId, MS_BOOL Enable);
//-----------------
// DSCMB_STR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Control DSCMB STR functions.
/// @ingroup DSCMB_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32  MDrv_DSCMB2_SetPowerState(EN_POWER_MODE u16PowerState);

//--------------------------------------------------------------------------------------------------
/// Get scrambler level of a pid filter ID
/// @ingroup DSCMB_General
/// @param u32EngId      \b IN: DSCMB engine id.
/// @param u32PidFltId    \b IN: Pid filter ID obtained from MApi_DMX_Open/MDrv_TSP_PidFlt_Alloc
/// @param pScmbLevel   \b OUT: The output scrambler level
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, SCMB_Level* pScmbLevel);

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver status (There is no any code use this parameter)
/// @ingroup DSCMB_ToBeRemove
/// @param pStatus   \b OUT: Descrambler status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_GetStatus(DSCMB_Status *pStatus);
//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver information
/// @ingroup DSCMB_General
/// @return DSCMB_Info
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC const DSCMB_Info* MDrv_DSCMB_GetInfo(void);
//--------------------------------------------------------------------------------------------------
/// Set Password for CA module activation
/// @ingroup DSCMB_Key
/// @param eActType    \b IN: Type of module for avtivation
/// @param pu8Pwd      \b IN: pointer to 128t Password buffer in BIG ENDIAN order
/// @param u32Status   \b IN: Active status
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_CAMod_Active(DSCMB_CAMod_ActType  eActType , MS_U8* pu8Pwd , DSCMB_CAMOD_STATUS* u32Status);

//-----------------
// DSCMB_STB_Kladder
//-----------------
//--------------------------------------------------------------------------------------------------
/// KLadder_ETSI
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_KLadder_ETSI(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, MS_U8 *pu8Nonce, MS_U8 * pu8Response, DSCMB_KL_Status* u32Status );
//--------------------------------------------------------------------------------------------------
/// KLadder_AtomicExec
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status );


//-----------------
// DSCMB_ToBeRemove
//-----------------
//-------------------------------------------------------------------------------------------------
/// Initialize DSCMB module. (Already have another function: DSCMB2_Init)
/// @ingroup DSCMB_ToBeRemove
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_Init(void);
//-------------------------------------------------------------------------------------------------
/// Finalize DSCMB module. (Already have another function: DSCMB2_Exit)
/// @ingroup DSCMB_ToBeRemove
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_Exit(void);
//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver library information. (Already have another function: DSCMB2_GetLibVer)
/// @ingroup DSCMB_ToBeRemove
/// @param ppVersion \b OUT: Descrambler library version infomation
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// Set DSCMB Debug Level. (Already have another function: DSCMB2_SetDBGLevel)
/// @ingroup DSCMB_ToBeRemove
/// @param  u32Level  \b IN: debug level.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_SetDBGLevel(MS_U32 u32Level);

//--------------------------------------------------------------------------------------------------
/// Check the status between DSCMB slot and DMX PID filter.
/// @ingroup DSCMB_STB_General
/// @param  u32EngId      \b IN: DSCMB engine id.
/// @param u32DscmbId   \b IN: descramble id
/// @param u32TspFltId    \b IN: tsp filter id
/// @return TRUE  - Success (connect)
/// @return FALSE - Failure (not connect)
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_GetConnectStatus(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32TspFltId);
//--------------------------------------------------------------------------------------------------
/// KLadder_AtomicExec
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status);
//--------------------------------------------------------------------------------------------------
/// KLadder_Reset
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Reset(void);
//--------------------------------------------------------------------------------------------------
/// KLadder_SetInput
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_SetInput(MS_U32 u32Level, MS_U8* pu8In, MS_U32 u32InLen);
//--------------------------------------------------------------------------------------------------
/// KLadder_SetKey
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_SetKey(DSCMB_KLSrc KLSrc, MS_U8* pu8Key, MS_U32 u32KeyLen);
//--------------------------------------------------------------------------------------------------
/// KLadder_SetDst
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_SetDst(DSCMB_KLDst KLDst);

//--------------------------------------------------------------------------------------------------
/// KLadder_SetConfig
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC void    MDrv_DSCMB_KLadder_SetConfig(DSCMB_KLCfg *KLCfg);

//--------------------------------------------------------------------------------------------------
/// KLadder_Start
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Start(void);

//--------------------------------------------------------------------------------------------------
/// KLadder_IsComplete
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_IsComplete(void);

//--------------------------------------------------------------------------------------------------
/// KLadder_Output_Start
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Output_Start(MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType);

//--------------------------------------------------------------------------------------------------
/// KLadder_Output_IsComplete
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Output_IsComplete(void);

//--------------------------------------------------------------------------------------------------
/// KLadder_Output_Stop
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Output_Stop(void);

//--------------------------------------------------------------------------------------------------
/// KLadder_ResetAcpuAck
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_ResetAcpuAck(void);

//--------------------------------------------------------------------------------------------------
/// KLadder_ReadDataFromAcpu
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_ReadDataFromAcpu(MS_U8* pu8Out);

//--------------------------------------------------------------------------------------------------
/// KLadder_Stop
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Stop(void); // write KTE

//--------------------------------------------------------------------------------------------------
/// KLadder_Lock
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Lock(void);

//--------------------------------------------------------------------------------------------------
/// KLadder_Unlock
/// @ingroup DSCMB_STB_Kladder
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_Unlock(void);

//--------------------------------------------------------------------------------------------------
/// Set the PID to be replaced
/// @ingroup DSCMB_STB_General
/// @param u32PidPrim     \b IN: The packet with this target PID(u32PidPrim) will be replaced in the field of PID by u32Pid2ndary
/// @param u32Pid2ndary  \b IN: The packet with this target PID(u32PidPrim) will be replaced in the field of PID by u32Pid2ndary
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_2ndarySet(MS_U32 u32PidPrim, MS_U32 u32Pid2ndary);
//--------------------------------------------------------------------------------------------------
/// Reset the PID to be replaced
/// @ingroup DSCMB_STB_General
/// @param u32PidPrim\b   IN: Reset the replacement of PID (u32PidPrim) set by MDrv_DSCMB_2ndarySet
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_2ndaryReset(MS_U32 u32PidPrim);
//--------------------------------------------------------------------------------------------------
/// The low-level implementation of @ref MDrv_DSCMB_FltTypeSet [RESERVED]
/// @ingroup DSCMB_STB_General
/// @param u32DscmbId \b IN: descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param eKeyType \b IN: key type for this setting
/// @param u32SW0 \b IN: The descramlber switch configuration
/// @param u32SW1 \b IN: reserved
/// @param u32SW2 \b IN: reserved
/// @return TRUE  - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB_SlotSwitchWrite(MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U32 u32SW0, MS_U32 u32SW1, MS_U32 u32SW2);

//--------------------------------------------------------------------------------------------------
/// Set record buffer
/// @ingroup DSCMB_STB_General
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_SetRecBuf(MS_U32 u32EngId , MS_U32 u32Start, MS_U32 u32Size);

//--------------------------------------------------------------------------------------------------
/// Record control
/// @ingroup DSCMB_STB_General
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_RecCtrl(MS_U32 u32EngId, MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Get record write pointer
/// @ingroup DSCMB_STB_General
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_GetRecWptr(MS_U32 u32EngId, MS_U32* pu32Write);

//-------------------------------------------------------------------------------------------------
/// Control DSCMB STR functions. (Already have another function: DSCMB2_SetPowerState)
/// @ingroup DSCMB_ToBeRemove
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32  MApi_DSCMB_SetPowerState(EN_POWER_MODE u16PowerState);


#ifdef __cplusplus
}
#endif

#endif
