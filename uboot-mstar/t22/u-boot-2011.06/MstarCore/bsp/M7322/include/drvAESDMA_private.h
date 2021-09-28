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
#ifndef _DRVAESDMA_PRIV_H_
#define _DRVAESDMA_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvAESDMA.h"
#include "drvCIPHER.h"
#include "regAESDMA.h"

#include "utopia_dapi.h"

typedef MS_U32 (*IOCTL_AESDMA_INIT)(MS_PHY, MS_PHY, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETIV)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_RESET)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETFILEINOUT)(MS_PHY, MS_U32, MS_PHY, MS_PHY);
typedef MS_U32 (*IOCTL_AESDMA_SETKEY)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_SELENG)(DrvAESDMA_CipherMode, MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_START)(MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_GETSTATUS)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_ISFINISHED)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_SETCLK)(MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_SETSECUREKEY)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETPS)(MS_U32, MS_U32, MS_BOOL, MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_PSRELEASE)(void);
typedef MS_U32 (*IOCTL_AESDMA_GETPSMATCHEDBYTECNT)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_GETPSMATCHEDPTN)(void);
typedef MS_U32 (*IOCTL_AESDMA_NOTIFY)(DrvAESDMA_Event, P_DrvAESDMA_EvtCallback);
typedef MS_U32 (*IOCTL_AESDMA_RAND)(MS_U32 *, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_GET_RAND_NUM)(MS_U8 *);
typedef MS_U32 (*IOCTL_RSA_CALCULATE)(DrvAESDMA_RSASig *, DrvAESDMA_RSAKey *, DrvAESDMA_RSAMode);
typedef MS_U32 (*IOCTL_RSA_ISFINISHED)(void);
typedef MS_U32 (*IOCTL_RSA_OUTPUT)(DrvAESDMA_RSAMode, DrvAESDMA_RSAOut *);
typedef MS_U32 (*IOCTL_SHA_CALCULATE)(DrvAESDMA_SHAMode, MS_PHY, MS_U32, MS_PHY);
typedef MS_U32 (*IOCTL_SHA_CALCULATE_MANUAL)(DrvAESDMA_HASHCFG, DrvAESDMA_HASH_STAGE, MS_U32, MS_U8 *);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_MASKSCRMB)(MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_SETSCRMBPATTERN)(DrvAESDMA_ScrmbPattern);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_SETADDEDSCRMBPATTERN)(DrvAESDMA_ScrmbPattern);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_BYPASSPID)(MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_SETPID)(MS_U8, MS_U16);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_ENCRYPT)(DrvAESDMA_ParserMode);
typedef MS_U32 (*IOCTL_AESDMA_PARSER_DECRYPT)(DrvAESDMA_ParserMode);
typedef MS_U8  (*IOCTL_AESDMA_QUERYPIDCOUNT)(MS_U8 *);
typedef MS_U32 (*IOCTL_AESDMA_SETKEYSEL)(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType);
typedef MS_U32 (*IOCTL_AESDMA_SETODDIV)(MS_U32 *pInitVector);
typedef MS_U32 (*IOCTL_IS_SECRETKEYINNORMALBLANK)(MS_U8 *);

typedef MS_U32 (*IOCTL_HDCP_PROCESSCIPHER)(MS_U8 , MS_U8*, MS_U8*);

typedef MS_U32 (*IOCTL_HDCP_GETHDCPCIPHERSTATE)(MS_U8 , MS_U8*);

typedef MS_U32 (*IOCTL_AESDMA_SETKEYINDEX)(MS_U32, MS_BOOL);

typedef MS_U32 (*IOCTL_AESDMA_SETAESCTR64)(MS_BOOL);

typedef MS_U32 (*IOCTL_AESDMA_SETSECUREKEYINDEX)(MS_U8);

typedef MS_U32 (*IOCTL_CIPHER_DMACALC)(DRV_CIPHER_DMACFG, MS_U32 *);
typedef MS_BOOL (*IOCTL_CIPHER_ISDMADONE)(MS_U32, MS_U32*);
typedef MS_U32 (*IOCTL_CIPHER_DMACALCLIST)(DRV_CIPHER_DMACFG*, MS_U32, MS_U32*, MS_PHY, MS_PHY, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETPOWERSTATE)(EN_POWER_MODE);
typedef MS_U32 (*IOCTL_AESDMA_QUERYCIPHERMODE)(DrvAESDMA_CipherMode);
typedef MS_U32 (*IOCTL_CIPHER_GCM_GETTAG)(MS_U8*, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETKEY_EX)(const MS_U8 *, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETIV_EX)(const MS_U8 *, MS_U32);


typedef struct _AESDMA_RESOURCE_PRIVATE
{
    MS_U32 AES_Dummy;
}AESDMA_RESOURCE_PRIVATE;

typedef struct _SHA_RESOURCE_PRIVATE
{
    MS_U32 SHA_Dummy;
}SHA_RESOURCE_PRIVATE;

typedef struct _RSA_RESOURCE_PRIVATE
{
    MS_U32 RSA_Dummy;
}RSA_RESOURCE_PRIVATE;

typedef struct _AESDMA_RESOURCE_SHARED
{
    void* pAESResource;
}AESDMA_RESOURCE_SHARED;

typedef struct _RSA_RESOURCE_SHARED
{
    void* pRSAResource;
}RSA_RESOURCE_SHARED;

typedef struct _AESDMA_INSTANT_PRIVATE
{
    MS_U8                              InitVector[16];
    MS_U32                             u32IVLen;
    MS_PHY                             u32FileinAddr;
    MS_U32                             u32FileInNum;
    MS_PHY                             u32FileOutSAddr;
    MS_PHY                             u32FileOutEAddr;
    MS_U8                              CipherKey[16];
    MS_U32                             u32KeyLen;
    DrvAESDMA_CipherMode               eMode;
    MS_BOOL                            bDescrypt;
    MS_BOOL                            bSecretKey;
    IOCTL_AESDMA_INIT                  fpAESDMAInit;
    IOCTL_AESDMA_SETIV                 fpAESDMASetIV;
    IOCTL_AESDMA_RESET                 fpAESDMAReset;
    IOCTL_AESDMA_SETFILEINOUT          fpAESDMASetFileInOut;
    IOCTL_AESDMA_SETKEY                fpAESDMASetKey;
    IOCTL_AESDMA_SELENG                fpAESDMASelEng;
    IOCTL_AESDMA_START                 fpAESDMAStart;
    IOCTL_AESDMA_GETSTATUS             fpAESDMAGetStatus;
    IOCTL_AESDMA_ISFINISHED            fpAESDMAIsFinished;
    IOCTL_AESDMA_SETCLK                fpAESDMASetClk;
    IOCTL_AESDMA_SETSECUREKEY          fpAESDMASetSecureKey;
    IOCTL_AESDMA_SETPS                 fpAESDMASetPS;
    IOCTL_AESDMA_PSRELEASE             fpAESDMAPSRelease;
    IOCTL_AESDMA_GETPSMATCHEDBYTECNT   fpAESDMAGetPSMatchedByteCNT;
    IOCTL_AESDMA_GETPSMATCHEDPTN       fpAESDMAGetPSMatchedPTN;
    IOCTL_AESDMA_NOTIFY                fpAESDMANotify;
    IOCTL_AESDMA_RAND                  fpAESDMARand;
    IOCTL_AESDMA_GET_RAND_NUM          fpAESDMAGetRandNum;
    IOCTL_RSA_CALCULATE                fpRSACalculate;
    IOCTL_RSA_ISFINISHED               fpRSAIsFinished;
    IOCTL_RSA_OUTPUT                   fpRSAOutput;
    IOCTL_SHA_CALCULATE                fpSHACalculate;
    IOCTL_SHA_CALCULATE_MANUAL         fpSHACalculateManual;

    DrvAESDMA_ScrmbPattern             eScrmbPattern;
    DrvAESDMA_ScrmbPattern             eAddedScrmbPattern;
    DrvAESDMA_ParserMode               eParserMode;
    MS_BOOL                            bParserDecrypt;
    MS_BOOL                            bMaskScrmb;
    MS_BOOL                            bBypassPid;
    MS_U8                              u8PidIndex;    //1st PID, 0x122C_01
    MS_U32                             u16Pid;        //1st PID, 0x122C_01

    IOCTL_AESDMA_PARSER_MASKSCRMB       fpAESDMAParserMaskscrmb;
    IOCTL_AESDMA_PARSER_SETSCRMBPATTERN fpAESDMAParserSetScrmbPatten;
    IOCTL_AESDMA_PARSER_BYPASSPID       fpAESDMAParserBypassPid;
    IOCTL_AESDMA_PARSER_SETPID          fpAESDMAParserSetPid;
    IOCTL_AESDMA_PARSER_ENCRYPT         fpAESDMAParserEncrypt;
    IOCTL_AESDMA_PARSER_DECRYPT         fpAESDMAParserDecrypt;
    IOCTL_AESDMA_QUERYPIDCOUNT          fpAESDMAParserQueryPidCount;
    IOCTL_AESDMA_PARSER_SETADDEDSCRMBPATTERN fpAESDMAParserSetAddedScrmbPattern;
    MS_BOOL                            bEnableTwoKey;
    MS_U8                              InitVectorOdd[16];
    MS_U8                              CipherOddKey[16];
    IOCTL_AESDMA_SETKEYSEL             fpAESDMASetKeySel;
    IOCTL_AESDMA_SETODDIV              fpAESDMASetOddIv;
    MS_U8                              u8PidIndex1;    //2nd PID, 0x122C_02
    MS_U32                             u16Pid1;        //2nd PID, 0x122C_02
    IOCTL_IS_SECRETKEYINNORMALBLANK    fpAESDMAIsSecretKeyInNormalBlank;
    IOCTL_HDCP_PROCESSCIPHER           fpHDCPHDCPProcessCipher;

    IOCTL_AESDMA_RESET                 fpAESDMANormalReset;
    MS_BOOL                            bEvenScrmbPatternEn;
    MS_BOOL                            bOddScrmbPatternEn;
    IOCTL_HDCP_GETHDCPCIPHERSTATE      fpGetHDCPCipherState;
    MS_BOOL                            bSetEng;
    MS_BOOL                            bSetKey;
    MS_BOOL                            bSetIV;
    MS_BOOL                            bHwParserEn;
    MS_BOOL                            bPid0En;
    MS_BOOL                            bPid1En;
    MS_BOOL                            bMaskScrmbEn;

    IOCTL_AESDMA_SETKEYINDEX           fpAESDMAKeyIndex;
    MS_U32                             u32KeyIndexEven;
    MS_U32                             u32KeyIndexOdd;
    MS_BOOL                            bEnableKeyIndexEvenKey;
    MS_BOOL                            bEnableKeyIndexOddKey;
    IOCTL_AESDMA_SETAESCTR64           fpAesCtr64;
    MS_BOOL                            bEnableAesCtr64;

    IOCTL_AESDMA_SETSECUREKEYINDEX     fpAESDMASecureKeyIdx;
    MS_U8                              u8SecureKeyIndex;

    MS_PHY                             u32miu0addr;
    MS_PHY                             u32miu1addr;
    MS_U32                             u32miunum;
    IOCTL_AESDMA_SETPOWERSTATE         fpAESDMASetPowerState;
    IOCTL_AESDMA_QUERYCIPHERMODE       fpAESDMAQueryCipherMode;

    IOCTL_CIPHER_GCM_GETTAG            fpCIPHERGCMGetTag;
    IOCTL_CIPHER_DMACALC               fpCIPHERDMACalc;
    IOCTL_CIPHER_ISDMADONE             fpCIPHERIsDMADone;
#if AESDMS_SECURED_BANK_EN
    IOCTL_CIPHER_DMACALCLIST           fpCIPHERDMACalcList;
#endif
    IOCTL_AESDMA_SETKEY_EX             fpAESDMASetKey_Ex;
    IOCTL_AESDMA_SETIV_EX              fpAESDMASetIV_Ex;
}AESDMA_INSTANT_PRIVATE;

#define SHA_DATA_MINIMUM_SIZE              64     // 512 bits
#define SHA_DATA_PADDING_RESERVE           8      // 64 bits length for total data bits
#define SHA_DATA_PADDING_MINIMUM_SIZE      (SHA_DATA_PADDING_RESERVE + 1)
#define SHA_DATA_SIZE_ALIGNMENT            (SHA_DATA_MINIMUM_SIZE - 1)

typedef struct _SEPARATE_SHA_INFO
{
    MS_U16  u16LastSha[16];
    MS_U32  u32TotalSize;
    MS_PHY  u32WorkingBufAddr;
    MS_U32  u32WorkingBufSize;
    MS_PHY  u32WorkingBufAddrCurrent;
    MS_BOOL bFirstStageSha;
}SEPARATE_SHA_INFO;

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 AESDMAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 AESDMAClose(void* pInstantTmp);
MS_U32 AESDMAIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
MS_U32 AESDMAStr(MS_U32 u32PowerState, void* pModule);

DRVAESDMA_RESULT _MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey);
DRVAESDMA_RESULT _MDrv_AESDMA_SetAesCtr64 (MS_BOOL bCtr64En);
DRVAESDMA_RESULT _MDrv_AESDMA_SetSecuredKeyIndex (MS_U8 u8KeyIdx);
MS_U32 _MDrv_AESDMA_SetPowerState(EN_POWER_MODE u16PowerState);
DRVAESDMA_RESULT _MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len);
DRVAESDMA_RESULT _MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len);
DRV_CIPHER_RET _MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId);
MS_U32 _MDrv_CIPHER_GCM_GetTag(MS_U8 *pu8OutputBuff, MS_U32 u32OutputBuffSize);

DLL_PUBLIC DRV_CIPHER_RET _MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC MS_BOOL _MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception);
#if AESDMS_SECURED_BANK_EN
DLL_PUBLIC DRV_CIPHER_RET _MDrv_CIPHER_DMACalcList(DRV_CIPHER_DMACFG* pstCfg, MS_U32 u32NumOfCmds, MS_U32 *pu32CmdId, MS_PHY BufferAddr, MS_PHY BufferLen,  MS_U32 CMDQ_MIU_SELECT);
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
typedef enum
{
    AESDMA_MDBCMDLINE_NONE,
    AESDMA_MDBCMDLINE_HELP,
    AESDMA_MDBCMDLINE_AESDMA,
    AESDMA_MDBCMDLINE_SHA,
    AESDMA_MDBCMDLINE_RSA,
    AESDMA_MDBCMDLINE_PERFORMANCE,
    AESDMA_MDBCMDLINE_DBGLEVEL,
}EN_AESDMA_MDBCMDLINE;

typedef struct DLL_PACKED
{
    MS_U32 DBG_AESDMA_Algorithm;
    MS_PHY DBG_AESDMA_InputAddr;
    MS_PHY DBG_AESDMA_OutputAddr;
    MS_PHY DBG_AESDMA_EnOutputAddr;
    MS_U32 DBG_AESDMA_DataSize;
    MS_U32 DBG_AESDMA_Location[2];
    MS_U32 DBG_AESDMA_KeySelect;
    MS_U32 DBG_AESDMA_Key[4];
    MS_U32 DBG_AESDMA_IV[4];
}DrvAESDMA_MDB_AESDMA, *PDrvAESDMA_MDB_AESDMA;

typedef struct DLL_PACKED
{
    MS_U32 DBG_SHA_Algorithm;
    MS_U32 DBG_SHA_BypassMode;
    MS_PHY DBG_SHA_InputAddr;
    MS_U32 DBG_SHA_Length;
    MS_PHY DBG_SHA_InputLocation;
    MS_U32 DBG_SHA_Status;
    MS_U32 DBG_SHA_Out[8];
}DrvAESDMA_MDB_SHA, *PDrvAESDMA_MDB_SHA;

typedef struct DLL_PACKED
{
    MS_U32 DBG_RSA_Mode;
    MS_U32 DBG_RSA_AutoIncre;
    MS_U32 DBG_RSA_AutoStart;
    MS_U32 DBG_RSA_InputAddr;
    MS_U32 DBG_RSA_WriteData[2];
    MS_U32 DBG_RSA_ReadData[2];
    MS_U32 DBG_RSA_Status;
    MS_U32 DBG_RSA_KeySelect;
    MS_U32 DBG_RSA_KeyLength;
}DrvAESDMA_MDB_RSA, *PDrvAESDMA_MDB_RSA;

typedef struct DLL_PACKED
{
    MS_U32 u32Total;
    MS_U32 u32Adverage;
    MS_U32 u32Max;
    MS_U32 u32Min;
}DrvAESDMA_MDB_DATA, *PDrvAESDMA_MDB_DATA;

typedef struct DLL_PACKED
{
    MS_U64 u64PerformanceRate;
    MS_U64 u64TotalByte;
    MS_U64 u64TotalByte_Overflow;
    MS_U32 u32Rounds;
    MS_U32 u32PreDoneTime;
    DrvAESDMA_MDB_DATA stIdleTime;
    DrvAESDMA_MDB_DATA stDataSize;
    DrvAESDMA_MDB_DATA stWorkingTime;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    struct timespec st_start_time;
    struct timespec st_done_time;
#endif
    MS_U32 start_time;
    MS_U32 done_time;
}DrvAESDMA_MDB_AES_PERFORMANCE, *PDrvAESDMA_MDB_AES_PERFORMANCE;

MS_BOOL MDrv_AESDMA_MdbGetInfo(MDBCMD_GETINFO_PARAMETER *paraGetInfo);
MS_BOOL MDrv_AESDMA_MdbCmdLine(MDBCMD_CMDLINE_PARAMETER *paraCmdLine, EN_AESDMA_MDBCMDLINE eMdbCmdLine);
#endif

#ifdef __cplusplus
}
#endif
#endif // _DRVAESDMA_PRIV_H_
