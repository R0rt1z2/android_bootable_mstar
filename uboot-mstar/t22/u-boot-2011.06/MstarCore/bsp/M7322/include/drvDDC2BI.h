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
/// @brief  DDC2Bi interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVDDC2BI_H_
#define _DRVDDC2BI_H_

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define MSIF_DDC_LIB_CODE               {'D','D','C','_'}
#define MSIF_DDC_LIBVER                 {'0','1'}
#define MSIF_DDC_BUILDNUM               {'0','2'}
#define MSIF_DDC_CHANGELIST             {'0','0','3','1','6','8','6','6'}
#define DDC2BI_DRV_VERSION              /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DDC_LIB_CODE,                  /* IP__                                             */  \
    MSIF_DDC_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_DDC_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_DDC_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//----------------------------
// Return value
//----------------------------
typedef enum
{
    E_DDC2BI_RET_FAIL = 0,
    E_DDC2BI_RET_OK,
} E_DDC2BI_ReturnValue;


// DDC2Bi receive/transmit status bytes definitions
#define    DDC2B_CLEAR                  0
#define    DDC2B_SRCADDRESS             1
#define    DDC2B_COMMAND                2
#define    DDC2B_RECBODY                3
#define    DDC2B_WAITFORCK              4
#define    DDC2B_COMPLETED              5
#define    DDC2B_MSGON                  6
#define    DDC2B_OVER                   7

// receive/transmit buffer syntax defines
#define LENGTH                          0
#define COMMAND                         1
#define DATA1                           2
#define DATA2                           3
#define CHKSUM                          4

// DDC2Bi standard definitions
#define DDC2B_DEST_ADDRESS              0x6e
#define DDC2B_SRC_ADDRESS               0x51
#define DDC2Bi_CONTROL_STATUS_FLAG      0x80

// checksum start values
#define    INITRxCK                     DDC2B_SRC_ADDRESS ^ DDC2B_DEST_ADDRESS
#define    INITTxCK                     0x50^DDC2B_DEST_ADDRESS
#define    INITNMVTxCK                  0x50^DDC2B_SRC_ADDRESS

//interrupt flag
#define TXBI                            0x10
#define RCBI                            0x08
//#define SlvBMI                        0x20


#define DDC2B_SRC_ADDRESS_CUSTOMER      0x50
#define MAX_TXWAIT_DELAY                100
#define MaxBufLen                       128


// This defination must be fine.

/*
    Please measure the execution time of a loop without any statement for DDC_WAITING_LOOPSCOUNT.
    ex:
        for ( i = 0 ; i < DDC_WAITING_LOOPSCOUNT ; i++) asm("");

    The asm("") is used to prevent compiler doing optimization on that loop.

    The total execution time of loops should be larger than 1/12M (70ns for DDC clk).
    (Move this part into HAL later)
*/
#define DDC_WAITING_LOOPSCOUNT 3   // 46ns/loop in 172Mhz HK.

typedef MS_U8(*CALLBACK_AlignControl)(MS_U8 *pDDCBuffData);


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
/**
*   DDC2BI Info
*/
typedef struct
{
    MS_U8 Reserved;
}MS_DDC2BI_INFO;

/**
*   DDC2BI Status
*/
typedef struct
{
    MS_BOOL bIsInitialized;
    MS_BOOL bIsRunning;
}MS_DDC2BI_Status;

E_DDC2BI_ReturnValue  MDrv_DDC2BI_GetLibVer(const MSIF_Version **ppVersion);      ///< Get version (without Mutex protect)
E_DDC2BI_ReturnValue  MDrv_DDC2BI_GetInfo(MS_DDC2BI_INFO *pInfo);
E_DDC2BI_ReturnValue  MDrv_DDC2BI_GetStatus(MS_DDC2BI_Status *pStatus);
E_DDC2BI_ReturnValue  MDrv_DDC2BI_SetDbgLevel(MS_U16 u16DbgSwitch);

void MDrv_DDC2BI_Init(void);
MS_BOOL MDrv_DDC2BI_Exit(void);
void MDrv_DDC2BI_Set_StandardCallBack(CALLBACK_AlignControl align_control);      ///< DDC2BI callback function for Standard
void MDrv_DDC2BI_Set_CustomerCallBack(CALLBACK_AlignControl align_control);      ///< DDC2BI callback function for Customer

#endif  // _DRVDDC2BI_H_
