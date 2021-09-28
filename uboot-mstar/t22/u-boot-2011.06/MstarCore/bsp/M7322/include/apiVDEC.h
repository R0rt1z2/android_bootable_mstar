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
/// @brief  VDEC API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_VDEC_H_
#define _API_VDEC_H_

#include "MsDevice.h"
#include "MsTypes.h"
#ifdef STELLAR
#include "UFO.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_VDEC_LIB_CODE      {'V','D','E','C'}
#define MSIF_VDEC_LIBVER        {'0','E'}
#define MSIF_VDEC_BUILDNUM      {'0','2'}
#define MSIF_VDEC_CHANGELIST    {'0','0','6','8','9','1','8','8'}

/// Version string.
#define VDEC_API_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_VDEC_LIB_CODE,                 /* IP__                                             */  \
    MSIF_VDEC_LIBVER,                   /* 0.0 ~ Z.Z                                        */  \
    MSIF_VDEC_BUILDNUM,                 /* 00 ~ 99                                          */  \
    MSIF_VDEC_CHANGELIST,               /* CL#                                              */  \
    MSIF_OS

#define VDEC_DEFAULT_DBG_MSG_LEVEL  E_VDEC_DBG_LEVEL_DBG
#define VDEC_RVD_BROKEN_BY_US   0x80000000
#define VDEC_MVD_PIC_START_FLAG 0x40000000
#define VDEC_BIT(_bit_)                  (1 << (_bit_))
#ifndef VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE_SIZE
#define VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE_SIZE
#endif
#ifndef VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE
#define VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE
#endif
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define VDEC_FPA_TYPE_CHECKERBOARD_INTERLEAVING  0
#define VDEC_FPA_TYPE_COLUMN_INTERLEAVEING       1
#define VDEC_FPA_TYPE_ROW_INTERLEAVEING          2
#define VDEC_FPA_TYPE_SIDE_BY_SIDE_PACKING       3
#define VDEC_FPA_TYPE_TOP_BOTTOM_PACKING         4
#define VDEC_FPA_TYPE_TEMPORAL_INTERLEAVING_FRM  5

/// Decoder event enumerator.
typedef enum
{
    /// turn off all event
    E_VDEC_EVENT_OFF                = 0x00,
    /// display one frame/field
    E_VDEC_EVENT_DISP_ONE           = VDEC_BIT(0),
    /// repeat one frame/field
    E_VDEC_EVENT_DISP_REPEAT        = VDEC_BIT(1),
    /// one CC data should be displayed
    E_VDEC_EVENT_DISP_WITH_CC       = VDEC_BIT(2),
    /// decode one frame
    E_VDEC_EVENT_DEC_ONE            = VDEC_BIT(3),
    /// decode one I frame
    E_VDEC_EVENT_DEC_I              = VDEC_BIT(4),
    /// decode error
    E_VDEC_EVENT_DEC_ERR            = VDEC_BIT(5),
    /// display information is changed
    E_VDEC_EVENT_DISP_INFO_CHG      = VDEC_BIT(6),
    /// find user data
    E_VDEC_EVENT_USER_DATA_FOUND    = VDEC_BIT(7),
    /// display information ready after be changed
    E_VDEC_EVENT_DISP_INFO_RDY      = VDEC_BIT(8),
    /// first frame decoded
    E_VDEC_EVENT_FIRST_FRAME        = VDEC_BIT(9),
    /// first picture found
    E_VDEC_EVENT_PIC_FOUND          = VDEC_BIT(10),
    /// video is ready to display (no garbage and avsync done)
    E_VDEC_EVENT_VIDEO_UNMUTE       = VDEC_BIT(11),
    /// new sequence header found
    E_VDEC_EVENT_SEQ_HDR_FOUND      = VDEC_BIT(12),
    /// active format description found
    E_VDEC_EVENT_AFD_FOUND          = VDEC_BIT(13),
    // ES data invalid
    E_VDEC_EVENT_ES_DATA_ERR        = VDEC_BIT(14),
    /// speed up the un-mute screen on XC.
    E_VDEC_EVENT_XC_LOW_DEALY       = VDEC_BIT(16)
} VDEC_EventFlag;

//define VDEC CB type
typedef enum
{
    E_VDEC_CB_MAIN  = 0,
    E_VDEC_CB_SUB,
} VDEC_CB_TYPE;

/// Codec type enumerator
typedef enum
{
    ///Unsupported codec type
    E_VDEC_CODEC_TYPE_NONE = 0,
    ///MPEG-1 / MPEG-2
    E_VDEC_CODEC_TYPE_MPEG2,
    ///H263 (short video header)
    E_VDEC_CODEC_TYPE_H263,
    ///MPEG4 (default)
    E_VDEC_CODEC_TYPE_MPEG4,
    ///MPEG4 (Divx311)
    E_VDEC_CODEC_TYPE_DIVX311,
    ///MPEG4 (Divx412)
    E_VDEC_CODEC_TYPE_DIVX412,
    ///FLV
    E_VDEC_CODEC_TYPE_FLV,
    ///VC1 advanced profile (VC1)
    E_VDEC_CODEC_TYPE_VC1_ADV,
    ///VC1 main profile (RCV)
    E_VDEC_CODEC_TYPE_VC1_MAIN,
    ///Real Video version 8
    E_VDEC_CODEC_TYPE_RV8,
    ///Real Video version 9 and 10
    E_VDEC_CODEC_TYPE_RV9,
    ///H264
    E_VDEC_CODEC_TYPE_H264,
    ///AVS
    E_VDEC_CODEC_TYPE_AVS,
    ///MJPEG
    E_VDEC_CODEC_TYPE_MJPEG,
    ///MVC
    E_VDEC_CODEC_TYPE_MVC,
    ///VP8
    E_VDEC_CODEC_TYPE_VP8,
    ///HEVC
    E_VDEC_CODEC_TYPE_HEVC,
    ///VP9
    E_VDEC_CODEC_TYPE_VP9,
    ///HEVC_DV
    E_VDEC_CODEC_TYPE_HEVC_DV,
    E_VDEC_CODEC_TYPE_NUM
} VDEC_CodecType;

/// Input source select enumerator
typedef enum
{
    ///DTV mode
    E_VDEC_SRC_MODE_DTV = 0,
    ///TS file mode
    E_VDEC_SRC_MODE_TS_FILE,
    ///generic file mode
    E_VDEC_SRC_MODE_FILE,
    /// TS file and dual ES buffer mode
    E_VDEC_SRC_MODE_TS_FILE_DUAL_ES,
    ///generic file and dual ES buffer mode
    E_VDEC_SRC_MODE_FILE_DUAL_ES,
} VDEC_SrcMode;

/// Function return enumerator
typedef enum
{
    ///failed
    E_VDEC_FAIL = 0,
    ///success
    E_VDEC_OK,
    ///invalid parameter
    E_VDEC_RET_INVALID_PARAM,
    ///access not allow
    E_VDEC_RET_ILLEGAL_ACCESS,
    ///hardware abnormal
    E_VDEC_RET_HARDWARE_BREAKDOWN,
     ///unsupported
    E_VDEC_RET_UNSUPPORTED,
     ///timeout
    E_VDEC_RET_TIMEOUT,
    ///not ready
    E_VDEC_RET_NOT_READY,
    ///not initial
    E_VDEC_RET_NOT_INIT,
    ///not exit after last initialization
    E_VDEC_RET_NOT_EXIT,
    ///not running, counter does not change
    E_VDEC_RET_NOT_RUNNING,
} VDEC_Result;

/// Action enumerator of display commands
typedef enum
{
    /// Action- display frame
    E_VDEC_DISP_ACTION_DISPLAY   = 1,
    /// Action - release frame
    E_VDEC_DISP_ACTION_RELEASE,
} VDEC_DispCmdAction;

/// Freeze picture select after flush decoder
typedef enum
{
    /// Freeze at current display picture
    E_VDEC_FREEZE_AT_CUR_PIC = 1,
    /// Freeze at the latest decode picture
    E_VDEC_FREEZE_AT_LAST_PIC,
} VDEC_FreezePicSelect;

/// Error code enumerator
typedef enum
{
    E_VDEC_ERR_CODE_BASE = 0x01000000,
    E_VDEC_ERR_CODE_NOT_SUPPORT,
    E_VDEC_ERR_CODE_ILLEGAL_ACCESS,
    E_VDEC_ERR_CODE_FRMRATE_NOT_SUPPORT,
    E_VDEC_ERR_CODE_DIVX_PLUS_UNSUPPORTED,
    E_VDEC_ERR_CODE_EXCEED_HW_CAP, ///over hw capability ex : avc 4k + avc 4k
    E_VDEC_ERR_CODE_CODEC_COMPARE_CASE1, ///Hash key error
    E_VDEC_ERR_CODE_CODEC_COMPARE_CASE2, ///Hash key error

    E_VDEC_MVD_ERR_CODE_BASE = 0x02000000,
        E_VDEC_MVD_ERR_CODE_SHAPE              ,
        E_VDEC_MVD_ERR_CODE_USED_SPRITE        ,
        E_VDEC_MVD_ERR_CODE_NOT_8_BIT          ,   ///< error_status : bits per pixel
        E_VDEC_MVD_ERR_CODE_NERPRED_ENABLE     ,
        E_VDEC_MVD_ERR_CODE_REDUCED_RES_ENABLE ,
        E_VDEC_MVD_ERR_CODE_SCALABILITY        ,
        E_VDEC_MVD_ERR_CODE_OTHER              ,
        E_VDEC_MVD_ERR_CODE_H263_ERROR         ,
        E_VDEC_MVD_ERR_CODE_RES_NOT_SUPPORT    ,   ///< error_status : none
        E_VDEC_MVD_ERR_CODE_MPEG4_NOT_SUPPORT  ,   ///< error_status : none
        E_VDEC_MVD_ERR_CODE_PROFILE_NOT_SUPPORT  , ///< error_status : none
        E_VDEC_MVD_ERR_CODE_RCV_ERROR_OCCUR    ,   ///< error_status : none
        E_VDEC_MVD_ERR_CODE_VC1_NOT_SUPPORT    ,   ///< error_status : none
        E_VDEC_MVD_ERR_CODE_UNKNOW_CODEC_NOT_SUPPORT  ,   ///< error_status : none
        E_VDEC_MVD_ERR_CODE_SLQ_TBL_NOT_SUPPORT  , ///< error_status : none
        E_VDEC_MVD_ERR_CODE_FRAME_BUF_NOT_ENOUGH , ///< error_status : none

    E_VDEC_HVD_ERR_CODE_BASE = 0x03000000,
        E_VDEC_HVD_ERR_CODE_GENERAL_BASE = (0x0000|E_VDEC_HVD_ERR_CODE_BASE),
        E_VDEC_HVD_ERR_CODE_OUT_OF_SPEC ,
        E_VDEC_HVD_ERR_CODE_UNKNOW_ERR,
        E_VDEC_HVD_ERR_CODE_HW_BREAK_DOWN,
        E_VDEC_HVD_ERR_CODE_HW_DEC_TIMEOUT,
        E_VDEC_HVD_ERR_CODE_OUT_OF_MEMORY,
        E_VDEC_HVD_ERR_CODE_UNKNOWN_CODEC,
        // AVC
        E_VDEC_HVD_ERR_CODE_AVC_BASE = (0x1000|E_VDEC_HVD_ERR_CODE_BASE),
        E_VDEC_HVD_ERR_CODE_AVC_SPS_BROKEN,
        E_VDEC_HVD_ERR_CODE_AVC_SPS_NOT_IN_SPEC,
        E_VDEC_HVD_ERR_CODE_AVC_SPS_NOT_ENOUGH_FRM,   ///< DPB size at specified level is smaller than the specified number of reference frames. This is not allowed
        E_VDEC_HVD_ERR_CODE_AVC_PPS_BROKEN,           ///< PPS is not valid
        E_VDEC_HVD_ERR_CODE_AVC_REF_LIST,
        E_VDEC_HVD_ERR_CODE_AVC_NO_REF,
        E_VDEC_HVD_ERR_CODE_AVC_RES,
        // AVS
        E_VDEC_HVD_ERR_CODE_AVS_BASE = (0x2000|E_VDEC_HVD_ERR_CODE_BASE),
        E_VDEC_HVD_ERR_CODE_AVS_RES,
        // RM
        E_VDEC_HVD_ERR_CODE_RM_BASE = (0x3000|E_VDEC_HVD_ERR_CODE_BASE),
        E_VDEC_HVD_ERR_CODE_RM_PACKET_HEADER,
        E_VDEC_HVD_ERR_CODE_RM_FRAME_HEADER,
        E_VDEC_HVD_ERR_CODE_RM_SLICE_HEADER,
        E_VDEC_HVD_ERR_CODE_RM_BYTE_CNT,
        E_VDEC_HVD_ERR_CODE_RM_DISP_TIMEOUT,
        E_VDEC_HVD_ERR_CODE_RM_NO_REF,
        E_VDEC_HVD_ERR_CODE_RM_VLC,
        E_VDEC_HVD_ERR_CODE_RM_SIZE,
        E_VDEC_HVD_ERR_CODE_RM_RES,

    E_VDEC_RVD_ERR_CODE_BASE = 0x04000000,
        E_VDEC_RVD_ERR_CODE_PACKET_HEADER,  ///< packet header version error
        E_VDEC_RVD_ERR_CODE_FRAME_HEADER,   ///< frame type error
        E_VDEC_RVD_ERR_CODE_SLICE_HEADER,   ///< slice header error
        E_VDEC_RVD_ERR_CODE_DECODE_TIMEOUT, ///< decode MB timeout
        E_VDEC_RVD_ERR_CODE_OUT_OF_MEMORY,  ///< frame buffer is out of memory
        E_VDEC_RVD_ERR_CODE_BYTE_POS,       ///< can not find in ID table
        E_VDEC_RVD_ERR_CODE_DISPLAY_TIMEOUT,///

    E_VDEC_MJPEG_ERR_CODE_BASE = 0x05000000,
        E_VDEC_HVD_ERR_CODE_MJPEG_RES,
} VDEC_ErrCode;

/// Frame rate conversion mode enumerator
typedef enum
{
    /// Disable FRC mode.
    E_VDEC_FRC_NORMAL = 0,
    /// Output rate is twice of input rate (ex. 30p to 60p)
    E_VDEC_FRC_DISP_TWICE,
    /// 3:2 pulldown mode (ex. 24p to 60i or 60p)
    E_VDEC_FRC_3_2_PULLDOWN,
    /// PAL to NTSC conversion (50i to 60i)
    E_VDEC_FRC_PAL_TO_NTSC,
    /// NTSC to PAL conversion (60i to 50i)
    E_VDEC_FRC_NTSC_TO_PAL,
    /// Output rate 50P ->60P
    E_VDEC_FRC_MODE_50P_60P,
    /// Output rate 60P ->50P
    E_VDEC_FRC_MODE_60P_50P,
} VDEC_FrcMode;

/// Trick decode mode enumerator
typedef enum
{
    /// decode all frame
    E_VDEC_TRICK_DEC_ALL = 0,
    /// decode all except of non-reference frame
    E_VDEC_TRICK_DEC_IP,
    /// only decode I frame
    E_VDEC_TRICK_DEC_I,
} VDEC_TrickDec;

/// Display speed setting enumerator
typedef enum
{
    /// default speed type
    E_VDEC_SPEED_DEFAULT = 0,
    /// fast display
    E_VDEC_SPEED_FAST,
    /// slow display
    E_VDEC_SPEED_SLOW,
} VDEC_SpeedType;

/// The display speed enumerator
typedef enum
{
    /// Normal display speed.
    E_VDEC_DISP_SPEED_1X = 1,
    /// 2X
    E_VDEC_DISP_SPEED_2X = 2,
    /// 4X
    E_VDEC_DISP_SPEED_4X = 4,
    /// 8X
    E_VDEC_DISP_SPEED_8X = 8,
    /// 16X
    E_VDEC_DISP_SPEED_16X = 16,
    /// 32X
    E_VDEC_DISP_SPEED_32X = 32,
} VDEC_DispSpeed;

/// Motion JPEG down scale factor enumerator
typedef enum
{
    ///Original size
    E_VDEC_MJPEG_SCALE_1to1 = 0,
    ///Down scale to 1/2
    E_VDEC_MJPEG_SCALE_2to1,
    ///Down scale to 1/4
    E_VDEC_MJPEG_SCALE_4to1,
    ///Down scale to 1/8
    E_VDEC_MJPEG_SCALE_8to1,
} VDEC_MJpegScaleFactor;

/// Timestamp type of command queue
typedef enum
{
    ///without timestamp information
    E_VDEC_TIME_STAMP_NONE = 0,
    ///PTS (Presentation Time Stamp)
    E_VDEC_TIME_STAMP_PTS,
    ///DTS (Decode Time Stamp)
    E_VDEC_TIME_STAMP_DTS,
    ///STS (Sorted Time Stamp)
    E_VDEC_TIME_STAMP_STS,
    ///PTS_RVU (Presentation Time Stamp)
    E_VDEC_TIME_STAMP_PTS_MPEG_DIRECTV_SD,
    ///DTS_RVU (Decode Time Stamp)
    E_VDEC_TIME_STAMP_DTS_MPEG_DIRECTV_SD,

} VDEC_TimeStampType;

/// Debug level of VDEC
typedef enum
{
    /// Disable all uart messages.
    E_VDEC_DBG_LEVEL_NONE = 0,
    /// Only output error messages
    E_VDEC_DBG_LEVEL_ERR,
    /// Output general messages, and above.
    E_VDEC_DBG_LEVEL_INFO,
    /// Output debug messages, and above.
    E_VDEC_DBG_LEVEL_DBG,
    /// Output function tracing messages, and above.
    E_VDEC_DBG_LEVEL_TRACE,
    /// Output FW messages.
    E_VDEC_DBG_LEVEL_FW,
} VDEC_DbgLevel;

/// Type of FW source
typedef enum
{
    E_VDEC_FW_SOURCE_NONE,
    E_VDEC_FW_SOURCE_DRAM,
    E_VDEC_FW_SOURCE_FLASH,
}VDEC_FWSourceType;

/// Format of CC (Closed Caption)
typedef enum
{
    E_VDEC_CC_NONE       = 0x00,
    E_VDEC_CC_608        = 0x01, //For CC608 or 157
    E_VDEC_CC_708        = 0x02, //For CC708
    E_VDEC_CC_UNPACKED   = 0x03,
} VDEC_CCFormat;

/// Type of CC
typedef enum
{
    E_VDEC_CC_TYPE_NONE = 0,
    E_VDEC_CC_TYPE_NTSC_FIELD1 = 1,
    E_VDEC_CC_TYPE_NTSC_FIELD2 = 2,
    E_VDEC_CC_TYPE_DTVCC = 3,
    E_VDEC_CC_TYPE_NTSC_TWOFIELD = 4,
} VDEC_CCType;

typedef enum
{
    E_VDEC_CC_GET_BUFF_START = 0x1,
    E_VDEC_CC_GET_BUFF_SIZE,
    E_VDEC_CC_GET_708_ENABLE
} VDEC_CCInfoCmd;

typedef enum
{
    E_VDEC_STAGE_STOP = 0,
    E_VDEC_STAGE_INIT,
    E_VDEC_STAGE_PLAY,
    E_VDEC_STAGE_PAUSE,
} VDEC_Stage;


typedef enum
{
    /// Used before MApi_VDEC_Flush().
    E_VDEC_PATTERN_FLUSH = 0,
    /// Used after MApi_VDEC_EnableLastFrameShow().
    E_VDEC_PATTERN_FILEEND,
}VDEC_PatternType;

typedef enum
{
    E_VDEC_FW_STATUS_FLAG_NONE       = 0,
    E_VDEC_FW_STATUS_FLAG_SEEK_TO_I  = VDEC_BIT(0),
}VDEC_FW_STATUS_FLAG;

typedef struct
{
    MS_BOOL bInit;
    MS_BOOL bIdle;
    VDEC_Stage  eStage;
} VDEC_Status;

typedef struct
{
    MS_U32 u32Tmp;
} VDEC_Info;

typedef enum
{
    E_VDEC_FRM_TYPE_I = 0,
    E_VDEC_FRM_TYPE_P,
    E_VDEC_FRM_TYPE_B,
    E_VDEC_FRM_TYPE_OTHER,
} VDEC_FrameType;

typedef enum
{
    E_VDEC_FIELDTYPE_NONE,  ///< No field.
    E_VDEC_FIELDTYPE_TOP,   ///< Top field only.
    E_VDEC_FIELDTYPE_BOTTOM,///< Bottom field only.
    E_VDEC_FIELDTYPE_BOTH,  ///< Both fields.
} VDEC_FieldType;

typedef enum
{
    E_VDEC_PATTERN_BEFORE_FRM = 0,
    E_VDEC_PATTERN_AFTER_FRM,
    E_VDEC_PATTERN_SKIP_DATA,
} VDEC_PatchPattern;

typedef enum
{
    E_VDEC_PIC_STRUCTURE_RSV = 0, //reserved
    E_VDEC_PIC_STRUCTURE_TOP,
    E_VDEC_PIC_STRUCTURE_BOT,
    E_VDEC_PIC_STRCUTURE_FRM,
} VDEC_PicStructure;

//VDEC set clock speed
typedef enum
{
    E_VDEC_CLOCK_SPEED_NONE = 0,
    E_VDEC_CLOCK_SPEED_HIGHEST,
    E_VDEC_CLOCK_SPEED_HIGH,
    E_VDEC_CLOCK_SPEED_MEDIUM,
    E_VDEC_CLOCK_SPEED_LOW,
    E_VDEC_CLOCK_SPEED_LOWEST,
    E_VDEC_CLOCK_SPEED_DEFAULT,
} VDEC_ClockSpeed;

//VDEC picture scan mode
typedef enum
{
    E_VDEC_PROGRESSIVE = 0,
    E_VDEC_INTERLACE_FRAME = 2,
    E_VDEC_INTERLACE_FIELD = 3,
} VDEC_PictureScanMode;

///vdec plus drop ratio
typedef enum
{
    E_VDEC_VDEC_PLUS_DROP_RATIO_1 = 1, //no skip data
    E_VDEC_VDEC_PLUS_DROP_RATIO_2 = 2, //skip 1/2 data
    E_VDEC_VDEC_PLUS_DROP_RATIO_4 = 4, //skip 3/4 data
} VDEC_VdecPlusDropRatio;

//VDEC user command id
typedef enum
{
    //Group1:Set Control command
    VDEC_USER_CMD_SET_CONTROL_BASE = 0x0000,
    VDEC_USER_CMD_REPEAT_LAST_FIELD,                // Param: 1(ON), 0(OFF)
    VDEC_USER_CMD_AVSYNC_REPEAT_TH,                 // Param:0x01 ~ 0xFF(repeat times), 0xFF:always repeat when av is not sync
    VDEC_USER_CMD_DISP_ONE_FIELD,                   // Param: 1(ON), 0(OFF)
    VDEC_USER_CMD_FD_MASK_DELAY_COUNT,              // Param: unit is in vsync base for mute the fd_mask
    VDEC_USER_CMD_FRC_OUTPUT,                       // Param: the address of VDEC_FRC_OutputParam
    VDEC_USER_CMD_FRC_DROP_TYPE,                    // Param: 1(FRC_DROP_FIELD), 0(FRC_DROP_FRAME), default:0
    VDEC_USER_CMD_FAST_DISPLAY,                     // Param: TRUE(Fast display), FALSE(Display until synced)
    VDEC_USER_CMD_IGNORE_ERR_REF,                   // Param: TRUE(Ignore error reference), FALSE(Enable error reference handle)
    VDEC_USER_CMD_FORCE_FOLLOW_DTV_SPEC,            // Param: 1(ON), 0(OFF)
    VDEC_USER_CMD_AVC_MIN_FRM_GAP,                  // Param: Set the theshold of H264 frame gap, 0xFFFFFFFF don't care frame gap
    VDEC_USER_CMD_DISABLE_SEQ_CHG,                  // Param: 1(Disable), 0(Enable)
    VDEC_USER_CMD_AVC_DISABLE_ANTI_VDEAD,           // Param: 1(Disable), 0(Enable)
    VDEC_USER_CMD_DTV_RESET_MVD_PARSER,             //
    VDEC_USER_CMD_PVR_FLUSH_FRAME_BUFFER,           //
    VDEC_USER_CMD_SET_DTV_USER_DATA_MODE,           // Param: 0(Support normal DVB CC, default case), 1(Support ATSC DirectTV CC), 2,3,4(Reserved)
    VDEC_USER_CMD_SET_SINGLE_TASK_MODE,             // Param: 0(default case), 1(single task mode) for dual decoder
    VDEC_USER_CMD_FORCE_INTERLACE_MODE,             // Param: 0(Disable), 1(Enable) DTV, TS-File mode force into interlace mode when framerate is 25 or 30
    VDEC_USER_CMD_RELEASE_FD_MASK,
    VDEC_USER_CMD_SET_DECODE_MODE,
    VDEC_USER_CMD_SHOW_DECODE_ORDER,                // Param: 1(Disable), 0(Enable)
    VDEC_USER_CMD_SUPPORT_AVC_TO_MVC,               // Param: 0(Do not support), 1(Support AVC to MVC)
    VDEC_USER_CMD_3DLR_VIEW_EXCHANGE,               // Param: 0(Disable), 1(View L/R exhange)
    VDEC_USER_CMD_FORCE_AUTO_MUTE,                  // param: 0(default case), 1(Enable)
    VDEC_USER_CMD_SET_VSIZE_ALIGN,                  // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_AVC_DISP_IGNORE_CROP,             // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SET_DISP_FINISH_MODE,             // Param: 0(Original), 1(Strict)
    VDEC_USER_CMD_SET_AVSYNC_MODE,                  // Param: 0(Original), 1(Strict)
    VDEC_USER_CMD_SET_CC608_INFO_ENHANCE_MODE,      // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_PUSH_DISPQ_WITH_REF_NUM,          // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SET_IDCT_MODE,                    //Param: 0(Original), 1(new IDCT)
    VDEC_USER_CMD_DROP_ERR_FRAME,                   // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SHOW_FIRST_FRAME_DIRECT,          // Param: 0(Disable), 1(Enable), Push first frame to display queue directly..
    VDEC_USER_CMD_SET_XC_LOW_DELAY_PARA,            // Param: arg0 for diff_field_number...
    VDEC_USER_CMD_AUTO_EXHAUST_ES_MODE,             // Param: set the upper bound (arg[31:16]), and lower bound (arg[15:0])of ES level, Unit = 1KBytes, Auto drop display to consume ES data as soon as possible when ES level is higher than upper bound
    VDEC_USER_CMD_SET_MIN_TSP_DATA_SIZE,            // Param: Resize HVD_FW_AVC_ES_MIN_TSP_DATA_SIZE
    VDEC_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE, // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_FRAMEBUFFER_AUTO_MODE,            // Param: 0(Disable), 1(Enable)
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_300)
    VDEC_USER_CMD_AUTO_DROP_DISPLAY_QUEUE,          // Param: 0(Disable), N = 1~16: Drop display queue when display queue above than N frames.
    VDEC_USER_CMD_SET_PTS_US_MODE,                   //Param: 1(enable), 0(disable ) PTS output by micro second level,
    VDEC_USER_CMD_FORCE_PROGRESSIVE_MODE,            // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_AVC_NEW_SLOW_MOTION,                 // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_USE_CPB_REMOVAL_DEALY,			// Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_RETURN_INVALID_AFD,               // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SET_DV_XC_SHM_ADDR,              // Param: PHY Addr for communicating with XC Dolby Vision DM/Comp
    VDEC_USER_CMD_SET_ENABLE_HDR,                  // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_DISABLE_PBFRAME_MODE,            // Param: 1(Disable), 0(Enable)
    VDEC_USER_CMD_SET_SLOW_SYNC,                    //Param: VDEC_EX_Slow_Sync, u8RepeatPeriod: 0(disable), 1~0xFF(Repeat Period, repeat one every u8RepeatPeriod display),  u8DropPeriod: 0(disable), 1~0xFF(Drop Period, Drop one every u8DropPeriod display)
    VDEC_USER_CMD_SET_FRAMEBUFF2,                   // Param[0]=Addr and Param[1]=size for the second frame buffer
    VDEC_USER_CMD_SET_TRICKPLAY_2X_MODE,           //Param:0(vsync),1(avsync)
    VDEC_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD,         // Param: 0(Disable), 1(Enable) only show top filed for FRC mode
    VDEC_USER_CMD_DISABLE_ES_FULL_STOP,             // Param: 0(Enable), 1(Disable) ES FULL STOP mode
    VDEC_USER_CMD_DIRECT_STC_MODE,                  // Param: stc in ms; 0x0~0x1FFFFFFFF/90, vdec fw use this value as stc; 0xFFFFFFFF, disable ths feature
#endif
#if defined(UFO_PUBLIC_HEADER_212)
    VDEC_USER_CMD_AUTO_DROP_DISPLAY_QUEUE,          // Param: 0(Disable), N = 1~16: Drop display queue when display queue above than N frames.
    VDEC_USER_CMD_USE_CPB_REMOVAL_DEALY,           // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SET_PTS_US_MODE,                   //Param: 1(enable), 0(disable ) PTS output by micro second level,
    VDEC_USER_CMD_FORCE_PROGRESSIVE_MODE,            // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SET_TRICKPLAY_2X_MODE,           //Param:0(vsync),1(avsync)
    VDEC_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD,         // Param: 0(Disable), 1(Enable) only show top filed for FRC mode
    VDEC_USER_CMD_DISABLE_ES_FULL_STOP,             // Param: 0(Enable), 1(Disable) ES FULL STOP mode
    VDEC_USER_CMD_DIRECT_STC_MODE,                  // Param: stc in ms; 0x0~0x1FFFFFFFF/90, vdec fw use this value as stc; 0xFFFFFFFF, disable ths feature
    VDEC_USER_CMD_AVC_NEW_SLOW_MOTION,                 // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_RETURN_INVALID_AFD,               // Param: 0(Disable), 1(Enable)
    VDEC_USER_CMD_SET_SLOW_SYNC,                    // Param: VDEC_EX_Slow_Sync, u8RepeatPeriod: 0(disable), 1~0xFF(Repeat Period, repeat one every u8RepeatPeriod display),  u8DropPeriod: 0(disable), 1~0xFF(Drop Period, Drop one every u8DropPeriod display)
    VDEC_USER_CMD_SET_FRAMEBUFF2,                   // Param[0]=Addr and Param[1]=size for the second frame buffer
    VDEC_USER_CMD_DISABLE_PBFRAME_MODE,            // Param: 1(Disable), 0(Enable)
    VDEC_USER_CMD_SET_DV_XC_SHM_ADDR,              // Param: PHY Addr for communicating with XC Dolby Vision DM/Comp
    VDEC_USER_CMD_SET_ENABLE_HDR,                  // Param: 0(Disable), 1(Enable)
#endif
    VDEC_USER_CMD_SET_BUFFER_INFO,                 //Param: VDEC_EX_BufferInfo
    VDEC_USER_CMD_HDR10_UPDATE_PERFRAME,
    VDEC_USER_CMD_SET_CODEC_CONFIG,
    VDEC_USER_CMD_SET_VP94K2KCHECK,
    VDEC_USER_CMD_PUSI_CONTROL,
    VDEC_USER_CMD_DISABLE_FDMASK,
    VDEC_USER_CMD_SET_AR_CHANGE_MODE,
    VDEC_USER_CMD_MPEG4_CAL_FRAMERATE,
    VDEC_USER_CMD_IGNORE_PIC_OVERRUN,
    VDEC_USER_CMD_PARSER_BYPASS,
    VDEC_USER_CMD_SET_MIN_VDEC_PLUS_DROP_RATIO,     // Set minimal vdec plus drop ratio for skip data, Param VDEC_VdecPlusDropRatio

    VDEC_USER_CMD_MVC_SET_CMD_BASE = 0x0800,
    VDEC_USER_CMD_MVC_BBU2_PUSH_PACKET,             // Param: Packet Info.
    VDEC_USER_CMD_MVC_BBU2_FIRE_DECCMD,             // Param: Non

    //Group2:Get Control command
    VDEC_USER_CMD_GET_CONTROL_BASE = 0x1000,
    VDEC_USER_CMD_GET_CHROMA_TYPE,
    VDEC_USER_CMD_GET_REAL_FRAMERATE,               // Get Real FrameRate reported by decoder
    VDEC_USER_CMD_GET_COLOR_MATRIX,                 // Get color matrix coefficients reported by decoder
    VDEC_USER_CMD_GET_DYNSCALE_ENABLED,             // Get if DynamicScaling is enabled by firmware
    VDEC_USER_CMD_GET_ORI_INTERLACE_MODE,
    VDEC_USER_CMD_GET_FPA_SEI,                      // Get Frame Packing Arrangment information
    VDEC_USER_CMD_GET_U64PTS,
    VDEC_USER_CMD_GET_FB_USAGE_MEM,                 // Get FrameBuufer Size needed by decoder
    VDEC_USER_CMD_GET_XC_LOW_DELAY_INT_STATE,       // Get xc_low_delay int state...
    VDEC_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM,		// Get Flush patten push based (unit : times)
    VDEC_USER_CMD_GET_FW_STATUS_FLAG,
    VDEC_USER_CMD_GET_DS_BUF_MIU_SEL,
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_300)
    VDEC_USER_CMD_GET_PVRSEAMLESS_INFO,
    VDEC_USER_CMD_GET_PRE_PAS_U64PTS,               // Get Pre-parsing PTS
    VDEC_USER_CMD_GET_SEQ_CHANGE_INFO,              // Get the reason why seq changes, parameter is MS_U32, definition is VDEC_SeqChangeInfo
    VDEC_USER_CMD_GET_MIN_TSP_DATA_SIZE,
    VDEC_USER_CMD_GET_CLLI_SEI,
    VDEC_USER_CMD_GET_DISP_QUEUE_EMPTY,
    VDEC_USER_CMD_GET_BUFFER_INFO,
    VDEC_USER_CMD_GET_NEXT_DISP_FRAME_QOS_INFO,
    VDEC_USER_CMD_GET_NOT_SUPPORT_INFO,
#endif
#if defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)
    VDEC_USER_CMD_GET_PRE_PAS_U64PTS,               // Get Pre-parsing PTS
    VDEC_USER_CMD_GET_SEQ_CHANGE_INFO,              // Get the reason why seq changes, parameter is MS_U32, definition is VDEC_SeqChangeInfo
    VDEC_USER_CMD_GET_DISP_QUEUE_EMPTY,
    VDEC_USER_CMD_GET_NOT_SUPPORT_INFO,
    VDEC_USER_CMD_GET_PVRSEAMLESS_INFO,
    VDEC_USER_CMD_GET_MIN_TSP_DATA_SIZE,
    VDEC_USER_CMD_GET_CLLI_SEI,
    VDEC_USER_CMD_GET_BUFFER_INFO,
    VDEC_USER_CMD_GET_NEXT_DISP_FRAME_QOS_INFO,
#endif
#if defined(UFO_PUBLIC_HEADER_212)
    VDEC_USER_CMD_GET_PVRSEAMLESS_INFO,
    VDEC_USER_CMD_GET_PRE_PAS_U64PTS,               // Get Pre-parsing PTS
    VDEC_USER_CMD_GET_SEQ_CHANGE_INFO,              // Get the reason why seq changes, parameter is MS_U32, definition is VDEC_SeqChangeInfo
    VDEC_USER_CMD_GET_MIN_TSP_DATA_SIZE,
    VDEC_USER_CMD_GET_CLLI_SEI,
    VDEC_USER_CMD_GET_DISP_QUEUE_EMPTY,
    VDEC_USER_CMD_GET_BUFFER_INFO,
    VDEC_USER_CMD_GET_NEXT_DISP_FRAME_QOS_INFO,
    VDEC_USER_CMD_GET_NOT_SUPPORT_INFO,
#endif
    VDEC_USER_CMD_IS_MORE_FRAME,
    VDEC_USER_CMD_GET_SIMPLE_FRAME,
    VDEC_USER_CMD_GET_PICTURE_SCAN_MODE,            // Get the picture scan mode, Param: VDEC_EX_V2_PictureScanMode

    VDEC_USER_CMD_MVC_GET_CMD_BASE = 0x1800,
    VDEC_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO,     // Param: VDEC_FrameInfo pointer.
    VDEC_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY,       // Param: BBU2 Dec Q Vacancy.
    VDEC_USER_CMD_GET_MVC_ES2_READ_PTR,            // Param: ES2 read pointer.
    VDEC_USER_CMD_GET_MVC_ES2_WRITE_PTR,           // Param: ES2 Write pointer.
    VDEC_USER_CMD_GET_ES_QUANTITY,                 // Param: Get ES buffer Level.
    VDEC_USER_CMD_GET_ES2_QUANTITY,                // Param: Get ES2 buffer Level.
    VDEC_USER_CMD_GET_MAIN_STREAM_ID,              // Param: Get main stream ID.

    //Group3:System Preset Control command======================
    //Group3-1:Common system Preset Control command
    VDEC_USER_CMD_SYSTEM_PRESET_CONTROL_BASE    = 0x2000,
    VDEC_USER_CMD_SYSTEM_PRESET_VPU_CLOCK,          //Param: VDEC_EX_ClockSpeed
    VDEC_USER_CMD_RELEASE_FB_SLOT,                  //Param: Task ID
    VDEC_USER_CMD_SHOW_FB_USAGE,                    //Param: None, used to show the FB status

    //Group3-2:HVD System Preset Control command
    VDEC_USER_CMD_SYSTEM_PRESET_HVD_BASE        = 0x2100,
    VDEC_USER_CMD_SYSTEM_PRESET_HVD_CLOCK,          //Param: VDEC_EX_ClockSpeed

    //Group3-3:MVD System Preset Control command
    VDEC_USER_CMD_SYSTEM_PRESET_MVD_BASE        = 0x2200,
    VDEC_USER_CMD_SYSTEM_PRESET_MVD_CLOCK,          //Param: VDEC_EX_ClockSpeed
    VDEC_USER_CMD_VPU_SECURITY_MODE,                //Param: 0:disable,1:enable

    //Group4:Preset Control command=============================
    VDEC_USER_CMD_PRESET_CONTROL_BASE           = 0x2300,
    VDEC_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE,      //Param: 0(Disable), 1(Enable), use only one pending buffer instead of two for HVD
    VDEC_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE,      //Param: 0(Disable), 1(Enable), Allcate HW buffer to start of frame buffer
    VDEC_USER_CMD_FRAMERATE_HANDLING,               //Param: 1000~60000, Set frame rate when the sequence did not have frame rate info. The unit is (param/1000)fps, Ex: 30000 = 30.000 fps
                                                    //Param: 0xFFFFFFFF, use default frame rate when no frame rate info in header
    VDEC_USER_CMD_SET_SHAREMEMORY_BASE,
    VDEC_USER_CMD_HVD_COL_BBU_MODE,                 //Param: HVD use colocated BBU mode, 0: disable, 1: enable /*johnny.ko*/
    VDEC_USER_CMD_HVD_IAPGN_BUF_SHARE_BW_MODE,      //Param: HVD IAP GN Buffer address,
    VDEC_USER_CMD_DTV_DEBUG_MODE,
    VDEC_USER_CMD_HVD_TS_IN_BBU_MODE,
    VDEC_USER_CMD_AUTO_ARRANGE_FRAMEBUFFER_USAGE,
    VDEC_USER_CMD_THUMBNAIL_MODE,
    VDEC_USER_CMD_FORCE_8BIT_DEC_MODE,
    VDEC_USER_CMD_MFCODEC_MODE,                     //Param: VDEC_EX_MFCodec_mode
    VDEC_USER_CMD_VDEC_FEATURE,                     //AP control VDEC features
    VDEC_USER_CMD_SET_DISPLAY_MODE,
    VDEC_USER_CMD_CAL_FRAMERATE,
    VDEC_USER_CMD_SETPOWERSTATE,
    VDEC_USER_CMD_FORCE_START_NON_I_SLICE,          //Param: Force to start decode from non I slice, 0: disable, 1: enable
    VDEC_USER_CMD_SWITCH_DISP_PATH,                 //Force switch MVOP, task0->mvop1, task1->mvop0

    //Group5:System PreGet Control command======================
    //Group5-1:Common system Preget Control command
    VDEC_USER_CMD_SYSTEM_PREGET_CONTROL_BASE  = 0x4000,
    VDEC_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE_SIZE,
    VDEC_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_PROFILE,
    VDEC_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_LEVEL,
    VDEC_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE,
    VDEC_USER_CMD_SYSTEM_PREGET_VDEC_UUID,       // Param:get VDEC UUID (OPTEE use)
    VDEC_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE_BY_FEATURE,

} VDEC_User_Cmd;

//VDEC FW TYPE
typedef enum
{
    VDEC_FW_TYPE_MVD = 0,
    VDEC_FW_TYPE_HVD,
} VDEC_FwType;

//VDEC FB reduction type
typedef enum
{
    VDEC_FB_REDUCTION_NONE = 0,
    VDEC_FB_REDUCTION_1_2,
    VDEC_FB_REDUCTION_1_4
} VDEC_FBReductionType;

//VDEC XC_LOW_DELAY
typedef enum
{
    // no any XC_LOW_DELAY event happened.
    VDEC_XC_LOW_DELAY_NONE              = 0x00,
    // XC_LOW_DELAY event for disable black screen.
    VDEC_XC_DISABLE_BLACK_SCREEN        = VDEC_BIT(0),
    // XC_LOW_DELAY event for release force_read_bank.
    VDEC_XC_RELEASE_FORCE_READ_BANK     = VDEC_BIT(1),
    // XC_LOW_DELAY event for release bob_mode.
    VDEC_XC_RELEASE_BOB_MODE            = VDEC_BIT(2),
} VDEC_XcLowDelayType;

// VDEC SEQ change info
typedef enum
{
    // seq change due to first sequence
    VDEC_SEQ_CHANGE_FIRST_TIME        = VDEC_BIT(0),
    // seq chagne due to resolution
    VDEC_SEQ_CHANGE_RESOLUTION        = VDEC_BIT(1),
    // seq chagne due to picture type
    VDEC_SEQ_CHANGE_PICTURE_TYPE      = VDEC_BIT(2),
    // seq chagne due to aspect ratio
    VDEC_SEQ_CHANGE_ASPECT_RATIO      = VDEC_BIT(3),
    // seq chagne due to frame rate
    VDEC_SEQ_CHANGE_FRAME_RATE        = VDEC_BIT(4),
    // seq chagne due to HDR info
    VDEC_SEQ_CHANGE_HDR_INFO          = VDEC_BIT(5),
} VDEC_SeqChangeInfo;


/// Data structure of CC Configuration
typedef struct
{
    /// 608 / 708
    VDEC_CCFormat   eFormat;
    /// CC type
    VDEC_CCType     eType;
    /// CC data start address
    MS_VIRT          u32BufStAdd;
    /// CC data size
    MS_U32          u32BufSize;
} VDEC_CCCfg;

/// Information for display setting
typedef struct
{
    ///Bitstream horizontal size
    MS_U16 u16HorSize;
    ///Bitstream vertical size
    MS_U16 u16VerSize;
    ///Frame rate
    MS_U32 u32FrameRate;
    ///Interlace flag
    MS_U8 u8Interlace;
    ///Active format description
    MS_U8 u8AFD;
    ///Sample aspect rate width
    MS_U16 u16SarWidth;
    ///Sample aspect rate height
    MS_U16 u16SarHeight;
    ///Right cropping
    MS_U16 u16CropRight;
    ///Left cropping
    MS_U16 u16CropLeft;
    ///Bottom cropping
    MS_U16 u16CropBottom;
    ///Top cropping
    MS_U16 u16CropTop;
    ///Pitch
    MS_U16 u16Pitch;
    ///Interval of PTS
    MS_U16 u16PTSInterval;
    ///MPEG1 flag
    MS_U8 u8MPEG1;
    ///Play mode (fixme)
    MS_U8 u8PlayMode;
    ///FRC mode
    MS_U8 u8FrcMode;
    ///Aspect ratio code
    MS_U8 u8AspectRate;
    ///if FALSE, set VOP as mono mode (only for H264)
    MS_BOOL bWithChroma;
    /// if true, color space is xvYCC (Y from 16 to 235 and Cb , Cr from 16 to 240).
    /// if false, color space is BT.601/709 (Y from  0 to 255 and Cb , Cr from  0 to 255).
    /// only MPEG might be with BT.601/709
    MS_BOOL bColorInXVYCC;
    ///Dynamic scaling buffer address
    MS_VIRT u32DynScalingAddr;
    ///Dynamic scaling buffer size
    MS_U32 u32DynScalingSize;
    ///Dynamic scaling depth
    MS_U8 u8DynScalingDepth;
    ///Dynamic scaling DS buffer on miu1 or miu0
    MS_BOOL bEnableMIUSel;
    ///Display width
    MS_U32 u32AspectWidth;
    ///Display height
    MS_U32 u32AspectHeight;
} VDEC_DispInfo;

/// System configuration
typedef struct
{
    ///FW binary start address
    MS_PHY u32FWBinaryAddr;
    ///FW binary size
    MS_U32 u32FWBinarySize;
    ///FW code buffer start address
    MS_PHY u32CodeBufAddr;
    ///FW code buffer size
    MS_U32 u32CodeBufSize;
    ///Frame buffer start address
    MS_PHY u32FrameBufAddr;
    ///Frame buffer size
    MS_U32 u32FrameBufSize;
    ///Bitstream buffer start address
    MS_PHY u32BitstreamBufAddr;
    ///Bitstream buffer size
    MS_U32 u32BitstreamBufSize;
    ///Driver process buffer start address
    MS_PHY u32DrvProcBufAddr;
    ///Driver process buffer size
    MS_U32 u32DrvProcBufSize;
    ///VLC table Binary address (RM only)
    MS_PHY u32VlcBinarySrcAddr;
    ///VLC table Binary size
    MS_U32 u32VlcTabBinarySize;
    ///Debug level setting
    VDEC_DbgLevel eDbgMsgLevel;
    ///FW source type seeting
    VDEC_FWSourceType eFWSourceType;
} VDEC_SysCfg;

/// Video information
typedef struct
{
    ///input source mode
    VDEC_SrcMode   eSrcMode;
    /// timestamp type of command queue
    VDEC_TimeStampType      eTimeStampType;
    /// MJPEG scale factor
    VDEC_MJpegScaleFactor   eMJpegScaleFactor;
    /// should be TRUE when codec type is H264 and container is MKV and MP4(MOV)
    MS_BOOL bWithoutNalStCode;
    /// needless when CodecType is MJPEG and divx311
    MS_U32 u32FrameRate;
    MS_U32 u32FrameRateBase;
    /// if divx311; use u16Width[0]; only need other elements when RV8
    MS_U16  u16Width[8];
    /// if divx311; use u16Height[0]; only need other elements when RV8
    MS_U16  u16Height[8];
    /// video number sizes (for RM)
    MS_U16  u16NumSizes;
} VDEC_VideoInfo;

/// Frame information
typedef struct
{
    /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_PHY u32LumaAddr;
    /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_PHY u32ChromaAddr;
    /// Time stamp(DTS, PTS) of current displayed frame. Unit: ms (todo: 90khz)
    MS_U32 u32TimeStamp;
    /// low part of ID number
    MS_U32 u32ID_L;
    /// high part of ID number or timestamp
    MS_U32 u32ID_H;
    /// pitch
    MS_U16 u16Pitch;
    /// width
    MS_U16 u16Width;
    /// height
    MS_U16 u16Height;
    ///< Frame type: I, P, B frame
    VDEC_FrameType eFrameType;
    ///< Field type: Top, Bottom, Both
    VDEC_FieldType eFieldType;
} VDEC_FrameInfo;

/// Extension display information
typedef struct
{
    /// vertical size from sequene_display_extension
    MS_U16 u16VSize;
    /// horizontal size from sequene_display_extension
    MS_U16 u16HSize;
    /// vertical offset from picture_display_extension
    MS_S16 s16VOffset;
    /// horizontal offset from picture_display_extension
    MS_S16 s16HOffset;
} VDEC_ExtDispInfo;

/// Display frame information
typedef struct
{
    ///< frame information
    VDEC_FrameInfo stFrmInfo;
    ///< firmware private data
    MS_U32 u32PriData;
    ///< index used by apiVDEC to manage VDEC_DispQ[][]
    MS_U32 u32Idx;
    ///< display information
    VDEC_DispInfo stDispInfo;
} VDEC_DispFrame;

/// Time code structure
typedef struct
{
    ///  time_code_hours
    MS_U8   u8TimeCodeHr;
    ///  time_code_minutes
    MS_U8   u8TimeCodeMin;
    ///  time_code_seconds
    MS_U8   u8TimeCodeSec;
    ///  time_code_pictures
    MS_U8   u8TimeCodePic;
    ///  drop_frame_flag
    MS_U8   u8DropFrmFlag;
    ///  reserved fields for 4-byte alignment
    MS_U8   u8Reserved[3];
} VDEC_TimeCode;

/// VDEC frame buffer reduction
typedef struct
{
    VDEC_FBReductionType eLumaFBReduction;
    VDEC_FBReductionType eChromaFBReduction;
    MS_BOOL              bEnableAutoMode;      /// 0: Disable, 1: Enable
} VDEC_FBReduction;

/// Initial parameter
typedef struct
{
    /// codec type
    VDEC_CodecType  eCodecType;
    /// system configuration
    VDEC_SysCfg     SysConfig;
    /// video information from container
    VDEC_VideoInfo  VideoInfo;
    /// dynamic scaling control bit
    MS_BOOL         EnableDynaScale;
    /// switch for display decode error frame or not
    MS_BOOL         bDisableDropErrFrame;
    /// switch for error concealment
    MS_BOOL         bDisableErrConceal;
    /// enable repeat last field when repeat happened at interlace stream
    MS_BOOL         bRepeatLastField;
    /// threshold to judge error frame
    MS_U8           u8ErrThreshold;
    /// dynamic scaling virtual box Width
    MS_U32          u32DSVirtualBoxWidth;
    /// dynamic scaling virtual box Height
    MS_U32          u32DSVirtualBoxHeight;
} VDEC_InitParam;

/// Initial parameter, version = 1
typedef struct
{
    /// init param version : 1
    MS_U32          u32Version;
    /// codec type
    VDEC_CodecType  eCodecType;
    /// system configuration
    VDEC_SysCfg     SysConfig;
    /// video information from container
    VDEC_VideoInfo  VideoInfo;
    /// dynamic scaling control bit
    MS_BOOL         EnableDynaScale;
    /// switch for display decode error frame or not
    MS_BOOL         bDisableDropErrFrame;
    /// switch for error concealment
    MS_BOOL         bDisableErrConceal;
    /// enable repeat last field when repeat happened at interlace stream
    MS_BOOL         bRepeatLastField;
    /// threshold to judge error frame
    MS_U8           u8ErrThreshold;
    /// dynamic scaling virtual box Width
    MS_U32          u32DSVirtualBoxWidth;
    /// dynamic scaling virtual box Height
    MS_U32          u32DSVirtualBoxHeight;
    /// vdec frame buffer reduction setting
    VDEC_FBReduction    stFBReduction;
} VDEC_InitParam_EX1;

/// Decode Command
typedef struct
{
    /// ID (high 4-bytes) or timestamp of payload (high byte)
    MS_U32  u32ID_H;
    /// ID (low 4-bytes)
    MS_U32  u32ID_L;
    /// start address of payload
    MS_U32  u32StAddr;
    /// size of payload
    MS_U32  u32Size;
    /// timestamp of payload
    MS_U32  u32Timestamp;
} VDEC_DecCmd;

/// Display Command
typedef struct
{
    /// ID (high 4-bytes)
    MS_U32  u32ID_H;
    /// ID (low 4-bytes)
    MS_U32  u32ID_L;
    /// action of command
    VDEC_DispCmdAction  eAction;
} VDEC_DispCmd;

typedef struct
{
    MS_BOOL bEnable;
    MS_U32  u32Delay;
    MS_U16  u16Tolerance;
} VDEC_AVSyncCfg;


typedef void (*VDEC_EventCb)(MS_U32 eFlag, void *param);

typedef struct
{
    MS_U32              u32Version;
    /// top, bottom or frame
    VDEC_PicStructure   u8PicStructure;
    MS_U8               u8TopFieldFirst;
    MS_U16              u16TempRef;
    MS_U32              u32Pts;
    /// address of cc data
    MS_U32              u32UserDataBuf;
    /// size of cc data
    MS_U32              u32UserDataSize;
} VDEC_CC_Info;

///CC input parameters for mstar proprietary CC library
typedef struct
{
    MS_U32 u32Ver;      ///version of this structure
    MS_U32 u32Val;
} VDEC_CC_InputPara;

typedef struct
{
    MS_U32      u32OutputFrameRate; ///< output frame rate, unit:vsync count
    MS_U8       u8Interlace;        ///< output scan:0:progress, 1:interlace
} VDEC_FRC_OutputParam;

typedef struct
{
    MS_U8   u8Frm_packing_arr_cnl_flag;
    MS_U8   u8Frm_packing_arr_type;
    MS_U8   u8content_interpretation_type;
    MS_U8   u1Quincunx_sampling_flag;

    MS_U8   u1Spatial_flipping_flag;
    MS_U8   u1Frame0_flipping_flag;
    MS_U8   u1Field_views_flag;
    MS_U8   u1Current_frame_is_frame0_flag;

    MS_U8   u1Frame0_self_contained_flag;
    MS_U8   u1Frame1_self_contained_flag;
    MS_U8   u4Frame0_grid_position_x;
    MS_U8   u4Frame0_grid_position_y;

    MS_U8   u4Frame1_grid_position_x;
    MS_U8   u4Frame1_grid_position_y;
    MS_U8   u8Reserved01;
    MS_U8   u8Reserved02;
}VDEC_Frame_packing_SEI;

typedef enum
{
    E_VDEC_TRICKPLAY2X_COUNTVSYNC = 0,
    E_VDEC_TRICKPLAY2X_AVSYNC
}VDEC_TrickPlay2xMode;


typedef struct
{
    VDEC_CodecType eCodecType;
    MS_PHY  u32DataAddr;
    MS_U32  u32MemUsageSize;
    MS_U16  u16DataSize;
} VDEC_FbMemUsage_Param;

typedef struct
{
    MS_U8   u8RepeatPeriod;
    MS_U8   u8DropPeriod;
}VDEC_Slow_Sync;

/// Preget Capability Version
#define VDEC_CAP_PREGET_CAPABILITY_SUPPORT_VERSION_MAX  1
/// Verion Control Structure
typedef struct DLL_PACKED
{
    MS_U32 u32version;
    MS_U32 u32size;
} VDEC_VerCtl;
/// Preget Capability Parameter
typedef struct DLL_PACKED
{
    /// in: version control
    VDEC_VerCtl stVerCtl;
    /// in: capability string
    MS_U8 u8CapName[64];
    /// out: capability version
    MS_U8 u8CapVersion;
    /// reserved
    MS_U8 u8Reserved[127];
} VDEC_PregetCapability;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetLibVer(const MSIF_Version **ppVersion);
VDEC_Result MApi_VDEC_GetInfo(const VDEC_Info **ppInfo);
MS_BOOL     MApi_VDEC_GetStatus(VDEC_Status *pStatus);
MS_BOOL     MApi_VDEC_CheckCaps(VDEC_CodecType eCodecType);
VDEC_Result MApi_VDEC_EnableTurboMode(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_Init(VDEC_InitParam *pInitParam);
VDEC_Result MApi_VDEC_Init_EX(void *pInfo, MS_U32 u32Size);
VDEC_Result MApi_VDEC_Rst(void);
VDEC_Result MApi_VDEC_Exit(void);
VDEC_Result MApi_VDEC_CheckDispInfoRdy(void);
VDEC_Result MApi_VDEC_SetFrcMode(VDEC_FrcMode eFrcMode);
VDEC_Result MApi_VDEC_SetDynScalingParams( MS_PHY u32Addr , MS_U32 u32Size);
VDEC_Result MApi_VDEC_SetDbgLevel(VDEC_DbgLevel eDbgLevel);


VDEC_Result MApi_VDEC_Play(void);
VDEC_Result MApi_VDEC_Pause(void);
VDEC_Result MApi_VDEC_Resume(void);
VDEC_Result MApi_VDEC_StepDisp(void);
VDEC_Result MApi_VDEC_IsStepDispDone(void);
VDEC_Result MApi_VDEC_StepDecode(void);
VDEC_Result MApi_VDEC_IsStepDecodeDone(void);
VDEC_Result MApi_VDEC_SetTrickMode(VDEC_TrickDec eTrickDec);
VDEC_Result MApi_VDEC_SetBlockDisplay(MS_BOOL bEnable);

VDEC_Result MApi_VDEC_EnableESBuffMalloc(MS_BOOL bEnable);
MS_U32 MApi_VDEC_GetESBuffVacancy(void* pData);
VDEC_Result MApi_VDEC_GetESBuff(MS_U32 u32ReqSize, MS_U32* u32AvailSize, MS_PHY* u32Addr);

/*
VDEC_Result MApi_VDEC_PushQueue(MS_U32 u32StAddr,
                                MS_U32 u32Size,
                                MS_U32 u32Timestamp,
                                MS_U32 u32ID_H,
                                MS_U32 u32ID_L);
MS_U32      MApi_VDEC_GetQueueVacancy(void);
VDEC_Result MApi_VDEC_FlushQueue(void);
*/

VDEC_Result MApi_VDEC_PushDecQ(VDEC_DecCmd *pCmd);
VDEC_Result MApi_VDEC_PushDispQ(VDEC_DispCmd *pCmd);
VDEC_Result MApi_VDEC_Flush(VDEC_FreezePicSelect eFreezePic);
VDEC_Result MApi_VDEC_EnableLastFrameShow(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_SetSpeed(VDEC_SpeedType eSpeedType, VDEC_DispSpeed eSpeed);
VDEC_Result MApi_VDEC_SetFreezeDisp(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_SetBlueScreen(MS_BOOL bOn);
VDEC_Result MApi_VDEC_ResetPTS(MS_U32 u32PtsBase);
VDEC_Result MApi_VDEC_AVSyncOn(MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);
VDEC_Result MApi_VDEC_SetAVSyncFreerunThreshold(MS_U32 u32Threshold );

VDEC_Result MApi_VDEC_GetDispInfo(VDEC_DispInfo *pDispinfo);
VDEC_Result MApi_VDEC_IsAVSyncOn(void);
VDEC_Result MApi_VDEC_IsWithValidStream(void);
VDEC_Result MApi_VDEC_IsDispFinish(void);
VDEC_Result MApi_VDEC_IsFrameRdy(void);
VDEC_Result MApi_VDEC_IsIFrameFound(void);
VDEC_Result MApi_VDEC_IsSeqChg(void);
VDEC_Result MApi_VDEC_IsReachSync(void);
VDEC_Result MApi_VDEC_IsFreerun(void);
VDEC_Result MApi_VDEC_IsStartSync(void);
VDEC_Result MApi_VDEC_IsWithLowDelay(void);
VDEC_Result MApi_VDEC_IsAllBufferEmpty(void);
VDEC_Result MApi_VDEC_GetExtDispInfo(VDEC_ExtDispInfo *pExtDispinfo);
VDEC_Result MApi_VDEC_GetDecFrameInfo(VDEC_FrameInfo *pFrmInfo);
VDEC_Result MApi_VDEC_GetDispFrameInfo(VDEC_FrameInfo *pFrmInfo);
VDEC_Result MApi_VDEC_GetNextDispFrame(VDEC_DispFrame** ppDispFrm);
VDEC_Result MApi_VDEC_DisplayFrame(VDEC_DispFrame *pDispFrm);
VDEC_Result MApi_VDEC_ReleaseFrame(VDEC_DispFrame *pDispFrm);

VDEC_Result MApi_VDEC_GetDecTimeCode(VDEC_TimeCode* pTimeCode);
VDEC_Result MApi_VDEC_GetDispTimeCode(VDEC_TimeCode* pTimeCode);
VDEC_Result MApi_VDEC_SetEvent(MS_U32 u32EnableEvent, VDEC_EventCb pfn, void* param);
VDEC_Result MApi_VDEC_UnsetEvent(MS_U32 u32EnableEvent);
VDEC_Result MApi_VDEC_SetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent, VDEC_EventCb pfn, void* param);
VDEC_Result MApi_VDEC_UnsetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent);
VDEC_Result MApi_VDEC_GetEventInfo(MS_U32* u32EventFlag);
VDEC_Result MApi_VDEC_GetPatchPattern(VDEC_PatchPattern ePatternType, MS_U32 *pu32Staddr, MS_U32 *pu32Len);
VDEC_Result MApi_VDEC_FireDecCmd(void);
VDEC_Result MApi_VDEC_SeekToPTS(MS_U32 u32PTS);
VDEC_Result MApi_VDEC_SkipToPTS(MS_U32 u32PTS);

// Advanced Settings / Mode
VDEC_Result MApi_VDEC_DisableDeblocking(MS_BOOL bDisable);
VDEC_Result MApi_VDEC_DisableQuarterPixel(MS_BOOL bDisable);
VDEC_Result MApi_VDEC_SetAutoRmLstZeroByte(MS_BOOL bOn);
VDEC_Result MApi_VDEC_SetBalanceBW(MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef);

MS_U8       MApi_VDEC_GetActiveFormat(void);
MS_U8       MApi_VDEC_GetColourPrimaries(void);
MS_U8       MApi_VDEC_GetGOPCnt(void);
MS_U32      MApi_VDEC_GetESWritePtr(void);
MS_U32      MApi_VDEC_GetESReadPtr(void);
MS_U32      MApi_VDEC_GetPTS(void);
MS_U32      MApi_VDEC_GetNextPTS(void);
MS_U32      MApi_VDEC_GetErrCode(void);
MS_U32      MApi_VDEC_GetErrCnt(void);
MS_U32      MApi_VDEC_GetBitsRate(void);
MS_U32      MApi_VDEC_GetFrameCnt(void);
MS_U32      MApi_VDEC_GetSkipCnt(void);
MS_U32      MApi_VDEC_GetDropCnt(void);
MS_U32      MApi_VDEC_GetDecQVacancy(void);
MS_U32      MApi_VDEC_GetDispQVacancy(void);
MS_BOOL     MApi_VDEC_Is32PullDown(void);
VDEC_Result MApi_VDEC_IsAlive(void);
MS_S64      MApi_VDEC_GetVideoPtsStcDelta(void);
MS_S64      MApi_VDEC_GetVideoFirstPts(void);
MS_BOOL     MApi_VDEC_IsCCAvailable(void);
VDEC_Result MApi_VDEC_GetCCInfo(void *pInfo, MS_U32 u32Size);

VDEC_TrickDec MApi_VDEC_GetTrickMode(void);
VDEC_CodecType MApi_VDEC_GetActiveCodecType(void);

VDEC_Result MDrv_VDEC_GenPattern(VDEC_PatternType ePatternType ,  MS_PHY u32Addr , MS_U32* u32Size);
VDEC_Result MApi_VDEC_GenPattern(VDEC_PatternType ePatternType, MS_PHY u32Addr, MS_U32 *pu32Size);
MS_U32 MApi_VDEC_GetPatternLeastLength(VDEC_PatternType ePatternType );

VDEC_Result MApi_VDEC_MHEG_DecodeIFrame(MS_PHY u32FrameBufAddr,
                                        MS_PHY u32SrcSt,
                                        MS_PHY u32SrcEnd);
VDEC_Result MApi_VDEC_MHEG_IsIFrameDecoding(void);
VDEC_Result MApi_VDEC_MHEG_RstIFrameDec(void);

//ATSC Closed Caption control
VDEC_Result MApi_VDEC_CC_StartParsing(VDEC_CCCfg *pCCParam);
VDEC_Result MApi_VDEC_CC_StopParsing(void);
VDEC_Result MApi_VDEC_CC_GetWritePtr(MS_U32 *pWrite);
VDEC_Result MApi_VDEC_CC_GetReadPtr(MS_U32 *pRead);
VDEC_Result MApi_VDEC_CC_UpdateReadPtr(MS_U32 u32EachPacketSize);
VDEC_Result MApi_VDEC_CC_GetIsOverflow(MS_BOOL *pbOverflow);

VDEC_Result MApi_VDEC_CC_Init(MS_U32 *pIn);
VDEC_Result MApi_VDEC_CC_SetCfg(VDEC_CCFormat eFmt, VDEC_CCType eType, MS_U32 *pIn);
VDEC_Result MApi_VDEC_CC_SetBuffStartAdd(VDEC_CCFormat eFmt, MS_U32 *pIn);
VDEC_Result MApi_VDEC_CC_UpdateWriteAdd(VDEC_CCFormat eFmt, MS_U32 *pIn);
VDEC_Result MApi_VDEC_CC_UpdateReadAdd(VDEC_CCFormat eFmt, MS_U32 *pIn);
VDEC_Result MApi_VDEC_CC_DisableParsing(VDEC_CCFormat eFmt);
VDEC_Result MApi_VDEC_CC_GetIsRstDone(VDEC_CCFormat eFmt);
VDEC_Result MApi_VDEC_CC_GetIsBuffOverflow(VDEC_CCFormat eFmt);
MS_PHY MApi_VDEC_CC_GetWriteAdd(VDEC_CCFormat eFmt);
MS_PHY MApi_VDEC_CC_GetReadAdd(VDEC_CCFormat eFmt);

VDEC_Result MApi_VDEC_DbgCmd(MS_U32 u8Cmd, MS_U32 u32CmdArg);
VDEC_Result MApi_VDEC_DbgSetData(MS_VIRT u32Addr, MS_U32 u32Data);
VDEC_Result MApi_VDEC_DbgGetData(MS_VIRT u32Addr, MS_U32 *u32Data);

VDEC_Result MApi_VDEC_SetControl(VDEC_User_Cmd cmd_id, MS_VIRT param);
#ifdef NCI_ENABLE
VDEC_Result MApi_VDEC_PreSetControl(VDEC_User_Cmd cmd_id, MS_VIRT param);
#else
VDEC_Result MApi_VDEC_PreSetControl(VDEC_User_Cmd cmd_id, MS_U32 param);
#endif

MS_U32      MApi_VDEC_GetFwVersion(VDEC_FwType eFwType);

MS_U32 MApi_VDEC_GetDispCnt(void);

VDEC_Result MApi_VDEC_GetControl(VDEC_User_Cmd cmd_id, MS_U32 *param);
VDEC_Result MApi_VDEC_FlushPTSBuf(void);

//System preseting function, it should be called when system init.
VDEC_Result MApi_VDEC_SystemPreSetControl(VDEC_User_Cmd cmd_id, void *pParam);

VDEC_Result MApi_VDEC_PregetCapability(void *pParam);


VDEC_Result MApi_VDEC_V2_SetPowerState(EN_POWER_MODE u16PowerState);

VDEC_Result MApi_VDEC_V2_GetLibVer(const MSIF_Version **ppVersion);
VDEC_Result MApi_VDEC_V2_GetInfo(const VDEC_Info **ppInfo);
MS_BOOL MApi_VDEC_V2_GetStatus(VDEC_Status *pStatus);
MS_BOOL MApi_VDEC_V2_CheckCaps(VDEC_CodecType eCodecType);
VDEC_Result MApi_VDEC_V2_EnableTurboMode(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_V2_Init(VDEC_InitParam *pInitParam);
VDEC_Result MApi_VDEC_V2_Init_EX(void *pInfo, MS_U32 u32Size);
VDEC_Result MApi_VDEC_V2_Rst(void);
VDEC_Result MApi_VDEC_V2_Exit(MS_BOOL);
VDEC_Result MApi_VDEC_V2_CheckDispInfoRdy(void);
VDEC_Result MApi_VDEC_V2_SetFrcMode(VDEC_FrcMode eFrcMode);
VDEC_Result MApi_VDEC_V2_SetDynScalingParams( MS_PHY u32Addr , MS_U32 u32Size);
VDEC_Result MApi_VDEC_V2_Play(void);
VDEC_Result MApi_VDEC_V2_Pause(void);
VDEC_Result MApi_VDEC_V2_Resume(void);
VDEC_Result MApi_VDEC_V2_StepDisp(void);
VDEC_Result MApi_VDEC_V2_IsStepDispDone(void);
VDEC_Result MApi_VDEC_V2_SeekToPTS(MS_U32 u32PTS);
VDEC_Result MApi_VDEC_V2_SkipToPTS(MS_U32 u32PTS);
VDEC_Result MApi_VDEC_V2_StepDecode(void);
VDEC_Result MApi_VDEC_V2_IsStepDecodeDone(void);
VDEC_Result MApi_VDEC_V2_SetTrickMode(VDEC_TrickDec eTrickDec);
VDEC_Result MApi_VDEC_V2_PushDecQ(VDEC_DecCmd *pCmd);
VDEC_Result MApi_VDEC_V2_FireDecCmd(void);
MS_U32 MApi_VDEC_V2_GetDecQVacancy(void);
MS_BOOL MApi_VDEC_V2_IsCCAvailable(void);
VDEC_Result MApi_VDEC_V2_GetCCInfo(void *pInfo, MS_U32 u32Size);
VDEC_Result MApi_VDEC_V2_Flush(VDEC_FreezePicSelect eFreezePic);
MS_U32 MApi_VDEC_V2_GetESWritePtr(void);
MS_U32 MApi_VDEC_V2_GetESReadPtr(void);
VDEC_Result MApi_VDEC_V2_EnableLastFrameShow(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_V2_IsDispFinish(void);
VDEC_Result MApi_VDEC_V2_SetSpeed(VDEC_SpeedType eSpeedType, VDEC_DispSpeed eSpeed);
VDEC_Result MApi_VDEC_V2_IsFrameRdy(void);
VDEC_Result MApi_VDEC_V2_SetFreezeDisp(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_V2_SetBlueScreen(MS_BOOL bOn);
VDEC_Result MApi_VDEC_V2_ResetPTS(MS_U32 u32PtsBase);
VDEC_Result MApi_VDEC_V2_AVSyncOn(MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);
VDEC_Result MApi_VDEC_V2_SetAVSyncFreerunThreshold(MS_U32 u32Threshold );
VDEC_Result MApi_VDEC_V2_IsAVSyncOn(void);
MS_U32 MApi_VDEC_V2_GetPTS(void);
MS_U32 MApi_VDEC_V2_GetNextPTS(void);
VDEC_Result MApi_VDEC_V2_IsStartSync(void);
VDEC_Result MApi_VDEC_V2_IsReachSync(void);
VDEC_Result MApi_VDEC_V2_IsFreerun(void);
VDEC_Result MApi_VDEC_V2_MHEG_DecodeIFrame(MS_PHY u32FrameBufAddr,
                                        MS_PHY u32SrcSt,
                                        MS_PHY u32SrcEnd);
VDEC_Result MApi_VDEC_V2_MHEG_IsIFrameDecoding(void);
VDEC_Result MApi_VDEC_V2_MHEG_RstIFrameDec(void);
MS_U32 MApi_VDEC_V2_GetErrCode(void);
MS_U32 MApi_VDEC_V2_GetErrCnt(void);
VDEC_CodecType MApi_VDEC_V2_GetActiveCodecType(void);
MS_U32 MApi_VDEC_V2_GetBitsRate(void);
MS_BOOL MApi_VDEC_V2_Is32PullDown(void);
VDEC_Result MApi_VDEC_V2_IsAlive(void);
MS_S64 MApi_VDEC_V2_GetVideoPtsStcDelta(void);
VDEC_Result MApi_VDEC_V2_IsWithValidStream(void);
VDEC_Result MApi_VDEC_V2_IsIFrameFound(void);
VDEC_Result MApi_VDEC_V2_IsWithLowDelay(void);
VDEC_Result MApi_VDEC_V2_IsAllBufferEmpty(void);
MS_U8 MApi_VDEC_V2_GetGOPCnt(void);
MS_U32 MApi_VDEC_V2_GetFrameCnt(void);
MS_U32 MApi_VDEC_V2_GetSkipCnt(void);
MS_U32 MApi_VDEC_V2_GetDropCnt(void);
VDEC_Result MApi_VDEC_V2_GetDispInfo(VDEC_DispInfo *pDispinfo);
VDEC_TrickDec MApi_VDEC_V2_GetTrickMode(void);
MS_U8 MApi_VDEC_V2_GetActiveFormat(void);
MS_U8 MApi_VDEC_V2_GetColourPrimaries(void);
VDEC_Result MApi_VDEC_V2_GetHWKey(MS_U8 *pu8Key);
VDEC_Result MApi_VDEC_V2_IsSeqChg(void);
VDEC_Result MApi_VDEC_V2_SetDbgLevel(VDEC_DbgLevel eDbgLevel);
VDEC_Result MApi_VDEC_V2_GetDecFrameInfo(VDEC_FrameInfo* pFrmInfo);
VDEC_Result MApi_VDEC_V2_GetDispFrameInfo(VDEC_FrameInfo *pFrmInfo);
VDEC_Result MApi_VDEC_V2_GetExtDispInfo(VDEC_ExtDispInfo *pExtDispinfo);
VDEC_Result MApi_VDEC_V2_GetDecTimeCode(VDEC_TimeCode* pTimeCode);
VDEC_Result MApi_VDEC_V2_GetDispTimeCode(VDEC_TimeCode* pTimeCode);
VDEC_Result MApi_VDEC_V2_CC_StartParsing(VDEC_CCCfg* pCCParam);
VDEC_Result MApi_VDEC_V2_CC_StopParsing(void);
VDEC_Result MApi_VDEC_V2_CC_GetWritePtr(MS_U32* pu32Write);
VDEC_Result MApi_VDEC_V2_CC_GetReadPtr(MS_U32* pu32Write);
VDEC_Result MApi_VDEC_V2_CC_UpdateReadPtr(MS_U32 u32EachPacketSize);
VDEC_Result MApi_VDEC_V2_CC_GetIsOverflow(MS_BOOL* pbOverflow);
VDEC_Result MApi_VDEC_V2_SetEvent(MS_U32 u32EnableEvent, VDEC_EventCb pfn, void* param);
VDEC_Result MApi_VDEC_V2_UnsetEvent(MS_U32 u32EnableEvent);
VDEC_Result MApi_VDEC_V2_SetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent, VDEC_EventCb pfn, void* param);
VDEC_Result MApi_VDEC_V2_UnsetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent);
VDEC_Result MApi_VDEC_V2_GetEventInfo(MS_U32* u32EventFlag);
VDEC_Result MApi_VDEC_V2_GenPattern(VDEC_PatternType ePatternType,	MS_PHY u32Addr, MS_U32 *pu32Size);
MS_U32 MApi_VDEC_V2_GetPatternLeastLength(VDEC_PatternType ePatternType );
VDEC_Result MApi_VDEC_V2_DisableDeblocking(MS_BOOL bDisable);
VDEC_Result MApi_VDEC_V2_DisableQuarterPixel(MS_BOOL bDisable);
VDEC_Result MApi_VDEC_V2_SetAutoRmLstZeroByte(MS_BOOL bOn);
VDEC_Result MApi_VDEC_V2_SetBalanceBW(MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef);
VDEC_Result MApi_VDEC_V2_DbgDumpStatus(void);
VDEC_Result MApi_VDEC_V2_SetControl(VDEC_User_Cmd cmd_id, MS_U32 param);
VDEC_Result MApi_VDEC_V2_GetControl(VDEC_User_Cmd cmd_id, MS_U32 *param);
MS_U32 MApi_VDEC_V2_GetDispCnt(void);
MS_U32 MApi_VDEC_V2_GetFwVersion(VDEC_FwType eFwType);
VDEC_Result MApi_VDEC_V2_SetBlockDisplay(MS_BOOL bEnable);
VDEC_Result MApi_VDEC_V2_EnableESBuffMalloc(MS_BOOL bEnable);
MS_U32 MApi_VDEC_V2_GetESBuffVacancy(void* pData);
VDEC_Result MApi_VDEC_V2_GetESBuff(MS_U32 u32ReqSize, MS_U32* u32AvailSize, MS_PHY* u32Addr);
VDEC_Result MApi_VDEC_V2_GetNextDispFrame(VDEC_DispFrame** ppDispFrm);
VDEC_Result MApi_VDEC_V2_DisplayFrame(VDEC_DispFrame *pDispFrm);
VDEC_Result MApi_VDEC_V2_DisplayFrame(VDEC_DispFrame *pDispFrm);
VDEC_Result MApi_VDEC_V2_FlushPTSBuf(void);
VDEC_Result MApi_VDEC_V2_CaptureFrame(MS_U32 u32FrmPriData, MS_BOOL bEnable);
VDEC_Result MApi_VDEC_V2_CC_Init(MS_U32 *pIn);
VDEC_Result MApi_VDEC_V2_CC_SetCfg(VDEC_CCFormat eFmt, VDEC_CCType eType, MS_U32 *pIn);
VDEC_Result MApi_VDEC_V2_CC_SetBuffStartAdd(VDEC_CCFormat eFmt, MS_U32 *pIn);
VDEC_Result MApi_VDEC_V2_CC_UpdateWriteAdd(VDEC_CCFormat eFmt, MS_U32 *pIn);
VDEC_Result MApi_VDEC_V2_CC_UpdateReadAdd(VDEC_CCFormat eFmt, MS_U32 *pIn);
VDEC_Result MApi_VDEC_V2_CC_DisableParsing(VDEC_CCFormat eFmt);
VDEC_Result MApi_VDEC_V2_CC_GetInfo(VDEC_CCFormat eFmt, VDEC_CCInfoCmd eCmd, MS_U32 *pOut);
VDEC_Result MApi_VDEC_V2_CC_GetIsRstDone(VDEC_CCFormat eFmt);
VDEC_Result MApi_VDEC_V2_CC_GetIsBuffOverflow(VDEC_CCFormat eFmt);
MS_PHY MApi_VDEC_V2_CC_GetWriteAdd(VDEC_CCFormat eFmt);
MS_PHY MApi_VDEC_V2_CC_GetReadAdd(VDEC_CCFormat eFmt);
VDEC_Result MApi_VDEC_V2_CC_SystemPreSetControl(VDEC_User_Cmd cmd_id, void *pParam);
VDEC_Result MApi_VDEC_V2_CC_PreSetControl(VDEC_User_Cmd cmd_id, MS_U32 param);
VDEC_Result MApi_VDEC_V2_ReleaseFrame(VDEC_DispFrame *pDispFrm);
VDEC_Result MApi_VDEC_V2_SystemPreSetControl(VDEC_User_Cmd cmd_id, void *pParam);
VDEC_Result MApi_VDEC_V2_PreSetControl(VDEC_User_Cmd cmd_id, MS_U32 param);


#ifdef __cplusplus
}
#endif

#endif // _API_VDEC_H_
