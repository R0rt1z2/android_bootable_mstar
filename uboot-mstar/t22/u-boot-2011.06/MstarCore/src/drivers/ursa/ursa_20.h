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

#ifndef __DRV_URSA_20_H__
#define __DRV_URSA_20_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>
#include <MsDebug.h>

#define MFC_CMD_DATA_SUM      7
//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_URSA20_SYSBIN)
typedef struct
{
    MS_U32 u32ID;
    MS_U32 u32Value;
} SysBin_DataFormat;

typedef enum
{
    E_URSA_SYS_BIN_U7_BIN_VERSION = 1,
    E_URSA_SYS_BIN_BACKLIGHT_PWM_CH_SEL,
    E_URSA_SYS_BIN_BACKLIGHT_PWM_POLARITY,
    E_URSA_SYS_BIN_BACKLIGHT_PWM_DUTY,
    E_URSA_SYS_BIN_BACKLIGHT_PWM_SHIFT,
    E_URSA_SYS_BIN_3D_GLASSES_USE_PWM_CH,
    E_URSA_SYS_BIN_3D_GLASSES_USE_PWM_PERIOD_N_VSYNC,
    E_URSA_SYS_BIN_3D_GLASSES_USE_PWM_SHIFT,

    E_URSA_SYS_BIN_RX_MODE_SEL = 500,
    E_URSA_SYS_BIN_LVDS_RX_TYPE,
    E_URSA_SYS_BIN_LVDS_RX_CHCONNECT,
    E_URSA_SYS_BIN_LVDS_RX_DATASEL,
    E_URSA_SYS_BIN_LVDS_RX_BIT_NUM,
    E_URSA_SYS_BIN_LVDS_RX_TI_MODE,
    E_URSA_SYS_BIN_LVDS_RX_SWAP_ODD_EVEN,
    E_URSA_SYS_BIN_LVDS_RX_SWAP_MSB_LSB,
    E_URSA_SYS_BIN_LVDS_RX_SWAP_P_N,
    E_URSA_SYS_BIN_LVDS_RX_HSLVDS_SWAP_P_N,
    E_URSA_SYS_BIN_LVDS_PIN_SEQ_SWAP,

    E_URSA_SYS_BIN_VBY1_RX_VIDEO_LANE_NUM = 520,
    E_URSA_SYS_BIN_VBY1_RX_VIDEO_H_SYNC_REVERSE,
    E_URSA_SYS_BIN_VBY1_RX_VIDEO_V_SYNC_REVERSE,

    E_URSA_SYS_BIN_VBY1_RX_OSD_LANE_NUM = 525,
    E_URSA_SYS_BIN_VBY1_RX_OSD_H_SYNC_REVERSE,
    E_URSA_SYS_BIN_VBY1_RX_OSD_V_SYNC_REVERSE,

    E_URSA_SYS_BIN_VBY1_RX_DATA_SEL_1 = 530,
    E_URSA_SYS_BIN_VBY1_RX_DATA_SEL_2,
    E_URSA_SYS_BIN_VBY1_RX_DATA_SEL_3,
    E_URSA_SYS_BIN_VBY1_RX_DATA_SEL_4,
    E_URSA_SYS_BIN_VBY1_RX_DATA_SEL_5,

    E_URSA_SYS_BIN_VBY1_RX_DATA_SWAP = 540,
    E_URSA_SYS_BIN_VBY1_RX_DATA_REVERSE,
    E_URSA_SYS_BIN_VBY1_RX_BYTE_MODE,
    E_URSA_SYS_BIN_VBY1_RX_VIDEO_BYTE_MODE,
    E_URSA_SYS_BIN_VBY1_RX_OSD_BYTE_MODE,
    E_URSA_SYS_BIN_RX_LR_MODE,
    E_URSA_SYS_BIN_HDMI_DEFAULT_EQ,
    E_URSA_SYS_BIN_OSD_DATA_FORMAT,

    E_URSA_SYS_BIN_MEMC_LEVEL = 560,
    E_URSA_SYS_BIN_MIU_SSC_ENABLE,
    E_URSA_SYS_BIN_MIU_SSC_SPAN_DEFAULT,
    E_URSA_SYS_BIN_MIU_SSC_STEP_DEFAULT,
    E_URSA_SYS_BIN_MFC_ENABLE_LVDS_SSC,
    E_URSA_SYS_BIN_MFC_LVDS_SSC_SPAN_DEFAULT,
    E_URSA_SYS_BIN_MFC_LVDS_SSC_STEP_DEFAULT,
    E_URSA_SYS_BIN_MOD_POWER_ON_MODE,

    E_URSA_SYS_BIN_CUS_GPIO_SETTING1 = 570,
    E_URSA_SYS_BIN_CUS_GPIO_SETTING2,
    E_URSA_SYS_BIN_CUS_GPIO_SETTING3,

    E_URSA_SYS_BIN_RGB_BYPASS_EN = 580,
    E_URSA_SYS_BIN_OSD_COLOR_KEY_ENABLE,
    E_URSA_SYS_BIN_OSD_COLOR_KEY_RGBA,
    E_URSA_SYS_BIN_OSD_COLOR_KEY_ENABLE2,
    E_URSA_SYS_BIN_OSD_COLOR_KEY_RGBA2,

    E_URSA_SYS_BIN_OSD_ENABLE = 590,
    E_URSA_SYS_BIN_INPUT_OSD_TIMMING_HACTIVE,
    E_URSA_SYS_BIN_INPUT_OSD_TIMMING_VACTIVE,
    E_URSA_SYS_BIN_INPUT_VIDEO_TIMMING_HACTIVE,
    E_URSA_SYS_BIN_INPUT_VIDEO_TIMMING_VACTIVE,

    E_URSA_SYS_BIN_CUS_HDCP22_KEY_SIZE = 600,

    E_URSA_SYS_BIN_3D_PANEL_TYPE = 1000,

    E_URSA_SYS_BIN_LVDS_TX_PANEL_TYPE = 1002,

    E_URSA_SYS_BIN_MASK_HS = 1004,
    E_URSA_SYS_BIN_MASK_VS,
    E_URSA_SYS_BIN_MASK_DE,
    E_URSA_SYS_BIN_PANEL_INV_HSYNC,
    E_URSA_SYS_BIN_PANEL_INV_VSYNC,
    E_URSA_SYS_BIN_PANEL_INV_DE,
    E_URSA_SYS_BIN_LVDS_TX_BIT_NUMBER,
    E_URSA_SYS_BIN_LVDS_TX_CHANNEL_NUMBER,
    E_URSA_SYS_BIN_CUS_ODCLK_DIV_2_MODE,
    E_URSA_SYS_BIN_LVDS_TX_CHANNEL_SWAP_CHANNEL,
    E_URSA_SYS_BIN_DITHER_EN,
    E_URSA_SYS_BIN_DITHER_8BITS,
    E_URSA_SYS_BIN_LVDS_TX_TI_MODE,
    E_URSA_SYS_BIN_LVDS_TX_SWAP_PAIR,
    E_URSA_SYS_BIN_LVDS_TX_SHIFT_PAIR,
    E_URSA_SYS_BIN_SWAP_LR,
    E_URSA_SYS_BIN_LVDS_TX_SWAP_POL,
    E_URSA_SYS_BIN_PANEL_SEQ_TYPE,

    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_00_03 = 1030,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_04_07,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_08_11,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_12_15,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_16_19,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_20_23,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_24_27,
    E_URSA_SYS_BIN_PANEL_CHANNEL_CONNECT_28_31,

    E_URSA_SYS_BIN_LVDS_TX_HSTART = 1100,
    E_URSA_SYS_BIN_LVDS_TX_VSTART,
    E_URSA_SYS_BIN_LVDS_TX_HACTIVE, //WIDTH
    E_URSA_SYS_BIN_LVDS_TX_VACTIVE, //HEIGHT
    E_URSA_SYS_BIN_LVDS_TX_HTOTAL,
    E_URSA_SYS_BIN_LVDS_TX_VTOTAL,
    E_URSA_SYS_BIN_LVDS_TX_PANEL_VFREQ,
    E_URSA_SYS_BIN_LVDS_TX_VTOTAL_MAX,
    E_URSA_SYS_BIN_LVDS_TX_VTOTAL_MIN,

    E_URSA_SYS_BIN_PANEL_HSTART_SPECIAL = 1120,
    E_URSA_SYS_BIN_PANEL_VSTART_SPECIAL,
    E_URSA_SYS_BIN_PANEL_HACTIVE_SPECIAL, //WIDTH
    E_URSA_SYS_BIN_PANEL_VACTIVE_SPECIAL, //HEIGHT
    E_URSA_SYS_BIN_PANEL_HTOTAL_SPECIAL,
    E_URSA_SYS_BIN_PANEL_VTOTAL_SPECIAL,
    E_URSA_SYS_BIN_PANEL_VFREQ_SPECIAL,
    E_URSA_SYS_BIN_PANEL_VTOTAL_MAX_SPECIAL,
    E_URSA_SYS_BIN_PANEL_VTOTAL_MIN_SPECIAL,

    E_URSA_SYS_BIN_LVDS_TX_INCREASE_VTOTAL_FOR_50HZ = 1150,
    E_URSA_SYS_BIN_PASSIVE_FIRST_LINE,
    E_URSA_SYS_BIN_PASSIVE_LINE_ORDER,
    E_URSA_SYS_BIN_ENABLE_MST_MIRROR_FUNC,
    E_URSA_SYS_BIN_DITHER_TAIL_CUT,
    E_URSA_SYS_BIN_CUS_PANEL_SWING_DATA,
    E_URSA_SYS_BIN_PANEL_ID,

    E_URSA_SYS_BIN_TYPE_MAX
}EN_URSA_SYS_BIN_TYPE;
#endif

// ursa
#define SPI_HEADER_DDC_TYPE_SUPPORT 0x0
#define SPI_HEADER_DDC_TYPE_DUAL_BIN_ENABLE 0x1
#define SPI_HEADER_DDC_TYPE_SU_AP1_ADDR 0x2
#define SPI_HEADER_DDC_TYPE_SU_AP2_ADDR 0x3
#define SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG_ADDR 0x4
#define SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG 0x5
#define SPI_HEADER_DDC_TYPE_FLASH_SIZE 0x6
#define SPI_HEADER_DDC_TYPE_FAST_UPGRADE_ENABLE 0x7
#define CRC32_TYPE_FLASH               (0x00)
#define CRC32_TYPE_DRAM                (0x01)

typedef struct {
    uint8_t  flash_size; // 1-4
    uint32_t first_bin_start_addr; // 5-8
    uint32_t second_bin_start_addr; // 9-12
    uint32_t boot_flag_addr; // 13-16
    uint8_t is_double_bin_enable; // 17
} SFlashHeader;

//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Ursa_20_SWI2C_Init(void);
MS_BOOL MDrv_Ursa_20_Set_Lane_VB1_per_init(MS_U8 u8OsdLane, MS_U8 u8VideoLane);
MS_BOOL MDrv_Ursa_20_Set_Lane_VB1(MS_U16 u16OsdWidth, MS_U16 u16VideoWidth);
MS_BOOL MDrv_Ursa_20_Panel_Initialize(void);
MS_BOOL MDrv_Ursa_20_SetModPowerOn(void);
MS_BOOL MDrv_Ursa_20_PanelOn(void);
MS_BOOL MDrv_Ursa_20_SetUrsaGpio(MS_U8 u8gpioID, MS_BOOL bOnOff);
MS_BOOL MDrv_Ursa_20_ReadUrsaGpio(MS_U8 u8gpioId);
void MDrv_Ursa_20_SetUrsaHardReset(void);
MS_U32 GetUrsa20VersionInfo(void);


#if defined(CONFIG_URSA20_SYSBIN) && defined(CONFIG_URSA_TXRX_SUPPORT)
MS_U32 MDrv_Ursa_20_GetMFCBinAddress(MS_U8 u8BinType);
MS_U16 MDrv_Ursa_20_GetMFCSysBinLen(); // unit: byte
MS_S32 MDrv_Ursa_20_GenMFCSysBin(MS_U8 *pu8Data);
#endif

#endif //__DRV_URSA_20_H__
