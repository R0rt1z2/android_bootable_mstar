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
#ifndef _DMS_PRIV_H_
#define _DMS_PRIV_H_
#include "UFO.h"
////////////////////////////////////////////////////////////////////////////////
/// @brief DMS control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef EN_DMS_RESULT                    (*IOCTL_DMS_INIT)                    (void*, ST_DMS_INITDATA*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETMEMTYPE)            (void*, ST_DMS_SET_MEMORY_TYPE*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_CREATEWINDOW)          (void*, ST_DMS_WINDOW*, ST_DMS_CREATE_WIN_INFO*, MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETDIGIDECINFO)       (void*, MS_U32, ST_DMS_DISPFRAMEFORMAT*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_CLEARDIGIDECINFO)    (void*, MS_U32);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_DESTORYWINDOW)        (void*, MS_U32);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETWINDOW)             (void*, MS_U32, ST_DMS_SETWIN_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETWINDOWINFO)        (void*, MS_U32, ST_DMS_WINDOW_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VIDEOFLIP)             (void*, MS_U32, ST_DMS_DISPFRAMEFORMAT*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETZORDER)             (void*, MS_U32, ST_DMS_ZORDER_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VIDEOMUTE)             (void*, MS_U32, ST_DMS_MUTE_WINDOW_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETMUTECOLOR)         (void*, MS_U32, ST_DMS_MUTE_COLOR_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VIDEOFREEZE)          (void*, MS_U32, ST_DMS_FREEZE_WINDOW_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETSTATUS)             (void*, EN_DMS_STATUS*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETCAPS)               (void*, EN_DMS_CAPABILITY*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETEXISTWIN)          (void*, MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETOUTPUTLAYER)      (void*, ST_DMS_WINDOW*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SET3DMODE)             (void*, MS_U32, ST_DMS_3D_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETCAPTUREINFO)      (void*, MS_U32, ST_DMS_SET_CAPTURE_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETCAPTUREBUF)        (void*, MS_U32, ST_DMS_CAPTURE_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_RELEASECAPTUREBUF)   (void*, MS_U32, ST_DMS_CAPTURE_INFO*);
#ifndef UFO_DMS_OLD_VERSION
typedef EN_DMS_RESULT                    (*IOCTL_DMS_DYNAMICSCALING)       (void*, MS_U32, MS_BOOL);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETWINDOWID)           (void*, ST_DMS_GET_ID_INFO*, MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VDECFRAMERELEASE)     (void*, MS_U32, ST_DMS_FRAME_RELEASE_INFO*, MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_DISPLAYINFO)          (void*, MS_U32, FUNC_DMS_DISPLAYINFO);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_REGISTERCALLBACKFUN)  (void*, MS_U32, ST_DMS_CALLBACK_STRUCT*,MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_ISSUPPORTCALLBACK)    (void*, MS_BOOL*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETVIEODELAY)          (void*, MS_U32, ST_DMS_VIDEO_DELAY_INFO*);
#endif
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SECURITYCONTROL)           (void*, EN_DMS_SECURITY_ACTION, ST_DMS_SECURITY_HANDLER*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETMETADATA)               (void*, MS_U32, ST_DMS_METADATA_INFO*);

#ifdef UFO_DMS_OLD_VERSION
typedef enum
{
    E_DMS_POOL_ID_INTERNAL_VARIABLE = 0,
    E_DMS_POOL_ID_MAX,
} E_DMS_POOL_ID;
#else
typedef enum
{
    E_DMS_POOL_ID_INTERNAL_VARIABLE = DMS_MAX_WINDOW_NUM,
    E_DMS_POOL_ID_MAX = DMS_MAX_WINDOW_NUM+1,
} E_DMS_POOL_ID;
#endif


#ifdef DMS_VERSION_DIPMVOP
typedef enum
{
    E_DMS_MVOP_3D_NO_CHANGE               = 0,
    E_DMS_MVOP_3D_3DMDOE_CHANGE           = 1,
    E_DMS_MVOP_3D_MVOPPATH_CHANGE         = 2,
} E_DMS_3DCHANGE_STATUS;
#endif

typedef struct __attribute__((packed))
{
    // DMS security enabled by window
    MS_BOOL bSecurityEnabled[DMS_MAX_WINDOW_NUM];
    MS_U32   u32PipeID[DMS_MAX_WINDOW_NUM];
} ST_DRV_DMS_SECURE;

#ifndef UFO_DMS_OLD_VERSION
typedef struct
{
    ST_DMS_WINDOW_INFO stWindowInfo;
    ST_DMS_BufferWriteReadPointer stBufferRWPointer;
    MS_U32 u32WindowFrameRate;
    MS_U32 u32DMS_ScalingCondition;
    ST_DMS_WINDOW stXCCaptWinInfo;
    MS_BOOL bStartCountFRC;
    ST_DMS_VDECFRAME_INFO stFrameBufferInfo[DMS_BUFFER_QUEUEDEPTH];
    MS_U8 u8LocalFrameRefCount[DMS_BUFFER_QUEUEDEPTH];
    MS_BOOL bWinUsed;
    MS_U32  u32ReleaseState[DMS_BUFFER_QUEUEDEPTH];
    MS_BOOL bFirstPlay;
    MS_BOOL bSetMute;
    MS_U32 u32MuteCounter;
    MS_U8 au8WinPattern[5];
    MS_BOOL bRefreshWin;
    MS_BOOL bUpdateSetWin;
    MS_U32 u32InputFrmRate;
    MS_U32 u32OutputFrmRate;
    MS_U32 u32AccumuleOutputRate;
    MS_BOOL bIsEnFRC;
    MS_BOOL bIsXCEnFBL;
    MS_BOOL bIsEnForceP;
    // 3D
    MS_BOOL bEnable3D;
    MS_U32 u32PQFrameCnt;
    MS_BOOL bXCEnableBob;

    //fod detect field order type
    MS_BOOL bFieldOrderChange;
    E_DMS_VIDEO_FIELD_ORDER_TYPE enDetectFieldOrderType;

    //SWDR Histogram (MVOP window only)
    MS_BOOL bHistogramValid;
    MS_U32 u32Histogram[DMS_BUFFER_QUEUEDEPTH][DMS_SWDRHISTOGRAM_INDEX];

    //VDEC Info
    MS_U32 u32QPMin[DMS_BUFFER_QUEUEDEPTH]; // Min of quantization parameter
    MS_U32 u32QPAvg[DMS_BUFFER_QUEUEDEPTH]; // Avg of quantization parameter
    MS_U32 u32QPMax[DMS_BUFFER_QUEUEDEPTH]; // Max of quantization parameter
    EN_DMS_FRAMETYPE enFrameType; // Frame type (I/P/B mode)
    //AVsync usage
    MS_U32 u32FlipTime[DMS_BUFFER_QUEUEDEPTH];
    MS_U32 u32DelayTime[DMS_BUFFER_QUEUEDEPTH];

    //fod, field order type for last field
    E_DMS_VIDEO_FIELD_ORDER_TYPE enPreFieldOrderType;
    //FDmask status
    MS_BOOL bFDMaskEnable;
    E_DMS_INPUTSRC_TYPE enInputSrcType;

    //first setwindow flag
    MS_BOOL bFirstSetwindow;
    //ds info
    ST_DMS_DS_INFO stDSInfo;

    //play a video flag
    MS_BOOL bPlayFlag;

    //u32RWDiff = Scaler read/write diff + FRC read/write diff
    MS_U32 u32RWDiff;
    //one field mode flag
    //1.fw/fb case
    //2.when video is paused, if zoom mode changed, need delay to enabe FD mask, and flip last field.
    MS_BOOL bOneFieldMode;

    //video aspect ratio
    MS_U32 u32AspectWidth;
    MS_U32 u32AspectHeight;

    //video delay time
    MS_U32 au32VideoFlipTime[DMS_BUFFER_QUEUEDEPTH];
    MS_BOOL abVideoFlip[DMS_BUFFER_QUEUEDEPTH];
    MS_U32 u32VideoDelayTime;
    MS_U32 u32PipeID[DMS_MAX_WINDOW_NUM];
} ST_DRV_DMS;
//
//#ifdef DMS_VERSION_DIPGOP
//typedef struct
//{
//    DMS_VDECFRAME_INFO _stXC_DIPDIInfo/*[DMS_MAX_WINDOW_NUM]*/[DMS_DIP_QUEUEDEPTH];
//    DMS_DIPWriteReadPointer _stXC_DIPWriteReadPointerInterlace[DMS_MAX_WINDOW_NUM];
//    MS_U8 _u8DMS_DiSetEventBufferID;
//    MS_U8 _u8DMS_DiReleaseVDECFrameBufferID;
//    MS_U8 u8DIPReadytoCaptureIDInterlace[DMS_MAX_WINDOW_NUM];
//    MS_U8 _stXC_LocalFrameRefCountInterlace/*[DMS_MAX_WINDOW_NUM]*/[DMS_DIP_QUEUEDEPTH];
//    MS_U8 u8DIRingBufferID;
//    MS_U8 _bDiNowShowRingBufID;
//    MS_U8 _bDiReleaseShowRingBufID;
//    MS_BOOL _bSetwinToDiTask_DoubleBuffer;
//
//    MS_PHY u32DIPDIAddress_pa;
//    MS_BOOL bFlipToDiTask;               //For set event can not use, temp solution for push di task
//} ST_DRV_DMS_DI;
//
//typedef struct
//{
//    DMS_DIPCAPTUREWIN_INFO _stXC_DIPCaptureWinInfo/*[DMS_MAX_WINDOW_NUM]*/;
//    DMS_DIPWriteReadPointer _stXC_DIPCapWin_WriteReadPointer/*[DMS_MAX_WINDOW_NUM]*/;
//    DMS_DIPCaptureWin_WritePointer _stXC_DIPCaptureWin_RingBufferPointer/*[DMS_MAX_WINDOW_NUM]*/;
//    MS_U8 _stXC_CaptureRingBufRefCount[DMS_MAX_WINDOW_NUM][DMS_CAPTUREWIN_RING_BUFFER];
//    MS_U8 _stXC_LocalFrameRefCountCaptureMode/*[DMS_MAX_WINDOW_NUM]*/[DMS_DIP_QUEUEDEPTH];
//    MS_U8 u8CapturedWinNum;
//    MS_U8 u8frameRepeatCnt/*[DMS_MAX_WINDOW_NUM]*/[DMS_DIP_QUEUEDEPTH];
//    MS_U32 bIsFrameValid/*[DMS_MAX_WINDOW_NUM]*/[DMS_DIP_QUEUEDEPTH];
//    MS_U32 bIsRingBufAccessed[DMS_CAPTUREWIN_RING_BUFFER];
//#if defined(DMS_DIP_Support_IMI) && DMS_DIP_Support_IMI
//    DMS_DIPCAPTURE_IMI_INFO _stXC_IMICaptureInfo[DMS_MAX_WINDOW_NUM];
//#endif
//} ST_DRV_DMS_CAPTUREMODE;
//
//#endif

typedef struct __attribute__((__packed__))
{
    // flow control related
//    MS_BOOL bResourceRegistered;
//    ST_HAL_DMS stHalDMS;
    ST_DRV_DMS stDrvDMS;
    ST_DRV_DMS_SECURE stDrvDMSSecure;
//    #ifdef DMS_VERSION_DIPGOP
//    ST_DRV_DMS_DI stDrvDMSDi;
//    ST_DRV_DMS_CAPTUREMODE stDrvDMSCapMode;
//    #endif
} DMS_RESOURCE_PRIVATE;

typedef struct __attribute__((__packed__))
{
    MS_BOOL bDummy;
    MS_U8 u8PathPattern[E_DMS_INPUTSRC_NUM][5];
    MS_BOOL bDrvInit;
    MS_U8 u8CreateWindowNum;
    ST_DMS_WINDOW stOutputLayer;
    MS_U16 u16DebugLevel;
    MS_BOOL bForceOffPQDS;
    MS_U8 u8CfdVersion;
} DMS_COMMON_RESOURCE_PRIVATE;



//UFO_DMS_OLD_VERSION
#else
#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
    MS_PHY phyDSAddr;
    MS_U8 u8DSIndex;
    DMS_DS_SWDB stDMS_DSSWDB;
#ifdef DMS_VERSION_DIPMVOP
    MS_U8 u8BypassMode;

    // GOP seamless flow need get mute status
    MS_BOOL bMute;
#endif
} ST_HAL_DMS;

#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
#ifdef DMS_VERSION_DIPGOP
    DMS_DIPMEM_INFO _stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MAX];
    DMS_DIPWIN_INFO _stXC_DIPWinInfo[DMS_MAX_WINDOW_NUM];
    DMS_DIPWIN_INFO _stXC_DIPWinInfo_MVOPDoubleBuffer;
    ST_DMS_WINDOW _stXC_PreDispWinInfo[DMS_MAX_WINDOW_NUM];
    DMS_DISP_BUFFER_FLAG _stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_MAX][DMS_MAX_WINDOW_NUM];

    DMS_DIPWriteReadPointer _stXC_DIPWriteReadPointer[DMS_MAX_WINDOW_NUM];
    MS_U16 u16WritePointerByDi;
    MS_U32 u32MaxOutputFrameRate;
    MS_U32 u32VsyncCount;
    MS_U32 u32OutputFrameRate;
    MS_U32 u32GOPDispCount;
    MS_U8 u8DIPReadytoCaptureID[DMS_MAX_WINDOW_NUM];
    MS_U8 u8DIP_Window[E_DMS_DIP_MAX];
    //Information about processing deinterlace, especially 3:2 pull down format
    E_DMS_VIDEO_FIELD_ORDER_TYPE enDMSBeginFieldOrder[DMS_MAX_WINDOW_NUM];

    MS_PHY u32DIPAddress_pa[E_DMS_DIP_CLIENT_MAX];
    MS_PHY u32GEAddress_pa;
    MS_PHY u32GarbageBufAddr_pa;

    MS_U8 u8GOPGWinNum;
    MS_BOOL bIsGOPInit;
    MS_BOOL bIsGEInit;
    MS_BOOL bDIPWriteFirstAddr;
    MS_BOOL bDIPMVOPWriteFirstAddr;
    MS_U32 u32WindowFrameRate[DMS_MAX_WINDOW_NUM];
#ifdef DMS_VERSION_DIPMVOP
    MS_U8 u8UseWindowCount;
#endif
    MS_U8 u8EnableWindowCount;
    MS_U8 u8UseDIPWindowCount;
    MS_BOOL bNeedGEstretch;

    E_DMS_MVOP_FLOW_CONTROL eDMS_MVOPFlowCtrl;
    MS_BOOL bDMS_MVOPFlowFromDi;
    E_DMS_MVOP_FLOW_CONTROL eDMS_PreMVOPFlowCtrl;
    MS_U8 gDMS_MVOPShowBufferID;
    MS_U8 gDMS_MVOPShowingBufferID;
    MS_U8 gDMS_MVOPRemoveBufferID;
    MS_BOOL bMVOPMarkRemoveBuffer;
    MS_BOOL bIsWindowMute[DMS_MAX_WINDOW_NUM];

    MS_U32 gDMS_ScalingCondition;
    MS_U32 gu8DMS_SetXC_Status[DMS_MAX_WINDOW_NUM];
    MS_U8 gu8DMS_Seq_Change_ID;
    ST_DMS_WINDOW _stXC_XCDispWinInfo;
    ST_DMS_WINDOW _stXC_XCCropWinInfo;
    ST_DMS_WINDOW _stXC_XCDispWinInfo_DoubleBuffer;
    ST_DMS_WINDOW _stXC_XCCropWinInfo_DoubleBuffer;
    ST_DMS_WINDOW _stXC_XCCapWinInfo_DoubleBuffer;
    E_DMS_VIDEO_TILE_MODE eVideoTile;

    // For DS regen crop
    ST_DMS_WINDOW _stXC_XCCropWinInfo_raw;
    MS_U16 u16FirstSrcWidth;
    MS_U16 u16FirstSrcHeight;
    MS_U16 u16FirstSrcCropLeft;
    MS_U16 u16FirstSrcCropRight;
    MS_U16 u16FirstSrcCropTop;
    MS_U16 u16FirstSrcCropBottom;

    MS_PHY u32MVOPFreezeAddress_pa;

#ifdef DMS_VERSION_DIPMVOP
    MS_BOOL bDMS_Freeze_DoubleBuffer;
    MS_BOOL bDMS_Freezing;
    MS_U8 u8DMS_FreezingBufferID;
    DMS_VDECFRAME_INFO _stXC_FreezePushInfo;
    MS_U32 u32ForceSwitchBufferFlag;
#else
    MS_BOOL bDMS_Freeze_DoubleBuffer[DMS_MAX_WINDOW_NUM];
    MS_BOOL bDMS_Freezing[DMS_MAX_WINDOW_NUM];
#endif

    MS_U32 u32OutputRate;
    MS_U32 bStartCountFRC[DMS_MAX_WINDOW_NUM];
    MS_U32 u32Accumule[DMS_MAX_WINDOW_NUM];
    MS_BOOL bHistogramValid;
    MS_U32 u32Histogram[DMS_DIP_QUEUEDEPTH][DMS_SWDRHISTOGRAM_INDEX];
#endif
    //ARC use
    EN_DMS_ASPECT_RATIO enARC;
    MS_U32 u32ARCWidth;
    MS_U32 u32ARCHeight;

    // 3D
    MS_BOOL bEnable3D;
#ifdef DMS_VERSION_DIPMVOP
    MS_BOOL b3DClearDIPWin;
    E_DMS_3DCHANGE_STATUS e3DChangeStatus;
#endif
    //CMA
    MS_BOOL bUsingCMA[E_DMS_MEMORY_TYPE_MAX];
    MS_U32 u32CMAHandleID[E_DMS_MEMORY_TYPE_MAX];

    // Mute
#ifdef DMS_VERSION_DIPMVOP
    MS_U8 u8DMS_Mute_DelayFrameCount;
    MS_BOOL bDMS_Mute[DMS_MAX_WINDOW_NUM];
    MS_BOOL bDMS_Mute_DoubleBuffer[DMS_MAX_WINDOW_NUM];
    MS_BOOL bDMS_ClearInfo_DoubleBuffer[DMS_MAX_WINDOW_NUM];

    MS_U32 u32CodecType[DMS_MAX_WINDOW_NUM];
    MS_U8 u83DMode[DMS_MAX_WINDOW_NUM];
#else
    MS_BOOL bDMS_Mute;
    MS_BOOL bDMS_Mute_DoubleBuffer;
#endif

    MS_U32 u32fbID;
#ifdef DMS_VERSION_DIPMVOP
    MS_BOOL bDMS_UHDFrameBuffer_ENABLE;
    MS_U32 u32GOP_FrameBuffer_Width;
    MS_U32 u32GOP_FrameBuffer_Height;

    MS_BOOL bEnableHDR;
    DMS_DISP_BUFFER_FLAG _stXC_DispBuf_Flag_DoubleBuffer[E_DMS_DIP_MULTIVIEW_BUF_ID_MAX][DMS_MAX_WINDOW_NUM];
#endif

    DMS_VDECFRAME_INFO _stXC_DIPPushInfo[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U8 _stXC_LocalFrameRefCount[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U32 WindowZOrder[DMS_MAX_WINDOW_NUM];
    MS_U8 WindowLayer[DMS_MAX_WINDOW_NUM];
    ST_DMS_WINDOW stOutputLayer;
#ifdef DMS_VERSION_DIPMVOP
    MS_BOOL bIsSDRSource;
#endif

#ifdef DMS_VERSION_VSYNCBRDIGE
    // vsyncbridge
    MS_PHY phyVsyncbridgeAddr[DMS_MAX_WINDOW_NUM];
    MS_PHY phyVsyncbridgeExtAddr[DMS_MAX_WINDOW_NUM];
    MS_U8 u8MirrorMode;
    MS_BOOL bWinChanged[DMS_MAX_WINDOW_NUM];
    MS_BOOL bWinUsed[DMS_MAX_WINDOW_NUM];
    MS_BOOL bWindowInfoReady[DMS_MAX_WINDOW_NUM];
    ST_DMS_OUTPUT_WIN_INFO stOutputWinInfo[DMS_MAX_WINDOW_NUM];
    ST_DMS_WINDOW stDisplayWin[DMS_MAX_WINDOW_NUM];
#endif
#ifdef DMS_VERSION_DIPGOP
    // For mvop release
    MS_U32  u32ReleaseState[DMS_DIP_QUEUEDEPTH];
    MS_BOOL bFirstPlay;
#ifdef DMS_VERSION_DIPMVOP
    MS_BOOL bIsGWINNeedStretchUp;
    MS_BOOL bIs3rdWindowSet;
    MS_BOOL bIs3rdWindowSet_DoubleBuffer;
    MS_U8 u8UseWindowCount_DoubleBuffer;

    MS_BOOL bNeed2StepScaling;

    // GOP seamless
    MS_BOOL bGOP_Freezing;
    MS_S32 s32GOP_Seamless_Event_id;

    MS_U8 u8OffsetCnt[DMS_MAX_WINDOW_NUM];
#endif
    MS_BOOL bDisableBlackScreen;
    MS_BOOL bSetMute;
    MS_U32 u32MuteCounter;
    MS_BOOL bXCEnableBob;

    MS_BOOL bIsFlipOccupied[DMS_MAX_WINDOW_NUM];
#endif

#ifdef DMS_VERSION_DIPGOP
    DMS_DS_INFO DSInfo;
#endif
    MS_U8 u8ForceRead_Delay;

#ifndef DMS_MVOP_DYNAMIC_FPS
    //Store the pre frame rate
    MS_U32 u32DynamicFPSPreFrameRate;
    //Store the MMFF2x pre frame rate
    MS_U32 u32MMFF2xPreFrameRate;
    //Decide store queue or not while in the MMFF2x case
    MS_BOOL bMMFF2xStoreQueueFlag;
    //the MMFF2x case start
    MS_BOOL bMMFF2xStartFlag;
#endif
    ST_DMS_WINDOW stOriginDispWindow;
#if (DMS_PMODE_MVOP_GEN_TIMING == 0)
    MS_BOOL bInterlace;
#endif
    MS_U32 u32PipeID[DMS_MAX_WINDOW_NUM];
} ST_DRV_DMS;

#ifdef DMS_VERSION_DIPGOP
#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
    DMS_VDECFRAME_INFO _stXC_DIPDIInfo[DMS_MAX_WINDOW_NUM][DMS_DIPDI_QUEUEDEPTH];
    DMS_DIPWriteReadPointer _stXC_DIPWriteReadPointerInterlace[DMS_MAX_WINDOW_NUM];
    MS_U8 _u8DMS_DiSetEventBufferID[DMS_MAX_WINDOW_NUM];
    MS_U8 _u8DMS_DiReleaseVDECFrameBufferID;
    MS_U8 _u8DMS_DiSeqChangeBufferID[DMS_MAX_WINDOW_NUM];

    MS_U8 u8DIPReadytoCaptureIDInterlace[DMS_MAX_WINDOW_NUM];
    MS_U8 _stXC_LocalFrameRefCountInterlace[DMS_MAX_WINDOW_NUM][DMS_DIPDI_QUEUEDEPTH];
    MS_U8 u8DIRingBufferID[DMS_MAX_WINDOW_NUM];
    MS_U8 _bDiNowShowRingBufID[DMS_MAX_WINDOW_NUM];
    MS_U8 _bDiReleaseShowRingBufID[DMS_MAX_WINDOW_NUM];
    MS_BOOL _bSetwinToDiTask_DoubleBuffer[DMS_MAX_WINDOW_NUM];

    MS_PHY u32DIPDIAddress_pa[DMS_MAX_WINDOW_NUM];
    MS_BOOL bFlipToDiTask; //For set event can not use, temp solution for push di task
    MS_BOOL bDiRingBufUpdated[DMS_MAX_WINDOW_NUM];
    MS_U8 u8BobCnt[DMS_MAX_WINDOW_NUM];
    MS_U8 _bDiNowShowingRingBufID;
#ifdef DMS_VERSION_DIPMVOP
    MS_U8 u8HistogramRefQueueID[DMS_MAX_WINDOW_NUM][DMS_DI_RING_BUFFER];
    MS_S32 XC_Di_Push_Event_id;
    MS_U8 u8FODChkCnt[DMS_MAX_WINDOW_NUM];
    MS_BOOL bFODInverse[DMS_MAX_WINDOW_NUM];
    DMS_DIPDI_EVENT_QUEUE stDIPDiEventQueue;
#endif
} ST_DRV_DMS_DI;

#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
    DMS_DIPCAPTUREWIN_INFO stDIPCapWinInfo[DMS_MAX_WINDOW_NUM];
    DMS_DIPWriteReadPointer stDIPCap_WriteReadPtr[DMS_MAX_WINDOW_NUM];
    DMS_DIPCaptureWin_WritePointer stDIPCap_RingBufPtr[DMS_MAX_WINDOW_NUM];
    MS_U8 stDIPCap_RingBufRefCount[DMS_MAX_WINDOW_NUM][DMS_CAPTUREWIN_RING_BUFFER];
    MS_U8 stDIPCap_LocalFrameRefCount[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U8 u8FrameRepeatCnt[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U32 bIsFrameValid[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U32 u32RingBufAccessCnt[DMS_MAX_WINDOW_NUM][DMS_CAPTUREWIN_RING_BUFFER];
    DMS_DIPCAPTURE_IMI_INFO stDIPCap_IMICaptureInfo[DMS_MAX_WINDOW_NUM];

#ifdef DMS_VERSION_DIPMVOP
    MS_U32 bIMICaptureRet[DMS_MAX_WINDOW_NUM];
#endif
    ST_DMS_WINDOW stPaddingPosition[DMS_MAX_WINDOW_NUM][E_DMS_DIP_MULTIVIEW_BUF_ID_MAX];
} ST_DRV_DMS_CAPTUREMODE;


#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
    // DMS debug log level
    MS_U32 u32DebugLevel;
    // DMS debug mode
    MS_U32 u32DebugMode;
#ifdef DMS_VERSION_DIPMVOP
    MS_U32 u32DisableFOD;
    MS_U32 u32DisableCGain;
    MS_U32 u32DisableFilm;
    MS_U32 u32GOPFrameBufferLevel;
    MS_U32 u32DisableHDR;
#endif
} ST_DRV_DMS_DBG;

#endif

#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
    // flow control related
    MS_BOOL bResourceRegistered;
    MS_BOOL bDrvInit;
    ST_HAL_DMS stHalDMS;
    ST_DRV_DMS stDrvDMS;
#ifdef DMS_VERSION_DIPGOP
    ST_DRV_DMS_DI stDrvDMSDi;
    ST_DRV_DMS_CAPTUREMODE stDrvDMSCapMode;
    ST_DRV_DMS_DBG stDrvDMSDbg;
    ST_DRV_DMS_SECURE stDrvDMSSecure;
#endif

} DMS_RESOURCE_PRIVATE;

typedef struct __attribute__((__packed__))
{
    MS_BOOL bDummy;
} DMS_COMMON_RESOURCE_PRIVATE;
#endif

#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed)) _DAC_INSTANT_PRIVATE
#else
typedef struct __attribute__((aligned(4))) _DAC_INSTANT_PRIVATE
#endif
{
    IOCTL_DMS_INIT                  fpDMSInit;
    IOCTL_DMS_SETMEMTYPE            fpDMSSetMemType;
    IOCTL_DMS_CREATEWINDOW          fpDMSCreateWin;
    IOCTL_DMS_SETDIGIDECINFO        fpDMSSetDigiDecInfo;
    IOCTL_DMS_CLEARDIGIDECINFO      fpDMSClearDigiDecInfo;
    IOCTL_DMS_DESTORYWINDOW         fpDMSDestoryWin;
    IOCTL_DMS_SETWINDOW             fpDMSSetWin;
    IOCTL_DMS_GETWINDOWINFO         fpDMSGetWinInfo;
    IOCTL_DMS_VIDEOFLIP             fpDMSFlip;
    IOCTL_DMS_SETZORDER             fpDMSZOrder;
    IOCTL_DMS_VIDEOMUTE             fpDMSMute;
    IOCTL_DMS_SETMUTECOLOR          fpDMSSetMuteColor;
    IOCTL_DMS_VIDEOFREEZE           fpDMSFreeze;
    IOCTL_DMS_GETSTATUS             fpDMSGetStatus;
    IOCTL_DMS_GETCAPS               fpDMSGetCaps;
    IOCTL_DMS_GETEXISTWIN           fpDMSGetExistWin;
    IOCTL_DMS_SETOUTPUTLAYER        fpDMSSetOutputLayer;
    IOCTL_DMS_SET3DMODE             fpDMSSet3DMode;
    IOCTL_DMS_SETCAPTUREINFO        fpDMSSetCaptureInfo;
    IOCTL_DMS_GETCAPTUREBUF         fpDMSGetCaptureBuf;
    IOCTL_DMS_RELEASECAPTUREBUF     fpDMSReleaseCaptureBuf;
#ifndef UFO_DMS_OLD_VERSION
    IOCTL_DMS_DYNAMICSCALING        fpDMSDynamicScaling;
    IOCTL_DMS_GETWINDOWID           fpDMSGetWindowID;
    IOCTL_DMS_VDECFRAMERELEASE      fpDMSRegFrameReleaseFun;
    IOCTL_DMS_DISPLAYINFO           fpDMSRegDisplayInfoFun;
    IOCTL_DMS_REGISTERCALLBACKFUN   fpDMSRegCallBackFun;
    IOCTL_DMS_ISSUPPORTCALLBACK     fpDMSIsSupportCallBack;
    IOCTL_DMS_GETVIEODELAY          fpDMSGetVideoDelay;
#endif
    IOCTL_DMS_SECURITYCONTROL       fpDMSSecurityControl;
    IOCTL_DMS_SETMETADATA           fpDMSSetMetaData;

}DMS_INSTANT_PRIVATE;

EN_DMS_RESULT MApi_DMS_Init_U2(void* pInstance, ST_DMS_INITDATA *pstDMS_InitData);
EN_DMS_RESULT MApi_DMS_SetMemory_U2(void* pInstance, ST_DMS_SET_MEMORY_TYPE *pstDMS_SetMemType);
EN_DMS_RESULT MApi_DMS_CreateWindow_U2(void* pInstance, ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID);
EN_DMS_RESULT MApi_DMS_SetDigitalDecodeSignalInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat);
EN_DMS_RESULT MApi_DMS_ClearDigitalDecodeSignalInfo_U2(void* pInstance, MS_U32 u32WindowID);
EN_DMS_RESULT MApi_DMS_DestroyWindow_U2(void* pInstance, MS_U32 u32WindowID);
EN_DMS_RESULT MApi_DMS_SetWindow_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info);
EN_DMS_RESULT MApi_DMS_GetWindowInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info);
EN_DMS_RESULT MApi_DMS_Video_Flip_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);
EN_DMS_RESULT MApi_DMS_SetZOrder_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_ZORDER_INFO *pstZorderInfo);
EN_DMS_RESULT MApi_DMS_Video_Freeze_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze);
EN_DMS_RESULT MApi_DMS_Video_Mute_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_MUTE_WINDOW_INFO *pstWindowMute);
EN_DMS_RESULT MApi_DMS_Set_MuteColor_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_MUTE_COLOR_INFO *pstMuteColor);
EN_DMS_RESULT MApi_DMS_GetStatus_U2(void* pInstance, EN_DMS_STATUS *peStatus);
EN_DMS_RESULT MApi_DMS_GetCapability_U2(void* pInstance, EN_DMS_CAPABILITY *peCapability);
EN_DMS_RESULT MApi_DMS_GetExistedWindows_U2(void* pInstance, MS_U32 *pu32WindowNums);
EN_DMS_RESULT MApi_DMS_SetOutputLayer_U2(void* pInstance, ST_DMS_WINDOW *pstLayer);
EN_DMS_RESULT MApi_DMS_Set_3D_Mode_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo);
EN_DMS_RESULT MApi_DMS_Set_CaptureInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo);
EN_DMS_RESULT MApi_DMS_Get_CaptureBuffer_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
EN_DMS_RESULT MApi_DMS_Release_CaptureBuffer_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
#ifndef UFO_DMS_OLD_VERSION
EN_DMS_RESULT MApi_DMS_GetWindowID_U2(void* pInstance, ST_DMS_GET_ID_INFO *pstGetIDInfo, MS_U32* pu32WindowID);
EN_DMS_RESULT MApi_DMS_DynamicScaling_Enable_U2(void* pInstance, MS_U32 u32WindowID, MS_BOOL bEnable);
EN_DMS_RESULT MApi_DMS_RegisterFrameReleaseFun_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_FRAME_RELEASE_INFO* pstFrameReleaseInfo, MS_U32* pu32ExitFlag);
EN_DMS_RESULT MApi_DMS_RegisterFirstDisplayFun_U2(void* pInstance, MS_U32 u32WindowID, FUNC_DMS_DISPLAYINFO pfFunDisplayInfo);
EN_DMS_RESULT MApi_DMS_RegisterCallBackFun_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CALLBACK_STRUCT* pstCallBack, MS_U32* pu32ExitFlag);
EN_DMS_RESULT MApi_DMS_IsSupportCallBackFun_U2(void* pInstance,MS_BOOL *pbIsSupport);
EN_DMS_RESULT MApi_DMS_GetVideoDelay_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_VIDEO_DELAY_INFO *pstVideoDelayInfo);
#endif
EN_DMS_RESULT MApi_DMS_Security_Control_U2(void* pInstance, EN_DMS_SECURITY_ACTION enAction, ST_DMS_SECURITY_HANDLER* pstDMSHandler);
EN_DMS_RESULT MApi_DMS_SetMetaData_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_METADATA_INFO *pstMetaDataInfo);


void DMSRegisterToUtopia(void);
MS_U32 DMSOpen(void** ppInstance, const void* const pAttribute);
MS_U32 DMSClose(void* pInstance);
MS_U32 DMSIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 DMSMdbIoctl(MS_U32 u32Cmd, const void* const pArgs);
#endif
#ifdef __cplusplus
}
#endif
#endif // _DMS_PRIV_H_
