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
/// @brief  MStar VIF Utopia interface
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup VIF VIF interface
    \ingroup  VIF
   
    \brief 
 
    VIF, in other words, is ATV demodulator. VIF is composed of analog and digital parts. 
    Analog part includes Programmable Gain Amplifier(PGA), mixer and LPF.
    After ADC sampling, low IF signal is digitally processed. 
    First, low IF signal is down-converted to baseband.
    Second, baseband filter pass video content and reject audio carrier.

    <b>Features</b>

    - PGA provides 25~35dB gain (depending on chips), 1dB gain per step.
    - Mixer down-converts video IF frequency (38.9MHz) to low IF frequency (9.548MHz)
    - There are separate internal control lines to provide 10 gain steps for PGA1 and 15 gain steps for PGA2. 
      Each gain step is 1 dB increment. The minimum PGA1 gain is 6dB and the minimum PGA2 gain is 0dB.
    - Gain adjustment
      RF signal strong -> weak: Once tuner gain reaches maximum, PGA gain start increasing.
      RF signal weak -> strong: Decreasing tuner gain first and then decreasing PGA gain for maintaining tuner voltage operating above GAIN_DISTRINUTION_THR. Once PGA gain is minimum, tuner gain start decreasing until minimum.

    <b> VIF Block Diagram: </b> \n
    \image html drvVIF_pic01.png
    \image html drvVIF_pic02.png
    \image html drvVIF_pic03.png

    <b> Operation Code Flow: </b> \n
    -# Prepare VIF setting for each operation
    -# Set and start VIF in command handle
    -# Get Tuner IF output signal
    -# Set setting
    -# Monitor VIF status (FOE, Lock status...etc)
    -# VIF done by polling

    \defgroup VIF VIF modules
    \ingroup  VIF
    \defgroup VIF_Basic VIF Basic
    \ingroup  VIF
    \defgroup VIF_Task VIF Task
    \ingroup  VIF
*/


#ifndef _DRVVIF_H_
#define _DRVVIF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#if 0 // move to drvvif.c
//****************************************************************************
// VIF lib Changelist
//****************************************************************************
#define MSIF_VIF_LIB_CODE     {'V','I','F','_'}
#define MSIF_VIF_LIBVER       {'1','7'}
#define MSIF_VIF_BUILDNUM     {'0','5'}
#define MSIF_VIF_CHANGELIST   {'0','0','6','1','8','0','0','4'}
#define MSIF_VIF_DASH          {'_'}
#define MSIF_VIF_DATE          {'1','3','0','8','1','4'}   // yymmdd

#define VIF_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_VIF_LIB_CODE,                  /* IP__                                             */  \
    MSIF_VIF_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_VIF_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_VIF_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS
#endif
//****************************************************************************
// Public attributes.
//****************************************************************************

#ifdef _DRVVIF_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifndef BOOL
#define BOOL MS_BOOL
#endif
#ifndef BYTE
#define BYTE MS_U8
#endif
#ifndef WORD
#define WORD MS_U16
#endif
#ifndef DWORD
#define DWORD MS_U32
#endif
#ifndef U8
#define U8 MS_U8
#endif
#ifndef U16
#define U16 MS_U16
#endif
#ifndef U32
#define U32 MS_U32
#endif
#ifndef VIRT
#define VIRT MS_VIRT
#endif

typedef enum
{
    afcBELOW_MINUS_187p5KHz = 0x07,
    afcMINUS_162p5KHz = 0x06,
    afcMINUS_137p5KHz = 0x05,
    afcMINUS_112p5KHz = 0x04,
    afcMINUS_87p5KHz = 0x03,
    afcMINUS_62p5KHz = 0x02,
    afcMINUS_37p5KHz = 0x01,
    afcMINUS_12p5KHz = 0x00,
    afcPLUS_12p5KHz = 0x0F,
    afcPLUS_37p5KHz = 0x0E,
    afcPLUS_62p5KHz = 0x0D,
    afcPLUS_87p5KHz = 0x0C,
    afcPLUS_112p5KHz = 0x0B,
    afcPLUS_137p5KHz = 0x0A,
    afcPLUS_162p5KHz = 0x09,
    afcABOVE_PLUS_187p5KHz = 0x08   //<NHK_070630_eAFC>
} eAFC;


typedef enum
{
    E_AFC_BELOW_MINUS_187p5KHz      = 0x07,
    E_AFC_MINUS_162p5KHz            = 0x06,
    E_AFC_MINUS_137p5KHz            = 0x05,
    E_AFC_MINUS_112p5KHz            = 0x04,
    E_AFC_MINUS_87p5KHz             = 0x03,
    E_AFC_MINUS_62p5KHz             = 0x02,
    E_AFC_MINUS_37p5KHz             = 0x01,
    E_AFC_MINUS_12p5KHz             = 0x00,
    E_AFC_PLUS_12p5KHz              = 0x0F,
    E_AFC_PLUS_37p5KHz              = 0x0E,
    E_AFC_PLUS_62p5KHz              = 0x0D,
    E_AFC_PLUS_87p5KHz              = 0x0C,
    E_AFC_PLUS_112p5KHz             = 0x0B,
    E_AFC_PLUS_137p5KHz             = 0x0A,
    E_AFC_PLUS_162p5KHz             = 0x09,
    E_AFC_ABOVE_PLUS_187p5KHz       = 0x08,
    E_AFC_OUT_OF_AFCWIN             = 0x10,
} AFC;

typedef enum
{
    VIF_IF_FREQ_INVALID,
    VIF_IF_FREQ_B,              // 38.90 MHz
    VIF_IF_FREQ_G,              // 38.90 MHz
    VIF_IF_FREQ_I,              // 38.90 MHz
    VIF_IF_FREQ_DK,             // 38.90 MHz
    VIF_IF_FREQ_L,              // 38.90 MHz
    VIF_IF_FREQ_L_PRIME,        // 33.90 MHz
    VIF_IF_FREQ_MN,             // 45.75 MHz
    VIF_IF_FREQ_J,              // 58.75 MHz
    VIF_IF_FREQ_PAL_38,         // 38.00 MHz
    VIF_IF_FREQ_MAX_IF_FREQ,
    VIF_IF_DIGITAL_MODE
} VIF_IF_FREQ;

typedef enum
{
    VIF_SOUND_B,                    // B_STEREO_A2
    VIF_SOUND_B_NICAM,      // B_MONO_NICAM
    VIF_SOUND_GH,                 //GH_STEREO_A2
    VIF_SOUND_GH_NICAM,   // GH_MONO_NICAM
    VIF_SOUND_I,
    VIF_SOUND_DK1,              // DK1_STEREO_A2
    VIF_SOUND_DK2,              // DK2_STEREO_A2
    VIF_SOUND_DK3,              // DK3_STEREO_A2
    VIF_SOUND_DK_NICAM,  // DK_MONO_NICAM
    VIF_SOUND_L,
    VIF_SOUND_LL,
    VIF_SOUND_MN,
    VIF_SOUND_NUMS
}VIFSoundSystem;

typedef enum
{
    IF_FREQ_3395, // SECAM-L'
    IF_FREQ_3800, // PAL
    IF_FREQ_3890, // PAL
    IF_FREQ_3950, // only for PAL-I
    IF_FREQ_4575, // NTSC-M/N
    IF_FREQ_5875, // NTSC-M/N
    IF_FREQ_NUMS
}IfFrequencyType;

typedef enum
{
    FREQ_VHF_L,
    FREQ_VHF_H,
    FREQ_UHF,
    FREQ_RANGE_NUMS
}FrequencyBand;

typedef struct DLL_PACKED _VIFInitialIn
{
    BYTE VifTop;
    BYTE VifIfBaseFreq;
    BYTE VifTunerStepSize;
    BYTE VifSawArch;
    WORD VifVgaMaximum;
    WORD VifVgaMinimum;
    WORD GainDistributionThr;
    BYTE VifAgcVgaBase;
    BYTE VifAgcVgaOffs;
    BYTE VifAgcRefNegative;
    BYTE VifAgcRefPositive;
    BYTE VifDagc1Ref;
    BYTE VifDagc2Ref;
    WORD VifDagc1GainOv;
    WORD VifDagc2GainOv;
    BYTE VifCrKf1;
    BYTE VifCrKp1;
    BYTE VifCrKi1;
    BYTE VifCrKp2;
    BYTE VifCrKi2;
    BYTE VifCrKp;
    BYTE VifCrKi;
    WORD VifCrLockThr;
    WORD VifCrThr;
    DWORD VifCrLockNum;
    DWORD VifCrUnlockNum;
    WORD VifCrPdErrMax;
    BOOL VifCrLockLeakySel;
    BOOL VifCrPdX2;
    BOOL VifCrLpfSel;
    BOOL VifCrPdModeSel;
    BOOL VifCrKpKiAdjust;
    BYTE VifCrKpKiAdjustGear;
    BYTE VifCrKpKiAdjustThr1;
    BYTE VifCrKpKiAdjustThr2;
    BYTE VifCrKpKiAdjustThr3;
    BOOL VifDynamicTopAdjust;
    BYTE VifDynamicTopMin;
    BOOL VifAmHumDetection;
    BOOL VifClampgainClampSel;
    BYTE VifClampgainSyncbottRef;
    BYTE VifClampgainSyncheightRef;
    BYTE VifClampgainKc;
    BYTE VifClampgainKg;
    BOOL VifClampgainClampOren;
    BOOL VifClampgainGainOren;
    WORD VifClampgainClampOvNegative;
    WORD VifClampgainGainOvNegative;
    WORD VifClampgainClampOvPositive;
    WORD VifClampgainGainOvPositive;
    BYTE VifClampgainClampMin;
    BYTE VifClampgainClampMax;
    BYTE VifClampgainGainMin;
    BYTE VifClampgainGainMax;
    WORD VifClampgainPorchCnt;
    BYTE VifPeakingFilterB_VHF_L;
    BYTE VifYcDelayFilterB_VHF_L;
    BYTE VifGroupDelayFilterB_VHF_L;
    BYTE VifPeakingFilterGH_VHF_L;
    BYTE VifYcDelayFilterGH_VHF_L;
    BYTE VifGroupDelayFilterGH_VHF_L;
    BYTE VifPeakingFilterDK_VHF_L;
    BYTE VifYcDelayFilterDK_VHF_L;
    BYTE VifGroupDelayFilterDK_VHF_L;
    BYTE VifPeakingFilterI_VHF_L;
    BYTE VifYcDelayFilterI_VHF_L;
    BYTE VifGroupDelayFilterI_VHF_L;
    BYTE VifPeakingFilterL_VHF_L;
    BYTE VifYcDelayFilterL_VHF_L;
    BYTE VifGroupDelayFilterL_VHF_L;
    BYTE VifPeakingFilterLL_VHF_L;
    BYTE VifYcDelayFilterLL_VHF_L;
    BYTE VifGroupDelayFilterLL_VHF_L;
    BYTE VifPeakingFilterMN_VHF_L;
    BYTE VifYcDelayFilterMN_VHF_L;
    BYTE VifGroupDelayFilterMN_VHF_L;
    BYTE VifPeakingFilterB_VHF_H;
    BYTE VifYcDelayFilterB_VHF_H;
    BYTE VifGroupDelayFilterB_VHF_H;
    BYTE VifPeakingFilterGH_VHF_H;
    BYTE VifYcDelayFilterGH_VHF_H;
    BYTE VifGroupDelayFilterGH_VHF_H;
    BYTE VifPeakingFilterDK_VHF_H;
    BYTE VifYcDelayFilterDK_VHF_H;
    BYTE VifGroupDelayFilterDK_VHF_H;
    BYTE VifPeakingFilterI_VHF_H;
    BYTE VifYcDelayFilterI_VHF_H;
    BYTE VifGroupDelayFilterI_VHF_H;
    BYTE VifPeakingFilterL_VHF_H;
    BYTE VifYcDelayFilterL_VHF_H;
    BYTE VifGroupDelayFilterL_VHF_H;
    BYTE VifPeakingFilterLL_VHF_H;
    BYTE VifYcDelayFilterLL_VHF_H;
    BYTE VifGroupDelayFilterLL_VHF_H;
    BYTE VifPeakingFilterMN_VHF_H;
    BYTE VifYcDelayFilterMN_VHF_H;
    BYTE VifGroupDelayFilterMN_VHF_H;
    BYTE VifPeakingFilterB_UHF;
    BYTE VifYcDelayFilterB_UHF;
    BYTE VifGroupDelayFilterB_UHF;
    BYTE VifPeakingFilterGH_UHF;
    BYTE VifYcDelayFilterGH_UHF;
    BYTE VifGroupDelayFilterGH_UHF;
    BYTE VifPeakingFilterDK_UHF;
    BYTE VifYcDelayFilterDK_UHF;
    BYTE VifGroupDelayFilterDK_UHF;
    BYTE VifPeakingFilterI_UHF;
    BYTE VifYcDelayFilterI_UHF;
    BYTE VifGroupDelayFilterI_UHF;
    BYTE VifPeakingFilterL_UHF;
    BYTE VifYcDelayFilterL_UHF;
    BYTE VifGroupDelayFilterL_UHF;
    BYTE VifPeakingFilterLL_UHF;
    BYTE VifYcDelayFilterLL_UHF;
    BYTE VifGroupDelayFilterLL_UHF;
    BYTE VifPeakingFilterMN_UHF;
    BYTE VifYcDelayFilterMN_UHF;
    BYTE VifGroupDelayFilterMN_UHF;
    BYTE ChinaDescramblerBox;
    BYTE VifDelayReduce;
    BOOL VifOverModulation;
    BOOL VifOverModulationDetect;
    BOOL VifACIDetect;
    BOOL VifSeriousACIDetect;
    BYTE VifACIAGCREF;
    BYTE VifADCOverflowAGCREF;
    BYTE VifChanelScanAGCREF;
    BYTE VifACIDetTHR1;
    BYTE VifACIDetTHR2;
    BYTE VifACIDetTHR3;
    BYTE VifACIDetTHR4;
    BYTE VifFreqBand;
    WORD VifSos21FilterC0;
    WORD VifSos21FilterC1;
    WORD VifSos21FilterC2;
    WORD VifSos21FilterC3;
    WORD VifSos21FilterC4;
    WORD VifSos22FilterC0;
    WORD VifSos22FilterC1;
    WORD VifSos22FilterC2;
    WORD VifSos22FilterC3;
    WORD VifSos22FilterC4;
    WORD VifSos31FilterC0;
    WORD VifSos31FilterC1;
    WORD VifSos31FilterC2;
    WORD VifSos31FilterC3;
    WORD VifSos31FilterC4;
    WORD VifSos32FilterC0;
    WORD VifSos32FilterC1;
    WORD VifSos32FilterC2;
    WORD VifSos32FilterC3;
    WORD VifSos32FilterC4;
    BYTE VifTunerType;
    DWORD VifCrRate_B;
    BOOL VifCrInvert_B;
    DWORD VifCrRate_GH;
    BOOL VifCrInvert_GH;
    DWORD VifCrRate_DK;
    BOOL VifCrInvert_DK;
    DWORD VifCrRate_I;
    BOOL VifCrInvert_I;
    DWORD VifCrRate_L;
    BOOL VifCrInvert_L;
    DWORD VifCrRate_LL;
    BOOL VifCrInvert_LL;
    DWORD VifCrRate_MN;
    BOOL VifCrInvert_MN;
    BYTE VifReserve;
}VIFInitialIn;

typedef enum
{
    VIF_PARA_USER_FILTER,                                          // VIFUserFilter
    VIF_PARA_TOP,                                                         // DWORD
    VIF_PARA_LEAKY_SEL,                                             // DWORD
    VIF_PARA_K_SEL,                                                     // DWORD
    VIF_PARA_GET_VERSION,                                       // U8[30] return
    VIF_PARA_SET_BYPASS_PEAKING_SOS21,          // DWORD
    VIF_PARA_GET_PGA2_LEVEL,                                   // BYTE return
    VIF_PARA_GET_IF,			// WORD return
    VIF_PARA_GET_K_SEL,			// BYTE return
    VIF_PARA_SET_HW_KPKI,       // DWORD
    VIF_PARA_SET_TAGC_ODMODE,    // DWORD
    VIF_PARA_SET_NOTCH_A1A2,    // VIFNotchA1A2: please set user notch after setting sound system to overwrite default values
    VIF_PARA_SET_SOS_1112,        // VIFSOS1112: please set user SOS after setting sound system to overwrite default values
    VIF_PARA_GET_PD_INV,    // BYTE return
    VIF_PARA_SET_PD_INV,    // DWORD
    VIF_PARA_GET_VGA, //WORD return
    VIF_PARA_SET_CLAMPGAIN_GAIN_OV_POS,    // DWORD
    VIF_PARA_SET_CLAMPGAIN_GAIN_OV_NEG,    // DWORD
    VIF_PARA_SET_AGC_K, //DWORD;     B3[0]: enable user steady agc_k, B2[2~0]: user steady agc_value, B1[0]: enable user non steady agc_k, B0[2~0]: user non steady agc_value
    VIF_PARA_SET_AGC_REF, //DWORD
    VIF_PARA_SET_IF_RATE_FM,  //DWORD
    VIF_PARA_SET_CR_RATE_FM, //DWORD
    VIF_PARA_SET_DESCRAMBLERBOX_DELAY, //DWORD
    VIF_PARA_SET_SSC_ENABLE, //DWORD
    VIF_PARA_SET_DISABLE_ADAGC,
    VIF_PARA_SET_ENABLE_SOS33,
    VIF_PARA_GROUP_NUMS
}VIF_PARA_GROUP;


typedef enum
{
    PK_START=1,
    PK_B_VHF_L=PK_START,  //PeakingFilterB_VHF_L,
    PK_GH_VHF_L, //PeakingFilterGH_VHF_L,
    PK_DK_VHF_L, //PeakingFilterDK_VHF_L,
    PK_I_VHF_L,  //PeakingFilterI_VHF_L,
    PK_L_VHF_L,  //PeakingFilterL_VHF_L,
    PK_LL_VHF_L, //PeakingFilterLL_VHF_L,
    PK_MN_VHF_L, //PeakingFilterMN_VHF_L,
    PK_B_VHF_H,  //PeakingFilterB_VHF_H,
    PK_GH_VHF_H, //PeakingFilterGH_VHF_H,
    PK_DK_VHF_H, //PeakingFilterDK_VHF_H,
    PK_I_VHF_H,  //PeakingFilterI_VHF_H,
    PK_L_VHF_H,  //PeakingFilterL_VHF_H,
    PK_LL_VHF_H, //PeakingFilterLL_VHF_H,
    PK_MN_VHF_H, //PeakingFilterMN_VHF_H,
    PK_B_UHF,  //PeakingFilterB_UHF,
    PK_GH_UHF, //PeakingFilterGH_UHF,
    PK_DK_UHF, //PeakingFilterDK_UHF,
    PK_I_UHF,  //PeakingFilterI_UHF,
    PK_L_UHF,  //PeakingFilterL_UHF,
    PK_LL_UHF, //PeakingFilterLL_UHF,
    PK_MN_UHF, //PeakingFilterMN_UHF,
    PK_END=PK_MN_UHF,

    YC_START,
    YC_B_VHF_L=YC_START,  //YcDelayFilterB_VHF_L,
    YC_GH_VHF_L, //YcDelayFilterGH_VHF_L,
    YC_DK_VHF_L, //YcDelayFilterDK_VHF_L,
    YC_I_VHF_L,  //YcDelayFilterI_VHF_L,
    YC_L_VHF_L,  //YcDelayFilterL_VHF_L,
    YC_LL_VHF_L, //YcDelayFilterLL_VHF_L,
    YC_MN_VHF_L, //YcDelayFilterMN_VHF_L,
    YC_B_VHF_H,  //YcDelayFilterB_VHF_H,
    YC_GH_VHF_H, //YcDelayFilterGH_VHF_H,
    YC_DK_VHF_H, //YcDelayFilterDK_VHF_H,
    YC_I_VHF_H,  //YcDelayFilterI_VHF_H,
    YC_L_VHF_H,  //YcDelayFilterL_VHF_H,
    YC_LL_VHF_H, //YcDelayFilterLL_VHF_H,
    YC_MN_VHF_H, //YcDelayFilterMN_VHF_H,
    YC_B_UHF,  //YcDelayFilterB_UHF,
    YC_GH_UHF, //YcDelayFilterGH_UHF,
    YC_DK_UHF, //YcDelayFilterDK_UHF,
    YC_I_UHF,  //YcDelayFilterI_UHF,
    YC_L_UHF,  //YcDelayFilterL_UHF,
    YC_LL_UHF, //YcDelayFilterLL_UHF,
    YC_MN_UHF, //YcDelayFilterMN_UHF,
    YC_END=YC_MN_UHF,

    GP_START,
    GP_B_VHF_L=GP_START,  //GroupDelayFilterB_VHF_L,
    GP_GH_VHF_L, //GroupDelayFilterGH_VHF_L,
    GP_DK_VHF_L, //GroupDelayFilterDK_VHF_L,
    GP_I_VHF_L,  //GroupDelayFilterI_VHF_L,
    GP_L_VHF_L,  //GroupDelayFilterL_VHF_L,
    GP_LL_VHF_L, //GroupDelayFilterLL_VHF_L,
    GP_MN_VHF_L, //GroupDelayFilterMN_VHF_L,
    GP_B_VHF_H,  //GroupDelayFilterB_VHF_H,
    GP_GH_VHF_H, //GroupDelayFilterGH_VHF_H,
    GP_DK_VHF_H, //GroupDelayFilterDK_VHF_H,
    GP_I_VHF_H,  //GroupDelayFilterI_VHF_H,
    GP_L_VHF_H,  //GroupDelayFilterL_VHF_H,
    GP_LL_VHF_H, //GroupDelayFilterLL_VHF_H,
    GP_MN_VHF_H, //GroupDelayFilterMN_VHF_H,
    GP_B_UHF,  //GroupDelayFilterB_UHF,
    GP_GH_UHF, //GroupDelayFilterGH_UHF,
    GP_DK_UHF, //GroupDelayFilterDK_UHF,
    GP_I_UHF,  //GroupDelayFilterI_UHF,
    GP_L_UHF,  //GroupDelayFilterL_UHF,
    GP_LL_UHF, //GroupDelayFilterLL_UHF,
    GP_MN_UHF, //GroupDelayFilterMN_UHF,
    GP_END=GP_MN_UHF,

    VIF_USER_FILTER_SELECT_NUMS
}VIF_USER_FILTER_SELECT;

typedef struct DLL_PACKED _VIFUserFilter
{
    WORD VifSos21FilterC0;
    WORD VifSos21FilterC1;
    WORD VifSos21FilterC2;
    WORD VifSos21FilterC3;
    WORD VifSos21FilterC4;
    WORD VifSos22FilterC0;
    WORD VifSos22FilterC1;
    WORD VifSos22FilterC2;
    WORD VifSos22FilterC3;
    WORD VifSos22FilterC4;
    WORD VifSos31FilterC0;
    WORD VifSos31FilterC1;
    WORD VifSos31FilterC2;
    WORD VifSos31FilterC3;
    WORD VifSos31FilterC4;
    WORD VifSos32FilterC0;
    WORD VifSos32FilterC1;
    WORD VifSos32FilterC2;
    WORD VifSos32FilterC3;
    WORD VifSos32FilterC4;
    //WORD VifSos33FilterC0;
    //WORD VifSos33FilterC1;
    //WORD VifSos33FilterC2;
    //WORD VifSos33FilterC3;
    //WORD VifSos33FilterC4;
    BYTE VifUserPeakingFilterSelect;
    BYTE VifUserYcDelayFilterSelect;
    BYTE VifUserGroupDelayFilterSelect;
}VIFUserFilter;

typedef struct DLL_PACKED _VIFNotchA1A2
{
    WORD Vif_N_A1_C0;
    WORD Vif_N_A1_C1;
    WORD Vif_N_A1_C2;
    WORD Vif_N_A2_C0;
    WORD Vif_N_A2_C1;
    WORD Vif_N_A2_C2;
}VIFNotchA1A2;

typedef struct DLL_PACKED _VIFSOS1112
{
    WORD Vif_SOS_11_C0;
    WORD Vif_SOS_11_C1;
    WORD Vif_SOS_11_C2;
    WORD Vif_SOS_11_C3;
    WORD Vif_SOS_11_C4;
    WORD Vif_SOS_12_C0;
    WORD Vif_SOS_12_C1;
    WORD Vif_SOS_12_C2;
    WORD Vif_SOS_12_C3;
    WORD Vif_SOS_12_C4;
}VIFSOS1112;

typedef struct DLL_PACKED _VIFSOS33
{
    WORD Vif_SOS_33_C0;
    WORD Vif_SOS_33_C1;
    WORD Vif_SOS_33_C2;
    WORD Vif_SOS_33_C3;
    WORD Vif_SOS_33_C4;
}VIFSOS33;

//****************************************************************************
// Public functions.
//****************************************************************************
//-------------------------------------------------------------------------------------------------
/// Check VIF version
/// @ingroup VIF_BASIC   
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_Version(void);
//-------------------------------------------------------------------------------------------------
/// VIF Set Clock
/// @ingroup VIF_BASIC
/// @param  bEnable   \b IN: 0:means enable
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_SetClock(BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Initialize VIF setting (any register access should be after this function)
/// @ingroup VIF_BASIC   
/// @param  pVIF_InitData     \b IN: init data 
/// @param  u32InitDataLen  \b IN: init data size
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen);
//-------------------------------------------------------------------------------------------------
/// VIF Software Reset
/// @ingroup VIF_Basic
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_Reset(void);
//-------------------------------------------------------------------------------------------------
/// VIF Software Exit
/// @ingroup VIF_Basic
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_Exit(void);
//-------------------------------------------------------------------------------------------------
/// VIF Handler (monitor all VIF functions)
/// @ingroup VIF_Basic
/// @param  bAutoScan     \b IN: init data 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_Handler(BOOL bAutoScan);
//-------------------------------------------------------------------------------------------------
/// VIF Set Sound System
/// @ingroup VIF_Basic
/// @param  ucSoundSystem     \b IN: VIFSoundSystems
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem);
//-------------------------------------------------------------------------------------------------
/// VIF Set IF Frequnecy 
/// @ingroup VIF_Basic
/// @param  u16IfFreq     \b IN: IF Frequency
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_SetIfFreq(IfFrequencyType u16IfFreq);
//-------------------------------------------------------------------------------------------------
/// VIF Read CR FOE 
/// @ingroup VIF_Basic
/// @return TRUE : CR_FOE
/// @return FALSE : 0, fail
//-------------------------------------------------------------------------------------------------
INTERFACE U8 DRV_VIF_Read_CR_FOE(void);
//-------------------------------------------------------------------------------------------------
/// VIF Read CR Lock Status
/// @ingroup VIF_Basic
/// @return TRUE : CR_LOCK_STATUS
/// @return FALSE : 0, fail
//-------------------------------------------------------------------------------------------------
INTERFACE U8 DRV_VIF_Read_CR_LOCK_STATUS(void);
//-------------------------------------------------------------------------------------------------
/// VIF Bypass DBB Audio Filter (A_DAGC_SEL)
/// @ingroup VIF_Task
/// @param  bEnable     \b IN: bEnable
/// @return TRUE : 1, input from a_lpf_up
/// @return FALSE : 0, input from a_sos
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_BypassDBBAudioFilter(BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// VIF Set Frquency Band
/// @ingroup VIF_Basic
/// @param  u8FreqBand     \b IN: frequency band
/// @return TRUE : VifFreqBand
/// @return FALSE : 0
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_SetFreqBand(FrequencyBand u8FreqBand);
//-------------------------------------------------------------------------------------------------
/// VIF Get Input Level Indicator
/// @ingroup VIF_Task
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
INTERFACE BOOL DRV_VIF_GetInputLevelIndicator(void);
//-------------------------------------------------------------------------------------------------
/// VIF Set Parameters
/// @ingroup VIF_Task
/// @param  paraGroup     \b IN: Parameters Group
/// @param  pVIF_Para     \b IN: Parameters 
/// @param  u32DataLen     \b IN: Data Length
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
INTERFACE BOOL DRV_VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen);
//-------------------------------------------------------------------------------------------------
/// VIF Set Shift Clock
/// @ingroup VIF_Task
/// @param  VifShiftClk     \b IN: 0 (42MHz, 140MHz), 1(44.4MHz, 148MHz), 2(43.2MHz, 142MHz)
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_ShiftClk(BYTE VifShiftClk);
//-------------------------------------------------------------------------------------------------
/// VIF Set Power State
/// @ingroup VIF_Task
/// @param  u16PowerState     \b IN: Power State
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
INTERFACE MS_U32 MDrv_VIF_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// VIF Write Byte
/// @ingroup VIF_Basic
/// @param  u32Reg     \b IN: Register address
/// @param  u8Val     \b IN: Value
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
INTERFACE void DRV_VIF_WriteByte(U32 u32Reg, U8 u8Val );
//-------------------------------------------------------------------------------------------------
/// VIF Read Byte
/// @ingroup VIF_Basic
/// @param  u32Reg     \b IN: Register address
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
INTERFACE U8 DRV_VIF_ReadByte(U32 u32Reg );
#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif //_DRVVIF_H_
