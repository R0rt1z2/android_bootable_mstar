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
/// @brief  CA Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CA_H__
#define __DRV_CA_H__

#include "MsTypes.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_CA_LIB_CODE              {'C','A', '_', '_'}               //Lib code
#define MSIF_CA_LIBVER                {'0','1'}                         //LIB version
#define MSIF_CA_BUILDNUM              {'1','1'}                         //Build Number
#define MSIF_CA_CHANGELIST            {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define CA_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                        /* 'MSIF'                                           */  \
    MSIF_CLASS,                      /* '00'                                             */  \
    MSIF_CUS,                        /* 0x0000                                           */  \
    MSIF_MOD,                        /* 0x0000                                           */  \
    MSIF_CHIP,                                                                               \
    MSIF_CPU,                                                                                \
    MSIF_CA_LIB_CODE,                /* IP__                                             */  \
    MSIF_CA_LIBVER,                  /* 0.0 ~ Z.Z                                        */  \
    MSIF_CA_BUILDNUM,                /* 00 ~ 99                                          */  \
    MSIF_CA_CHANGELIST,              /* CL#                                              */  \
    MSIF_OS


// CA_TA_TDC_CFG version of current CA library
#define CA_TA_TDC_CFG_VERSION    1


typedef enum
{
    E_CA_DEBUG_PORT_I2C,
    E_CA_DEBUG_PORT_JTAG,
    E_CA_DEBUG_PORT_SCAN,
} CA_DEBUG_PORT;

typedef enum
{
    E_CA_DEBUG_PORT_OPEN,
    E_CA_DEBUG_PORT_PASSWD,
    E_CA_DEBUG_PORT_CLOSE
} CA_DEBUG_PORT_MODE;

typedef enum
{
    E_CA_LOCK_WRITE = 1 << 0,
    E_CA_LOCK_READ = 1 << 1
} CA_LOCK_TYPE;

typedef enum
{
    E_BOOT_LockWord_W0000_0511_Addr,
    E_BOOT_LockWord_W0512_1023_Addr,
    E_BOOT_LockWord_W1024_1535_Addr,
    E_BOOT_LockWord_W1536_2047_Addr,
    E_BOOT_LockWord_W2048_2559_Addr,
    E_BOOT_LockWord_W2560_3071_Addr,
    E_BOOT_LockWord_W3072_3583_Addr,
    E_BOOT_LockWord_W3584_3759_Addr,
    E_LockWord_v_PubOtpOID,
    E_LockWord_v_PubOtpVID,
    E_LockWord_v_pubOtpUniqueID1,
    E_LockWord_v_pubOtpUniqueID2,
    E_LockWord_CA_Reserved,
    E_LockWord_CA_MSID,

    E_BOOT_LockWord_W3072_3391_Addr,
    E_LockWord_v_pubOtpUniqueID3,
    E_LockWord_v_pubOtpUniqueID4,

    //SecretKey lock WRITE
    E_LockWord_CAV_SecretKey1_W = 0x101,
    E_LockWord_CAV_SecretKey2_W = 0x102,
    E_LockWord_CAV_SecretKey3_W = 0x103,
    E_LockWord_CAV_SecretKey4_W = 0x104,
    E_LockWord_CAV_SecretKey5_W = 0x105,
    E_LockWord_CAV_SecretKey6_W = 0x106,
    E_LockWord_CAV_SecretKey7_W = 0x107,
    E_LockWord_CAV_SecretKey8_W = 0x108,

    //SecretKey lock READ
    E_LockWord_CAV_SecretKey1_R = 0x111,
    E_LockWord_CAV_SecretKey2_R = 0x112,
    E_LockWord_CAV_SecretKey3_R = 0x113,
    E_LockWord_CAV_SecretKey4_R = 0x114,
    E_LockWord_CAV_SecretKey5_R = 0x115,
    E_LockWord_CAV_SecretKey6_R = 0x116,
    E_LockWord_CAV_SecretKey7_R = 0x117,
    E_LockWord_CAV_SecretKey8_R = 0x118,

    //RSA Key lock WRITE
    E_LockWord_RSA_1_W=0x201,

    //Debug ports lock WRITE
    E_LockWord_Debugport_PWD_W = 0x301,
    //Debug ports lock READ
    E_LockWord_Debugport_PWD_R = 0x311,

    //Reserved Field lock
    E_LockWord_CA_reserved0 = 0x401,
    E_LockWord_CA_reserved1 = 0x402,
    E_LockWord_CA_reserved2 = 0x403,
    E_LockWord_CA_reserved3 = 0x404,
} CA_OTP_LOCK;

// OTP function configuration
typedef enum
{
    E_CA_OTP_FUNCTION_ACPU_WRITE_OTP   = 0x2000,
    E_CA_OTP_FUNCTION_ACPU2KT          = 0x2010,
    E_CA_OTP_FUNCTION_ACPU2DMA         = 0x2014,
    E_CA_OTP_FUNCTION_SECURE_BOOT      = 0x2040,
    E_CA_OTP_FUNCTION_TEE              = 0x2041,
    E_CA_OTP_FUNCTION_EX_CERT          = 0x2050,
    E_CA_OTP_FUNCTION_FORCE_ENC_LOADER = 0x2060,
    E_CA_OTP_FUNCTION_FORCE_ENC_IMAGE  = 0x2061,
} CA_OTP_FUNCATION;

// OTP function configuration
typedef enum
{
    E_CA_OTP_CONFIG_DISABLE = 0,
    E_CA_OTP_CONFIG_ENABLE  = 1,
} CA_OTP_CONFIG_TYPE;

/*
    CA Vendor code rule
    First 8 bits: code by initial of CA Vendor
    "A": 01, "B": 02, "C": 03, "D": 04, ... , "Y": 19, "Z": 1A
    Second 4 bits: Increase if the initial of CA Vendor repeat
    Thrid 4 bits: Serial number of CA Vendor

    0x0000 reserve for NULL
    0xFFFF for unknow CA
  */
typedef enum
{
    E_CA_VENDOR_NULL,
    E_CA_VENDOR_CONAX       = 0x0300,
    E_CA_VENDOR_ECHOSTAR    = 0x0500,
    E_CA_VENDOR_IRDETO      = 0x0900,
    E_CA_VENDOR_LATENS      = 0x0C00,
    E_CA_VENDOR_NDS         = 0x0E00,
    E_CA_VENDOR_NAGRA       = 0x0E10,
    E_CA_VENDOR_SMI         = 0x1300,
    E_CA_VENDOR_VIACCESS    = 0x1600,
    E_CA_VENDOR_VERIMATRIX  = 0x1610,
    E_CA_VENDOR_UNKNOW      = 0xFFFF
} CA_VENDOR_CODE;

typedef struct
{
    MS_U32 u32FwAddr;
    MS_U32 u32FwSize;
} CA_BGC_CFG, CA_TA_CFG;

typedef struct
{
    MS_U32 u32BGCMode;        //0:dynamic, 1:static
    MS_U32 u32ProtectAddr;    //protected address, physicall address
    MS_U32 u32ProtectSize;    //protected size
    MS_U8  u8Golden[32];      //password is used for reset section
    MS_U8  u8ResetPassword[4];//only use for dynamic mode
    MS_U16 u16Interval;       //unit: millisecond, the frequency to check the proteced area
} CA_BGC_SEC_CFG;

typedef struct
{
    // Version of current structure. Must set to "CA_TA_TDC_CFG_VERSION"
    MS_U16   u16TA_TDC_CFG_Version;
    // Length of this structure. Must set to "sizeof(CA_TA_TDC_CFG)"
    MS_U16   u16TA_TDC_CFG_Length;

    // Address of TASK encrypted Transformation Data Container (TDC)
    MS_U8   *pu8EncTDC;
    // The size of encrypted TDC in Bytes
    MS_U32  u32EncTDCSize;
    // Address of TAUK encrypted TASK or further keys, the size of each key is 128-bit
    MS_U8   *pu8EncTASK;
    // The KeyLadder level to decrypt EncTASK
    MS_U32  u32Level;
} CA_TA_TDC_CFG;

typedef enum
{
    E_CA_SECRET_KEY1,
    E_CA_SECRET_KEY2,
    E_CA_SECRET_KEY3,
    E_CA_SECRET_KEY4,
    E_CA_SECRET_KEY5,
    E_CA_SECRET_KEY6,
    E_CA_SECRET_KEY7,
    E_CA_SECRET_KEY8,
} CA_SECRET_KEY;

typedef enum
{
    E_CA_RSA_KEY0,
    E_CA_RSA_KEY1,
} CA_RSA_KEY;

typedef enum
{
    E_CA_CUSTOM_SECURE_MAGIC,
    E_CA_CUSTOM_RPMB_KEY_INIT_STATUS,
    E_CA_CUSTOM_PROJECT_VERSION,
} CA_CUSTOM_DATA;

typedef enum
{
    E_CA_SECURE_SEC51_OFF,        // disable SEC51

} CA_SECURE_ITEM;

typedef enum
{
    E_CA_BOOTDEVICE_HWSTRAPPING,
    E_CA_BOOTDEVICE_SPI,
    E_CA_BOOTDEVICE_NAND,
    E_CA_BOOTDEVICE_EMMC,
} CA_BOOT_DEVICE;

DLL_PUBLIC MS_BOOL MDrv_CA_Init(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxDeviceIdSize(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxReservedSize(void);

//-------------------------------------------------------------------------------------------------
// The functions will read/write directly from/to OTP
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *eLockType);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetRSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetTEERSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetTEERSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_TEE_REF_VER(MS_U32 u32HASH_TEE0_REF_VER, MS_U32 u32HASH_TEE1_REF_VER, MS_U32 u32HASH_TEE2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_TEE_REF_VER(MS_U32 *pu32HASH_TEE0_REF_VER, MS_U32 *pu32HASH_TEE1_REF_VER, MS_U32 *pu32HASH_TEE2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_WriteReserved(const MS_U8 *pu8RData, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_ReadReserved(MS_U8 *pu8RData, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureCWMode(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureCWMode(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_DumpKeyProperty(MS_U32 u32KeyNumber);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetSecretKey(CA_SECRET_KEY eKeySel, const MS_U8 *pu8SecretKey, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetSecretKey(CA_SECRET_KEY eKeySel, MS_U8 *pu8SecretKey, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAKey(CA_RSA_KEY eKeySel, const MS_U8 *pu8RSAKey, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetRSAKey(CA_RSA_KEY eKeySel, MS_U8 *pu8RSAKey, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortPassword(CA_DEBUG_PORT eDebugPort, const MS_U8 *pu8Password, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortPassword(CA_DEBUG_PORT eDebugPort, MS_U8 *pu8Password, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetBootDevice(CA_BOOT_DEVICE eBootDevice);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetBootDevice(CA_BOOT_DEVICE *peBootDevice);

//-------------------------------------------------------------------------------------------------
// The OTP read/write
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsBlank(MS_U32 addr);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_Read(MS_U32 addr);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value);

DLL_PUBLIC MS_BOOL MDrv_CA_Locked(CA_OTP_LOCK eLock);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock);

//-------------------------------------------------------------------------------------------------
// The OTP function config
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetFunctionConfig(CA_OTP_FUNCATION eFunction, CA_OTP_CONFIG_TYPE eActive);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetFunctionConfig(CA_OTP_FUNCATION eFunction, CA_OTP_CONFIG_TYPE *peActive, MS_BOOL *pbDefault);

//-------------------------------------------------------------------------------------------------
// Hardware 16-bits Random number generator
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MDrv_CA_Random(void);

//-------------------------------------------------------------------------------------------------
// The CA Vendor
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MDrv_CA_OTP_ReadCAVendor(void);


//-------------------------------------------------------------------------------------------------
// The CA MBX between R2 and ACPU
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_MBX_Init(void);
DLL_PUBLIC MS_U32  MDrv_CA_MBX_Write(MS_U8* u8Addr, MS_U32 u32Size);
DLL_PUBLIC MS_U32  MDrv_CA_MBX_Read(MS_U8* u8Addr, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
// The CA STR PM Config
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_STR_Init(void);
DLL_PUBLIC MS_BOOL MDrv_CA_STR_SetParamAddr(MS_U8 *pu8VAddr);
DLL_PUBLIC MS_BOOL MDrv_CA_STR_SetMAC(MS_U8 *pu8MAC, MS_U32 u32MACSize);

//-------------------------------------------------------------------------------------------------
// The function will config BGC
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_Init(CA_BGC_CFG stBGCCfg);
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_SetSection(MS_U32 u32SecId, const CA_BGC_SEC_CFG* const pstSecCfg);
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_ResetSection(MS_U32 u32SecId, const MS_U8* const pu8RstPSWD);

//-------------------------------------------------------------------------------------------------
// These functions are used for setting up inverse transform algorithm
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_TA_Init(CA_TA_CFG stTACfg);
DLL_PUBLIC MS_BOOL MDrv_CA_TA_Setup(CA_TA_TDC_CFG * const pstTDCCfg);

//-------------------------------------------------------------------------------------------------
// The function is used for reading/writing a 32-bit customer specific data
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_CustomData_Write(CA_CUSTOM_DATA eData, MS_U32 u32Data);
DLL_PUBLIC MS_BOOL MDrv_CA_CustomData_Read(CA_CUSTOM_DATA eData, MS_U32* pu32Data);

//-------------------------------------------------------------------------------------------------
// The function is used for reading/writing a specific data from secure register
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_Secure_Write(CA_SECURE_ITEM eItem, MS_U32 u32Data, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_Secure_Read(CA_SECURE_ITEM eItem, MS_U32* pu32Data, MS_U32 u32Size);


#ifdef __cplusplus
}
#endif

#endif
