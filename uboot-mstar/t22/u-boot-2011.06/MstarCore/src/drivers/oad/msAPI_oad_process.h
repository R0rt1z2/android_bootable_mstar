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
#ifndef _MSAPI_OAD_PROCESS_H_
#define _MSAPI_OAD_PROCESS_H_

/*------------------------------------------------------------------------------
*   include files
*-----------------------------------------------------------------------------*/
#include "linux/types.h"
#include <MsTypes.h>
#include <MsVersion.h>
#if defined(BUILD_LIB)
#include "MsVersion.h"
#endif

/*------------------------------------------------------------------------------
*   Local defines
*-----------------------------------------------------------------------------*/
#define MSIF_OAD_LIB_CODE         {'O','A','D','_'} //Lib code
#define MSIF_OAD_LIBVER           {'0','0'} //LIB version
#define MSIF_OAD_BUILDNUM         {'0','0'} //Build Number
#define MSIF_OAD_CHANGELIST       {'0','0','2','6','9','7','8','1'} //P4 ChangeList Number

#define MW_OAD_VER                  /* Character String for DRV/API version             */  \
    MSIF_TAG,                   /* 'MSIF'                                           */  \
    MSIF_CLASS,                 /* '00'                                             */  \
    MSIF_CUS,                   /* 0x0000                                           */  \
    MSIF_MOD,                   /* 0x0000                                           */  \
    MSIF_CHIP,                                                                          \
    MSIF_CPU,                                                                           \
    MSIF_OAD_LIB_CODE,              /* IP__                                             */  \
    MSIF_OAD_LIBVER,                /* 0.0 ~ Z.Z                                        */  \
    MSIF_OAD_BUILDNUM,              /* 00 ~ 99                                          */  \
    MSIF_OAD_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS

#define SECTION_HEADER_SIZE 8
#define BIOP_HEADER_LEN     44

#define DSMMSG_HEADER_SIZE  12
#define DSIMSG_HEADER_SIZE  20
#define DIIMSG_HEADER_SIZE  20
#define DDBMSG_HEADER_SIZE  6

#define OAD_CONTROL_SECTION_TYPE 0x3B   // control sections: DSI, DII
#define OAD_DATA_SECTION_TYPE    0x3C   // data sections:    DDB
#define OAD_UNT_SECTION_TYPE     0x4B

#define OAD_DSMCC_MSGID_DSI     0x1006  // DSI
#define OAD_DSMCC_MSGID_DII     0x1002  // DII
#define OAD_DSMCC_MSGID_DDB     0x1003  // DDB

#define OAD_INVALID_ADDR    0xFFFFFFFFUL

#define OAD_MODE_INVALID    0xFF
#define OAD_MODE_DATACAR    0x00     // Data Carousel
#define OAD_MODE_OBJCAR     0x01     // Object Carousel (specially for UK)

#define GET_2BYTE( cp ) ( ( ( (MS_U16)(*cp) ) << 8 ) | ( ( (MS_U16)(*(cp+1)) ) ) )
#define GET_3BYTE( cp ) ( ( ( (MS_U32)(*cp) )<< 16 ) | ( ( (MS_U32)(*(cp+1)) ) <<  8 ) | ( ( (MS_U32)(*(cp+2)) ) ) )
#define GET_4BYTE( cp ) ( ( ( (MS_U32)(*cp) )<< 24 ) | ( ( (MS_U32)(*(cp+1)) ) << 16 ) | ( ( (MS_U32)(*(cp+2)) ) << 8 ) | ( (MS_U32)(*(cp+3)) ) )


/*------------------------------------------------------------------------------
*   Debug defines
*-----------------------------------------------------------------------------*/
#define ENABLE_DBG 1
#if ENABLE_DBG
#define OAD_INFO(x)         x
#define OAD_DEBUG(x)        //x
#define OAD_ASSERT(x)       x       // dont turn off this
#else
#define OAD_INFO(x)
#define OAD_DEBUG(x)
#define OAD_ASSERT(x)
#endif

typedef struct
{
    MS_U32 u32StartTime;       // Unit: sec
    MS_U32 u32EndTime;         // Unit: sec
    MS_U8  u8FinalAvail: 1;    // 1: final version, no further update
    MS_U8  u8PeriodFlag: 1;    // 1: Periodicity
    MS_U8  u8PeriodUnit: 2;    // 00: Second, 01: Minute, 10: Hour, 11:Day
    MS_U8  u8DurationUnit: 2;  // 00: Second, 01: Minute, 10: Hour, 11:Day
    MS_U8  u8EstimateUnit: 2;  // 00: Second, 01: Minute, 10: Hour, 11:Day
    MS_U8  u8Period;
    MS_U8  u8Duration;
    MS_U8  u8Estimate;
} UNT_SCHEDULE;

typedef struct
{
   MS_U16 association_tag ;
} UNT_LOCATION;

typedef struct
{
  UNT_LOCATION untLocation ;
  UNT_SCHEDULE untSchedule ;
} UNT_DESCRIPTOR ;

typedef struct
{
    MS_U16 u16Year;
    MS_U8 u8Month;
    MS_U8 u8Day;
    MS_U8 u8Hour;
    MS_U8 u8Min;
    MS_U8 u8Sec;
} ST_TIME_OAD;

typedef enum
{
    OAD_SUCCEED = 0 ,
    OAD_FAIL,
    OAD_COMPLETE,
    OAD_CONTINUE
}OAD_STATUS_REPORT_TYPE;

void msAPI_OAD_ClearData( void );
void msAPI_OAD_SetOui(MS_U8 byte1, MS_U8 byte2, MS_U8 byte3);
void msAPI_OAD_SetDDBBuffer( MS_U32, MS_U32 );
MS_U32 msAPI_OAD_GetDDBBufferAddr( void );
void msAPI_OAD_SetVersionCheck(MS_U8 (*cb)(MS_U16,MS_U8 *));
MS_U8 msAPI_OAD_GetDsiVersionNum(void);
void msAPI_OAD_GetModuleInfo( MS_U32 *startAddr, MS_U32 *moduleSize );
MS_U8 msAPI_OAD_GetProgress( void );
void msAPI_OAD_SetOADFileFound(MS_BOOL bFound);
MS_BOOL msAPI_OAD_IsOADFileFound(void);
MS_U32 msAPI_OAD_MemAlloc ( MS_U32 u32MemSize );
OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessDSI ( MS_U8 * );
OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessDII ( MS_U8 * );
OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessDDB ( MS_U8 * ,MS_U16 *,MS_U16 *,MS_U32 *,MS_U16 *);
OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessUNT ( MS_U8 * ,UNT_DESCRIPTOR *);
MS_BOOL msAPI_OAD_GetLibVer(const MSIF_Version **ppVersion);
extern MS_U32 MsOS_PA2KSEG1(MS_U32 addr);
void msAPI_OAD_SetOADMode(MS_U8 mode);
void msAPI_OAD_SetGroupId(MS_U32 u32GroupId);
MS_U32 msAPI_OAD_GetBlockSize(void);
MS_U32 msAPI_OAD_GetDownloadId(void);
MS_U32 msAPI_OAD_GetGroupId(void);
#endif // _MSAPI_OAD_PROCESS_H_
