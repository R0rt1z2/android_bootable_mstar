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
/// @brief  AVD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup VDEC VDEC modules

 *! \defgroup AVD AVD interface (drvAVD.h)
 *  \ingroup VDEC

     AVD (Analog Video Decoder) gets CVBS signal status from VDMCU through VD mailbox. VDMCU reports information like H/V sync status, video standard...\n
     AVD separate CVBS signal into Y/C signal by using 2D/3D COMB filter.

     <b>Features</b>

     - detect CVBS signal status and video stadnard
     - 3D COMB filter
     - Get signal Noise
     - CVBS color on/off

     <b> AVD Block Diagram: </b> \n
     \image html drvAVD_1.png

     <b> AVD Code Flow: </b> \n
     \image html drvAVD_2.png

 *! \defgroup VDMCU VDMCU
 *  \ingroup AVD

 *! \defgroup VD_Info VD_Info
 *  \ingroup AVD

 *! \defgroup VD_Task VD_Task
 *  \ingroup AVD

 *! \defgroup COMB COMB relative
 *  \ingroup AVD

 *! \defgroup AVD_ToBeModified AVD api to be modified
 *  \ingroup AVD

 *! \defgroup AVD_ToBeRemove AVD api to be removed
 *  \ingroup AVD

 *! \defgroup Get_VD_Info Get_VD_Info
 *  \ingroup VD_Info

 *! \defgroup Set_VD_Info Set_VD_Info
 *  \ingroup VD_Info

 */


#ifndef _DRV_AVD_H_
#define _DRV_AVD_H_

#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define MSTAR   1
#define STELLAR 2
#define CUSTOMER MSTAR

#if (CUSTOMER == MSTAR)
#define MSTAR_STR
#define MSTAR_FLAG
#elif (CUSTOMER == STELLAR)
#define STELLAR_STR
#define STELLAR_FLAG
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
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
#define VD_FSC_NTSC443                  ((BIT_(6))|(BIT_(5)))         // For NTSC443 Forced mode // ivan
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
// Coarse Gain Define
#define VD_AGC_COARSE_GAIN_X_0_5        0   // x0.5
#define VD_AGC_COARSE_GAIN_X_1          1   // x1
#define VD_AGC_COARSE_GAIN_X_2          2   // x2
#define VD_AGC_COARSE_GAIN_X_4          3   // x4
#define VD_AGC_COARSE_GAIN_X_13         13  // 0xD  1x  A1 & A5 series has different gain mapping

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
#define AVD_PATCH_DSPCODE_0x24_ATSC BIT_(25) //L_ATSC or 2013.12.24 for lm14 bringup
#define AVD_PATCH_DSPCODE_0x25 BIT_(26) //L_MSTAR or 2013.12.24 for lm14 bringup
#define AVD_PATCH_DSPCODE_0x21_ATSC BIT_(27) //S_ATSC
#define AVD_PATCH_DSPCODE_0x21 BIT_(28) //S
#define AVD_PATCH_SKY_STB_COLOR_STRIPE BIT_(27)
#define AVD_PATCH_INTERNAL_DEMOD BIT_(28)
/// For vd init
typedef struct __attribute__((__packed__))
{
        // register init
    MS_U8 *u8VdDecInitializeExt; // TODO use system variable type
    #if !defined (__aarch64__)
    MS_U32 u32AlignDummy;
    #endif

    MS_U32 u32XTAL_Clock;
    AVD_LoadCodeType eLoadCodeType;
    MS_U32 u32VD_DSP_Code_Address;
    MS_U8 *pu8VD_DSP_Code_Address;
    #if !defined (__aarch64__)
    MS_U32 u32AlignDummy1;
    #endif

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
    MS_PHY u32COMB_3D_ADR;
    MS_U32 u32COMB_3D_LEN;

    //heap id
    MS_U32 u32CMA_HEAP_ID;

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
    E_FACTORY_PARA_AFEC_D7_HIGH,
    E_FACTORY_PARA_AFEC_D7_LOW,
    E_FACTORY_PARA_AFEC_MAX
}AVD_Factory_Para;

//typedef void(*P_AVD_ISR_Proc)(MS_U8 u8AVDID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Set VDMCU freeze
/// @ingroup VDMCU
/// @param  bEnable   \b IN: MCU freeze enable
/// @return NA
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_MCUFreeze(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Turn OFF AVD
/// @ingroup VD_Task
/// @return NA
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_Exit(void);
////////////////////////////////////////////////////////////////////////////////
/// To initialize Video Decoder.\n
/// When turning power on or waking up from power saving mode,\n
/// this function should be called before using all funcions\n
/// MS_U32 u32XTAL_Clock        : external crystal clock.\n
/// MS_U32 u32VD_DSP_Code_Address        : absolute flash address of VDDSP bin file\n
/// MS_U8 *pu8VD_DSP_Code_Address        : address of VDDSP code\n
/// MS_U32 u32VD_DSP_Code_Len        : VDDSP code length.\n
/// AVD_DemodType eDemodType        : analog Demod. type.\n
/// MS_U16 u16VDDSPBinID     : binary ID of VD DSP code.\n
/// MS_U8 bRFGainSel        : RF auto/fix gain selection\n
/// MS_U8 bAVGainSel        : AV auto/fix gain selection\n
/// MS_U8 u8RFGain        : when RF is fix gain, u8RFGain will set gain value\n
/// MS_U8 u8AVGain        : when AV is fix gain, u8AVGain will set gain value\n
/// MS_U32 u32VDPatchFlag        : set AVD control flag\n
/// MS_U8 u8ColorKillHighBound        : when color burst is detected and color burst S/N is lower than u8ColorKillLowBound, turn off color.\n
/// MS_U8 u8ColorKillLowBound        : when color burst is not detected and color burst S/N is higher than u8ColorKillHighBound, turn on color.\n
///                                                                          u8ColorKillHighBound should be larger than u8ColorKillLowBound for hysteresis\n
/// MS_U8 u8SwingThreshold        : when signal swing(top-bottom) is smaller than u8SwingThreshold*16mV, report no sync\n
///                                                                      this threshold is used to filter out false channel, but the value should be set to 0 at channel scan to prevent channel lost\n
/// VD_HSYNC_SENSITIVITY eVDHsyncSensitivityNormal        : hsync sensitivity at normal condition\n
/// VD_HSYNC_SENSITIVITY eVDHsyncSensitivityTuning        : hsync sensitivity at channel scan\n
/// MS_U32 u32COMB_3D_ADR    : 3D COMB protected memory buffer start address.\n
/// MS_U32 u32COMB_3D_LEN    : 3D COMB protected memory length.\n
/// MS_U8 *u8VdDecInitializeExt    : additional initial table for different project requirements\n
///
/// MS_U32 u32InitDataLen : used to check data size correct or not
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Initialize AVD
/// @ingroup VD_Task
/// @param  pVD_InitData   \b IN: AVD initialize parameters
/// @param  u32InitDataLen   \b IN: InitData length
/// @return NA
//-------------------------------------------------------------------------------------------------
extern AVD_Result MDrv_AVD_Init(VD_INITDATA *pVD_InitData, MS_U32 u32InitDataLen);

//-------------------------------------------------------------------------------------------------
/// Reset VDMCU
/// @ingroup VDMCU
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_McuReset(void);
////////////////////////////////////////////////////////////////////////////////
/// Get video decoder status
/// VD_SYNC_LOCKED                  ((BIT(15)))\n
/// VD_HSYNC_LOCKED                 ((BIT(14)))\n
/// VD_INTERLACED                   ((BIT(13)))\n
/// VD_VSYNC_50HZ                   ((BIT(12)))\n
/// VD_RESET_ON                     ((BIT(11)))\n
/// VD_COLOR_LOCKED                 ((BIT(10)))\n
/// VD_PAL_SWITCH                   ((BIT(9)))\n
/// VD_FSC_TYPE                     ((BIT(7))|(BIT(6))|(BIT(5)))\n
/// VD_FSC_3579                     ((BIT(6)))              // NTSC\n
/// VD_FSC_3575                     ((BIT(7)))              // PAL(M)\n
/// VD_FSC_3582                     ((BIT(7))|(BIT(6)))         // PAL(Nc)\n
/// VD_FSC_4433                     (0)                 // PAL or NTSC443\n
/// VD_FSC_4285                     (BIT(5))              // SECAM\n
/// VD_VCR_MODE                     (BIT(4))\n
/// VD_VCR_PAUSE                    (BIT(3))\n
/// VD_MACROVISION                  (BIT(2))\n
/// VD_BURST_ON                        (BIT(1))\n
/// VD_STATUS_RDY                   (BIT(0))\n
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Get VD Status
/// @ingroup Get_VD_Info
/// @return U16 parameters : AVD U16 Status
//-------------------------------------------------------------------------------------------------
extern MS_U16 MDrv_AVD_GetStatus(void);
//-------------------------------------------------------------------------------------------------
/// Get VD Status
/// @ingroup AVD_ToBeRemove
/// @return U16 parameters : AVD U16 Status
//-------------------------------------------------------------------------------------------------
extern AVD_Result MDrv_AVD_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// Set AVD debug parameters
/// @ingroup Set_VD_Info
/// @param  u8DbgLevel   \b IN: debug parameters
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_SetDbgLevel(AVD_DbgLv u8DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Get AVD Info(standard, source type, VD status)
/// @ingroup Get_VD_Info
/// @return : AVD Info struct
//-------------------------------------------------------------------------------------------------
extern const AVD_Info* MDrv_AVD_GetInfo(void);

//-------------------------------------------------------------------------------------------------
/// Get Hsync lock information
/// @ingroup Get_VD_Info
/// @return TRUE: Hsync lock
/// @return FALSE: Hsync unlock
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_IsSyncLocked(void);
#if 0 // use MDrv_AVD_IsHsyncLocked
extern MS_BOOL MDrv_VD_IsSyncDetected(void);
#endif
//-------------------------------------------------------------------------------------------------
/// Get Hsync lock information while ATV auto-tuning
/// @ingroup Get_VD_Info
/// @param  u8HtotalTolerance   \b IN: Htotal tolerance for Hsync lock detection
/// @return TRUE : Find legal channel while auto-tuning
/// @return FALSE : No legal channel while auto-tuning
//-------------------------------------------------------------------------------------------------
extern MS_U16 MDrv_AVD_Scan_HsyncCheck(MS_U8 u8HtotalTolerance); //TODO should add delay between register read

//-------------------------------------------------------------------------------------------------
/// Get VD Status
/// @ingroup AVD_ToBeRemove
/// @return U16 parameters : AVD U16 Status
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_IsSignalInterlaced(void);

//-------------------------------------------------------------------------------------------------
/// Get VD Status
/// @ingroup AVD_ToBeRemove
/// @return U16 parameters : AVD U16 Status
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_IsColorOn(void);
#if 0 // BY 20090403 unused function
MS_BOOL MDrv_VD_IsStandardValid(void);
#endif
//-------------------------------------------------------------------------------------------------
/// Get Vsync freq information
/// @ingroup Get_VD_Info
/// @return Vsync frequency
//-------------------------------------------------------------------------------------------------
extern AVD_VideoFreq MDrv_AVD_GetVerticalFreq(void);

//-------------------------------------------------------------------------------------------------
/// Get V total
/// @ingroup Get_VD_Info
/// @return U16 Vtotal
//-------------------------------------------------------------------------------------------------
extern MS_U16 MDrv_AVD_GetVTotal(void);

//-------------------------------------------------------------------------------------------------
/// Get Noise magnitude
/// @ingroup Get_VD_Info
/// @return U8 Noise
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AVD_GetNoiseMag(void);

//-------------------------------------------------------------------------------------------------
/// Set Input source and MUX
/// @ingroup VD_Task
/// @param  eSource   \b IN: Input source type
/// @param  u8ScartFB   \b IN: Fast blanking setting
/// @return TRUE: succeed
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_SetInput(AVD_InputSourceType eSource, MS_U8 u8ScartFB);

//-------------------------------------------------------------------------------------------------
/// Set Video standard
/// @ingroup Set_VD_Info
/// @param  eStandard   \b IN: Input video standard type
/// @param  bIsInAutoTuning   \b IN: Auto-tuning flag
/// @return TRUE: succeed
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_SetVideoStandard(AVD_VideoStandardType eStandard, MS_BOOL bIsInAutoTuning);

//-------------------------------------------------------------------------------------------------
/// Set auto standard detection from force standard
/// @ingroup Set_VD_Info
/// @return NA
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_StartAutoStandardDetection(void);
#if 0 // BY 20090403 unused function
extern void MDrv_VD_StopAutoStandardDetection(void);
#endif
//-------------------------------------------------------------------------------------------------
/// Force video standard and disable auto standard detection
/// @ingroup Set_VD_Info
/// @param  eVideoStandardType   \b IN: Force video standard type
/// @return TRUE: succeed
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_ForceVideoStandard(AVD_VideoStandardType eVideoStandardType);
#if 0 // BY 20090403 use MDrv_VD_StartAutoStandardDetection, not supported
extern void MDrv_VD_DisableForceMode(void);
#endif
#if 0 // BY 20090403 use MDrv_VD_GetStandardDetection, not supported
extern AVD_VIDEOSTANDARD_TYPE MDrv_VD_GetResultOfAutoStandardDetection(void);
#endif
//-------------------------------------------------------------------------------------------------
/// Get video standard
/// @ingroup Get_VD_Info
/// @param  u16LatchStatus   \b IN: AVD status
/// @return AVD video standard
//-------------------------------------------------------------------------------------------------
extern AVD_VideoStandardType MDrv_AVD_GetStandardDetection(MS_U16 *u16LatchStatus);
#if 0 // BY 20090408 unused function
extern AVD_VIDEOSTANDARD_TYPE MDrv_VD_DetectStandardAutomatically(void);
#endif
//-------------------------------------------------------------------------------------------------
/// Get VD Status
/// @ingroup AVD_ToBeRemove
/// @return U16 parameters : AVD U16 Status
//-------------------------------------------------------------------------------------------------
extern AVD_VideoStandardType MDrv_AVD_GetLastDetectedStandard(void);
#if 0 // BY 20090403 unused function
extern void MDrv_VD_EnableAutoGainControl(MS_BOOL bEnable);
#endif
//-------------------------------------------------------------------------------------------------
/// Set 3D COMB para
/// @ingroup COMB relative
/// @param  bEnable   \b IN: set 3D COMB enable
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_Set3dComb(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set 3D COMB speed up settings
/// @ingroup COMB relative
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_3DCombSpeedup(void);

//-------------------------------------------------------------------------------------------------
/// Get VD Status
/// @ingroup AVD_ToBeRemove
/// @return U16 parameters : AVD U16 Status
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetFreerunPLL(AVD_VideoFreq eVideoFreq);// 060508 chris

//-------------------------------------------------------------------------------------------------
/// Set free run vertical lines
/// @ingroup COMB relative
/// @param  eFreerunfreq   \b IN: input freen run frequency
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetFreerunFreq(AVD_FreeRunFreq eFreerunfreq);

//-------------------------------------------------------------------------------------------------
/// Set register from DSP and monitor register settings
/// @ingroup VD_Task
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetRegFromDSP(void);

//-------------------------------------------------------------------------------------------------
/// Set AVD patch flag
/// @ingroup Set_VD_Info
/// @param  u32VDPatchFlag   \b IN: AVD patch flag
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetFlag(MS_U32  u32VDPatchFlag);

//-------------------------------------------------------------------------------------------------
/// Get AVD patch flag
/// @ingroup Get_VD_Info
/// @return AVD patch flag
//-------------------------------------------------------------------------------------------------
extern MS_U32 MDrv_AVD_GetFlag(void);

//-------------------------------------------------------------------------------------------------
/// Set output sampling htotal by user
/// @ingroup Set_VD_Info
/// @param  u16Htt   \b IN: output htotal
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_Set_Htt_UserMD(MS_U16 u16Htt);

//-------------------------------------------------------------------------------------------------
/// Reset DSP AGC duriing ATV channel change
/// @ingroup Set_VD_Info
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetChannelChange(void);

//-------------------------------------------------------------------------------------------------
/// Set Hsync lock sensitivity for auto-tuning/normal
/// @ingroup Set_VD_Info
/// @param  bEnable   \b IN: auto-tuning:1  normal:0   Hsync sensitivity settings
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetHsyncDetectionForTuning(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set AVD register
/// @ingroup Set_VD_Info
/// @param  u16Addr   \b IN: register address
/// @param  u8Value   \b IN: register value
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetRegValue(MS_U16 u16Addr,MS_U8 u8Value);

//-------------------------------------------------------------------------------------------------
/// Get AVD register value
/// @ingroup Get_VD_Info
/// @param  u16Addr   \b IN: register address
/// @return register value
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AVD_GetRegValue(MS_U16 u16Addr);

//-------------------------------------------------------------------------------------------------
/// Load VD DSP code
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_LoadDSP(MS_U8 *pu8VD_DSP, MS_U32 len);

//-------------------------------------------------------------------------------------------------
/// Set fixed gain enable
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetPQFineTune(void);

//-------------------------------------------------------------------------------------------------
/// Set 3D COMB speed para
/// @ingroup COMB relative
/// @param  u8COMB57   \b IN: set BK_COMB_57 value
/// @param  u8COMB58   \b IN: set BK_COMB_58 value
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_Set3dCombSpeed(MS_U8 u8COMB57, MS_U8 u8COMB58);

//-------------------------------------------------------------------------------------------------
/// Get capture window
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_GetCaptureWindow(void *stCapWin, AVD_VideoStandardType eVideoStandardType);

//-------------------------------------------------------------------------------------------------
/// Read value from VD mailbox
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AVD_MBX_ReadByteByVDMbox(MS_U8 u8Addr);

//-------------------------------------------------------------------------------------------------
/// Get Hsync edge
/// @ingroup Get_VD_Info
/// @return Hsync edge
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AVD_GetHsyncEdge(void);

//-------------------------------------------------------------------------------------------------
/// Set 3D COMB stillimage parameters
/// @ingroup COMB relative
/// @param  param   \b IN: Stillimage parameters
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetStillImageParam(AVD_Still_Image_Param param);

//-------------------------------------------------------------------------------------------------
/// Set BK_AFEC_D4 value
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetAFECD4Factory(MS_U8 u8Value);

//-------------------------------------------------------------------------------------------------
/// Set 2D/3D COMB patch
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_Set2D3DPatchOnOff(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set back porch position
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_BackPorchWindowPositon(MS_BOOL bEnable , MS_U8 u8Value);

//-------------------------------------------------------------------------------------------------
/// Set AVD factory parameters
/// @ingroup Set_VD_Info
/// @param  FactoryPara   \b IN: factory parameters/register
/// @param  u8Value   \b IN: setting value
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetFactoryPara(AVD_Factory_Para FactoryPara,MS_U8 u8Value);

//-------------------------------------------------------------------------------------------------
/// Set fixed gain
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AVD_SetAutoFineGainToFixed(void);

//-------------------------------------------------------------------------------------------------
/// Get DSP fine gain value
/// @ingroup Get_VD_Info
/// @return fine gain
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AVD_GetDSPFineGain(void);

//-------------------------------------------------------------------------------------------------
/// Get DSP version
/// @ingroup Get_VD_Info
/// @return DSP version
//-------------------------------------------------------------------------------------------------
extern MS_U16 MDrv_AVD_GetDSPVersion(void);

//-------------------------------------------------------------------------------------------------
/// Shift clock function
/// @ingroup AVD_ToBeRemove
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_ShiftClk(AVD_ATV_CLK_TYPE eShiftMode);

//-------------------------------------------------------------------------------------------------
/// Set shift clock parameters
/// @ingroup Set_VD_Info
/// @param  bEnable   \b IN: shift clock enable
/// @param  eShiftMode   \b IN: shift clock mode
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SetShiftClk(MS_BOOL bEnable,AVD_ATV_CLK_TYPE eShiftMode);

//-------------------------------------------------------------------------------------------------
/// shift clock monitor function
/// @ingroup VD_Task
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_ShifClk_Monitor(void);

//-------------------------------------------------------------------------------------------------
/// SCART monitor function
/// @ingroup VD_Task
//-------------------------------------------------------------------------------------------------
extern void MDrv_AVD_SCART_Monitor(void);

//-------------------------------------------------------------------------------------------------
/// Set AVD STR(resume/suspend) state
/// @ingroup Set_VD_Info
/// @param  u16PowerState   \b IN: STR state
/// @return status
//-------------------------------------------------------------------------------------------------
extern MS_U32 MDrv_AVD_SetPowerState(EN_POWER_MODE u16PowerState);

//-------------------------------------------------------------------------------------------------
/// Get MacroVision detection
/// @ingroup Get_VD_Info
/// @return Macro Vision detection status
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_GetMacroVisionDetect(void);

//-------------------------------------------------------------------------------------------------
/// Get VBI CGMS detection
/// @ingroup Get_VD_Info
/// @return CGMS detection status
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_AVD_GetCGMSDetect(void);

//-------------------------------------------------------------------------------------------------
/// Set Burst Window detection
/// @ingroup Get_VD_Info
//-------------------------------------------------------------------------------------------------
extern void SYMBOL_WEAK MDrv_AVD_SetBurstWinStart(MS_U8 u8BusrtStartPosition);

//-------------------------------------------------------------------------------------------------
/// Check VD lock audio carrier
/// @ingroup Get_VD_Info
/// @return TRUE: lock in audio carrier
/// @return FALSE:unlock in audio carrier
//-------------------------------------------------------------------------------------------------
extern MS_BOOL SYMBOL_WEAK MDrv_AVD_IsLockAudioCarrier(void);
//-------------------------------------------------------------------------------------------------
/// Set Ana Demod
/// @ingroup Get_VD_Info
/// @return TRUE: Demode MSTAR VIF
/// @return FALSE:Demode Normal
//-------------------------------------------------------------------------------------------------
extern void SYMBOL_WEAK MDrv_AVD_SetAnaDemod(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set Hsync Sensitivity For Debug
/// @ingroup Set_VD_Info
/// @param  u8AFEC99   \b IN: set BK_AFEC_99 value
/// @param  u8AFEC9C   \b IN: set BK_AFEC_9C value
//-------------------------------------------------------------------------------------------------
extern void SYMBOL_WEAK MDrv_AVD_SetHsyncSensitivityForDebug(MS_U8 u8AFEC99 ,MS_U8 u8AFEC9C);
#ifdef __cplusplus
}
#endif


#endif // _DRV_AVD_H_
