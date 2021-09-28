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


#ifndef __CMD_MS_SECURE_BOOT_H__
#define __CMD_MS_SECURE_BOOT_H__
#include <secure/MsSecureCommon.h>

#if defined (CONFIG_R2TEE_TEELOADER)
typedef struct
{
    _SUB_SECURE_INFO customer;
    U8 u8RSABootPublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAUpgradePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAImagePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESBootKey[AES_KEY_LEN];
    U8 u8AESUpgradeKey[AES_KEY_LEN];
    U8 u8ReDRMKey[AES_KEY_LEN];
    U8 u8MagicID[16];
    U8 crc[4];
}CUSTOMER_KEY_BANK;
#else
typedef struct
{
    _SUB_SECURE_INFO customer;
    U8 u8RSABootPublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAUpgradePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAImagePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESBootKey[AES_KEY_LEN];
    U8 u8AESUpgradeKey[AES_KEY_LEN];
    U8 u8MagicID[16];
    U8 crc[4];
}CUSTOMER_KEY_BANK;
#endif

//-------------------------------------------------------------------------------------------------
/// Decrypt and authenticate RawData image
/// @param KLName                \b IN: RawData image name
/// @param u32kernelAddr             \b IN: RawData image adderss
/// @return  int                               \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int RawData_Authentication(char *KLName,unsigned int u32kernelAddr);

//-------------------------------------------------------------------------------------------------
/// Authenticate rootfs.ubifs, mslib.ubifs and application.ubifs
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int FileSystem_Authentication(void);

//-------------------------------------------------------------------------------------------------
/// get RSA N Public Key according different KEY_INDEX.
/// @param  enKey                 \b IN: select the enum key you need.
/// @param  buffer                \b IN: The dram address of RSA Key
/// @param  len                   \b IN: RSA Key Length
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int GetPublicKeyN(KEY_INDEX enKey, U8 *buffer, U32 len);

//-------------------------------------------------------------------------------------------------
/// get RSA E Public Key according different KEY_INDEX.
/// @param  enKey                 \b IN: select the enum key you need.
/// @param  buffer                \b IN: The dram address of RSA Key
/// @param  len                   \b IN: RSA Key Length
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int GetPublicKeyE(KEY_INDEX enKey, U8 *buffer, U32 len);

//-------------------------------------------------------------------------------------------------
/// get AES Key according different KEY_INDEX.
/// @param  enKey                 \b IN: select the enum key you need.
/// @param  keyBuf                \b IN: The dram address of AES Key
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int GetAESKey(KEY_INDEX enKey,U8 *keyBuf);
int do_ReadKL(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_BootKL(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_SecureBootCmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_verify_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_rsa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_Secure_Init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase5(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_Authenticate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DumpSignature(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_performance_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_AN_Authenticate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
void Secure_HaltCPU(void);
int CopyCustomerKeyBank(U32 u32DramAddr);
int DecryptKernelImage(char *dataBuf, unsigned int len);
int GetPublicKeyN(KEY_INDEX enKey, U8 *buffer, U32 len);
int GetPublicKeyE(KEY_INDEX enKey, U8 *buffer, U32 len);
int GetSignature(char *volume, U8 num, U8 *buffer, U32 len);
int GetImageSize(char *volume, U8 num, U32 *size);
int do_secure_image_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
MS_BOOL get_secure_init_status(void);
#endif

