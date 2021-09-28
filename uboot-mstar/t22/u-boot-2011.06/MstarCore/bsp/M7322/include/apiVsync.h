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
/// @brief  VDEC EXTENSION API FOR DUAL STREAMS
///////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef _API_VSYNC_H_
#define _API_VSYNC_H_


#include "UFO.h"
#include "apiVDEC_EX.h"
#include "apiDMS_Type.h"
#include "apiDMX.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

#ifndef ANDROID
#define VSYNC_PRINT printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>
#define VSYNC_PRINT ALOGD
#endif

#define VSYNC_ATTR_PACKED __attribute__((packed))

#ifdef MSOS_TYPE_LINUX_KERNEL
#if defined(CHIP_M5621) || defined(CHIP_M7621) || defined(CHIP_M7622)
#define VSYNC_USE_DMS
#endif
#endif

#if defined(CHIP_K7U) || defined(CHIP_C2P) || defined(CONFIG_API_DMS) || defined(CHIP_M7221)
#define VSYNC_USE_DMS
#endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
#define __vdec_weak
#else
#define __vdec_weak      __attribute__((weak))
#endif

#define __vdec_if            extern

__vdec_if EN_DMS_RESULT MApi_DMS_Init(ST_DMS_INITDATA *pstDMS_InitData);
__vdec_if EN_DMS_RESULT MApi_DMS_Video_Flip(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);
__vdec_if EN_DMS_RESULT MApi_DMS_DestroyWindow(MS_U32 u32WindowID);
__vdec_if EN_DMS_RESULT MApi_DMS_ClearDigitalDecodeSignalInfo(MS_U32 u32WindowID);
__vdec_if EN_DMS_RESULT MApi_DMS_SetDigitalDecodeSignalInfo(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat);
__vdec_if EN_DMS_RESULT MApi_DMS_GetCapability(EN_DMS_CAPABILITY *peCapability);
__vdec_if DMX_FILTER_STATUS MApi_DMX_Stc_Get(MS_U32* pu32Stc32, MS_U32* pu32Stc);


#define VSYNC_MAX_DISP_PATH 33
#define AVSYNC_REPEAT_THRESHOLD         1
#define AVSYNC_REPEAT_ALWAYS            0xff
#define VSYNC_INVALID_WIN_ID 0xFFFFFFFF

/// function return enumerator
typedef enum
{
    ///failed
    E_VSYNC_FAIL = 0,
    ///success
    E_VSYNC_OK,
    ///invalid parameter
    E_VSYNC_RET_INVALID_PARAM,
    ///access not allow
    E_VSYNC_RET_ILLEGAL_ACCESS,
     ///unsupported
    E_VSYNC_RET_UNSUPPORTED,
     ///timeout
    E_VSYNC_RET_TIMEOUT,
    ///not ready
    E_VSYNC_RET_NOT_READY,
    ///not initial
    E_VSYNC_RET_NOT_INIT,
    ///not exit after last initialization
    E_VSYNC_RET_NOT_EXIT,
    ///not running, counter does not change
    E_VSYNC_RET_NOT_RUNNING,
    ///max value
    E_VSYNC_RET_NUM,
} VSYNC_Result;

typedef enum
{
    ///stop
    E_VSYNC_STOP = 0,
    ///play
    E_VSYNC_PLAY,
    ///flush
    E_VSYNC_FLUSH,
    ///pause
    E_VSYNC_PAUSE,
    ///smooth trick (x2, slow motion)
    E_VSYNC_FREEZE,

    E_VSYNC_STEP_DISP,

    E_VSYNC_STEP_DECODE,
    //freeze to pts - for tcl
    E_VSYNC_SEEK,
    ///max value
    E_VSYNC_CMD_NUM,
} VSYNC_Vdec_Status;

typedef enum
{
    E_VSYNC_STC_MODE = 0,
    E_VSYNC_AUDIO_MASTER_MODE,
    E_VSYNC_VIDEO_MASTER_MODE,
    E_VSYNC_MAX_MODE,
} VSYNC_SyncMode;

typedef enum
{
    E_VSYNC_USER_CMD_SET_CONTROL_BASE = 0x0,
    E_VSYNC_USER_CMD_SET_STATUS,
    E_VSYNC_USER_CMD_SET_3D_MODE,
    E_VSYNC_USER_CMD_SET_3DLAYOUTUNLOCKED,
    E_VSYNC_USER_CMD_SET_LOW_LATENCY,
    E_VSYNC_USER_CMD_SET_SLOW_SYNC,
    E_VSYNC_USER_CMD_SET_AVSYNC_FREERUN_THRESHOLD,
    E_VSYNC_USER_CMD_SET_DISP_ONE_FIELD,
    E_VSYNC_USER_CMD_SET_FREEZE_DISP,
    E_VSYNC_USER_CMD_SET_MVOP_MCU_MODE,// may use by vsync bridge
    E_VSYNC_USER_CMD_SET_SHOW_FIRST_FRAME_DIRECT,
    E_VSYNC_USER_CMD_SET_TRICKPLAY_2X_MODE,
    E_VSYNC_USER_CMD_SET_DISP_WIN_ID,
    E_VSYNC_USER_CMD_PRESET_STC,
    E_VSYNC_USER_CMD_FD_MASK_DELAY_COUNT,
    E_VSYNC_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD,
    E_VSYNC_USER_CMD_AVC_DISP_IGNORE_CROP,
    E_VSYNC_USER_CMD_AVSYNC_REPEAT_TH,
    E_VSYNC_USER_CMD_SET_BLUE_SCREEN,
    E_VSYNC_USER_CMD_DISP_ONE_FIELD,
    E_VSYNC_USER_CMD_STEP_DISP,
    E_VSYNC_USER_CMD_ENABLE_LAST_FRAME_SHOW,// how do I know we meet file end ?
    E_VSYNC_USER_CMD_SET_MULTI_SPEED_ENCODE,
    E_VSYNC_USER_CMD_DBG_IDX,
    E_VSYNC_USER_CMD_SET_SELF_SEQCHANGE,
    E_VSYNC_USER_CMD_SUSPEND_DYNAMIC_SCALE,
    E_VSYNC_USER_CMD_SET_WIN_INIT,
    E_VSYNC_USER_CMD_STEP_DECODE,
    E_VSYNC_USER_CMD_SET_TRICK_SMOOTH_SCAN,
    E_VSYNC_USER_CMD_SET_DECODE_I,
    E_VSYNC_USER_CMD_PUSH_DECQ,
    E_VSYNC_USER_CMD_SET_SW_STC_CONFIG,
    E_VSYNC_USER_CMD_FORCE_INTERLACE_MODE,
    E_VSYNC_USER_CMD_FORCE_PROGRESSIVE_MODE,
    E_VSYNC_USER_CMD_SET_FREEZE_TO_PTS,
    //=====================================
    E_VSYNC_USER_CMD_GET_CONTROL_BASE = 0x1000,
    E_VSYNC_USER_CMD_GET_IsAVSyncOn,
    E_VSYNC_USER_CMD_GET_IS_REACH_SYNC,
    E_VSYNC_USER_CMD_GET_DROP_CNT,
    E_VSYNC_USER_CMD_GET_DISP_CNT,
    E_VSYNC_USER_CMD_GET_PTS,
    E_VSYNC_USER_CMD_GET_NEXT_PTS,
    E_VSYNC_USER_CMD_GET_U64PTS,
    E_VSYNC_USER_CMD_GET_IS_FRAME_RDY,
    E_VSYNC_USER_CMD_GET_IS_STEP_DISP_DONE,
    E_VSYNC_USER_CMD_IS_DISP_FINISH, // Huston, we got a problem !!!
    E_VSYNC_USER_CMD_GET_VIDEO_PTS_STC_DELTA,
    E_VSYNC_USER_CMD_IS_FRAME_RDY,
    E_VSYNC_USER_CMD_IS_DISPQ_EMPTY,
    E_VSYNC_USER_CMD_GET_DISP_INFO,
    E_VSYNC_USER_CMD_CHECK_DISPINFO_READY,
    E_VSYNC_USER_CMD_GET_DIGITAL_SIG_INFO_DONE,
    E_VSYNC_USER_CMD_GET_IS_STEP_DECODE_DONE,
    E_VSYNC_USER_CMD_GET_CMD_DONE,
    E_VSYNC_USER_CMD_GET_PRECISION_SEEK_PTS,
    E_VSYNC_USER_CMD_IS_SEQ_CHG,
    E_VSYNC_USER_CMD_GET_SEQ_CHANGE_INFO,
    E_VSYNC_USER_CMD_GET_DBG_INFO,
    //=====================================
    E_VSYNC_USER_CMD_MAX,
}VSYNC_User_Cmd;

typedef enum
{
    E_VSYNC_SCAN_NORAML = 0,
    E_VSYNC_SCAN_FORCE_P,
    E_VSYNC_SCAN_FORCE_I,
} VSYNC_CustomerScanType;

typedef struct
{
    union
    {
        struct
        {
            unsigned int vsync_log_dbg : 1;
            unsigned int vsync_log_status : 1;
            unsigned int vsync_log_time : 1;
            unsigned int vsync_log_cmd : 1;
            unsigned int vsync_log_drop : 1;
            unsigned int vsync_log_verbose : 1;
            unsigned int vsync_log_seq_chg : 1;
            unsigned int vsync_log_idle : 1;
            unsigned int vsync_log_performance : 1;
            unsigned int vsync_log_frame_info : 1;
            unsigned int vsync_log_flip : 1;
            unsigned int vsync_log_dms_release : 1;
            unsigned int vsync_log_loop : 1;
            unsigned int mvdcmd_handshake_reserved : 20;    // reserved for extend
        };
        unsigned int value;
    };
}VSYNC_LOG_IDX;

typedef EN_DMS_RESULT (*VsyncDispConnectCb)(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);
typedef EN_DMS_RESULT (*VsyncDispDisconnectCb)(MS_U32 u32WindowID);
typedef EN_DMS_RESULT (*VsyncDispFlipCb)(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);
typedef EN_DMS_RESULT (*VsyncDispGetStatusCb)(EN_DMS_STATUS *peStatus);
typedef void (*VDEC_EX_FireUserCallbackFunc)(VDEC_StreamId *pStreamId, MS_U32 u32EventFlag);


//void MApi_VSync_Dbg_test(void);
//void MApi_VSync_Dbg_selftest(VDEC_StreamId *pStreamId, VDEC_EX_InitParam *pInitParam);
//void MApi_VSync_Dbg_stop_selftest(VDEC_StreamId *pStreamId);

VSYNC_Result MApi_VSync_Init(int VsynDispId, VDEC_StreamId *pStreamId, VDEC_EX_InitParam *pInitParam, MS_U32 eWinID, MS_U32 u32OverlayId);
VSYNC_Result MApi_VSync_Deinit(int VsynDispId);
VDEC_EX_Result MApi_VSync_SetControl(int VsynDispId, VSYNC_User_Cmd cmd_id, void* param);
VDEC_EX_Result MApi_VSync_GetControl(int VsynDispId, VSYNC_User_Cmd cmd_id, void* param);
VSYNC_Result MApi_VSync_AVSyncOn(int VsynDispId, MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);
VSYNC_Result MApi_VSync_SetSpeed(int VsynDispId, VDEC_EX_SpeedType eSpeedType, VDEC_EX_DispSpeed eSpeed);
VSYNC_Result MApi_VSync_IsInited(int VsynDispId);
VSYNC_Result MApi_VSync_SetCallbackFunc(int VsynDispId, VsyncDispConnectCb VsyncDispConnect, VsyncDispDisconnectCb VsyncDispDisconnect, VsyncDispFlipCb VsyncDispFlip);
VSYNC_Result MApi_VSync_SetVdecCBFunc(int VsynDispId, VDEC_EX_FireUserCallbackFunc VDEC_EX_FireUserCBFunc);

#endif

#ifdef __cplusplus
}
#endif


#endif // _API_VSYNC_H
