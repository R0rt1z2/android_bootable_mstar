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
/// @brief  TCF Driver
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRV_TCF_H__
#define __DRV_TCF_H__

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define DRV_TCF_OK                   0x00000000
#define DRV_TCF_FAIL                 0x00000001
#define DRV_TCF_BUSY                 0x00000002
#define DRV_TCF_INVALID_INPUT        0x00000003


//--------------------------------------------------------------------------------------------------
//  Local enum
//--------------------------------------------------------------------------------------------------
/// CF
///
#define CF_NVM_RAW_WORDS                    (64)
#define CF_PERSIST_STATE_WORDS              (128)
#define CF_REG128_WORDS                     (4)
#define CF_REG64_WORDS                      (2)
#define CF_BITMAP_WORDS                     (8)
#define CF_CMD_WORDS                        (21)
#define CF_CWC_WORDS                        (4)
#define CF_PRODUCT_OR_ENTRIES               (8)

#define CF_OP_TYPE_MASK             0x0000E000  // Op[0][15:13]
#define CF_OP_TYPE_POS              13
typedef enum
{
    E_CF_OP_TYPE_OP_CWC                     = 0x0,
    E_CF_OP_TYPE_OP_PERSO                   = 0x1,
    E_CF_OP_TYPE_OP_DIFF                    = 0x2,
    E_CF_OP_TYPE_OP_FEATURE                 = 0x3,
    E_CF_OP_TYPE_OP_MFR_TEST                = 0x4
} CF_OPERATION_TYPE;

typedef enum
{
    E_CF_TRANS_STATUS_BUSY_INIT             = 0x0,    /**< Reset from power on condition. */
    E_CF_TRANS_STATUS_BUSY_SETUP            = 0x1,    /**< Reset after a command has completed. */
    E_CF_TRANS_STATUS_READY                 = 0x2,    /**< CF ready to receive a new transaction. */
    E_CF_TRANS_STATUS_INPUT                 = 0x3,    /**< CF is receiving a new transaction. */
    E_CF_TRANS_STATUS_BUSY_OP               = 0x4,    /**< CF is processing transaction. */
    E_CF_TRANS_STATUS_DONE_OK               = 0x8,    /**< Transaction was completed successfully. */
    E_CF_TRANS_STATUS_DONE_ERROR            = 0x9,    /**< Transaction has completed with an error. */
    E_CF_TRANS_STATUS_DONE_MFR_TEST         = 0xE     /**< Special case of successful test unlock. */
} CF_TRANS_STATUS_VALUES;

typedef enum
{
    E_CF_NVM_STATUS_READY                   = 0x0,    /**< NVM personalized and ready. */
    E_CF_NVM_STATUS_UNBISTED                = 0x1,    /**< NVM is unbisted.  Currently unsupported state and should be treated as an error. */
    E_CF_NVM_STATUS_UNPERSO                 = 0x2,    /**< NVM is pre-personalized. */
    E_CF_NVM_STATUS_ERROR                   = 0x3     /**< NVM personalized and ready. */
} CF_NVM_STATUS_VALUES;

typedef enum
{
    E_CF_DIFF_STATUS_UNDIFFERENTIATED       = 0x0,    /**< CF is undifferentiated. */
    E_CF_DIFF_STATUS_DIFFERENTIATED         = 0x1     /**< CF is differentiated. */
} CF_DIFF_STATUS_VALUES;

typedef enum
{
    E_CF_OPERATION_TYPE_OP_CWC              = 0x0,    /**< Request an CWC generation operation. */
    E_CF_OPERATION_TYPE_OP_PERSO            = 0x1,    /**< Request a Personalization operation. */
    E_CF_OPERATION_TYPE_OP_DIFF             = 0x2,    /**< Request a Differentiation operation. */
    E_CF_OPERATION_TYPE_OP_FEATURE          = 0x3,    /**< Request a Feature operation. */
    E_CF_OPERATION_TYPE_OP_MFR_TEST         = 0x4,    /**< Request a Manufacturer Test operation. */
    E_CF_OPERATION_TYPE_OP_UNDEFINED1       = 0x5,    /**< Reserved operation. */
    E_CF_OPERATION_TYPE_OP_UNDEFINED2       = 0x6,    /**< Reserved operation. */
    E_CF_OPERATION_TYPE_OP_UNDEFINED3       = 0x7     /**< Reserved operation. */
} CF_OPERATION_TYPE_VALUES;

typedef enum
{
    E_CF_DECM_SOURCE_NONE                   = 0x0,    /**< Invalid DECM source. Should not be used in normal operation. */
    E_CF_DECM_SOURCE_SW                     = 0x1,    /**< DECM source from software. */
    E_CF_DECM_SOURCE_KEY                    = 0x2,    /**< DECM source from key table. */
    E_CF_DECM_SOURCE_MIX                    = 0x3     /**< Combine DECM input from software and key table. */
} CF_DECM_SOURCE_VALUES;

typedef enum
{
    E_CF_OUTPUT_USAGE_SHV                   = 0x0,    /**< Output a secure hash value. */
    E_CF_OUTPUT_USAGE_CWC_DIRECT            = 0x1,    /**< Direct transfer of CWC to key table. */
    E_CF_OUTPUT_USAGE_CWC_XOR               = 0x2,    /**< XOR of key table entry with CWC. */
    E_CF_OUTPUT_USAGE_CWC_AES_KEY           = 0x3     /**< 128-bit AES decrypt of key table entry with CWC. */
} CF_OUTPUT_USAGE_VALUES;


/// CFB
///
typedef enum
{
    E_CFB_USE_CASE_3_1_1 = 0x00,  // 0
    E_CFB_USE_CASE_3_1_2,         // 1
    E_CFB_USE_CASE_3_2_1,         // 2
    E_CFB_USE_CASE_3_2_2,         // 3
    E_CFB_USE_CASE_3_2_3,         // 4
    E_CFB_USE_CASE_3_3_1,
    E_CFB_USE_CASE_3_3_2,
    E_CFB_USE_CASE_3_3_3,
    E_CFB_USE_CASE_3_3_4,
    E_CFB_USE_CASE_3_4_1,
    E_CFB_USE_CASE_3_4_2,
    E_CFB_USE_CASE_3_5_1,
    E_CFB_USE_CASE_3_5_2,
    E_CFB_USE_CASE_3_5_3,
    E_CFB_USE_CASE_3_5_4,
    E_CFB_USE_CASE_3_6,
    E_CFB_USE_CASE_KLGEN_CACWC,
} CFB_USE_CASE;

typedef enum
{
    E_CFB_NO_OTP_KEY = 0x00,  // 0
    E_CFB_OTP_KEY_1,          // 1
    E_CFB_OTP_KEY_2,          // 2
    E_CFB_OTP_KEY_3,          // 3
    E_CFB_OTP_KEY_4,
    E_CFB_OTP_KEY_5,
    E_CFB_OTP_KEY_6,
    E_CFB_OTP_KEY_7,
    E_CFB_OTP_KEY_8,
} CFB_OTP_KEY_IDX;

/// CFKE
///
#define CFKE_OP_CLRKE_CMD 	                (0x00005FFF)
#define CFKE_STATUS_DONE                    (0x8)
#define CFKE_INPUT_WORDS                    (4)

typedef enum  /* CFKE_STATUS [31:28] */
{
    E_CFKE_TRANS_STATUS_BUSY_INIT           = 0x0,    /**< Reset from power-on condition. */
    E_CFKE_TRANS_STATUS_READY               = 0x2,    /**< CFKE ready to receive a new operation request. */
    E_CFKE_TRANS_STATUS_BUSY_OP             = 0x4,    /**< CFKE is processing operation. */
    E_CFKE_TRANS_STATUS_DONE_OK             = 0x8,    /**< Operation completed successfully. CFKE is ready to receive another operation request. */
    E_CFKE_TRANS_STATUS_DONE_ERROR          = 0x9,    /**< Operation completed with an error. */
    E_CFKE_TRANS_STATUS_DONE_TEST           = 0xE,    /**< Special case of successful manufacturing test operation. */
} CFKE_TRANS_STATUS_VALUES;

typedef enum  /* CFKE_STATUS [19:16] */
{
    E_CFKE_ERROR_NO_ERROR                   = 0x0,
    E_CFKE_ERROR_INVALID_COMMAND            = 0x1,    /**< Indicates a malformed command. */
    E_CFKE_ERROR_CF                         = 0x2,    /**< Indicates that TCF output was requested when CWC was not available. */
    E_CFKE_ERROR_INVALID_TAGS               = 0x4,    /**< Indicates that the tags of the SRC1 and/or SRC2 were not valid for the given command. */
    E_CFKE_ERROR_COMMAND_FAIL               = 0x8,    /**< Indicates that command authorization failed, or that validator was incorrect for UNW operation, or that TEST command encountered an error. */
} CFKE_ERROR_CODE;

typedef enum  /* CFKE_STATUS [15:12] *//* CFKE_COMMAND [15:12] */
{
    E_CFKE_OP_DCF                           = 0x0,    /**< Request an Direct CF operation. */
    E_CFKE_OP_XOR                           = 0x1,    /**< Request a XOR operation. */
    E_CFKE_OP_AES                           = 0x2,    /**< Request a AES operation. */
    E_CFKE_OP_UNW                           = 0x3,    /**< Request a Unwrap operation. */
    E_CFKE_OP_HWDECM                        = 0x4,    /**< Request a HWDECM operation. */
    E_CFKE_OP_CLRKE                         = 0x5,    /**< Request a CFKE Clear operation. */
    E_CFKE_OP_TEST                          = 0xA,    /**< Request a test operation. */
    E_CFKE_OP_USER_HASH                     = 0xF,    /**< Write user hash data. */
} CFKE_OPERATION_TYPE, CFKE_CMD_OP;

typedef enum  /* CFKE_STATUS [3] */
{
    E_CFKE_UNW_REQUIRED_TRUE                = 0x0,    /**< The TCF CWC must be used for the SRC2 for CFKE_OP_UNW command. */
    E_CFKE_UNW_REQUIRED_FALSE               = 0x1,    /**< The TCF CWC may be used in any command. */
} CFKE_DIFF_ALLOW_NON_UNW;

typedef enum  /* CFKE_STATUS [2] */
{
    E_CFKE_CK_ALLOWED_FALSE                 = 0x0,    /**< The TCF CWC may be only used to derive a CW. */
    E_CFKE_CK_ALLOWED_TRUE                  = 0x1,    /**< The TCF CWC may be used to derive either a CW or CK. */
} CFKE_DIFF_ALLOW_CK;

typedef enum  /* CFKE_STATUS [1:0] */
{
    E_CFKE_CONTENT_OUTPUT_ALLOW_NONE        = 0x0,
    E_CFKE_CONTENT_OUTPUT_ALLOW_A           = 0x1,
    E_CFKE_CONTENT_OUTPUT_ALLOW_AB          = 0x2,
    E_CFKE_CONTENT_OUTPUT_ALLOW_ABC         = 0x3,
} CFKE_DIFF_CONTENT_OUTPUT_ALLOWED;

typedef enum  /* CFKE_COMMAND [11:8] */
{
    E_CFKE_DEST_CW                          = 0x0,    /**< Output should be placed on the cfCwOut output, to be used as the control word for the transport stream descrambler. */
    E_CFKE_DEST_CK                          = 0x1,    /**< Output should be placed on the cfCkOut output, to be used as the content key for the bulk descrambler. */
    E_CFKE_DEST_SLOTA                       = 0x4,    /**< Output should be written back into Key Slot A. */
    E_CFKE_DEST_SLOTB                       = 0x5,    /**< Output should be written back into Key Slot B. */
    E_CFKE_DEST_SLOTC                       = 0x6,    /**< Output should be written back into Key Slot C. */
    E_CFKE_DEST_SLOTD                       = 0x7,    /**< Output should be written back into Key Slot D. */
    E_CFKE_DEST_NULL                        = 0xF,    /**< No destination for this operation. */
    // Values 0x2-0x3 and 0x8-0xE are undefined and will cause an INVALID_COMMAND error if encountered.
} CFKE_CMD_DEST;

typedef enum  /* CFKE_COMMAND [7:4]: Src1 *//* CFKE_COMMAND [3:0]: Src2 */
{
    E_CFKE_SRC_NULL                         = 0x0,    /**< No SRC is used for the operation. */
    E_CFKE_SRC_CF                           = 0x1,    /**< Output of CF is used as SRC. */
    E_CFKE_SRC_TVEF                         = 0x2,    /**< eFuse input is used as SRC. */
    E_CFKE_SRC_SLOTA                        = 0x4,    /**< Existing value in Key Slot A is used as SRC. */
    E_CFKE_SRC_SLOTB                        = 0x5,    /**< Existing value in Key Slot B is used as SRC. */
    E_CFKE_SRC_SLOTC                        = 0x6,    /**< Existing value in Key Slot C is used as SRC. */
    E_CFKE_SRC_SLOTD                        = 0x7,    /**< Existing value in Key Slot D is used as SRC. */
    E_CFKE_SRC_SLOTAL                       = 0x8,    /**< Load a new value in Key Slot A which is used as SRC. */
    E_CFKE_SRC_SLOTBL                       = 0x9,    /**< Load a new value in Key Slot B which is used as SRC. */
    E_CFKE_SRC_SLOTCL                       = 0xA,    /**< Load a new value in Key Slot C which is used as SRC. */
    E_CFKE_SRC_SLOTDL                       = 0xB,    /**< Load a new value in Key Slot D which is used as SRC. */
} CFKE_CMD_SRC;


/// CRIKL
///
#define CRIKL_KL_ENG_NPD                    (0x80)
#define CRIKL_KL_ENG_MULTI2                 (0x40)
#define CRIKL_KL_ENG_CSAV2                  (0x20)
#define CRIKL_KL_ENG_CSAV3                  (0x10)
#define CRIKL_KL_ENG_ASA                    (0x08)
#define CRIKL_KL_ENG_DES                    (0x04)
#define CRIKL_KL_ENG_TDES                   (0x02)
#define CRIKL_KL_ENG_AES                    (0x01)

#define CRIKL_KL_DEST_AESDMA                (0x04)
#define CRIKL_KL_DEST_PVR                   (0x02)
#define CRIKL_KL_DEST_CIPHER                (0x01)


typedef enum
{
    E_CRIKL_SCMB_CTL_NONE                   = 0x0,
    E_CRIKL_SCMB_CTL_RESERVE                = 0x1,
    E_CRIKL_SCMB_CTL_EVEN_KEY               = 0x2,
    E_CRIKL_SCMB_CTL_ODD_KEY                = 0x3,
} CRIKL_SCRAMBLE_CONTROL;

typedef enum
{
    E_CRIKL_MIRROR_CF_STATUS                = 0x0,
    E_CRIKL_MIRROR_CFKE_STATUS              = 0x1,
    E_CRIKL_MIRROR_CF_PLATFORM              = 0x2,
    E_CRIKL_MIRROR_CF_FEATURE               = 0x3,
} CRIKL_MIRROR_MEM;

typedef enum
{
    E_CRIKL_KEY_SIZE_64                     = 0x0,
    E_CRIKL_KEY_SIZE_128                    = 0x1,
    E_CRIKL_KEY_SIZE_192                    = 0x2,
    E_CRIKL_KEY_SIZE_256                    = 0x3,
} CRIKL_KEY_SZIE;

typedef enum
{
    E_CRIKL_OP_DONT_CARE                    = 0x0,
    E_CRIKL_OP_DECRYPT                      = 0x1,
    E_CRIKL_OP_ENCRYPT                      = 0x2,
} CRIKL_OP_SEL;

typedef enum
{
    E_CRIKL_KTE0                            = 0x0,
    E_CRIKL_KTE1                            = 0x1,
    E_CRIKL_KTE2                            = 0x2,
    E_CRIKL_KTE3                            = 0x3,
} CRIKL_KTE_SEL;


//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
/// TCF DDI return value
typedef MS_U32                    DRV_TCF_RET;


/// CF
///
typedef struct      // MSTAR_CF_VERSION //
{
    MS_U8                         u8VersionEpoch;
    MS_U8                         u8ManufacturerId;
    MS_U8                         u8NetlistVersion;
    MS_U8                         u8VersionBuildId;
} DRV_CF_VERSION;

typedef struct      // MSTAR_CF_FEATURE //
{
    MS_U32                        u32FeatureVector;
} DRV_CF_FEATURE_VECTOR;

typedef struct      // MSTAR_CF_TRANS_STATUS //
{
    MS_U8                         eTransStatus;
    MS_U8                         u8UseNvmKey;
    MS_U8                         eOperationType;
    MS_U8                         eDecmSrc;
    MS_U8                         eOutputUsage;
    MS_U8                         u8ProductRange;
    MS_U8                         u8ProductOffset;
} DRV_CF_TRANS_STATUS;

typedef struct      // MSTAR_CF_CF_STATUS //
{
    MS_U8                         eNvmStatus;
    MS_U8                         eDiffStatus;
    MS_U8                         u8RecentReset;
    MS_U8                         u8CfAlert;
    MS_U8                         u8DevelopmentMode;
    MS_U8                         u8FuseActivate;
    MS_U8                         u8FuseBlock;
} DRV_CF_CF_STATUS;

typedef struct      // MSTAR_CF_DECM_STATUS //
{
    MS_U8                         u8HwDecmValid;
    MS_U8                         u8HwDecmError;
} DRV_CF_DECM_STATUS;

typedef struct      // MSTAR_CF_OPERATION //
{
    MS_U32                        Op[CF_CMD_WORDS];
} DRV_CF_OPERATION;

typedef struct      // MSTAR_CF_RESULT //
{
    MS_U8                         eOperationType;
    MS_U8                         eTransStatus;
    MS_U8                         u8CwcValid;
    MS_U32                        Shv[4];
} DRV_CF_RESULT;


/// CFB
///
typedef struct
{
    MS_U8                         u8CwcPid;
    MS_U8                         u8CwcFld;
    MS_U8                         u8CwcFscb;
    MS_U8                         u8CwcScb;
    MS_U8                         u8CwcTsid;
    MS_U8                         u8CwcDst;
} DRV_CFB_CWCCFG;

typedef struct
{
    MS_U8                         u8UseCase;
    MS_U8                         u8OtpKeyIdx;
    MS_U8                         u8CAcwcSrc;
    MS_U32                        CACWC[4];
    MS_U32                        EPK[4];
    MS_U32                        EFUV[4];

    DRV_CFB_CWCCFG                stCwcCfg;
} DRV_CFB_CFBCFG;

/// CFKE
///
typedef struct
{
	CFKE_CMD_SRC                  eSrcType;               /**< Type of input to be written. */
	MS_U32                        pu32KeySlotData[4];     /**< Write the 128-bit Key Slot value. */
} DRV_CFKE_SOURCE;

typedef struct
{
	MS_U32                        u32UnwPathData;         /**< Write the 32-bit Unwrap path value. */
	MS_U32                        pu832ValidatorData[2];  /**< Write the 64 bit Validator value. */
} DRV_CFKE_UNWRAP_DATA;

typedef struct
{
	MS_U32                        pu32UsrHashData[4];     /**< Write the 128 bit User Hash value. */
} DRV_CFKE_USER_HASH_DATA;

typedef struct
{
    CFKE_OPERATION_TYPE           eOpType;                /**< Type of operation to be issued. */
    DRV_CFKE_SOURCE               eOpSrc1;                /**< Source 1 for the operation (if needed). */
    DRV_CFKE_SOURCE               eOpSrc2;                /**< Source 2 for the operation (if needed). */
    CFKE_CMD_DEST                 eOpDest;                /**< Destination for the operation (if needed). */
    DRV_CFKE_UNWRAP_DATA          UnwData;                /**< Unwrap data for the \link #CFKE_OP_UNW \endlink operation. */
} DRV_CFKE_OPERATION;


/// CRIKL
///
typedef struct
{
    /// Key Index
    MS_BOOL                       bDscmbKey;              // 1: Dscmb Key, 0: AESDMA Key
    // kt_index to KeyTable-- DSCMB
    CRIKL_SCRAMBLE_CONTROL        eScmbCtl;               // for Dscmb Key
    MS_U8                         u8DscmbId;              // for Dscmb Key
    // ks_index to KeySlot-- AESDMA
    MS_U8                         u8KeySlotIdx;           // for Aesdma Key

    /// High HalfWord [63:48]
    MS_U8                         u8VendorNo;             // [63:60]
    MS_U16                        u16VendorAtt;           // [59:46]

    /// 2nd HalfWord [47:32]
    MS_U8                         u8OutputAllowed;        // [45:44]
    MS_BOOL                       bMultiEng;              // [37]
    MS_BOOL                       bLsb64;                 // [36]
    CRIKL_KEY_SZIE                eKeySize;               // [35:32]

    /// 3rd HalfWord [31:16]
    MS_U8                         u8KlEng;                // [31:24]
    MS_U8                         u8Drop;                 // [23:20]
    MS_U8                         u8Cnt;                  // [19:16]

    /// Low HalfWord [15:0]
    MS_U8                         u8KlDest;               // [12:10]
    CRIKL_OP_SEL                  eOpSel;                 // [09:08]
    CRIKL_KTE_SEL                 eKteSel;                // [07:04]
    MS_BOOL                       bLock;                  // [03]
    MS_BOOL                       bParaVld;               // [02]
    MS_BOOL                       bKlEn;                  // [00]
} DRV_CRIKL_KEY_CONFIG;


//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
/// CF APIs
///
DLL_PUBLIC DRV_TCF_RET MDrv_TCF_Init(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Version_Info(DRV_CF_VERSION *pstVersion);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Trans_Status(DRV_CF_TRANS_STATUS *pstTransStatus);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Cf_Status(DRV_CF_CF_STATUS *pstCfStatus);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_FeatureVector(DRV_CF_FEATURE_VECTOR *pstCfFeature);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Issue_Op(DRV_CF_OPERATION *pCfOperations);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_IsFinished(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Read_Op_Result(DRV_CF_RESULT *pCfResult);

/// CFB APIs
///
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Init(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Enable(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Reset(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Setup(DRV_CFB_CFBCFG *pstCFBCfg);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Is_Ready(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Is_Done(void);

/// CFB DBG APIs
DLL_PUBLIC void MDrv_CFB_DBG_KT_Response(void);
DLL_PUBLIC MS_U8 MDrv_CFB_DBG_CFB_FSM(void);

/// CFKE APIs
///
DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_Cmd_Exe(DRV_CFKE_OPERATION *pCfkeOp);
DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_IsDone(CFKE_ERROR_CODE *eCfkeErr);
DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_IsReady(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_UserHash_Setup(DRV_CFKE_USER_HASH_DATA *pUserHash);

/// CRIKL APIs
DLL_PUBLIC MS_U32 MDrv_CRIKL_Mirror(CRIKL_MIRROR_MEM eCandidate);
DLL_PUBLIC DRV_TCF_RET MDrv_CRIKL_Set_KeyConfig(DRV_CRIKL_KEY_CONFIG *stKeyCfg);

#endif /* __DRV_TCF_H__ */
