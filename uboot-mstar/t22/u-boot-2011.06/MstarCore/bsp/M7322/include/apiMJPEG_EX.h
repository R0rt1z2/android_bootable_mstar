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
/// file    apiMJPEG.h
/// @brief  MJPEG API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_MJPEG_H_
#define _API_MJPEG_H_

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)
#include "MsCommon.h"
#ifndef UTOPIA_STRIP
#include "drvHVD_EX.h"
#else
#include "../../drv/hvd_v3/drvHVD_EX.h"
#endif
#include "controller.h"
#include "fwHVD_if.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_MJPEG_RET_FAIL                    = 0,
    E_MJPEG_RET_SUCCESS,
    E_MJPEG_RET_INVALID_PARAMETER,
    E_MJPEG_RET_DECODE_HDR_ERROR,
    E_MJPEG_RET_DECODE_ERROR,
    E_MJPEG_RET_DECODING,
} MJPEG_Result;

typedef enum
{
    E_MJPEG_SPEED_DEFAULT = 0,
    E_MJPEG_SPEED_FAST,
    E_MJPEG_SPEED_SLOW
} MJPEG_SpeedType;

typedef enum
{
    /// Normal display speed.
    E_MJPEG_DISP_SPEED_1X = 1,
    /// 2X
    E_MJPEG_DISP_SPEED_2X = 2,
    /// 4X
    E_MJPEG_DISP_SPEED_4X = 4,
    /// 8X
    E_MJPEG_DISP_SPEED_8X = 8,
    /// 16X
    E_MJPEG_DISP_SPEED_16X = 16,
    /// 32X
    E_MJPEG_DISP_SPEED_32X = 32,
} MJPEG_DispSpeed;

typedef enum
{
    /// disable all uart message.
    E_MJPEG_DBG_LEVEL_NONE = 0,
    /// Only output error message
    E_MJPEG_DBG_LEVEL_ERR,
    /// output general message, and above.
    E_MJPEG_DBG_LEVEL_INFO,
    /// output debug message, and above.
    E_MJPEG_DBG_LEVEL_DBG,
    /// output function tracing message, and above.
    E_MJPEG_DBG_LEVEL_TRACE,
    /// output FW message.
    E_MJPEG_DBG_LEVEL_FW,
} MJPEG_DbgLevel;

typedef enum
{
    E_MJPEG_FW_SOURCE_NONE,       ///< No input fw.
    E_MJPEG_FW_SOURCE_DRAM,       ///< input source from DRAM.
    E_MJPEG_FW_SOURCE_FLASH,      ///< input source from FLASH.
} MJPEG_FWSourceType;

typedef enum
{
    E_MJPEG_ERROR_CODE_NONE = 0,
    E_MJPEG_ERR_CODE_RES,
} MJPEG_ErrorCode;

//for Dual Decode
typedef enum
{
    E_MJPEG_API_STREAM_NONE = 0,
    E_MJPEG_API_MAIN_STREAM,
    E_MJPEG_API_SUB_STREAM,
#ifdef VDEC3
    E_MJPEG_API_N_STREAM,
#endif
} MJPEG_API_StreamType;


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_PHY u32FwBinAddr;        // Address of firmware binary
    MS_U32 u32FwBinSize;            // Size of firmware binary
    MS_PHY u32FwAddr;           // Address to load firmware
    MS_U32 u32FwSize;               // Firmware size
    MS_PHY u32MRCBufAddr;       // MRC buffer address
    MS_U32 u32MRCBufSize;           // MRC buffer size
    MS_PHY u32MWCBufAddr;       // MWC buffer address
    MS_U32 u32MWCBufSize;           // MWC buffer size
    MS_PHY u32InternalBufAddr;  // internal buffer address
    MS_U32 u32InternalBufSize;      // internal buffer size
    //MS_U32 u32FrameTime;            // frame time
    MS_U32 u32FrameRate;
    MS_U32 u32FrameRateBase;
    MJPEG_FWSourceType eFwSrcType;  // the input FW source type.
    MS_U32 u32Width;
    MS_U32 u32Height;
} MJPEG_INIT_PARAM;

typedef struct
{
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U32 u32FrameRate;
    MS_U16 u16CropRight;
    MS_U16 u16CropLeft;
    MS_U16 u16CropBottom;
    MS_U16 u16CropTop;
    MS_U16 u16Pitch;
} MJPEG_DISP_INFO;

typedef enum
{
    E_MJPEG_STAGE_STOP = 0,
    E_MJPEG_STAGE_INIT,
    E_MJPEG_STAGE_PLAY,
    E_MJPEG_STAGE_PAUSE,
} MJPEG_Stage;

typedef struct
{
    MS_BOOL bInit;
    MS_BOOL bIdle;
    MJPEG_Stage  eStage;
} MJPEG_Status;

typedef enum
{
    E_MJPEG_FRM_TYPE_I      = 0,
    //E_MJPEG_FRM_TYPE_P,
    //E_MJPEG_FRM_TYPE_B,
    E_MJPEG_FRM_TYPE_OTHER  = 3,
} MJPEG_FrameType;

#if 0
/// frame information
typedef struct
{
    MS_U32 u32LumaAddr;                 ///< The start offset of luma data. Unit: byte.
    MS_U32 u32ChromaAddr;               ///< The start offset of chroma data. Unit: byte.
    MS_U32 u32PpInLumaAddr;             ///< Luma address (For post-process use)
    MS_U32 u32PpInChromaAddr;           ///< Chroma address (For post-process use)
    MS_U32 u32TimeStamp;                ///< Time stamp(DTS, PTS) of current displayed frame. Unit: 90khz.
    MS_U32 u32ID_L;                     ///< low part of ID number decided by MDrv_HVD_PushQueue().
    MS_U32 u32ID_H;                     ///< high part of ID number decided by MDrv_HVD_PushQueue().
    MS_U8  u8FrmType;                   ///< HVD_Picture_Type, picture type: I, P, B frame
    MS_U8  u8FieldType;                 ///< HVD_Field_Type, none, top , bottom, both field
    MS_U16 u16Pitch;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U32 u32Status;                   ///< 0:None, 1:Init, 2:View, 3:Disp, 4:Free
    MS_U32 u32PrivateData;              ///[STB]only for AVC
    MS_U32 u32LumaAddr_2bit;            ///< The start offset of 2bit luma data. Unit: byte.
    MS_U32 u32ChromaAddr_2bit;          ///< The start offset of 2bit chroma data. Unit: byte.
    MS_U16 u16Pitch_2bit;
    MS_U8  u8LumaBitdepth;
    MS_U8  u8ChromaBitdepth;
    MS_U8  u2Luma0Miu:2;
    MS_U8  u2Luma1Miu:2;
    MS_U8  u2Chroma0Miu:2;
    MS_U8  u2Chroma1Miu:2;
    MS_U8  u8FbIndex;
    MS_U8  u8ModeFlag;                  /// use HVD_Frm_Info_Mode_Flag
    MS_U8  reserved8;

    //
    #ifdef VDEC_FW_CAP_UMA
    MS_U8 u8LumaAddr_H;                 ///< The start offset of luma data. Unit: byte.
    MS_U8 u8ChromaAddr_H;               ///< The start offset of chroma data. Unit: byte.
    MS_U8 u8PpInLumaAddr_H;             ///< Luma address (For post-process use)
    MS_U8 u8PpInChromaAddr_H;           ///< Chroma address (For post-process use)
    MS_U8 u8LumaAddr_2bit_H;            ///< The start offset of 2bit luma data. Unit: byte.
    MS_U8 u8ChromaAddr_2bit_H;          ///< The start offset of 2bit chroma data. Unit: byte.
    #endif
    //
} MJPEG_FrameInfo;
#endif

typedef struct
{
    MS_PHY u32StAddr;       ///< Packet offset from bitstream buffer base address. unit: byte.
    MS_U32 u32Size;         ///< Packet size. unit: byte.
    MS_U32 u32TimeStamp;    ///< Packet time stamp. unit: ms.
    MS_U32 u32ID_L;         ///< Packet ID low part.
    MS_U32 u32ID_H;         ///< Packet ID high part.
} MJPEG_Packet_Info;

typedef enum
{
    E_MJPEG_VDEC_CMD_SET_DISPLAY_OUTSIDE_MODE  = 0x0000,
}MJPEG_VDEC_Cmd;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
// Init
#ifdef VDEC3
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Init(MS_U32 u32Id, MJPEG_INIT_PARAM *pInitParam, MS_BOOL bFWdecideFB);
#else
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Init(MS_U32 u32Id, MJPEG_INIT_PARAM *pInitParam);
#endif
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Rst(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Stop(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_CheckDispInfoRdy(MS_U32 u32Id);
// Decode control
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Play(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Pause(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Resume(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_StepPlay(MS_U32 u32Id);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsStepPlayDone(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_StepDecode(MS_U32 u32Id);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsStepDecodeDone(MS_U32 u32Id);
// Data control
//MJPEG_Result MApi_MJPEG_PushQueue(MS_PHY u32StAddr, MS_U32 u32Size, MS_U32 u32TimeStamp);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_PushQueue(MS_U32 u32Id, MJPEG_Packet_Info *pInfo);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetQueueVacancy(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_FlushQueue(MS_U32 u32Id);
DLL_PUBLIC MS_PHY MApi_MJPEG_GetESWritePtr(MS_U32 u32Id);
DLL_PUBLIC MS_PHY MApi_MJPEG_GetESReadPtr(MS_U32 u32Id);
// Display control
DLL_PUBLIC MJPEG_Result MApi_MJPEG_EnableLastFrameShow(MS_U32 u32Id, MS_BOOL bEnable);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsDispFinish(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetSpeed(MS_U32 u32Id, MJPEG_SpeedType eSpeedType, MJPEG_DispSpeed eDispSpeed);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetFreezeDisp(MS_U32 u32Id, MS_BOOL bEnable);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetBlueScreen(MS_U32 u32Id, MS_BOOL bOn);
// AV Sync
DLL_PUBLIC MJPEG_Result MApi_MJPEG_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetPTS(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_AVSyncOn(MS_U32 u32Id, MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsAVSyncOn(MS_U32 u32Id);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsReachSync(MS_U32 u32Id);
// Get Info
DLL_PUBLIC MS_BOOL MApi_MJPEG_GetStatus(MS_U32 u32Id, MJPEG_Status *pStatus);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetErrCode(MS_U32 u32Id);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetErrCnt(MS_U32 u32Id);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetSkipCnt(MS_U32 u32Id);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetFrameCnt(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetDispInfo(MS_U32 u32Id, MJPEG_DISP_INFO *pInfo);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsPlaying(MS_U32 u32Id);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsIdle(MS_U32 u32Id);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsIFrameFound(MS_U32 u32Id);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsWithLowDelay(MS_U32 u32Id);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetDecFrameInfo(MS_U32 u32Id, HVD_Frm_Information *pFrmInfo);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetDispFrameInfo(MS_U32 u32Id, HVD_Frm_Information *pFrmInfo);
// Debug
DLL_PUBLIC void MApi_MJPEG_DbgSetMsgLevel(MJPEG_DbgLevel eDbgLevel);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_DbgSetData(MS_U32 u32Id, MS_PHY u32Addr, MS_U32 u32Data);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_DbgGetData(MS_U32 u32Id, MS_PHY u32Addr, MS_U32* u32Data);

// Display Control
DLL_PUBLIC MJPEG_Result MApi_MJPEG_EnableDispCmdQ(MS_U32 u32Id, MS_BOOL bEnable);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_PushDispCmdQ(MS_U32 u32Id);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetDispCmdQVacancy(MS_U32 u32Id);

DLL_PUBLIC MJPEG_Result MApi_MJPEG_DispFrame(MS_U32 u32Id, MS_U32 u32DispIdx);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_FreeFrame(MS_U32 u32Id, MS_U32 u32DispIdx);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetNextDispFrame(MS_U32 u32Id, HVD_Frm_Information *pFrmInfo);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetDispFrmNum(MS_U32 u32Id);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetNextDispQPtr(MS_U32 u32Id);

//
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SeekToPTS(MS_U32 u32Id, MS_U32 u32PTS);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SkipToPTS(MS_U32 u32Id, MS_U32 u32PTS);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetControl(MS_U32 u32Id, MJPEG_VDEC_Cmd cmd_id, MS_U32 param);

DLL_PUBLIC void MApi_MJPEG_EnableTurboFWMode(MS_BOOL bEnable);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetDynmcDispPath(MS_U32 u32Id, MS_BOOL bConnect, HVD_EX_DISPLAY_PATH eDispPath);

//for Dual Decode
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetFreeStream(MS_U32 *pu32Id, MJPEG_API_StreamType eStreamType);

#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
DLL_PUBLIC void MApi_MJPEG_SetCMAInformation(void* pcmaInitParam);
#endif

#ifdef __cplusplus
}
#endif
#endif

#endif // _API_MJPEG_H_

