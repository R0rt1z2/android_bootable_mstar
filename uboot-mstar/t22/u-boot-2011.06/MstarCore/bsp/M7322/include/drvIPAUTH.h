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

#ifndef _DRVIPAUTH_H_
#define _DRVIPAUTH_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "MsTypes.h"
#include "MsCommon.h"

#ifndef DLL_PUBLIC
#define DLL_PUBLIC __attribute__ ((visibility ("default"))) 
#endif

#ifndef DLL_LOCAL
#define DLL_LOCAL __attribute__ ((visibility ("hidden")))
#endif

#define IP128_SUPPORT                    0
#define IP256_SUPPORT                    0
#define IP1024_SUPPORT                   1

#define CUSHEADER_LEN                    16

#define IP128_CUSIP_LEN                  32
#define IP256_CUSIP_LEN                  64
#define IP1024_CUSIP_LEN                 256

#define IP128_CUSINFO_LEN                (CUSHEADER_LEN + IP128_CUSIP_LEN)
#define IP256_CUSINFO_LEN                (CUSHEADER_LEN + IP256_CUSIP_LEN)
#define IP1024_CUSINFO_LEN               (CUSHEADER_LEN + IP1024_CUSIP_LEN)

#define IP128_IPCTRL_LEN                 16
#define IP256_IPCTRL_LEN                 32
#define IP1024_IPCTRL_LEN                128

#if (IP1024_SUPPORT)
#define CUSIP_LEN                        IP1024_CUSIP_LEN
#define CUSINFO_LEN                      IP1024_CUSINFO_LEN
#define IPCTRL_LEN                       IP1024_IPCTRL_LEN
#elif(IP256_SUPPORT)
#define CUSIP_LEN                        IP256_CUSIP_LEN
#define CUSINFO_LEN                      IP256_CUSINFO_LEN
#define IPCTRL_LEN                       IP256_IPCTRL_LEN
#else
#define CUSIP_LEN                        IP128_CUSIP_LEN
#define CUSINFO_LEN                      IP128_CUSINFO_LEN
#define IPCTRL_LEN                       IP128_IPCTRL_LEN
#endif

#define IPAUTH_NG                           0
#define IPAUTH_OK                           1
#define DOLBYVER_OK                         0
#define DOLBYVER_NG                         1

#define MSIF_IPAUTH_LIB_CODE               {'I','P','A','_'} //Lib code
#define MSIF_IPAUTH_LIBVER                 {'0','0'}      //LIB version
#define MSIF_IPAUTH_BUILDNUM               {'0','0' }    //Build Number
#define MSIF_IPAUTH_CHANGELIST             {'0','0','1','1','5','7','5','8'} //P4 ChangeList Number

#define IPAUTH_CONTROL_VIDEO_H264           27
#define IPAUTH_CONTROL_XC_DVBC              39

#define IPAUTH_VER                         /* Character String for DRV/API version             */  \
    MSIF_TAG,                              /* 'MSIF'                                           */  \
    MSIF_CLASS,                            /* '00:driver, 02:app, 03:mw                        */  \
    MSIF_CUS,                              /* 0x6666                                           */  \
    MSIF_MOD,                              /* 0x0001                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_IPAUTH_LIB_CODE,                  /* IP__                                             */  \
    MSIF_IPAUTH_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_IPAUTH_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_IPAUTH_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define IPAUTH_SHM_NAME    "Linux IPAUTH driver"
typedef struct
{
    MS_U32  g_ShmID;
    MS_U32  g_u32ClientCounter;
    MS_U8   g_IpVersion;
    MS_U8   g_IpctrlLen;
    MS_U8   g_IpControl[IPCTRL_LEN];
    MS_U8   g_HashKeyData[64];
}ST_IPAUTH_SHARED_VARS;

typedef struct
{
    MS_U32 erk[64];     /* encryption round keys */
    MS_U32 drk[64];     /* decryption round keys */
    MS_S32 nr;             /* number of rounds */
}
ST_AES_CONTEXT;

enum
{
    E_AUTH_GET_CHIPID,
    E_AUTH_GET_DOLBYVERSION,
    E_AUTH_GET_HASHDATA,
    E_AUTH_GET_HASHDIGEST,
};

extern MS_BOOL MApp_IPAUTH_GetLibVer(const MSIF_Version **ppVersion);
DLL_PUBLIC void MApi_AUTH_Process(MS_U8 *string, MS_U8 *hash);
DLL_PUBLIC MS_U8 MApi_AUTH_State(void);
DLL_PUBLIC MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);
DLL_PUBLIC MS_U8 MDrv_AUTH_IPVCheck(MS_U16 u16Bit);
DLL_PUBLIC MS_BOOL MDrv_AUTH_InitialVars(void);
DLL_PUBLIC ST_IPAUTH_SHARED_VARS * MDrv_AUTH_AllocateVars(void);
DLL_PUBLIC MS_BOOL MDrv_AUTH_ResetDefaultVars(ST_IPAUTH_SHARED_VARS * pIPAUTHShared);
DLL_PUBLIC void MDrv_AUTH_GetHashInfo (MS_U8 *hashinfo);
DLL_PUBLIC MS_BOOL MDrv_AUTH_GetHashKeyData (MS_U8 type, MS_U8 *data);

DLL_LOCAL MS_S32 MDrv_AUTH_AES_Set_Key( ST_AES_CONTEXT *ctx, MS_U8 *key, MS_S32 nbits );
DLL_LOCAL void MDrv_AUTH_AES_Encrypt( ST_AES_CONTEXT *ctx, MS_U8 input[16], MS_U8 output[16] );
DLL_LOCAL void MDrv_AUTH_AES_Decrypt( ST_AES_CONTEXT *ctx, MS_U8 input[16], MS_U8 output[16] );


#ifdef __cplusplus

}
#endif

#endif //_DRVIPAUTH_H_
