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

#ifndef __PANEL_INX_VB1_H__
#define __PANEL_INX_VB1_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>

//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
typedef enum
{
    CMD_INX_VB1_SET_PRE_INIT,
    CMD_INX_VB1_SET_FHD,
    CMD_INX_VB1_SET_4K2K,
    CMD_INX_VB1_SET_2D,
    CMD_INX_VB1_SET_3D,
    CMD_INX_VB1_RX_INIT
}PANEL_INX_VB1_CMD_TYPE;

typedef struct
{
    MS_U8 u8Cmd0;
    MS_U8 u8Cmd1;
    MS_U8 u8Cmd2;
    MS_U8 u8Cmd3;
    MS_U8 u8Cmd4;
    MS_U8 u8Cmd5;
    MS_U8 u8Cmd6;
}PANEL_CMD_DATA;

//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_INX_VB1_SWI2C_Init(void);
MS_BOOL MDrv_Panel_INX_VB1_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_VB1_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_VB1_SendCmd(PANEL_INX_VB1_CMD_TYPE eCmdType);
MS_BOOL MDrv_Panel_INX_VB1_Write_Cmd(MS_U8 *pu8CmdData);
MS_BOOL MDrv_Panel_INX_VB1_Print_Cmd(MS_U8 *pu8CmdData);

MS_BOOL MDrv_Panel_INX_VB1_Set_Pre_Init(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_FHD(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_4K2K(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_2D(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_3D(void);
MS_BOOL MDrv_Panel_INX_VB1_RX_INIT(void);
#endif //__PANEL_INX_VB1_H__

