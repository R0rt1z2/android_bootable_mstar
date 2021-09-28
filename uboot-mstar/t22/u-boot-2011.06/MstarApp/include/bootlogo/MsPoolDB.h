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

#ifndef _SPI_DATABASE_H__
#define _SPI_DATABASE_H__
#include <ShareType.h>
#include <MsApiKeypad.h>
#include <apiPNL.h>

#include <MsApiPanel.h>
#include <gop/MsDrvGop.h>
#include <MsSystem.h>
#include <MsBoot.h>
#if defined(CONFIG_VOICE_CM4)
#include <MsApiCM4.h>
#endif

#define BUFFER_SIZE                 128
#define spi_sector_size             0x10000
#define env_sector_size             0x20000

#define Kpd_ChID_Offset             20

#define PWM_DUTY   "u32PWMDuty"
#define PWM_PERIOD "u32PWMPeriod"
#define PWM_DIVPWM "u16DivPWM"
#define PWM_POLPWM "bPolPWM"
#define PWM_MAXPWM "u16MaxPWMvalue"
#define PWM_MINPWM "u16MinPWMvalue"
#define PWM_CH "pnl_pwmch"

#if(CONFIG_ENABLE_V_I_PWM == 1)
// wuyian for pwm0 pwm2
#define I_PWM_ENABLE "I_PWM_ENABLE"
#define I_PWM_DUTY   "I_u32DutyPWM"
#define I_PWM_PERIOD "I_u32PeriodPWM"
#define I_PWM_DIVPWM "I_u16DivPWM"
#define I_PWM_POLPWM "I_bPolPWM"
#define I_PWM_MAXPWM "I_u16MaxPWMvalue"
#define I_PWM_MINPWM "I_u16MinPWMvalue"
#define I_PWM_CH "I_u8PWM_CH"

#define V_PWM_ENABLE "V_PWM_ENABLE"
#define V_PWM_DUTY   "V_u32DutyPWM"
#define V_PWM_PERIOD "V_u32PeriodPWM"
#define V_PWM_DIVPWM "V_u16DivPWM"
#define V_PWM_POLPWM "V_bPolPWM"
#define V_PWM_MAXPWM "V_u16MaxPWMvalue"
#define V_PWM_MINPWM "V_u16MinPWMvalue"
#define V_PWM_CH "V_u8PWM_CH"
#endif

#define KEYPAD_SEL "KEYPAD_SEL"

typedef enum
{
    E_DB_LOGO=0,
    E_DB_MUSIC,
    E_DB_BOARD_PARA,
    E_DB_PANEL_PARA,
    E_DB_URSA_6M30,
    E_DB_URSA,
    E_DB_TCON,
    E_DB_PNL_VB1,
    E_DB_PNL_GOPMUX,
    E_DB_PWM,
    E_DB_MISC,
    E_DB_MSPI,
    E_DB_DMA,
    E_DB_HDMITX,
    E_DB_IR_BOOTCHECK,
    E_DB_IR_WAKEUP,
    E_DB_RGBW,
    E_DB_KEYPAD,
    E_DB_3DLUT,
#if defined(CONFIG_DATA_SEPARATION)
    E_DB_KEYPAD_WAKEUP,
#endif
#if defined(CONFIG_VOICE_CM4)
    E_DB_MIC_CONFIG,
#endif
#if defined(CONFIG_URSA_TXRX_SUPPORT)
    E_DB_URSA_TXRX_PANEL_PARA,
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
    E_DB_OLED_LG_PANEL_PARA,
#endif

    E_DB_MAX,
}DB_INDEX;

typedef struct
{
    U32 Offset;
    U32 Size;
    U32 u32CrcCheck;
}ST_DBDATA;

typedef struct
{
    ST_DBDATA dbdata[E_DB_MAX];
    U32 u32CrcCheck;
}ST_DBTABLE;

typedef struct
{
    U8  enInColorFmt;
    U8  enOutColorFmt;
    U8  enInColorQuantRange;
    U8  enOutColorQuantRange;
    U8  enOutputMode;
    U8  enColorDepth;
    U8  enLoadDefaultFromMboot;
}st_hdmitx_setting;

int write_dbtable(U32 u32LogoOffset);
int read_dbtable(U32 u32LogoOffset);

int Read_LogoToDisplayAddr(U32 u32LogoDramAddr);
int Read_PanelParaFromflash(PanelType * panel_data);
#if defined(CONFIG_URSA_TXRX_SUPPORT)
int Read_UrsaRXTXPanelSetting_ToFlash(st_PNL_UrsaRXTX_PanelParameter * stUrsaRxTxPanel_data);
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
int Read_OLEDSpecificParameter_ToFlash(st_PNL_OLED_SpecificParameter * stPnlOledSpecifc_data);
#endif
int Read_BoardParaFromflash(st_board_para * board_data);
int Read_KeypadParaFromflash(st_keypad_para *keypad_data);
int Read_MusicToPlayAddr(MS_PHYADDR u32MusicDramAddr);
int Read_Ursa_6m30_Para(ursa_6m30_cmd_table * cmd_table);
int Read_Gop_Mux_ParaFromFlash(st_gopmux_para * gopmux_para);

#if (ENABLE_DYNAMIC_IR)
int Read_IRBootcheck_FromFlash(st_IRBootCheck_t * bootcheck_info);
#endif
#if defined(CONFIG_VOICE_CM4)
int Read_MicParaFromFlash(stMic_config* stMicConfig_para);
#endif
#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M30 == 1)
int iniparser_6m30_cmd(ursa_6m30_cmd_table * cmd_table);
int Load_Ursa6m30Para_ToFlash(void);
int Ursa_6M30_Setting(ursa_6m30_cmd_table * cmd_table);
#elif (ENABLE_URSA_8 == 1)|| (ENABLE_URSA_6M40 == 1)
int Read_Ursa_Para(ursa_cmd_table * cmd_table);
int Load_UrsaPara_ToFlash(void);
#if (ENABLE_URSA_8 == 1)
int Ursa_8_Setting(ursa_cmd_table * cmd_table);
#else
int Ursa_6M40_Syetting(ursa_cmd_table * cmd_table);
#endif

int Parser_Ursa_Ini(ursa_cmd_table * cmd_table);
#endif
#endif

int Read_TConFromFlash(U32 u32TConAddr);

//int Read_PnlVb1FromFlash(U32 u32PnlVb1Addr);
int Read_PnlVb1FromFlash(pnl_VB1ChannelOrder * channelOrder_data);
int Read_MiscSetting_ToFlash(st_sys_misc_setting * misc_data);
int Read_PWMSetting_ToFlash(st_pwm_setting * pwm_data);
int Read_HDMITXFromFlash(st_hdmitx_setting * u32HDMITXAddr);
int Read_RGBWFromFlash(U32 u32RGBWAddr);
int Read_3DLUTFromFlash(U32 u323DLUTAddr);
#if (CONFIG_LOCAL_DIMMING)
//MSPI DRV For Local Dimming  API Structure Declaration
#include <drvMSPI.h>

typedef enum
{
    E_LD_EDGE_TB_TYPE = 0, // top and bottom
    E_LD_EDGE_LR_TYPE = 1, // left and right
    E_LD_DIRECT_TYPE  = 2, //default
    E_LD_LOCAL_TYPE   = 3  // new mode supporting 32x32 LED
} EN_LD_LED_TYPE;

//ST_DRV_LD_DMA_INFO describe DMA
typedef struct
{
    MS_U8 bEdge2DEn;
    MS_U8 u8LDMAchanel;
    MS_U8 u8LDMATrimode;
    MS_U8 u8LDMACheckSumMode;
    MS_U16 u16DMADelay[4];
    MS_U8 u8cmdlength;                //the number of spi head
    MS_U16 u16MspiHead[8];            //led mcu  start Indicator1 command
    MS_U16 u16LedNum;
    MS_U8 u8ClkHz;                    //60hz or 120HZ
    //for LD config
    //MS_U8 u8PackOffset;
    //MS_U8 u8PackLength;
    MS_U8 u8YoffEnd;
    MS_U8 u8LEDWidth;                //led Width
    MS_U8 u8LEDHeight;               //led Height
    MS_U8 u8LDFWidth;                // Statistic width
    MS_U8 u8LDFHeight;               // Statistic height
    MS_U8 u8LSFWidth;                // LSF width
    MS_U8 u8LSFHeight;               // LSF height
    EN_LD_LED_TYPE  eLEDType;        // E_LD_DIRECT_TYPE EnuLD_LED_TYPE
    MS_U32 u32DMABaseOffset;
}ST_DRV_LD_DMA_INFO;


int Read_MSPIPara_FromFash(ST_DRV_MSPI_INFO * mspi_info);
int Read_DMAPara_FromFash(ST_DRV_LD_DMA_INFO * dma_info);
#endif
#endif //__SPI_DATABASE_H__
