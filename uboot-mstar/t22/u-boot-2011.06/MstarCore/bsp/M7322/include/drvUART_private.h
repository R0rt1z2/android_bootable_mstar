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
#ifndef _DRVUART_PRIV_H_
#define _DRVUART_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief UART control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "MsTypes.h"

typedef enum {
    MDrv_CMD_UART_Open,
    MDrv_CMD_UART_Close,
    MDrv_CMD_UART_Connect,
    MDrv_CMD_UART_SetBaudRate,
} eUartIoctlOpt;

void UARTRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 UARTOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 UARTClose(void* pInstance);
MS_U32 UARTIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

#if defined(CONFIG_UTOPIA_PROC_DBG_SUPPORT)
#define UART_CMD_STRING_LENGTH    20
#define UART_CMD_HELP_STRING "help"
#define UART_CMD_LOGLEVEL_STRING "LogLevel"

typedef enum
{
    UART_CMD_TYPE_HELP,
    UART_CMD_TYPE_LOGLEVEL,
    UART_CMD_TYPE_MAX
} UART_CMD_TYPE;

typedef struct
{
    UART_CMD_TYPE u8CmdType;
    MS_U32 u32SettingValue;
} UART_CMD_INFO;

void MDrv_UART_Atoi(char *s, MS_U32 *u32UartCmd);
void MDrv_UART_CmdParsing(MS_U64 *u64Hdl, char *s, UART_CMD_INFO *pstCmdInfo);
void MDrv_UART_CmdExecute(MS_U64 *u64Hdl, UART_CMD_INFO *pstCmdInfo);

void MDrv_UART_PrintType(MS_U64 *u64Hdl, UART_DEVICE_TYPE eType);

void MDrv_UART_Debug_Cmd(MS_U64 *u64Hdl, char *cmd);
void MDrv_UART_Debug_cat(MS_U64 *u64Hdl);
#endif

typedef MS_VIRT (*IOCTL_UART_OPEN)(UART_DEVICE_TYPE);
typedef MS_U16 (*IOCTL_UART_CLOSE)(MS_VIRT);
typedef MS_U16 (*IOCTL_UART_CONNECT)(UART_PORT_TYPE, UART_DEVICE_TYPE);
typedef MS_U16 (*IOCTL_UART_SET_BAUDRATE)(MS_VIRT, MS_U32);

typedef struct _UART_OPEN
{
    UART_DEVICE_TYPE eUartType;
}UART_OPEN, *PUART_OPEN;

typedef struct _UART_CLOSE
{
    MS_U32 u32Fd;
}UART_CLOSE, *PUART_CLOSE;

typedef struct _UART_CONNECT
{
    UART_PORT_TYPE ePortType;
    UART_DEVICE_TYPE eDeviceType;
}UART_CONNECT, *PUART_CONNECT;

typedef struct _UART_SET_BAUDRATE
{
    MS_U32 u32Uart;
    MS_U32 u32BaudRate;
}UART_SET_BUADRATE, *PUART_SET_BUAD_RATE;

typedef struct _UART_INSTANT_PRIVATE
{
    IOCTL_UART_OPEN                       fpUARTOpen;
    IOCTL_UART_CLOSE                      fpUARTClose;
    IOCTL_UART_CONNECT                    fpUARTConnect;
    IOCTL_UART_SET_BAUDRATE               fpUARTSetBaudRate;
}UART_INSTANT_PRIVATE;

typedef struct _UART_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}UART_RESOURCE_PRIVATE;

//instant
typedef struct _UART_INSTANT_ATTRIBUTE
{
    UART_DEVICE_TYPE  eUartType;
    MS_U32 u32BaudRate;
}UART_INSTANT_ATTRIBUTE;

#ifdef __cplusplus
}
#endif
#endif //_DRVUART_PRIV_H_

