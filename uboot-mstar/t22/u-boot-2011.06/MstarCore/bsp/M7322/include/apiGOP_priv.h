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
#ifndef _APIGOP_PRIV_H_
#define _APIGOP_PRIV_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsTypes.h"
#include "MsDevice.h"
#include "apiGOP_v2.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// GOP Version
/// GOP CMD
typedef enum
{
    //gop property
    MAPI_CMD_GOP_INIT = 0x0,
    MAPI_CMD_GOP_GET_CHIPCAPS,
    MAPI_CMD_GOP_SET_CONFIG,
    MAPI_CMD_GOP_GET_CONFIG,
    MAPI_CMD_GOP_SET_PROPERTY,
    MAPI_CMD_GOP_GET_PROPERTY,
    MAPI_CMD_GOP_SET_DST,
    MAPI_CMD_GOP_GET_DST,
    MAPI_CMD_GOP_SET_MUX,
    MAPI_CMD_GOP_SET_LAYER,
    MAPI_CMD_GOP_GET_LAYER,
    MAPI_CMD_GOP_SET_MIRROR,
    MAPI_CMD_GOP_GET_MIRROR,
    MAPI_CMD_GOP_GET_STATUS,

    //Stretch Win
    MAPI_CMD_GOP_GWIN_SET_STRETCH = 0x100,
    MAPI_CMD_GOP_GWIN_GET_STRETCH,

    //GWIN info
    MAPI_CMD_GOP_GWIN_SET_PROPERTY = 0x200,
    MAPI_CMD_GOP_GWIN_GET_PROPERTY,
    MAPI_CMD_GOP_GWIN_MAPFB2WIN,
    MAPI_CMD_GOP_GWIN_SET_WININFO,
    MAPI_CMD_GOP_GWIN_GET_WININFO,
    //MAPI_CMD_GOP_GWIN_ENABLE,
    MAPI_CMD_GOP_GWIN_SETDISPLAY,
    MAPI_CMD_GOP_GWIN_DESTROY,

    //GFLIP
    MAPI_CMD_GOP_GFLIP_CLEARQUEUE = 0x300,
    MAPI_CMD_GOP_GFLIP_SWITCHGWIN,
    MAPI_CMD_GOP_GFLIP_SWITCHMULTIGWIN,
    MAPI_CMD_GOP_GFLIP_RESTORE_VSYNC_LMT,

    //PINPON
    MAPI_CMD_GOP_SET_PINPON  = 0x400,

    //FB Info
    MAPI_CMD_GOP_FB_CREATE   = 0x500,
    MAPI_CMD_GOP_FB_DESTROY,
    MAPI_CMD_GOP_FB_GETINFO,
    MAPI_CMD_GOP_FB_SETINFO,
    MAPI_CMD_GOP_FB_SET_PROPERTY,
    MAPI_CMD_GOP_FB_GET_PROPERTY,
    //palette
    MAPI_CMD_GOP_PALETTE_SET_CONFIG = 0x600,
    MAPI_CMD_GOP_PALETTE_SET_ENTRY,
    MAPI_CMD_GOP_PALETTE_GET_ENTRY,

    //Scroll
    MAPI_CMD_GOP_SET_SCROLL = 0x700,

    //FadeInOut
    MAPI_CMD_GOP_GWIN_SET_FADE = 0x800,

    //DWIN
    MAPI_CMD_GOP_DWIN_INIT = 0x900,
    MAPI_CMD_GOP_DWIN_CAPTURE,
    MAPI_CMD_GOP_DWIN_INTR,
    MAPI_CMD_GOP_DWIN_SET_PROPERTY,
    MAPI_CMD_GOP_DWIN_GET_PROPERTY,

    //MIXER
    MAPI_CMD_GOP_MIXER_SET_PROPERTY = 0xA00,
    MAPI_CMD_GOP_MIXER_GET_PROPERTY,

    //VE
    MAPI_CMD_GOP_VE_SET_PROPERTY = 0xB00,

    //MISC
    MAPI_CMD_GOP_UPDATE = 0xC00,
    MAPI_CMD_GOP_REGISTER_CB,
    MAPI_CMD_GOP_TESTPATTERN,
    MAPI_CMD_GOP_SELECTION,
    MAPI_CMD_GOP_MUTEX,
    MAPI_CMD_GOP_MISC,
    MAPI_CMD_GOP_RESETPOOL,
    //LEGACY

    //POWERSTATE
    MAPI_CMD_GOP_POWERSTATE =0xD00,
    //MAPI_CMD_GOP_LEGACY_FUNC,
    MAPI_CMD_GOP_INTERRUPT,

    //SDR2HDR
    MAPI_CMD_GOP_SDR2HDR,
    //HDR central control setting
    MAPI_CMD_GOP_HDR_CENTRALCTRL,
}E_GOP_API_CMD_TYPE;


typedef enum
{
    E_GOP_STRETCH_WIN     = 0x1,
    E_GOP_STRETCH_HSCALE  = 0x2,
    E_GOP_STRETCH_VSCALE  = 0x4,
    E_GOP_STRETCH_WIN_POSITION = 0x8,
    E_GOP_STRETCH_HSTRETCH_MODE = 0x10,
    E_GOP_STRETCH_VSTRETCH_MODE = 0x20,
    E_GOP_STRETCH_TRANSPCOLOR_MODE = 0x40,
}EN_GOP_STRETCH_TYPE;

typedef enum
{
    GOP_DWIN_FMT_UV7Y8 = 0,
    /// CrYCb domain.
    GOP_DWIN_FMT_UV8Y8,
    /// RGB domain (CSC bypass).
    GOP_DWIN_FMT_ARGB8888,
    /// RGB domain (CSC bypass).
    GOP_DWIN_FMT_RGB565,
    GOP_DWIN_FMT_MAX,
} EN_GOP_DWIN_FMT;

/// DWIN source selection
typedef enum
{
    /// DWIN OP source
    GOP_DWIN_SRC_OP   = 0,
    /// DWIN MVOP source
    GOP_DWIN_SRC_MVOP = 1,
    /// DWIN IP source
    GOP_DWIN_SRC_IP   = 2,
    /// DWIN OP1 source
    GOP_DWIN_SRC_OP1 =3,
    /// DWIN SUB MVOP source
    GOP_DWIN_SRC_SUBMVOP =4,
    /// DWIN GOPScaling source
    GOP_DWIN_SRC_GOPScaling =5,
    GOP_DWIN_SRC_MAX = 6,
} EN_GOP_DWIN_SRC;

typedef enum
{
    E_GOP_SCROLL_CONFIG,
    E_GOP_SCROLL_RATE,
} EN_GOP_SCROLL_TYPE;

typedef enum
{
    E_GOP_FADE_CONFIG,
} EN_GOP_FADE_SET_TYPE;

typedef enum
{
    E_GOP_MISC_ENABLE_T3D,
    E_GOP_MISC_SET_CAPTURE_CLK,
    E_GOP_MISC_POWER_OFF,
    E_GOP_MISC_POWER_ON,
    E_GOP_MISC_GET_RT_STATUS,
    E_GOP_MISC_GET_INFO,
    E_GOP_MISC_AT_EXIT,
    E_GOP_MISC_OC_FB_INFO,
    E_GOP_MISC_TESTPATTERN_ALPHA,
    E_GOP_MISC_DELETE_GWINSIZE,
    E_GOP_MISC_SET_DBG_LEVEL,
} EN_GOP_MISC_TYPE;


typedef enum
{
    E_GOP_MIUSEL,
    E_GOP_CONTRAST,
    E_GOP_BRIGHTNESS,
    E_GOP_OUTPUT_COLOR,
    E_GOP_UV_SWAP,
    E_GOP_YC_SWAP,
    E_GOP_HSTART,
    E_GOP_TRANSCOLOR,
    E_GOP_PROGRESSIVE,
    E_GOP_ALPHAINVERSE,
    E_GOP_PREALPHAMODE,
    E_GOP_FIELD_INVERSE,
    E_GOP_HSCALE_RATIO,
    E_GOP_VSCALE_RATIO,
    E_GOP_OUTPUT_TIMING,
    E_GOP_PIXELBASE_ENABLE,
    E_GOP_CLK,
    E_GOP_FORCE_WRITE,
    E_GOP_BANK_FORCE_WRITE,
    E_GOP_3D_LR_EXCHANGE,
    E_GOP_VE_OSD,
    E_GOP_OUTPUT_LAYER_SWITCH,
    E_GOP_SWAP_OVERLAP_WIN,
    E_GOP_BW_STRENGTH,
    E_GOP_LB_COUPLE,
    E_GOP_YUV,
    E_GOP_REG_UPDATED,
    E_GOP_RELATIVE_WIN_PRIO,
    E_GOP_RESOURCE,
    E_GOP_MAXFBNUM,
    E_GOP_MULTI_ALPHA_ENABLE,
    E_GOP_GWIN_TLB_ENABLE,
    E_GOP_R2Y_COLOR_MATRIX,
    E_GOP_AFBC_CORE_RESET,
    E_GOP_AFBC_CORE_ENABLE,
    E_GOP_NUM_IS_INIT,
    E_GOP_SET_R2YMatrix_ENABLE,
    E_GOP_SET_VOP_PATH,
    E_GOP_SET_CSC_TUNING,
    E_GOP_GET_CSC_CTRL_INFO,
    E_GOP_SET_CSC_CTRL,
}EN_GOP_PROPERTY;

/// Define GOP gwin fade in/out.
typedef enum
{
    /// GOP gwin fade in.
    E_GOP_GWIN_FADE_TYPE_IN,
    /// GOP gwin fade out.
    E_GOP_GWIN_FADE_TYPE_OUT,
    /// GOP gwin fade unknow type.
    E_GOP_GWIN_FADE_TYPE_UNKNOWN,
} EN_GOP_SET_FADE_TYPE;

typedef enum
{
    E_GOP_DWIN_R2Y,
    E_GOP_DWIN_UV_SWAP,
    E_GOP_DWIN_UV_SAMPLE,
    E_GOP_DWIN_SOURCE_SCAN_TYPE,
    E_GOP_DWIN_ALPHA_VALUE,
    E_GOP_DWIN_ALPHA_SOURCE,
    E_GOP_DWIN_ALPHA_INVERSE,
    E_GOP_DWIN_SKIP_FRAME,
    E_GOP_DWIN_PINPON,
    E_GOP_DWIN_INTERRUP_INFO,
    E_GOP_DWIN_CAPTURE_INFO,
}EN_GOP_DWIN_PROPERTY;

typedef enum
{
    E_GOP_MIXER_OUTPUT_TIMING,
    E_GOP_MIXER_TO_OP_OUTPUT_TIMING,
    E_GOP_MIXER_V_FILTER,
    E_GOP_MIXER_OLD_BLENDING_MODE,
}EN_GOP_MIXER_PROPERTY;

typedef enum
{
    E_GOP_VE_OUTPUT_TIMING,
}EN_GOP_VE_PROPERTY;

typedef enum
{
    E_GOP_SET_PINPON,
}EN_GOP_PINPON_PROPERTY;

typedef enum
{
    E_GOP_LOCK,
    E_GOP_UNLOCK,
}EN_GOP_MUTEX;

typedef enum
{
    E_GOP_GWIN_ENABLE,
    E_GOP_GWIN_BLENDING,
    E_GOP_GWIN_NEWAPLHA,
    E_GOP_GWIN_BLINK,
    E_GOP_GWIN_SHARE,
    E_GOP_GWIN_SHARE_CNT,
    E_GOP_GWIN_SWITCH_2_GWIN,
    E_GOP_GWIN_3D_MODE,
    E_GOP_GWIN_EXIST,
    E_GOP_GWIN_FREE_ID,
    E_GOP_GWIN_GET_FB,
    E_GOP_GWIN_GET_GOP,
    E_GOP_GWIN_GPUTILE,
    E_GOP_GWIN_STRETCH_OFFSET,
}EN_GOP_GWIN_PROPERTY;

typedef enum
{
    E_GOP_FB_POOLID,
    E_GOP_FB_EXIST,
    E_GOP_FB_OBTAIN,
}EN_GOP_FB_PROPERTY;

typedef enum
{
    E_GOP_UPDATE_FORCEWRITE,
    E_GOP_UPDATE_CURRENT_ONCE,
    E_GOP_UPDATE_ONCE,
}EN_GOP_UPDATE_TYPE;

typedef enum
{
    /// DWIN capture type -- default case
    GOP_API_VER,
}EN_GOP_API_TYPE;

typedef enum
{
    GOP_CREATE_BUFFER,        //Use GOP allocator
    GOP_CREATE_BUFFER_BYADDR  //By static addr
}EN_GOP_CREATEBUFFER_TYPE;

typedef enum
{
    E_GOP_MIRROR_NONE,
    E_GOP_MIRROR_H_ONLY,
    E_GOP_MIRROR_V_ONLY,
    E_GOP_MIRROR_HV,
    E_GOP_MIRROR_H_NONE,
    E_GOP_MIRROR_V_NONE,
}EN_GOP_MIRROR_TYPE;

typedef enum
{
    E_GOP_STATUS_CURRENT_GOP,
    E_GOP_STATUS_CURRENT_GWIN,
    E_GOP_STATUS_CURRENT_FBID,
    E_GOP_STATUS_GOP_MAXNUM,
    E_GOP_STATUS_GWIN_MAXNUM,
    E_GOP_STATUS_GWIN_TOTALNUM,
    E_GOP_STATUS_INIT,
    E_GOP_STATUS_VERSION,
    E_GOP_STATUS_MUX,
}EN_GOP_STATUS;

/// GFlip Switch Gwin
typedef enum
{
    GOP_FLIP_BY_ADDR,
    GOP_FLIP_BY_FBID
}EN_GOP_GFLIP_SWITCH_TYPE;

typedef enum
{
    E_GOP_DWIN_INTR_CONTROL,
    E_GOP_DWIN_INTR_GETINFO,
    E_GOP_DWIN_INTR_GETINFO_TIMEOUT,
}EN_GOP_DWIN_INTR;

typedef enum _GOP_CB_TYPE
{
   GOP_CB_XC_REDUCE_BW,
   GOP_CB_XC_INTERLACE,
   GOP_CB_XC_PANEL_HSTART,
   GOP_CB_GFX_FBFMT,
   GOP_CB_EVENT_NOTIFY,
   GOP_CB_DWIN_INFO,
}GOP_CB_TYPE;

typedef enum _EN_GOP_SELECT_TYPE
{
   EN_GOP_SEL_GOP,
   EN_GOP_SEL_GWIN,
   EN_GOP_SEL_FB,
}EN_GOP_SELECT_TYPE;

typedef enum _EN_TRANSCLR_PROPERTY
{
    EN_TRANSCLR_SET_COLOR,
    EN_TRANSCLR_ENABLE,
}EN_TRANSCLR_PROPERTY;

//-------------------------------------------------------------------------------------------------
//  Struct
//-------------------------------------------------------------------------------------------------

///the GOP MIXER2OP timing info
typedef struct DLL_PACKED
{
    MS_U8 u8GOP;
    MS_BOOL bEnable;
}GOP_MixerOldBlendingMode;

typedef struct DLL_PACKED GOP_RECT_T
{
    MS_U32 x;
    MS_U32 y;
    MS_U32 w;
    MS_U32 h;
} GOP_RECT;

typedef struct DLL_PACKED
{
    MS_U32 gwin;
    MS_U32 fbid;
    GOP_GwinInfo gwin_info;
    GOP_StretchInfo stretch_info;
    EN_GOP_STRETCH_DIRECTION  dir;
    GOP_RECT  dst_size;
}GOP_GWINDISPLAY_INFO, *PGOP_GWINDISPLAY_INFO;

typedef struct DLL_PACKED
{
    EN_GOP_DWIN_SEL dwinsel;   //select the dwin num
    EN_GOP_DWIN_SRC src;
    EN_GOP_DWIN_FMT fmt;
    MS_PHY     addr[DWIN_SUPPORT_BUFFER];  //capture base addr
    MS_PHY     addr1[DWIN_SUPPORT_BUFFER]; //capture addr bound
    MS_U32       pitch;
    GOP_RECT     rect;
    MS_U16       framecount;
    MS_BOOL      bInterlace;
    MS_BOOL      bEn;
    MS_BOOL      bWait;
} GOP_CAPTURE_INFO, *PGOP_CAPTURE_INFO;

//Fade
typedef struct DLL_PACKED _GOP_FADE_INFO
{
    MS_U32  gwin_idx;
    MS_BOOL   bEnable;
    EN_GOP_SET_FADE_TYPE eGwinFadeType;
    MS_U8   u8FadeRate;
}GOP_FADE_INFO, *PGOP_FADE_INFO;

/// Capture ring buffer info
typedef struct DLL_PACKED _GOP_RINGBUFF_INFO
{
    MS_U16 Gfx_RingBuffCnt;
    MS_U16 Gfx_CurrentIdx;
    MS_PHY Gfx_BufAddr[0x20];
} GOP_RINGBUFF_INFO, *PGOP_RINGBUFF_INFO;

typedef struct DLL_PACKED _GOP_PINPON_INFO
{
    MS_PHY u64Addr0;
    MS_PHY u64Addr1;
} GOP_PINPON_INFO, *PGOP_PINPON_INFO;

typedef struct DLL_PACKED _GOP_MIXER_TIMING_INFO
{
    GOP_MIXER_TIMINGTYPE mode;
    GOP_MixerTiming pMT;
} GOP_MIXER_TIMING_INFO, *PGOP_MIXER_TIMING_INFO;

typedef struct DLL_PACKED _GOP_VE_TIMING_INFO
{
    GOP_VE_TIMINGTYPE mode;
} GOP_VE_TIMING_INFO, *PGOP_VE_TIMING_INFO;

typedef struct DLL_PACKED _GOP_PAL_OPT
{
    MS_U32 gop_idx;
    EN_GOP_PALETTE_READTYPE paltype;
    EN_GOP_PALETTE palSrc;
}GOP_PAL_OPT,*PGOP_PAL_OPT;

typedef struct DLL_PACKED _GOP_UPDATE_INFO
{
    MS_U32  gop_idx;
    EN_GOP_UPDATE_TYPE  update_type;
    MS_BOOL bEn;
    MS_BOOL bSync;
}GOP_UPDATE_INFO,*PGOP_UPDATE_INFO;

typedef struct DLL_PACKED _GOP_OC_FB_INFO
{
    MS_PHY u64Addr;
    EN_GOP_OC_SEL_TYPE BankMode;
    MS_U32 enBufSize;
}GOP_OC_FB_INFO,*PGOP_OC_FB_INFO;

////////////////////////////
/// GOP init
////////////////////////////

typedef struct DLL_PACKED _GOP_INIT_PARAM
{
    MS_U32 gop_idx;                       //specify gop init, if gop_idx == max gop , init all gop.
    MS_U32* pInfo;            //GOP_InitInfo
    MS_U32 u32Size;
}GOP_INIT_PARAM, *PGOP_INIT_PARAM;


////////////////////////////
/// GOP caps
////////////////////////////

typedef struct DLL_PACKED _GOP_GETCAPS_PARAM
{
    MS_U32 caps;    //EN_GOP_CAPS
    void* pInfo;  //Return Caps info
    MS_U32 u32Size; //Corresponding to the require caps structure size
}GOP_GETCAPS_PARAM, *PGOP_GETCAPS_PARAM;

typedef struct DLL_PACKED _GOP_BUFFER_INFO
{
    MS_PHY addr;
    GOP_RECT disp_rect;
    MS_U32   width;
    MS_U32   height;
    MS_U32   pitch;
    MS_U32   fbFmt;
    EN_GOP_FRAMEBUFFER_STRING FBString;
}GOP_BUFFER_INFO, *PGOP_BUFFER_INFO;

typedef struct DLL_PACKED _GOP_SETCONFIG_PARAM
{
    EN_GOP_CONFIG_TYPE cfg_type;    //EN_GOP_CONFIG_TYPE
    void* pCfg;  //Return Caps info
    MS_U32 u32Size; //Corresponding to the require caps structure size
}GOP_SETCONFIG_PARAM, *PGOP_SETCONFIG_PARAM;

typedef struct DLL_PACKED _GOP_MISC_PARAM
{
    EN_GOP_MISC_TYPE misc_type;    //EN_GOP_CONFIG_TYPE
    MS_U32* pMISC;  //Return Caps info
    MS_U32 u32Size; //Corresponding to the require caps structure size
}GOP_MISC_PARAM, *PGOP_MISC_PARAM;


////////////////////////////
/// GOP Display
////////////////////////////

typedef struct DLL_PACKED _GOP_GWIN_DISPLAY_PARAM
{
    MS_U32 type;                         //API VER TYPE
    MS_U32 GwinId;                       //specify gwin.
    MS_U32* pDisplayInfo;                //Gwin display info  - GOP_GWINDISPLAY_INFO
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_DISPLAY_PARAM, *PGOP_GWIN_DISPLAY_PARAM;

typedef struct DLL_PACKED _GOP_GWIN_BLENDING
{
    MS_U32  Coef;
    MS_BOOL bEn;
}GOP_GWIN_BLENDING, *PGOP_GWIN_BLENDING;

typedef struct DLL_PACKED _GOP_GWIN_BLINK
{
    MS_U32  rate;
    MS_BOOL bEn;
}GOP_GWIN_BLINK, *PGOP_GWIN_BLINK;

typedef struct DLL_PACKED _GOP_GWIN_3D_MODE
{
    MS_U32 u32MainFBId;
    MS_U32 u32SubFBId;
    EN_GOP_3D_MODETYPE en3DMode;
}GOP_GWIN_3D_MODE, *PGOP_GWIN_3D_MODE;

typedef struct DLL_PACKED _GOP_GWIN_PROPERTY_PARAM
{
    MS_U32 type;                         //API VER TYPE
    EN_GOP_GWIN_PROPERTY  en_property;
    MS_U32 GwinId;                       //specify gwin.
    MS_U32* pSet;                        //reserved - for further use.
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_PROPERTY_PARAM, *PGOP_GWIN_PROPERTY_PARAM;


typedef struct DLL_PACKED _GOP_GWIN_MAPFBINFO_PARAM
{
    MS_U32 type;                         //API VER TYPE
    MS_U32 GwinId;                       //specify gwin.
    MS_U32 fbid;                         //Corresponding frame buffer Id
    MS_U32* pinfo;                       //reserved - for further use.
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_MAPFBINFO_PARAM, *PGOP_GWIN_MAPFBINFO_PARAM;

typedef struct DLL_PACKED _GOP_FB_PROPERTY_PARAM
{
    MS_U32 type;                         //API VER TYPE
    EN_GOP_FB_PROPERTY  en_property;
    MS_U32 FBId;                       //specify gwin.
    MS_U32* pSet;                        //reserved - for further use.
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_FB_PROPERTY_PARAM, *PGOP_FB_PROPERTY_PARAM;

typedef struct DLL_PACKED _GOP_GWIN_SETWININFO_PARAM
{
    MS_U32 type;                         //API VER TYPE
    MS_U32 GwinId;                       //specify gwin.
    MS_U32* pinfo;                       //Gwin mapping frame buffer info  - GOP_BUFFER_INFO
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_SETWININFO_PARAM, *PGOP_GWIN_SETWININFO_PARAM;

typedef struct DLL_PACKED _GOP_GWIN_WIN_ENABLE_PARAM
{
    MS_U32 type;                         //API VER TYPE
    MS_U32 GwinId;                       //specify gwin.
    MS_U32* pEn;                       //Gwin mapping frame buffer info  - GOP_BUFFER_INFO
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_WIN_ENABLE_PARAM, *PGOP_GWIN_WIN_ENABLE_PARAM;

typedef struct DLL_PACKED _GOP_GWIN_DESTROY_PARAM
{
    MS_U32 type;                         //API VER TYPE
    MS_U32 GwinId;                       //specify gwin.
    MS_U32* ptr;                          //reserved
    MS_U32 u32Size;                      //reserved - input param size - for error check.
}GOP_GWIN_DESTROY_PARAM, *PGOP_GWIN_DESTROY_PARAM;

////////////////////////////
/// GOP Fade
////////////////////////////

typedef struct DLL_PACKED _GOP_SET_FADE_PARAM
{
    MS_U32  type;      //API VER TYPE
    EN_GOP_FADE_SET_TYPE en_type;
    MS_U32* pSetting;
    MS_U32 u32Size;   //Size check for different input structure.
}GOP_SET_FADE_PARAM, *PGOP_SET_FADE_PARAM;

////////////////////////////
/// GOP Frame Buffer
////////////////////////////

typedef struct DLL_PACKED _GOP_CREATE_BUFFER_PARAM
{
    MS_U32  type;  //API VER TYPE
    EN_GOP_CREATEBUFFER_TYPE fb_type;
    MS_U32  fbid;
    MS_U32* pBufInfo;
    MS_U32 u32Size;
}GOP_CREATE_BUFFER_PARAM, *PGOP_CREATE_BUFFER_PARAM;

typedef struct DLL_PACKED _GOP_DELETE_BUFFER_PARAM
{
    MS_U32  type;  //API VER TYPE
    MS_U32* pBufId;
    MS_U32 u32Size;
}GOP_DELETE_BUFFER_PARAM, *PGOP_DELETE_BUFFER_PARAM;

typedef struct DLL_PACKED _GOP_FB_INFO_PARAM
{
    MS_U32  type;  //API VER TYPE
    MS_U32  fbid;
    MS_U32* pBufInfo;
    MS_U32  u32Size;
}GOP_FB_INFO_PARAM, *PGOP_FB_INFO_PARAM;


typedef struct DLL_PACKED _GOP_CONTRAST
{
    MS_U32 y;  //R - if rgb format
    MS_U32 u;  //G - if rgb format
    MS_U32 v;  //B - if rgb format
}GOP_CONTRAST, *PGOP_CONTRAST;

typedef struct _GOP_BRIGHTNESS
{
    MS_BOOL bMSB;  //Y - if yuv format
    MS_U32  Brightness;
}GOP_BRIGHTNESS, *PGOP_BRIGHTNESS;

typedef struct DLL_PACKED _GOP_TRANSCOLOR
{
    EN_GOP_TRANSCLR_FMT fmt;
    MS_U32              color;
    MS_BOOL             bEn;
    EN_TRANSCLR_PROPERTY transclr_property;
}GOP_TRANSCOLOR, *PGOP_TRANSCOLOR;


typedef struct DLL_PACKED _GOP_SET_PROPERTY_PARAM
{
    MS_U32  type;      //API VER TYPE
    EN_GOP_PROPERTY  en_pro;
    MS_U32  gop_idx;
    void* pSetting;
    MS_U32 u32Size;   //Size check for different input structure.
}GOP_SET_PROPERTY_PARAM, *PGOP_SET_PROPERTY_PARAM;


typedef struct DLL_PACKED _GOP_SETDST_PARAM
{
    MS_U32  type;      //API VER TYPE
    MS_U32  gop_idx;
    EN_GOP_DST_TYPE  en_dst;
    MS_U32* pDst;      //Reserved for furthur usage
    MS_U32 u32Size;
}GOP_SETDST_PARAM, *PGOP_SETDST_PARAM;

typedef struct DLL_PACKED _GOP_GETDST_PARAM
{
    MS_U32  type;      //API VER TYPE
    MS_U32  gop_idx;
    MS_U32* pDst;      //Reserved for furthur usage
    MS_U32 u32Size;
}GOP_GETDST_PARAM, *PGOP_GETDST_PARAM;

///the GOP and mux setting info for utopia2.0
typedef struct DLL_PACKED _GOP_SETMUX
{
    MS_U32 MuxCount;
    MS_U32 gop[0xF];
    MS_U32 mux[0xF];
}GOP_SETMUX, *PGOP_SETMUX;

///the GOP and Layer setting info for utopia2.0
typedef struct DLL_PACKED _GOP_SETLayer
{
    MS_U32 u32LayerCount;
    MS_U32 u32Gop[0xF];
    MS_U32 u32Layer[0xF];
}GOP_SETLayer, *PGOP_SETLayer;

typedef struct DLL_PACKED _GOP_SETMUX_PARAM
{
    MS_U32  type;      //API VER TYPE
    MS_U32* pMuxInfo;  //GOP_MuxConfig
    MS_U32 u32Size;
}GOP_SETMUX_PARAM, *PGOP_SETMUX_PARAM;

typedef struct DLL_PACKED _GOP_SETLAYER_PARAM
{
    MS_U32  type;      //API VER TYPE
    MS_U32* pLayerInfo;  //GOP_SETLayer
    MS_U32 u32Size;
}GOP_SETLAYER_PARAM, *PGOP_SETLAYER_PARAM;

typedef struct DLL_PACKED _GOP_SETMIRROR_PARAM
{
    MS_U32  type;      //API VER TYPE
    MS_U32  gop_idx;  //GOP_MuxConfig
    EN_GOP_MIRROR_TYPE  dir;  //Mirror type
    MS_U32  u32Size;
}GOP_SETMIRROR_PARAM, *PGOP_SETMIRROR_PARAM;

typedef struct DLL_PACKED _GOP_GETMIRROR_PARAM
{
    MS_U32  type;             //API VER TYPE
    MS_U32  gop_idx;
    EN_GOP_MIRROR_TYPE*  pdir;  //Mirror type
    MS_U32  u32Size;
}GOP_GETMIRROR_PARAM, *PGOP_GETMIRROR_PARAM;

typedef struct DLL_PACKED _GOP_TLB_Enable
{
    MS_U8   gop_idx;  //GOP number
    MS_BOOL bEn;  //enable
    MS_PHY  u64TLBAddr;
}GOP_TLB_Enable, *PGOP_TLB_Enable;

typedef struct DLL_PACKED _GOP_INIT_STATUS
{
    MS_U32 gop_idx;  //In
    MS_BOOL bInit;   //gop_idx has init or not.
}GOP_INIT_STATUS,*PGOP_INIT_STATUS;

typedef struct DLL_PACKED _GOP_GWIN_NUM
{
    MS_U32 gop_idx;  //In
    MS_U32 gwin_num;   //gop_idx has init or not.
}GOP_GWIN_NUM,*PGOP_GWIN_NUM;


typedef struct DLL_PACKED _GOP_GET_STATUS_PARAM
{
    MS_U32  type;      //API VER TYPE
    EN_GOP_STATUS  en_status;
    MS_U32* pStatus;      //Out - return value for input query
    MS_U32  u32Size;
}GOP_GET_STATUS_PARAM, *PGOP_GET_STATUS_PARAM;

typedef struct DLL_PACKED _GOP_MUTEX_PARAM
{
    MS_U32 gop_idx;
    EN_GOP_MUTEX en_mutex;
    MS_U32* pSet;                        //reserved - for further use.
    MS_U32 u32Size;
}GOP_MUTEX_PARAM, *PGOP_MUTEX_PARAM;

typedef struct DLL_PACKED _GOP_SET_PINPON_PARAM
{
    EN_GOP_PINPON_PROPERTY  en_pro;
    MS_U32* pSetting;
    MS_U32 u32Size;   //Size check for different input structure.
}GOP_SET_PINPON_PARAM, *PGOP_SET_PINPON_PARAM;

////////////////////////////
/// GOP Test Pattern
////////////////////////////
typedef struct DLL_PACKED _GOP_TEST_PATTERN_PARAM
{
    MS_U32 gop_idx;
    EN_GOP_TST_PATTERN TestPatternType;
    MS_U32 u32ARGB;
    MS_U32 u32Size;
}GOP_TEST_PATTERN_PARAM, *PGOP_TEST_PATTERN_PARAM;

typedef struct DLL_PACKED
{
    MS_U32 u32GOP_idx;
    MS_U32 u32HSize;
    MS_U32 u32VSize;
}GOP_DeleteWinSize_PARAM;

////////////////////////////
/// GOP Stretch
////////////////////////////

typedef struct DLL_PACKED _GOP_STRETCH_INFO
{
    GOP_RECT SrcRect;
    GOP_RECT DstRect;
    EN_GOP_STRETCH_HMODE enHMode;
    EN_GOP_STRETCH_VMODE enVMode;
    EN_GOP_STRCH_TRANSPCOLORMODE  enTColorMode;
}GOP_STRETCH_INFO, *PGOP_STRETCH_INFO;

typedef struct DLL_PACKED _GOP_STRETCH_SET_PARAM
{
    MS_U32 type;
    EN_GOP_STRETCH_TYPE  enStrtchType;
    MS_U32 gop_idx;
    MS_U32* pStretch;   //stretch win info  - PGOP_STRETCH_INFO
    MS_U32 u32Size;    // input structure size
}GOP_STRETCH_SET_PARAM, *PGOP_STRETCH_SET_PARAM;

typedef struct DLL_PACKED _GOP_STRETCH_OFFSET_ST
{
    MS_S32 s32Hoffset;
    MS_S32 s32Voffset;
} GOP_STRETCH_OFFSET_ST, *PGOP_STRETCH_OFFSET_ST;

////////////////////////////
/// GFlip Clear Queue
////////////////////////////
typedef struct DLL_PACKED _GOP_GWIN_CLEARQUEUE_INFO
{
    MS_U32 GwinId;                       //specify gwin.
//    MS_U32* pInfo;                       //Gwin display info  - GOP_GWINDISPLAY_INFO
}GOP_GWIN_CLEARQUEUE_INFO,*PGOP_GWIN_CLEARQUEUE_INFO;


typedef struct DLL_PACKED _GOP_GWIN_CLEARQUEUE_PARAM
{
    MS_U32 type;                         //API VER TYPE
    MS_U32* pClearInfo;                  //GOP_GWIN_CLEARQUEUE_INFO
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_CLEARQUEUE_PARAM, *PGOP_GWIN_CLEARQUEUE_PARAM;

typedef struct DLL_PACKED _GOP_GWIN_FLIP_WIN_INFO
{
    EN_GOP_GFLIP_SWITCH_TYPE enFlip;
    MS_U32  GwinId;                       //specify gwin.
    MS_U32  FbId;
    MS_U32  SubFbId;
    MS_PHY  FlipAddr;
    MS_PHY  SubFlipAddr;
    MS_U32  WaitTagID;
    MS_U32* pQueueCnt;
    MS_BOOL b3DEnable;
}GOP_GWIN_FLIP_WIN_INFO,*PGOP_GWIN_FLIP_WIN_INFO;

typedef struct DLL_PACKED _GOP_GWIN_FLIP_MULTI_WIN_INFO
{
    MS_U8 u8InfoCnt;
    GOP_GWIN_FLIP_WIN_INFO FlipInfo[GOP_MULTIINFO_NUM];
}GOP_GWIN_FLIP_MULTI_WIN_INFO,*PGOP_GWIN_FLIP_MULTI_WIN_INFO;

typedef struct DLL_PACKED _GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM
{
    MS_U32 type;                         //API VER TYPE
    void* pFlipInfo;                  //GOP_GWIN_CLEARQUEUE_INFO
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM, *PGOP_GWIN_GFLIP_SWITCH_GWIN_PARAM;

typedef struct DLL_PACKED _GOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM
{
    MS_U32 type;                         //API VER TYPE
    void* pMultiFlipInfo;                  //GOP_GWIN_CLEARQUEUE_INFO
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM, *PGOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM;

////////////////////////////
/// GOP PINPON
////////////////////////////
typedef struct DLL_PACKED _GOP_SET_PINPON_INFO
{
    MS_U8  GwinId;
    MS_BOOL bEnable;
    EN_GOP_PINPON_MODE mode;
}GOP_SET_PINPON_INFO,*PGOP_SET_PINPON_INFO;

////////////////////////////
/// GOP Palette
////////////////////////////

typedef struct DLL_PACKED _GOP_PALETTE_CONFIG_PARAM
{
    MS_U32  type;
    MS_U32  gop_idx;
    MS_U32* pPaletteInfo;
    MS_U32  u32Size;
}GOP_PALETTE_CONFIG_PARAM, *PGOP_PALETTE_CONFIG_PARAM;

/// GOP Palette Entry Set
typedef struct DLL_PACKED _GOP_PALETTE_ENTRY
{
    MS_U32 gop_idx; // specify gop to update palette
    MS_U32 start;   // start point
    MS_U32 end;     // end point
    MS_U32* pClut;  // Color look up table
    EN_GOP_PALETTE_TYPE palType; //input structure size
    EN_GOP_PALETTE palSrc;
}GOP_PALETTE_ENTRY, *PGOP_PALETTE_ENTRY;

typedef struct DLL_PACKED _GOP_PALETTE_SET_PARAM
{
    MS_U32 type;
    void* pClut;   //color look up table
    MS_U32 u32Size; // input structure size
}GOP_PALETTE_SET_PARAM, *PGOP_PALETTE_SET_PARAM;


////////////////////////////
/// GOP Dwin Capture
////////////////////////////

typedef struct DLL_PACKED _GOP_DWIN_CAPTURE_PARAM
{
    MS_U32  type;  //API VER TYPE
    MS_U32* pDwin;
    MS_U32 u32Size;
}GOP_DWIN_CAPTURE_PARAM, *PGOP_DWIN_CAPTURE_PARAM;

typedef struct DLL_PACKED _GOP_DWIN_CAPTURE_RINGBUFF_INFO_PARAM
{
    MS_U32  type;  //API VER TYPE
    MS_U32* pRingInfo;
    MS_U32 u32Size;
}GOP_DWIN_CAPTURE_RINGBUFF_INFO_PARAM, *PGOP_DWIN_CAPTURE_RINGBUFF_INFO_PARAM;

typedef struct DLL_PACKED _GOP_DWIN_PROPERTY_PARAM
{
    MS_U32  type;  //API VER TYPE
    EN_GOP_DWIN_PROPERTY  en_dwin_property;
    MS_U32* pDwinProp;
    MS_U32 u32Size;
}GOP_DWIN_PROPERTY_PARAM, *PGOP_DWIN_PROPERTY_PARAM;

typedef struct DLL_PACKED _GOP_MIXER_PROPERTY_PARAM
{
    MS_U32  type;  //API VER TYPE
    EN_GOP_MIXER_PROPERTY  en_mixer_property;
    MS_U32* pMixerProp;
    MS_U32 u32Size;
}GOP_MIXER_PROPERTY_PARAM, *PGOP_MIXER_PROPERTY_PARAM;

typedef struct DLL_PACKED _GOP_VE_PROPERTY_PARAM
{
    MS_U32  type;  //API VER TYPE
    EN_GOP_VE_PROPERTY  en_ve_property;
    MS_U32* pVEProp;
    MS_U32 u32Size;
}GOP_VE_PROPERTY_PARAM, *PGOP_VE_PROPERTY_PARAM;

typedef struct DLL_PACKED _GOP_DWIN_INTR_CTRL
{
    MS_U32 mask;
    MS_U32 intrEn;
}GOP_DWIN_INTR_CTRL, *PGOP_DWIN_INTR_CTRL;

typedef struct DLL_PACKED _GOP_DWIN_INTR_TIME_OUT
{
    GOP_DWinIntInfo DWinIntInfo;
    MS_U32 u32Timeout;
}GOP_DWIN_INTR_TIME_OUT, *PGOP_DWIN_INTR_TIME_OUT;


typedef struct DLL_PACKED _GOP_DWIN_INTR_STATUS_PARAM
{
    MS_U32  type;  //API VER TYPE
    EN_GOP_DWIN_INTR  intr_type;
    MS_U32* pIntrInfo;
    MS_U32 u32Size;
}GOP_DWIN_INTR_STATUS_PARAM, *PGOP_DWIN_INTR_STATUS_PARAM;

typedef struct DLL_PACKED _GOP_SCROLL_INFO
{
    MS_U8 u8WinId;
    GOP_SCROLL_TYPE type;
    MS_U16 u16Offset;
    MS_BOOL bEnable;
}GOP_SCROLL_INFO, *PGOP_SCROLL_INFO;

typedef struct DLL_PACKED
{
    EN_GOP_SCROLL_TYPE scroll_type;    //EN_GOP_CONFIG_TYPE
    MS_U32* pCfg;  //Return Caps info
    MS_U32 u32Size; //Corresponding to the require caps structure size
} GOP_SCROLL_PARAM, *PGOP_SCROLL_PARAM;

////////////////////////////
/// GOP Write Ack Update
////////////////////////////
typedef struct DLL_PACKED _GOP_UPDATE_PARAM
{
    MS_U32  type;           //API VER TYPE
    MS_U32* pUpdateInfo;    //GOP_UPDATE_INFO
    MS_U32 u32Size;
}GOP_UPDATE_PARAM, *PGOP_UPDATE_PARAM;


////////////////////////////
/// GOP register call back
////////////////////////////

typedef struct DLL_PACKED _GOP_REGISTER_CB_PARAM
{
    MS_U32  type;           //API VER TYPE
    GOP_CB_TYPE  cb_type;   //GOP_CB_TYPE
    void* pCB;              //Call back pointer
    MS_U32 u32Size;         //reserved for error check size
}GOP_REGISTER_CB_PARAM, *PGOP_REGISTER_CB_PARAM;

typedef struct DLL_PACKED _GOP_SELECTION_PROPERTY_PARAM
{
    MS_U32 type;                         //API VER TYPE
    EN_GOP_SELECT_TYPE sel_type;         //select gop / gwin or fb.
    MS_U32  id;                          //Corresponding GOP/Gwin/FB Id
    MS_U32* pinfo;                       //reserved - for further use.
    MS_U32 u32Size;                      //input param size - for error check.
}GOP_SELECTION_PROPERTY_PARAM, *PGOP_SELECTION_PROPERTY_PARAM;

typedef struct DLL_PACKED
{
    MS_U32  type;           //API VER TYPE
    void*   pInfo;
    MS_U32  u32Size;         //reserved for error check size
}GOP_POWERSTATE_PARAM, *PGOP_POWERSTATE_PARAM;

typedef struct DLL_PACKED
{
    MS_PHY u64Addr;
    MS_U16 u16Pitch;
    MS_U16 u16Fmt;
}GOP_CBFmtInfo, *PGOP_CBFmtInfo;

typedef struct DLL_PACKED
{
    EN_GOP_SDR2HDR enSDR2HDRstate;
}ST_GOP_SDR2HDR_PARAM, *PST_GOP_SDR2HDR_PARAM;

typedef struct
{
    MS_BOOL bCentralCtrl;
    MS_U8 u8GOPNum;
    EN_GOP_SDR2HDR eSDR2HDRstate;
}ST_GOP_HDR_CENTRAL_CTRL_PARAM, *PST_GOP_HDR_CENTRAL_CTRL_PARAM;

typedef struct DLL_PACKED
{
    MS_U32  type;      //API VER TYPE
    MS_U32  gop_idx;
    MS_U32* pSetting;
    MS_U32 u32Size;   //Size check for different input structure.
}GOP_INTERRUPT_PARAM, *PGOP_INTERRUPT_PARAM;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//======================================================================================
//  GOP Common utility
//======================================================================================
#ifdef CONFIG_GOP_UTOPIA10
MS_U32 Ioctl_GOP_DWIN_Init(void* pInstance);
MS_U32 Ioctl_GOP_Get_ScaleDst(void* pInstance,MS_U8 u8GOPNum,MS_U16 *pHScaleDst,MS_U16 *pVScaleDst);
MS_U32 Ioctl_GOP_Init(void* pInstance,MS_U8 u8GOP,GOP_InitInfo *pGopInit);
MS_U32 Ioctl_GOP_GetCaps(void* pInstance,EN_GOP_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);
MS_U32 Ioctl_GOP_SetConfig(void* pInstance,EN_GOP_CONFIG_TYPE cfg_type,MS_U32* pCfg,MS_U32 u32Size);
MS_U32 Ioctl_GOP_GetConfig(void* pInstance,EN_GOP_CONFIG_TYPE cfg_type,MS_U32* pCfg,MS_U32 u32Size);
MS_U32 Ioctl_GOP_SetProperty(void* pInstance,EN_GOP_PROPERTY type,MS_U32 gop, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_GetProperty(void* pInstance,EN_GOP_PROPERTY type,MS_U32 gop, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_SetDst(void* pInstance,MS_U8 u8GOP, EN_GOP_DST_TYPE dsttype);
MS_U32 Ioctl_GOP_GetDst(void* pInstance,MS_U32 gop, MS_U32* pDst);
MS_U32 Ioctl_GOP_SetMux(void* pInstance,PGOP_SETMUX pGopMuxConfig, MS_U32 u32SizeOfMuxInfo);
MS_U32 Ioctl_GOP_SetMirror(void* pInstance,MS_U32 gop, EN_GOP_MIRROR_TYPE mirror);
MS_U32 Ioctl_GOP_GetMirror(void* pInstance,MS_U32 gop, EN_GOP_MIRROR_TYPE* mirror);
MS_U32 Ioctl_GOP_GetStatus(void* pInstance,EN_GOP_STATUS type, MS_U32* pStatus, MS_U32 u32Size);
MS_U32 Ioctl_GOP_TestPattern(void* pInstance,EN_GOP_TST_PATTERN eTstPatternType, MS_U32 u32ARGB);
MS_U32 Ioctl_GOP_Set_Stretch(void* pInstance,EN_GOP_STRETCH_TYPE enStretchType,MS_U32 gop ,PGOP_STRETCH_INFO pStretchInfo);
MS_U32 Ioctl_GOP_Get_Stretch(void* pInstance,EN_GOP_STRETCH_TYPE enStretchType,MS_U32 gop ,PGOP_STRETCH_INFO pStretchInfo);
MS_U32 Ioctl_GOP_GWin_SetProperty(void* pInstance,EN_GOP_GWIN_PROPERTY type,MS_U32 gwin, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_GWin_GetProperty(void* pInstance,EN_GOP_GWIN_PROPERTY type,MS_U32 gwin, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_FB_SetProperty(void* pInstance,EN_GOP_FB_PROPERTY type,MS_U32 FBId, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_GWin_GetProperty(void* pInstance,EN_GOP_GWIN_PROPERTY type,MS_U32 gwin, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_FB_SetProperty(void* pInstance,EN_GOP_FB_PROPERTY type,MS_U32 FBId, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_FB_GetProperty(void* pInstance,EN_GOP_FB_PROPERTY type,MS_U32 FBId, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_DWIN_SetProperty(void* pInstance,EN_GOP_DWIN_PROPERTY type,MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_DWIN_GetProperty(void* pInstance,EN_GOP_DWIN_PROPERTY type,MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_MIXER_SetProperty(void* pInstance,EN_GOP_MIXER_PROPERTY type,MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_MIXER_GetProperty(void* pInstance,EN_GOP_MIXER_PROPERTY type,MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_VE_SetProperty(void* pInstance,EN_GOP_VE_PROPERTY type,MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_MapFB2Win(void* pInstance,MS_U32 fbid, MS_U32 gwin);
MS_U32 Ioctl_GOP_Dwin_Capture(void* pInstance,PGOP_CAPTURE_INFO pinfo);
MS_U32 Ioctl_GOP_Dwin_Intr(void* pInstance,EN_GOP_DWIN_INTR intr_type, MS_U32* pIntr ,MS_U32 u32Size);
MS_U32 Ioctl_GOP_SetWinInfo(void* pInstance,MS_U32 gwinId,PGOP_BUFFER_INFO pInfo);
MS_U32 Ioctl_GOP_GetWinInfo(void* pInstance,MS_U32 gwinId,PGOP_BUFFER_INFO pInfo);
MS_U32 Ioctl_GOP_Win_Enable(void* pInstance,MS_U32 gwinId,MS_BOOL bEn);
MS_U32 Ioctl_GOP_SetDisplay(void* pInstance,PGOP_GWINDISPLAY_INFO pInfo);
MS_U32 Ioctl_GOP_Win_Destroy(void* pInstance,MS_U32 gId);
MS_U32 Ioctl_GOP_FBCreate(void* pInstance,EN_GOP_CREATEBUFFER_TYPE fbtype,PGOP_BUFFER_INFO  pBuff,MS_U32 fbId);
MS_U32 Ioctl_GOP_FBDestroy(void* pInstance,MS_U32* pId);
MS_U32 Ioctl_GOP_GetFBInfo(void* pInstance,MS_U32 u32fbId, PGOP_BUFFER_INFO pBuff);
MS_U32 Ioctl_GOP_SetFBInfo(void* pInstance,MS_U32 u32fbId, PGOP_BUFFER_INFO pBuff);
MS_U32 Ioctl_GOP_TriggerRegWriteIn(void* pInstance,MS_U32 gop, EN_GOP_UPDATE_TYPE update_type, MS_BOOL bEn, MS_BOOL bSync);
MS_U32 Ioctl_GOP_Select(void* pInstance,EN_GOP_SELECT_TYPE sel, MS_U32 id, MS_U32* pSet);
MS_U32 Ioctl_GOP_Register_CB(void* pInstance,GOP_CB_TYPE cb_type, void* pCB, MS_U32 u32Size);
MS_U32 Ioctl_GOP_Palette_EntrySet(void* pInstance,PGOP_PALETTE_ENTRY pClutEntry);
MS_U32 Ioctl_GOP_Palette_EntryGet(void* pInstance,PGOP_PALETTE_ENTRY pClutEntry);
MS_U32 Ioctl_GOP_Palette_Set_Config(void* pInstance,MS_U32 gop, EN_GOP_PALETTE_READTYPE type,EN_GOP_PALETTE src);
MS_U32 Ioctl_GOP_MISC(void* pInstance,EN_GOP_MISC_TYPE type, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_PowerState(void* pInstance,MS_U32 u32PowerState, void* pModule);
MS_U32 Ioctl_GOP_PINPON(void* pInstance,EN_GOP_PINPON_PROPERTY en_pro, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_Fade(void* pInstance,EN_GOP_FADE_SET_TYPE type, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_Scroll(void* pInstance,EN_GOP_SCROLL_TYPE type, MS_U32* pSet, MS_U32 u32Size);
MS_U32 Ioctl_GOP_SetLayer(void* pInstance,PGOP_SETLayer pstLayer,MS_U32 u32Size);
MS_U32 Ioctl_GOP_GetLayer(void* pInstance,PGOP_SETLayer pstLayer,MS_U32 u32Size);
int Ioctl_GOP_Interrupt(void* pInstance,MS_U8 u8Gop,MS_BOOL bEnable);
MS_U32 Ioctl_GFlip_ClearFlipQueue(void* pInstance,MS_U32 gwin);
MS_U32 Ioctl_GFlip_SwitchGwin(void* pInstance,PGOP_GWIN_FLIP_WIN_INFO pflip);
MS_U32 Ioctl_GFlip_SwitchMultiGwin(void* pInstance,PGOP_GWIN_FLIP_MULTI_WIN_INFO pMultiflip);
#endif
//======================================================================================
//  GOP GWIN APIs
//======================================================================================


//======================================================================================
//  GOP FB management
//======================================================================================

//======================================================================================
//  GOP  CallBack Functions
//======================================================================================

//======================================================================================
//  OC
//======================================================================================

//======================================================================================
//  MISC
//======================================================================================
void Ioctl_GOP_AtExit(void* pInstance);
#ifdef __cplusplus
}
#endif

#endif
