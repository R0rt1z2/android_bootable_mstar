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

#ifndef __DRV_URSA_6M30_H__
#define __DRV_URSA_6M30_H__

#ifdef __DRV_URSA_6M30_C__
#define URSA_INTERFACE
#else
#define URSA_INTERFACE extern
#endif

#include <common.h>
#include <MsTypes.h>
#include <ShareType.h>
#if(CONFIG_MSTARI2C == 1)
#include <apiSWI2C.h>
#endif


#define  IIC_MODE  EN_IIC_DDC2BI_MODE

#define BOOLEAN MS_BOOL
#define MAPI_BOOL MS_BOOL
#define MAPI_U8 U8
#define MAPI_U16 U16
#define MAPI_TRUE TRUE
#define MAPI_U32 U32


#define  MFC_CMD_DATA_SUM   7
#define URSA_6M30_DDC2BI_MODE_ADDR		0x40
#define URSA_6M30_STD_MODE_ADDR 		   0xB4


typedef enum
{
    PANEL_3D_MODE_HALF_SIDE_BY_SIDE = 0,
    PANEL_3D_MODE_TOP_AND_BOTTOM,
    PANEL_3D_MODE_FRAME_INTERLEAVED,
    PANEL_3D_MODE_PIXEL_INTERLEAVED,
    PANEL_3D_MODE_LINE_INTERLEAVED,
    PANEL_3D_MODE_CHECKERBOARD,
    PANEL_3D_MODE_NONE,
    PANEL_3D_MODE_LONLY,
    PANEL_3D_MODE_LREXCHANGE,
    PANEL_3D_MODE_MAXNUM
} PANEL_3D_MODE;//command 0x301

typedef enum
{
    PANEL_3D_MODE_2D_TO_3D_ON,
    PANEL_3D_MODE_2D_TO_3D_OFF
} PANEL_2D_TO_3D_MODE;//command 0x30A


typedef enum
{
    CHANNEL_SWAP_MODE1 = 0,
    CHANNEL_SWAP_MODE2,
    CHANNEL_SWAP_MODE3,
    CHANNEL_SWAP_MODE4,
    CHANNEL_SWAP_MODE5,
    CHANNEL_SWAP_MODE6,
    CHANNEL_SWAP_MODE7,
    CHANNEL_SWAP_MODE8,
    CHANNEL_SWAP_MODE9,
    CHANNEL_SWAP_MODE10,
    CHANNEL_SWAP_MODE11,
    CHANNEL_SWAP_MODE12,
    CHANNEL_SWAP_MODE13,
    CHANNEL_SWAP_MODE14,
    CHANNEL_SWAP_MODE15,
    CHANNEL_SWAP_MODE16,
    CHANNEL_SWAP_MODE17,
    CHANNEL_SWAP_MODE18,
    CHANNEL_SWAP_MODE19,
    CHANNEL_SWAP_MODE20,
    CHANNEL_SWAP_MODE21,
    CHANNEL_SWAP_MODE22,
    CHANNEL_SWAP_MODE23,
    CHANNEL_SWAP_MODE24,
    CHANNEL_SWAP_MODEDETAULT = CHANNEL_SWAP_MODE1,
} CHANNEL_SWAP_MODE;//command 0x202

typedef enum
{
    INPUT_DUAL_CHANNEL = 0,
    INPUT_SINGLE_CHANNEL,
    INPUT_JEIDA_MODE,
    INPUT_VESA_MODE,
    INPUT_SIXBITNUMS,
    INPUT_EIGHTBITNUMS,
    INPUT_TENBITNUMS
} SET_INPUT_LVDS_MODE; //command 0x101

typedef enum
{
    INPUT_SWAP_EVENODD_ON = 0,
    INPUT_SWAP_EVENODD_OFF,
    INPUT_SWAP_PN_ON,
    INPUT_SWAP_PN_OFF,
    INPUT_SWAP_MSBLSB_ON,
    INPUT_SWAP_MSBLSB_OFF
} SET_INPUT_LVDS_SWAP; //command 0x102

typedef enum
{
    OUTPUT_SIXBITNUMS = 0,
    OUTPUT_EIGHTBITNUMS,
    OUTPUT_TENBITNUMS,
    OUTPUT_JEIDA_MODE,
    OUTPUT_VESA_MODE,
    OUTPUT_LVDS_SWAPPOL_ON,
    OUTPUT_LVDS_SWAPPOL_OFF,
    OUTPUT_LVDS_SHIFTPAIR_ON,
    OUTPUT_LVDS_SHIFTPAIR_OFF,
    OUTPUT_LVDS_SWAPPAIR_ON,
    OUTPUT_LVDS_SWAPPAIR_OFF
} SET_OUTPUT_LVDS_MODE; //command 0x201

typedef enum
{
    SWING_LEVLE_100 = 0,
    SWING_LEVLE_200,
    SWING_LEVLE_300,
    SWING_LEVLE_400,
    SWING_LEVLE_500,
    SWING_LEVLE_600,
    SWING_LEVLE_700
} SWING_LEVEL; //command 0x204

typedef enum
{
    MFC_OFF = 0,
    MFC_LOW,
    MFC_MIDDLE,
    MFC_HIGH
} MFC_LEVEL_MODE;//command 0x302

typedef enum
{
    VIDEO_OUTPUT_UNMUTE = 0,
    VIDEO_OUTPUT_MUTE,
    VIDEO_OUTPUT_DEFAULT = VIDEO_OUTPUT_UNMUTE,
} VIDEO_OUTPUT_MODE;//command 0x303

typedef enum
{
    VMIRROR_ON = 0,
    VMIRROR_OFF,
    HMIRROR_ON,
    HMIRROR_OFF
} HVMIRROR_MODE;//command 0x30C

typedef enum
{
    FRAMELOCK_OFF = 0,
    FRAMELOCK_ON
} FRAMELOCK_MODE;//command 0x30D

typedef enum
{
    IIC_DDC2BI_MODE = 0,
    IIC_STD_MODE,
    IIC_DEFAULT = IIC_DDC2BI_MODE,
} EN_6M30_IIC_MODE;

typedef struct
{
    MAPI_U8    CMDIdx0;    //0st    [3:0]->CmdIdx[11:8],[7:4]->Checksum;
    MAPI_U8    CMDIdx1;    // 1st    [7:0]->CmdIdx[7:0]
    MAPI_U8    CMDIdx2;    // 2nd   CMD data
    MAPI_U8    CMDIdx3;    // 3rd   CMD data
    MAPI_U8    CMDIdx4;    // 4th   CMD data
    MAPI_U8    CMDIdx5;    // 5th   CMD data
    MAPI_U8    CMDIdx6;    // 6th   CMD data
} URSA_CMD_DATA;

typedef struct
{
    U8    bsingleCh: 1;
    U8    bTIMode: 1;
    U8    BitNums: 2;
    U8    bSwap_evenodd: 1;
    U8    bSwap_pn: 1;
    U8    bSwap_msblsb: 1;
    U8    bTTL: 1;
} URSA_CMD_INIT_IN_LVDSALL_DATA;

typedef struct
{
    U8    bTIMode: 1;
    U8    bShiftPair: 1;
    U8    bSwapPol: 1;
    U8    bSwapPair: 1;
    U8    PanelBitNums: 2;
    U8    bDither8Bits: 1;
    U8    bDitherEn: 1;
    U8    SwapCh;
    U8    PanelType: 4;
    U8    PanelChannel: 4;
} URSA_CMD_INIT_OUT_ALL_1_DATA;

typedef struct
{
    U16    VTotal;
    U16    De_XStart_high: 4;
    U16    HTotal: 12;
    U8      De_XStart_low;
} URSA_CMD_INIT_OUT_ALL_2_DATA;

typedef struct
{
    U16    PanelHeight;
    U16    De_YStart_high: 4;
    U16    PanelWidth: 12;
    U8      De_YStart_low;
} URSA_CMD_INIT_OUT_ALL_3_DATA;

typedef struct
{
    U8    Reserve: 3;
    U8    PanelBitNums: 2;
    U8    bTIMode: 1;
    U8    bLvdsSwapPol: 1;
    U8    bLvdsShiftPair: 1;
    U8    bLvdsSwapPair: 1;
} URSA_CMD_OUT_LVDS_MODE_DATA;

typedef struct
{
    U8    LvdsSwapCh;
} URSA_CMD_OUT_LVDS_SWAP_DATA;

typedef struct
{
    U8    Reserve: 4;
    U8    PanelChannel: 4;
} URSA_CMD_OUT_LVDS_CHANNELTYPE_DATA;

typedef struct
{
    U8    Reserve: 1;
    U8    CtrlSwing: 4;
    U8    Reserve1: 3;
} URSA_CMD_SWING_DATA;

typedef struct
{
    U8    Set3Dmode: 4;
    U8    Reserve: 3;
    U8    bL_Only: 1;
    U8    Reserve1: 7;
    U8    bLR_Exchange: 1;
} URSA_CMD_3D_MODE_DATA;

typedef struct
{
    U8    MFClevel;
} URSA_CMD_MFC_LEVEL_DATA;

typedef struct
{
    U8    Reserve: 7;
    U8    bMute: 1;
} URSA_CMD_VIDEO_MUTE_DATA;

typedef struct
{
    U8    AdjustIdx;
    U16  wAdjustValue;
} URSA_CMD_GAIN_OFFSET_DATA;

typedef struct
{
    U8    Reserve: 4;
    U8    WinID: 3;
    U8    bWinEn: 1;
    U16  WinHStart;
    U16  WinHEnd;
} URSA_CMD_OSD_WINDOWS_H_DATA;

typedef struct
{
    U8    Reserve: 4;
    U8    WinID: 3;
    U8    bWinEn: 1;
    U16  WinVStart;
    U16  WinVEnd;
} URSA_CMD_OSD_WINDOWS_V_DATA;

typedef struct
{
    U8    Reserve: 3;
    U8    PwmPolarity: 1;
    U8    PwmCH: 4;
    U16  PwmDuty;
    U16  PwmPhase;
} URSA_CMD_DUTY_PHASE_DATA;

typedef struct
{
    U8    Reserve: 3;
    U8    SscSel: 1; // 0 :MIU , 1: LVDS
    U8    Reserve1: 3;
    U8    SscEN: 1; //  0:disable, 1 enable
    U16   SscFreq;
    U16   SscPercent;
} URSA_CMD_FREQ_PERCENT_DATA;


typedef struct
{
    U8    Reserve: 7;
    U8    bOnOff: 1;
} URSA_CMD_MOD_POWER_DATA;

typedef struct
{
    U8    Reserve: 6;
    U8    bConcave: 1;
    U8    bEnable2Dto3D: 1;
    U8    Gain: 4;
    U8    ArtificalGain: 4;
    U8    Offset;
} URSA_CMD_2D_TO_3D_DATA;

typedef struct
{
    U8    Reserve: 6;
    U8    bVMirror: 1;
    U8    bHMirror: 1;
} URSA_CMD_HVMIRROR_DATA;

typedef struct
{
    U8    Reserve: 7;
    U8    bFrameLock: 1;
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

} URSA_CMD_GEN_SETTINGS;

typedef enum
{
    UC_INIT_IN_ALL,//= 0x0001

    UC_INIT_FINISH,//= 0x0100

    UC_SET_IN_LVDS_MODE,//= 0x0101

    UC_SET_IN_LVDS_SWAP,//= 0x0102

    UC_SET_IN_LVDS_TTL,//= 0x0103

    UC_INIT_OUT_ALL_1,//= 0x0010

    UC_INIT_OUT_ALL_2,//= 0x0011

    UC_INIT_OUT_ALL_3,//= 0x0012

    //UC_SET_OUT_TGEN_BASIC_1,//= 0x0110

    //UC_SET_OUT_TGEN_BASIC_2,//= 0x0111

    //UC_SET_OUT_TGEN_ADV,//= 0x0112

    UC_SET_OUT_LVDS_MODE,//= 0x0201

    UC_SET_OUT_LVDS_SWAP,//= 0x0202

    UC_SET_OUT_LVDS_CHANNELTYPE,//= 0x0203

    UC_SET_OUT_SWING_LELVEL,//= 0x0204
    //==3D mode CMD==

    UC_SET_3DMODE,// = 0x3010, //No 3D; MEMC 240H



    //==MFC level CMD ==
    UC_SET_MFC_LEVEL,//  = 0x3020,


    //==video mute/on CMD ==
    UC_SET_VIDEO_MUTE,//= 0x3030,


    UC_SET_GAIN_OFFSET,//= 0x3040,
    UC_SET_OSD_WINDOWS_H,//= 0x3050,
    UC_SET_OSD_WINDOWS_V,//= 0x3060,


    UC_SET_PWM_DUTY_PHASE,//= 0x3070,


    UC_SET_LVDS_OFF,//= 0x3080,
    UC_SET_LVDS_ON, // 0x3081,
    UC_SET_SSC_FREQ_PERCENT, // 0x309,

    UC_SET_2D_TO_3D,//=0x30a0,

    UC_SET_HVMIRROR,//0x30c

    UC_SET_FRAMELOCK,//0x30d

    UC_SET_CMDTYPE_END, // EN_CMD Type index Max
} UC_CMD_TYPE;

URSA_INTERFACE BOOLEAN MDrv_Ursa_6M30_3D_Cmd ( UC_CMD_TYPE eCmdIdx ) ;
URSA_INTERFACE BOOLEAN MDrv_Ursa_6M30_LVDS_Enalbe ( BOOLEAN bEnable ) ;

URSA_INTERFACE URSA_CMD_GEN_SETTINGS g_UrsaCMDGenSetting;

URSA_INTERFACE MAPI_BOOL    bMst6m30Installed;

URSA_INTERFACE void  MDrv_Ursa_6M30_Initialize ( void );

URSA_INTERFACE MAPI_BOOL devMst6m30_Install ( void );

URSA_INTERFACE MAPI_BOOL MDrv_6M30_3D_DataWrite ( EN_6M30_IIC_MODE eIICMode, MAPI_U8 *u8Buf );

URSA_INTERFACE void MDrv_Ursa_6M30_Data_Init ( void );

URSA_INTERFACE void MDrv_Ursa_6M30_CMD_Init ( void );


URSA_INTERFACE void MDrv_6M30_SetInitInLvdsAllData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetInitOutAll1Data ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetInitOutAll2Data ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetInitOutAll3Data ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetOutLvdsModeData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetOutLvdsSwapData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetOutLvdsChannelTypeData ( MAPI_U8 *u8Buf );

URSA_INTERFACE void MDrv_6M30_SetOutLvdsSwingData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_Set3DModeData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetMFClevelData ( MAPI_U8 *u8Buf );



URSA_INTERFACE void MDrv_6M30_SetVideoMuteData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetGainOffsetData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetOSDWinHData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetOSDWinVData ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetPwmDutyPhaseData ( MAPI_U8 *u8Buf );
URSA_INTERFACE void MDrv_6M30_SetSSCFreqPercent ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_6M30_SetModPowerData ( MAPI_U8 *u8Buf );

URSA_INTERFACE void MDrv_6M30_Set2Dto3DMode ( MAPI_U8 *u8Buf );

URSA_INTERFACE void MDrv_6M30_SetHVMirrorMode ( MAPI_U8 *u8Buf );

URSA_INTERFACE void MDrv_6M30_SetFrameLockMode ( MAPI_U8 *u8Buf );


URSA_INTERFACE void MDrv_Ursa_6M30_CalCheckSum ( MAPI_U8 *pBuf, U8 u8BufLen );



//URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_3D_Cmd( UC_CMD_TYPE eCmdIdx);

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_3D_MODE ( PANEL_3D_MODE ePanel3DMode );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_2DTO3D_MODE ( PANEL_2D_TO_3D_MODE ePanel2DTO3DMode );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_LVDS_IN_MODE ( SET_INPUT_LVDS_MODE eInputLvdsMode );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_LVDS_IN_Swap ( SET_INPUT_LVDS_SWAP eInputLvdsSwap );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_LVDS_OUT_MODE ( SET_OUTPUT_LVDS_MODE eLvdsOutputMode );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_LVDS_OUT_Swing ( SWING_LEVEL eLvdsOutputSwing );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_LVDS_OUT_MFC ( MFC_LEVEL_MODE eLvdsOutputMFC );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_VideoOutput_Mode ( VIDEO_OUTPUT_MODE eVideoOutputMode );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_ChanelSwap_Mode ( CHANNEL_SWAP_MODE eChanelSwap );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_HVMirror_Mode ( HVMIRROR_MODE eHVMirrorMode );

URSA_INTERFACE MAPI_BOOL MDrv_Ursa_6M30_SET_FrameLock_Mode ( FRAMELOCK_MODE eFrameLockMode );


#endif
