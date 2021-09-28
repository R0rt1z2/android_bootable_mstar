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

#ifndef __DRV_URSA_COMMON_H__
#define __DRV_URSA_COMMON_H__

#include <MsTypes.h>

#define URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG     0x0EA2
#define URSA_COMMON_URSA6_HANDSHAKE                0x2  // U6, FRC and KS2
#define URSA_COMMON_URSA7_HANDSHAKE                0x4  // U7 and U9


/*
 * URSA status code:
 * successful operation returns URSA_SUCCED;
 * failed operation returns URSA_FAILED.
 */
typedef enum {
    URSA_SUCCED = 0,
    URSA_FAILED,
} URSA_COMMON_STATUS;

/*
 * URSA type
 */
typedef enum {
    URSA_COMMON_UNKNOWN = 0,
    URSA_COMMON_U6 = 6,
    URSA_COMMON_U7 = 7,
    URSA_COMMON_U8 = 8,
    URSA_COMMON_U9 = 9,
    URSA_COMMON_U20 = 20,
    URSA_COMMON_KS2 = 1000,
    URSA_COMMON_FRC = 2000,
    URSA_COMMON_U11 = 11,
} URSA_COMMON_TYPE;
typedef enum _FlashProgErrorType
{
    FlashEnterIspModeErr,
    FlashBlankingErr,
    FlashVerifyErr,
    FlashIDErr,
    FlashProgOK,
} FlashProgErrorType;

typedef enum _FlashProgStateType
{
    FlashProgStateInit,
    FlashProgStateEnterIsp,
    FlashProgStateErase,
    FlashProgStateBlank,
    FlashProgStateProgram,
    FlashProgStateVerify,
    FlashProgStateExit,
    FlashProgStateIdle,
} FlashProgStateType;

typedef struct
{
    MS_U32 size;            // 1-4
    MS_U8 manufacturer_id;  // 5
    MS_U8 device_id;        // 6

    // command
    MS_U8 WREN;             // 7
    MS_U8 WRDI;             // 8
    MS_U8 RDSR;             // 9
    MS_U8 WRSR;             // 10
    MS_U8 READS;            // 11
    MS_U8 FAST_READ;        // 12
    MS_U8 PG_PROG;          // 13
    MS_U8 SECTOR_ERASE;     // 14
    MS_U8 BLOCK_ERASE;      // 15
    MS_U8 CHIP_ERASE;       // 16
    MS_U8 RDID;             // 17
    MS_U8 LOCK;             // 18
    MS_U16 ChipEraseMaxTime;// 19-20//Unit:100ms
} SFlashType;

typedef enum _FlashType
{
    Flash_PMC512,//PMC
    Flash_PMC010,
    Flash_PMC020,
    Flash_PMC040,
    Flash_EN25P10,//EON
    Flash_EN25P20,
    Flash_EN25P40,
    Flash_EN25P80,
    Flash_EN25F40,
    Flash_EN25F80,
    Flash_EN25F16,
    Flash_EN25P16,
    Flash_EN25B20,
    Flash_EN25B40,
    Flash_EN25B40T,
    Flash_EN25B16,
    Flash_EN25B16T,
    Flash_EN25B32,
    Flash_S25FL004A,//SPANSION
    Flash_S25FL016A,
    Flash_S25FL040A,
    Flash_NX25P16,//Winbond & NX
    Flash_W25X10,//not test
    Flash_W25X20,//not test
    Flash_W25X40,//not test
    Flash_W25P20,//not test
    Flash_W25P40,
    Flash_W25X16,
    Flash_W25X32,
    Flash_W25X64,
    Flash_W25Q32,
    Flash_SST25VF016B,//SST
    Flash_SST25VF040B,//not test
    Flash_MX25L4005A,//MX
    Flash_MX25L8005,//MX
    Flash_MX25L2005,//not test
    Flash_MX25L1605,
    Flash_MX25L3205,
    Flash_MX25L6405,
    Flash_QB25F160S,//intel QB25F160S33B8
    Flash_QB25F320S,//intel QB25F320S33B8
    Flash_QB25F640S,//intel QB25F640S33B8
    Flash_A25L40P,//AMIC A25L40P
    Flash_GD25Q32,//GD
    Flash_EN25Q32A,//EN
    Flash_NUMS,
} FlashType;

URSA_COMMON_STATUS MDrv_URSA_COM_Write_URSA_HandshakeType(void);
URSA_COMMON_TYPE MDrv_URSA_COM_Get_URSA_Type(void);

#endif //__DRV_URSA_COMMON_H__
