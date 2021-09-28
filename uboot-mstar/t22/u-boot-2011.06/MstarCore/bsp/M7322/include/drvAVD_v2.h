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
/// @brief  AVD Driver utopia 2.0 Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "drvAVD.h"

#ifndef _DRV_AVD_V2_H_
#define _DRV_AVD_V2_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifdef MS_DEBUG
#define AVD_DRV_DEBUG 1
#else
#define AVD_DRV_DEBUG 0
#endif

#if 0
#define MSIF_AVD_LIB_CODE                   {'A','V','D','_'} //Lib code
#define MSIF_AVD_LIBVER                        {'1','1'}      //LIB version
#define MSIF_AVD_BUILDNUM                 {'0','0' }    //Build Number
#define MSIF_AVD_CHANGELIST             {'0','0','6','1','6','9','9','1'} //P4 ChangeList Number

#define AVD_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_AVD_LIB_CODE,                  /* IP__                                             */  \
    MSIF_AVD_LIBVER,                       /* 0.0 ~ Z.Z                                        */  \
    MSIF_AVD_BUILDNUM,                 /* 00 ~ 99                                          */  \
    MSIF_AVD_CHANGELIST,             /* CL#                                              */  \
    MSIF_OS

// video decoder status
#define VD_SYNC_LOCKED                  ((BIT_(15)))
#define VD_HSYNC_LOCKED                 ((BIT_(14)))
#define VD_INTERLACED                   ((BIT_(13)))
#define VD_VSYNC_50HZ                   ((BIT_(12)))
#define VD_RESET_ON                     ((BIT_(11)))
#define VD_COLOR_LOCKED                 ((BIT_(10)))
#define VD_PAL_SWITCH                   ((BIT_(9)))
#define VD_FSC_TYPE                     ((BIT_(7))|(BIT_(6))|(BIT_(5)))
#define VD_FSC_3579                     ((BIT_(6)))              // NTSC
#define VD_FSC_3575                     ((BIT_(7)))              // PAL(M)
#define VD_FSC_3582                     ((BIT_(7))|(BIT_(6)))         // PAL(Nc)
#define VD_FSC_4433                     (0)                 // PAL or NTSC443
#define VD_FSC_4285                     (BIT_(5))              // SECAM
#define VD_VCR_MODE                     (BIT_(4))
#define VD_VCR_PAUSE                    (BIT_(3))
#define VD_MACROVISION                  (BIT_(2))
#define VD_BURST_ON                        (BIT_(1))
#define VD_STATUS_RDY                   (BIT_(0))
#define VD_MODE_DETECT_MASK             (VD_SYNC_LOCKED|VD_HSYNC_LOCKED|VD_VSYNC_50HZ|VD_FSC_TYPE|VD_RESET_ON) // 20100331 remove BURST check first because too many burst on/off issues

// Gain Control (Auto/Fix/Mix) Define
#define VD_USE_FIX_GAIN                 0
#define VD_USE_AUTO_GAIN                1
// Gain Control (Auto/Fix/Mix) Define
//#define AVD_AGC_ENABLE                              0x00
//#define AVD_AGC_DISABLE                             0x03


#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))
#define VD_MODE_STANDARD_MASK                   (VD_VSYNC_50HZ | VD_PAL_SWITCH | VD_FSC_TYPE | VD_STATUS_RDY)

#define FSC_AUTO_DET_ENABLE                     0x00
#define FSC_AUTO_DET_DISABLE                    0x01

#define VD_USE_FB                               0

// VD Check Hsync Thresholdffff
#define VD_CHK_HSYNC_WAIT                1         // ms
#define VD_CHK_HSYNC_CONT                60//40        // timeout=VD_CHK_HSYNC_CONT*VD_CHK_HSYNC_WAIT=20ms
#define VD_CHK_HSYNC_OK_THR             2   //7
#define VD_CHK_DEBOUNCE_WAIT         20        // ms
#define VD_CHK_DEBOUNCE_CONT         40        // timeout=VD_CHK_COLOR_CONT*VD_CHK_COLOR_WAIT=800ms
#define VD_CHK_NSPL_WAIT                   1       //ms
#define VD_CHK_NSPL_CONT                   500       //timeout=VD_CHK_NSPL_WAIT*VD_CHK_NSPL_CONT=400ms
#define VD_CHK_NSPL_OK_THR              20//10
#define VD_CHK_VideoSys_CONT                   250       //timeout=VD_CHK_NSPL_WAIT*VD_CHK_NSPL_CONT=400ms

#define DSP_VER_OFFSET 255

//----------------AVD Patch flag item--------------------------------------------------------------------------
#define AVD_PATCH_NONE 0
#define AVD_PATCH_NTSC_50 BIT_(0)
#define AVD_PATCH_FINE_TUNE_FH_DOT BIT_(1)
#define AVD_PATCH_FINE_TUNE_COMB_F2 BIT_(2)
#define AVD_PATCH_FINE_TUNE_3D_COMB BIT_(3)
#define AVD_PATCH_FINE_TUNE_FSC_SHIFT BIT_(4)
#define AVD_PATCH_NON_STANDARD_VTOTAL BIT_(5)
#define AVD_PATCH_FINE_TUNE_STILL_IMAGE BIT_(6)

#define AVD_PATCH_HTOTAL_1135_MUL_15 0
#define AVD_PATCH_HTOTAL_1135 BIT_(8)
#define AVD_PATCH_HTOTAL_DYNAMIC BIT_(9)
#define AVD_PATCH_HTOTAL_USER (BIT_(9)|BIT_(8))
#define AVD_PATCH_HTOTAL_MASK (BIT_(9)|BIT_(8))
#define AVD_PATCH_SCART_SVIDEO (BIT_(10))
#define AVD_PATCH_CVBS_NEGATIVESIG (BIT_(11))

#define AVD_PATCH_DISABLE_PWS BIT_(24)    // used to store ENABLE_PWS

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    AVD_LOAD_CODE_BDMA_FROM_SPI,
    AVD_LOAD_CODE_BDMA_FROM_DRAM,
    AVD_LOAD_CODE_BYTE_WRITE,
} AVD_LoadCodeType;

typedef enum
{
    DEMODE_NORMAL                   = 0,
    DEMODE_MSTAR_VIF                = 1,
    DEMODE_MSTAR_VIF_MSB1210        = 2,
} AVD_DemodType;

typedef enum
{
    E_VIDEO_SYSTEM_ATSC,
    E_VIDEO_SYSTEM_DVB
} AVD_VideoSystem;

typedef enum
{
    E_VIDEO_FQ_NOSIGNAL             = 0,     // Video Frequency No signal
    E_VIDEO_FQ_50Hz                 = 50,    // Video Frequency 50Hz
    E_VIDEO_FQ_60Hz                 = 60     // Video Frequency 60Hz
} AVD_VideoFreq;

typedef enum
{
    E_FREERUN_FQ_AUTO               = 0x00,
    E_FREERUN_FQ_50Hz               = 0x01,
    E_FREERUN_FQ_60Hz               = 0x02,
    E_FREERUN_FQ_MAX
} AVD_FreeRunFreq;

typedef enum
{
    E_VIDEOSTANDARD_PAL_BGHI        = 0x00,        // Video standard PAL BGHI
    E_VIDEOSTANDARD_NTSC_M          = 0x01,        // Video standard NTSC M
    E_VIDEOSTANDARD_SECAM           = 0x02,        // Video standard SECAM
    E_VIDEOSTANDARD_NTSC_44         = 0x03,        // Video standard  NTSC 44
    E_VIDEOSTANDARD_PAL_M           = 0x04,        // Video standard  PAL M
    E_VIDEOSTANDARD_PAL_N           = 0x05,        // Video standard  PAL N
    E_VIDEOSTANDARD_PAL_60          = 0x06,        // Video standard PAL 60
    E_VIDEOSTANDARD_NOTSTANDARD     = 0x07,        // NOT Video standard
    E_VIDEOSTANDARD_AUTO            = 0x08,         // Video standard AUTO
    E_VIDEOSTANDARD_MAX                                // Max Number
} AVD_VideoStandardType;

typedef enum
{
    E_INPUT_SOURCE_INVALID                  =   0x00,               // Video source Invalid
    E_INPUT_SOURCE_ATV                      =   0x01,               // Video source ATV
    E_INPUT_SOURCE_CVBS1                    =   0x02,               // Video source CVBS 1
    E_INPUT_SOURCE_CVBS2                    =   0x03,               // Video source CVBS 2
    E_INPUT_SOURCE_CVBS3                    =   0x04,               // Video source CVBS 2
    E_INPUT_SOURCE_SVIDEO1                  =   0x05,               // Video source SVIDEO 1
    E_INPUT_SOURCE_SVIDEO2                  =   0x06,               // Video source SVIDEO 2
    E_INPUT_SOURCE_SCART1                   =   0x07,               // Video source SCART 1
    E_INPUT_SOURCE_SCART2                   =   0x08,               // Video source SCART 2
    E_INPUT_SOURCE_YPbPr                    =   0x09,               // Video source YPbPr
    E_INPUT_SOURCE_THROUGH_3DCOMB           =   0x50,               // Video source through 3D Comb
    E_INPUT_SOURCE_THROUGH_3DCOMB_ATV       =   (E_INPUT_SOURCE_THROUGH_3DCOMB | E_INPUT_SOURCE_ATV),   // through 3D Comb ATV
    E_INPUT_SOURCE_THROUGH_3DCOMB_CVBS1     =   (E_INPUT_SOURCE_THROUGH_3DCOMB | E_INPUT_SOURCE_CVBS1),// through 3D Comb CVBS1
    E_INPUT_SOURCE_THROUGH_3DCOMB_CVBS2     =   (E_INPUT_SOURCE_THROUGH_3DCOMB | E_INPUT_SOURCE_CVBS2),// through 3D Comb CVBS2
    E_INPUT_SOURCE_THROUGH_3DCOMB_SCART1    =   (E_INPUT_SOURCE_THROUGH_3DCOMB | E_INPUT_SOURCE_SCART1),// through 3D Comb SCART1
    E_INPUT_SOURCE_THROUGH_3DCOMB_SCART2    =   (E_INPUT_SOURCE_THROUGH_3DCOMB | E_INPUT_SOURCE_SCART2),// through 3D Comb SCART2
    E_INPUT_SOURCE_MAX                      =   0x60
} AVD_InputSourceType;

/// Auto AV Source type
typedef enum
{
    E_AUTOAV_SOURCE_1   =   E_INPUT_SOURCE_SCART1,       // Auto AV source SCART 1
    E_AUTOAV_SOURCE_2   =   E_INPUT_SOURCE_SCART2,       // Auto AV source SCART 2
    E_AUTOAV_SOURCE_ALL =   0xFF                // Auto AV source ALL
} AVD_AutoAVSourceType;

typedef enum    // 0x2580[5:4]
{
    SCART_FB_NONE = 0x00,
    SCART_FB0,
    SCART_FB1,
    SCART_FB2,
} AVD_INPUT_SCART_FB;

typedef enum
{
    E_ATV_CLK_ORIGIN_43P2MHZ   =    0x00,
    E_ATV_CLK_TYPE1_42MHZ         =    0x01,
    E_ATV_CLK_TYPE2_44P4MHZ     =    0x02,
}AVD_ATV_CLK_TYPE;

typedef struct
{
    MS_U8 u8DetectWinBeforeLock;
    MS_U8 u8DetectWinAfterLock;
    MS_U8 u8CNTRFailBeforeLock;
    MS_U8 u8CNTRSyncBeforeLock;
    MS_U8 u8CNTRSyncAfterLock;
} VD_HSYNC_SENSITIVITY;

typedef enum
{
    AVD_DBGLV_NONE,    // disable all the debug message
    AVD_DBGLV_INFO,    // information
    AVD_DBGLV_NOTICE,  // normal but significant condition
    AVD_DBGLV_WARNING, // warning conditions
    AVD_DBGLV_ERR,     // error conditions
    AVD_DBGLV_CRIT,    // critical conditions
    AVD_DBGLV_ALERT,   // action must be taken immediately
    AVD_DBGLV_EMERG,   // system is unusable
    AVD_DBGLV_DEBUG,   // debug-level messages
} AVD_DbgLv;

typedef struct
{
        // register init
    MS_U8 *u8VdDecInitializeExt; // TODO use system variable type

    MS_U32 u32XTAL_Clock;
    AVD_LoadCodeType eLoadCodeType;
    MS_U32 u32VD_DSP_Code_Address;
    MS_U8 *pu8VD_DSP_Code_Address;
    MS_U32 u32VD_DSP_Code_Len;
    // VIF related
    AVD_DemodType eDemodType;

    // afec related
    MS_U16 u16VDDSPBinID;
    MS_U8 bRFGainSel;
    MS_U8 bAVGainSel;
    MS_U8 u8RFGain;
    MS_U8 u8AVGain;
    MS_U32  u32VDPatchFlag;
    MS_U8 u8ColorKillHighBound;
    MS_U8 u8ColorKillLowBound;
    MS_U8 u8SwingThreshold;
    VD_HSYNC_SENSITIVITY eVDHsyncSensitivityNormal;
    VD_HSYNC_SENSITIVITY eVDHsyncSensitivityTuning;

    // comb related
    MS_U32 u32COMB_3D_ADR;
    MS_U32 u32COMB_3D_LEN;

} VD_INITDATA;

typedef enum
{
    E_AVD_FAIL=0,
    E_AVD_OK=1
} AVD_Result;


typedef struct
{
    AVD_InputSourceType eVDInputSource;
    AVD_VideoStandardType eVideoSystem;
    AVD_VideoStandardType eLastStandard;
    MS_U8  u8AutoDetMode;
    MS_U16 u16CurVDStatus;
    MS_U8 u8AutoTuningIsProgress;
} AVD_Info;

typedef struct
{
    MS_U8 u8Threshold1;
    MS_U8 u8Threshold2;
    MS_U8 u8Threshold3;
    MS_U8 u8Threshold4;

    MS_U8 u8Str1_COMB37;
    MS_U8 u8Str1_COMB38;
    MS_U8 u8Str1_COMB7C;
    MS_U8 u8Str1_COMBED;

    MS_U8 u8Str2_COMB37;
    MS_U8 u8Str2_COMB38;
    MS_U8 u8Str2_COMB7C;
    MS_U8 u8Str2_COMBED;

    MS_U8 u8Str3_COMB37;
    MS_U8 u8Str3_COMB38;
    MS_U8 u8Str3_COMB7C;
    MS_U8 u8Str3_COMBED;

    MS_BOOL bMessageOn;
} AVD_Still_Image_Param;

typedef enum
{
    E_FACTORY_PARA_AFEC_D4,
    E_FACTORY_PARA_AFEC_D8,
    E_FACTORY_PARA_AFEC_D5_BIT2,
    E_FACTORY_PARA_AFEC_D9_BIT0,
    E_FACTORY_PARA_AFEC_A0,
    E_FACTORY_PARA_AFEC_A1,
    E_FACTORY_PARA_AFEC_66_BIT67,
    E_FACTORY_PARA_AFEC_6E_BIT7654,
    E_FACTORY_PARA_AFEC_6E_BIT3210,
    E_FACTORY_PARA_AFEC_43,
    E_FACTORY_PARA_AFEC_44,
    E_FACTORY_PARA_AFEC_CB,
    E_FACTORY_PARA_AFEC_CF_BIT2,
    E_FACTORY_PARA_AFEC_D5_BIT3,
    E_FACTORY_PARA_AFEC_MAX
}AVD_Factory_Para;
#endif

// collect global variable into _AVD_RESOURCE_PRIVATE 201303 weicheng
typedef struct DLL_PACKED _AVD_RESOURCE_PRIVATE
{
    MS_BOOL _bShiftClkFlag;
    MS_BOOL _b2d3dautoflag;
    MS_BOOL _bSTRFlag;
	    MS_U8  _u8HtotalDebounce;
    MS_U8  _u8AutoDetMode;
    MS_U8 _u8AfecD4Factory;
    MS_U8 _u8Comb10Bit3Flag;
    MS_U8 _u8Comb57;
    MS_U8 _u8Comb58;
    MS_U8 _u8Comb5F;
    MS_U8 _u8SCARTSwitch;  // 0: CVBS, 1:Svideo;
    MS_U8 _u8SCARTPrestandard;
    MS_U8 _u8AutoTuningIsProgress;

    MS_U16 _u16CurVDStatus;
    MS_U16 _u16DataH[3];
    MS_U16 _u16LatchH;
    MS_U16 _u16Htt_UserMD;
    MS_U16 _u16DPL_MSB;
    MS_U16 _u16DPL_LSB;
    MS_U32 u32VDPatchFlagStatus;
    MS_U32 _u32VideoSystemTimer;
    MS_U32 _u32SCARTWaitTime;
    MS_S32 _s32AVD_Mutex;

    AVD_VideoStandardType _eVideoSystem;
    AVD_VideoStandardType _eForceVideoStandard;
    AVD_VideoStandardType _eLastStandard;
    AVD_InputSourceType _eVDInputSource;
    VD_INITDATA g_VD_InitData;
    AVD_ATV_CLK_TYPE gShiftMode;
    AVD_Still_Image_Param g_stillImageParam;

    #if AVD_DRV_DEBUG
    MS_U32 u32PreVDPatchFlagStatus;
    AVD_DbgLv _u8AVDDbgLevel;
    #endif
    #ifndef MSOS_TYPE_LINUX
    MS_U16 u16PreVtotal;
    #endif

}AVD_RESOURCE_PRIVATE;

typedef enum {
    MDrv_CMD_AVD_Init,
    MDrv_CMD_AVD_Exit,
    MDrv_CMD_AVD_ResetMCU,
    MDrv_CMD_AVD_FreezeMCU,
    MDrv_CMD_AVD_Scan_HsyncCheck,
    MDrv_CMD_AVD_StartAutoStandardDetection,
    MDrv_CMD_AVD_ForceVideoStandard,
    MDrv_CMD_AVD_3DCombSpeedup,
    MDrv_CMD_AVD_LoadDSP,
    MDrv_CMD_AVD_BackPorchWindowPositon,
    MDrv_CMD_AVD_MBX_ReadByteByVDMbox,

    MDrv_CMD_AVD_SetFlag,
    MDrv_CMD_AVD_SetRegValue,
    MDrv_CMD_AVD_SetRegFromDSP,
    MDrv_CMD_AVD_SetInput,
    MDrv_CMD_AVD_SetVideoStandard,
    MDrv_CMD_AVD_SetChannelChange,
    MDrv_CMD_AVD_SetHsyncDetectionForTuning,
    MDrv_CMD_AVD_Set3dComb,
    MDrv_CMD_AVD_SetShiftClk,
    MDrv_CMD_AVD_SetFreerunPLL,
    MDrv_CMD_AVD_SetFreerunFreq,
    MDrv_CMD_AVD_SetFactoryPara,
    MDrv_CMD_AVD_Set_Htt_UserMD,
    MDrv_CMD_AVD_SetDbgLevel,
    MDrv_CMD_AVD_SetPQFineTune,
    MDrv_CMD_AVD_Set3dCombSpeed,
    MDrv_CMD_AVD_SetStillImageParam,
    MDrv_CMD_AVD_SetAFECD4Factory,
    MDrv_CMD_AVD_Set2D3DPatchOnOff,
    MDrv_CMD_AVD_SetAutoFineGainToFixed,

    MDrv_CMD_AVD_GetFlag,
    MDrv_CMD_AVD_GetRegValue,
    MDrv_CMD_AVD_GetStatus,
    MDrv_CMD_AVD_GetNoiseMag,
    MDrv_CMD_AVD_GetVTotal,
    MDrv_CMD_AVD_GetStandardDetection,
    MDrv_CMD_AVD_GetLastDetectedStandard,
    MDrv_CMD_AVD_GetCaptureWindow,
    MDrv_CMD_AVD_GetVerticalFreq,
    MDrv_CMD_AVD_GetHsyncEdge,
    MDrv_CMD_AVD_GetDSPFineGain,
    MDrv_CMD_AVD_GetDSPVersion,
    MDrv_CMD_AVD_GetLibVer,
    MDrv_CMD_AVD_GetInfo,
    MDrv_CMD_AVD_IsSyncLocked,
    MDrv_CMD_AVD_IsSignalInterlaced,
    MDrv_CMD_AVD_IsColorOn,
    MDrv_CMD_AVD_SetPowerState,
    MDrv_CMD_AVD_GetMacroVisionDetect,
    MDrv_CMD_AVD_GetCGMSDetect,
    MDrv_CMD_AVD_SetBurstWinStart,
    MDrv_CMD_AVD_AliveCheck,
    MDrv_CMD_AVD_IsLockAudioCarrier,
    MDrv_CMD_AVD_SetAnaDemod,
    MDrv_CMD_AVD_SetHsyncSensitivityForDebug,
} eAvdIoctlOpt;

typedef struct DLL_PACKED _AVD_INIT
{
    VD_INITDATA pVD_InitData;
    MS_U32 u32InitDataLen;
    AVD_Result pVD_Result;
}AVD_INIT, *PAVD_INIT;

typedef struct DLL_PACKED _AVD_LOADDSP
{
    MS_U8 *pu8VD_DSP;
    MS_U32 len;
}AVD_LOADDSP, *PAVD_LOADDSP;

typedef struct DLL_PACKED _AVD_BACKPORCHWINPOS
{
    MS_BOOL bEnable ;
    MS_U8 u8Value;
}AVD_BACKPORCHWINPOS, *PAVD_BACKPORCHWINPOS;

typedef struct DLL_PACKED _AVD_SETREGVALUE
{
    MS_U16 u16Addr;
    MS_U8 u8Value;
}AVD_SETREGVALUE, *PAVD_SETREGVALUE;

typedef struct DLL_PACKED _AVD_SETINPUT
{
    AVD_InputSourceType eSource;
    MS_U8 u8ScartFB;
    MS_BOOL bEnable;
}AVD_SETINPUT, *PAVD_SETINPUT;

typedef struct DLL_PACKED _AVD_SETVIDEOSTANDARD
{
    AVD_VideoStandardType eStandard;
    MS_BOOL bIsInAutoTuning;
    MS_BOOL bEnable;
}AVD_SETVIDEOSTANDARD, *PAVD_SETVIDEOSTANDARD;

typedef struct DLL_PACKED _AVD_SETSHIFTCLK
{
    MS_BOOL bEnable;
    AVD_ATV_CLK_TYPE eShiftMode;
}AVD_SETSHIFTCLK, *PAVD_SETSHIFTCLK;

typedef struct DLL_PACKED _AVD_SETFACTORYPARA
{
    AVD_Factory_Para FactoryPara;
    MS_U8 u8Value;
}AVD_SETFACTORYPARA, *PAVD_SETFACTORYPARA;

typedef struct DLL_PACKED _AVD_SET3DCOMBSPEED
{
    MS_U8 u8COMB57;
    MS_U8 u8COMB58;
}AVD_SET3DCOMBSPEED, *PAVD_SET3DCOMBSPEED;

typedef struct DLL_PACKED _AVD_GETCAPTUREWINDOW
{
    void *stCapWin;
    AVD_VideoStandardType eVideoStandardType;
}AVD_GETCAPTUREWINDOW, *PAVD_GETCAPTUREWINDOW;

typedef struct DLL_PACKED _AVD_GETSTANDARDDETECTION
{
    AVD_VideoStandardType VideoStandardType;
    MS_U16 vdLatchStatus;
}AVD_GETSTANDARDDETECTION, *PAVD_GETSTANDARDDETECTION;

typedef struct DLL_PACKED _AVD_SCANHSYNCCHECK
{
    MS_U8 u8HtotalTolerance;
    MS_U16 u16ScanHsyncCheck;
}AVD_SCANHSYNCCHECK, *PAVD_SCANHSYNCCHECK;

typedef struct DLL_PACKED _AVD_FORCEVIDEOSTANDARD
{
    AVD_VideoStandardType eVideoStandardType;
    MS_BOOL bEnable;
}AVD_FORCEVIDEOSTANDARD, *PAVD_FORCEVIDEOSTANDARD;

typedef struct DLL_PACKED _AVD_MBXREADBYTEBYVDMBOX
{
    MS_U8 u8Addr;
    MS_U8 u8Value;
}AVD_MBXREADBYTEBYVDMBOX, *PAVD_MBXREADBYTEBYVDMBOX;

typedef struct DLL_PACKED _AVD_SETDBGLEVEL
{
    AVD_DbgLv u8DbgLevel;
    MS_BOOL bEnable;
}AVD_SETDBGLEVEL, *PAVD_SETDBGLEVEL;

typedef struct DLL_PACKED _AVD_GETREGVALUE
{
    MS_U16 u16Addr;
    MS_U8 u8Value;
}AVD_GETREGVALUE, *PAVD_GETREGVALUE;

typedef struct DLL_PACKED _AVD_SETPOWERSTATE
{
    EN_POWER_MODE u16PowerState;
    MS_U32 u32Value;
}AVD_SETPOWERSTATE, *PAVD_SETPOWERSTATE;

typedef struct DLL_PACKED _AVD_GETLIBVER
{
    const MSIF_Version **ppVersion;
    AVD_Result eAVDResult;
}AVD_GETLIBVER, *PAVD_GETLIBVER;

typedef struct DLL_PACKED _AVD_COPYTOUSER
{
    MS_BOOL bEnable;
    MS_U8 u8Value;
    MS_U16 u16Value;
    MS_U32 u32Value;
    AVD_VideoStandardType eVideoStandardType;
    AVD_VideoFreq eVideoFreq;
    AVD_Info eAVDInfo;
}AVD_COPYTOUSER, *PAVD_COPYTOUSER;

typedef struct _AVD_SETHSYNCSENSITIVITYFORDEBUG
{
    MS_U8 u8AFEC99;
    MS_U8 u8AFEC9C;
}AVD_SETHSYNCSENSITIVITYFORDEBUG, *PAVD_SETHSYNCSENSITIVITYFORDEBUG;
#endif // _DRV_AVD_V2_H_
