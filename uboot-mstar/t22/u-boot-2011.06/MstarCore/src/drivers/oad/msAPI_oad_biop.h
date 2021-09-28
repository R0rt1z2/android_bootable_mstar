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
#ifndef _MSAPI_OAD_BIOP_H_
#define _MSAPI_OAD_BIOP_H_

/******************************************************************************/
/*                       Global Variable Declarations                         */
/******************************************************************************/

/*------------------------------------------------------------------------------
*   Debug defines
*-----------------------------------------------------------------------------*/
#define OAD_BIOP_DBG(x)     //x
#define OAD_BIOP_ASSERT(x)  x
#define OAD_BIOP_D1(x)      //x

/* magic header value */
#define BIOP_MAGIC_STR              "BIOP"
#define BIOP_MAGIC_LEN              4

/* version we are expecting */
#define BIOP_VSN_MAJOR              1
#define BIOP_VSN_MINOR              0

/* byte order */
#define BIOP_BIGENDIAN              0

/* message type we are expecting */
#define BIOP_MSG_TYPE               0

/* objectKind types */
#define BIOP_DIR                    "dir"
#define BIOP_FILE                   "fil"
#define BIOP_STREAM                 "str"
#define BIOP_SERVICEGATEWAY         "srg"
#define BIOP_STREAMEVENT            "ste"

/* IOP::IOR profileId_tag values */
#define TAG_BIOP                    0x49534f06
#define TAG_LITE_OPTIONS            0x49534f05

/* BIOP::ObjectLocation tag */
#define TAG_ObjectLocation          0x49534f50
#define TAG_ServiceLocation         0x49534f46

/* DSM::ConnBinder tag */
#define TAG_ConnBinder              0x49534f40

/* use types */
#define BIOP_DELIVERY_PARA_USE      0x0016

/* selector_type we are expecting */
#define SELECTOR_TYPE_MESSAGE_LEN   0x0a
#define SELECTOR_TYPE_MESSAGE       0x0001

/* allowed tap use definitions for taps in a BIOP::StreamMessage */
#define STR_NPT_USE                 0x000b
#define STR_STATUS_AND_EVENT_USE    0x000c
#define STR_EVENT_USE               0x000d
#define STR_STATUS_USE              0x000e
#define BIOP_ES_USE                 0x0018
#define BIOP_PROGRAM_USE            0x0019

/* Invalid part */
#define INVALID_MANUFACTURER_ID     0xFFFFFFFFUL
#define INVALID_VERSION_ID          0xFFFFFFFFUL
#define INVALID_TRANSACTION_ID      0xFFFFFFFFUL

#define MIS_SELBYTE_LEN 32

typedef enum
{
    XDATA_TYPE = 0,
    SDRAM_TYPE = 1,
} EN_DRAM_TYPE;

struct BIOPVersion
{
    MS_U8  u8major;
    MS_U8  u8minor;
} __attribute__ ((packed)) ;

struct BIOPMessageHeader
{
    char                magic[4];                       /* 'BIOP' */
    struct  BIOPVersion biop_version;       /* 1.0 */
    MS_U8                  byte_order;                     /* 0 = big endian */
    MS_U8                  message_type;                   /* should be 0 */
    MS_U32                 message_size;                   /* not including this header */
} __attribute__ ((packed)) ;

/* data type to hold a sequence */
struct biop_sequence
{
    MS_U32 size;
    MS_U32 data_p;
}__attribute__ ((packed));

/* data type to hold an IOP::IOR object reference */
struct biop_iop_ior
{
    MS_U16                     association_tag;    /* maps to an elementary_PID */
    MS_U32                     carousel_id;
    MS_U16                     module_id;
    struct  biop_sequence   key;
} __attribute__ ((packed));

typedef struct biop_profile_body_ior
{
    MS_U16 association_tag;    /* maps to an elementary_PID */
    MS_U32 carousel_id;
    MS_U16 module_id;
    MS_BOOL b_valid;
} __attribute__ ((packed)) BIOP_BODY_PROFILE_IOR_INFO;

/* BIOP: Module Info Message */
struct biop_module_info
{
    MS_U32 module_time_out;
    MS_U32 block_time_out;
    MS_U32 min_block_time;
    MS_U8  taps_count;
}__attribute__ ((packed)) ;

/* BIOP: Module Info User Info Message */
struct biop_module_info_user_info
{
    MS_U16 id;
    MS_U16 use;
    MS_U16 association_tag;
    MS_U8  selector_length;
    MS_U8  user_info_length;
} __attribute__ ((packed));

typedef struct biop_manufacturer_info
{
    MS_U32 manufacturer_id;
    MS_U32 version_id;
    MS_U8  reserved[MIS_SELBYTE_LEN];
} __attribute__ ((packed))BIOP_USERINFO_MIS;

typedef struct biop_file_info
{
    MS_U32 FileBufferAddr;
    MS_U32 FileBufferSize;
}__attribute__ ((packed)) BIOP_FILE_INFO;


void process_userinfo_GetMISInfo( BIOP_USERINFO_MIS *mis_info );
MS_U32   process_profile_Body_GetTransID( void );
void process_fileinfo_Get( BIOP_FILE_INFO *FileInfoData );
void process_serviceGateway_IOR_Get( BIOP_BODY_PROFILE_IOR_INFO *ior_info );
void process_biop_service_gateway_info( unsigned char *section );
MS_BOOL process_biop( MS_U32 Addr, MS_U32 size, MS_U8 DataType );

#endif //_MSAPI_OAD_BIOP_H_

