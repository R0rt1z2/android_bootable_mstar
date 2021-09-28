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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiSecureBoot.h
/// @brief  Secure Boot Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_SECUREBOOT_H_
#define _API_SECUREBOOT_H_

#ifdef API_SECUREBOOT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#include <MsTypes.h>
#include <drvAESDMA.h>
#include <ShareType.h>
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SECUREBOOT_MBX_QUEUESIZE        8

#define MBX_CTRL_FIRE            BIT(0)
#define MBX_CTRL_READBACK        BIT(1)
#define MBX_CTRL_INSTANT         BIT(2)
#define MBX_STATE1_OVERFLOW      BIT(13)
#define MBX_STATE1_ERROR         BIT(14)
#define MBX_STATE1_BUSY          BIT(15)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// emurate SECUREBOOT fucntion return result
typedef enum
{
    /// SECUREBOOT result for failure
    E_SECUREBOOT_FAIL =0,
    /// SECUREBOOT result for OK
    E_SECUREBOOT_OK = 1,

} SECUREBOOT_Result;

///Define SECUREBOOT Acknoledge Flags
typedef enum
{
    E_SECUREBOOT_ACKFLG_WAIT_GETKEY     = (1<<0),      
    E_SECUREBOOT_ACKFLG_WAIT_AESENCODE  = (1<<1),      
    E_SECUREBOOT_ACKFLG_WAIT_AESDECODE  = (1<<2),      
    E_SECUREBOOT_ACKFLG_SETEKEY_WITH_DECRYPTION    = (1<<3),
    E_SECUREBOOT_ACKFLG_ASSIGN_DECRYPTED_KEY    = (1<<4),   
    E_SECUREBOOT_ACKFLG_GETDID    = (1<<5), 

} SECUREBOOT_AckFlags;

typedef enum
{
	E_CMD_SECUREENG_GETKEY=0x00,
	E_CMD_SECUREENG_AES_ENCODE,
    E_CMD_SECUREENG_AES_DECODE,
    E_CMD_SECUREENG_SETEKEY_WITH_DECRYPTION,    
    E_CMD_SECUREENG_ASSIGN_DECRYPTED_KEY,    
    E_CMD_SECUREENG_GETDID,
}SECUREENG_CPUTo51_CMDIDX;

typedef enum
{
    //(1) Ack from 51 to CPU
    E_SECUREENG_51ToCPU_CMD_ACK_GETKEY     = 0x00,
    E_SECUREENG_51ToCPU_CMD_ACK_AES_ENCODE,
    E_SECUREENG_51ToCPU_CMD_ACK_AES_DECODE,
    E_SECUREENG_51ToCPU_CMD_ACK_SETEKEY_WITH_DECRYPTION,
    E_SECUREENG_51ToCPU_CMD_ACK_ASSIGN_DECRYPTED_KEY,    
    E_SECUREENG_51ToCPU_CMD_ACK_GETDID,        
} SECUREENG_51ToCPU_CMDIDX;

typedef struct
{
    MS_U32  u32Pm51ToCpuAddr;
    MS_U32  u32Length;
    MS_U8   u8Status;
} SECUREBOOT_MbxRetStruct;

#if 0
/// Callback function which is called in SECUREBOOT send MAILBOX ISR.
typedef void ( *SECUREBOOT_Callback ) (void* pData);
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL Secure_MailBox_Init(void);
INTERFACE SECUREBOOT_Result Secure_MailBox_GetKey(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct,U32 u32OutAddr,U8  u8KeyImageIndex,U16  u16LenInKeyImage);
INTERFACE SECUREBOOT_Result Secure_MailBox_AESEncode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_AESDecode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_ASSIGN_DECRYPTED_KEY(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U8  u8KeyImageIndex,U16  u16LenInKeyImage, U8 u8IsNoSwap);
INTERFACE SECUREBOOT_Result Secure_MailBox_SET_KEY_WITH_DECRYPTION(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U16 u16Len, U16 u16KeyOffset,U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_GETDID(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U16 u16Len);

#ifdef __cplusplus
}
#endif

#undef INTERFACE
#endif // _API_SECUREBOOT_H_

