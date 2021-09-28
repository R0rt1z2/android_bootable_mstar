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
#ifndef __MSDLC_H
#define __MSDLC_H

#ifdef _MSDLC_C_
  #define _MSDLCDEC_
#else
  #define _MSDLCDEC_ extern
#endif
#include "UFO.h"
#if ENABLE_DLC
//------------------------------------------------------------------------
// Input:
//      wWidth: Output image Width
//      wHeight: Output iamge Height
//------------------------------------------------------------------------
void msDlcInit(WORD wWidth, WORD wHeight);
void msDlcInitWithCurve( WORD wHStart, WORD wHEnd, WORD wVStart, WORD wVEnd );


//------------------------------------------------------------------------
// Put this function at main loop
// This function will use 33ms for 5xxx/6xxx version chip
// This function will use 22ms for 5xxxA/6xxxA version chip
//------------------------------------------------------------------------
void msDlcHandler(BOOL bWindow);
BOOL msGetHistogramHandler(BOOL bWindow);
//void msDlcStaticCurveOnOff(BOOL bEnable);

void msDlcEnableSetDLCCurveBoth(BOOL bEnable);

//------------------------------------------------------------------------
// Input:
//      bWindow: 0 main window, 1 sub window
//      bSwitch: 0 is OFF, 1 is ON
//------------------------------------------------------------------------
void msDlcOnOff(BOOL bSwitch, BOOL bWindow);
void msDlcSpeedupTrigger(BYTE ucLoop);
void msDlcSetBleOnOff(BOOL bSwitch);

//------------------------------------------------------------------------
// CGC
//------------------------------------------------------------------------
void msDlc_CGC_Init(void);
void msDlc_CGC_ReInit(void);
#if defined(UFO_PUBLIC_HEADER_500)
#define MDrv_msDlc_CGC_SetGain  msDlc_CGC_SetGain
#endif
void msDlc_CGC_SetGain(BYTE u8CGain);
void msDlc_CGC_Handler(void);

//------------------------------------------------------------------------
// For Debug Command
//------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500)
#define MDrv_msDLC_ParametersTool  msDLC_ParametersTool
#endif
void msDLC_ParametersTool(BYTE *pCmdBuff);
//------------------------------------------------------------------------
// LIB version check
//------------------------------------------------------------------------
WORD msDlcLibVersionCheck(void);
WORD msDlcCGCLibVersionCheck(void);
//------------------------------------------------------------------------
//------------------------------------------------------------------------

typedef struct
{
    // Default luma curve
    BYTE ucLumaCurve[16];
    BYTE ucLumaCurve2_a[16];
    BYTE ucLumaCurve2_b[16];
    BYTE ucLumaCurve2[16];

    BYTE u8_L_L_U; // default value: 10
    BYTE u8_L_L_D; // default value: 10
    BYTE u8_L_H_U; // default value: 10
    BYTE u8_L_H_D; // default value: 10
    BYTE u8_S_L_U; // default value: 128 (0x80)
    BYTE u8_S_L_D; // default value: 128 (0x80)
    BYTE u8_S_H_U; // default value: 128 (0x80)
    BYTE u8_S_H_D; // default value: 128 (0x80)
    BYTE ucCGCCGain_offset; // -31 ~ 31 (bit7 = minus, ex. 0x88 => -8)
    BYTE ucCGCChroma_GainLimitH; // 0x00~0x6F
    BYTE ucCGCChroma_GainLimitL; // 0x00~0x10
    BYTE ucCGCYCslope; // 0x01~0x20
    BYTE ucCGCYth; // 0x01
    BYTE ucDlcPureImageMode; // Compare difference of max and min bright
    BYTE ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    BYTE ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
    BYTE ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15 => 0.1 ~ 1.5 enable still curve
    BYTE ucDlcFastAlphaBlending; // min 17 ~ max 32
    BYTE ucDlcSlowEvent; // some event is triggered, DLC must do slowly // for PIP On/Off, msMultiPic.c
    BYTE ucDlcTimeOut; // for IsrApp.c
    BYTE ucDlcFlickAlphaStart; // for force to do fast DLC in a while
    BYTE ucDlcYAvgThresholdH; // default value: 128
    BYTE ucDlcYAvgThresholdL; // default value: 0
    BYTE ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
    BYTE ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
    BYTE bCGCCGainCtrl : 1; // 1: enable; 0: disable
    BYTE bEnableBLE : 1; // 1: enable; 0: disable
    BYTE bEnableWLE : 1; // 1: enable; 0: disable
    BYTE ucDlcYAvgThresholdM;
    BYTE ucDlcCurveMode;
    BYTE ucDlcCurveModeMixAlpha;
    BYTE ucDlcAlgorithmMode;
    BYTE ucDlcHistogramLimitCurve[17];
    BYTE ucDlcSepPointH;
    BYTE ucDlcSepPointL;
    WORD uwDlcBleStartPointTH;
    WORD uwDlcBleEndPointTH;
    BYTE ucDlcCurveDiff_L_TH;
    BYTE ucDlcCurveDiff_H_TH;
    WORD uwDlcBLESlopPoint_1;
    WORD uwDlcBLESlopPoint_2;
    WORD uwDlcBLESlopPoint_3;
    WORD uwDlcBLESlopPoint_4;
    WORD uwDlcBLESlopPoint_5;
    WORD uwDlcDark_BLE_Slop_Min;
    BYTE ucDlcCurveDiffCoringTH;
    BYTE ucDlcAlphaBlendingMin;
    BYTE ucDlcAlphaBlendingMax;
    BYTE ucDlcFlicker_alpha;
    BYTE ucDlcYAVG_L_TH;
    BYTE ucDlcYAVG_H_TH;
    BYTE ucDlcDiffBase_L;
    BYTE ucDlcDiffBase_M;
    BYTE ucDlcDiffBase_H;
    BYTE ucLMaxThreshold;
    BYTE ucLMinThreshold;
    BYTE ucLMaxCorrection;
    BYTE ucLMinCorrection;
    BYTE ucRMaxThreshold;
    BYTE ucRMinThreshold;
    BYTE ucRMaxCorrection;
    BYTE ucRMinCorrection;
    BYTE ucAllowLoseContrast;
    MS_U8 u8Dlc_Mode;
    MS_U8 u8Tmo_Mode;
}StuDlc_FinetuneParamaters;


extern XDATA StuDlc_FinetuneParamaters g_DlcParameters;

#define DLC_PURE_IMAGE_DLC_CURVE        0L
#define DLC_PURE_IMAGE_LINEAR_CURVE     1L
#define DLC_PURE_IMAGE_DO_NOTHING       2L

extern XDATA WORD g_wLumaHistogram32H[32];

extern XDATA BYTE g_ucHistogramMax, g_ucHistogramMin;
extern XDATA WORD g_wLumiAverageTemp;
extern XDATA BYTE g_ucTable[16];
extern XDATA char* g_pInitLumaCurve;
extern XDATA WORD g_wLumiTotalCount;

#define DLC_EVENT_SLOW_ENABLE   40

#if (ENABLE_DLC == DLC_8SEG_HIS_MODE)
extern XDATA WORD g_wLumaHistogram8H[8];
#endif

void msDlc_SetCaptureRange(MS_U16 wHStart,MS_U16 wHEnd, MS_U16 wVStart, MS_U16 wVEnd);



#endif
#endif
