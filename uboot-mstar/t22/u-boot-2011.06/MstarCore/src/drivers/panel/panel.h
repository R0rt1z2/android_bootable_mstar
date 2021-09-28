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

#include <apiPNL.h>

#define INTERFACE extern

#ifndef LVDS_PN_SWAP_L
#define LVDS_PN_SWAP_L                  0x00
#endif
#ifndef LVDS_PN_SWAP_H
#define LVDS_PN_SWAP_H                  0x00
#endif

typedef enum{
    PANEL_RES_MIN = 0,

    PNL_FULLHD_SEC_LE32A = PANEL_RES_MIN,
    // FOR NORMAL LVDS PANEL
    PNL_SXGA_AU17_EN05       = 1,            // 1280X1024, PNL_AU17_EN05_SXGA
    PNL_WXGA_AU20_T200XW02   = 2,            // 1366X768, PNL_AU20_T200XW02_WXGA,
    PNL_WXGA_PLUS_CMO190_M190A1 = 3,         // 1440X900, PNL_CMO19_M190A1_WXGA, PNL_AU19PW01_WXGA
    PNL_WSXGA_AU22_M201EW01  = 4,            // 1680X1050, PNL_AU20_M201EW01_WSXGA,
    PNL_FULLHD_CMO216_H1L01  = 5,            // 1920X1080, PNL_AU37_T370HW01_HD, PNL_CMO216H1_L01_HD.H
    PANEL_RES_FULL_HD       = 5,            // for auotbuild compatitable

    // FOR DAC/HDMI TX OUTPUT
    DACOUT_480I              = 6,            // JUST FOR U3 DAC OUTPUT 480I TIMING USAGE
    DACOUT_480P              = 7,            // JUST FOR U3 DAC OUTPUT 480P TIMING USAGE
    DACOUT_576I              = 8,            // JUST FOR U3 DAC OUTPUT 576I TIMING USAGE
    DACOUT_576P              = 9,            // JUST FOR U3 DAC OUTPUT 576P TIMING USAGE
    DACOUT_720P_50           = 10,           // JUST FOR U3 DAC OUTPUT 720P TIMING USAGE
    DACOUT_720P_60           = 11,           // JUST FOR U3 DAC OUTPUT 720P TIMING USAGE
    DACOUT_1080I_50          = 12,           // JUST FOR U3 DAC OUTPUT 1080I TIMING USAGE
    DACOUT_1080I_60          = 13,           // JUST FOR U3 DAC OUTPUT 1080I TIMING USAGE
    DACOUT_1080P_50          = 14,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1080P_60          = 15,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    DACOUT_1080P_30          = 16,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1080P_25          = 17,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1080P_24          = 18,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_640x480_60        = 19,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1920x2205P_24     = 20,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    DACOUT_1280x1470P_50     = 21,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1280x1470P_60     = 22,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_3840x2160P_24     = 23,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_3840x2160P_25     = 24,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_3840x2160P_30     = 25,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    DACOUT_3840x2160P_50    =26,
    DACOUT_3840x2160P_60    =27,
    DACOUT_4096x2160P_24    =28,
    DACOUT_4096x2160P_25    =29,
    DACOUT_4096x2160P_30    =30,

    DACOUT_4096x2160P_50    =31,
    DACOUT_4096x2160P_60    =32,
    DACOUT_3840x2160P420_30    =33,
    DACOUT_3840x2160P420_50    =34,
    DACOUT_3840x2160P420_60    =35,
    DACOUT_4096x2160P420_30    =36,
    DACOUT_4096x2160P420_50    =37,
    DACOUT_4096x2160P420_60    =38,


    PNL_WUXGA_CMO260_J2,           // 1920*1200
#if ENABLE_MFC
    MFC_DEFAULT,
    MFC120_FULLHD_CMO216_H1L01 = MFC_DEFAULT,
    MFC60_FULLHD_CMO216_H1L01,
#endif
#if ENABLE_TCON
    PNL_FULLHD_TCON,
#endif
    PNL_RES_MAX_NUM,
}PANEL_RESOLUTION_TYPE;

#if defined(CONFIG_URSA_TXRX_SUPPORT)
/// Ursa RXTX Panel parameter from Panel ini
typedef struct
{
    MS_U8      m_eMfc_lvds_rx_channel;
    MS_U8      m_eMfc_lvds_rx_bit_num;
    MS_U8      m_eMfc_lvds_rx_ti_mode;
    MS_U8      m_bMfc_lvds_rx_swap_msb_lsb;
    MS_U8      m_bMfc_lvds_rx_swap_p_n;
    MS_U8      m_bMfc_lvds_rx_swap_odd_even;
    MS_U16    m_u16Mfc_panel_htotal;
    MS_U16    m_u16Mfc_panel_vtotal;
    MS_U16    m_u16Mfc_panel_vstart;
    MS_U16    m_u16Mfc_panel_hstart;
    MS_U16    m_u16Mfc_panel_height;
    MS_U16    m_u16Mfc_panel_width;
    MS_U8      m_u8Mfc_panel_vfreq;
    MS_U8      m_eMfc_panel_inc_vtotal_for_50hz;
    MS_U8      m_eMfc_panel_type;
    MS_U8      m_eMfc_panel_channel;
    MS_U8      m_u8Mfc_panel_lvds_swap_ch;
    MS_U8      m_eMfc_lvds_tx_bit_num;
    MS_U8      m_bMfc_panel_lvds_tx_shift_pair;
    MS_U8      m_eMfc_panel_tx_ti_mode;
    MS_U8      m_bMfc_panel_lvds_tx_swap_pol;
    MS_U8      m_bMfc_panel_lvds_tx_swap_pair;
    MS_U8      m_bMfc_tx_dither_enable;
    MS_U8      m_bMfc_rx_ttl;
    MS_U8      m_eMfc_panel_sub_type;
    MS_U8      m_bMfc_tx_mask_HS;
    MS_U8      m_bMfc_tx_mask_VS;
    MS_U8      m_bMfc_tx_mask_DE;
    MS_U8      m_bMfc_tx_invert_HS;
    MS_U8      m_bMfc_tx_invert_VS;
    MS_U8      m_bMfc_tx_invert_DE;
    MS_U8      m_bMfc_tx_dither_8_bits;
    MS_U8      m_u8Mfc_ctrl_swing;
    MS_U8      m_u8Mfc_Safe_Range;
    MS_U8      m_eMfc_panel_ursa_selection;
    MS_U8      m_eMfc_dither_tailcut;
    MS_U8      m_bMfc_Swap_LR;
    MS_U8      m_eMfc_Passive_First_Line;
    MS_U8      m_eMfc_Passive_Line_Order;
    MS_U8      m_eMfc_Sequence_Type;
    MS_U8      m_eMfc_Panel_3D_Type;
    MS_U8      m_eMfc_panel_pair_swap_type;
    MS_U8      m_u8MfcLVDSSwapCh;
    MS_U8      m_bMfcLVDSSwapPol;
    MS_U8      m_bMfcPanelSwapLR;
    MS_U8      m_u8MfcUrsaType;

    MS_U8      m_u8Mfc_VbyOne_0_ChanMap_0;
    MS_U8      m_u8Mfc_VbyOne_0_ChanMap_1;
    MS_U8      m_u8Mfc_VbyOne_0_ChanMap_2;
    MS_U8      m_u8Mfc_VbyOne_0_ChanMap_3;

    MS_U8      m_u8Mfc_VbyOne_1_ChanMap_0;
    MS_U8      m_u8Mfc_VbyOne_1_ChanMap_1;
    MS_U8      m_u8Mfc_VbyOne_1_ChanMap_2;
    MS_U8      m_u8Mfc_VbyOne_1_ChanMap_3;

    MS_U8      m_u8Mfc_VbyOne_2_ChanMap_0;
    MS_U8      m_u8Mfc_VbyOne_2_ChanMap_1;
    MS_U8      m_u8Mfc_VbyOne_2_ChanMap_2;
    MS_U8      m_u8Mfc_VbyOne_2_ChanMap_3;

    MS_U8      m_u8Mfc_VbyOne_3_ChanMap_0;
    MS_U8      m_u8Mfc_VbyOne_3_ChanMap_1;
    MS_U8      m_u8Mfc_VbyOne_3_ChanMap_2;
    MS_U8      m_u8Mfc_VbyOne_3_ChanMap_3;

    MS_U8      m_u8Mfc_VbyOne_4_ChanMap_0;
    MS_U8      m_u8Mfc_VbyOne_4_ChanMap_1;
    MS_U8      m_u8Mfc_VbyOne_4_ChanMap_2;
    MS_U8      m_u8Mfc_VbyOne_4_ChanMap_3;

    MS_U8      m_u8Mfc_VbyOne_5_ChanMap_0;
    MS_U8      m_u8Mfc_VbyOne_5_ChanMap_1;
    MS_U8      m_u8Mfc_VbyOne_5_ChanMap_2;
    MS_U8      m_u8Mfc_VbyOne_5_ChanMap_3;
    MS_U8      m_bMfc_Spread_Spectrum_Enable;
}st_PNL_UrsaRXTX_PanelParameter;
#endif

#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
typedef enum
{
    EN_PANEL_BACKLIGHT_CCFL,
    EN_PANEL_BACKLIGHT_LED,
    EN_PANEL_BACKLIGHT_OLED

}typPanelBacklightType;

typedef struct
{
    MS_U8      m_eBacklightType;
    MS_U8      m_u16off_timing3;
    MS_U16    m_u16on_timing3;
}st_PNL_OLED_SpecificParameter;
#endif

PanelType * MApi_GetPanelSpec(PANEL_RESOLUTION_TYPE enResolutionType);
void MApi_PNL_En(MS_BOOL bPanelOn, MS_U16 u16PanelTiming);
void MApi_PNL_On(MS_U16 u16PanelOnTiming );
void MApi_PNL_SetBackLight(MS_BOOL bEnable);
PANEL_RESOLUTION_TYPE MApi_PNL_GetPnlTypeSetting(void);
void MApi_PNL_Dump(void);
MS_BOOL MApi_SetEnv2BootArgs(const char* name,const char* value);
unsigned long MApi_PNL_GetPanelVccOnTime(void);
MS_U16 GetOnTiming1(void);
MS_U16 GetOnTiming2(void);
MS_U16 GetOffTiming1(void);
MS_U16 GetOffTiming2(void);
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
MS_U8 GetBacklightType(void);
MS_BOOL IsTVOled(void);
MS_U16 GetOnTiming3(void);
MS_U16 GetOffTiming3(void);
#endif
#if defined(CONFIG_URSA_TXRX_SUPPORT)
st_PNL_UrsaRXTX_PanelParameter GetUrsaRxTxPanelParameter(void);
#endif

#endif

