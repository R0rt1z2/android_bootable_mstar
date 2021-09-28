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

#ifndef __PANEL_INX_NOVA_VB1_H__
#define __PANEL_INX_NOVA_VB1_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>

//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 Idx0;
    MS_U8 Idx1;
    MS_U8 Idx2;
    MS_U8 Idx3;
    MS_U8 Idx4;
    MS_U8 Idx5;
    MS_U8 Idx6;
    MS_U8 Idx7;
    MS_U8 Idx8;
}PANEL_INX_NOVA_VB1_CMD_DATA;

typedef enum
{
    CMD_INX_NOVA_VB1_UNLOCK_AHB = 0,
    CMD_INX_NOVA_VB1_SET_UHD_DIVISION_MODE,
    CMD_INX_NOVA_VB1_SET_INPUT_MODE,
    CMD_INX_NOVA_VB1_SET_3D_MODE
}PANEL_INX_NOVA_VB1_CMD_TYPE;

typedef enum
{
    INPUT_FHD_120 = 0,  //FHD@100/120(4LAN,0~3)
    INPUT_FHD_60,       //FHD@50/60(2LAN 0~1)
    INPUT_UHD_60,       //QFHD@50/60(8LAN)
    INPUT_UHD_30        //QFHD@24/30(4LAN 0~3)
}INPUT_TIMMING_TYPE;

typedef enum
{
    INPUT_3D_NONE = 0,  //2D Mode
    INPUT_3D_FS,        //Frame Sequential
    INPUT_3D_TB,        //Top & Bottom
    INPUT_3D_SBS        //Side by Side
}INPUT_3D_TYPE;

typedef enum
{
    UHD_0_DIVISION = 0,
    UHD_1_DIVISION,
    UHD_2_DIVISION
}UHD_DIVISION_TYPE;
//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_INX_NOVA_VB1_SWI2C_Init(void);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_SendCmd(PANEL_INX_NOVA_VB1_CMD_TYPE eCmdType, MS_U8 u8Data);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Write_Cmd(MS_U8 *pu8CmdData, MS_U8 u8CmdLen);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Read_Data(MS_U8 *pu8CmdData, MS_U8 u8DataLen);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Print_Cmd(MS_BOOL bWrite, MS_U8 *pu8CmdData, MS_U8 u8CmdLen);

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Run_Cmd(PANEL_INX_NOVA_VB1_CMD_DATA *pCmdArray, MS_U8 u8ArraySize);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Check_Cmd(void);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Check_Status(void);

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Unlock_AHB(void);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_UHD_DIVISION(UHD_DIVISION_TYPE eDivisionType);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_INPUT_MODE(INPUT_TIMMING_TYPE eTimmingType);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_3D_MODE(INPUT_3D_TYPE e3DType);

#endif //__PANEL_INX_NOVA_VB1_H__

