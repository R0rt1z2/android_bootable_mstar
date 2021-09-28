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
///////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar GOP Driver Interface
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------
/*! \defgroup GOP GOP interface

 \image html apiGOP_schema.png
 <b>Mstar GOP(Graphic Output Path) engine is used to for graphic output. </b> \n
 - Alpha blending??the content of framebuffer to video.
 - GWIN is a window defined for displaying framebuffer content of GOP.
 - Once enabled, GOP read framebuffer pixel data continuously upon output timing

 <b> Features: </b> \ns
 - Data Format : RGB565/RGB1555/ARGB4444/I8/ARGB8888/2266/YUV??etc
 - Output mode : RGB/YUV
 - Window dimension : h-start, h-end, v-start, v-end
 - Out path:  MVOP/IP main/IP sub/VOP/MIXER/VE/OC
 - Scaling UP
 - H/V Mirror
 - H/V Scolling
 - Testpattern
 - Color colorkey

 <b> Operation Code Flow:  </b> \ns
 -# ResetPool
 -# ResetGOP
 -# InitByGOP
 -# UpdateRegOnce(TRUE)
 -# ...AP Behavior...
 -# GWIN_Enable
 -# UpdateRegOnce(FALSE)

  *! \defgroup GOP_INIT GOP init related functions
  *   \ingroup   GOP

  *! \defgroup INIT GOP init related functions
  *   \ingroup   GOP_INIT

  *! \defgroup RESET GOP rest related functions
  *   \ingroup   GOP_INIT

  *! \defgroup PRE_CONFIG GOP pre config related functions
  *   \ingroup   GOP_INIT

  *! \defgroup GOP_CAPS GOP get capability related functions
  *   \ingroup   GOP

  *! \defgroup GOP_CB GOP call back related functions
  *   \ingroup   GOP

  *! \defgroup GOP_LOCK_BEGINE_END GOP lock begine/end related functions
  *   \ingroup   GOP

  *! \defgroup GOP_CONFIG GOP config related functions
  *   \ingroup   GOP

  *! \defgroup DST GOP dst related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup MIRROR GOP mirror related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup PALETTE GOP palette related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup 3D GOP 3D related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup GOP_ORDER GOP order related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup COLOR_TRANSFOR GOP color transfor related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup TRANSPARENT_CLR_CEY GOP transparent color key related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup MIU GOP MIU related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup BW GOP BW related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup GOP_OPTION GOP option related functions
  *   \ingroup   GOP_CONFIG

  *! \defgroup GOP_UPDATE GOP update related functions
  *   \ingroup   GOP

  *! \defgroup FORCE_WRITE GOP force write related functions
  *   \ingroup   GOP_UPDATE

  *! \defgroup UPDATE_GROUP_ONCE GOP update group once related functions
  *   \ingroup   GOP_UPDATE

  *! \defgroup UPDATE GOP update related functions
  *   \ingroup   GOP_UPDATE

  *! \defgroup GOP_TEST_PATTERN GOP test pattern related functions
  *   \ingroup   GOP

  *! \defgroup GOP_POWER_STATE GOP power state related functions
  *   \ingroup   GOP

  *! \defgroup STRETCH_WINDOW Stretch-window related functions
  *   \ingroup   GOP

  *! \defgroup SET_STRETCH_WINDOW Stretch-window set window related functions
  *   \ingroup   STRETCH_WINDOW

  *! \defgroup SCALE Stretch-window scaling related functions
  *   \ingroup   STRETCH_WINDOW

  *! \defgroup GWIN GWIN related functions
  *   \ingroup   GOP

  *! \defgroup GWIN_CREATE GWIN create related functions
  *   \ingroup   GWIN

  *! \defgroup SHARE GWIN share related functions
  *   \ingroup   GWIN

  *! \defgroup GWIN_DELETE GWIN delete related functions
  *   \ingroup   GWIN

  *! \defgroup SET_GWIN_WINDOW GWIN set window related functions
  *   \ingroup   GWIN

  *! \defgroup FLIP GWIN flip related functions
  *   \ingroup   GWIN

  *! \defgroup ALPHA_BLENDING GWIN alpha blending related functions
  *   \ingroup   GWIN

  *! \defgroup BASIC GWIN alpha blending basic related functions
  *   \ingroup   ALPHA_BLENDING

  *! \defgroup EXTRA GWIN alpha blending option related functions
  *   \ingroup   ALPHA_BLENDING

  *! \defgroup ENABLE GWIN enable related functions
  *   \ingroup   GWIN

  *! \defgroup GWIN_STATUS GWIN status related functions
  *   \ingroup   GWIN

  *! \defgroup GWIN_OPTION GWIN option related functions
  *   \ingroup   GWIN

  *! \defgroup GWIN_ORDER GWIN order related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup BG GWIN BG related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup SCROLL GWIN scroll related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup FADING GWIN fading related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup BLINK GWIN blink related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup PINPON GWIN pin-pon related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup TILE_MODE GWIN tile-mode related functions
  *   \ingroup   GWIN_OPTION

  *! \defgroup FB FB related functions
  *   \ingroup   GOP

  *! \defgroup POOL FB pool related functions
  *   \ingroup   FB

  *! \defgroup FB_CREATE FB create related functions
  *   \ingroup   FB

  *! \defgroup SET FB set related functions
  *   \ingroup   FB

  *! \defgroup FB_STATUS FB status related functions
  *   \ingroup   FB

  *! \defgroup FB_DELETE FB delete related functions
  *   \ingroup   FB

  *! \defgroup DWIN DWIN related functions
  *   \ingroup   GOP

  *! \defgroup DWIN_CONFIG DWIN config related functions
  *   \ingroup   DWIN

  *! \defgroup DWIN_INTERRUPT DWIN interrupt related functions
  *   \ingroup   DWIN

  *! \defgroup DWIN_CAPTURE DWIN capture related functions
  *   \ingroup   DWIN

  *! \defgroup DWIN_OPTION DWIN option related functions
  *   \ingroup   DWIN

  *! \defgroup GOP_MIXER MIXER related functions
  *   \ingroup   GOP

  *! \defgroup GOP_VE VE related functions
  *   \ingroup   GOP

  *! \defgroup GOP_ToBeRemove GOP to be remove related functions
  *   \ingroup   GOP

  */
//-------------------------------------------------


#ifndef _API_GOP_H_
#define _API_GOP_H_



#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"
#include "MsCommon.h"
#include "MsDevice.h"
#include "apiGOP_v2.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// GOP Version
#define MSIF_GOP_LIB_CODE                   {'G','O','P','_'}    //Lib code
#define MSIF_GOP_LIBVER                     {'8','9'}            //LIB version
#define MSIF_GOP_BUILDNUM                   {'0','2'}            //Build Number
#define MSIF_GOP_CHANGELIST                 {'0','0','5','2','4','9','1','6'} //P4 ChangeList Number

#define GOP_API_VERSION                   /* Character String for DRV/API version             */  \
    MSIF_TAG,                             /* 'MSIF'                                           */  \
    MSIF_CLASS,                           /* '00'                                             */  \
    MSIF_CUS,                             /* 0x0000                                           */  \
    MSIF_MOD,                             /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_GOP_LIB_CODE,                    /* IP__                                             */  \
    MSIF_GOP_LIBVER,                      /* 0.0 ~ Z.Z                                        */  \
    MSIF_GOP_BUILDNUM,                    /* 00 ~ 99                                          */  \
    MSIF_GOP_CHANGELIST,                  /* CL#                                              */  \
    MSIF_OS

#define ST_GOP_PALETTE_SHARESRAM_INFO_VERSION    (1)

#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
///GOP_PALETTE_ENTRY_NUM
#define GOP_PALETTE_ENTRY_NUM   256
///FB_FMT_AS_DEFAULT
#define FB_FMT_AS_DEFAULT   0xFFFF
///DEFAULT_FB_FMT
#define DEFAULT_FB_FMT 0xffff
///INVALID_WIN_ID
#define INVALID_WIN_ID GWIN_NO_AVAILABLE
///GWIN_ID_INVALID
#define GWIN_ID_INVALID 0xFF
///LAYER_ID_INVALID
#define LAYER_ID_INVALID 0xFFFF
///INVALID_GOP_NUM
#define INVALID_GOP_NUM  0xFF
#else
///GOP_PALETTE_ENTRY_NUM
#define GOP_PALETTE_ENTRY_NUM   256UL
///FB_FMT_AS_DEFAULT
#define FB_FMT_AS_DEFAULT   0xFFFFUL
///DEFAULT_FB_FMT
#define DEFAULT_FB_FMT 0xFFFFUL
///INVALID_WIN_ID
#define INVALID_WIN_ID GWIN_NO_AVAILABLE
///GWIN_ID_INVALID
#define GWIN_ID_INVALID 0xFFUL
///LAYER_ID_INVALID
#define LAYER_ID_INVALID 0xFFFFUL
///INVALID_GOP_NUM
#define INVALID_GOP_NUM  0xFFUL
#endif
///MAX_GWIN_SUPPORT
#define MAX_GWIN_SUPPORT MApi_GOP_GWIN_GetTotalGwinNum()


#ifndef MAX_GWIN_FB_SUPPORT
#define MAX_GWIN_FB_SUPPORT MApi_GOP_GWIN_GetMax32FBNum()
#endif

#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#define MAX_NUM_GOP_API_INF_SUPPORT 6
#else
#define MAX_NUM_GOP_API_INF_SUPPORT 6UL
#endif

//the macro defines only be applied for the upper user, not for the internal driver,otherwise it will cause the deadlock
#define MAX_GOP0_SUPPORT MApi_GOP_GWIN_GetGwinNum(0)
#define MAX_GOP1_SUPPORT MApi_GOP_GWIN_GetGwinNum(1)
#define MAX_GOP2_SUPPORT MApi_GOP_GWIN_GetGwinNum(2)
#define MAX_GOP3_SUPPORT MApi_GOP_GWIN_GetGwinNum(3)
#define MAX_GOP4_SUPPORT MApi_GOP_GWIN_GetGwinNum(4)
#define MAX_GOP5_SUPPORT MApi_GOP_GWIN_GetGwinNum(5)
//#define MAX_GOP_SUPPORT MApi_GOP_GWIN_GetMaxGOPNum()

//the macro defines only be applied for the upper user, not for the internal driver,otherwise it will cause the deadlock
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#define GOP0_GWIN_START 0
#else
#define GOP0_GWIN_START 0UL
#endif
#define GOP1_GWIN_START MAX_GOP0_SUPPORT
#define GOP2_GWIN_START MAX_GOP0_SUPPORT + MAX_GOP1_SUPPORT
#define GOP3_GWIN_START MAX_GOP0_SUPPORT + MAX_GOP1_SUPPORT + MAX_GOP2_SUPPORT
#define GOP4_GWIN_START MAX_GOP0_SUPPORT + MAX_GOP1_SUPPORT + MAX_GOP2_SUPPORT + MAX_GOP3_SUPPORT
#define GOP5_GWIN_START MAX_GOP0_SUPPORT + MAX_GOP1_SUPPORT + MAX_GOP2_SUPPORT + MAX_GOP3_SUPPORT + MAX_GOP4_SUPPORT

/// GOP API return value
typedef enum
{
    ///GOP API return fail.
    GOP_API_FAIL                        = 0,
    ///GOP API return success.
    GOP_API_SUCCESS                     = 1,
    ///GOP API return non align address.
    GOP_API_NON_ALIGN_ADDRESS           ,
    ///GOP API return non align pitch.
    GOP_API_NON_ALIGN_PITCH             ,
    ///GOP API return depend no avail.
    GOP_API_DEPEND_NOAVAIL              ,
    ///GOP API return mutex obtain fail.
    GOP_API_MUTEX_OBTAIN_FAIL           ,
    ///GOP API return mutex obtain.
    GOP_API_MUTEX_OBTAINED              ,
    ///GOP API return invalid buffer info.
    GOP_API_INVALID_BUFF_INFO          ,
    ///GOP API return invalid parameter.
    GOP_API_INVALID_PARAMETERS,
    ///GOP API return this interface not supported.
    GOP_API_FUN_NOT_SUPPORTED,
    ///GOP API return enum not supported.
    GOP_API_ENUM_NOT_SUPPORTED,
    ///GOP API return create gwin fail.
    GOP_API_CRT_GWIN_FAIL               = 0xFE,
    ///GOP API return create no avail
    GOP_API_CRT_GWIN_NOAVAIL            = 0xFF,
} E_GOP_API_Result;

///GWIN_FAIL
#define GWIN_FAIL         GOP_API_CRT_GWIN_FAIL
///GWIN_OK
#define GWIN_OK           GOP_API_SUCCESS
///GWIN_NO_AVAILABLE
#define GWIN_NO_AVAILABLE GOP_API_CRT_GWIN_NOAVAIL

#define ST_GOP_CSC_PARAM_VERSION (1)
#define ST_GOP_CSC_TABLE_VERSION (1)
#define ST_GOP_CSC_TUNING_INFO_VERSION (1)

///GWINID
typedef MS_U8 GWINID;

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// GWIN display property
typedef struct DLL_PACKED
{
    ///gwin id.
    MS_U8 gId;
    ///gwin start x coordinate.
    MS_U16 StartX;
    ///gwin start y coordinate.
    MS_U16 StartY;
    ///gwin win width.
    MS_U16 Win_Width;
    ///gwin win height.
    MS_U16 Win_Height;
    ///gwin frame buffer offset x.
    MS_U16 FBOffset_X;
    ///gwin frame buffer offset y.
    MS_U16 FBOffset_Y;
} GOP_GwinDispProperty;

/// GOP init info
typedef struct DLL_PACKED
{
    ///panel width.
    MS_U16 u16PanelWidth;
    ///panel height.
    MS_U16 u16PanelHeight;
    ///panel h-start.
    MS_U16 u16PanelHStr;
    ///vsync interrupt flip enable flag.
    MS_BOOL bEnableVsyncIntFlip;
    ///gop frame buffer starting address.
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
    MS_PHY u32GOPRBAdr;
#else
    MS_PHYADDR u32GOPRBAdr;
#endif
    ///gop frame buffer length.
    MS_U32 u32GOPRBLen;
    ///gop regdma starting address.
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
    MS_PHY u32GOPRegdmaAdr;
#else
    MS_PHYADDR u32GOPRegdmaAdr;
#endif
    ///gop regdma length.
    MS_U32 u32GOPRegdmaLen;
}GOP_InitInfo;

/// GOP palette entry
typedef struct DLL_PACKED
{
    /// A.
    MS_U8 u8A;
    /// R.
    MS_U8 u8R;
    /// G.
    MS_U8 u8G;
    /// B.
    MS_U8 u8B;
} GOP_Palette;

/// Frame buffer attributes for GWIN
typedef struct DLL_PACKED
{
    /// frame buffer is mapped to which gwin.
    MS_U8 gWinId;
    /// frame buffer enable flag.
    MS_U8 enable:1;
    /// frame buffer allocated flag.
    MS_U8 allocated:1;
    /// frame buffer x0 (unit: pix).
    MS_U16 x0;
    /// frame buffer y0 (unit: line).
    MS_U16 y0;
    /// frame buffer x1 (unit: pix).
    MS_U16 x1;
    /// frame buffer y1 (unit: line).
    MS_U16 y1;
    /// frame buffer width (unit: pix).
    MS_U16 width;
    /// frame buffer height (unit: line).
    MS_U16 height;
    /// frame buffer pitch (unit: byte).
    MS_U16 pitch;
    /// frame buffer color format.
    MS_U16 fbFmt;
    /// frame buffer starting address.
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
    MS_PHY addr;
#else
    MS_U32 addr;
#endif
    /// frame buffer size.
    MS_U32 size;
    /// next frame buffer id in the same pool.
    MS_U8 next;
    /// previous frame buffer in the same pool.
    MS_U8 prev;
    ///store which AP use this FB.
    MS_U8 string;
    /// frame buffer s_x (unit: pix).
    MS_U16 s_x;
    /// frame buffer  s_y (unit: pix).
    MS_U16 s_y;
    /// frame buffer dispWidth (unit: pix).
    MS_U16 dispWidth;
    /// frame buffer dispHeight (unit: line).
    MS_U16 dispHeight;
} GOP_GwinFBAttr;

/// Dump Window Information
typedef struct DLL_PACKED
{
    /// dwin v-start (unit:line).
    MS_U16 u16VPixelStart;
    /// dwin v-end (unit: line).
    MS_U16 u16VPixelEnd;
    /// dwin h-start (unit: pix).
    MS_U16 u16HPixelStart;
    /// dwin h-end (unit: pix).
    MS_U16 u16HPixelEnd;
    /// dwin dram starting address (unit: Byte).
    MS_PHY u32TFDRAMAddr;
    /// dwin access address high bond (unit: Byte).
    MS_PHY u32BFDRAMAddr;
    /// dwin jump address length after each dwin line end to next line start (unit: Byte).
    MS_U16 u16DRAMJumpLen;
    /// DWIN format: 0 UV7Y8, 1 UV8Y8, 2 ARGB8888, 3 RGB565.
    MS_U8 u8fmt;
} GOP_DwinInfo;

/// DWIN source selection
typedef enum
{
    /// DWIN OP source
    DWIN_SRC_OP = 0,
    /// DWIN MVOP source
    DWIN_SRC_MVOP = 1,
    /// DWIN IP source
    DWIN_SRC_IP = 2,
    /// DWIN OP1 source
    DWIN_SRC_OP1 = 3,
    /// DWIN SUBMVOP source
    DWIN_SRC_SUBMVOP = 4,
    /// DWIN GOPScaling source
    DWIN_SRC_GOPScaling = 5,
    DWIN_SRC_MAX = 6,
}EN_GOP_DWIN_SRC_SEL;

/// DWIN source data format
typedef enum
{
    /// CrYCb domain.
    DWIN_DATA_FMT_UV7Y8 = 0,
    /// CrYCb domain.
    DWIN_DATA_FMT_UV8Y8,
    /// RGB domain (CSC bypass).
    DWIN_DATA_FMT_ARGB8888,
    /// RGB domain (CSC bypass).
    DWIN_DATA_FMT_RGB565,
    DWIN_DATA_FMT_MAX
} EN_GOP_DWIN_DATA_FMT;


/// DWIN Interrupt mask
typedef enum
{
    /// DWIN PROG INT mask
    GOPDWIN_INT_MASK_PROG   = 0x0010,
    /// DWIN TOP Field INT mask
    GOPDWIN_INT_MASK_TF     = 0x0020,
    /// DWIN Bottom Field INT mask
    GOPDWIN_INT_MASK_BF     = 0x0040,
    /// DWIN Vsync INT mask
    GOPDWIN_INT_MASK_VS     = 0x0080,
} EN_GOP_DWIN_INT_MASK;

/// GWIN output color domain
typedef enum
{
    /// output color RGB.
    GOPOUT_RGB,
    /// output color YUV.
    GOPOUT_YUV,
} EN_GOP_OUTPUT_COLOR;


/// Define GOP FB string.
typedef enum
{
    E_GOP_FB_NULL,
    /// OSD frame buffer.
    E_GOP_FB_OSD,
    /// Mute frame buffer.
    E_GOP_FB_MUTE,
    /// subtitle frame buffer.
    E_GOP_FB_SUBTITLE,
    /// teltext frame buffer.
    E_GOP_FB_TELTEXT,
    /// MHEG5 frame buffer.
    E_GOP_FB_MHEG5,
    /// CC frame buffer.
    E_GOP_FB_CLOSEDCAPTION,
    /// AFBC frame buffer.
    E_GOP_FB_AFBC_SPLT_YUVTRNS_ARGB8888= 0x100,
    E_GOP_FB_AFBC_NONSPLT_YUVTRS_ARGB8888,
    E_GOP_FB_AFBC_SPLT_NONYUVTRS_ARGB8888,
    E_GOP_FB_AFBC_NONSPLT_NONYUVTRS_ARGB8888,
} EN_GOP_FRAMEBUFFER_STRING;


/// Define GOP Event for Notification.
typedef enum
{
    /// E_GOP_EVENT_BUF_ARRANGE. Begin of Rearrange GOP buffers.
    E_GOP_EVENT_BUF_ARRANGE_BEGIN    = 0,
    /// E_GOP_Event_Buffer_Rearrange. End of Rearrange GOP buffers.
    E_GOP_EVENT_BUF_ARRANGE_END    = 1,
} EN_GOP_EVENT_TYPE;

/// Define GOP G3D DATA FORMAT.
typedef enum
{
    /// E_GOP_TILE_DATA_16BPP. 16 BPP DATA FOR TILE MODE
    E_GOP_TILE_DATA_16BPP    = 0,
    /// E_GOP_TILE_DATA_16BPP. 32 BPP DATA FOR TILE MODE
    E_GOP_TILE_DATA_32BPP    = 1,
} EN_GOP_TILE_DATA_TYPE;

/// Define GOP MIU SEL
typedef enum
{
    /// E_GOP_SEL_MIU0. gop access miu 0
    E_GOP_SEL_MIU0    = 0,
    /// E_GOP_SEL_MIU1. gop access miu1
    E_GOP_SEL_MIU1    = 1,
    /// E_GOP_SEL_MIU2. gop access miu2
    E_GOP_SEL_MIU2    = 2,
    /// E_GOP_SEL_MIU3. gop access miu3
    E_GOP_SEL_MIU3    = 3,
} EN_GOP_SEL_TYPE;


///the GOP and mux setting info on utopia
typedef struct DLL_PACKED _GOP_MuxConfig
{
  //GopMux arrays record the u8GopIndex and the corresponding MuxIndex
  //u8MuxCounts: how many mux need to modify the gop settings

  struct DLL_PACKED
  {
      MS_U8 u8GopIndex;
      MS_U8 u8MuxIndex;
  }GopMux[6];
  MS_U8 u8MuxCounts;
}GOP_MuxConfig, *PGOP_MuxConfig;

///the GOP and OSD Layer setting info on utopia
typedef struct DLL_PACKED _GOP_LayerConfig
{
  struct DLL_PACKED
  {
      MS_U32 u32GopIndex;
      MS_U32 u32LayerIndex;
  }stGopLayer[6];
  MS_U32 u32LayerCounts;
}GOP_LayerConfig, *PGOP_LayerConfig;

typedef struct DLL_PACKED
{
    MS_U8 gWinId;
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
    MS_PHY u32FlipAddr;
    MS_PHY u32SubAddr;
#else
    MS_U32 u32FlipAddr;
    MS_U32 u32SubAddr;
#endif
    MS_U16 u16WaitTagID;
    MS_U16 *pU16QueueCnt;
}GOP_FlipConfig, *PGOP_FlipConfig;

typedef struct DLL_PACKED
{
    MS_U8 u8InfoCnt;
    GOP_FlipConfig astGopInfo[GOP_MULTIINFO_NUM];
}GOP_MultiFlipInfo, *PGOP_MultiFlipInfo;

/// Define Mux
typedef enum
{
    ///Select gop output to mux0
    EN_GOP_MUX0 = 0,
    /// Select gop output to mux1
    EN_GOP_MUX1 = 1,
    /// Select gop output to mux2
    EN_GOP_MUX2 = 2,
    /// Select gop output to mux3
    EN_GOP_MUX3 = 3,
        ///Select gop output to IP0
    EN_GOP_IP0_MUX = 4,
    /// Select gop output to IP1
    EN_GOP_IP1_MUX = 5,
    /// Select gop output to vop0
    EN_GOP_VOP0_MUX = 6,
    /// Select gop output to vop1
    EN_GOP_VOP1_MUX = 7,
    /// Select gop output to FRC mux0
    EN_GOP_FRC_MUX0 = 8,
    /// Select gop output to FRC mux1
    EN_GOP_FRC_MUX1 = 9,
    /// Select gop output to FRC mux2
    EN_GOP_FRC_MUX2 = 10,
    /// Select gop output to FRC mux3
    EN_GOP_FRC_MUX3 = 11,
    /// Select gop output to BYPASS mux
    EN_GOP_BYPASS_MUX0 = 12,
    /// Select gop output to IP0_SUB
    EN_GOP_IP0_SUB_MUX = 13,
    /// Select gop output to VOP0_SUB
    EN_GOP_VOP0_SUB_MUX = 14,
    /// Select gop output to OP1
    EN_GOP_OP1_MUX = 15,
    /// Select gop output to DIP
    EN_GOP_DIP_MUX = 16,
    /// Select gop output to DIP
    EN_GOP_GS_MUX = 17,
    /// Select gop output to mux4
    EN_GOP_MUX4  = 18,
    EN_MAX_GOP_MUX_SUPPORT,
} EN_Gop_MuxSel;

/// Define DEBUG level.
typedef enum
{
    /// GOP DEBUG LEVEL LOW. Just printf error message.
    E_GOP_Debug_Level_LOW    = 0,
    /// GOP DEBUG LEVEL Medium. printf warning message and error message.
    E_GOP_Debug_Level_MED    = 1,
    /// GOP DEBUG LEVEL hIGH. printf all message with function.
    E_GOP_Debug_Level_HIGH    = 2,
} EN_GOP_DEBUG_LEVEL;


/// Define GOP gwin fade in/out.
typedef enum
{
    /// GOP gwin fade in.
    E_GOP_GWIN_FADE_IN,
    /// GOP gwin fade out.
    E_GOP_GWIN_FADE_OUT,
    /// GOP gwin fade unknow type.
    E_GOP_GWIN_FADE_UNKNOWN,
} EN_GOP_FADE_TYPE;


typedef enum
{
    GOP_PINPON_G3D ,
    GOP_PINPON_VE,
    GOP_PINPON_DWIN,
    GOP_PINPON_DIP,
    GOP_PINPON_DWIN0,
    GOP_PINPON_DWIN1,
}GOP_PINPON_MODE;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable; //InOut
    MS_U8   u8FrameCount; //Out, value range: 0~3
    MS_U8   u8Result; //Out
}MS_GOP_VECAPTURESTATE, *PMS_GOP_VECAPTURESTATE;

typedef enum
{
    E_GOP_VALID_6BITS             =   0,
    E_GOP_VALID_8BITS             =   1,
} EN_GOP_CONSALPHA_BITS;


/// Define GOP Ignore init value
typedef enum
{
    /// GOP init ingore mux init
    E_GOP_IGNORE_MUX              =   0x0001,
    /// GOP init ingore gwin reset
    E_GOP_IGNORE_GWIN             =   0x0002,
    /// GOP init ingore stretchwin reset
    E_GOP_IGNORE_STRETCHWIN      =   0x0004,
    /// GOP init ingore palette table reset
    E_GOP_IGNORE_PALETTE          =   0x0008,
    /// GOP init ingore rest GOP destination to OP
    E_GOP_IGNORE_SET_DST_OP       =   0x0010,
    /// GOP init ingore enable transparent color key
    E_GOP_IGNORE_ENABLE_TRANSCLR =   0x0020,
    /// GOP set destination ignore VEOSD for bootlogo case
    E_GOP_BOOTLOGO_IGNORE_VEOSDEN =  0x0040,
    /// GOP init ingore all
    E_GOP_IGNORE_ALL              =   0xFFFF,
    ///Not Ingore
    E_GOP_IGNORE_DISABLE          =   0x0000,
} EN_GOP_IGNOREINIT;


//GOP Capability
typedef enum
{
    E_GOP_CAP_WORD_UNIT ,
    E_GOP_CAP_GWIN_NUM,
    E_GOP_CAP_VERSION,
    E_GOP_CAP_RESERVED,
    E_GOP_CAP_CONSALPHA_VALIDBITS,
    E_GOP_CAP_PALETTE,
    E_GOP_CAP_DWIN,
    E_GOP_CAP_GOP_MUX,
    E_GOP_CAP_PIXELMODE_SUPPORT,
    E_GOP_CAP_STRETCH,
    E_GOP_CAP_TLBMODE_SUPPORT,
    E_GOP_CAP_AFBC_SUPPORT,
    E_GOP_CAP_BNKFORCEWRITE,
    E_GOP_CAP_SUPPORT_H4TAP_256PHASE,
    E_GOP_CAP_SUPPORT_V4TAP_256PHASE,
    E_GOP_CAP_SUPPORT_MIXER_SDR2HDR,
    E_GOP_CAP_SUPPORT_H8V4TAP,
    E_GOP_CAP_PALETTE_SHARESRAM_INFO,
    E_GOP_CAP_BRIGHTNESS_INFO,
    E_GOP_CAP_SUPPORT_V_SCALING_BILINEAR,
}EN_GOP_CAPS;

//GOP Function work type
typedef enum
{
    E_GOP_FUN_AFBC,
}EN_GOP_FUNS;

// GOP palette type
typedef enum
{
    E_GOP_CAP_PAL_SIZE_NONE = 0x0000,
    E_GOP_CAP_PAL_SIZE_256  = 0x0100,
    E_GOP_CAP_PAL_SIZE_64   = 0x0200,
    E_GOP_CAP_PAL_SIZE_MASK = 0x0F00,
}EN_GOP_CAP_PAL;

typedef enum
{
    E_GOP_SHARE_FROM_NONE = 0x0,
    E_GOP_SHARE_FROM_GOP0 = 0x1,
    E_GOP_SHARE_FROM_GOP1 = 0x2,
    E_GOP_SHARE_FROM_GOP2 = 0x3,
    E_GOP_SHARE_FROM_MAX,
}EN_GOP_PAL_SHARESRAM_INFO;

//Note: It's for MApi_GOP_GetChipCaps, and driver will return E_GOP_BRIG_SIGNBIT or E_GOP_BRIG_2SCOMPLEMENT.
//if return E_GOP_BRIG_SIGNBIT, meas when caller call MApi_GOP_SetGOPBrightness and the variable "u16BriVal" is 0xFF, "bMSB" is TRUE,
//the actual result is -255.
//Otherwise, if driver return E_GOP_BRIG_2SCOMPLEMENT, the actual result is -1.
typedef enum
{
    E_GOP_BRIG_SIGNBIT,
    E_GOP_BRIG_2SCOMPLEMENT,
    E_GOP_BRIG_MAX,
}EN_GOP_BRIG_INFO;

///Dwin Capability of IC
typedef struct DLL_PACKED
{
    MS_BOOL bSupportWindowDECapture; //TRUE: Support it; FALSE:Not support, should use FrameDE to capture video
    MS_BOOL bSupportOSDCapture;      //TRUE: Support it; FALSE:Not support
}GOP_DWIN_CAP, *PGOP_DWIN_CAP;

///GOP MUX PRIORITY when gop goes OP
typedef struct DLL_PACKED
{
    MS_U8 u8MuxNumber;
    MS_U8 *pu8MuxProrityArray;
    MS_BOOL bNewMux;
    MS_BOOL bNeedSetMUX1ToIP0;
    MS_BOOL bNeedSetMUX3ToIP0;
}ST_GOP_MUX_CAP, *PST_GOP_MUX_CAP;

///GOP lib info
typedef struct DLL_PACKED
{
    MS_U32 GOP_LibVer;
    MS_U32 GOP_BuildNum;
    MS_U32 GOP_ChangeList;
}GOP_LIB_VER, *PGOP_LIB_VER;

///GOP palette info
typedef struct DLL_PACKED
{
    MS_U32 GOP_NumOfTbl;
    MS_U32 GOP_PalTbl[0x10];
    MS_U32 GOP_PalTblIdx;
}GOP_CAP_PAL_TYPE, *PGOP_CAP_PAL_TYPE;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    MS_U8 u8GOPIdx;
    EN_GOP_PAL_SHARESRAM_INFO enGopPalInfo;
    void* pGopPalInfo;
#if !defined (__aarch64__)
    void* pDummy0;
#endif
}ST_GOP_PALETTE_SHARESRAM_INFO, *PST_GOP_PALETTE_SHARESRAM_INFO;

typedef enum
{
    E_GOP_DWIN_INIT ,
    E_GOP_DWIN_ENABLE ,
    E_GOP_DWIN_ENABLEINTR ,
    E_GOP_DWIN_SRCSLCT ,
    E_GOP_DWIN_ONESHOT ,
    E_GOP_DWIN_ONESHOT2 ,
    E_GOP_DWIN_SET_INFO ,
    E_GOP_DWIN_SET_YUORD ,
    E_GOP_DWIN_GET_BUFSTUS ,
    E_GOP_DWIN_CLR_BUFSTUS ,
    E_GOP_DWIN_SET_SCANTYPE ,
    E_GOP_DWIN_SET_DATAFMT ,
    E_GOP_DWIN_SET_ALPHA ,
    E_GOP_DWIN_SET_R2Y ,
} EN_GOP_DWIN_CNTL;

///GOP palette info
typedef struct DLL_PACKED
{
    MS_U32 GOP_VStretch_Support;
}GOP_CAP_STRETCH_INFO, *PGOP_CAP_STRETCH_INFO;

///GOP TLB support info
typedef struct DLL_PACKED
{
    MS_U32 GOP_TLB_Support;
}GOP_CAP_TLB_INFO, *PGOP_CAP_TLB_INFO;

///GOP AFBC support info
typedef struct
{
    MS_U32 GOP_AFBC_Support;
}GOP_CAP_AFBC_INFO, *PGOP_CAP_AFBC_INFO;

///GOP timing Information
typedef struct DLL_PACKED
{
    MS_U16 u16HDTotal;// Output horizontal total
    MS_U16 u16VDTotal; //Output vertical total

    MS_U16 u16DEHStart; //Output DE horizontal start
    MS_U16 u16DEHSize;// Output DE horizontal size

    MS_U16 u16DEVStart; //Output DE vertical start
    MS_U16 u16DEVSize;//Output DE Vertical size

    MS_BOOL bInterlaceMode;
    MS_BOOL bYUVInput;
    MS_BOOL bCLK_EN;
    MS_BOOL bINVALPHA_EN;
}ST_GOP_TIMING_INFO, *PST_GOP_TIMING_INFO;

typedef struct DLL_PACKED
{
    MS_U16 Matrix[3][3];
}ST_GOP_R2Y_Matrix, *PST_GOP_R2Y_Matrix;

typedef enum
{
    E_GOP_MULTIWIN_SET_COLORKEY,
    E_GOP_MULTIWIN_SET_FRAMECOLOR,
    E_GOP_MULTIWIN_GET_COLORKEY,
    E_GOP_MULTIWIN_GET_FRAMECOLOR,
}EN_GOP_MULTIWIN_COLOR_MODE;

typedef struct DLL_PACKED
{
    MS_BOOL enable;
    MS_U8 u8WinAlpha;
    MS_U8 u8BoarderWidth;
    MS_U8 u8BoarderAlpha;
    MS_U8 u8BoarderY_G;
    MS_U8 u8BoarderCr_R;
    MS_U8 u8BoarderCb_B;
    MS_U16 u16WinHstart;
    MS_U16 u16WinHend;
    MS_U16 u16WinVstart;
    MS_U16 u16WinVend;
}GOP_MULTIWIN_COLORKEY, *PGOP_MULTIWIN_COLORKEY;

typedef struct DLL_PACKED
{
    MS_U8 u8frameAlpha;
    MS_U8 u8frameY_G;
    MS_U8 u8frameCr_R;
    MS_U8 u8frameCb_B;
}GOP_MULTIWIN_FRAMECOLOR, *PGOP_MULTIWIN_FRAMECOLOR;

typedef enum
{
    GOP_SDR2HDR_OFF,
    GOP_SDR2HDR_HDR10,
}EN_GOP_SDR2HDR;

typedef enum
{
    E_GOP_VOPPATH_DEF                = 0x0, /// Select osd blending position at last
    E_GOP_VOPPATH_BEF_RGB3DLOOKUP    = 0x1, /// Select osd blending position before RGB 3D LookUP
    E_GOP_VOPPATH_BEF_PQGAMMA        = 0x2, /// Select osd blending position before gamma table
    E_GOP_VOPPATH_AFT_PQGAMMA        = 0x3, /// Select osd blending position after gamma table
    E_GOP_VOPPATH_MAX,
}EN_GOP_VOP_PATH_MODE;

//Mapping CFD E_CFD_CFIO define
typedef enum
{
    E_GOP_CFD_CFIO_RGB_NOTSPECIFIED      = 0x0, //means RGB, but no specific colorspace
    E_GOP_CFD_CFIO_RGB_BT601_625         = 0x1,
    E_GOP_CFD_CFIO_RGB_BT601_525         = 0x2,
    E_GOP_CFD_CFIO_RGB_BT709             = 0x3,
    E_GOP_CFD_CFIO_RGB_BT2020            = 0x4,
    E_GOP_CFD_CFIO_SRGB                  = 0x5,
    E_GOP_CFD_CFIO_ADOBE_RGB             = 0x6,
    E_GOP_CFD_CFIO_YUV_NOTSPECIFIED      = 0x7, //means YUV, but no specific colorspace
    E_GOP_CFD_CFIO_YUV_BT601_625         = 0x8,
    E_GOP_CFD_CFIO_YUV_BT601_525         = 0x9,
    E_GOP_CFD_CFIO_YUV_BT709             = 0xA,
    E_GOP_CFD_CFIO_YUV_BT2020_NCL        = 0xB,
    E_GOP_CFD_CFIO_YUV_BT2020_CL         = 0xC,
    E_GOP_CFD_CFIO_XVYCC_601             = 0xD,
    E_GOP_CFD_CFIO_XVYCC_709             = 0xE,
    E_GOP_CFD_CFIO_SYCC601               = 0xF,
    E_GOP_CFD_CFIO_ADOBE_YCC601          = 0x10,
    E_GOP_CFD_CFIO_DOLBY_HDR_TEMP        = 0x11,
    E_GOP_CFD_CFIO_SYCC709               = 0x12,
    E_GOP_CFD_CFIO_DCIP3_THEATER         = 0x13, /// HDR10+
    E_GOP_CFD_CFIO_DCIP3_D65             = 0x14, /// HDR10+
    E_GOP_CFD_CFIO_RGB_DEFAULT           = E_GOP_CFD_CFIO_RGB_BT709,
    E_GOP_CFD_CFIO_YUV_DEFAULT           = E_GOP_CFD_CFIO_YUV_BT709,
    E_GOP_CFD_CFIO_MAX,
}EN_GOP_CFD_CFIO;

//Mapping CFD E_CFD_MC_FORMAT definition
typedef enum
{
    E_GOP_CFD_MC_FORMAT_RGB                   = 0x0,
    E_GOP_CFD_MC_FORMAT_YUV422                = 0x1,
    E_GOP_CFD_MC_FORMAT_YUV444                = 0x2,
    E_GOP_CFD_MC_FORMAT_YUV420                = 0x3,
    E_GOP_CFD_MC_FORMAT_MAX,
}EN_GOP_CFD_MC_FORMAT;

//Mapping CFD E_CFD_CFIO_RANGE definition
typedef enum
{
    E_GOP_CFD_CFIO_RANGE_LIMIT                = 0x0,
    E_GOP_CFD_CFIO_RANGE_FULL                 = 0x1,
    E_GOP_CFD_CFIO_RANGE_MAX,
}EN_GOP_CFD_CFIO_RANGE;

//CFD input param structure
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    MS_BOOL bCscEnable;
    MS_BOOL bUpdateWithVsync;
    //STU_CFDAPI_UI_CONTROL
    MS_U16 u16Hue;
    MS_U16 u16Saturation;
    MS_U16 u16Contrast;
    MS_U16 u16Brightness;
    MS_U16 u16RGBGGain[3];
    //STU_CFDAPI_MAIN_CONTROL_LITE
    EN_GOP_CFD_CFIO enInputFormat;
    EN_GOP_CFD_MC_FORMAT enInputDataFormat;
    EN_GOP_CFD_CFIO_RANGE enInputRange;
    EN_GOP_CFD_CFIO enOutputFormat;
    EN_GOP_CFD_MC_FORMAT enOutputDataFormat;
    EN_GOP_CFD_CFIO_RANGE enOutputRange;
}ST_GOP_CSC_PARAM;

//CFD output structure
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    MS_U16 u16CscControl;
    ST_GOP_R2Y_Matrix stCSCMatrix;
    MS_U16 u16BrightnessOffsetR;
    MS_U16 u16BrightnessOffsetG;
    MS_U16 u16BrightnessOffsetB;
}ST_GOP_CSC_TABLE;

//Get CFD input & output structure
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    ST_GOP_CSC_PARAM *pstCSCParam;
    ST_GOP_CSC_TABLE *pstCSCTable;
}ST_GOP_CSC_TUNING_INFO;
//-----------------------------------------------------------------------------
//  Functions
//-----------------------------------------------------------------------------
//-----------------
// GOP_INIT
//-----------------
//-----------------
// INIT
//-----------------
//-------------------------------------------------------------------------------------------------
/// Initial all GOP driver (include gop0, gop1 ext..)
/// @ingroup INIT
/// @param pGopInit \b IN:gop driver init info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_Init(GOP_InitInfo *pGopInit);
//-----------------
// GOP_ToBeRemove
//-----------------
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GetInfo
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetInfo(GOP_ApiInfo* pGopApiInfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GetStatus
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetStatus(GOP_ApiStatus *pGopApiSts);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_SetDbgLevel
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetDbgLevel(EN_GOP_DEBUG_LEVEL level);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GetLibVer
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// Initial individual GOP driver
/// @ingroup INIT
/// @param pGopInit \b IN:gop driver init info
/// @param u8GOP \b IN: only init by which gop
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_InitByGOP(GOP_InitInfo *pGopInit, MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SwitchGOP
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SwitchGOP(MS_U8 u8GOP);
//-----------------
// STRETCH_WINDOW
//-----------------
//-----------------
// SET_STRETCH_WINDOW
//-----------------
//-------------------------------------------------------------------------------------------------
/// Restore initial stretch window setting of GOP. It will set gop to OP path and stretch window width/height
/// @ingroup SET_STRETCH_WINDOW
/// to panel width/height
/// @param u8GOPNum \b IN : Number of GOP
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_Initialize_StretchWindow(MS_U8 u8GOPNum);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_SetGOPClk
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetGOPClk(MS_U8 gopNum, EN_GOP_DST_TYPE eDstType);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetCurrentGOP
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetCurrentGOP(void);

//-----------------
// GOP_CAPS
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get maximum support gop number
/// @ingroup GOP_CAPS
/// @param  void                \b IN: none
/// @return gop number
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetMaxGOPNum(void);
//-------------------------------------------------------------------------------------------------
/// Get maximum gwin number by individual gop
/// @ingroup GOP_CAPS
/// @param u8GopNum \b IN: Number of GOP
/// @return maximum gwin number by individual gop
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetGwinNum(MS_U8 u8GopNum);

//-------------------------------------------------------------------------------------------------
/// Get maximum support gwin number by all gop
/// @ingroup GOP_CAPS
/// @param  void                \b IN: none
/// @return gwin number
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetTotalGwinNum(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_PowerOn
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_PowerOn(void); //temparially naming, maybe change
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_PowerOff
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_PowerOff(void); //temparially naming, maybe change
//-----------------
// RESET
//-----------------
//-------------------------------------------------------------------------------------------------
/// GOP reset GOP resource
/// @ingroup RESET
/// @param  u32Gop                \b IN: GOP
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result Mapi_GOP_GWIN_ResetGOP(MS_U32 u32Gop);


//-------------------------------------------------------------------------------------------------
/// GOP reset lock resource
/// @ingroup RESET
/// @param  void                \b IN: none
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_ResetPool(void);

//-----------------
// GOP_POWER_STATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// set GOP power state
/// @ingroup GOP_POWER_STATE
/// @param enPowerState\b IN power status
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetPowerState(EN_POWER_MODE enPowerState);
//-------------------------------------------------------------------------------------------------
/// Set GOP bandwidth saving mode.
/// @ingroup BW
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetGOPBWStrength(EN_GOP_BW_STRENGTH eGOPBWStr, MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
/// Get GOP Power OFF for saving power.
/// @ingroup BW
/// @return EN_GOP_BW_STRENGTH - mode of GOP bw saving .
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetGOPBWStrength(MS_U8 u8GOP, EN_GOP_BW_STRENGTH* curBW);

//-----------------
// GOP_OPTION
//-----------------
//-------------------------------------------------------------------------------------------------
/// Configure panel hstart timing for GOP; For configuring correct OSD position.
/// @ingroup GOP_OPTION
/// @param u8GOP \b IN:  GOP number
/// @param u16PanelHStr \b IN horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetGOPHStart(MS_U8 u8GOP,MS_U16 u16PanelHStr);
//-------------------------------------------------------------------------------------------------
/// Configure GOP brightness
/// @ingroup COLOR_TRANSFOR
/// @param u8GOP \b IN:  GOP number
/// @param u16BriVal \b IN brightness value
/// @param bMSB \b IN MSB
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetGOPBrightness(MS_U8 u8GOP,MS_U16 u16BriVal,MS_BOOL bMSB);
//-------------------------------------------------------------------------------------------------
/// Get GOP brightness value
/// @ingroup COLOR_TRANSFOR
/// @param u8GOP \b IN:  GOP number
/// @param u16BriVal \b OUT brightness value
/// @param bMSB \b OUT MSB
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetGOPBrightness(MS_U8 u8GOP,MS_U16* u16BriVal,MS_BOOL* bMSB);
//-----------------
// BW
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable LB Couple Feature to enlarge GOP FIFO for BW issue
/// @ingroup BW
/// @param u8GOP \b IN:  GOP number
/// @param bEnable \b IN Enable or Disable
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_EnableLBCouple(MS_U8 u8GOP, MS_BOOL bEnable);
//-----------------
// GOP_VE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Configure panel hstart timing for GOP; For configuring correct OSD position.
/// @ingroup GOP_VE
/// @param u16PanelHStr \b IN horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_VE_SetOutputTiming(GOP_VE_TIMINGTYPE mode);

//-------------------------------------------------------------------------------------------------
/// API for Enable VEOSD
/// @ingroup GOP_VE
/// @param u8win \b IN: GOP ID
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_Enable_VEOSD(MS_U8 u8GOP, MS_BOOL bEn);
//-----------------
// GOP_MIXER
//-----------------
//-------------------------------------------------------------------------------------------------
/// Configure panel hstart timing for GOP; For configuring correct OSD position.
/// @ingroup GOP_MIXER
/// @param u16PanelHStr \b IN horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_MIXER_SetOutputTiming(GOP_MIXER_TIMINGTYPE mode, GOP_MixerTiming *pMT);


//-------------------------------------------------------------------------------------------------
/// Configure mixer timing for GOP destination MIXER2OP; For configuring correct OSD position.
/// @ingroup GOP_MIXER
/// @param pMT \b IN:  mixer timing
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_MIXER_SetMIXER2OPOutputTiming(GOP_Mixer2OPTiming *pMT);

//-------------------------------------------------------------------------------------------------
/// Enable Mixer for Vfilter
/// @ingroup GOP_MIXER
/// @param bEnable \b IN:
///   - # TRUE Enable Vfilter
///   - # FALSE Disable Vfilter
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_MIXER_EnableVfilter(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable Mixer for old mode blending
/// @ingroup GOP_MIXER
/// @param bEnable \b IN:
///   - # TRUE Enable old mode
///   - # FALSE Disable old mode
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_MIXER_EnableOldBlendMode(MS_U8 u8GOP, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_RestoreFromVsyncLimitation
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RestoreFromVsyncLimitation(void);

//-----------------
// MIU
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for set gop miu selection
/// @ingroup MIU
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_MIUSel(MS_U8 u8GOP, EN_GOP_SEL_TYPE MiuSel);
//-------------------------------------------------------------------------------------------------
/// API for set U/V SWAP
/// @ingroup COLOR_TRANSFOR
/// @param u8GOP \b IN: GOP number
/// @param bEnable \b IN:
///   - # TRUE Enable U/V SWAP for GOP
///   - # FALSE Disable U/V SWAP for GOP
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetUVSwap(MS_U8 u8GOP, MS_BOOL bEn);
//-------------------------------------------------------------------------------------------------
/// API for set Y/C SWAP
/// @ingroup COLOR_TRANSFOR
/// @param u8GOP \b IN: GOP number
/// @param bEnable \b IN:
///   - # TRUE Enable Y/C SWAP for GOP
///   - # FALSE Disable Y/C SWAP for GOP
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetYCSwap(MS_U8 u8GOP, MS_BOOL bEn);
//-------------------------------------------------------------------------------------------------
/// API for query gop miu selection
/// @ingroup MIU
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GetMIUSel(MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
/// API for set contrast
/// @ingroup COLOR_TRANSFOR
/// @param u8GOP            \b IN: GOP number
/// @param u16YContrast     \b IN: Y contrast
/// @param u16UContrast     \b IN: U contrast
/// @param u16VContrast     \b IN: V contrast
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetGOPContrast(MS_U8 u8GOP, MS_U16 u16YContrast, MS_U16 u16UContrast, MS_U16 u16VContrast);
//-------------------------------------------------------------------------------------------------
/// API for get contrast
/// @ingroup COLOR_TRANSFOR
/// @param u8GOP \b IN: GOP number
/// @param u16YContrast     \b IN: Y contrast
/// @param u16UContrast     \b IN: U contrast
/// @param u16VContrast     \b IN: V contrast
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetGOPContrast(MS_U8 u8GOP, MS_U16* u16YContrast, MS_U16* u16UContrast, MS_U16* u16VContrast);
//-------------------------------------------------------------------------------------------------
/// API for VE captue
/// @ingroup GOP_VE
/// @param pstVECapState \b IN: paramater of capture
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_EnaVECapture(PMS_GOP_VECAPTURESTATE pstVECapState);
//-------------------------------------------------------------------------------------------------
/// API for get VE captue status
/// @ingroup GOP_VE
/// @param pstVECapState \b IN: paramater of capture
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetVECaptureState(PMS_GOP_VECAPTURESTATE pstVECapState);
//-------------------------------------------------------------------------------------------------
/// API for wait one frame done
/// @ingroup GOP_VE
/// @param pstVECapState \b IN: paramater of capture
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_VECaptureWaitOnFrame(PMS_GOP_VECAPTURESTATE pstVECapState);

/*******************************************************************************/
//Set configuration for special cases
///        - This is for special case usage. Do NOT overuse this function
/// @param type \b IN:  Config type
///        - 0: E_GOP_IGNOREINIT,
///        - 1: E_GOP_CONSALPHA_BITS
/// @param plist \b IN:  Config list for the specific type
//         - This list could be an enum or a structure
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetConfig(EN_GOP_CONFIG_TYPE type, MS_U32 *plist);
#else
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetConfig(EN_GOP_CONFIG_TYPE type, void *plist);
#endif
/********************************************************************************/
/// Set config by GOP; For dynamic usage.
/// @param u8GOP \b IN:  GOP number
/// @param enType \b IN:  GOP info type
/// @param pstInfo \b IN misc info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_SetConfigEx(MS_U8 u8Gop, EN_GOP_CONFIG_TYPE enType, MS_U32 *plist);
#else
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_SetConfigEx(MS_U8 u8Gop, EN_GOP_CONFIG_TYPE enType, void *plist);
#endif
/********************************************************************************/
/// Get config by GOP.
/// @param u8GOP \b IN:  GOP number
/// @param enType \b IN:  GOP info type
/// @param pstInfo \b IN misc info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GetConfigEx(MS_U8 u8Gop, EN_GOP_CONFIG_TYPE enType, MS_U32 *plist);
#else
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GetConfigEx(MS_U8 u8Gop, EN_GOP_CONFIG_TYPE enType, void *plist);
#endif
//-----------------
// 3D
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for set GWIN 3D Mode
/// @ingroup 3D
/// @param u8GwinId \b IN: GWin ID
/// @param u16MainFBId \b IN: Main-Frame Buffer ID
/// @param u16SubFBId \b IN: Sub-Frame Buffer ID
/// @param EN_3DOSD_Mode \b IN: 3D Osd mode select.
/// @return GOP_API_SUCCESS - Success
DLL_PUBLIC E_GOP_API_Result MApi_GOP_Set3DOSDMode(MS_U8 u8Gwin, MS_U16 u16MainFBId, MS_U16 u16SubFBId,EN_GOP_3D_MODETYPE en3DMode);

//-------------------------------------------------------------------------------------------------
/// API for change L/R frame for GOP 3D function
/// @ingroup 3D
/// @param u8GOP \b IN: GOP ID
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_Set3D_LR_FrameExchange(MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_Set_GPIO3DPin
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_Set_GPIO3DPin(MS_U32 u32GPIO3DPin);
//-------------------------------------------------------------------------------------------------
/// API for Query GOP Capability
/// @ingroup GOP_CAPS
/// @param eCapType \b IN: Capability type
/// @param pRet     \b OUT: return value
/// @param ret_size \b IN: input structure size
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetChipCaps(EN_GOP_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);
#else
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GetChipCaps(EN_GOP_CAPS eCapType, void* pRet, MS_U32 ret_size);
#endif
//-----------------
// GOP_TEST_PATTERN
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set test pattern
/// @ingroup GOP_TEST_PATTERN
/// @param eTstPatternType      \b IN:  test pattern type
/// @param u32ARGB              \b IN   color
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_TestPattern(EN_GOP_TST_PATTERN eTstPatternType, MS_U32 u32ARGB);
//-------------------------------------------------------------------------------------------------
/// Set test pattern alpha
/// @ingroup GOP_TEST_PATTERN
/// @param bTSTAlpha            \b IN:  test pattern alpha
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_TestPatternAlpha_Enable(MS_BOOL bTSTAlpha);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_IsRegUpdated
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_IsRegUpdated(MS_U8 u8GopType);

/*******************************************************************************/
//Get Layer From GOP
//@param u32GOP \b IN: gop id
//@return MS_U32 - the layer id which the gop is on, LAYER_ID_INVALID for invalid
/*******************************************************************************/
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_GetLayerFromGOP(MS_U32 u32GOP);

/*******************************************************************************/
//Get GOP From layer
//@param u32Layer \b IN: layer id
//@return MS_U32 - the gop id which the layer have,  INVALID_GOP_NUM for invalid
/*******************************************************************************/
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_GetGOPFromLayer(MS_U32 u32Layer);

//-----------------
// ENABLE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable GWIN for display
/// @ingroup ENABLE
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Show GWIN
///   - # FALSE Hide GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Enable(MS_U8 winId, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_EnableTransClr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_EnableTransClr(EN_GOP_TRANSCLR_FMT fmt, MS_BOOL bEnable);
//-----------------
// TRANSPARENT_CLR_CEY
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable gop transparent color
/// @ingroup TRANSPARENT_CLR_CEY
/// @param fmt \b IN: RGB mode or u8Index mode color format
/// @param bEnable \b IN:
///   - # TRUE Enable gop transparent color
///   - # FALSE Disable gop transparent color
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_EnableTransClr_EX(MS_U8 u8GOP,EN_GOP_TRANSCLR_FMT fmt, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_EnableProgressive
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_EnableProgressive(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable gop progressive mode
/// @ingroup GOP_OPTION
/// @param u8GOP \b IN:  GOP number
/// @param bEnable \b IN:
///   - # TRUE Enable gop progressive mode
///   - # FALSE Disable gop progressive mode
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_EnableProgressive_EX(MS_U8 u8GOP,MS_BOOL bEnable);
//-----------------
// GWIN_DELETE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Delete the GWIN, free corresponding frame buffer
/// @ingroup GWIN_DELETE
/// @param winId \b IN GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_DeleteWin(MS_U8 winId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_ReleaseWin
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_ReleaseWin(MS_U8 gId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_DestroyWin
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_DestroyWin(MS_U8 gId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_DeleteWinHVSize
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_DeleteWinHVSize(MS_U8 u8GOP, MS_U16 u16HSize, MS_U16 u16VSize);

/*******************************************************************************/
//Set which OSD Layer select which GOP
//@param pGopLayer \b IN:information about GOP and corresponding Layer
//   #u32LayerCounts: the total GOP/Layer counts to set
//   #stGopLayer[i].u32GopIndex :the GOP which need to change Layer
//   #stGopLayer[i].u32LayerIndex :the GOP corresponding Layer
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetLayer(GOP_LayerConfig *pGopLayer, MS_U32 u32SizeOfLayerInfo);
/*******************************************************************************/
//Get which OSD Layer select which GOP
//@param pGopLayer \b OUT:information about GOP and corresponding Layer
//   #u32LayerCounts: the total GOP/Layer inited
//   #stGopLayer[i].u32GopIndex :the GOP which need to change Layer
//   #stGopLayer[i].u32LayerIndex :the GOP corresponding Layer
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetLayer(GOP_LayerConfig *pGopLayer, MS_U32 u32SizeOfLayerInfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_OutputColor
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_OutputColor(EN_GOP_OUTPUT_COLOR type);
//-----------------
// COLOR_TRANSFOR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set gop output color type
/// @ingroup COLOR_TRANSFOR
/// @param type \b IN: gop output color type
///   - # GOPOUT_RGB => RGB mode
///   - # GOPOUT_YUV => YUV mode
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_OutputColor_EX(MS_U8 u8GOP,EN_GOP_OUTPUT_COLOR type);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SwapOverlapWin
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SwapOverlapWin(MS_U8 u8GOP, MS_U8 u8win);
//-----------------
// GWIN_OPTION
//-----------------
//-----------------
// GWIN_ORDER
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GWIN relative Priority
/// Will Ensure Gwin Priority in the same sequence in hw as pGwinPri defined
/// @ingroup GWIN_ORDER
/// @param u8GOP \b IN:  GOP number
/// @param pGwinPri \b IN: Relative Gwin ID Priority Defines
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetRelativeWinPrio(MS_U8 u8GOP, GOP_GwinPri *pGwinPri);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Switch2Gwin
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Switch2Gwin(MS_U8 winId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetAlphaInverse
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetAlphaInverse(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable/Disable gop alpha inverse
/// @param u8GOP \b IN:  GOP number
/// @ingroup GOP_OPTION
/// @param bEnable \b IN: TRUE or FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetAlphaInverse_EX(MS_U8 u8GOP,MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetAlphaInverse
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetAlphaInverse(MS_BOOL *bEnable);
//-------------------------------------------------------------------------------------------------
/// Get gop alpha inverse setting
/// @param u8GOP \b IN:  GOP number
/// @ingroup GOP_OPTION
/// @param bEnable \b OUT: TRUE or FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_GetAlphaInverse_EX(MS_U8 u8GOP,MS_BOOL *bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetPalette
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetPalette(void);

/******************************************************************************/
/// Enable/Disable multi alpha for one gop
/// @param u32GopIdx \b IN: gop ID
/// @param bEnable \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_EnableMultiAlpha(MS_U32 u32GopIdx, MS_BOOL bEnable);

/******************************************************************************/
/// get multi alpha status for one gop
/// @param u32GopIdx \b IN: gop ID
/// @return multi alpha status
/******************************************************************************/
DLL_PUBLIC SYMBOL_WEAK MS_BOOL MApi_GOP_GWIN_IsMultiAlphaEnable(MS_U32 u32GopIdx);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetAlphaValue
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetAlphaValue(MS_U16 * Gwin01AlphaValue, MS_U16 * Gwin23AlphaValue);
DLL_PUBLIC void MApi_GOP_GWIN_SetHScroll(MS_U8 nWinID, MS_U8 nRate, MS_BOOL bEnable);
DLL_PUBLIC void MApi_GOP_GWIN_SetVScroll(MS_U8 nWinID, MS_U8 nRate, MS_BOOL bEnable);
//-----------------
// SCROLL
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for Set Scroll rate for Gwin
/// @ingroup SCROLL
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetScrollRate(MS_U8 u8rate);
//-------------------------------------------------------------------------------------------------
/// Set back ground window
/// @ingroup SCROLL
/// @param u8WinId              \b IN:  GWIN number
/// @param type                 \b IN:  scroll type
/// @param u16Offset            \b IN:  scroll offset
/// @param bEnable              \b IN:  Enable/Disable
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetScrollType(MS_U8 u8WinId, GOP_SCROLL_TYPE type,  MS_U16 u16Offset, MS_BOOL bEnable );
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetSWScrollBuffer(MS_U16 bufNum, MS_U32* pBuffer);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetFMT0TransClr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetFMT0TransClr(MS_U16 clr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetFMT0TransClr_EX
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetFMT0TransClr_EX(MS_U8 u8GOP,MS_U16 clr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetTransClr_8888
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetTransClr_8888(MS_U32 clr, MS_U32 mask);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetTransClr_8888_EX
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetTransClr_8888_EX(MS_U8 u8GOP,MS_U32 clr, MS_U32 mask);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_EnableT3DMode
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_EnableT3DMode(MS_BOOL bEnable);

/********************************************************************************/
/// Set GWIN alpha blending
/// @param u8win \b IN GWIN id
/// @param bEnable \b IN
///   - # TRUE enable pixel alpha
///   - # FALSE disable pixel alpha
/// @param u8coef \b IN alpha blending coefficient (0-7)
/// @return GOP_API_SUCCESS - Success
/// if setting constant alpha blending,please firstly set User_ConsAlpha_bits by calling MApi_GOP_SetConfig(...)
/// example:
/// MS_U32 aConf32[] = {DRV_VALID_8BITS};
/// MApi_GOP_SetConfig(E_GOP_CONSALPHA_BITS,aConf32);
/// MApi_GOP_GWIN_SetBlending(0, FALSE, 0xFF);
/********************************************************************************/
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetBlending(MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetDuplication
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetDuplication(MS_U8 hext, MS_U8 vext);
//-----------------
// SET_GWIN_WINDOW
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set the GWIN's position
/// @ingroup SET_GWIN_WINDOW
/// @param winId  \b IN: GWIN id
/// @param dispX \b IN: x coordinate
/// @param dispY \b IN: y coordinate
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetWinPosition(MS_U8 winId, MS_U16 dispX, MS_U16 dispY);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetWinPositionOffset
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetWinPositionOffset(MS_U8 winId, MS_U16 x, MS_U16 y);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetWinProperty
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetWinProperty(GOP_GwinDispProperty WinProperty);

//-----------------
// GOP_CONFIG
//-----------------
//-----------------
// DST
//-----------------
//-------------------------------------------------------------------------------------------------
/// Configure the destination of a specific GOP
/// @ingroup DST
/// @param u8GOP \b IN : Number of GOP
/// @param dsttype \b IN : GOP destination
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_ENUM_NOT_SUPPORTED - GOP destination not support
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetGOPDst(MS_U8 u8GOP, EN_GOP_DST_TYPE dsttype);
//-----------------
// BLINK
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GWIN Blink
/// @ingroup BLINK
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @param u8rate \b IN Blink frame rate
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetBlink(MS_BOOL bEnable, MS_U8 u8rate);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetFieldInver
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetFieldInver(MS_BOOL enable);
//-------------------------------------------------------------------------------------------------
/// Enable/disable GOP field inverse
/// @param u8GOP \b IN:  GOP number
/// @ingroup GOP_OPTION
/// @param enable  \b IN TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetFieldInver_EX(MS_U8 u8GOP,MS_BOOL enable);
//-------------------------------------------------------------------------------------------------
/// Set stretch window property
/// @ingroup SET_STRETCH_WINDOW
/// @param u8GOP_num \b IN 0: GOP0  1:GOP1
/// @param eDstType \b IN \copydoc EN_GOP_DST_TYPE
/// @param x \b IN stretch window horizontal start position
/// @param y \b IN stretch window vertical start position
/// @param width \b IN stretch window width
/// @param height \b IN stretch window height
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_STRETCHWIN(MS_U8 u8GOP_num, EN_GOP_DST_TYPE eDstType,MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height);
//-------------------------------------------------------------------------------------------------
/// Set stretch window position
/// @ingroup SET_STRETCH_WINDOW
/// @param u8GOPNum \b IN 0: GOP0  1:GOP1
/// @param u16DispX \b IN stretch window horizontal start position
/// @param u16DispY \b IN stretch window vertical start position
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_INVALID_PARAMETERS - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetStretchWinPosition(MS_U8 u8GOPNum, MS_U16 u16DispX, MS_U16 u16DispY);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetHDisplaySize
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetHDisplaySize(MS_U8 gId, MS_U16 Width,MS_U16 X_Start);
//-------------------------------------------------------------------------------------------------
/// Set GWIN information to GOP registers
/// @ingroup SET_GWIN_WINDOW
/// @param u8win \b IN: GWINID
/// @param pinfo \b IN: GWIN Information
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetWinInfo(MS_U8 u8win, GOP_GwinInfo* pinfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetTransClr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetTransClr(EN_GOP_TRANSCLR_FMT fmt, MS_U16 clr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetTransparentClr
/// @param u8GOP \b IN: GOP number
/// @param fmt \b IN:
///  # GOPTRANSCLR_FMT0: RGB mode
///  # GOPTRANSCLR_FMT1: index mode
///  # GOPTRANSCLR_FMT2: ARGB8888 mode
///  # GOPTRANSCLR_FMT3: YUV mode
/// @param st_Transcolor \b IN This structure has an element color which is used for trans color.
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetTransparentClr(MS_U8 u8GOP, EN_GOP_TRANSCLR_FMT fmt, ST_Transparent_Color *st_Transcolor);
//-------------------------------------------------------------------------------------------------
/// Set transparent color for the GWIN
/// @ingroup TRANSPARENT_CLR_CEY
/// @param fmt \b IN:
///  # GOPTRANSCLR_FMT0: RGB mode
///  # GOPTRANSCLR_FMT1: index mode
/// @param clr \b IN Transparent color for in specific color format. Note that this is only for
/// GOPTRANSCLR_FMT0(RGB mode) use. If you use GOPTRANSCLR_FMT1 (index mode), do not care this
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetTransClr_EX(MS_U8 u8GOP,EN_GOP_TRANSCLR_FMT fmt, MS_U16 clr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetWinDispPosition
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetWinDispPosition(MS_U8 gId, MS_U16 dispX, MS_U16 dispY);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetHMirror
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetHMirror(MS_BOOL bEnable);
//-----------------
// MIRROR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GOP H-Mirror
/// @ingroup MIRROR
/// @param u8GOP \b IN : Number of GOP
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetHMirror_EX(MS_U8 u8GOP,MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetVMirror
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetVMirror(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set GOP V-Mirror
/// @ingroup MIRROR
/// @param u8GOP \b IN : Number of GOP
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetVMirror_EX(MS_U8 u8GOP,MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_IsMirrorOn
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_IsMirrorOn(MS_BOOL *bHMirror, MS_BOOL *bVMirror);
//-------------------------------------------------------------------------------------------------
/// Get GOP Mirror status
/// @ingroup MIRROR
/// @param u8GOP \b IN : Number of GOP
/// @param bHMirror          \b IN : H mirror status
/// @param bVMirror          \b IN : V mirror status
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_IsMirrorOn_EX(MS_U8 u8GOP,MS_BOOL *bHMirror, MS_BOOL *bVMirror);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Set_HSCALE
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_HSCALE(MS_BOOL bEnable, MS_U16 src, MS_U16 dst) ;
//-----------------
// SCALE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set stretch window H-Stretch ratio.
/// Example: gwin size:960*540  target gwin size: 1920*1080
///     step1: MApi_GOP_GWIN_Set_HSCALE(TRUE, 960, 1920);
///     step2: MApi_GOP_GWIN_Set_VSCALE(TRUE, 540, 1080);
///     step3: MApi_GOP_GWIN_Set_STRETCHWIN(u8GOPNum, E_GOP_DST_OP0, 0, 0, 960, 540);
/// @ingroup SCALE
/// @param u8GOP \b IN:  GOP number
/// @param bEnable \b IN:
///   - # TRUE enable
///   - # FALSE disable
/// @param src \b IN: original size
/// @param dst \b IN: target size
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_HSCALE_EX(MS_U8 u8GOP,MS_BOOL bEnable, MS_U16 src, MS_U16 dst);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Set_VSCALE
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_VSCALE(MS_BOOL bEnable, MS_U16 src, MS_U16 dst) ;

//-------------------------------------------------------------------------------------------------
/// Set stretch window V-Stretch ratio.
/// Example: gwin size:960*540  target gwin size: 1920*1080
///     step1: MApi_GOP_GWIN_Set_HSCALE(TRUE, 960, 1920);
///     step2: MApi_GOP_GWIN_Set_VSCALE(TRUE, 540, 1080);
///     step3: MApi_GOP_GWIN_Set_STRETCHWIN(u8GOPNum, E_GOP_DST_OP0, 0, 0, 960, 540);
/// @ingroup SCALE
/// @param u8GOP \b IN:  GOP number
/// @param bEnable \b IN:
///   - # TRUE enable
///   - # FALSE disable
/// @param src \b IN: original size
/// @param dst \b IN: target size
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_VSCALE_EX(MS_U8 u8GOP,MS_BOOL bEnable, MS_U16 src, MS_U16 dst);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Set_HStretchMode
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_HStretchMode(EN_GOP_STRETCH_HMODE HStrchMode);
//-------------------------------------------------------------------------------------------------
/// Set GOP H stretch mode
/// @ingroup SCALE
/// @param u8GOP \b IN:  GOP number
/// @param HStrchMode \b IN:
///   - # E_GOP_HSTRCH_6TAPE
///   - # E_GOP_HSTRCH_DUPLICATE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_Set_HStretchMode_EX(MS_U8 u8GOP,EN_GOP_STRETCH_HMODE HStrchMode);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Set_VStretchMode
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_VStretchMode(EN_GOP_STRETCH_VMODE VStrchMode);
//-------------------------------------------------------------------------------------------------
/// Set GOP V stretch mode
/// @ingroup SCALE
/// @param u8GOP \b IN:  GOP number
/// @param VStrchMode \b IN:
///   - # E_GOP_VSTRCH_LINEAR
///   - # E_GOP_VSTRCH_DUPLICATE
///   - # E_GOP_VSTRCH_NEAREST
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_Set_VStretchMode_EX(MS_U8 u8GOP,EN_GOP_STRETCH_VMODE VStrchMode);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Set_TranspColorStretchMode
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_TranspColorStretchMode(EN_GOP_STRCH_TRANSPCOLORMODE  TColorStrchMode);
//-------------------------------------------------------------------------------------------------
/// Set GOP Transparent color Stretch Mode
/// @ingroup SCALE
/// @param u8GOP \b IN:  GOP number
/// @param VStrchMode \b IN:
///   - # E_GOP_TRANSPCOLOR_STRCH_DUPLICATE
///   - # E_GOP_TRANSPCOLOR_STRCH_ASNORMAL
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_Set_TranspColorStretchMode_EX(MS_U8 u8GOP,EN_GOP_STRCH_TRANSPCOLORMODE  TColorStrchMode);
//-----------------
// GOP_UPDATE
//-----------------
//-----------------
// FORCE_WRITE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GOP force write mode for update register. When enable force write mode, all update gop registers action will directly
/// take effect (do not wait next v-sync to update gop register!).
/// @ingroup FORCE_WRITE
/// @param bEnable \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetForceWrite(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set GOP bank force write mode for update register. When enable bank force write mode, update gop num registers action will directly
/// take effect (do not wait next v-sync to update gop register!).
/// @ingroup FORCE_WRITE
/// @param bEnable \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetBnkForceWrite(MS_U8 u8GOP, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetAlphaValue
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetAlphaValue(MS_U16 * Gwin01AlphaValue, MS_U16 * Gwin23AlphaValue);
//-------------------------------------------------------------------------------------------------
/// Get GWin position info
/// @ingroup SET_GWIN_WINDOW
/// @param winId  \b IN: gwin id
/// @param dispX \b OUT: position-X
/// @param dispY \b OUT: position-Y
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetWinPosition(MS_U8 winId, MS_U16 * dispX, MS_U16 *dispY);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetWinProperty
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetWinProperty(GOP_GwinDispProperty *WinProperty);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetWinAttr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetWinAttr(MS_U8 gId, MS_U16 *x0, MS_U16 *y0, MS_U16 *width, MS_U16 *height,  MS_U16 *fbPitch, MS_U16 *fbFmt, MS_PHY *addr, MS_U32 *size);
//-------------------------------------------------------------------------------------------------
/// Open API function:
///     Read GWIN information from GOP registers
/// @ingroup SET_GWIN_WINDOW
/// @param u8win \b IN  GWINID
/// @param pinfo \b OUT buffer to store GWIN information
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetWinInfo(MS_U8 u8win, GOP_GwinInfo* pinfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_UpdateRegOnceEx
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnceEx(MS_BOOL bWriteRegOnce, MS_BOOL bSync);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_UpdateRegOnceEx2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnceEx2(MS_U8 u8GOP,MS_BOOL bWriteRegOnce, MS_BOOL bSync);

//******************************************************************************
/// Extend MApi_GOP_GWIN_UpdateRegOnceEx, update special gop.
/// Set gop update register method by only once.
/// Example: if you want to update GOP function A, B, C in the same V sync, please write down your code like below
/// MApi_GOP_GWIN_UpdateRegOnceByMask(u16GopMask,TRUE, TRUE);
/// GOP_FUN_A;
/// GOP_FUN_B;
/// GOP_FUN_C;
/// MApi_GOP_GWIN_UpdateRegOnceByMask(u16GopMask,FALSE, TRUE);
/// @param u16GopMask    \b IN:bit0-gop0, bit1-gop1...
/// @param bWriteRegOnce    \b IN: TRUE/FALSE
/// @param bSync            \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnceByMask(MS_U16 u16GopMask,MS_BOOL bWriteRegOnce, MS_BOOL bSync);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_TriggerRegWriteIn
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_TriggerRegWriteIn(MS_U8 u8win, MS_BOOL bForceWriteIn);
//-----------------
// UPDATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for Trigger reg writes in
/// @ingroup UPDATE
/// @param u8win \b IN: GWin ID
/// @param bForceWriteIn \b IN:
///   - # TRUE , Register will take effect immediately
///   - # FALSE, Register will take effect after the next vsync
/// @param bSync \b IN:
///   - # FALSE , Register will fire without waiting vsync (driver will not polling the vsync)
///   - # TRUE, Register will fire and waiting vsync
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_TriggerRegWriteIn_Ex(MS_U8 u8win, MS_BOOL bForceWriteIn, MS_BOOL bSync);
//-----------------
// FADING
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set gwin fade in/out
/// @ingroup FADING
/// @param gId  \b IN: gwin id
/// @param Width \b IN: gwin display width
/// @param X_Start \b IN: gwin display H display starting position
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetFadeInOut(MS_U8 gId, EN_GOP_FADE_TYPE eGwinFadeType, MS_BOOL bEnable, MS_U8 u8FadeRate);
//-----------------
// SHARE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GWin Attribute to Shared. If shared GWin, More than one process could
/// access this GWin.
/// @ingroup SHARE
/// @param winId \b IN: GWIN ID for shared
/// @param bIsShared \b IN: shared or not
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetGWinShared(MS_U8 winId, MS_BOOL bIsShared);
//-------------------------------------------------------------------------------------------------
/// Set Reference cnt of shared GWin.
/// @ingroup SHARE
/// @param winId \b IN: GWIN ID for shared
/// @param u16SharedCnt \b IN: shared reference cnt.
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetGWinSharedCnt(MS_U8 winId, MS_U16 u16SharedCnt);
//-----------------
// TILE_MODE
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for DIP Tile mode for GWIN
/// @ingroup TILE_MODE
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @param tiletype \b IN: tile type
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_EnableTileMode(MS_U8 winId, MS_BOOL bEnable, EN_GOP_TILE_DATA_TYPE tiletype );
//-----------------
// PINPON
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for set gop pinpong
/// @ingroup PINPON
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetPINPON(MS_U8 winId, MS_BOOL bEn,GOP_PINPON_MODE PINPON_Mode);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_SetGOPYUV
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetGOPYUV(MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
//Set which MUX select which GOP of mixer, when different gop do the alpha blending
/// @ingroup GOP_MIXER
/// @param u8GOP \b IN:  GOP number
/// @param u8Mux \b IN:  Mux number
/// @param bEn \b IN:
///   - # TRUE Enable u8GOP to mixer u8Mux
///   - # FALSE Disable u8GOP to mixer u8Mux
//@return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_MIXER_SetMux(MS_U8 u8GOP,MS_U8 u8Mux, MS_BOOL bEn);
//-----------------
// EXTRA
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for set GWIN New Alpha Mode
/// @ingroup EXTRA
/// @param gWinId \b IN: GWin ID
/// @param bEnable \b IN:
///   - # TRUE enable new alpha mode
///   - # FALSE disable new alpha mode
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetNewAlphaMode(MS_U8 gWinId, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// API for set GWIN Pre Alpha Mode
/// @ingroup EXTRA
/// @param gWinId \b IN: GWin ID
/// @param bEnable \b IN:
///   - # TRUE enable new alpha mode
///   - # FALSE disable new alpha mode
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetPreAlphaMode(MS_U8 u8GOP, MS_BOOL bEnble);
//-----------------
// GWIN
//-----------------
//-----------------
// GWIN_CREATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Create GWIN
/// @ingroup GWIN_CREATE
/// @param width \b IN: GWIN width
/// @param height \b IN: GWIN height
/// @param fbFmt \b IN: frame buffer pixel format
///   - #0xFFFF default FB format
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateWin(MS_U16 width, MS_U16 height, MS_U16 fbFmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateWin2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateWin2(MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateWin_Assign_FB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateWin_Assign_FB(MS_U8 u8GOP, MS_U8 FBId, MS_U16 dispX, MS_U16 dispY);
//-------------------------------------------------------------------------------------------------
/// Create GWIN and assign frame buffer to this GWIN
/// @ingroup GWIN_CREATE
/// @param u8GOP \b IN: GOP number
/// @param FBId \b IN: frame buffer ID
/// @param dispX \b IN: frame buffer starting x position
/// @param dispY \b IN: frame buffer starting y position
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateWin_Assign_32FB(MS_U8 u8GOP, MS_U32 u32FBId, MS_U16 dispX, MS_U16 dispY);
//-------------------------------------------------------------------------------------------------
/// Create given GWIN id
/// @ingroup GWIN_CREATE
/// @param u8WinId \b IN: GWIN id
/// @param width \b IN: GWIN width
/// @param height \b IN: GWIN height
/// @param fbFmt \b IN: GWIN color format
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateStaticWin(MS_U8 u8WinId, MS_U16 width, MS_U16 height, MS_U16 fbFmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateStaticWin2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateStaticWin2(MS_U8 u8WinId, MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateStaticWin_Assign_FB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateStaticWin_Assign_FB(MS_U8 u8WinId, MS_U8 u8GOP, MS_U8 FBId, MS_U16 dispX, MS_U16 dispY);

//-------------------------------------------------------------------------------------------------
/// Create GWIN by given GWIN id and frame buffer id
/// @ingroup GWIN_CREATE
/// @param u8WinId \b IN: GWIN id
/// @param u8GOP \b IN: GOP number
/// @param FBId \b IN: frame buffer id
/// @param dispX \b IN: gwin's frame buffer starting x
/// @param dispY \b IN: gwin's frame buffer starting y
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateStaticWin_Assign_32FB(MS_U8 u8WinId, MS_U8 u8GOP, MS_U32 u32FBId, MS_U16 dispX, MS_U16 dispY);

//-----------------
// GWIN_STATUS
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check if GWIN is created
/// @ingroup GWIN_STATUS
/// @param gId \b IN: GWINID
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_IsGwinExist(MS_U8 gId);

//-------------------------------------------------------------------------------------------------
/// Get the free GWIN id
/// @ingroup GWIN_STATUS
/// @return the GWIN id
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetFreeWinID(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetMAXWinID
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetMAXWinID(void);
//-------------------------------------------------------------------------------------------------
/// Get current active GWIN id
/// @ingroup GWIN_STATUS
/// @return current active GWIN id
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetCurrentWinId(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetMaxActiveGwin_v
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_GWIN_GetMaxActiveGwin_v(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Get_HSTART
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_GWIN_Get_HSTART(void);


//-------------------------------------------------------------------------------------------------
/// Get total active gwin number
/// @ingroup GWIN_STATUS
/// @return total active gwin number
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_GWIN_GetActiveGWIN(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetMaxVEnd
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_GWIN_GetMaxVEnd(MS_BOOL bActive);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_IsAllGWINDisabled
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_GWIN_IsAllGWINDisabled(void);

//-------------------------------------------------------------------------------------------------
/// Check if all some GWIN is currently enabled
/// @ingroup ENABLE
/// @param  winId \b IN: gwin id
/// @return  - the according GWin is enabled or not
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_GWIN_IsGWINEnabled(MS_U8 winId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_IsEnabled
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_GWIN_IsEnabled(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CheckOpmodeIsOn
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_GWIN_CheckOpmodeIsOn(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CheckOpmodeIsOn_EX
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK MS_BOOL MApi_GOP_GWIN_CheckOpmodeIsOn_EX(MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetResolution
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetResolution(MS_U8 u8GwinId, MS_U8 u8FbId, GOP_GwinInfo* pGwinInfo, GOP_StretchInfo* pStretchInfo, EN_GOP_STRETCH_DIRECTION direction, MS_U16 u16DstWidth, MS_U16 u16DstHeight);
//-------------------------------------------------------------------------------------------------
/// API for set GWIN resolution in one function
/// @ingroup SET_STRETCH_WINDOW
/// @param u8GwinId \b IN: GWin ID
/// @param u8FbId \b IN: Frame Buffer ID
/// @param pGwinInfo \b IN: pointer to GOP_GwinInfo structure
/// @param pStretchInfo \b IN: pointer to GOP_StretchInfo
/// @param direction \b IN: to decide which direction to stretch
/// @param u16DstWidth \b IN: set scaled width if H direction is specified
/// @param u16DstHeight \b IN: set scaled height if V direction is specified
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetResolution_32FB(MS_U8 u8GwinId, MS_U32 u32FbId, GOP_GwinInfo* pGwinInfo, GOP_StretchInfo* pStretchInfo, EN_GOP_STRETCH_DIRECTION direction, MS_U16 u16DstWidth, MS_U16 u16DstHeight);
//-------------------------------------------------------------------------------------------------
/// GOP Exit
/// @ingroup INIT
/// @param  void                \b IN: none
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC void MApi_GOP_Exit(void);


//======================================================================================
//  GOP FB management
//======================================================================================
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_FB_SEL
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_FB_SEL(MS_U8 u8FB);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetDoubleHeap
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetDoubleHeap(MS_U32 u32DblHeapAdr, MS_U32 u32DblHeapLen);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_MapFB2Win
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_MapFB2Win(MS_U8 fbId, MS_U8 gwinId);
//-----------------
// FLIP
//-----------------
//-------------------------------------------------------------------------------------------------
/// Change a GWIN's frame buffer, this enables an off screen buffer to be shown
/// @ingroup FLIP
/// @param fbId \b IN frame buffer id
/// @param gwinId \b IN \copydoc GWINID
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Map32FB2Win(MS_U32 u32fbId, MS_U8 u8gwinId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Switch2FB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Switch2FB(MS_U8 fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Switch2_32FB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Switch2_32FB(MS_U32 u32fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_Switch_GWIN_2_FB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_Switch_GWIN_2_FB(MS_U8 gWinId, MS_U8 fbId, MS_U16 u16WaitTagID, MS_U16 *pU16QueueCnt);
//-------------------------------------------------------------------------------------------------
/// Set GOP color matrix
/// @ingroup COLOR_TRANSFOR
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetR2YMatrix(MS_U8 u8GOP,ST_GOP_R2Y_Matrix *matrix);
//-------------------------------------------------------------------------------------------------
/// Set Flip GWIN frame buffer
/// @ingroup FLIP
/// @param gWinId \b IN: gwin id
/// @param fbId \b IN: frame buffer id
/// @param u16WaitTagID \b IN: WaitTagID
/// @param pU16QueueCnt \b IN: Queue count
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_Switch_GWIN_2_32FB(MS_U8 gWinId, MS_U32 u32fbId, MS_U32 u32WaitTagID, MS_U32 *pU32QueueCnt);
//-------------------------------------------------------------------------------------------------
/// Set Flip GWIN frame buffer by addr
/// @ingroup FLIP
/// @param gWinId \b IN: gwin id
/// @param u32FlipAddr \b IN: addr
/// @param u16WaitTagID \b IN: WaitTagID
/// @param pU16QueueCnt \b IN: Queue count
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_Switch_GWIN_2_FB_BY_ADDR(MS_U8 gWinId, MS_PHY phyFlipAddr, MS_U16 u16WaitTagID, MS_U16 *pU16QueueCnt);
//-------------------------------------------------------------------------------------------------
/// Set 3D Flip GWIN frame buffer by addr
/// @ingroup FLIP
/// @param gWinId \b IN: gwin id
/// @param u32MainFlipAddr \b IN: frame buffer id
/// @param u32SubFlipAddr \b IN: frame buffer id
/// @param u16WaitTagID \b IN: WaitTagID
/// @param pU16QueueCnt \b IN: Queue count
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_Switch_3DGWIN_2_FB_BY_ADDR(MS_U8 gWinId, MS_PHY phyMainFlipAddr, MS_PHY phySubFlipAddr, MS_U16 u16WaitTagID, MS_U16 *pU16QueueCnt);
//-------------------------------------------------------------------------------------------------
/// Set Multi GOP Flip GWIN frame buffer by addr
/// @ingroup FLIP
/// @param gWinId \b IN: gwin id
/// @param fbId \b IN: frame buffer id
/// @param u16WaitTagID \b IN: WaitTagID
/// @param pU16QueueCnt \b IN: Queue count
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MApi_GOP_Switch_Multi_GWIN_2_FB_BY_ADDR(GOP_MultiFlipInfo GopMultiFlipInfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_DeleteFB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_DeleteFB(MS_U8 fbId);

//-----------------
// FB_DELETE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Destroy a frame buffer
/// @ingroup FB_DELETE
/// @param fbId \b IN frame buffer id
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Delete32FB(MS_U32 u32fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetFBRegion
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetFBRegion(MS_U8 fbID, MS_U16 *pX,MS_U16 *pY, MS_U16 *pW, MS_U16 *pH);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Get32FBRegion
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Get32FBRegion(MS_U32 u32fbID, MS_U16 *pX, MS_U16 *pY, MS_U16 *pW, MS_U16 *pH);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetFBPosition
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetFBPosition(MS_U8 fbID, MS_U16 *pX,MS_U16 *pY);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Get32FBPosition
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Get32FBPosition(MS_U32 u32fbID, MS_U16 *pX, MS_U16 *pY);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetFBInfo
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr);
//-----------------
// SET
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set frame buffer info
/// @ingroup SET
/// @param fbId  \b IN: frame buffer id
/// @param fbAttr \b IN: frame buffer attribute
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set32FBInfo(MS_U32 u32fbId, GOP_GwinFBAttr *fbAttr);

//-------------------------------------------------------------------------------------------------
/// Get frame buffer info
/// @ingroup SET
/// @param fbId  \b IN: frame buffer id
/// @param fbAttr \b IN: frame buffer attribute
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Get32FBInfo
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Get32FBInfo(MS_U32 u32fbId, GOP_GwinFBAttr *fbAttr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetFBAddr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_GetFBAddr(MS_U8 fbID, MS_PHY *pAddr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Get32FBAddr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Get32FBAddr(MS_U32 u32fbID, MS_PHY *pAddr);
//-------------------------------------------------------------------------------------------------
/// API for Clear GOP GWIN FlipQueue
/// @ingroup FLIP
/// @param u8GwinId \b IN: GWin ID
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_ClearFlipQueue(MS_U8 gWinId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetMaxFBNum
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetMaxFBNum(void);
//-------------------------------------------------------------------------------------------------
/// Get maximum FB number by all gop
/// @ingroup GOP_CAPS
/// @param  void                \b IN: none
/// @return maximum fb number by all gop
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_GetMax32FBNum(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_FB_Get
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_FB_Get(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetMAXFBID
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetMAXFBID(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetMAX32FBID
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_GetMAX32FBID(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_DestroyFB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_DestroyFB(MS_U8 fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Destroy32FB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Destroy32FB(MS_U32 u32fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateFBFrom3rdSurf
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateFBFrom3rdSurf(MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr, MS_U16 pitch, MS_U8* ret_FBId);
//-------------------------------------------------------------------------------------------------
/// Create frame buffer from given dram address
/// @ingroup FB_CREATE
/// @param width  \b IN: frame buffer width
/// @param height \b IN: frame buffer height
/// @param fbFmt \b IN: frame buffer color format
/// @param u32FbAddr \b IN: frame buffer starting address
/// @param pitch \b IN: frame buffer pitch
/// @param ret_FBId \b OUT: frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Create32FBFrom3rdSurf(MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr, MS_U16 pitch, MS_U32* u32ret_FBId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateFBbyStaticAddr
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateFBbyStaticAddr(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr );
//-------------------------------------------------------------------------------------------------
/// Create frame buffer from given frame buffer id and dram address. User should call MApi_GOP_GWIN_GetFree32FBID()
/// first and then use this frame buffer id as this function input parameter
/// @ingroup FB_CREATE
/// @param fbId  \b IN: frame buffer id
/// @param dispX \b IN: frame buffer starting x
/// @param dispY \b IN: frame buffer starting y
/// @param width \b IN: frame buffer width
/// @param height \b IN: frame buffer height
/// @param fbFmt \b IN: frame buffer color format
/// @param u32FbAddr \b IN: frame buffer starting address
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Create32FBbyStaticAddr(MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateFBbyStaticAddr2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateFBbyStaticAddr2(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr, EN_GOP_FRAMEBUFFER_STRING FBString);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Create32FBbyStaticAddr2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Create32FBbyStaticAddr2(MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr ,EN_GOP_FRAMEBUFFER_STRING FBString);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateFB2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateFB2(MS_U8 FBId,MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Create32FB2
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Create32FB2(MS_U32 u32FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateFB2_EXT
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateFB2_EXT(MS_U8 FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Create32FB2_EXT
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Create32FB2_EXT(MS_U32 u32FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetCurrentFBID
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetCurrentFBID(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetCurrent32FBID
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_GetCurrent32FBID(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetFBfromGWIN
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8  MApi_GOP_GWIN_GetFBfromGWIN(MS_U8 gwinId) ;
//-------------------------------------------------------------------------------------------------
/// Get frame bufferid from the gwin id
/// @ingroup FB_STATUS
/// @param gwinId  \b IN gwin id
/// @return return frame buffer id. If return 0xFF, it represents this function fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_Get32FBfromGWIN(MS_U8 gwinId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_IsFBExist
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_IsFBExist(MS_U8 fbId);
//-----------------
// FB_STATUS
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check if a frame buffer is allocated
/// @ingroup FB_STATUS
/// @param fbId \b IN frame buffer id
/// @return
///   - # TRUE fbId is allocated
///   - # FALSE fbId is invalid or not allocated
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Is32FBExist(MS_U32 u32fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetFreeFBID
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_GetFreeFBID(void);
//-------------------------------------------------------------------------------------------------
/// Get free frame buffer id
/// @ingroup FB_STATUS
/// @return frame buffer id. If return oxFF, it represents no free frame buffer id for use.
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_GOP_GWIN_GetFree32FBID(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_CreateFB
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_CreateFB(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt);
//-----------------
// FB_CREATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Create frame buffer from given frame buffer id . User should call MApi_GOP_GWIN_GetFree32FBID()
/// first and then use this frame buffer id as this function input parameter
/// @ingroup FB_CREATE
/// @param fbId  \b IN frame buffer id
/// @param dispX \b IN display x
/// @param dispY \b IN display y
/// @param width \b IN width
/// @param height \b IN height
/// @param fbFmt \b IN frame buffer format
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_CRT_GWIN_NOAVAIL - Failure
DLL_PUBLIC MS_U8 MApi_GOP_GWIN_Create32FB(MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height, MS_U16 fbFmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_GetFBFmt
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//----------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_GWIN_GetFBFmt(MS_U8 fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_Get32FBFmt
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_GWIN_Get32FBFmt(MS_U32 u32fbId);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetPaletteOpt
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetPaletteOpt( GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, EN_GOP_PALETTE_TYPE ePalType);
//-----------------
// PALETTE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Write optional palette values to GOP0 registers
/// Note that this GOP0 palette table only support 256 group palette data
/// @ingroup PALETTE
/// @param u8GOP \b IN : Number of GOP
/// @param *pPalArray \b IN Pointer to pallete list gonna set into pallete table
/// @param u32PalStart \b IN start of pallete table gonna be overwrite
/// @param u32PalEnd \b IN end of pallete table gonna be overwrite
/// @param ePalType \b IN Pallete format, true/false ARGB8888
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetPaletteOpt_EX(MS_U8 u8GOP,GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, EN_GOP_PALETTE_TYPE ePalType);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetPaletteRead
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_SetPaletteRead(EN_GOP_PALETTE_READTYPE type);
//-------------------------------------------------------------------------------------------------
/// Set GOP0 palette table read out control
/// @ingroup PALETTE
/// @param u8GOP \b IN : Number of GOP
/// @param type \b IN \copydoc EN_GOP_PALETTE_READTYPE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetPaletteRead_EX(MS_U8 u8GOP,EN_GOP_PALETTE_READTYPE type);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_ReadPalette
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_ReadPalette(MS_U8 idx, MS_U32* clr);
//-------------------------------------------------------------------------------------------------
/// Read GOP0 palette values From Pallete
/// @ingroup PALETTE
/// @param u8GOP \b IN : Number of GOP
/// @param idx \b IN Index for pallete wanna retrieve
/// @param clr \b OUT color of specific pallete index
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_ReadPalette_EX(MS_U8 u8GOP,MS_U8 idx, MS_U32* clr);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GetInfo
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteOpt(GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd);
//-------------------------------------------------------------------------------------------------
/// Write optional palette values to GOP1 registers
/// Note that this GOP1 palette table only support 64 group palette data
/// @ingroup PALETTE
/// @param u8GOP \b IN : Number of GOP
/// @param *pPalArray \b IN Pointer to pallete list gonna set into pallete table
/// @param u32PalStart \b IN start of pallete table gonna be overwrite
/// @param u32PalEnd \b IN end of pallete table gonna be overwrite
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteOpt_EX(MS_U8 u8GOP,GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_2GSetPaletteRead
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteRead(EN_GOP_PALETTE_READTYPE type);
//-------------------------------------------------------------------------------------------------
/// Set GOP1 palette table read out control
/// @ingroup PALETTE
/// @param u8GOP \b IN : Number of GOP
/// @param type \b IN \copydoc EN_GOP_PALETTE_READTYPE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteRead_EX(MS_U8 u8GOP,EN_GOP_PALETTE_READTYPE type);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_2GReadPalette
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_2GReadPalette(MS_U8 idx, MS_U32* clr);
//-------------------------------------------------------------------------------------------------
/// Read GOP2G(GOP1) palette values From Pallete
/// @ingroup PALETTE
/// @param u8GOP \b IN : Number of GOP
/// @param idx \b IN Index for pallete wanna retrieve
/// @param clr \b OUT color of specific pallete index
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_2GReadPalette_EX(MS_U8 u8GOP,MS_U8 idx, MS_U32* clr);


//-----------------
// DWIN
//-----------------
//-----------------
// DWIN_CONFIG
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for initialize the GOP DWIN
/// @ingroup DWIN_CONFIG
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_Init(void);

//-------------------------------------------------------------------------------------------------
/// Config Dwin source (OP or MVOP).
/// @ingroup DWIN_CONFIG
/// @param enScanMode \b IN: EN_GOP_DWIN_SCAN_MODE
/// @return GOP_API_SUCCESS - Success
///            GOP_API_INVALID_PARAMETERS - don't support this dwin source
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetSourceSel(EN_GOP_DWIN_SRC_SEL enSrcSel);
//-------------------------------------------------------------------------------------------------
/// Set gop dwin capture one frame
/// @ingroup DWIN_CAPTURE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_CaptureOneFrame2(void);
//-----------------
// DWIN_INTERRUPT
//-----------------
//-------------------------------------------------------------------------------------------------

/// Set interrupt mask of GOP DWIN.
/// @ingroup DWIN_INTERRUPT
/// @param u8mask \b IN bitmask of interrupt type
///   - bit7 DWIN VSYNC interrupt mask
///   - bit6 Dump Window interlace Bottom Field ACK Interrupt Mask
///   - bit5 Dump Window interlace Top Field ACK Interrupt Mask
///   - bit4 Dump Window Progressive ACK Interrupt Mask
///   - bit3:1 Reserved
///   - bit0 GWIN VSYNC Interrupt Mask
/// @param bEnable \b IN
///   - # TRUE enable interrupts specified by u8mask
///   - # FALSE disable interrupts specified by u8mask
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_EnableIntr(MS_U16 u8mask, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set gop dwin capture one frame
/// @ingroup DWIN_CAPTURE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_CaptureOneFrame(void);
//-----------------
// DWIN_CAPTURE
//-----------------
//-------------------------------------------------------------------------------------------------
/// API for enable or disable DWIN
/// @ingroup DWIN_CAPTURE
/// @param bEnable \b IN DWIN enable or not
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_Enable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get DWIN property
/// @ingroup DWIN_INTERRUPT
/// @param pproperty \b OUT buffer to store DWIN property
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_GetWinProperty(GOP_DwinProperty *pproperty);
//-------------------------------------------------------------------------------------------------
/// Set DWIN property
/// @ingroup DWIN_INTERRUPT
/// @param pproperty \b IN buffer to store DWIN property
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetWinProperty(GOP_DwinProperty *pproperty);
//-------------------------------------------------------------------------------------------------
/// Set DWIN data source scan type
/// @ingroup DWIN_CONFIG
/// @param enScanMode \b IN: EN_GOP_DWIN_SCAN_MODE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SelectSourceScanType(EN_GOP_DWIN_SCAN_MODE enScanMode);
//-------------------------------------------------------------------------------------------------
/// Set DWIN Capture format
/// @ingroup DWIN_CONFIG
/// @param fmt \b IN: buffer format to store DWIN content
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetDataFmt(EN_GOP_DWIN_DATA_FMT fmt);
//-------------------------------------------------------------------------------------------------
/// Set DWIN Capture constant alpha value for format ARGB8888
/// @ingroup DWIN_CONFIG
/// @param u8AlphaVal \b IN: constant alpha value
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetAlphaValue(MS_U8 u8AlphaVal);
//-----------------
// DWIN_OPTION
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set DWIN Capture alpha source
/// @ingroup DWIN_OPTION
/// @param source \b IN: alpha source
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetAlphaSrc(EN_GOP_DWIN_ALPHA_SRC source);
//-------------------------------------------------------------------------------------------------
/// Set DWIN Capture alpha inverse
/// @ingroup DWIN_OPTION
/// @param source \b IN: enable alpha inverse
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetAlphaInverse(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set DWIN UVSample mode
/// @ingroup DWIN_OPTION
/// @param mode \b IN: UVSample mode
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetUVSample(EN_GOP_DWIN_UVSAMPLE_Mode mode);
//-------------------------------------------------------------------------------------------------
/// Set DWIN skipframe count
/// @ingroup DWIN_OPTION
/// @param mode \b IN: skipframe count
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetSkipFrame(MS_U32 framecount);
//-------------------------------------------------------------------------------------------------
/// clear DWIN interrupt
/// @ingroup DWIN_INTERRUPT
/// @param u8mask \b IN: interrup mask
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_ClearIntr(MS_U16 u8mask);

//-------------------------------------------------------------------------------------------------
/// Get DWIN interrupt Information.
/// @ingroup DWIN_CAPTURE
/// @param pDWinIntInfo \b OUT: dwin interrup flag
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_GetDWinIntInfo(GOP_DWinIntInfo *pDWinIntInfo);
//-------------------------------------------------------------------------------------------------
/// Get DWIN interrupt Information.
/// @ingroup DWIN_CAPTURE
/// @param pDWinIntInfo \b OUT: dwin interrup flag
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_GetDWinIntInfoTimeout(GOP_DWinIntInfo *pDWinIntInfo, MS_U32 u32Timeout);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_SetClkForCapture
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_SetClkForCapture(void);   //only for source is OP path clock
//-------------------------------------------------------------------------------------------------
/// Get GOP Dwin interrupt status
/// @ingroup DWIN_INTERRUPT
/// @return GOP Dwin interrupt status
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_GOP_DWIN_GetIntrStatus(void);
//-------------------------------------------------------------------------------------------------
/// API for enable or disable DWIN
/// @ingroup DWIN_OPTION
/// @param bEnable \b IN DWIN enable or not
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_EnableR2YCSC(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// API for enable or disable DWIN UV SWAP
/// @ingroup DWIN_OPTION
/// @param bEnable \b IN DWIN enable or not
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetUVSwap(MS_BOOL bEnable);
//-----------------
// GOP_LOCK_BEGINE_END
//-----------------
//-------------------------------------------------------------------------------------------------
/// Begine of lock GOP.
/// @ingroup GOP_LOCK_BEGINE_END
/// @param  void
/// @return GOP_API_SUCCESS     - Success
/// @return GOP_API_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_BeginDraw(void);
//-------------------------------------------------------------------------------------------------
/// End of lock GOP.
/// @ingroup GOP_LOCK_BEGINE_END
/// @param  void
/// @return GOP_API_SUCCESS     - Success
/// @return GOP_API_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_EndDraw(void);
//-------------------------------------------------------------------------------------------------
/// Lock Force Write
/// @ingroup FORCE_WRITE
/// @param  void
/// @return GOP_API_SUCCESS     - Success
/// @return GOP_API_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_LockForForceWrite(MS_BOOL bLock);
//-----------------
// BG
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable back ground window
/// @ingroup BG
/// @param bgwID \b IN:  GWIN number
/// @param bEnable \b IN: ENABLE/DISABLE
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Enable_BGWIN(MS_U8 bgwID, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set back ground window
/// @ingroup BG
/// @param bgwID            \b IN:  GWIN number
/// @param x0               \b IN:  left position
/// @param y0               \b IN:  top position
/// @param w                \b IN:  width
/// @param h                \b IN:  height
/// @param color            \b IN:  color
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_BGWIN(MS_U8 bgwID, MS_U16 x0, MS_U16 y0, MS_U16 w, MS_U16 h, MS_U16 color);
//-------------------------------------------------------------------------------------------------
/// Set back ground window
/// @ingroup BG
/// @param bgwID            \b IN:  GWIN number
/// @param bEnable          \b IN:  ENABLE/DISABLE
/// @param alpha            \b IN:  alpha
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_Set_BGWIN_Alpha(MS_U8 bgwID, MS_BOOL bEnable, MS_U8 alpha);

//-------------------------------------------------------------------------------------------------
/// API for set gop pinpong
/// @ingroup DWIN_OPTION
/// @param winId \b IN: GWIN id
/// @param u32fbaddr0   \b IN:  pinpon addr0
/// @param u32fbaddr1   \b IN:  pinpon addr1
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_DWIN_SetBufferPINPON(MS_PHY phyfbaddr0,MS_PHY phyfbaddr1);

//-----------------
// GOP_CB
//-----------------
//-------------------------------------------------------------------------------------------------
/// GOP call back function: Set destination buffer pitch,addr,fmt to GE.
/// @ingroup GOP_CB
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RegisterFBFmtCB(MS_U32 (*fpGOP_CB)(MS_U16 pitch,MS_PHY addr , MS_U16 fmt )  );
//-------------------------------------------------------------------------------------------------
/// GOP call back function: Get vedio status (is interlace mode?) from scaler.
/// @ingroup GOP_CB
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RegisterXCIsInterlaceCB(MS_BOOL (*fpGOP_CB)(void));
//-------------------------------------------------------------------------------------------------
/// GOP call back function: Get capture window h-start from scaler.
/// @ingroup GOP_CB
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RegisterXCGetCapHStartCB(MS_U16 (*fpGOP_CB)(void));

//-------------------------------------------------------------------------------------------------
/// GOP call back function: Reduce scaler bandwidth when OSD on.
/// @ingroup GOP_CB
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RegisterXCReduceBWForOSDCB(void (*fpGOP_CB)(MS_U8 PqWin, MS_BOOL enable));
//-------------------------------------------------------------------------------------------------
/// GOP call back function: Notify callback for the end of GOP events such as buffer rearrange.
/// @ingroup GOP_CB
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RegisterEventNotify(void (*fpGOP_CB)(MS_U32 u32EventID, void* reserved0));

//-------------------------------------------------------------------------------------------------
/// GOP call back function: Register XC DIP call back
/// @ingroup GOP_CB
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_RegisterXCSetDwinInfo(MS_BOOL (*fpGOP_CB)(MS_U16 cmd,MS_U32 *arg,MS_U16 size));

//-------------------------------------------------------------------------------------------------
/// MApi_GOP_OC_SetOCFBinfo
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_OC_SetOCFBinfo(MS_PHY phyAddr, EN_GOP_OC_SEL_TYPE BankMode, MS_U32 enBufSize);

//-------------------------------------------------------------------------------------------------
/// Set GPU Tile Mode
/// @ingroup TILE_MODE
/// @param u8win \b IN GWIN id
/// @param TileMode \b IN
///   - # TRUE Gwin Tile Mode
///   - # FALSE Gwin Tile Mode
/// @param tiletype \b IN: tile type
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetGPUTile(MS_U8 u8win, EN_GOP_GPU_TILE_MODE TileMode);

//-------------------------------------------------------------------------------------------------
/// MApi_GOP_TLB_Enable
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_TLB_Enable(MS_U8 u8GOP,MS_BOOL bEnable, MS_PHY phytlbaddr);

//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_UpdateRegOnce
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnce(MS_BOOL bWriteRegOnce);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetMux
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetMux(GOP_MuxConfig * pGopMuxConfig, MS_U32 u32SizeOfMuxInfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_OutputLayerSwitch
/// @ingroup GOP_ToBeRemove
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_OutputLayerSwitch(MS_U8 u8GOP);
//-------------------------------------------------------------------------------------------------
/// MApi_GOP_MultiWin_ColorKey
/// @ingroup GOP MultiWindow ColorKey
/// @param mwinId \b Window index selection
/// @param eMultiWinType \b IN MultiWin ColorKey mode selection
/// @param pGOP_WinColorKey \b IN MultiWin ColorKey Information
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_MultiWin_ColorKey(MS_U8 mwinId, EN_GOP_MULTIWIN_COLOR_MODE eMultiWinType, GOP_MULTIWIN_COLORKEY* pGOP_WinColorKey);
//--------------------------------------------------------------------------------------------------
/// MApi_GOP_MultiWin_FrameColor
/// @ingroup GOP MultiWindow ColorKey
/// @param eMultiWinType \b IN MultiWin ColorKey mode selection
/// @param pGOP_WinFrameColor \b IN MultiWin FrameColor Information
/// @return GOP_API_SUCCESS - Success
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_MultiWin_FrameColor(EN_GOP_MULTIWIN_COLOR_MODE eMultiWinType, GOP_MULTIWIN_FRAMECOLOR* pGOP_WinFrameColor);

//---------------------------------------------------------------------------------------------------
/// MApi_GOP_GWIN_SetPixelShift
/// @ingroup GSET_STRETCH_WINDOW
/// @param s32Hoffset \b IN Stretch window H offset
/// @param s32Voffset \b IN Stretch window V offset
/// @return GOP_API_SUCCESS - Success
//---------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GWIN_SetPixelShift(MS_S32 s32Hoffset, MS_S32 s32Voffset);

//---------------------------------------------------------------------------------------------------
/// MApi_GOP_SetSDR2HDR
/// @ingroup GOP HDR
/// @param eHDRstate \b The HDR information
/// @return GOP_API_SUCCESS - Success
//---------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_SetSDR2HDR(EN_GOP_SDR2HDR eHDRstate);

//---------------------------------------------------------------------------------------------------
/// MApi_GOP_Set_HDRCentral_Ctrl
/// @ingroup GOP HDR
/// @param u8GOPNum \b The GOP number
/// @param bCentralCtrl \b HDR output is controled by kernel gflip or not
/// @param eHDRstate \b If HDR output is not controled by kernel gflip, then is decieded by this param
/// @return GOP_API_SUCCESS - Success
//---------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_Set_HDRCentral_Ctrl(MS_U8 u8GOPNum, MS_BOOL bCentralCtrl, EN_GOP_SDR2HDR eHDRstate);

//---------------------------------------------------------------------------------------------------
/// MApi_GOP_SetGOPCscTuning
/// Using the input parameter which is calculated by CFD drvier, and set to GOP HW
/// @ingroup GOP CSC
/// @param u32GOPNum \b IN The GOP number
/// @param pstGOPCscParam \b IN CFD input param
/// @return GOP_API_SUCCESS - Success
//---------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_SetGOPCscTuning(MS_U32 u32GOPNum,ST_GOP_CSC_PARAM *pstGOPCscParam);
//---------------------------------------------------------------------------------------------------
/// MApi_GOP_GetGOPCscCtrlInfo
/// Get tuning info and CFD output table
/// @ingroup GOP CSC
/// @param u32GOPNum \b IN The GOP number
/// @param pstTuningInfo \b IN GOP CSC info structure
/// @return GOP_API_SUCCESS - Success
//---------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_GetGOPCscCtrlInfo(MS_U32 u32GOPNum,ST_GOP_CSC_TUNING_INFO *pstCtrlInfo);
//---------------------------------------------------------------------------------------------------
/// MApi_GOP_SetGOPCscCtrl
/// Use CSC table and brightness info to set GOP CSC and Brightness HW reg.
/// @ingroup GOP CSC
/// @param u32GOPNum \b IN The GOP number
/// @param pstGOPCscParam \b IN CFD output table
/// @return GOP_API_SUCCESS - Success
//---------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_GOP_API_Result MApi_GOP_SetGOPCscCtrl (MS_U32 u32GOPNum,ST_GOP_CSC_TABLE *pstCSCTbl);

#ifdef __cplusplus
}
#endif

#endif // _API_GOP_H_

