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
#ifndef _ENUM_DEVICE_H_
#define _ENUM_DEVICE_H_

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define FREQ_12MHZ              (12000000UL)
#define FREQ_14P318MHZ          (14318180UL)
#define FREQ_24MHZ              (24000000UL)
#define FREQ_67P5MHZ            (67500000UL)
#define FREQ_83MHZ              (83000000UL)
#define FREQ_90MHZ              (90000000UL)
#define FREQ_108MHZ             (108000000UL)
#define FREQ_120MHZ             (120000000UL)
#define FREQ_123MHZ             (123428000UL)
#define FREQ_125MHZ             (125000000UL)
#define FREQ_130MHZ             (130000000UL)
#define FREQ_135MHZ             (135000000UL)
#define FREQ_141MHZ             (141000000UL)
#define FREQ_144MHZ             (144000000UL)
#define FREQ_160MHZ             (160000000UL)
#define FREQ_166MHZ             (166000000UL)
#define FREQ_170MHZ             (170000000UL)
#define FREQ_172P8MHZ           (172800000UL)
#define FREQ_180MHZ             (180000000UL)
#define FREQ_195MHZ             (195000000UL)
#define FREQ_200MHZ             (200000000UL)
#define FREQ_210MHZ             (210000000UL)
#define FREQ_220MHZ             (220000000UL)
#define FREQ_230MHZ             (230000000UL)
#define FREQ_250MHZ             (250000000UL)


//------------------------------------------------------------------------------------
// IR_TYPE_SEL
//------------------------------------------------------------------------------------
#define IR_TYPE_OLD             0
#define IR_TYPE_NEW                1
#define IR_TYPE_MSTAR_DTV       2
#define IR_TYPE_MSTAR_RAW       3
#define IR_TYPE_MSTAR_SZ1       4
#define IR_TYPE_RC_V16          5
#define IR_TYPE_CUS03_DTV       6
#define IR_TYPE_CUS4            7
#define IR_TYPE_DC_LWB1         8
#define IR_TYPE_DC_BN59         9
#define IR_TYPE_P_DTV           10
#define IR_TYPE_SZ_CTV1         11
#define IR_TYPE_SZ_CHO1         12
#define IR_TYPE_SZ_ALC1            13
#define IR_TYPE_SZ_TON1         14
#define IR_TYPE_SZ_KON1         15
#define IR_TYPE_SZ_KON2         16
#define IR_TYPE_SZ_MTC1         17
#define IR_TYPE_SZ_HAI1         18
#define IR_TYPE_SZ_PRI1         19
#define IR_TYPE_SZ_TEN          20
#define IR_TYPE_CUS21SH         21
#define IR_TYPE_DC_BN59_611     22
#define IR_TYPE_CUS30_BK        23
#define IR_TYPE_TPV_AOC         24
#define IR_TYPE_JEAN_RC5         25
#define IR_TYPE_HYF_51F         26
#define IR_TYPE_HIS_NEC         27
#define IR_TYPE_MSTAR_FANTASY   28
#define IR_TYPE_HIS_NEW         29//for Hisense 6I78


#define IR_TYPE_CUS_DCLWB1       40
#define IR_TYPE_CUS_RC260I       41
#define IR_TYPE_CUS_WDE          42
#define IR_TYPE_CUS_SZ         43
#define IR_TYPE_CUS_SK         44
#define IR_TYPE_J_DTV            45
#define IR_TYPE_CUS_RC260D      46
#define IR_TYPE_J_RC5           47
#define IR_TYPE_CUS_NS_RC01G_09 48
#define IR_TYPE_XMP             49

//------------------------------------------------------------------------------------
// PANEL_TYPE_SEL
//------------------------------------------------------------------------------------
#define PNL_DAC_720X480I        0
#define PNL_DAC_720X480P        1
#define PNL_DAC_720X576I        2
#define PNL_DAC_720X576P        3
#define PNL_DAC_720X833I        4
#define PNL_DAC_1024X768P       5
#define PNL_DAC_1920X1080       6
#define PNL_AU17_EN05_SXGA      7
#define PNL_AU20_VGA            8
#define PNL_AU20_SVGA           9
#define PNL_AU20_UXGA           10
#define PNL_CMO19_SXGA          11
#define PNL_CMO20_VGA           12
#define PNL_CMO27_WXGA          13
#define PNL_CMO29_WXGA          14
#define PNL_CMO32_WXGA          15
#define PNL_FU17_FLC43_WXGA     16
#define PNL_HAN23_HSD230WX01A   17
#define PNL_LG17_SXGA           18
#define PNL_LG19_SXGA           19
#define PNL_LG17_LC171_WXGA     20
#define PNL_LG20_UXGA           21
#define PNL_LG23_LC230_WXGA     22
#define PNL_LG26_WXGA           23
#define PNL_LG30_LC300_WXGA     24
#define PNL_QDI17_EL07_SXGA     25
#define PNL_SEC_LTM170W1_L01    26
#define PNL_SH19_LQ197_VGA      27
#define PNL_SH20_B7B_UXGA       28
#define PNL_SAMSUNG24_WUXGA     29
#define PNL_CMO37_WUXGA         30
#define PNL_AU26_T260XW01_WXGA  31
#define PNL_AU20_M201EW01_WSXGA 32
#define PNL_LG32_WXGA           33
#define PNL_SAMSUNG21_WSXGA     34
#define PNL_LG20_VGA            35
#define PNL_HSD260WX11_A        36
#define PNL_AU10_2_DT           37
#define PNL_CPT20_VGA           38
#define PNL_PVI10_VGA           39
#define PNL_CMO20_WXGA          40
#define PNL_CPT15_XGA           41
#define PNL_PANASONIC22_WVGA    42
#define PNL_TMD133_WXGA         43
#define PNL_HANNS96_SVGA        44
#define PNL_HANNS12_VGA         45
#define PNL_LG15_XGA            46
#define PNL_SHARP08_D           47
#define PNL_TMD12_SVGA          48
#define PNL_AU17_WXGA           49
#define PNL_AU17_EG01_SXGA      50
#define PNL_CMO19_M190A1_WXGA   51
#define PNL_CPT15_XG09_XGA      52
#define PNL_AU26_T260XW02_WXGA  53
#define PNL_AU19PW01_WXGA       54
#define PNL_SAMSUNG19M2_WXGAL01 55
#define PNL_AU07_AT             56
#define PNL_AU20_T200XW02_WXGA  57
#define PNL_CMO26_V260B1_WXGA   58
#define PNL_AU85_AT             59
#define PNL_CMO22_WSXGA         60
#define PNL_AU23_T230XW01_WXGA  61
#define PNL_CMO42_WUXGA         62
#define PNL_LPL22_LM220WE1_WSXGA 63
#define PNL_LPL19_LM190WX1_WXGA 64
#define PNL_SAMSUNG23_LTA230W2L1_WXGA 65
#define PNL_LPL42_WXGA          66
#define PNL_LG_PDP32F1_WVGA        67
#define PNL_AU37_T370HW01_HD    68
#define PNL_BOE190WG01_WXGA     69
#define PNL_CPT320WF01_WXGA     70
#define PNL_AU154_WXGA           71
#define PNL_SH32_LQ315T3LZ50Z_WXGA 72
#define PNL_SVSVA150XG04TB_XGA  73

#define PNL_CMO16_WXGA          74
#define PNL_CPT22_A01_V2        75
#define PNL_AU37_T370XW01_WXGA  76 // 120HZ
#define PNL_LG42_WXGA           77 // 120HZ
#define PNL_CMO260J2_WUXGA      78
#define PNL_CMO216H1_L01_HD     79
#define PNL_AU32_T315HW02_HD    80

//
#define PNL_DAC_CRT             99
//------------------------------------------------------------------------------------
//  FRONTPANEL_TYPE
//------------------------------------------------------------------------------------
#define FRONTPANEL_TYPE_NONE            0   // no FRONTPANEL
#define FRONTPANEL_TYPE_74HC164         1   // 74HC164
#define FRONTPANEL_TYPE_PT6958          2   // PT6958

//------------------------------------------------------------------------------------
//KEYPAD_TYPE_SEL
//------------------------------------------------------------------------------------
#define KEYPAD_TYPE_NONE        0   // no keypad
#define KEYPAD_TYPE_ORIG        1   // MStar normal keypad
#define KEYPAD_TYPE_DEMO        2   // MStar demoset keypad
#define KEYPAD_TYPE_CUSTMOER    3   // Customer
#define KEYPAD_TYPE_BOARDDEP    4   // none regular slow ADC keypad

//------------------------------------------------------------------------------------
//RM_EEPROM_TYPE
//------------------------------------------------------------------------------------
#define RM_TYPE_24C01           0
#define RM_TYPE_24C02           1
#define RM_TYPE_24C04           2
#define RM_TYPE_24C08           3
#define RM_TYPE_24C16           4
#define RM_TYPE_24C32           5
#define RM_TYPE_24C64           6
#define RM_TYPE_24C512          7

//------------------------------------------------------------------------------------
//I2C_IO_EXPANDER_TYPE
//------------------------------------------------------------------------------------
#define I2C_IO_EXPANDER_NONE            0
#define I2C_IO_EXPANDER_PCA9557         1
#define I2C_IO_EXPANDER_PCF8574         2
#define I2C_IO_EXPANDER_MSG1016RC       3

//------------------------------------------------------------------------------------
// SATURN_FLASH_IC
//------------------------------------------------------------------------------------
#define FLASH_IC_INVALID        0x0000
#define FLASH_IC_SST25VF080B    0x0100      // 1M    SST
#define FLASH_IC_SST25VF016B    0x0101      // 2M
#define FLASH_IC_Pm25LV080      0x0200      // 1M    PMC
#define FLASH_IC_Pm25LV016      0x0201      // 2M
#define FLASH_IC_ST25P80_A      0x0300      // 1M    ST
#define FLASH_IC_ST25P16_A      0x0301      // 2M
#define FLASH_IC_STM25P32       0x0302      // 4M
#define FLASH_IC_AT26DF081A     0x0400      // 1M    ATMEL
#define FLASH_IC_AT26DF161      0x0401      // 2M
#define FLASH_IC_AT26DF321      0x0402      // 4M
#define FLASH_IC_MX25L8005      0x0500      // 1M    MXIC
#define FLASH_IC_MX25L1605A     0x0501      // 2M
#define FLASH_IC_MX25L3205D     0x0502      // 4M
#define FLASH_IC_MX25L6405D     0x0503      // 8M
#define FLASH_IC_NX25P80        0x0600      // 1M    NX
#define FLASH_IC_NX25P16        0x0601      // 2M
#define FLASH_IC_W25X80         0x0700      // 1M    WINB
#define FLASH_IC_W25X16         0x0701      // 2M
#define FLASH_IC_W25X32         0x0702      // 4M
#define FLASH_IC_S25FL008A      0x0800      // 1M    SPANSION
#define FLASH_IC_S25FL016A      0x0801      // 2M
#define FLASH_IC_EN25B20T       0x0900      // 2M    EON
#define FLASH_IC_EN25B20B       0x0901      // 2M
#define FLASH_IC_EN25B10T       0x0902      // 1M
#define FLASH_IC_EN25B10B       0x0903      // 1M
#define FLASH_IC_EN25B32B       0x0904      // 4M    (Bottom Boot)
#define FLASH_IC_EN25B64B       0x0905      // 4M
#define FLASH_IC_ESMT_F25L016A  0x0A00      // 2M
// Audio external I2S selection
#define AUDIO_I2S_NONE              0
#define AUDIO_I2S_STA333            1
#define AUDIO_I2S_STA559BW          2
#define AUDIO_I2S_TAS5706           3
#define AUDIO_I2S_TAS5707           4

#endif
