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

////////////////////////////////////////////////////////////////////////////////
//
/// @file apiSecureBoot.h
/// This file includes MStar application interface for Secure Boot
/// @brief API for Secure Boot
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define API_SECUREBOOT_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <ShareType.h>
#include <linux/string.h> //for memset()
#include <uboot_mmap.h>
#include <MsSystem.h>
#include <secure/apiSecureBoot.h>
#include <drvMBX.h>
#include <MsDebug.h>
#include <malloc.h>
#include <drvAESDMA.h>
#include <secure/MsSecureCommon.h>

extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define SB_API_DBG(x)   x

#if defined (CONFIG_SECURITY_BOOT)
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SECUREBOOT_MBX_QUEUESIZE        8
#define SECUREBOOT_MBX_TIMEOUT          5000

#define RIU_TMPBASE 0xBF200000  //for test
#define CMEM(addr)         (*((volatile unsigned char*)(addr)))
#define AES_BUF_ALIGNMENT_SIZE 16  //for Secure_R2_SHA256
#define SACTTERGATHER_TABLE_SIZE 16  //for Secure_R2_SHA256
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static SECUREBOOT_Result gSecureBootResult;
static SECUREBOOT_AckFlags gSecureBootAckFlags;
static MBX_Result gMBXResult;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _SecureBoot_MailBoxHandler(MBX_Class eMsgClass, SECUREBOOT_MbxRetStruct *pSecureBootRetStruct)
{
    MBX_Msg MB_Command;

    if(eMsgClass != E_MBX_CLASS_SECURE_WAIT)
    {
        //printf("Invalid Message Class!\n");
        return;
    }

    gMBXResult = MDrv_MBX_RecvMsg(eMsgClass, &MB_Command, SECUREBOOT_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);

    if (gMBXResult == E_MBX_ERR_TIME_OUT)
    {
        printf("Handler Timeout!\n");
        return;
    }

    if (gMBXResult == E_MBX_SUCCESS)
    {
        if ((MB_Command.u8Ctrl != 0) && (MB_Command.u8Ctrl != 1))
        {
            gSecureBootResult = E_SECUREBOOT_FAIL;
            printf("Not Implemented!\n");
            return;
        }

        {
            MS_U8 i;
            //printf("Get SECURE command: 0x%02x, 0x%02x\n", MB_Command.u8Index, MB_Command.u8ParameterCount);
            for(i=0; i<MB_Command.u8ParameterCount; i++)
            {
                printf("Parameter[%d]=0x%02x\n", i, MB_Command.u8Parameters[i]);
            }
        }

       gSecureBootResult = E_SECUREBOOT_FAIL;
        switch (MB_Command.u8Index)
        {
            case E_SECUREENG_51ToCPU_CMD_ACK_GETKEY:
                gSecureBootResult = E_SECUREBOOT_OK;
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_GETKEY);
                //printf("mailbox:E_SECUREENG_51ToCPU_CMD_ACK_GETKEY \n");
                break;
            case E_SECUREENG_51ToCPU_CMD_ACK_AES_ENCODE:
                gSecureBootResult = E_SECUREBOOT_OK;
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_AESENCODE);
                //printf("mailbox:E_SECUREENG_51ToCPU_CMD_ACK_AES_ENCODE \n");
                break;
            case E_SECUREENG_51ToCPU_CMD_ACK_AES_DECODE:
                gSecureBootResult = E_SECUREBOOT_OK;
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_AESDECODE);
                //printf("mailbox:E_SECUREENG_51ToCPU_CMD_ACK_AES_DECODE \n");
                break;
            case E_SECUREENG_51ToCPU_CMD_ACK_SETEKEY_WITH_DECRYPTION:
                gSecureBootResult = E_SECUREBOOT_OK;
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_SETEKEY_WITH_DECRYPTION);
                //printf("mailbox:E_SECUREENG_51ToCPU_CMD_ACK_SETEKEY_WITH_DECRYPTION \n");
                break;
            case E_SECUREENG_51ToCPU_CMD_ACK_ASSIGN_DECRYPTED_KEY:
                gSecureBootResult = E_SECUREBOOT_OK;
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_ASSIGN_DECRYPTED_KEY);
                //printf("mailbox:E_SECUREENG_51ToCPU_CMD_ACK_ASSIGN_DECRYPTED_KEY \n");
            case E_SECUREENG_51ToCPU_CMD_ACK_GETDID:
                gSecureBootResult = E_SECUREBOOT_OK;
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_GETDID);

                break;
            default:
                break;
        }
    }
    return;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
/******************************************************************************/
/// API for Secure Boot init Mailbox:
/// Secure_MailBox_Init
/// @param \b IN:
/// @param \b IN:
/// @return BOOLEAN: Success or not
/// - TRUE: Success
/// - FALSE: Fail
/******************************************************************************/

MS_BOOL Secure_MailBox_Init(void)
{
    UBOOT_TRACE("IN\n");
    if(MsApi_MBX_Init()==TRUE)
    {

        if( E_MBX_SUCCESS != MDrv_MBX_RegisterMSG(E_MBX_CLASS_SECURE_WAIT, SECUREBOOT_MBX_QUEUESIZE))
    {
            UBOOT_DEBUG("E_MBX_CLASS_SECURE_WAIT - MBX register msg error\n");
    }
        UBOOT_TRACE("OK\n");
        return TRUE;
    }
    else
    {
        UBOOT_ERROR("fail\n");
        return FALSE;
    }
}

MS_BOOL Secure_MailBox_Deinit(void)
{
    UBOOT_TRACE("IN\n");
    if( E_MBX_SUCCESS != MDrv_MBX_UnRegisterMSG(E_MBX_CLASS_SECURE_WAIT, TRUE))
    {
        UBOOT_DEBUG("@@ E_MBX_CLASS_SECURE_WAIT - MBX unregister msg error\n");
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}

SECUREBOOT_Result Secure_MailBox_GetKey(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct,U32 u32OutAddr,U8  u8KeyImageIndex,U16  u16LenInKeyImage)
{
    MBX_Msg MB_Command;
    UBOOT_TRACE("IN\n");
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_GETKEY;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_NOWAIT;
    MB_Command.u8Index = E_CMD_SECUREENG_GETKEY;
    MB_Command.u8ParameterCount = 10;
    #if 1
    MB_Command.u8Parameters[0] =  (U8)(u32OutAddr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32OutAddr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32OutAddr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32OutAddr&0x000000FF);
    MB_Command.u8Parameters[4] =   u8KeyImageIndex;
    MB_Command.u8Parameters[5] =  (U8)(u16LenInKeyImage>>8);
    MB_Command.u8Parameters[6] =  (U8)(u16LenInKeyImage&0xFF);
    MB_Command.u8Parameters[7] =  0;
    MB_Command.u8Parameters[8] =  0;
    MB_Command.u8Parameters[9] =  0;
    #endif
    MDrv_MBX_SendMsg(&MB_Command);
    UBOOT_DEBUG("wait mailbx response\n");
    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECURE_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_GETKEY) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        UBOOT_ERROR("Fail\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return gSecureBootResult;

}

SECUREBOOT_Result Secure_MailBox_AESEncode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode)
{
    MBX_Msg MB_Command;
    UBOOT_TRACE("IN\n");
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_AESENCODE;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_NOWAIT;
    MB_Command.u8Index = E_CMD_SECUREENG_AES_ENCODE;
    MB_Command.u8ParameterCount = 10;
    MB_Command.u8Parameters[0] =  (U8)(u32Addr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32Addr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32Addr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32Addr&0x000000FF);

    MB_Command.u8Parameters[4] =  (U8)(u32Len>>24);
    MB_Command.u8Parameters[5] =  (U8)(u32Len>>16);
    MB_Command.u8Parameters[6] =  (U8)(u32Len>>8);
    MB_Command.u8Parameters[7] =  (U8)(u32Len&0x000000FF);

    MB_Command.u8Parameters[8] =  u8Mode;
    MB_Command.u8Parameters[9] =  0;

    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECURE_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_AESENCODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));


    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        UBOOT_ERROR("Secure_MailBox_AESEncode Fail\n");
        //while(1);
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return gSecureBootResult;
}

SECUREBOOT_Result Secure_MailBox_ASSIGN_DECRYPTED_KEY(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U8  u8KeyImageIndex,U16  u16LenInKeyImage, U8 u8IsNoSwap)
{
    MBX_Msg MB_Command;
    UBOOT_TRACE("IN\n");

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    //gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_AESDECODE;
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_ASSIGN_DECRYPTED_KEY;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_NOWAIT;
    MB_Command.u8Index = E_CMD_SECUREENG_ASSIGN_DECRYPTED_KEY;
    MB_Command.u8ParameterCount = 10;
    MB_Command.u8Parameters[0] =  u8KeyImageIndex;
    MB_Command.u8Parameters[1] =  (U8)(u16LenInKeyImage>>8);
    MB_Command.u8Parameters[2] =  (U8)(u16LenInKeyImage&0xFF);
    MB_Command.u8Parameters[3] =  u8IsNoSwap;
    MB_Command.u8Parameters[4] =  0;
    MB_Command.u8Parameters[5] =  0;
    MB_Command.u8Parameters[6] =  0;
    MB_Command.u8Parameters[7] =  0;
    MB_Command.u8Parameters[8] =  0;
    MB_Command.u8Parameters[9] =  0;

    MDrv_MBX_SendMsg(&MB_Command);
        //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECURE_WAIT, pSecureBootMbxRetStruct);
    }
    //while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_AESDECODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_ASSIGN_DECRYPTED_KEY) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        UBOOT_ERROR("Secure_MailBox_ASSIGN_DECRYPTED_KEY Fail\n");
        //while(1);
    }
    else
    {
         UBOOT_TRACE("OK\n");
    }

    return gSecureBootResult;

}

SECUREBOOT_Result Secure_MailBox_SET_KEY_WITH_DECRYPTION(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U16 u16Len, U16 u16KeyOffset,U8 u8Mode)
{
    MBX_Msg MB_Command;
    UBOOT_TRACE("IN\n");

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    //gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_AESDECODE;
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_SETEKEY_WITH_DECRYPTION;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_NOWAIT;
    MB_Command.u8Index = E_CMD_SECUREENG_SETEKEY_WITH_DECRYPTION;
    MB_Command.u8ParameterCount = 10;

    MB_Command.u8Parameters[0] =  (U8)(u32Addr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32Addr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32Addr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32Addr&0x000000FF);

    MB_Command.u8Parameters[4] =  (U8)(u16Len>>8);
    MB_Command.u8Parameters[5] =  (U8)(u16Len&0x000000FF);

    MB_Command.u8Parameters[6] =  (U8)(u16KeyOffset>>8);
    MB_Command.u8Parameters[7] =  (U8)(u16KeyOffset&0x000000FF);

    MB_Command.u8Parameters[8] =  (U8)(u8Mode);
    MB_Command.u8Parameters[9] =  0;


    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECURE_WAIT, pSecureBootMbxRetStruct);
    }
    //while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_AESDECODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_SETEKEY_WITH_DECRYPTION) && (gMBXResult != E_MBX_ERR_TIME_OUT));



    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        UBOOT_ERROR("Secure_MailBox_ASSIGN_DECRYPTED_KEY Fail\n");
        //while(1);
    }
    else
    {
       UBOOT_TRACE("OK\n");
    }

    return gSecureBootResult;

}



SECUREBOOT_Result Secure_MailBox_AESDecode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode)
{
    MBX_Msg MB_Command;
    UBOOT_TRACE("IN\n");

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_AESDECODE;


    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_NOWAIT;
    MB_Command.u8Index = E_CMD_SECUREENG_AES_DECODE;


    MB_Command.u8ParameterCount = 10;
    MB_Command.u8Parameters[0] =  (U8)(u32Addr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32Addr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32Addr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32Addr&0x000000FF);

    MB_Command.u8Parameters[4] =  (U8)(u32Len>>24);
    MB_Command.u8Parameters[5] =  (U8)(u32Len>>16);
    MB_Command.u8Parameters[6] =  (U8)(u32Len>>8);
    MB_Command.u8Parameters[7] =  (U8)(u32Len&0x000000FF);

    MB_Command.u8Parameters[8] =  (U8)(u8Mode);
    MB_Command.u8Parameters[9] =  0;

    MDrv_MBX_SendMsg(&MB_Command);


    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECURE_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_AESDECODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));



    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        UBOOT_ERROR("Secure_MailBox_AESDecode Fail\n");
        //while(1);
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return gSecureBootResult;
}


SECUREBOOT_Result Secure_MailBox_GETDID(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U16 u16Len)
{
    MBX_Msg MB_Command;
    UBOOT_TRACE("IN\n");

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_GETDID;


    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_NOWAIT;
    MB_Command.u8Index = E_CMD_SECUREENG_GETDID;


    MB_Command.u8ParameterCount = 10;
    MB_Command.u8Parameters[0] =  (U8)(u32Addr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32Addr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32Addr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32Addr&0x000000FF);

    MB_Command.u8Parameters[4] =  (U8)(u16Len>>8);
    MB_Command.u8Parameters[5] =  (U8)(u16Len&0x000000FF);
    MB_Command.u8Parameters[6] =  0;
    MB_Command.u8Parameters[7] =  0;
    MB_Command.u8Parameters[8] =  0;
    MB_Command.u8Parameters[9] =  0;

    MDrv_MBX_SendMsg(&MB_Command);


    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECURE_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_GETDID) && (gMBXResult != E_MBX_ERR_TIME_OUT));



    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        UBOOT_ERROR("Secure_MailBox_AESDecode Fail\n");
        //while(1);
    }
    else
    {
        //printf("Secure_MailBox_AESDecode OK\n");
        UBOOT_TRACE("OK\n");
    }

    return gSecureBootResult;
}
#endif
#undef API_SECUREBOOT_C

