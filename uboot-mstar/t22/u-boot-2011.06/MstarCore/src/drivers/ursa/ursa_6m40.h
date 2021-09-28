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

#ifndef __DRV_URSA_6M40_H__
#define __DRV_URSA_6M40_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>
#include <apiPNL.h>
//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8    bsingleCh: 1;
    MS_U8    bTIMode: 1;
    MS_U8    BitNums: 2;
    MS_U8    bSwap_evenodd: 1;
    MS_U8    bSwap_pn: 1;
    MS_U8    bSwap_msblsb: 1;
    MS_U8    bTTL: 1;
} URSA_CMD_INIT_IN_LVDSALL_DATA;

typedef struct
{
    MS_U8    bTIMode: 1;
    MS_U8    bShiftPair: 1;
    MS_U8    bSwapPol: 1;
    MS_U8    bSwapPair: 1;
    MS_U8    PanelBitNums: 2;
    MS_U8    bDither8Bits: 1;
    MS_U8    bDitherEn: 1;
    MS_U8    SwapCh;
    MS_U8    PanelType: 4;
    MS_U8    PanelChannel: 4;
} URSA_CMD_INIT_OUT_ALL_1_DATA;

typedef struct
{
    MS_U16    VTotal;
    MS_U16    De_XStart_high: 4;
    MS_U16    HTotal: 12;
    MS_U8      De_XStart_low;
} URSA_CMD_INIT_OUT_ALL_2_DATA;

typedef struct
{
    MS_U16    PanelHeight;
    MS_U16    De_YStart_high: 4;
    MS_U16    PanelWidth: 12;
    MS_U8      De_YStart_low;
} URSA_CMD_INIT_OUT_ALL_3_DATA;

typedef struct
{
    MS_U8    Reserve: 3;
    MS_U8    PanelBitNums: 2;
    MS_U8    bTIMode: 1;
    MS_U8    bLvdsSwapPol: 1;
    MS_U8    bLvdsShiftPair: 1;
    MS_U8    bLvdsSwapPair: 1;
} URSA_CMD_OUT_LVDS_MODE_DATA;

typedef struct
{
    MS_U8    LvdsSwapCh;
} URSA_CMD_OUT_LVDS_SWAP_DATA;

typedef struct
{
    MS_U8    Reserve: 4;
    MS_U8    PanelChannel: 4;
} URSA_CMD_OUT_LVDS_CHANNELTYPE_DATA;

typedef struct
{
    MS_U8    Reserve: 1;
    MS_U8    CtrlSwing: 4;
    MS_U8    Reserve1: 3;
} URSA_CMD_SWING_DATA;

typedef struct
{
    MS_U8    Set3Dmode: 4;
    MS_U8    Reserve: 3;
    MS_U8    bL_Only: 1;
    MS_U8    Reserve1: 7;
    MS_U8    bLR_Exchange: 1;
} URSA_CMD_3D_MODE_DATA;

typedef struct
{
    MS_U8    MFClevel;
} URSA_CMD_MFC_LEVEL_DATA;

typedef struct
{
    MS_U8    Reserve: 7;
    MS_U8    bMute: 1;
} URSA_CMD_VIDEO_MUTE_DATA;

typedef struct
{
    MS_U8    AdjustIdx;
    MS_U16  wAdjustValue;
} URSA_CMD_GAIN_OFFSET_DATA;

typedef struct
{
    MS_U8    Reserve: 4;
    MS_U8    WinID: 3;
    MS_U8    bWinEn: 1;
    MS_U16  WinHStart;
    MS_U16  WinHEnd;
} URSA_CMD_OSD_WINDOWS_H_DATA;
typedef struct
{
    MS_U16  protect_mode;
} URSA_CMD_OSD_PROTECT_MOdE;

typedef struct
{
    MS_U8    Reserve: 4;
    MS_U8    WinID: 3;
    MS_U8    bWinEn: 1;
    MS_U16  WinVStart;
    MS_U16  WinVEnd;
} URSA_CMD_OSD_WINDOWS_V_DATA;

typedef struct
{
    MS_U8    Reserve: 3;
    MS_U8    PwmPolarity: 1;
    MS_U8    PwmCH: 4;
    MS_U16  PwmDuty;
    MS_U16  PwmPhase;
} URSA_CMD_DUTY_PHASE_DATA;

typedef struct
{
    MS_U8    Reserve: 3;
    MS_U8    SscSel: 1; // 0 :MIU , 1: LVDS
    MS_U8    Reserve1: 3;
    MS_U8    SscEN: 1; //  0:disable, 1 enable
    MS_U16   SscFreq;
    MS_U16   SscPercent;
} URSA_CMD_FREQ_PERCENT_DATA;


typedef struct
{
    MS_U8    Reserve: 7;
    MS_U8    bOnOff: 1;
} URSA_CMD_MOD_POWER_DATA;

typedef struct
{
    MS_U8    Reserve: 6;
    MS_U8    bConcave: 1;
    MS_U8    bEnable2Dto3D: 1;
    MS_U8    Gain: 4;
    MS_U8    ArtificalGain: 4;
    MS_U8    Offset;
} URSA_CMD_2D_TO_3D_DATA;

typedef struct
{
    MS_U8    Reserve: 6;
    MS_U8    bVMirror: 1;
    MS_U8    bHMirror: 1;
} URSA_CMD_HVMIRROR_DATA;

typedef struct
{
    MS_U8    Reserve: 7;
    MS_U8    bFrameLock: 1;
} URSA_CMD_FRAMELOCK_DATA;


typedef struct
{
    URSA_CMD_INIT_IN_LVDSALL_DATA   g_InitInLvdsAll;
    URSA_CMD_INIT_OUT_ALL_1_DATA   g_InitOutAll1;
    URSA_CMD_INIT_OUT_ALL_2_DATA   g_InitOutAll2;
    URSA_CMD_INIT_OUT_ALL_3_DATA   g_InitOutAll3;
    URSA_CMD_OUT_LVDS_MODE_DATA   g_OutLvdsMode;
    URSA_CMD_OUT_LVDS_SWAP_DATA   g_OutLvdsSwap;
    URSA_CMD_OUT_LVDS_CHANNELTYPE_DATA   g_OutLvdsChannelType;
    URSA_CMD_SWING_DATA    g_CtrlSwing;
    URSA_CMD_3D_MODE_DATA   g_3DMode;
    URSA_CMD_MFC_LEVEL_DATA   g_MFCLevel;
    URSA_CMD_VIDEO_MUTE_DATA   g_VideoMute;
    URSA_CMD_GAIN_OFFSET_DATA   g_GainOffset;
    URSA_CMD_OSD_WINDOWS_H_DATA   g_OSDWinH;
    URSA_CMD_OSD_WINDOWS_V_DATA   g_OSDWinV;
    URSA_CMD_DUTY_PHASE_DATA   g_PwmDutyPhase;
    URSA_CMD_FREQ_PERCENT_DATA   g_SscFreqPercent;
    URSA_CMD_MOD_POWER_DATA   g_ModPower;
    URSA_CMD_2D_TO_3D_DATA   g_2Dto3D;
    URSA_CMD_HVMIRROR_DATA  g_HVMirror;
    URSA_CMD_FRAMELOCK_DATA g_FrameLock;
	URSA_CMD_OSD_PROTECT_MOdE g_OsdMode;

} URSA_CMD_GEN_SETTINGS;

extern URSA_CMD_GEN_SETTINGS g_UrsaCMDGenSetting;

typedef enum
{
    CMD_6M40_OSD_MUTE,
    CMD_6M40_OSD_ON,
    CMD_6M40_OSD_OFF,
    CMD_6M40_OSD_UNMUTE,
    CMD_6M40_LVDS_ON,
    CMD_6M40_LVDS_OFF,
    CMD_6M40_2K_MODE_ON,
    CMD_6M40_4K_MODE_ON,
    //sword add
    CMD_6M40_OSD_PROTECT_OFF,
    CMD_6M40_OSD_PROTECT_ON,
    CMD_6M40_OSD_PROTECT_ON_EMMC
}URSA_6M40_CMD_TYPE;

typedef enum
{
	URSA_6M40_OSD_PROTECT_OFF  ,
	URSA_6M40_OSD_PROTECT_ON  ,
	URSA_6M40_OSD_PROTECT_ON_EMMC
}URSA_6M40_OSD_PROTECT_MODE;

typedef struct
{
    MS_U8 u8Cmd0;
    MS_U8 u8Cmd1;
    MS_U8 u8Cmd2;
    MS_U8 u8Cmd3;
    MS_U8 u8Cmd4;
    MS_U8 u8Cmd5;
    MS_U8 u8Cmd6;
}URSA_6M40_CMD_DATA;


//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Ursa_6M40_SWI2C_Init(void);
MS_BOOL MDrv_Ursa_6M40_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Ursa_6M40_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
void MDrv_Ursa_6M40_Cmd_CheckSum(MS_U8 *pCmd, MS_U8 u8CmdLen);
MS_BOOL MDrv_Ursa_6M40_SendCmd(URSA_6M40_CMD_TYPE eCmdType);
MS_BOOL MDrv_Ursa_6M40_Write_Cmd(MS_U8 *pu8CmdData);
MS_BOOL MDrv_Ursa_6M40_Write_Cmd_Array(URSA_6M40_CMD_DATA *pCmdArray, MS_U8 u8ArraySize);

MS_BOOL MDrv_Ursa_6M40_Set_Osd_On(void);
MS_BOOL MDrv_Ursa_6M40_Set_Osd_Off(void);
MS_BOOL MDrv_Ursa_6M40_Set_Osd_Mute(void);
MS_BOOL MDrv_Ursa_6M40_Set_Osd_Unmute(void);
MS_BOOL MDrv_Ursa_6M40_Set_Lvds_On(void);
MS_BOOL MDrv_Ursa_6M40_Set_Lvds_Off(void);
MS_BOOL MDrv_Ursa_6M40_Set_2K_Mode_On(void);
MS_BOOL MDrv_Ursa_6M40_Set_VB1_Init(APIPNL_LINK_EXT_TYPE eLinkExtType);


MS_U8 MDrv_Ursa_6M40__ReadChipJEDEC_ID(MS_U8 *ReadIDBuf);
MS_U8 MDrv_Ursa_6M40_ReadChipID(void);
MS_U8 MDrv_Ursa_6M40_ReadChipIDBuffer(MS_U8 *ReadIDBuf);
void MDrv_Ursa_6M40_SWI2C_WriteDatas(MS_U8 * paddr, MS_U8 addrSize, MS_U8  * pdatabuf, MS_U8 dataSize);
void MDrv_Ursa_6M40_SWI2C_WriteStop(void);
void MDrv_Ursa_6M40_SWI2C_ReadDatas(MS_U8  * paddr, MS_U8 addrSize, MS_U8 * pdatabuf, MS_U8 dataSize);
void MDrv_Ursa_6M40_SWI2C_CommandRead(void);

MS_BOOL MDrv_Ursa_6M40_EnterIspMode(void);
MS_BOOL MDrv_Ursa_6M40_ExitIspMode(void);
void MDrv_Ursa_6M40_WP_PullHigh(void);

#endif //__DRV_URSA_6M40_H__

