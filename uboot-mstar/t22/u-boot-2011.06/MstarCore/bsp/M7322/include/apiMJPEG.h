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

#include "MsCommon.h"


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

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_PHYADDR u32FwBinAddr;        // Address of firmware binary
    MS_U32 u32FwBinSize;            // Size of firmware binary
    MS_PHYADDR u32FwAddr;           // Address to load firmware
    MS_U32 u32FwSize;               // Firmware size
    MS_PHYADDR u32MRCBufAddr;       // MRC buffer address
    MS_U32 u32MRCBufSize;           // MRC buffer size
    MS_PHYADDR u32MWCBufAddr;       // MWC buffer address
    MS_U32 u32MWCBufSize;           // MWC buffer size
    MS_PHYADDR u32InternalBufAddr;  // internal buffer address
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

/// frame information
typedef struct
{
    /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_PHYADDR u32LumaAddr;
    /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_PHYADDR u32ChromaAddr;
    /// Time stamp(DTS, PTS) of current displayed frame. Unit: ms (todo: 90khz)
    MS_U32 u32TimeStamp;
    /// low part of ID number
    MS_U32 u32ID_L;
    /// high part of ID number
    MS_U32 u32ID_H;
    /// pitch
    MS_U16 u16Pitch;
    /// width
    MS_U16 u16Width;
    /// hight
    MS_U16 u16Height;
    ///< Frame type: I, P, B frame
    MJPEG_FrameType eFrameType;
} MJPEG_FrameInfo;

typedef struct
{
    MS_U32 u32StAddr;       ///< Packet offset from bitstream buffer base address. unit: byte.
    MS_U32 u32Size;         ///< Packet size. unit: byte.
    MS_U32 u32TimeStamp;    ///< Packet time stamp. unit: ms.
    MS_U32 u32ID_L;         ///< Packet ID low part.
    MS_U32 u32ID_H;         ///< Packet ID high part.
} MJPEG_Packet_Info;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
// Init
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Init(MJPEG_INIT_PARAM *pInitParam);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Rst(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Stop(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_CheckDispInfoRdy(void);
// Decode control
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Play(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Pause(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_Resume(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_StepPlay(void);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsStepPlayDone(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_StepDecode(void);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsStepDecodeDone(void);
// Data control
//MJPEG_Result MApi_MJPEG_PushQueue(MS_PHYADDR u32StAddr, MS_U32 u32Size, MS_U32 u32TimeStamp);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_PushQueue(MJPEG_Packet_Info *pInfo);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetQueueVacancy(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_FlushQueue(void);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetESWritePtr(void);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetESReadPtr(void);
// Display control
DLL_PUBLIC MJPEG_Result MApi_MJPEG_EnableLastFrameShow(MS_BOOL bEnable);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsDispFinish(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetSpeed(MJPEG_SpeedType eSpeedType, MJPEG_DispSpeed eDispSpeed);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetFreezeDisp(MS_BOOL bEnable);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SetBlueScreen(MS_BOOL bOn);
// AV Sync
DLL_PUBLIC MJPEG_Result MApi_MJPEG_ResetPTS(MS_U32 u32PtsBase);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetPTS(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_AVSyncOn(MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsAVSyncOn(void);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsReachSync(void);
// Get Info
DLL_PUBLIC MS_BOOL MApi_MJPEG_GetStatus(MJPEG_Status *pStatus);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetErrCode(void);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetErrCnt(void);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetFrameCnt(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetDispInfo(MJPEG_DISP_INFO *pInfo);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsPlaying(void);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsIdle(void);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsIFrameFound(void);
DLL_PUBLIC MS_BOOL MApi_MJPEG_IsWithLowDelay(void);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetDecFrameInfo(MJPEG_FrameInfo *pFrmInfo);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetDispFrameInfo(MJPEG_FrameInfo *pFrmInfo);
// Debug
DLL_PUBLIC void MApi_MJPEG_DbgSetMsgLevel(MJPEG_DbgLevel eDbgLevel);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_DbgSetData(MS_PHYADDR u32Addr, MS_U32 u32Data);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_DbgGetData(MS_PHYADDR u32Addr, MS_U32* u32Data);

// Display Control
DLL_PUBLIC MJPEG_Result MApi_MJPEG_EnableDispCmdQ(MS_BOOL bEnable);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_PushDispCmdQ(void);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetDispCmdQVacancy(void);

DLL_PUBLIC MJPEG_Result MApi_MJPEG_DispFrame(MS_U32 u32DispIdx);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_FreeFrame(MS_U32 u32DispIdx);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_GetNextDispFrame(MJPEG_FrameInfo *pFrmInfo);
DLL_PUBLIC MS_U32 MApi_MJPEG_GetNextDispQPtr(void);

//
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SeekToPTS(MS_U32 u32PTS);
DLL_PUBLIC MJPEG_Result MApi_MJPEG_SkipToPTS(MS_U32 u32PTS);

DLL_PUBLIC void MApi_MJPEG_EnableTurboFWMode(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif // _API_MJPEG_H_

