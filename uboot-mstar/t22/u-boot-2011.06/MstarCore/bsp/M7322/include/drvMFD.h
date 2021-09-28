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
/// @brief  MStar gflip Interface header file
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MFD_H
#define _DRV_MFD_H

#ifdef _MDRV_MFD_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if defined(__cplusplus)
extern "C" {
#endif
#include "UFO.h"
#include "MsDevice.h"
#include "MsOS.h"
#ifdef MFD_UTOPIA2K
    #define MFDDRIVER_BASE              KERNEL_MODE
#else
    #define MFDDRIVER_BASE              0UL
#endif

//=============================================================================
// Defines & Macros
//=============================================================================

// Resoruce type definition
#define MFD_RES_TYPE                    MS_U32

#define MFD_RES_TYPE_MASK               0xFF000000
#define MFD_RES_TYPE_MVOP               0x01000000
#define MFD_RES_TYPE_DIP                0x02000000
#define MFD_RES_TYPE_GPU                0x03000000
#define MFD_RES_TYPE_CPU                0x04000000

#define MFD_RES_TYPE_OPT_MASK           0x00FF0000
#define MFD_RES_TYPE_OPT_NORMAL         0x00010000
#define MFD_RES_TYPE_OPT_MFDEC          0x00020000
#define MFD_RES_TYPE_OPT_DETILE         0x00030000

#define MFD_RES_TYPE_IDX_MASK           0x000000FF
#define MFD_RES_TYPE_IDX_NONE           0x000000FF

#define MFD_RES_HANDLE                  MFD_RES_TYPE


#define ST_DRV_MFD_MVOP_DIP_INFO_VERSION                 0

#define MFD_GPU_FRAME_IDX_NONE          0xFFFF
#define MFD_GPU_BRIDGE_IDX_NONE         0xFFFF

//=============================================================================
// Type and Structure Declaration
//============================================================================

typedef enum
{
    MFD_DIP_SET_MFDEC                       = 0x200,
    MFD_DIP_SET_MFDEC_INFO                  = 0x201,

    MFD_GPU_CONFIG_FRAME_SETTING            = 0x300,
    MFD_GPU_DISABLE_FRAME_SETTING           = 0x301,
    MFD_GPU_CONFIG_BRIDGE_SETTING           = 0x302,
    MFD_GPU_DISABLE_BRIDGE_SETTING          = 0x303,
    MFD_GPU_GET_ROI_VALID_WINDOW_NUM        = 0x304,
    MFD_GPU_GET_ROI_RAWDATA_WORD_NUM        = 0x305,
    MFD_GPU_GET_ROI_DETECT_RESULT           = 0x306,

    MFD_CPU_CONFIG_FRAME_REG                = 0x400,
    MFD_CPU_DISABLE_FRAME_REG               = 0x401,

    MFD_MVOP_SET_REG                        = 0x500,
    MFD_MVOP_SET_MFDEC_INFO                 = 0x501,

} MFD_CMD_TYPE;
//CMD: 0x00~0xFF general
//CMD: 0x100~0x1FF MVOP
//CMD: 0x200~0x2FF DIP
//CMD: 0x300~0x3FF GPU
//CMD: 0x400~0x4FF CPU

typedef enum
{
    E_MFD_TILE_16_32,
    E_MFD_TILE_32_16,
    E_MFD_TILE_32_32,
    E_MFD_TILE_NONE = 0Xff,
}EN_DRV_MFD_TILE_MODE;

typedef enum
{
    E_MFD_MVOP_DIP_H26X_MODE,
    E_MFD_MVOP_DIP_VP9_MODE,
}EN_DRV_MFD_VP9_MODE;

//=============================================================================
// MVOP/DIP Case Data Structure
//=============================================================================

typedef enum
{
    E_MFD_DEV_0 = 0,
    E_MFD_DEV_1 = 1,
    E_MFD_DEV_MAX
}MFD_DEVICE;

typedef struct DLL_PACKED _ST_DRV_MFD_MVOP_DIP_INFO
{
    MS_U32 u32DrvMFDInfo_Version;   ///<Version of current structure. Please always set to "ST_DRV_MFD_MVOP_DIP_INFO_VERSION" as input
    MS_U16 u16DrvMFDInfo_Length;    ///<Length of this structure, u16PanelInfoEX_Length=sizeof(ST_DRV_MFD_MVOP_DIP_INFO)

    MS_BOOL bMFDec_Enable;
    MFD_DEVICE u8MFDec_Select;
    MS_PHY phyLuma_FB_Base;
    MS_PHY phyChroma_FB_Base;
    MS_U16 u16FB_Pitch;
    MS_U16 u16FB_HSize;
    MS_U16 u16FB_VSize;
    MS_PHY phyBitlen_Base;
    MS_U16 u16Bitlen_Pitch;
    EN_DRV_MFD_TILE_MODE enMFDec_tile_mode;
    MS_BOOL bUncompress_mode;
    MS_BOOL bBypass_codec_mode;
    EN_DRV_MFD_VP9_MODE en_MFDecVP9_mode;
    MS_BOOL bHMirror;
    MS_BOOL bVMirror;
    MS_U16 u16Crop_X;
    MS_U16 u16Crop_Y;
    MS_U16 u16Crop_H;
    MS_U16 u16Crop_V;
    void* pMFDecInfo;
} ST_DRV_MFD_MVOP_DIP_INFO, *PST_DRV_MFD_MVOP_DIP_INFO;

typedef enum
{
    MFD_SUCCESS = 0,
    MFD_FAIL
}MFD_Result;

//=============================================================================
// CPU Case Data Structure
//=============================================================================

typedef enum
{
    E_MFD_CODEC_HEVC = 0,
    E_MFD_CODEC_VP9 = 1,
    E_MFD_CODEC_TILE_16_32,
    E_MFD_CODEC_TILE_32_16,
    E_MFD_CODEC_TILE_32_32,
} EN_DRV_MFD_CODEC_MODE;

typedef struct DLL_PACKED _DRV_MFD_CPU_BRIDGE_INFO
{
    MS_U16 u16WinIdx;
    MS_BOOL bWinValid;
    MS_U16 u16Pitch;
    MS_PHY phyYstart;
    MS_PHY phyYend;
    MS_PHY phyCstart;
    MS_PHY phyCend;
} DRV_MFD_CPU_BRIDGE_INFO, *PDRV_MFD_CPU_BRIDGE_INFO;

typedef struct DLL_PACKED _DRV_MFD_CPU_MFDEC_INFO
{
    MS_BOOL bLogTrace;
    MS_BOOL bMFDECEnable;
    MS_PHY  phyLogBase;
    MS_BOOL bTileWriteMode;
    MS_U16  u16FBIndex;
    MS_PHY  phyYBase;
    MS_PHY  phyCBase;
    MS_U8   u8YMIU;
    MS_U8   u8CMIU;
    MS_U16  u16Pitch;
    MS_U32  u32Height;
    MS_U32  u32Width;
    MS_BOOL bDecompress;
    EN_DRV_MFD_CODEC_MODE   enCodecType;
} DRV_MFD_CPU_MFDEC_INFO, *PDRV_MFD_CPU_MFDEC_INFO;

typedef struct DLL_PACKED _ST_MFD_CPU_VER_CRL
{
    MS_U32 u32version;
    MS_U32 u32size;
} ST_MFD_CPU_VER_CRL, *PST_MFD_CPU_VER_CRL;

typedef struct DLL_PACKED _DRV_MFD_CPU_FULL_INFO
{
    ST_MFD_CPU_VER_CRL stMFD_CPU_VerCtl;
    DRV_MFD_CPU_BRIDGE_INFO stCPUBridgeInfo;
    DRV_MFD_CPU_MFDEC_INFO stCPUMFDECInfo;
    void* pCPUFullInfo;
} DRV_MFD_CPU_FULL_INFO, *PDRV_MFD_CPU_FULL_INFO;

typedef struct DLL_PACKED _DRV_MFD_RST_INFO
{
    MS_U16 u16WinIdx;
} DRV_MFD_RST_INFO, *PDRV_MFD_RST_INFO;

//=============================================================================
// GPU Case Data Structure
//=============================================================================
typedef struct DLL_PACKED _DRV_MFD_GPU_FRAME_MFDEC_INFO
{
    MS_U16 u16BridgeIdx;
    MS_U16 u16FrameIdx;
    MS_U32 u32Width;
    MS_U32 u32Height;
    MS_BOOL bTiledYUV;
    MS_PHY phyBLDBase;
    MS_PHY phyYBase;
    MS_PHY phyCBase;
    MS_U32 u32BLDPitch;
    MS_U32 u32YPitch;
    EN_DRV_MFD_CODEC_MODE enCodecType;
    MS_U16 u16BLDMif;
    MS_BOOL bEnableHTLB;
    MS_U16 u16HTLBPageSizeMin;
    MS_U16 u16HTLBPageSize1st;
    MS_U16 u16HTLBDirectMap;
    MS_PHY phyBridgeBase;
} DRV_MFD_GPU_FRAME_MFDEC_INFO, *PDRV_MFD_GPU_FRAME_MFDEC_INFO;

typedef struct DLL_PACKED _DRV_MFD_GPU_ROI_WIN_UNIT
{
    MS_U32 x;                           ///< x coordinate of upper-left pixel
    MS_U32 y;                           ///< y coordinate of upper-left pixel
    MS_U32 width;                       ///< window width
    MS_U32 height;                      ///< window height
} DRV_MFD_GPU_ROI_WIN_UNIT, *PDRV_MFD_GPU_ROI_WIN_UNIT;

typedef struct DLL_PACKED _DRV_MFD_GPU_ROI_RESULT
{
    DRV_MFD_GPU_ROI_WIN_UNIT* pWin;     ///< store the window information
    MS_U16* pRawData;                   ///< store the raw data
} DRV_MFD_GPU_ROI_RESULT, *PDRV_MFD_GPU_ROI_RESULT;

//=============================================================================
// Function
//=============================================================================

//Init related:
//-------------------------------------------------------------------------------------------------
/// Initialized MFDEC
/// @ingroup MFD_General
/// @param pInstance        \b IN: Pointer to access shared memory
/// @return MFD_SUCCESS     - Success
/// @return MFD_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK MS_BOOL MDrv_MFD_Init(void);
//-------------------------------------------------------------------------------------------------
/// EXIT MFDEC
/// @ingroup MFD_General
/// @param pInstance        \b IN: Pointer to access shared memory
/// @return MFD_SUCCESS     - Success
/// @return MFD_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK MS_BOOL MDrv_MFD_Exit(void);
//-------------------------------------------------------------------------------------------------
/// Allocate MFDEC resource
/// @ingroup MFD_General
/// @param ResType               \b IN:  Resource type to allocate
/// @param pResHandle            \b OUT: Handler to store resource status
/// @return MFD_SUCCESS     - Success
/// @return MFD_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK MS_BOOL MDrv_MFD_ResAllocate(MFD_RES_TYPE ResType, MFD_RES_HANDLE *pResHandle);
//-------------------------------------------------------------------------------------------------
/// Free MFDEC resource
/// @ingroup MFD_General
/// @param pResHandle            \b IN:  Resource status handler to free resource
/// @return MFD_SUCCESS     - Success
/// @return MFD_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK MS_BOOL MDrv_MFD_ResFree(MFD_RES_HANDLE ResHandle);
//-------------------------------------------------------------------------------------------------
/// Set disp_mfdec related registers
/// @ingroup MFD_General
/// @param ResHandle            \b IN: Pointer to access shared memory
/// @param enCmdtype            \b IN:
/// @param psetting             \b IN:
/// @return MFD_SUCCESS     - Success
/// @return MFD_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK MS_BOOL MDrv_MFD_SetReg(MFD_RES_HANDLE ResHandle, MFD_CMD_TYPE enCmdtype, void* psetting);
//-------------------------------------------------------------------------------------------------
/// Get disp_mfdec related information
/// @ingroup MFD_General
/// @param ResHandle            \b IN: Pointer to access shared memory
/// @param enCmdtype            \b IN:
/// @param psetting             \b OUT:
/// @return MFD_SUCCESS     - Success
/// @return MFD_FAIL        - Failure
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK MS_BOOL MDrv_MFD_GetInfo(MFD_RES_HANDLE ResHandle, MFD_CMD_TYPE enCmdtype, void* psetting);

//Drv Interface related(drv interface):

#if defined(__cplusplus)
}
#endif

#undef INTERFACE

#endif //_DRV_MFD_H

