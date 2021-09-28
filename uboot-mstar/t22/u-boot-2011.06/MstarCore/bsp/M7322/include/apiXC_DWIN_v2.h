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
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
#include "UFO.h"


#define ST_DIP_GET_CHIP_CAPS_VERSION       (1)

typedef enum {
    DIP_TRIG_OFF,
    DIP_TRIG_ON,
    DIP_TRIG_ONESHOT,
    DIP_TRIG_ONESHOTFAST,
    DIP_TRIG_RESET,
}eDipTriggerMode;

typedef enum {
    DIP_INTR_DISABLE,
    DIP_INTR_ENABLE,
    DIP_INTR_GET,
    DIP_INTR_CLEAR,
    DIP_INTR_ATTACH,
    DIP_INTR_DETACH,
    DIP_INTR_QUERY,
}eDipInterruptAct;

typedef enum {
    DIP_PRO_NONE        = 0x000000000,
    DIP_PRO_SCANMODE    = 0x000000001,
    DIP_PRO_DATAFMT     = 0x000000002,
    DIP_PRO_R2Y         = 0x000000004,
    DIP_PRO_Y2R         = 0x000000008,
    DIP_PRO_SWAPUV      = 0x000000010,
    DIP_PRO_SWAPYC      = 0x000000020,
    DIP_PRO_SWAPRGB     = 0x000000040,
    DIP_PRO_INTERLACE_W = 0x000000080,
    DIP_PRO_ALPHA       = 0x000000100,
    DIP_PRO_FRC         = 0x000000200,
    DIP_PRO_OP_CAPTURE  = 0x000000400,
    DIP_PRO_MIRROR      = 0x000000800,
    DIP_PRO_PINPON      = 0x000001000,
    DIP_PRO_ROTATION    = 0x000002000,
    DIP_PRO_HVSP        = 0x000004000,
    DIP_PRO_TILEBLOCK   = 0x000008000,
}eDipPropertyflag;

typedef enum {
    E_DIP_SET_DIPR_NORMAL,
    E_DIP_SET_DIPR_EX,
}EN_DIP_Diprflag;

typedef enum {
    MDrv_CMD_DIP_INIT,
    MDrv_CMD_DIP_GET_RESOURCE,
    MDrv_CMD_DIP_RELEASE_RESOURCE,
    MDrv_CMD_DIP_QUERY_RESOURCE,
    MDrv_CMD_DIP_SET_INPUTSOURCE,
    MDrv_CMD_DIP_SET_WINDOW,
    MDrv_CMD_DIP_SET_WINPROPERTY,
    MDrv_CMD_DIP_SET_PROPERTY,
    MDrv_CMD_DIP_GET_BUFINFO,
    MDrv_CMD_DIP_INTERRUPT,
#if defined(UFO_PUBLIC_HEADER_700)
    MDrv_CMD_DIP_INTERRUPT_CB,
#endif
    MDrv_CMD_DIP_TRIGGER,
    MDrv_CMD_DIP_SET_DIPR_PROPERTY,
    MDrv_CMD_DIP_CMDQ_SETACTION,
    MDrv_CMD_DIP_GET_LATEST_FRAME,
    MDrv_CMD_DIP_SET_SRC_INFO,
    MDrv_CMD_DIP_GET_CHIPCAPS,
    MDrv_CMD_DIP_SET_DBG_LEVEL = 0x100,
    MDrv_CMD_DIP_GET_RES_BY_PIPE = 0x200,
    MDrv_CMD_DIP_CONFIG_PIPE = 0x201,
    MDrv_CMD_DIP_CHECK_PIPE = 0x202,
    MDrv_CMD_DIP_SET_CONFIG = 0x300,
    MDrv_CMD_DIP_SET_POWERSTATE = 0x400,
} eDipIoctlOpt;

typedef struct DLL_PACKED
{
    MS_BOOL bFrameRateCtrl;
    MS_U16 u16In;
    MS_U16 u16Out;
} ST_XC_DIP_FRC;

typedef struct DLL_PACKED
{
    MS_BOOL bSwap;
    EN_XC_DIP_RGB_SWAP_TYPE eSwapType;
} ST_XC_DIP_SWAPRGB;

typedef struct DLL_PACKED
{
    MS_BOOL bOpCapture;
    EN_XC_DIP_OP_CAPTURE eOpCapture;
} ST_XC_DIP_OP_CAPTURE;

typedef struct DLL_PACKED
{
    MS_BOOL bHMirror;
    MS_BOOL bVMirror;
} ST_XC_DIP_MIRROR;

typedef struct DLL_PACKED
{
    MS_BOOL bPinpon;
    MS_PHY u32PinponAddr;
} ST_XC_DIP_PINPON;

typedef struct DLL_PACKED
{
    MS_BOOL bRotation;
    EN_XC_DIP_ROTATION eRoDirection;
} ST_XC_DIP_ROTATION;

typedef struct DLL_PACKED
{
    EN_XC_DIP_SCAN_MODE eScanMode;
    EN_XC_DIP_DATA_FMT eDataFmt;
    ST_XC_DIP_OP_CAPTURE stOpCapture;
    ST_XC_DIP_SWAPRGB stSwapRGB;
    ST_XC_DIP_MIRROR stMirror;
    ST_XC_DIP_FRC stFrameRateControl;
    ST_XC_DIP_PINPON stPinpon;
    ST_XC_DIP_ROTATION stRotation;
    MS_BOOL bSwapUV;
    MS_BOOL bSwapYC;
    MS_BOOL bR2Y;
    MS_BOOL bY2R;
    MS_BOOL bInterlaceW;
    MS_U8 u8Alpha;
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
    MS_BOOL bHVSP;
#endif
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)|| defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
    EN_XC_DIP_TILE_BLOCK eTileBlock;
#endif
}ST_XC_DIP_PROPERTY;

typedef struct DLL_PACKED _DIP_RESOURCE
{
    SCALER_DIP_WIN eWindow;
}DIP_RESOURCE;

typedef struct DLL_PACKED _DIP_INPUT_SOURCE
{
    INPUT_SOURCE_TYPE_t SrcPath;
}DIP_INPUT_SOURCE;

typedef struct _DIP_SET_SRC_INFO_WINDOW
{
    ST_DIP_SET_SOURCE_INFO stDIPSetSrcInfo;
    SCALER_DIP_WIN eWindow;
}DIP_SET_SRC_INFO_WINDOW;

typedef struct DLL_PACKED _DIP_SET_WINDOW
{
    MS_U32 u32DataLen;
    XC_SETWIN_INFO WinInfo;
    SCALER_DIP_WIN eWindow;
}DIP_SET_WINDOW;

typedef struct DLL_PACKED _DIP_SET_PROPERTY
{
    eDipPropertyflag eFlag;
    ST_XC_DIP_PROPERTY Property;
    SCALER_DIP_WIN eWindow;
}DIP_SET_PROPERTY;

typedef struct DLL_PACKED _DIP_SET_WIN_PROPERTY
{
    ST_XC_DIP_WINPROPERTY WinProperty;
    SCALER_DIP_WIN eWindow;
}DIP_SET_WIN_PROPERTY;

typedef struct DLL_PACKED _DIP_BUF_INFO
{
    BUFFER_INFO BufInfo;
    SCALER_DIP_WIN eWindow;
}DIP_BUF_INFO;

typedef struct DLL_PACKED _DIP_INTER_STUS
{
    eDipInterruptAct eFlag;
    MS_U16 IntrStautus;
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500)
    InterruptCb pIntCb;
#endif
    SCALER_DIP_WIN eWindow;
}DIP_INTER_STUS;

typedef struct DLL_PACKED _DIP_LATEST_FRAME_INFO
{
    MS_U16 u16Frame;
    SCALER_DIP_WIN eWindow;
} DIP_LATEST_FRAME_INFO;

typedef struct DLL_PACKED _DIP_INTER_CB
{
    eDipInterruptAct eFlag;
    InterruptCb pIntCb;
	SCALER_DIP_WIN eWindow;
}DIP_INTER_CB;

typedef struct DLL_PACKED _DIP_TRIGGER_MODE
{
    eDipTriggerMode eTrigMode;
	SCALER_DIP_WIN eWindow;
}DIP_TRIGGER_MODE;

typedef struct DLL_PACKED _DIP_SET_DIPR_PROPERTY
{
    ST_XC_DIPR_PROPERTY DIPRProperty;
	SCALER_DIP_WIN eWindow;
}DIP_SET_DIPR_PROPERTY;

typedef struct DLL_PACKED _DIP_SET_DIPR_PROPERTY_EX
{
    ST_XC_DIPR_PROPERTY_EX stDIPRProperty_ex;
    EN_DIP_Diprflag enDIPRFlag;
    SCALER_DIP_WIN eWindow;
}DIP_SET_DIPR_PROPERTY_EX;

typedef struct DLL_PACKED _DIP_CMDQ_SET_ACTION
{
    SCALER_DIP_WIN eWindow;
    EN_XC_DIP_CMDQ_ACTION eAction;
}DIP_CMDQ_SET_ACTION;

typedef struct DLL_PACKED _DIP_SETCONFIG_PARAM
{
    EN_DIP_CONFIG_TYPE cfg_type;
    void * pCfg;
    SCALER_DIP_WIN eWindow;
}DIP_SETCONFIG_PARAM;

typedef struct DLL_PACKED _DIP_SETPOWERSTATE
{
    EN_POWER_MODE enPowerState;
}DIP_SETPOWERSTATE;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    EN_DIP_CHIPCAPS enDIPCaps;
    SCALER_DIP_WIN eWindow;
    MS_U32*   pRet;
#if !defined (__aarch64__)
    MS_U32    u32AlignDummy;
#endif
} ST_DIP_GET_CHIP_CAPS;
