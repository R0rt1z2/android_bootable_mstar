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
#ifndef _PANEL_H_
#define _PANEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "apiDAC.h"
#include "apiHDMITx.h"

#include "apiPNL.h"


#define INTERFACE extern

#define ENABLE_OD      0
#define ENABLE_MFC     0
////////////////////////////////////////////////////
// Include the real header file
////////////////////////////////////////////////////


////////////////////////////////////////////////////
//      Type define
////////////////////////////////////////////////////

typedef enum{
    PANEL_RES_MIN = 0,

    PANEL_SEC32_LE32A_FULLHD = PANEL_RES_MIN,
    // For Normal LVDS panel
    PANEL_RES_SXGA          = 1,            // 1280x1024, Pnl_AU17_EN05_SXGA
    PANEL_RES_WXGA          = 2,            // 1366x768, Pnl_AU20_T200XW02_WXGA,
    PANEL_RES_WXGA_PLUS     = 3,            // 1440x900, Pnl_CMO19_M190A1_WXGA, Pnl_AU19PW01_WXGA
    PANEL_RES_WSXGA         = 4,            // 1680x1050, Pnl_AU20_M201EW01_WSXGA,
    PANEL_RES_FULL_HD       = 5,            // 1920x1080, Pnl_AU37_T370HW01_HD, Pnl_CMO216H1_L01_HD.h

    // For DAC/HDMI Tx output
    PANEL_DACOUT_480I       = 6,            // just for U3 Dac output 480i timing usage
    PANEL_DACOUT_480P       = 7,            // just for U3 Dac output 480p timing usage
    PANEL_DACOUT_576I       = 8,            // just for U3 Dac output 576i timing usage
    PANEL_DACOUT_576P       = 9,            // just for U3 Dac output 576p timing usage
    PANEL_DACOUT_720P_50    = 10,           // just for U3 Dac output 720p timing usage
    PANEL_DACOUT_720P_60    = 11,           // just for U3 Dac output 720p timing usage
    PANEL_DACOUT_1080I_50   = 12,           // just for U3 Dac output 1080i timing usage
    PANEL_DACOUT_1080I_60   = 13,           // just for U3 Dac output 1080i timing usage
    PANEL_DACOUT_1080P_24   = 14,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_25   = 15,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_30   = 16,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_50   = 17,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_60   = 18,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_640X480P   = 19,

    Panel_CMO_CMO260J2_WUXGA,           // 1920*1200
#if ENABLE_MFC
    PANEL_RES_URSA,
    PANEL_URSA_RES_FULL_HD_120Hz = PANEL_RES_URSA,
    PANEL_URSA_RES_FULL_HD_60Hz,
#endif
    PANEL_RES_MAX_NUM,
}PANEL_RESOLUTION_TYPE;

////////////////////////////////////////////////////
//      global variable
////////////////////////////////////////////////////
#define PANEL_POWER_VCC         0x01
#define PANEL_POWER_LIGHT_ON    0x02
#define PANEL_POWER_BLUESCREEN  0x04

#define PANEL_DEFAULT_TYPE_SEL  PANEL_DACOUT_720P_50
//#define PANEL_DEFAULT_TYPE_SEL  PANEL_DACOUT_480I
INTERFACE PANEL_RESOLUTION_TYPE g_PNL_TypeSel ;
////////////////////////////////////////////////////
//      Functions
////////////////////////////////////////////////////

/*
*   below are customized functions,
*
*
*
*/


/*
*   Transfer panel select value into a PanelType structure,
*   which informed XC_PNL_OBJ which panel used.
*
*   used it with MApi_PNL_Init to initialzed a panel.
*/
INTERFACE PanelType* MApi_XC_GetPanelSpec(PANEL_RESOLUTION_TYPE enResolutionType);

extern E_OUTPUT_VIDEO_TIMING_TYPE MApi_XC_GetDACType(PANEL_RESOLUTION_TYPE enResolutionType);
extern HDMITX_VIDEO_TIMING MApi_XC_GetHDMIType(PANEL_RESOLUTION_TYPE enResolutionType);
//extern MS_COMPONENT_OUTPUT_TYPE MApi_XC_GetComponentType(PANEL_RESOLUTION_TYPE enResolutionType)

////////////////////////////////////////////////////
//      need to be refined
////////////////////////////////////////////////////

#if  ( ENABLE_OD )
    #include "MsOD_LIB.h"
#endif

#ifndef PANEL_SYNC_MODE_1
#define PANEL_SYNC_MODE_1            0
#endif

#ifndef PANEL_ANALOG_TCON
#define PANEL_ANALOG_TCON           0
#endif

#ifndef ENABLE_DPWM_FUNCTION
#define ENABLE_DPWM_FUNCTION        0
#endif

#ifndef DPWM_BURST_MODE
#define DPWM_BURST_MODE             0
#endif

#ifndef DISABLE_SHORT_FRAME_TUNE
#define DISABLE_SHORT_FRAME_TUNE    1
#endif

#ifndef DISABLE_SHORT_LINE_TUNE
#define DISABLE_SHORT_LINE_TUNE     0
#endif

#ifndef SET_SSC_SPAN
#define SET_SSC_SPAN                0x00
#endif

#ifndef SET_SSC_STEP
#define SET_SSC_STEP                0x00
#endif

#ifndef PANEL_DOT_HEIGHT
#define PANEL_DOT_HEIGHT            100
#endif

#ifndef PANEL_DOT_WIDTH
#define PANEL_DOT_WIDTH            PANEL_DOT_HEIGHT
#endif

#ifndef CRT_INTERLACE_DOUBLOUT
#define CRT_INTERLACE_DOUBLOUT  0
#endif

#define WIDE_PANEL  0
#define _4_3_PANEL  1
#define OTHER_PANEL 2
#define _5_4_PANEL  3
#define _16_9_PANEL 4

////////////////////////////////////////////////////
// Panel common constant
////////////////////////////////////////////////////
#define PANEL_PWM_BRIGHT        1

////////////////////////////////////////////////////
// Panel power sequence
////////////////////////////////////////////////////
//Truman temp
//#define pnlSetFPVCCOn()     SetPinFpVcc()
//#define pnlSetFPVCCOff()    ClrPinFpVcc()

////////////////////////////////////////////////////
// Ouput Clock PLL
////////////////////////////////////////////////////

// Output clock = XTAL * MM * ((524288 * LM * K) / (SET * A))
//  where XTAL = external XTAL frequency
//        MM = PLL divider MM, SREG D2[4:0]
//        LM = PLL divider LM, SREG D3[4:0]
//        K = PLL divider K, SREG D4[4:3]
//        SET = PLL divider SET, SREG D5, D6, D7
//        A = PLL divider A, LVDS single channel = 7
//                           LVDS dual channel = 3.5
//                           RSDS/TTL = 4

//
//  TCON setting, not every panel have it,
//  so we add redundant define here to let compile ok
//
#ifndef SET_PTC_MODE1
#define SET_PTC_MODE1           0
#endif
#ifndef SET_PTC_MODE2
#define SET_PTC_MODE2           0
#endif
#ifndef SET_PTC_MODE3
#define SET_PTC_MODE3           0
#endif
#ifndef SET_FRP_TRAN
#define SET_FRP_TRAN            0
#endif
#ifndef SET_STH_START
#define SET_STH_START           0
#endif
#ifndef SET_STH_WIDTH
#define SET_STH_WIDTH           0
#endif
#ifndef SET_OEH_START
#define SET_OEH_START           0
#endif
#ifndef SET_OEH_WIDTH
#define SET_OEH_WIDTH           0
#endif
#ifndef SET_OEV_START
#define SET_OEV_START           0
#endif
#ifndef SET_OEV_WIDTH
#define SET_OEV_WIDTH           0
#endif
#ifndef SET_CKV_START
#define SET_CKV_START           0
#endif
#ifndef SET_CKV_START2
#define SET_CKV_START2          0
#endif
#ifndef SET_CKV_WIDTH
#define SET_CKV_WIDTH           0
#endif
#ifndef SET_STV_LINE_TH
#define SET_STV_LINE_TH         0
#endif
#ifndef SET_STV_START
#define SET_STV_START           0
#endif
#ifndef SET_STV_WIDTH
#define SET_STV_WIDTH           0
#endif
#ifndef SET_OEV2_START
#define SET_OEV2_START          0
#endif
#ifndef SET_OEV3_START
#define SET_OEV3_START          0
#endif
#ifndef SET_H_ST_DLY_L
#define SET_H_ST_DLY_L          0
#endif
#ifndef SET_H_ST_DLY_H
#define SET_H_ST_DLY_H          0
#endif
#ifndef SET_CLK_DLY_SYNC_OUT
#define SET_CLK_DLY_SYNC_OUT    0
#endif
#ifndef SET_CKV_END2
#define SET_CKV_END2            0
#endif
#ifndef SET_Q1H
#define SET_Q1H                 0
#endif
#ifndef SET_OEV2_WIDTH
#define SET_OEV2_WIDTH          0
#endif
#ifndef SET_OEV3_WIDTH
#define SET_OEV3_WIDTH          0
#endif
#ifndef SET_OEV_DELTA
#define SET_OEV_DELTA           0
#endif
// Every panel must have his own PanelFineTune()
// The function will be called after mode change successfully
//extern void PanelFineTune();

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif


