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

#ifndef __DRVFCIE_H__
#define __DRVFCIE_H__


#include "drvFCIE_reg.h"
#include "drvFCIE_err.h"
#include "drvFCIE_config.h"

/**************************************
* Function define
**************************************/
#define SD_TIMEOUT_VALUE      1000 // 1 second
#define SD_DMA_TIMEOUT_VALUE  3000 // 3 second
#define BYTES_8          0
#define BYTES_16         1
#define BYTES_32         2
#define MIU_BURST_LEN    BYTES_8

/**************************************
* Structure define
**************************************/

typedef union _CMD_RSP
{
    U8 Command[16];
    U8 Response[16];
} CMD_RSP;

typedef enum
{
    CMD_TYPE_BC     = 0, // broadcast command
    CMD_TYPE_BCR    = 1, // broadcast command with response
    CMD_TYPE_AC     = 2, // addressed command
    CMD_TYPE_ADTC   = 3, // addressed data transfer command
} E_CMD_TYPE;

typedef enum
{
    RSP_TYPE_NO  = 0,
    RSP_TYPE_R1  = 1,
    RSP_TYPE_R1b = 2,
    RSP_TYPE_R2  = 3,
    RSP_TYPE_R3  = 4,
    RSP_TYPE_R6  = 5,
    RSP_TYPE_R7  = 6, // CMD08 send if cond
} E_RSP_TYPE;

typedef enum
{
    DAT_READ     = 0,
    DAT_WRITE    = 1,
} E_DAT_DIR;

typedef struct _CMD_RSP_INFO
{
    U32         CardClock;
    CMD_RSP     CmdRsp;
    U8          CmdSize;
    U8          RspSize;
    E_DAT_DIR   Dir;
    E_CMD_TYPE  CmdType;
    E_RSP_TYPE  RspType;
    U16         DataLength;
} CMD_RSP_INFO;

typedef enum
{
    DIR_R = 0,
    DIR_W = 1,
} E_RWDIR;

typedef enum
{
    DEST_MIU    = 0,
    DEST_CIFD   = 1,
} E_DEST;

typedef struct _RW_INFO
{
    U32     CardClock;
    CMD_RSP CmdRsp;
    U32     MiuAddr;
    U16     JobCount;
    E_RWDIR RwDir;
    E_DEST  Dest;
} RW_INFO;

typedef enum
{
    BUS_1_BIT   = 0,
    BUS_4_BITS  = 1,
    BUS_8_BITS  = 2,
} E_DATA_BUS;

U8 halFCIE_CheckRsp(void);
void halFCIE_Init(void);
void halFCIE_DumpRegister(void);
void halFCIE_OpenClock(U8 On_off);
void halFCIE_EnableCardInf(U8 u8MIE_PATH);

void halFCIE_Platform_Reset_Fcie(void);

E_IO_STS halFCIE_SDWaitD0High(U32);
E_IO_STS halFCIE_SendCmdEx(CMD_RSP_INFO * pCmdRspInfo, U8 *rsp);
E_IO_STS halFCIE_TransferData(E_DAT_DIR eDir);
E_IO_STS halFCIE_SetBusWidth(U8 * pu8BusWidth, E_DATA_BUS DataBus);

void halFCIE_Disable_Stop_Clock(void);
void halFCIE_Reset_Toggle_Count(void);
void halFCIE_Set_Read_Toggle_Count(void);
void halFCIE_Set_Write_Toggle_Count(void);
void halFCIE_Config_DDR_Mode(void);

#endif
