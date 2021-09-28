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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMS_V2_H_
#define _DMS_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// DMS CMD
typedef enum
{
    //DMS command
    E_DMS_CMD_INIT,
    E_DMS_CMD_SETMEMORY,
    E_DMS_CMD_CREATEWINDOW,
    E_DMS_CMD_SETDIGITALDECODESIGNALINFO,
    E_DMS_CMD_CLEARDIGITALDECODESIGNALINFO,
    E_DMS_CMD_DESTORYWINDOW,
    E_DMS_CMD_SETWINDOW,
    E_DMS_CMD_GETWINDOWINFO,
    E_DMS_CMD_VIDEOFLIP,
    E_DMS_CMD_SETZORDER,
    E_DMS_CMD_VIDEOMUTE,
    E_DMS_CMD_SETMUTECOLOR,
    E_DMS_CMD_VIDEOFREEZE,
    E_DMS_CMD_GETSTATUS,
    E_DMS_CMD_GETCAPS,
    E_DMS_CMD_GETEXISTEDWINDOW,
    E_DMS_CMD_SETOUTPUTLAYER,
    E_DMS_CMD_SET3DMODE,
    E_DMS_CMD_SETCAPTUREINFO,
    E_DMS_CMD_GETCAPTUREBUFFER,
    E_DMS_CMD_RELEASECAPTUREBUFFER,
    E_DMS_CMD_SECURITYCONTROL,
    E_DMS_CMD_SETMETADATA,
#ifndef UFO_DMS_OLD_VERSION
    E_DMS_CMD_DYNAMICSCALING,
    E_DMS_CMD_GETWINDOWID,
    E_DMS_CMD_REGISTERFRAMERELEASEFUN,
    E_DMS_CMD_REGISTERFDISPLAYINFOFUN,
    E_DMS_CMD_REGISTERCALLBACKFUN,
    E_DMS_CMD_ISSUPPORTCALLBACK,
    E_DMS_CMD_GETVIDEODELAYTIME,
#endif
    E_DMS_CMD_NUM,
    E_DMS_CMD_MAX = E_DMS_CMD_NUM,
}EN_DMS_IOCTL_CMDS;

typedef struct DLL_PACKED
{
    ST_DMS_INITDATA *pstDMS_InitData;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_INIT,*PST_DMS_INIT;

typedef struct DLL_PACKED
{
    ST_DMS_SET_MEMORY_TYPE *pstDMS_SetMemType;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETMEMTYPE,*PST_DMS_SETMEMTYPE;

typedef struct DLL_PACKED
{
    ST_DMS_WINDOW* pstOutputWin;
    ST_DMS_CREATE_WIN_INFO* pstCreateWin_Info;
    MS_U32* pu32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_CREATEWIN,*PST_DMS_CREATEWIN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETDIGIDECINFO,*PST_DMS_SETDIGIDECINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_CLEARDIGIDECINFO,*PST_DMS_CLEARDIGIDECINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_DESTORYWIN,*PST_DMS_DESTORYWIN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETWIN,*PST_DMS_SETWIN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETWININFO,*PST_DMS_GETWININFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_FLIP,*PST_DMS_FLIP;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_ZORDER_INFO *pstZorderInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_ZORDER,*PST_DMS_ZORDER;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_FREEZE,*PST_DMS_FREEZE;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_MUTE_WINDOW_INFO *pstWindowMute;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_MUTE,*PST_DMS_MUTE;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_MUTE_COLOR_INFO *pstMuteColor;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_MUTECOLOR,*PST_DMS_MUTECOLOR;

typedef struct DLL_PACKED
{
    EN_DMS_STATUS *peStatus;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETSTATUS,*PST_DMS_GETSTATUS;

typedef struct DLL_PACKED
{
    EN_DMS_CAPABILITY *peCapability;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETCAPS,*PST_DMS_GETCAPS;

typedef struct DLL_PACKED
{
    MS_U32 *pu32WindowNums;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETEXISTWIN,*PST_DMS_GETEXISTWIN;

typedef struct DLL_PACKED
{
    ST_DMS_WINDOW *pstLayer;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETOUTPUTLAYER,*PST_DMS_SETOUTPUTLAYER;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_3D_INFO *pst3DInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SET3DMODE,*PST_DMS_SET3DMODE;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETCAPTUREINFO,*PST_DMS_SETCAPTUREINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_CAPTURE_INFO *pstCaptureInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETCAPTUREBUF,*PST_DMS_GETCAPTUREBUF;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_CAPTURE_INFO *pstCaptureInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_RELEASECAPTUREBUF,*PST_DMS_RELEASECAPTUREBUF;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_METADATA_INFO *pstDmsMetaDataInfo;
    EN_DMS_RESULT eReturnValue;
}ST_DMS_SETMETADATA, *PST_DMS_SETMETADATA;

typedef struct DLL_PACKED
{
    EN_DMS_SECURITY_ACTION enAction;
    ST_DMS_SECURITY_HANDLER* pstDMSHandler;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SECURITYCONTROL,*PST_DMS_SECURITYCONTROL;

typedef struct DLL_PACKED
{
    ST_DMS_GET_ID_INFO* pstGetIDInfo;
    MS_U32* pu32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETWINDOWID,*PST_DMS_GETWINDOWID;
#ifndef UFO_DMS_OLD_VERSION
typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    MS_BOOL bEnable;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_DynamicScaling,*PST_DMS_DynamicScaling;

//notify vsync control to release related vdec frame. only one process uses this struct.
typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_FRAME_RELEASE_INFO* pstFrameReleaseInfo;
    MS_U32* pu32ExitFlag;
    EN_DMS_RESULT eReturnValue;
}ST_DMS_RELEASEFRAMEFUN, *PST_DMS_RELEASEFRAMEFUN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    FUNC_DMS_DISPLAYINFO fpFunDisplayInfoFun;
    EN_DMS_RESULT eReturnValue;
}ST_DMS_DISPLAYINFOFUN, *PST_DMS_DISPLAYINFOFUN;

//Call Back Usage.
typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_CALLBACK_STRUCT* pstCallBack;
    MS_U32* pu32ExitFlag;
    EN_DMS_RESULT enReturnValue;
}ST_DMS_REGISTERCALLBACKFUN, *PST_DMS_REGISTERCALLBACKFUN;

//To Check Wether Call Back Is Supported or not.
typedef struct DLL_PACKED
{
    MS_BOOL *pbIsSupport;
    EN_DMS_RESULT enReturnValue;
}ST_DMS_ISSUPPORTCALLBACK, *PST_DMS_ISSUPPORTCALLBACK;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_VIDEO_DELAY_INFO *pstDmsVideoDelay;
    EN_DMS_RESULT enReturnValue;
}ST_DMS_GETVIDEODELAY, *PST_DMS_GETVIDEODELAY;

#endif

#ifdef __cplusplus
}
#endif

#endif

