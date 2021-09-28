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

#include <stdio.h>

#include "datatype.h"
#include "drvRIU.h"
#include "Board.h"
#include "c_riubase.h"
#include "hwreg_M7322.h"
#include "chip/bond.h"

#if (ENABLE_ONEBIN_ENABLE == 1)
#define PREBUFFERSIZE                  512      //array size
#define BUFFERSIZE                     2048      //array size
#endif

#define GPIO_NONE                   0       // Not GPIO pin (default)
#define GPIO_IN                     1       // GPI
#define GPIO_OUT_LOW                2       // GPO output low
#define GPIO_OUT_HIGH               3       // GPO output high

#if defined(ARM_CHAKRA) || defined(MIPS_CHAKRA) || defined(MSOS_TYPE_LINUX)
#define _MapBase_nonPM_M7322       (RIU_MAP + 0x00200000)
#define _MapBase_PM_M7322          RIU_MAP
#else
#define _MapBase_nonPM_M7322        0xa0200000
#define _MapBase_PM_M7322           0xa0000000
#endif

#define _MEMMAP_REGBANK_00_         _RVM1(0x0000, 0x00, 0xFF)
#define _MEMMAP_REGBANK_10_         _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_REGBANK_11_         _RVM1(0x0000, 0x11, 0xFF)
#define _MEMMAP_REGBANK_15_         _RVM1(0x0000, 0x15, 0xFF)

#if (ENABLE_ONEBIN_ENABLE == 1)
//U8 PRERomBuffer[PREBUFFERSIZE] ={0};
U8 MIUBuffer[BUFFERSIZE] ={0};
U8 MIUSTRBuffer[BUFFERSIZE] ={0};
U8 RamBuffer[BUFFERSIZE] ={0};
#endif

const U8 padInitTbl_PreInit[] =
{
    0xFF, 0xFF, 0xFF, 0xFF,         // magic code for ISP_Tool

    // programable device number
    // spi flash count
    0,
    0x00,                           // nor
    0x00,                           // nand
    0x00,                           // reserved
    0x00,                           // reserved
    0x00,                           // reserved

//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------
    _MEMMAP_REGBANK_00_,

    #if(PAD_VID0_IS_GPIO != GPIO_NONE)
        #define PAD_VID0_OEN (PAD_VID0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_VID0_OUT (PAD_VID0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2e84, PAD_VID0_OUT, BIT0),
        _RVM1(0x2e84, PAD_VID0_OEN, BIT1),
        //reg_vid_is_gpio
        _RVM1(0x0e39, BIT2, BIT2),   //reg[0e39]#2 = 1b
        //reg_miic_mode0[1:0]
        _RVM1(0x0ec9, 0, BIT7 | BIT6),   //reg[0ec9]#7~#6 = 00b
    #endif

    #if(PAD_VID1_IS_GPIO != GPIO_NONE)
        #define PAD_VID1_OEN (PAD_VID1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_VID1_OUT (PAD_VID1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2e85, PAD_VID1_OUT, BIT0),
        _RVM1(0x2e85, PAD_VID1_OEN, BIT1),
        //reg_vid_is_gpio
        _RVM1(0x0e39, BIT2, BIT2),   //reg[0e39]#2 = 1b
        //reg_miic_mode0[1:0]
        _RVM1(0x0ec9, 0, BIT7 | BIT6),   //reg[0ec9]#7~#6 = 00b
    #endif

    #if(PAD_VID2_IS_GPIO != GPIO_NONE)
        #define PAD_VID2_OEN (PAD_VID2_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_VID2_OUT (PAD_VID2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f22, PAD_VID2_OUT, BIT1),
        _RVM1(0x0f22, PAD_VID2_OEN, BIT0),
    #endif

    #if(PAD_VID3_IS_GPIO != GPIO_NONE)
        #define PAD_VID3_OEN (PAD_VID3_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_VID3_OUT (PAD_VID3_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f24, PAD_VID3_OUT, BIT1),
        _RVM1(0x0f24, PAD_VID3_OEN, BIT0),
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------

    _MEMMAP_REGBANK_10_,

// SDR/DDR  1.8V/3.3V, DQS at PAD_EMMC_IO8
#ifdef PADS_NAND_MODE
#if (PADS_NAND_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_NAND_MODE_MODE1 ((PADS_NAND_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                (PADS_NAND_MODE == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                (PADS_NAND_MODE == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1ede, CONFIG_NAND_MODE_MODE1, BITMASK(7:6)),
#endif
#endif

// NAND CS1
#ifdef PADS_NAND_CS1_EN
#if (PADS_NAND_CS1_EN != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_NAND_CS1_EN_MODE1 ((PADS_NAND_CS1_EN == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1ede, CONFIG_NAND_CS1_EN_MODE1, BIT5),
#endif
#endif

// EMMC RST
#ifdef PADS_EMMC_RSTN_EN_PM
#if (PADS_EMMC_RSTN_EN_PM != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EMMC_RSTN_EN_PM_MODE1 ((PADS_EMMC_RSTN_EN_PM == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
_MEMMAP_REGBANK_00_,
    _RVM1(0x0e62, CONFIG_EMMC_RSTN_EN_PM_MODE1, BIT4),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EMMC RST
#ifdef PADS_EMMC_RSTN_EN
#if (PADS_EMMC_RSTN_EN != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EMMC_RSTN_EN_MODE1 ((PADS_EMMC_RSTN_EN == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
    _RVM1(0x1e9e, CONFIG_EMMC_RSTN_EN_MODE1, BIT2),
#endif
#endif

// EMMC MODE
#ifdef PADS_EMMC_CONFIG
#if (PADS_EMMC_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EMMC_CONFIG_MODE1 ((PADS_EMMC_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1edc, CONFIG_EMMC_CONFIG_MODE1, BITMASK(7:6)),
#endif
#endif

//=============================================================================
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),     //reg[101ea1]#7 = 0b
    _END_OF_TBL_,
};

const U8 padInitTbl[] =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
    1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
    0x00,                       // nor
    0x00,                       // nand
    0x00,                       // reserved
    0x00,                       // reserved
    0x00,                       // reserved

//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------

_MEMMAP_REGBANK_00_,
    #if(PAD_IRIN_IS_GPIO != GPIO_NONE)
        #define PAD_IRIN_OEN (PAD_IRIN_IS_GPIO == GPIO_IN ? 0: 0)
        #define PAD_IRIN_OUT (PAD_IRIN_IS_GPIO == GPIO_OUT_HIGH ? 0: 0)
        //_RVM1(0x0f26, PAD_IRIN_OUT, 0),
        //_RVM1(0x0f26, PAD_IRIN_OEN, 0),
        //reg_ir_is_gpio
        _RVM1(0x0e38, BIT4, BIT4),   //reg[0e38]#4 = 1b
    #endif

    #if(PAD_CEC0_IS_GPIO != GPIO_NONE)
        #define PAD_CEC0_OEN (PAD_CEC0_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_CEC0_OUT (PAD_CEC0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f2a, PAD_CEC0_OUT, BIT1),
        _RVM1(0x0f2a, PAD_CEC0_OEN, BIT0),
        //reg_cec_is_gpio
        _RVM1(0x0e38, BIT6, BIT6),   //reg[0e38]#6 = 1b
    #endif

    #if(PAD_PWM_PM_IS_GPIO != GPIO_NONE)
        #define PAD_PWM_PM_OEN (PAD_PWM_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_PWM_PM_OUT (PAD_PWM_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f28, PAD_PWM_PM_OUT, BIT1),
        _RVM1(0x0f28, PAD_PWM_PM_OEN, BIT0),
        //reg_pwm_pm_is_gpio
        _RVM1(0x0e38, BIT5, BIT5),   //reg[0e38]#5 = 1b
    #endif

    #if(PAD_DDCA_CK_IS_GPIO != GPIO_NONE)
        #define PAD_DDCA_CK_OEN (PAD_DDCA_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCA_CK_OUT (PAD_DDCA_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x0494, PAD_DDCA_CK_OUT, BIT2),
        _RVM1(0x0494, PAD_DDCA_CK_OEN, BIT1),
        //reg_gpio2a0_en
        _RVM1(0x0494, BIT7, BIT7),   //reg[0494]#7 = 1b
    #endif

    #if(PAD_DDCA_DA_IS_GPIO != GPIO_NONE)
        #define PAD_DDCA_DA_OEN (PAD_DDCA_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
        #define PAD_DDCA_DA_OUT (PAD_DDCA_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
        _RVM1(0x0494, PAD_DDCA_DA_OUT, BIT6),
        _RVM1(0x0494, PAD_DDCA_DA_OEN, BIT5),
        //reg_gpio2a0_en
        _RVM1(0x0494, BIT7, BIT7),   //reg[0494]#7 = 1b
    #endif

    #if(PAD_GPIO0_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO0_PM_OEN (PAD_GPIO0_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO0_PM_OUT (PAD_GPIO0_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f00, PAD_GPIO0_PM_OUT, BIT1),
        _RVM1(0x0f00, PAD_GPIO0_PM_OEN, BIT0),
    #endif

    #if(PAD_GPIO1_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO1_PM_OEN (PAD_GPIO1_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO1_PM_OUT (PAD_GPIO1_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f02, PAD_GPIO1_PM_OUT, BIT1),
        _RVM1(0x0f02, PAD_GPIO1_PM_OEN, BIT0),
        //reg_uart_is_gpio_4[3:0]
        _RVM1(0x0eec, 0, BIT7 | BIT6),   //reg[0eec]#7~#6 = 00b
        _RVM1(0x0eed, 0, BIT1 | BIT0),   //reg[0eed]#1~#0 = 00b
        //reg_uart_is_gpio_3[3:0]
        _RVM1(0x0eec, 0, 0x3C),   //reg[0eec]#5~#2 = 0000b
        //reg_uart_is_gpio[3:0]
        _RVM1(0x0e6b, 0, 0x0F),   //reg[0e6b]#3~#0 = 0000b
    #endif

    #if(PAD_GPIO2_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO2_PM_OEN (PAD_GPIO2_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO2_PM_OUT (PAD_GPIO2_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f04, PAD_GPIO2_PM_OUT, BIT1),
        _RVM1(0x0f04, PAD_GPIO2_PM_OEN, BIT0),
        //reg_miic_mode0[1:0]
        _RVM1(0x0ec9, 0, BIT7 | BIT6),   //reg[0ec9]#7~#6 = 00b
    #endif

    #if(PAD_USB_CTRL_IS_GPIO != GPIO_NONE)
        #define PAD_USB_CTRL_OEN (PAD_USB_CTRL_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_USB_CTRL_OUT (PAD_USB_CTRL_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f06, PAD_USB_CTRL_OUT, BIT1),
        _RVM1(0x0f06, PAD_USB_CTRL_OEN, BIT0),
        //reg_vid_mode[1:0]
        _RVM1(0x0e4f, 0, BIT5 | BIT4),   //reg[0e4f]#5~#4 = 00b
        //reg_uart_is_gpio_4[3:0]
        _RVM1(0x0eec, 0, BIT7 | BIT6),   //reg[0eec]#7~#6 = 00b
        _RVM1(0x0eed, 0, BIT1 | BIT0),   //reg[0eed]#1~#0 = 00b
        //reg_uart_is_gpio_3[3:0]
        _RVM1(0x0eec, 0, 0x3C),   //reg[0eec]#5~#2 = 0000b
        //reg_uart_is_gpio_2[1:0]
        _RVM1(0x0eec, 0, BIT1 | BIT0),   //reg[0eec]#1~#0 = 00b
    #endif

    #if(PAD_GPIO5_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO5_PM_OEN (PAD_GPIO5_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO5_PM_OUT (PAD_GPIO5_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f0a, PAD_GPIO5_PM_OUT, BIT1),
        _RVM1(0x0f0a, PAD_GPIO5_PM_OEN, BIT0),
        //reg_uart_is_gpio_4[3:0]
        _RVM1(0x0eec, 0, BIT7 | BIT6),   //reg[0eec]#7~#6 = 00b
        _RVM1(0x0eed, 0, BIT1 | BIT0),   //reg[0eed]#1~#0 = 00b
        //reg_uart_is_gpio_3[3:0]
        _RVM1(0x0eec, 0, 0x3C),   //reg[0eec]#5~#2 = 0000b
        //reg_uart_is_gpio[3:0]
        _RVM1(0x0e6b, 0, 0x0F),   //reg[0e6b]#3~#0 = 0000b
    #endif

    #if(PAD_GPIO6_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO6_PM_OEN (PAD_GPIO6_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO6_PM_OUT (PAD_GPIO6_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f0c, PAD_GPIO6_PM_OUT, BIT1),
        _RVM1(0x0f0c, PAD_GPIO6_PM_OEN, BIT0),
        //reg_spicsz1_gpio
        _RVM1(0x0e6a, BIT2, BIT2),   //reg[0e6a]#2 = 1b
        //reg_ld_spi2_config
        _RVM1(0x0ee5, 0, BIT6),   //reg[0ee5]#6 = 0b
        //reg_ld_spi3_config
        _RVM1(0x0ee5, 0, BIT7),   //reg[0ee5]#7 = 0b
        //reg_i2s_tx_pm
        _RVM1(0x0ed9, 0, BIT0),   //reg[0ed9]#0 = 0b
    #endif

    #if(PAD_GPIO7_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO7_PM_OEN (PAD_GPIO7_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO7_PM_OUT (PAD_GPIO7_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f0e, PAD_GPIO7_PM_OUT, BIT1),
        _RVM1(0x0f0e, PAD_GPIO7_PM_OEN, BIT0),
        //reg_sd_cdz_mode
        _RVM1(0x0e4f, 0, BIT6),   //reg[0e4f]#6 = 0b
    #endif

    #if(PAD_GPIO8_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO8_PM_OEN (PAD_GPIO8_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO8_PM_OUT (PAD_GPIO8_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f10, PAD_GPIO8_PM_OUT, BIT1),
        _RVM1(0x0f10, PAD_GPIO8_PM_OEN, BIT0),
        //reg_vid_mode[1:0]
        _RVM1(0x0e4f, 0, BIT5 | BIT4),   //reg[0e4f]#5~#4 = 00b
        //reg_uart_is_gpio_4[3:0]
        _RVM1(0x0eec, 0, BIT7 | BIT6),   //reg[0eec]#7~#6 = 00b
        _RVM1(0x0eed, 0, BIT1 | BIT0),   //reg[0eed]#1~#0 = 00b
        //reg_uart_is_gpio_3[3:0]
        _RVM1(0x0eec, 0, 0x3C),   //reg[0eec]#5~#2 = 0000b
        //reg_uart_is_gpio_2[1:0]
        _RVM1(0x0eec, 0, BIT1 | BIT0),   //reg[0eec]#1~#0 = 00b
        //reg_uart_is_gpio[3:0]
        _RVM1(0x0e6b, 0, 0x0F),   //reg[0e6b]#3~#0 = 0000b
    #endif

    #if(PAD_GPIO9_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO9_PM_OEN (PAD_GPIO9_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO9_PM_OUT (PAD_GPIO9_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f12, PAD_GPIO9_PM_OUT, BIT1),
        _RVM1(0x0f12, PAD_GPIO9_PM_OEN, BIT0),
        //reg_mhl_cable_detect_sel
        _RVM1(0x0ee4, 0, BIT6),   //reg[0ee4]#6 = 0b
        //reg_ld_spi1_config
        _RVM1(0x0ee5, 0, BIT5),   //reg[0ee5]#5 = 0b
        //reg_ld_spi3_config
        _RVM1(0x0ee5, 0, BIT7),   //reg[0ee5]#7 = 0b
        //reg_miic_mode0[1:0]
        _RVM1(0x0ec9, 0, BIT7 | BIT6),   //reg[0ec9]#7~#6 = 00b
    #endif

    #if(PAD_GPIO10_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO10_PM_OEN (PAD_GPIO10_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO10_PM_OUT (PAD_GPIO10_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f14, PAD_GPIO10_PM_OUT, BIT1),
        _RVM1(0x0f14, PAD_GPIO10_PM_OEN, BIT0),
        //reg_vbus_en_sel
        _RVM1(0x0ee4, 0, BIT7),   //reg[0ee4]#7 = 0b
    #endif

    #if(PAD_GPIO11_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO11_PM_OEN (PAD_GPIO11_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO11_PM_OUT (PAD_GPIO11_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f16, PAD_GPIO11_PM_OUT, BIT1),
        _RVM1(0x0f16, PAD_GPIO11_PM_OEN, BIT0),
        //reg_ld_spi2_config
        _RVM1(0x0ee5, 0, BIT6),   //reg[0ee5]#6 = 0b
        //reg_ld_spi3_config
        _RVM1(0x0ee5, 0, BIT7),   //reg[0ee5]#7 = 0b
        //reg_uart_is_gpio_4[3:0]
        _RVM1(0x0eec, 0, BIT7 | BIT6),   //reg[0eec]#7~#6 = 00b
        _RVM1(0x0eed, 0, BIT1 | BIT0),   //reg[0eed]#1~#0 = 00b
        //reg_uart_is_gpio_3[3:0]
        _RVM1(0x0eec, 0, 0x3C),   //reg[0eec]#5~#2 = 0000b
        //reg_uart_is_gpio_1[1:0]
        _RVM1(0x0e6b, 0, BIT7 | BIT6),   //reg[0e6b]#7~#6 = 00b
        //reg_i2s_tx_pm
        _RVM1(0x0ed9, 0, BIT0),   //reg[0ed9]#0 = 0b
    #endif

    #if(PAD_GPIO12_PM_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO12_PM_OEN (PAD_GPIO12_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_GPIO12_PM_OUT (PAD_GPIO12_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f18, PAD_GPIO12_PM_OUT, BIT1),
        _RVM1(0x0f18, PAD_GPIO12_PM_OEN, BIT0),
        //reg_ld_spi1_config
        _RVM1(0x0ee5, 0, BIT5),   //reg[0ee5]#5 = 0b
        //reg_ld_spi3_config
        _RVM1(0x0ee5, 0, BIT7),   //reg[0ee5]#7 = 0b
        //rreg_uart_is_gpio_4[3:0]
        _RVM1(0x0eec, 0, BIT7 | BIT6),   //reg[0eec]#7~#6 = 00b
        _RVM1(0x0eed, 0, BIT1 | BIT0),   //reg[0eed]#1~#0 = 00b
        //reg_uart_is_gpio_3[3:0]
        _RVM1(0x0eec, 0, 0x3C),   //reg[0eec]#5~#2 = 0000b
        //reg_uart_is_gpio_1[1:0]
        _RVM1(0x0e6b, 0, BIT7 | BIT6),   //reg[0e6b]#7~#6 = 00b
        //reg_i2s_tx_pm
        _RVM1(0x0ed9, 0, BIT0),   //reg[0ed9]#0 = 0b
    #endif

    #if(PAD_DDCDA_CK_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDA_CK_OEN (PAD_DDCDA_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCDA_CK_OUT (PAD_DDCDA_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x0496, PAD_DDCDA_CK_OUT, BIT2),
        _RVM1(0x0496, PAD_DDCDA_CK_OEN, BIT1),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2do_en
        _RVM1(0x0496, BIT7, BIT7),   //reg[0496]#7 = 1b
    #endif

    #if(PAD_DDCDA_DA_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDA_DA_OEN (PAD_DDCDA_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
        #define PAD_DDCDA_DA_OUT (PAD_DDCDA_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
        _RVM1(0x0496, PAD_DDCDA_DA_OUT, BIT6),
        _RVM1(0x0496, PAD_DDCDA_DA_OEN, BIT5),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2do_en
        _RVM1(0x0496, BIT7, BIT7),   //reg[0496]#7 = 1b
    #endif

    #if(PAD_DDCDB_CK_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDB_CK_OEN (PAD_DDCDB_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCDB_CK_OUT (PAD_DDCDB_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x0497, PAD_DDCDB_CK_OUT, BIT2),
        _RVM1(0x0497, PAD_DDCDB_CK_OEN, BIT1),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2d1_en
        _RVM1(0x0497, BIT7, BIT7),   //reg[0497]#7 = 1b
    #endif

    #if(PAD_DDCDB_DA_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDB_DA_OEN (PAD_DDCDB_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
        #define PAD_DDCDB_DA_OUT (PAD_DDCDB_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
        _RVM1(0x0497, PAD_DDCDB_DA_OUT, BIT6),
        _RVM1(0x0497, PAD_DDCDB_DA_OEN, BIT5),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2d1_en
        _RVM1(0x0497, BIT7, BIT7),   //reg[0497]#7 = 1b
    #endif

    #if(PAD_DDCDC_CK_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDC_CK_OEN (PAD_DDCDC_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCDC_CK_OUT (PAD_DDCDC_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x0498, PAD_DDCDC_CK_OUT, BIT2),
        _RVM1(0x0498, PAD_DDCDC_CK_OEN, BIT1),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2d2_en
        _RVM1(0x0498, BIT7, BIT7),   //reg[0498]#7 = 1b
    #endif

    #if(PAD_DDCDC_DA_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDC_DA_OEN (PAD_DDCDC_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
        #define PAD_DDCDC_DA_OUT (PAD_DDCDC_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
        _RVM1(0x0498, PAD_DDCDC_DA_OUT, BIT6),
        _RVM1(0x0498, PAD_DDCDC_DA_OEN, BIT5),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2d2_en
        _RVM1(0x0498, BIT7, BIT7),   //reg[0498]#7 = 1b
    #endif

    #if(PAD_DDCDD_CK_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDD_CK_OEN (PAD_DDCDD_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCDD_CK_OUT (PAD_DDCDD_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x0499, PAD_DDCDD_CK_OUT, BIT2),
        _RVM1(0x0499, PAD_DDCDD_CK_OEN, BIT1),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2d2_en
        _RVM1(0x0499, BIT7, BIT7),   //Reg[0499]#7 = 1b
    #endif

    #if(PAD_DDCDD_DA_IS_GPIO != GPIO_NONE)
        #define PAD_DDCDD_DA_OEN (PAD_DDCDD_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
        #define PAD_DDCDD_DA_OUT (PAD_DDCDD_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
        _RVM1(0x0499, PAD_DDCDD_DA_OUT, BIT6),
        _RVM1(0x0499, PAD_DDCDD_DA_OEN, BIT5),
        //reg_ej_mode[2:0]
        _RVM1(0x2ec4, 0, 0x07),   //reg[2ec4]#2~#0 = 000b
        //reg_gpio2d2_en
        _RVM1(0x0499, BIT7, BIT7),   //Reg[0499]#7 = 1b
    #endif

    #if(PAD_SAR0_IS_GPIO != GPIO_NONE)
        #define PAD_SAR0_OEN (PAD_SAR0_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_SAR0_OUT (PAD_SAR0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x1424, PAD_SAR0_OUT, BIT0),
        _RVM1(0x1423, PAD_SAR0_OEN, BIT0),
        //reg_sar_aisel[0]
        _RVM1(0x1422, 0, BIT0),   //reg[1422]#0 = 0b
    #endif

    #if(PAD_SAR1_IS_GPIO != GPIO_NONE)
        #define PAD_SAR1_OEN (PAD_SAR1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_SAR1_OUT (PAD_SAR1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x1424, PAD_SAR1_OUT, BIT1),
        _RVM1(0x1423, PAD_SAR1_OEN, BIT1),
        //reg_sar_aisel[1]
        _RVM1(0x1422, 0, BIT1),   //reg[1422]#1 = 0b
    #endif

    #if(PAD_SAR2_IS_GPIO != GPIO_NONE)
        #define PAD_SAR2_OEN (PAD_SAR2_IS_GPIO == GPIO_IN ? BIT2: 0)
        #define PAD_SAR2_OUT (PAD_SAR2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x1424, PAD_SAR2_OUT, BIT2),
        _RVM1(0x1423, PAD_SAR2_OEN, BIT2),
        //reg_sar_aisel[2]
        _RVM1(0x1422, 0, BIT2),   //reg[1422]#2 = 0b
    #endif

    #if(PAD_SAR3_IS_GPIO != GPIO_NONE)
        #define PAD_SAR3_OEN (PAD_SAR3_IS_GPIO == GPIO_IN ? BIT3: 0)
        #define PAD_SAR3_OUT (PAD_SAR3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
        _RVM1(0x1424, PAD_SAR3_OUT, BIT3),
        _RVM1(0x1423, PAD_SAR3_OEN, BIT3),
        //reg_sar_aisel[3]
        _RVM1(0x1422, 0, BIT3),   //reg[1422]#3 = 0b
    #endif

    #if(PAD_SAR4_IS_GPIO != GPIO_NONE)
        #define PAD_SAR4_OEN (PAD_SAR4_IS_GPIO == GPIO_IN ? BIT4: 0)
        #define PAD_SAR4_OUT (PAD_SAR4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
        _RVM1(0x1424, PAD_SAR4_OUT, BIT4),
        _RVM1(0x1423, PAD_SAR4_OEN, BIT4),
        //reg_sar_aisel[4]
        _RVM1(0x1422, 0, BIT4),   //reg[1422]#4 = 0b
    #endif

    #if(PAD_VPLUGIN_IS_GPIO != GPIO_NONE)
        #define PAD_VPLUGIN_OEN (PAD_VPLUGIN_IS_GPIO == GPIO_IN ? BIT5: 0)
        #define PAD_VPLUGIN_OUT (PAD_VPLUGIN_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
        _RVM1(0x1424, PAD_VPLUGIN_OUT, BIT5),
        _RVM1(0x1423, PAD_VPLUGIN_OEN, BIT5),
        //reg_sar_aisel[5]
        _RVM1(0x1422, 0, BIT5),   //reg[1422]#5 = 0b
    #endif

#if 0
    #if(PAD_VID0_IS_GPIO != GPIO_NONE)
        #define PAD_VID0_OEN (PAD_VID0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_VID0_OUT (PAD_VID0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2e84, PAD_VID0_OUT, BIT0),
        _RVM1(0x2e84, PAD_VID0_OEN, BIT1),
        //reg_vid_is_gpio
        _RVM1(0x0e39, BIT2, BIT2),   //reg[0e39]#2 = 1b
        //reg_miic_mode0[1:0]
        _RVM1(0x0ec9, 0, BIT7 | BIT6),   //reg[0ec9]#7~#6 = 00b
    #endif

    #if(PAD_VID1_IS_GPIO != GPIO_NONE)
        #define PAD_VID1_OEN (PAD_VID1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_VID1_OUT (PAD_VID1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2e85, PAD_VID1_OUT, BIT0),
        _RVM1(0x2e85, PAD_VID1_OEN, BIT1),
        //reg_vid_is_gpio
        _RVM1(0x0e39, BIT2, BIT2),   //reg[0e39]#2 = 1b
        //reg_miic_mode0[1:0]
        _RVM1(0x0ec9, 0, BIT7 | BIT6),   //reg[0ec9]#7~#6 = 00b
    #endif

    #if(PAD_VID2_IS_GPIO != GPIO_NONE)
        #define PAD_VID2_OEN (PAD_VID2_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_VID2_OUT (PAD_VID2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f22, PAD_VID2_OUT, BIT1),
        _RVM1(0x0f22, PAD_VID2_OEN, BIT0),
    #endif

    #if(PAD_VID3_IS_GPIO != GPIO_NONE)
        #define PAD_VID3_OEN (PAD_VID3_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_VID3_OUT (PAD_VID3_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f24, PAD_VID3_OUT, BIT1),
        _RVM1(0x0f24, PAD_VID3_OEN, BIT0),
    #endif
#endif

    #if(PAD_WOL_INT_OUT_IS_GPIO != GPIO_NONE)
        #define PAD_WOL_INT_OUT_OEN (PAD_WOL_INT_OUT_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_WOL_INT_OUT_OUT (PAD_WOL_INT_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2e82, PAD_WOL_INT_OUT_OUT, BIT0),
        _RVM1(0x2e82, PAD_WOL_INT_OUT_OEN, BIT1),
        //reg_wol_is_gpio
        _RVM1(0x0e39, BIT1, BIT1),   //reg[0e39]#1 = 1b
    #endif

_MEMMAP_REGBANK_10_,
    #if(PAD_DDCR_CK_IS_GPIO != GPIO_NONE)
        #define PAD_DDCR_CK_OEN (PAD_DDCR_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCR_CK_OUT (PAD_DDCR_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b87, PAD_DDCR_CK_OUT, BIT0),
        _RVM1(0x2b87, PAD_DDCR_CK_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ddcrmode
        _RVM1(0x1eae, 0, BIT1 | BIT0),   //reg[101eae]#1 ~ #0 = 00b
        //reg_ddcrmode
        _RVM1(0x1eae, 0, BIT1 | BIT0),   //reg[101eae]#1 ~ #0 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_DDCR_DA_IS_GPIO != GPIO_NONE)
        #define PAD_DDCR_DA_OEN (PAD_DDCR_DA_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_DDCR_DA_OUT (PAD_DDCR_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b86, PAD_DDCR_DA_OUT, BIT0),
        _RVM1(0x2b86, PAD_DDCR_DA_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ddcrmode
        _RVM1(0x1eae, 0, BIT1 | BIT0),   //reg[101eae]#1 ~ #0 = 00b
        //reg_ddcrmode
        _RVM1(0x1eae, 0, BIT1 | BIT0),   //reg[101eae]#1 ~ #0 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO0_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO0_OEN (PAD_GPIO0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO0_OUT (PAD_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b00, PAD_GPIO0_OUT, BIT0),
        _RVM1(0x2b00, PAD_GPIO0_OEN, BIT1),
        //reg_i2soutconfig4
        _RVM1(0x1eb3, 0, BIT5),   //reg[101eb3]#5 = 0b
        //reg_thirduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_p1_enable_b0
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ea4, 0, BIT0),   //reg[101ea4]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b01, PAD_GPIO1_OUT, BIT0),
        _RVM1(0x2b01, PAD_GPIO1_OEN, BIT1),
        //reg_vsync_like_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, 0x07),   //reg[151e50]#2 ~ #0 = 000b
        //reg_vsync_like_config
        _RVM1(0x1e50, 0, 0x07),   //reg[151e50]#2 ~ #0 = 000b
        //reg_ld_spi3_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_thirduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_fastuartmode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e04, 0, BIT5 | BIT4),   //reg[101e04]#5 ~ #4 = 00b
        //reg_odfastuart
        _RVM1(0x1ea9, 0, BIT7 | BIT6),   //reg[101ea9]#7 ~ #6 = 00b
        //reg_extint4
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e45, 0, BIT1 | BIT0),   //reg[151e45]#1 ~ #0 = 00b
        //reg_p1_enable_b1
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ea4, 0, BIT1),   //reg[101ea4]#1 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO8_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO8_OEN (PAD_GPIO8_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO8_OUT (PAD_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b08, PAD_GPIO8_OUT, BIT0),
        _RVM1(0x2b08, PAD_GPIO8_OEN, BIT1),
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_seconduartmode
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_fifthuartmode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e08, 0, BIT3 | BIT2),   //reg[101e08]#3 ~ #2 = 00b
        //reg_od5thuart
        _RVM1(0x1eaa, 0, BIT5 | BIT4),   //reg[101eaa]#5 ~ #4 = 00b
        //reg_fastuartmode
        _RVM1(0x1e04, 0, BIT5 | BIT4),   //reg[101e04]#5 ~ #4 = 00b
        //reg_odfastuart
        _RVM1(0x1ea9, 0, BIT7 | BIT6),   //reg[101ea9]#7 ~ #6 = 00b
        //reg_p1_enable_b7
        _RVM1(0x1ea4, 0, BIT7),   //reg[101ea4]#7 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO9_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO9_OEN (PAD_GPIO9_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO9_OUT (PAD_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b09, PAD_GPIO9_OUT, BIT0),
        _RVM1(0x2b09, PAD_GPIO9_OEN, BIT1),
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_seconduartmode
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_thirduartmode
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_fifthuartmode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e08, 0, BIT3 | BIT2),   //reg[101e08]#3 ~ #2 = 00b
        //reg_od5thuart
        _RVM1(0x1eaa, 0, BIT5 | BIT4),   //reg[101eaa]#5 ~ #4 = 00b
        //reg_fastuartmode
        _RVM1(0x1e04, 0, BIT5 | BIT4),   //reg[101e04]#5 ~ #4 = 00b
        //reg_odfastuart
        _RVM1(0x1ea9, 0, BIT7 | BIT6),   //reg[101ea9]#7 ~ #6 = 00b
        //reg_tconconfig4
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e42, 0, BIT1 | BIT0),   //reg[151e42]#1 ~ #0 = 00b
        //reg_p1_enable_b6
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ea4, 0, BIT6),   //reg[101ea4]#6 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO10_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO10_OEN (PAD_GPIO10_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO10_OUT (PAD_GPIO10_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b0a, PAD_GPIO10_OUT, BIT0),
        _RVM1(0x2b0a, PAD_GPIO10_OEN, BIT1),
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_et_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_thirduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_tconconfig5
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ea0, 0, BIT5),   //reg[101ea0]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO11_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO11_OEN (PAD_GPIO11_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO11_OUT (PAD_GPIO11_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b0b, PAD_GPIO11_OUT, BIT0),
        _RVM1(0x2b0b, PAD_GPIO11_OEN, BIT1),
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_et_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_fourthuartmode
        _RVM1(0x1e04, 0, BIT7 | BIT6),   //reg[101e04]#7 ~ #6 = 00b
        //reg_od4thuart
        _RVM1(0x1ea9, 0, BIT5 | BIT4),   //reg[101ea9]#5 ~ #4 = 00b
        //reg_tconconfig6
        _RVM1(0x1ea0, 0, BIT6),   //reg[101ea0]#6 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO12_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO12_OEN (PAD_GPIO12_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO12_OUT (PAD_GPIO12_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b0c, PAD_GPIO12_OUT, BIT0),
        _RVM1(0x2b0c, PAD_GPIO12_OEN, BIT1),
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_et_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_fourthuartmode
        _RVM1(0x1e04, 0, BIT7 | BIT6),   //reg[101e04]#7 ~ #6 = 00b
        //reg_od4thuart
        _RVM1(0x1ea9, 0, BIT5 | BIT4),   //reg[101ea9]#5 ~ #4 = 00b
        //reg_miic_mode4
        _RVM1(0x1ede, 0, BIT0),   //reg[101ede]#0 = 0b
        //reg_tconconfig7
        _RVM1(0x1ea0, 0, BIT7),   //reg[101ea0]#7 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO13_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO13_OEN (PAD_GPIO13_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO13_OUT (PAD_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b0d, PAD_GPIO13_OUT, BIT0),
        _RVM1(0x2b0d, PAD_GPIO13_OEN, BIT1),
        //reg_et_mode
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_fastuartmode
        _RVM1(0x1e04, 0, BIT5 | BIT4),   //reg[101e04]#5 ~ #4 = 00b
        //reg_odfastuart
        _RVM1(0x1ea9, 0, BIT7 | BIT6),   //reg[101ea9]#7 ~ #6 = 00b
        //reg_miic_mode4
        _RVM1(0x1ede, 0, BIT0),   //reg[101ede]#0 = 0b
        //reg_extint1
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e44, 0, BIT3 | BIT2),   //reg[151e44]#3 ~ #2 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO14_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO14_OEN (PAD_GPIO14_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO14_OUT (PAD_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b0e, PAD_GPIO14_OUT, BIT0),
        _RVM1(0x2b0e, PAD_GPIO14_OEN, BIT1),
        //reg_et_mode
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_fastuartmode
        _RVM1(0x1e04, 0, BIT5 | BIT4),   //reg[101e04]#5 ~ #4 = 00b
        //reg_odfastuart
        _RVM1(0x1ea9, 0, BIT7 | BIT6),   //reg[101ea9]#7 ~ #6 = 00b
        //reg_miic_mode5
        _RVM1(0x1ede, 0, BIT1),   //reg[101ede]#1 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO15_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO15_OEN (PAD_GPIO15_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO15_OUT (PAD_GPIO15_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b0f, PAD_GPIO15_OUT, BIT0),
        _RVM1(0x2b0f, PAD_GPIO15_OEN, BIT1),
        //reg_agc_dbg
        _RVM1(0x1e9e, 0, BIT7),   //reg[101e9e]#7 = 0b
        //reg_tserrout
        _RVM1(0x1ec9, 0, BIT1 | BIT0),   //reg[101ec9]#1 ~ #0 = 00b
        //reg_diseqc_out_config
        _RVM1(0x1ed0, 0, BIT4),   //reg[101ed0]#4 = 0b
        //reg_i2smutemode
        _RVM1(0x1e05, 0, BIT7 | BIT6),   //reg[101e05]#7 ~ #6 = 00b
        //reg_et_mode
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_fifthuartmode
        _RVM1(0x1e08, 0, BIT3 | BIT2),   //reg[101e08]#3 ~ #2 = 00b
        //reg_od5thuart
        _RVM1(0x1eaa, 0, BIT5 | BIT4),   //reg[101eaa]#5 ~ #4 = 00b
        //reg_miic_mode5
        _RVM1(0x1ede, 0, BIT1),   //reg[101ede]#1 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO16_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO16_OEN (PAD_GPIO16_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO16_OUT (PAD_GPIO16_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b10, PAD_GPIO16_OUT, BIT0),
        _RVM1(0x2b10, PAD_GPIO16_OEN, BIT1),
        //reg_et_mode
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_fifthuartmode
        _RVM1(0x1e08, 0, BIT3 | BIT2),   //reg[101e08]#3 ~ #2 = 00b
        //reg_od5thuart
        _RVM1(0x1eaa, 0, BIT5 | BIT4),   //reg[101eaa]#5 ~ #4 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO17_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO17_OEN (PAD_GPIO17_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO17_OUT (PAD_GPIO17_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b11, PAD_GPIO17_OUT, BIT0),
        _RVM1(0x2b11, PAD_GPIO17_OEN, BIT1),
        //reg_et_mode
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_led_mode
        _RVM1(0x1eb4, 0, BIT1 | BIT0),   //reg[101eb4]#1 ~ #0 = 00b
        //reg_seconduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_fifthuartmode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e08, 0, BIT3 | BIT2),   //reg[101e08]#3 ~ #2 = 00b
        //reg_od5thuart
        _RVM1(0x1eaa, 0, BIT5 | BIT4),   //reg[101eaa]#5 ~ #4 = 00b
        //reg_miic_mode2
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e22, 0, BIT1 | BIT0),   //reg[151e22]#1 ~ #0 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO18_IS_GPIO != GPIO_NONE)
        #define PAD_GPIO18_OEN (PAD_GPIO18_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_GPIO18_OUT (PAD_GPIO18_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b12, PAD_GPIO18_OUT, BIT0),
        _RVM1(0x2b12, PAD_GPIO18_OEN, BIT1),
        //reg_agc_dbg
        _RVM1(0x1e9e, 0, BIT7),   //reg[101e9e]#7 = 0b
        //reg_tserrout
        _RVM1(0x1ec9, 0, BIT1 | BIT0),   //reg[101ec9]#1 ~ #0 = 00b
        //reg_diseqc_in_config
        _RVM1(0x1ed0, 0, BIT2),   //reg[101ed0]#2 = 0b
        //reg_freeze_tuner
        _RVM1(0x1e67, 0, BIT7 | BIT6),   //reg[101e67]#7 ~ #6 = 00b
        //reg_et_mode
        _RVM1(0x1edf, 0, BIT0),   //reg[101edf]#0 = 0b
        //reg_led_mode
        _RVM1(0x1eb4, 0, BIT1 | BIT0),   //reg[101eb4]#1 ~ #0 = 00b
        //reg_seconduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_fifthuartmode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e08, 0, BIT3 | BIT2),   //reg[101e08]#3 ~ #2 = 00b
        //reg_od5thuart
        _RVM1(0x1eaa, 0, BIT5 | BIT4),   //reg[101eaa]#5 ~ #4 = 00b
        //reg_miic_mode2
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e22, 0, BIT1 | BIT0),   //reg[151e22]#1 ~ #0 = 00b
        //reg_extint5
        _RVM1(0x1e45, 0, BIT2),   //reg[151e45]#2 = 0b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_HDMIRX_ARCTX_IS_GPIO != GPIO_NONE)
        #define PAD_HDMIRX_ARCTX_OEN (PAD_HDMIRX_ARCTX_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_HDMIRX_ARCTX_OUT (PAD_HDMIRX_ARCTX_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
_MEMMAP_REGBANK_11_,
        _RVM1(0x0320, PAD_HDMIRX_ARCTX_OUT, BIT0),
        _RVM1(0x0320, PAD_HDMIRX_ARCTX_OEN, BIT1),
        //reg_arc_gpio_en
        _RVM1(0x0320, BIT4, BIT4),   //reg[110320]#4 = 1b
        //reg_arc_mode
        _RVM1(0x0320, 0, BIT3),   //reg[110320]#3 = 0b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_BCK_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_IN_BCK_OEN (PAD_I2S_IN_BCK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_IN_BCK_OUT (PAD_I2S_IN_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b37, PAD_I2S_IN_BCK_OUT, BIT0),
        _RVM1(0x2b37, PAD_I2S_IN_BCK_OEN, BIT1),
        //reg_i2s_in_pe[0]
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e17, BIT0, BIT0),   //reg[151e17]#0 = 1b
        //reg_ej_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_mspi0_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ed1, 0, BIT0),   //reg[101ed1]#0 = 0b
        //reg_mspi1_config
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_thirduartmode
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_miic_mode2
        _RVM1(0x1e22, 0, BIT1 | BIT0),   //reg[151e22]#1 ~ #0 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_DIN0_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_IN_DIN0_OEN (PAD_I2S_IN_DIN0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_IN_DIN0_OUT (PAD_I2S_IN_DIN0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b38, PAD_I2S_IN_DIN0_OUT, BIT0),
        _RVM1(0x2b38, PAD_I2S_IN_DIN0_OEN, BIT1),
        //reg_ej_config
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_mspi0_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ed1, 0, BIT0),   //reg[101ed1]#0 = 0b
        //reg_mspi1_config
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_miic_mode2
        _RVM1(0x1e22, 0, BIT1 | BIT0),   //reg[151e22]#1 ~ #0 = 00b
        //reg_extint6
        _RVM1(0x1e45, 0, BIT5 | BIT4),   //reg[151e45]#5 ~ #4 = 00b
        //reg_3dflagconfig
_MEMMAP_REGBANK_10_,
        _RVM1(0x1eb3, 0, BIT7 | BIT6),   //reg[101eb3]#7 ~ #6 = 00b
        //reg_osd3dflag_config
        _RVM1(0x1ef6, 0, BIT7 | BIT6),   //reg[101ef6]#7 ~ #6 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_DIN1_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_IN_DIN1_OEN (PAD_I2S_IN_DIN1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_IN_DIN1_OUT (PAD_I2S_IN_DIN1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
_MEMMAP_REGBANK_11_,
        _RVM1(0x0346, PAD_I2S_IN_DIN1_OUT, BIT1),
        _RVM1(0x0348, PAD_I2S_IN_DIN1_OEN, BIT1),
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_MCK_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_IN_MCK_OEN (PAD_I2S_IN_MCK_IS_GPIO == GPIO_IN ? BIT0: 0)
        #define PAD_I2S_IN_MCK_OUT (PAD_I2S_IN_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
_MEMMAP_REGBANK_11_,
        _RVM1(0x0346, PAD_I2S_IN_MCK_OUT, BIT0),
        _RVM1(0x0348, PAD_I2S_IN_MCK_OEN, BIT0),
        //reg_i2s_in_pe[1]
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e17, BIT1, BIT1),   //reg[151e17]#1 = 1b
        //reg_ej_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_mspi0_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ed1, 0, BIT0),   //reg[101ed1]#0 = 0b
        //reg_mspi1_config
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_WCK_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_IN_WCK_OEN (PAD_I2S_IN_WCK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_IN_WCK_OUT (PAD_I2S_IN_WCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b36, PAD_I2S_IN_WCK_OUT, BIT0),
        _RVM1(0x2b36, PAD_I2S_IN_WCK_OEN, BIT1),
        //reg_ej_config
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_pdm_md
        _RVM1(0x1e20, 0, BIT1 | BIT0),   //reg[151e20]#1 ~ #0 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_tdm_md
        _RVM1(0x1e20, 0, BIT5 | BIT4),   //reg[151e20]#5 ~ #4 = 00b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_mspi0_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ed1, 0, BIT0),   //reg[101ed1]#0 = 0b
        //reg_mspi1_config
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_thirduartmode
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_extint6
        _RVM1(0x1e45, 0, BIT5 | BIT4),   //reg[151e45]#5 ~ #4 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_BCK_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_OUT_BCK_OEN (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_OUT_BCK_OUT (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b3d, PAD_I2S_OUT_BCK_OUT, BIT0),
        _RVM1(0x2b3d, PAD_I2S_OUT_BCK_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_i2soutconfig0
        _RVM1(0x1eae, 0, BIT4),   //reg[101eae]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_MCK_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_OUT_MCK_OEN (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_OUT_MCK_OUT (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b3c, PAD_I2S_OUT_MCK_OUT, BIT0),
        _RVM1(0x2b3c, PAD_I2S_OUT_MCK_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_i2soutconfig0
        _RVM1(0x1eae, 0, BIT4),   //reg[101eae]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD0_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_OUT_SD0_OEN (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_OUT_SD0_OUT (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b3e, PAD_I2S_OUT_SD0_OUT, BIT0),
        _RVM1(0x2b3e, PAD_I2S_OUT_SD0_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_i2soutconfig1
        _RVM1(0x1eae, 0, BIT5),   //reg[101eae]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD1_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_OUT_SD1_OEN (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_OUT_SD1_OUT (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b3f, PAD_I2S_OUT_SD1_OUT, BIT0),
        _RVM1(0x2b3f, PAD_I2S_OUT_SD1_OEN, BIT1),
        //reg_spdifoutconfig2
        _RVM1(0x1eb3, 0, BIT0),   //reg[101eb3]#0 = 0b
        //reg_i2soutconfig2
        _RVM1(0x1eb3, 0, BIT3),   //reg[101eb3]#3 = 0b
        //reg_fourthuartmode
        _RVM1(0x1e04, 0, BIT7 | BIT6),   //reg[101e04]#7 ~ #6 = 00b
        //reg_od4thuart
        _RVM1(0x1ea9, 0, BIT5 | BIT4),   //reg[101ea9]#5 ~ #4 = 00b
        //reg_extint7
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e45, 0, BIT7 | BIT6),   //reg[151e45]#7 ~ #6 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD2_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_OUT_SD2_OEN (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_OUT_SD2_OUT (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b40, PAD_I2S_OUT_SD2_OUT, BIT0),
        _RVM1(0x2b40, PAD_I2S_OUT_SD2_OEN, BIT1),
        //reg_i2soutconfig3
        _RVM1(0x1eb3, 0, BIT4),   //reg[101eb3]#4 = 0b
        //reg_fourthuartmode
        _RVM1(0x1e04, 0, BIT7 | BIT6),   //reg[101e04]#7 ~ #6 = 00b
        //reg_od4thuart
        _RVM1(0x1ea9, 0, BIT5 | BIT4),   //reg[101ea9]#5 ~ #4 = 00b
        //reg_extint7
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e45, 0, BIT7 | BIT6),   //reg[151e45]#7 ~ #6 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_WCK_IS_GPIO != GPIO_NONE)
        #define PAD_I2S_OUT_WCK_OEN (PAD_I2S_OUT_WCK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_I2S_OUT_WCK_OUT (PAD_I2S_OUT_WCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b3b, PAD_I2S_OUT_WCK_OUT, BIT0),
        _RVM1(0x2b3b, PAD_I2S_OUT_WCK_OEN, BIT1),
        //reg_i2soutconfig0
        _RVM1(0x1eae, 0, BIT4),   //reg[101eae]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_LD_SPI_CK_IS_GPIO != GPIO_NONE)
        #define PAD_LD_SPI_CK_OEN (PAD_LD_SPI_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_LD_SPI_CK_OUT (PAD_LD_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2bb0, PAD_LD_SPI_CK_OUT, BIT0),
        _RVM1(0x2bb0, PAD_LD_SPI_CK_OEN, BIT1),
        //reg_ld_spi_pe[3]
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e1c, BIT3, BIT3),   //reg[151e1c]#3 = 1b
        //reg_ej_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_ld_spi1_config
        _RVM1(0x1e9c, 0, BIT1 | BIT0),   //reg[101e9c]#1 ~ #0 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_fastuartmode
        _RVM1(0x1e04, 0, BIT5 | BIT4),   //reg[101e04]#5 ~ #4 = 00b
        //reg_odfastuart
        _RVM1(0x1ea9, 0, BIT7 | BIT6),   //reg[101ea9]#7 ~ #6 = 00b
        //reg_p1_enable_b5
        _RVM1(0x1ea4, 0, BIT5),   //reg[101ea4]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_LD_SPI_CS_IS_GPIO != GPIO_NONE)
        #define PAD_LD_SPI_CS_OEN (PAD_LD_SPI_CS_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_LD_SPI_CS_OUT (PAD_LD_SPI_CS_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2bb2, PAD_LD_SPI_CS_OUT, BIT0),
        _RVM1(0x2bb2, PAD_LD_SPI_CS_OEN, BIT1),
        //reg_ld_spi_pe[1]
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e1c, BIT1, BIT1),   //reg[151e1c]#1 = 1b
        //reg_ej_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_ld_spi2_config
        _RVM1(0x1e9c, 0, BIT3 | BIT2),   //reg[101e9c]#3 ~ #2 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_seconduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_fourthuartmode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e04, 0, BIT7 | BIT6),   //reg[101e04]#7 ~ #6 = 00b
        //reg_od4thuart
        _RVM1(0x1ea9, 0, BIT5 | BIT4),   //reg[101ea9]#5 ~ #4 = 00b
        //reg_p1_enable_b3
        _RVM1(0x1ea4, 0, BIT3),   //reg[101ea4]#3 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_LD_SPI_MISO_IS_GPIO != GPIO_NONE)
        #define PAD_LD_SPI_MISO_OEN (PAD_LD_SPI_MISO_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_LD_SPI_MISO_OUT (PAD_LD_SPI_MISO_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2bb3, PAD_LD_SPI_MISO_OUT, BIT0),
        _RVM1(0x2bb3, PAD_LD_SPI_MISO_OEN, BIT1),
        //reg_ld_spi_pe[0]
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e1c, BIT0, BIT0),   //reg[151e1c]#0 = 1b
        //reg_ej_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_ld_spi2_config
        _RVM1(0x1e9c, 0, BIT3 | BIT2),   //reg[101e9c]#3 ~ #2 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_fourthuartmode
        _RVM1(0x1e04, 0, BIT7 | BIT6),   //reg[101e04]#7 ~ #6 = 00b
        //reg_od4thuart
        _RVM1(0x1ea9, 0, BIT5 | BIT4),   //reg[101ea9]#5 ~ #4 = 00b
        //reg_p1_enable_b2
        _RVM1(0x1ea4, 0, BIT2),   //reg[101ea4]#2 = 0b
        //reg_3dflagconfig
        _RVM1(0x1eb3, 0, BIT7 | BIT6),   //reg[101eb3]#7 ~ #6 = 00b
        //reg_osd3dflag_config
        _RVM1(0x1ef6, 0, BIT7 | BIT6),   //reg[101ef6]#7 ~ #6 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_LD_SPI_MOSI_IS_GPIO != GPIO_NONE)
        #define PAD_LD_SPI_MOSI_OEN (PAD_LD_SPI_MOSI_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_LD_SPI_MOSI_OUT (PAD_LD_SPI_MOSI_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2bb1, PAD_LD_SPI_MOSI_OUT, BIT0),
        _RVM1(0x2bb1, PAD_LD_SPI_MOSI_OEN, BIT1),
        //reg_ld_spi_pe[2]
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e1c, BIT2, BIT2),   //reg[151e1c]#2 = 1b
        //reg_ej_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1e27, 0, BIT1 | BIT0),   //reg[101e27]#1 ~ #0 = 00b
        //reg_ld_spi1_config
        _RVM1(0x1e9c, 0, BIT1 | BIT0),   //reg[101e9c]#1 ~ #0 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_ld_spi3_config
        _RVM1(0x1e9c, 0, BIT5 | BIT4),   //reg[101e9c]#5 ~ #4 = 00b
        //reg_seconduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_p1_enable_b4
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ea4, 0, BIT4),   //reg[101ea4]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_CD_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM2_CD_N_OEN (PAD_PCM2_CD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM2_CD_N_OUT (PAD_PCM2_CD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b67, PAD_PCM2_CD_N_OUT, BIT0),
        _RVM1(0x2b67, PAD_PCM2_CD_N_OEN, BIT1),
        //reg_pcm_pe[35]
        _RVM1(0x1e16, BIT3, BIT3),   //reg[101e16]#3 = 1b
        //reg_pcm2ctrlconfig
        _RVM1(0x1ec8, 0, BIT3),   //reg[101ec8]#3 = 0b
        //reg_pcm2_cdn_config
        _RVM1(0x1e9e, 0, BIT0),   //reg[101e9e]#0 = 0b
        //reg_extint3
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e44, 0, BIT7 | BIT6),   //reg[151e44]#7 ~ #6 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_CE_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM2_CE_N_OEN (PAD_PCM2_CE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM2_CE_N_OUT (PAD_PCM2_CE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b63, PAD_PCM2_CE_N_OUT, BIT0),
        _RVM1(0x2b63, PAD_PCM2_CE_N_OEN, BIT1),
        //reg_pcm2ctrlconfig
        _RVM1(0x1ec8, 0, BIT3),   //reg[101ec8]#3 = 0b
        //reg_extint0
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e44, 0, BIT0),   //reg[151e44]#0 = 0b
        //reg_sdio_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ef6, 0, BIT4),   //reg[101ef6]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_IRQA_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM2_IRQA_N_OEN (PAD_PCM2_IRQA_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM2_IRQA_N_OUT (PAD_PCM2_IRQA_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b64, PAD_PCM2_IRQA_N_OUT, BIT0),
        _RVM1(0x2b64, PAD_PCM2_IRQA_N_OEN, BIT1),
        //reg_pcm_pe[33]
        _RVM1(0x1e16, BIT1, BIT1),   //reg[101e16]#1 = 1b
        //reg_pcm2ctrlconfig
        _RVM1(0x1ec8, 0, BIT3),   //reg[101ec8]#3 = 0b
        //reg_extint2
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e44, 0, BIT5 | BIT4),   //reg[151e44]#5 ~ #4 = 00b
        //reg_sdio_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ef6, 0, BIT4),   //reg[101ef6]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_RESET_IS_GPIO != GPIO_NONE)
        #define PAD_PCM2_RESET_OEN (PAD_PCM2_RESET_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM2_RESET_OUT (PAD_PCM2_RESET_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b66, PAD_PCM2_RESET_OUT, BIT0),
        _RVM1(0x2b66, PAD_PCM2_RESET_OEN, BIT1),
        //reg_pcm_pe[32]
        _RVM1(0x1e16, BIT0, BIT0),   //reg[101e16]#0 = 1b
        //reg_pcm2ctrlconfig
        _RVM1(0x1ec8, 0, BIT3),   //reg[101ec8]#3 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_WAIT_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM2_WAIT_N_OEN (PAD_PCM2_WAIT_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM2_WAIT_N_OUT (PAD_PCM2_WAIT_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b65, PAD_PCM2_WAIT_N_OUT, BIT0),
        _RVM1(0x2b65, PAD_PCM2_WAIT_N_OEN, BIT1),
        //reg_pcm_pe[34]
        _RVM1(0x1e16, BIT2, BIT2),   //reg[101e16]#2 = 1b
        //reg_pcm2ctrlconfig
        _RVM1(0x1ec8, 0, BIT3),   //reg[101ec8]#3 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A0_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A0_OEN (PAD_PCM_A0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A0_OUT (PAD_PCM_A0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b5d, PAD_PCM_A0_OUT, BIT0),
        _RVM1(0x2b5d, PAD_PCM_A0_OEN, BIT1),
        //reg_pcm_pe[16]
        _RVM1(0x1e14, BIT0, BIT0),   //reg[101e14]#0 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A1_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A1_OEN (PAD_PCM_A1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A1_OUT (PAD_PCM_A1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b5c, PAD_PCM_A1_OUT, BIT0),
        _RVM1(0x2b5c, PAD_PCM_A1_OEN, BIT1),
        //reg_pcm_pe[17]
        _RVM1(0x1e14, BIT1, BIT1),   //reg[101e14]#1 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A2_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A2_OEN (PAD_PCM_A2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A2_OUT (PAD_PCM_A2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b5a, PAD_PCM_A2_OUT, BIT0),
        _RVM1(0x2b5a, PAD_PCM_A2_OEN, BIT1),
        //reg_pcm_pe[18]
        _RVM1(0x1e14, BIT2, BIT2),   //reg[101e14]#2 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A3_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A3_OEN (PAD_PCM_A3_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A3_OUT (PAD_PCM_A3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b59, PAD_PCM_A3_OUT, BIT0),
        _RVM1(0x2b59, PAD_PCM_A3_OEN, BIT1),
        //reg_pcm_pe[19]
        _RVM1(0x1e14, BIT3, BIT3),   //reg[101e14]#3 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A4_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A4_OEN (PAD_PCM_A4_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A4_OUT (PAD_PCM_A4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b58, PAD_PCM_A4_OUT, BIT0),
        _RVM1(0x2b58, PAD_PCM_A4_OEN, BIT1),
        //reg_pcm_pe[20]
        _RVM1(0x1e14, BIT4, BIT4),   //reg[101e14]#4 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A5_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A5_OEN (PAD_PCM_A5_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A5_OUT (PAD_PCM_A5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b56, PAD_PCM_A5_OUT, BIT0),
        _RVM1(0x2b56, PAD_PCM_A5_OEN, BIT1),
        //reg_pcm_pe[21]
        _RVM1(0x1e14, BIT5, BIT5),   //reg[101e14]#5 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A6_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A6_OEN (PAD_PCM_A6_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A6_OUT (PAD_PCM_A6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b55, PAD_PCM_A6_OUT, BIT0),
        _RVM1(0x2b55, PAD_PCM_A6_OEN, BIT1),
        //reg_pcm_pe[22]
        _RVM1(0x1e14, BIT6, BIT6),   //reg[101e14]#6 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A7_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A7_OEN (PAD_PCM_A7_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A7_OUT (PAD_PCM_A7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b54, PAD_PCM_A7_OUT, BIT0),
        _RVM1(0x2b54, PAD_PCM_A7_OEN, BIT1),
        //reg_pcm_pe[23]
        _RVM1(0x1e14, BIT7, BIT7),   //reg[101e14]#7 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_nand_mode
        _RVM1(0x1ede, 0, BIT7 | BIT6),   //reg[101ede]#7 ~ #6 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A8_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A8_OEN (PAD_PCM_A8_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A8_OUT (PAD_PCM_A8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b4e, PAD_PCM_A8_OUT, BIT0),
        _RVM1(0x2b4e, PAD_PCM_A8_OEN, BIT1),
        //reg_pcm_pe[24]
        _RVM1(0x1e15, BIT0, BIT0),   //reg[101e15]#0 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A9_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A9_OEN (PAD_PCM_A9_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A9_OUT (PAD_PCM_A9_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b4c, PAD_PCM_A9_OUT, BIT0),
        _RVM1(0x2b4c, PAD_PCM_A9_OEN, BIT1),
        //reg_pcm_pe[25]
        _RVM1(0x1e15, BIT1, BIT1),   //reg[101e15]#1 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A10_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A10_OEN (PAD_PCM_A10_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A10_OUT (PAD_PCM_A10_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b48, PAD_PCM_A10_OUT, BIT0),
        _RVM1(0x2b48, PAD_PCM_A10_OEN, BIT1),
        //reg_pcm_pe[26]
        _RVM1(0x1e15, BIT2, BIT2),   //reg[101e15]#2 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A11_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A11_OEN (PAD_PCM_A11_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A11_OUT (PAD_PCM_A11_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b4a, PAD_PCM_A11_OUT, BIT0),
        _RVM1(0x2b4a, PAD_PCM_A11_OEN, BIT1),
        //reg_pcm_pe[27]
        _RVM1(0x1e15, BIT3, BIT3),   //reg[101e15]#3 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A12_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A12_OEN (PAD_PCM_A12_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A12_OUT (PAD_PCM_A12_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b53, PAD_PCM_A12_OUT, BIT0),
        _RVM1(0x2b53, PAD_PCM_A12_OEN, BIT1),
        //reg_pcm_pe[28]
        _RVM1(0x1e15, BIT4, BIT4),   //reg[101e15]#4 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A13_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A13_OEN (PAD_PCM_A13_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A13_OUT (PAD_PCM_A13_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b4f, PAD_PCM_A13_OUT, BIT0),
        _RVM1(0x2b4f, PAD_PCM_A13_OEN, BIT1),
        //reg_pcm_pe[29]
        _RVM1(0x1e15, BIT5, BIT5),   //reg[101e15]#5 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A14_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_A14_OEN (PAD_PCM_A14_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_A14_OUT (PAD_PCM_A14_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b50, PAD_PCM_A14_OUT, BIT0),
        _RVM1(0x2b50, PAD_PCM_A14_OEN, BIT1),
        //reg_pcm_pe[30]
        _RVM1(0x1e15, BIT6, BIT6),   //reg[101e15]#6 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_CD_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_CD_N_OEN (PAD_PCM_CD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_CD_N_OUT (PAD_PCM_CD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b62, PAD_PCM_CD_N_OUT, BIT0),
        _RVM1(0x2b62, PAD_PCM_CD_N_OEN, BIT1),
        //reg_pcm_pe[31]
        _RVM1(0x1e15, BIT7, BIT7),   //reg[101e15]#7 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_CE_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_CE_N_OEN (PAD_PCM_CE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_CE_N_OUT (PAD_PCM_CE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b47, PAD_PCM_CE_N_OUT, BIT0),
        _RVM1(0x2b47, PAD_PCM_CE_N_OEN, BIT1),
        //reg_pcm_pe[36]
        _RVM1(0x1e16, BIT4, BIT4),   //reg[101e16]#4 = 1b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D0_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D0_OEN (PAD_PCM_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D0_OUT (PAD_PCM_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b5e, PAD_PCM_D0_OUT, BIT0),
        _RVM1(0x2b5e, PAD_PCM_D0_OEN, BIT1),
        //reg_pcm_pe[0]
        _RVM1(0x1e12, BIT0, BIT0),   //reg[101e12]#0 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D1_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D1_OEN (PAD_PCM_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D1_OUT (PAD_PCM_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b5f, PAD_PCM_D1_OUT, BIT0),
        _RVM1(0x2b5f, PAD_PCM_D1_OEN, BIT1),
        //reg_pcm_pe[1]
        _RVM1(0x1e12, BIT1, BIT1),   //reg[101e12]#1 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D2_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D2_OEN (PAD_PCM_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D2_OUT (PAD_PCM_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b60, PAD_PCM_D2_OUT, BIT0),
        _RVM1(0x2b60, PAD_PCM_D2_OEN, BIT1),
        //reg_pcm_pe[2]
        _RVM1(0x1e12, BIT2, BIT2),   //reg[101e12]#2 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D3_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D3_OEN (PAD_PCM_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D3_OUT (PAD_PCM_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b42, PAD_PCM_D3_OUT, BIT0),
        _RVM1(0x2b42, PAD_PCM_D3_OEN, BIT1),
        //reg_pcm_pe[3]
        _RVM1(0x1e12, BIT3, BIT3),   //reg[101e12]#3 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D4_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D4_OEN (PAD_PCM_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D4_OUT (PAD_PCM_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b43, PAD_PCM_D4_OUT, BIT0),
        _RVM1(0x2b43, PAD_PCM_D4_OEN, BIT1),
        //reg_pcm_pe[4]
        _RVM1(0x1e12, BIT4, BIT4),   //reg[101e12]#4 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D5_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D5_OEN (PAD_PCM_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D5_OUT (PAD_PCM_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b44, PAD_PCM_D5_OUT, BIT0),
        _RVM1(0x2b44, PAD_PCM_D5_OEN, BIT1),
        //reg_pcm_pe[5]
        _RVM1(0x1e12, BIT5, BIT5),   //reg[101e12]#5 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D6_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D6_OEN (PAD_PCM_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D6_OUT (PAD_PCM_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b45, PAD_PCM_D6_OUT, BIT0),
        _RVM1(0x2b45, PAD_PCM_D6_OEN, BIT1),
        //reg_pcm_pe[6]
        _RVM1(0x1e12, BIT6, BIT6),   //reg[101e12]#6 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D7_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_D7_OEN (PAD_PCM_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_D7_OUT (PAD_PCM_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b46, PAD_PCM_D7_OUT, BIT0),
        _RVM1(0x2b46, PAD_PCM_D7_OEN, BIT1),
        //reg_pcm_pe[7]
        _RVM1(0x1e12, BIT7, BIT7),   //reg[101e12]#7 = 1b
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_pcmadconfig
        _RVM1(0x1ec8, 0, BIT4),   //reg[101ec8]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_IORD_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_IORD_N_OEN (PAD_PCM_IORD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_IORD_N_OUT (PAD_PCM_IORD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b4b, PAD_PCM_IORD_N_OUT, BIT0),
        _RVM1(0x2b4b, PAD_PCM_IORD_N_OEN, BIT1),
        //reg_pcm_pe[10]
        _RVM1(0x1e13, BIT2, BIT2),   //reg[101e13]#2 = 1b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_IOWR_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_IOWR_N_OEN (PAD_PCM_IOWR_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_IOWR_N_OUT (PAD_PCM_IOWR_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b4d, PAD_PCM_IOWR_N_OUT, BIT0),
        _RVM1(0x2b4d, PAD_PCM_IOWR_N_OEN, BIT1),
        //reg_pcm_pe[11]
        _RVM1(0x1e13, BIT3, BIT3),   //reg[101e13]#3 = 1b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_IRQA_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_IRQA_N_OEN (PAD_PCM_IRQA_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_IRQA_N_OUT (PAD_PCM_IRQA_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b52, PAD_PCM_IRQA_N_OUT, BIT0),
        _RVM1(0x2b52, PAD_PCM_IRQA_N_OEN, BIT1),
        //reg_pcm_pe[13]
        _RVM1(0x1e13, BIT5, BIT5),   //reg[101e13]#5 = 1b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_OE_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_OE_N_OEN (PAD_PCM_OE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_OE_N_OUT (PAD_PCM_OE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b49, PAD_PCM_OE_N_OUT, BIT0),
        _RVM1(0x2b49, PAD_PCM_OE_N_OEN, BIT1),
        //reg_pcm_pe[9]
        _RVM1(0x1e13, BIT1, BIT1),   //reg[101e13]#1 = 1b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_REG_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_REG_N_OEN (PAD_PCM_REG_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_REG_N_OUT (PAD_PCM_REG_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b5b, PAD_PCM_REG_N_OUT, BIT0),
        _RVM1(0x2b5b, PAD_PCM_REG_N_OEN, BIT1),
        //reg_pcm_pe[15]
        _RVM1(0x1e13, BIT7, BIT7),   //reg[101e13]#7 = 1b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_RESET_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_RESET_OEN (PAD_PCM_RESET_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_RESET_OUT (PAD_PCM_RESET_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b61, PAD_PCM_RESET_OUT, BIT0),
        _RVM1(0x2b61, PAD_PCM_RESET_OEN, BIT1),
        //reg_pcm_pe[8]
        _RVM1(0x1e13, BIT0, BIT0),   //reg[101e13]#0 = 1b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_WAIT_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_WAIT_N_OEN (PAD_PCM_WAIT_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_WAIT_N_OUT (PAD_PCM_WAIT_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b57, PAD_PCM_WAIT_N_OUT, BIT0),
        _RVM1(0x2b57, PAD_PCM_WAIT_N_OEN, BIT1),
        //reg_pcm_pe[14]
        _RVM1(0x1e13, BIT6, BIT6),   //reg[101e13]#6 = 1b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_WE_N_IS_GPIO != GPIO_NONE)
        #define PAD_PCM_WE_N_OEN (PAD_PCM_WE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PCM_WE_N_OUT (PAD_PCM_WE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b51, PAD_PCM_WE_N_OUT, BIT0),
        _RVM1(0x2b51, PAD_PCM_WE_N_OEN, BIT1),
        //reg_pcm_pe[12]
        _RVM1(0x1e13, BIT4, BIT4),   //reg[101e13]#4 = 1b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_pcmctrlconfig
        _RVM1(0x1ec8, 0, BIT5),   //reg[101ec8]#5 = 0b
        //reg_extint4
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e45, 0, BIT1 | BIT0),   //reg[151e45]#1 ~ #0 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM0_IS_GPIO != GPIO_NONE)
        #define PAD_PWM0_OEN (PAD_PWM0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PWM0_OUT (PAD_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b88, PAD_PWM0_OUT, BIT0),
        _RVM1(0x2b88, PAD_PWM0_OEN, BIT1),
        //reg_vsense_pe
_MEMMAP_REGBANK_11_,
        _RVM1(0x0321, BIT1, BIT1),   //reg[110321]#1 = 1b
        //reg_sense_en
        _RVM1(0x0321, 0, BIT0),   //reg[110321]#0 = 0b
        //reg_pwm0_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ec8, 0, BIT2),   //reg[101ec8]#2 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM1_IS_GPIO != GPIO_NONE)
        #define PAD_PWM1_OEN (PAD_PWM1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PWM1_OUT (PAD_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b89, PAD_PWM1_OUT, BIT0),
        _RVM1(0x2b89, PAD_PWM1_OEN, BIT1),
        //reg_vsync_like_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, 0x07),   //reg[151e50]#2 ~ #0 = 000b
        //reg_vsync_like_config
        _RVM1(0x1e50, 0, 0x07),   //reg[151e50]#2 ~ #0 = 000b
        //reg_pwm1_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ec8, 0, BIT6),   //reg[101ec8]#6 = 0b
        //reg_ire_mode
        _RVM1(0x1edf, 0, BIT5 | BIT4),   //reg[101edf]#5 ~ #4 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM2_IS_GPIO != GPIO_NONE)
        #define PAD_PWM2_OEN (PAD_PWM2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_PWM2_OUT (PAD_PWM2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b8a, PAD_PWM2_OUT, BIT0),
        _RVM1(0x2b8a, PAD_PWM2_OEN, BIT1),
        //reg_i2smutemode
        _RVM1(0x1e05, 0, BIT7 | BIT6),   //reg[101e05]#7 ~ #6 = 00b
        //reg_pwm2_mode
        _RVM1(0x1ec9, 0, BIT7 | BIT6),   //reg[101ec9]#7 ~ #6 = 00b
        //reg_ire_mode
        _RVM1(0x1edf, 0, BIT5 | BIT4),   //reg[101edf]#5 ~ #4 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPDIF_IN_IS_GPIO != GPIO_NONE)
        #define PAD_SPDIF_IN_OEN (PAD_SPDIF_IN_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_SPDIF_IN_OUT (PAD_SPDIF_IN_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b39, PAD_SPDIF_IN_OUT, BIT0),
        _RVM1(0x2b39, PAD_SPDIF_IN_OEN, BIT1),
        //reg_spdifinconfig
        _RVM1(0x1e67, 0, BIT4),   //reg[101e67]#4 = 0b
        //reg_3dflagconfig
        _RVM1(0x1eb3, 0, BIT7 | BIT6),   //reg[101eb3]#7 ~ #6 = 00b
        //reg_osd3dflag_config
        _RVM1(0x1ef6, 0, BIT7 | BIT6),   //reg[101ef6]#7 ~ #6 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
        #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b3a, PAD_SPDIF_OUT_OUT, BIT0),
        _RVM1(0x2b3a, PAD_SPDIF_OUT_OEN, BIT1),
        //reg_spdifoutconfig
        _RVM1(0x1eae, 0, BIT7),   //reg[101eae]#7 = 0b
        //reg_extint1
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e44, 0, BIT3 | BIT2),   //reg[151e44]#3 ~ #2 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TCON0_IS_GPIO != GPIO_NONE)
        #define PAD_TCON0_OEN (PAD_TCON0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TCON0_OUT (PAD_TCON0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b93, PAD_TCON0_OUT, BIT0),
        _RVM1(0x2b93, PAD_TCON0_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_led_mode
        _RVM1(0x1eb4, 0, BIT1 | BIT0),   //reg[101eb4]#1 ~ #0 = 00b
        //reg_seconduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_tconconfig0
        _RVM1(0x1e40, 0, BIT1 | BIT0),   //reg[151e40]#1 ~ #0 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TCON1_IS_GPIO != GPIO_NONE)
        #define PAD_TCON1_OEN (PAD_TCON1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TCON1_OUT (PAD_TCON1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b92, PAD_TCON1_OUT, BIT0),
        _RVM1(0x2b92, PAD_TCON1_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_led_mode
        _RVM1(0x1eb4, 0, BIT1 | BIT0),   //reg[101eb4]#1 ~ #0 = 00b
        //reg_seconduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_tconconfig1
        _RVM1(0x1e40, 0, BIT5 | BIT4),   //reg[151e40]#5 ~ #4 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TCON2_IS_GPIO != GPIO_NONE)
        #define PAD_TCON2_OEN (PAD_TCON2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TCON2_OUT (PAD_TCON2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b95, PAD_TCON2_OUT, BIT0),
        _RVM1(0x2b95, PAD_TCON2_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_thirduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_tconconfig2
        _RVM1(0x1e41, 0, BIT1 | BIT0),   //reg[151e41]#1 ~ #0 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TCON3_IS_GPIO != GPIO_NONE)
        #define PAD_TCON3_OEN (PAD_TCON3_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TCON3_OUT (PAD_TCON3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b94, PAD_TCON3_OUT, BIT0),
        _RVM1(0x2b94, PAD_TCON3_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_thirduartmode
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_tconconfig3
        _RVM1(0x1e41, 0, BIT5 | BIT4),   //reg[151e41]#5 ~ #4 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TCON4_IS_GPIO != GPIO_NONE)
        #define PAD_TCON4_OEN (PAD_TCON4_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TCON4_OUT (PAD_TCON4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b97, PAD_TCON4_OUT, BIT0),
        _RVM1(0x2b97, PAD_TCON4_OEN, BIT1),
        //reg_i2sin_sd1
_MEMMAP_REGBANK_15_,
        _RVM1(0x1efc, 0, BIT0),   //reg[151efc]#0 = 0b
        //reg_pwm2_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ec9, 0, BIT7 | BIT6),   //reg[101ec9]#7 ~ #6 = 00b
        //reg_ire_mode
        _RVM1(0x1edf, 0, BIT5 | BIT4),   //reg[101edf]#5 ~ #4 = 00b
        //reg_tconconfig4
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e42, 0, BIT1 | BIT0),   //reg[151e42]#1 ~ #0 = 00b
        //reg_extint3
        _RVM1(0x1e44, 0, BIT7 | BIT6),   //reg[151e44]#7 ~ #6 = 00b
        //reg_allpad_in
_MEMMAP_REGBANK_10_,
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO0_IS_GPIO != GPIO_NONE)
        #define PAD_TGPIO0_OEN (PAD_TGPIO0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TGPIO0_OUT (PAD_TGPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b8d, PAD_TGPIO0_OUT, BIT0),
        _RVM1(0x2b8d, PAD_TGPIO0_OEN, BIT1),
        //reg_vsync_vif_out_en
        _RVM1(0x1ea3, 0, BIT6),   //reg[101ea3]#6 = 0b
        //reg_freeze_tuner
        _RVM1(0x1e67, 0, BIT7 | BIT6),   //reg[101e67]#7 ~ #6 = 00b
        //reg_miic_mode0
        _RVM1(0x1edc, 0, BIT0),   //reg[101edc]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO1_IS_GPIO != GPIO_NONE)
        #define PAD_TGPIO1_OEN (PAD_TGPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TGPIO1_OUT (PAD_TGPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b8e, PAD_TGPIO1_OUT, BIT0),
        _RVM1(0x2b8e, PAD_TGPIO1_OEN, BIT1),
        //reg_freeze_tuner
        _RVM1(0x1e67, 0, BIT7 | BIT6),   //reg[101e67]#7 ~ #6 = 00b
        //reg_miic_mode0
        _RVM1(0x1edc, 0, BIT0),   //reg[101edc]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO2_IS_GPIO != GPIO_NONE)
        #define PAD_TGPIO2_OEN (PAD_TGPIO2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TGPIO2_OUT (PAD_TGPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b8f, PAD_TGPIO2_OUT, BIT0),
        _RVM1(0x2b8f, PAD_TGPIO2_OEN, BIT1),
        //reg_sixthuartmode
        _RVM1(0x1e08, 0, BIT6),   //reg[101e08]#6 = 0b
        //reg_od6thuart
        _RVM1(0x1eaa, 0, BIT6),   //reg[101eaa]#6 = 0b
        //reg_miic_mode1
        _RVM1(0x1edc, 0, BIT1),   //reg[101edc]#1 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO3_IS_GPIO != GPIO_NONE)
        #define PAD_TGPIO3_OEN (PAD_TGPIO3_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TGPIO3_OUT (PAD_TGPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b90, PAD_TGPIO3_OUT, BIT0),
        _RVM1(0x2b90, PAD_TGPIO3_OEN, BIT1),
        //reg_sixthuartmode
        _RVM1(0x1e08, 0, BIT6),   //reg[101e08]#6 = 0b
        //reg_od6thuart
        _RVM1(0x1eaa, 0, BIT6),   //reg[101eaa]#6 = 0b
        //reg_miic_mode1
        _RVM1(0x1edc, 0, BIT1),   //reg[101edc]#1 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b26, PAD_TS0_CLK_OUT, BIT0),
        _RVM1(0x2b26, PAD_TS0_CLK_OEN, BIT1),
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b1c, PAD_TS0_D0_OUT, BIT0),
        _RVM1(0x2b1c, PAD_TS0_D0_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b1d, PAD_TS0_D1_OUT, BIT0),
        _RVM1(0x2b1d, PAD_TS0_D1_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b1e, PAD_TS0_D2_OUT, BIT0),
        _RVM1(0x2b1e, PAD_TS0_D2_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b1f, PAD_TS0_D3_OUT, BIT0),
        _RVM1(0x2b1f, PAD_TS0_D3_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b20, PAD_TS0_D4_OUT, BIT0),
        _RVM1(0x2b20, PAD_TS0_D4_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b21, PAD_TS0_D5_OUT, BIT0),
        _RVM1(0x2b21, PAD_TS0_D5_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b22, PAD_TS0_D6_OUT, BIT0),
        _RVM1(0x2b22, PAD_TS0_D6_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b23, PAD_TS0_D7_OUT, BIT0),
        _RVM1(0x2b23, PAD_TS0_D7_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b25, PAD_TS0_SYNC_OUT, BIT0),
        _RVM1(0x2b25, PAD_TS0_SYNC_OEN, BIT1),
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
        #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b24, PAD_TS0_VLD_OUT, BIT0),
        _RVM1(0x2b24, PAD_TS0_VLD_OEN, BIT1),
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_ts0config
        _RVM1(0x1eaf, 0, 0x07),   //reg[101eaf]#2 ~ #0 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_CLK_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_CLK_OEN (PAD_TS1_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_CLK_OUT (PAD_TS1_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b27, PAD_TS1_CLK_OUT, BIT0),
        _RVM1(0x2b27, PAD_TS1_CLK_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_tconconfig10
        _RVM1(0x1ea1, 0, BIT2),   //reg[101ea1]#2 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D0_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D0_OEN (PAD_TS1_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D0_OUT (PAD_TS1_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b31, PAD_TS1_D0_OUT, BIT0),
        _RVM1(0x2b31, PAD_TS1_D0_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D1_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D1_OEN (PAD_TS1_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D1_OUT (PAD_TS1_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b30, PAD_TS1_D1_OUT, BIT0),
        _RVM1(0x2b30, PAD_TS1_D1_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D2_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D2_OEN (PAD_TS1_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D2_OUT (PAD_TS1_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b2f, PAD_TS1_D2_OUT, BIT0),
        _RVM1(0x2b2f, PAD_TS1_D2_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D3_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D3_OEN (PAD_TS1_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D3_OUT (PAD_TS1_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b2e, PAD_TS1_D3_OUT, BIT0),
        _RVM1(0x2b2e, PAD_TS1_D3_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D4_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D4_OEN (PAD_TS1_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D4_OUT (PAD_TS1_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b2d, PAD_TS1_D4_OUT, BIT0),
        _RVM1(0x2b2d, PAD_TS1_D4_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D5_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D5_OEN (PAD_TS1_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D5_OUT (PAD_TS1_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b2c, PAD_TS1_D5_OUT, BIT0),
        _RVM1(0x2b2c, PAD_TS1_D5_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D6_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D6_OEN (PAD_TS1_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D6_OUT (PAD_TS1_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b2b, PAD_TS1_D6_OUT, BIT0),
        _RVM1(0x2b2b, PAD_TS1_D6_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_sm_config
        _RVM1(0x1edc, 0, BIT5 | BIT4),   //reg[101edc]#5 ~ #4 = 00b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D7_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_D7_OEN (PAD_TS1_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_D7_OUT (PAD_TS1_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b2a, PAD_TS1_D7_OUT, BIT0),
        _RVM1(0x2b2a, PAD_TS1_D7_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_SYNC_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_SYNC_OEN (PAD_TS1_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_SYNC_OUT (PAD_TS1_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b28, PAD_TS1_SYNC_OUT, BIT0),
        _RVM1(0x2b28, PAD_TS1_SYNC_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_tconconfig9
        _RVM1(0x1ea1, 0, BIT1),   //reg[101ea1]#1 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_VLD_IS_GPIO != GPIO_NONE)
        #define PAD_TS1_VLD_OEN (PAD_TS1_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS1_VLD_OUT (PAD_TS1_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b29, PAD_TS1_VLD_OUT, BIT0),
        _RVM1(0x2b29, PAD_TS1_VLD_OEN, BIT1),
        //reg_test_in_mode
        //_RVM1(0x1e24, 0, BIT1 | BIT0),   //reg[101e24]#1 ~ #0 = 00b
        //reg_test_out_mode
        //_RVM1(0x1e24, 0, BIT5 | BIT4),   //reg[101e24]#5 ~ #4 = 00b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts1config
        _RVM1(0x1eaf, 0, 0x38),   //reg[101eaf]#5 ~ #3 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_ts_out_mode
        _RVM1(0x1e80, 0, 0x70),   //reg[101e80]#6 ~ #4 = 000b
        //reg_i2sout_in_tcon
        _RVM1(0x1e81, 0, BIT0),   //reg[101e81]#0 = 0b
        //reg_tconconfig8
        _RVM1(0x1ea1, 0, BIT0),   //reg[101ea1]#0 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_CLK_IS_GPIO != GPIO_NONE)
        #define PAD_TS2_CLK_OEN (PAD_TS2_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS2_CLK_OUT (PAD_TS2_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b35, PAD_TS2_CLK_OUT, BIT0),
        _RVM1(0x2b35, PAD_TS2_CLK_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_led_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1eb4, 0, BIT1 | BIT0),   //reg[101eb4]#1 ~ #0 = 00b
        //reg_mspi1_config
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_seconduartmode
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_tconconfig0
        _RVM1(0x1e40, 0, BIT1 | BIT0),   //reg[151e40]#1 ~ #0 = 00b
        //reg_sdio_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ef6, 0, BIT4),   //reg[101ef6]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D0_IS_GPIO != GPIO_NONE)
        #define PAD_TS2_D0_OEN (PAD_TS2_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS2_D0_OUT (PAD_TS2_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b32, PAD_TS2_D0_OUT, BIT0),
        _RVM1(0x2b32, PAD_TS2_D0_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_mspi1_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_thirduartmode
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_tconconfig3
        _RVM1(0x1e41, 0, BIT5 | BIT4),   //reg[151e41]#5 ~ #4 = 00b
        //reg_extint2
        _RVM1(0x1e44, 0, BIT5 | BIT4),   //reg[151e44]#5 ~ #4 = 00b
        //reg_sdio_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ef6, 0, BIT4),   //reg[101ef6]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_SYNC_IS_GPIO != GPIO_NONE)
        #define PAD_TS2_SYNC_OEN (PAD_TS2_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS2_SYNC_OUT (PAD_TS2_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b34, PAD_TS2_SYNC_OUT, BIT0),
        _RVM1(0x2b34, PAD_TS2_SYNC_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_led_mode
_MEMMAP_REGBANK_10_,
        _RVM1(0x1eb4, 0, BIT1 | BIT0),   //reg[101eb4]#1 ~ #0 = 00b
        //reg_mspi1_config
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_seconduartmode
        _RVM1(0x1e53, 0, 0x07),   //reg[151e53]#2 ~ #0 = 000b
        //reg_od2nduart
        _RVM1(0x1e46, 0, 0x07),   //reg[151e46]#2 ~ #0 = 000b
        //reg_tconconfig1
        _RVM1(0x1e40, 0, BIT5 | BIT4),   //reg[151e40]#5 ~ #4 = 00b
        //reg_sdio_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ef6, 0, BIT4),   //reg[101ef6]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_VLD_IS_GPIO != GPIO_NONE)
        #define PAD_TS2_VLD_OEN (PAD_TS2_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
        #define PAD_TS2_VLD_OUT (PAD_TS2_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2b33, PAD_TS2_VLD_OUT, BIT0),
        _RVM1(0x2b33, PAD_TS2_VLD_OEN, BIT1),
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_ts2config
        _RVM1(0x1eaf, 0, BIT7 | BIT6),   //reg[101eaf]#7 ~ #6 = 00b
        //reg_i2sinconfig
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2sinconfig
        _RVM1(0x1e21, 0, 0x07),   //reg[151e21]#2 ~ #0 = 000b
        //reg_i2s_bt_md
        _RVM1(0x1e21, 0, BIT5 | BIT4),   //reg[151e21]#5 ~ #4 = 00b
        //reg_mspi1_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ed1, 0, BIT2 | BIT1),   //reg[101ed1]#2 ~ #1 = 00b
        //reg_mspi_tslink_config
_MEMMAP_REGBANK_15_,
        _RVM1(0x1e50, 0, BIT5 | BIT4),   //reg[151e50]#5 ~ #4 = 00b
        //reg_thirduartmode
        _RVM1(0x1e53, 0, 0x70),   //reg[151e53]#6 ~ #4 = 000b
        //reg_od3rduart
        _RVM1(0x1e46, 0, 0x70),   //reg[151e46]#6 ~ #4 = 000b
        //reg_tconconfig2
        _RVM1(0x1e41, 0, BIT1 | BIT0),   //reg[151e41]#1 ~ #0 = 00b
        //reg_sdio_config
_MEMMAP_REGBANK_10_,
        _RVM1(0x1ef6, 0, BIT4),   //reg[101ef6]#4 = 0b
        //reg_allpad_in
        //_RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------

_MEMMAP_REGBANK_10_,

//---------------------------------------------------------------------
// JTAG
#ifdef PADS_EJ_CONFIG
#if (PADS_EJ_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EJ_CONFIG_MODE1 ((PADS_EJ_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                (PADS_EJ_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                                (PADS_EJ_CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : 0)
    _RVM1(0x1e27, CONFIG_EJ_CONFIG_MODE1, BITMASK(1:0)),
#endif
#endif

// AGC Debug
#ifdef PADS_AGC_DBG
#if (PADS_AGC_DBG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_AGC_DBG_MODE1 ((PADS_AGC_DBG == CONFIG_PADMUX_MODE1) ? (0x01 << 7) : 0)
    _RVM1(0x1e9e, CONFIG_AGC_DBG_MODE1, BIT7),
#endif
#endif

// Tserr Out
#ifdef PADS_TSERROUT
#if (PADS_TSERROUT != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TSERROUT_MODE1 ((PADS_TSERROUT == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                               (PADS_TSERROUT == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
    _RVM1(0x1ec9, CONFIG_TSERROUT_MODE1, BITMASK(1:0)),
#endif
#endif

// VIF VSYNC
#ifdef PADS_VSYNC_VIF_OUT_EN
#if (PADS_VSYNC_VIF_OUT_EN != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_VSYNC_VIF_OUT_EN_MODE1 ((PADS_VSYNC_VIF_OUT_EN == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1ea3, CONFIG_VSYNC_VIF_OUT_EN_MODE1, BIT6),
#endif
#endif

// DISEQC OUT
#ifdef PADS_DISEQC_OUT_CONFIG
#if (PADS_DISEQC_OUT_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_DISEQC_OUT_CONFIG_MODE1 ((PADS_DISEQC_OUT_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1ed0, CONFIG_DISEQC_OUT_CONFIG_MODE1, BIT4),
#endif
#endif

// DISEQC IN
#ifdef PADS_DISEQC_IN_CONFIG
#if (PADS_DISEQC_IN_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_DISEQC_IN_CONFIG_MODE1 ((PADS_DISEQC_IN_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
    _RVM1(0x1ed0, CONFIG_DISEQC_IN_CONFIG_MODE1, BIT2),
#endif
#endif

// Freeze Tuner
#ifdef PADS_FREEZE_TUNER
#if (PADS_FREEZE_TUNER != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_FREEZE_TUNER_MODE1 ((PADS_FREEZE_TUNER == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                   (PADS_FREEZE_TUNER == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                   (PADS_FREEZE_TUNER == CONFIG_PADMUX_MODE2) ? (0x03 << 6) : 0)
    _RVM1(0x1e67, CONFIG_FREEZE_TUNER_MODE1, BITMASK(7:6)),
#endif
#endif

// ts0 = 1p in
#ifdef PADS_TS0CONFIG
#if (PADS_TS0CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TS0CONFIG_MODE1 ((PADS_TS0CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                (PADS_TS0CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                                (PADS_TS0CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : \
                                (PADS_TS0CONFIG == CONFIG_PADMUX_MODE4) ? (0x04 << 0) : 0)
    _RVM1(0x1eaf, CONFIG_TS0CONFIG_MODE1, BITMASK(2:0)),
#endif
#endif

// ts1 = 1p in
#ifdef PADS_TS1CONFIG
#if (PADS_TS1CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TS1CONFIG_MODE1 ((PADS_TS1CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 3) : \
                                (PADS_TS1CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 3) : \
                                (PADS_TS1CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 3) : \
                                (PADS_TS1CONFIG == CONFIG_PADMUX_MODE4) ? (0x04 << 3) : \
                                (PADS_TS1CONFIG == CONFIG_PADMUX_MODE5) ? (0x05 << 3) : 0)
    _RVM1(0x1eaf, CONFIG_TS1CONFIG_MODE1, BITMASK(5:3)),
#endif
#endif

// None
#ifdef PADS_TS_OUT_MODE
#if (PADS_TS_OUT_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TS_OUT_MODE_MODE1 ((PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                  (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                  (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : \
                                  (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE4) ? (0x04 << 4) : \
                                  (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE5) ? (0x05 << 4) : 0)
    _RVM1(0x1e80, CONFIG_TS_OUT_MODE_MODE1, BITMASK(6:4)),
#endif
#endif

// ts2 = 1s in
#ifdef PADS_TS2CONFIG
#if (PADS_TS2CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TS2CONFIG_MODE1 ((PADS_TS2CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                (PADS_TS2CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                (PADS_TS2CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1eaf, CONFIG_TS2CONFIG_MODE1, BITMASK(7:6)),
#endif
#endif

#if 0
// SDR/DDR  1.8V/3.3V, DQS at PAD_EMMC_IO8
#ifdef PADS_NAND_MODE
#if (PADS_NAND_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_NAND_MODE_MODE1 ((PADS_NAND_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                (PADS_NAND_MODE == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                (PADS_NAND_MODE == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1ede, CONFIG_NAND_MODE_MODE1, BITMASK(7:6)),
#endif
#endif

// NAND CS1
#ifdef PADS_NAND_CS1_EN
#if (PADS_NAND_CS1_EN != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_NAND_CS1_EN_MODE1 ((PADS_NAND_CS1_EN == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1ede, CONFIG_NAND_CS1_EN_MODE1, BIT5),
#endif
#endif
#endif

// SMART Card
#ifdef PADS_SM_CONFIG
#if (PADS_SM_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SM_CONFIG_MODE1 ((PADS_SM_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                (PADS_SM_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
    _RVM1(0x1edc, CONFIG_SM_CONFIG_MODE1, BITMASK(5:4)),
#endif
#endif

// PCM1 CTRL
#ifdef PADS_PCMCTRLCONFIG
#if (PADS_PCMCTRLCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PCMCTRLCONFIG_MODE1 ((PADS_PCMCTRLCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1ec8, CONFIG_PCMCTRLCONFIG_MODE1, BIT5),
#endif
#endif

// PCM1  AD
#ifdef PADS_PCMADCONFIG
#if (PADS_PCMADCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PCMADCONFIG_MODE1 ((PADS_PCMADCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1ec8, CONFIG_PCMADCONFIG_MODE1, BIT4),
#endif
#endif

// PCM2 CTRL
#ifdef PADS_PCM2CTRLCONFIG
#if (PADS_PCM2CTRLCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PCM2CTRLCONFIG_MODE1 ((PADS_PCM2CTRLCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 3) : 0)
    _RVM1(0x1ec8, CONFIG_PCM2CTRLCONFIG_MODE1, BIT3),
#endif
#endif

// PCM2 CTRL
#ifdef PADS_PCM2_CDN_CONFIG
#if (PADS_PCM2_CDN_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PCM2_CDN_CONFIG_MODE1 ((PADS_PCM2_CDN_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1e9e, CONFIG_PCM2_CDN_CONFIG_MODE1, BIT0),
#endif
#endif

// SPDIF OUT
#ifdef PADS_SPDIFOUTCONFIG
#if (PADS_SPDIFOUTCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SPDIFOUTCONFIG_MODE1 ((PADS_SPDIFOUTCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 7) : 0)
    _RVM1(0x1eae, CONFIG_SPDIFOUTCONFIG_MODE1, BIT7),
#endif
#endif

// SPDIF IN
#ifdef PADS_SPDIFINCONFIG
#if (PADS_SPDIFINCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SPDIFINCONFIG_MODE1 ((PADS_SPDIFINCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1e67, CONFIG_SPDIFINCONFIG_MODE1, BIT4),
#endif
#endif

// 2nd SPDIF OUT
#ifdef PADS_SPDIFOUTCONFIG2
#if (PADS_SPDIFOUTCONFIG2 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SPDIFOUTCONFIG2_MODE1 ((PADS_SPDIFOUTCONFIG2 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1eb3, CONFIG_SPDIFOUTCONFIG2_MODE1, BIT0),
#endif
#endif

// I2S Rx 2ch
#ifdef PADS_I2SINCONFIG
#if (PADS_I2SINCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SINCONFIG_MODE1 ((PADS_I2SINCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
(PADS_I2SINCONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
(PADS_I2SINCONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : \
(PADS_I2SINCONFIG == CONFIG_PADMUX_MODE4) ? (0x04 << 0) : \
(PADS_I2SINCONFIG == CONFIG_PADMUX_MODE5) ? (0x05 << 0) : \
(PADS_I2SINCONFIG == CONFIG_PADMUX_MODE6) ? (0x06 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e21, CONFIG_I2SINCONFIG_MODE1, BITMASK(2:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// 3rd I2S Rx 6ch
#ifdef PADS_I2SIN_SD1
#if (PADS_I2SIN_SD1 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SIN_SD1_MODE1 ((PADS_I2SIN_SD1 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1efc, CONFIG_I2SIN_SD1_MODE1, BIT0),
_MEMMAP_REGBANK_10_,
#endif
#endif

// Dig Mic 8ch
#ifdef PADS_I2S_PDM_MD
#if (PADS_I2S_PDM_MD != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2S_PDM_MD_MODE1 ((PADS_I2S_PDM_MD == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                 (PADS_I2S_PDM_MD == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                                 (PADS_I2S_PDM_MD == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e20, CONFIG_I2S_PDM_MD_MODE1, BITMASK(1:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// I2S Rx 4ch
#ifdef PADS_I2S_TDM_MD
#if (PADS_I2S_TDM_MD != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2S_TDM_MD_MODE1 ((PADS_I2S_TDM_MD == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                 (PADS_I2S_TDM_MD == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e20, CONFIG_I2S_TDM_MD_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// BT TRx 2ch
#ifdef PADS_I2S_BT_MD
#if (PADS_I2S_BT_MD != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2S_BT_MD_MODE1 ((PADS_I2S_BT_MD == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                (PADS_I2S_BT_MD == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                (PADS_I2S_BT_MD == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e21, CONFIG_I2S_BT_MD_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// I2S MUTE
#ifdef PADS_I2SMUTEMODE
#if (PADS_I2SMUTEMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SMUTEMODE_MODE1 ((PADS_I2SMUTEMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                  (PADS_I2SMUTEMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : 0)
    _RVM1(0x1e05, CONFIG_I2SMUTEMODE_MODE1, BITMASK(7:6)),
#endif
#endif

// I2S OUT at TS1
#ifdef PADS_I2SOUT_IN_TCON
#if (PADS_I2SOUT_IN_TCON != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SOUT_IN_TCON_MODE1 ((PADS_I2SOUT_IN_TCON == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1e81, CONFIG_I2SOUT_IN_TCON_MODE1, BIT0),
#endif
#endif

// I2S OUT
#ifdef PADS_I2SOUTCONFIG0
#if (PADS_I2SOUTCONFIG0 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SOUTCONFIG0_MODE1 ((PADS_I2SOUTCONFIG0 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1eae, CONFIG_I2SOUTCONFIG0_MODE1, BIT4),
#endif
#endif

// I2S OUT SD
#ifdef PADS_I2SOUTCONFIG1
#if (PADS_I2SOUTCONFIG1 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SOUTCONFIG1_MODE1 ((PADS_I2SOUTCONFIG1 == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1eae, CONFIG_I2SOUTCONFIG1_MODE1, BIT5),
#endif
#endif

// I2S OUT SD1
#ifdef PADS_I2SOUTCONFIG2
#if (PADS_I2SOUTCONFIG2 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SOUTCONFIG2_MODE1 ((PADS_I2SOUTCONFIG2 == CONFIG_PADMUX_MODE1) ? (0x01 << 3) : 0)
    _RVM1(0x1eb3, CONFIG_I2SOUTCONFIG2_MODE1, BIT3),
#endif
#endif

// I2S OUT SD2
#ifdef PADS_I2SOUTCONFIG3
#if (PADS_I2SOUTCONFIG3 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SOUTCONFIG3_MODE1 ((PADS_I2SOUTCONFIG3 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1eb3, CONFIG_I2SOUTCONFIG3_MODE1, BIT4),
#endif
#endif

// I2S OUT SD3
#ifdef PADS_I2SOUTCONFIG4
#if (PADS_I2SOUTCONFIG4 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_I2SOUTCONFIG4_MODE1 ((PADS_I2SOUTCONFIG4 == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1eb3, CONFIG_I2SOUTCONFIG4_MODE1, BIT5),
#endif
#endif

// External ETH PHY
#ifdef PADS_ET_MODE
#if (PADS_ET_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_ET_MODE_MODE1 ((PADS_ET_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1edf, CONFIG_ET_MODE_MODE1, BIT0),
#endif
#endif

// LED Mode
#ifdef PADS_LED_MODE
#if (PADS_LED_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_LED_MODE_MODE1 ((PADS_LED_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                               (PADS_LED_MODE == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                               (PADS_LED_MODE == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : 0)
    _RVM1(0x1eb4, CONFIG_LED_MODE_MODE1, BITMASK(1:0)),
#endif
#endif

// LDM_SPI0
#ifdef PADS_VSYNC_LIKE_CONFIG
#if (PADS_VSYNC_LIKE_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_VSYNC_LIKE_CONFIG_MODE1 ((PADS_VSYNC_LIKE_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                        (PADS_VSYNC_LIKE_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                                        (PADS_VSYNC_LIKE_CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : \
                                        (PADS_VSYNC_LIKE_CONFIG == CONFIG_PADMUX_MODE4) ? (0x04 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e50, CONFIG_VSYNC_LIKE_CONFIG_MODE1, BITMASK(2:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// 2-wired MSPI
#ifdef PADS_LD_SPI1_CONFIG
#if (PADS_LD_SPI1_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_LD_SPI1_CONFIG_MODE1 ((PADS_LD_SPI1_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : \
                                     (PADS_LD_SPI1_CONFIG == CONFIG_PADMUX_MODE2) ? (0x01 << 5) : 0)
_MEMMAP_REGBANK_00_,
    _RVM1(0x1e50, CONFIG_LD_SPI1_CONFIG_MODE1, BIT5),
_MEMMAP_REGBANK_10_,
#endif
#endif

// 2-wired MSPI
#ifdef PADS_LD_SPI2_CONFIG
#if (PADS_LD_SPI2_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_LD_SPI2_CONFIG_MODE1 ((PADS_LD_SPI2_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                     (PADS_LD_SPI2_CONFIG == CONFIG_PADMUX_MODE2) ? (0x01 << 6) : 0)
_MEMMAP_REGBANK_00_,
    _RVM1(0x1e50, CONFIG_LD_SPI2_CONFIG_MODE1, BIT6),
_MEMMAP_REGBANK_10_,
#endif
#endif

// 4wired MSPI for LXX FCIC/DEMURA or CHN Local Dimming
#ifdef PADS_LD_SPI3_CONFIG
#if (PADS_LD_SPI3_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_LD_SPI3_CONFIG_MODE1 ((PADS_LD_SPI3_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 7) : \
                                     (PADS_LD_SPI3_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 7) : \
                                     (PADS_LD_SPI3_CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 7) : 0)
_MEMMAP_REGBANK_00_,
    _RVM1(0x1e50, CONFIG_LD_SPI3_CONFIG_MODE1, BIT7),
_MEMMAP_REGBANK_10_,
#endif
#endif

// MSPI0
#ifdef PADS_MSPI0_CONFIG
#if (PADS_MSPI0_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MSPI0_CONFIG_MODE1 ((PADS_MSPI0_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1ed1, CONFIG_MSPI0_CONFIG_MODE1, BIT0),
#endif
#endif

// MSPI1
#ifdef PADS_MSPI1_CONFIG
#if (PADS_MSPI1_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MSPI1_CONFIG_MODE1 ((PADS_MSPI1_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 1) : \
                                   (PADS_MSPI1_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 1) : 0)
    _RVM1(0x1ed1, CONFIG_MSPI1_CONFIG_MODE1, BITMASK(2:1)),
#endif
#endif

// MSPI_TS_LINK
#ifdef PADS_MSPI_TSLINK_CONFIG
#if (PADS_MSPI_TSLINK_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MSPI_TSLINK_CONFIG_MODE1 ((PADS_MSPI_TSLINK_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                         (PADS_MSPI_TSLINK_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e50, CONFIG_MSPI_TSLINK_CONFIG_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// UART2
#ifdef PADS_SECONDUARTMODE
#if (PADS_SECONDUARTMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SECONDUARTMODE_MODE1 ((PADS_SECONDUARTMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                     (PADS_SECONDUARTMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                                     (PADS_SECONDUARTMODE == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : \
                                     (PADS_SECONDUARTMODE == CONFIG_PADMUX_MODE4) ? (0x04 << 0) : \
                                     (PADS_SECONDUARTMODE == CONFIG_PADMUX_MODE5) ? (0x05 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e53, CONFIG_SECONDUARTMODE_MODE1, BITMASK(2:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// OD UART2
#ifdef PADS_OD2NDUART
#if (PADS_OD2NDUART != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_OD2NDUART_MODE1 ((PADS_OD2NDUART == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                (PADS_OD2NDUART == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : \
                                (PADS_OD2NDUART == CONFIG_PADMUX_MODE3) ? (0x03 << 0) : \
                                (PADS_OD2NDUART == CONFIG_PADMUX_MODE4) ? (0x04 << 0) : \
                                (PADS_OD2NDUART == CONFIG_PADMUX_MODE5) ? (0x05 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e46, CONFIG_OD2NDUART_MODE1, BITMASK(2:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// UART3
#ifdef PADS_THIRDUARTMODE
#if (PADS_THIRDUARTMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_THIRDUARTMODE_MODE1 ((PADS_THIRDUARTMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                    (PADS_THIRDUARTMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                    (PADS_THIRDUARTMODE == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : \
                                    (PADS_THIRDUARTMODE == CONFIG_PADMUX_MODE4) ? (0x04 << 4) : \
                                    (PADS_THIRDUARTMODE == CONFIG_PADMUX_MODE5) ? (0x05 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e53, CONFIG_THIRDUARTMODE_MODE1, BITMASK(6:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// OD UART3
#ifdef PADS_OD3RDUART
#if (PADS_OD3RDUART != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_OD3RDUART_MODE1 ((PADS_OD3RDUART == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                (PADS_OD3RDUART == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                (PADS_OD3RDUART == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : \
                                (PADS_OD3RDUART == CONFIG_PADMUX_MODE4) ? (0x04 << 4) : \
                                (PADS_OD3RDUART == CONFIG_PADMUX_MODE5) ? (0x05 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e46, CONFIG_OD3RDUART_MODE1, BITMASK(6:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// UART4
#ifdef PADS_FOURTHUARTMODE
#if (PADS_FOURTHUARTMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_FOURTHUARTMODE_MODE1 ((PADS_FOURTHUARTMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                     (PADS_FOURTHUARTMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                     (PADS_FOURTHUARTMODE == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1e04, CONFIG_FOURTHUARTMODE_MODE1, BITMASK(7:6)),
#endif
#endif

// OD UART4
#ifdef PADS_OD4THUART
#if (PADS_OD4THUART != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_OD4THUART_MODE1 ((PADS_OD4THUART == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                (PADS_OD4THUART == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                (PADS_OD4THUART == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : 0)
    _RVM1(0x1ea9, CONFIG_OD4THUART_MODE1, BITMASK(5:4)),
#endif
#endif

// UART5
#ifdef PADS_FIFTHUARTMODE
#if (PADS_FIFTHUARTMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_FIFTHUARTMODE_MODE1 ((PADS_FIFTHUARTMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : \
                                    (PADS_FIFTHUARTMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 2) : \
                                    (PADS_FIFTHUARTMODE == CONFIG_PADMUX_MODE3) ? (0x03 << 2) : 0)
    _RVM1(0x1e08, CONFIG_FIFTHUARTMODE_MODE1, BITMASK(3:2)),
#endif
#endif

// OD UART5
#ifdef PADS_OD5THUART
#if (PADS_OD5THUART != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_OD5THUART_MODE1 ((PADS_OD5THUART == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                (PADS_OD5THUART == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                (PADS_OD5THUART == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : 0)
    _RVM1(0x1eaa, CONFIG_OD5THUART_MODE1, BITMASK(5:4)),
#endif
#endif

// UART6
#ifdef PADS_SIXTHUARTMODE
#if (PADS_SIXTHUARTMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SIXTHUARTMODE_MODE1 ((PADS_SIXTHUARTMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1e08, CONFIG_SIXTHUARTMODE_MODE1, BIT6),
#endif
#endif

// OD UART6
#ifdef PADS_OD6THUART
#if (PADS_OD6THUART != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_OD6THUART_MODE1 ((PADS_OD6THUART == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1eaa, CONFIG_OD6THUART_MODE1, BIT6),
#endif
#endif

// Fast UART
#ifdef PADS_FASTUARTMODE
#if (PADS_FASTUARTMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_FASTUARTMODE_MODE1 ((PADS_FASTUARTMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                   (PADS_FASTUARTMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                                   (PADS_FASTUARTMODE == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : 0)
    _RVM1(0x1e04, CONFIG_FASTUARTMODE_MODE1, BITMASK(5:4)),
#endif
#endif

// Fast UART
#ifdef PADS_ODFASTUART
#if (PADS_ODFASTUART != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_ODFASTUART_MODE1 ((PADS_ODFASTUART == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                 (PADS_ODFASTUART == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                 (PADS_ODFASTUART == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1ea9, CONFIG_ODFASTUART_MODE1, BITMASK(7:6)),
#endif
#endif

// PWM0
#ifdef PADS_PWM0_MODE
#if (PADS_PWM0_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PWM0_MODE_MODE1 ((PADS_PWM0_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
    _RVM1(0x1ec8, CONFIG_PWM0_MODE_MODE1, BIT2),
#endif
#endif

// PWM1
#ifdef PADS_PWM1_MODE
#if (PADS_PWM1_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PWM1_MODE_MODE1 ((PADS_PWM1_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1ec8, CONFIG_PWM1_MODE_MODE1, BIT6),
#endif
#endif

// PWM2
#ifdef PADS_PWM2_MODE
#if (PADS_PWM2_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_PWM2_MODE_MODE1 ((PADS_PWM2_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                (PADS_PWM2_MODE == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : 0)
    _RVM1(0x1ec9, CONFIG_PWM2_MODE_MODE1, BITMASK(7:6)),
#endif
#endif

// IRE
#ifdef PADS_IRE_MODE
#if (PADS_IRE_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_IRE_MODE_MODE1 ((PADS_IRE_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                               (PADS_IRE_MODE == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : \
                               (PADS_IRE_MODE == CONFIG_PADMUX_MODE3) ? (0x03 << 4) : 0)
    _RVM1(0x1edf, CONFIG_IRE_MODE_MODE1, BITMASK(5:4)),
#endif
#endif

// DDCR
#ifdef PADS_DDCRMODE
#if (PADS_DDCRMODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_DDCRMODE_MODE1 ((PADS_DDCRMODE == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                               (PADS_DDCRMODE == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
    _RVM1(0x1eae, CONFIG_DDCRMODE_MODE1, BITMASK(1:0)),
#endif
#endif

// MIIC0
#ifdef PADS_MIIC_MODE0
#if (PADS_MIIC_MODE0 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MIIC_MODE0_MODE1 ((PADS_MIIC_MODE0 == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
_MEMMAP_REGBANK_00_,
    _RVM1(0x1eae, CONFIG_MIIC_MODE0_MODE1, BITMASK(7:6)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// MIIC1
#ifdef PADS_MIIC_MODE1
#if (PADS_MIIC_MODE1 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MIIC_MODE1_MODE1 ((PADS_MIIC_MODE1 == CONFIG_PADMUX_MODE1) ? (0x01 << 1) : 0)
    _RVM1(0x1edc, CONFIG_MIIC_MODE1_MODE1, BIT1),
#endif
#endif

// MIIC2
#ifdef PADS_MIIC_MODE2
#if (PADS_MIIC_MODE2 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MIIC_MODE2_MODE1 ((PADS_MIIC_MODE2 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                 (PADS_MIIC_MODE2 == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e22, CONFIG_MIIC_MODE2_MODE1, BITMASK(1:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// MIIC4
#ifdef PADS_MIIC_MODE4
#if (PADS_MIIC_MODE4 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MIIC_MODE4_MODE1 ((PADS_MIIC_MODE4 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1ede, CONFIG_MIIC_MODE4_MODE1, BIT0),
#endif
#endif

// MIIC5
#ifdef PADS_MIIC_MODE5
#if (PADS_MIIC_MODE5 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_MIIC_MODE5_MODE1 ((PADS_MIIC_MODE5 == CONFIG_PADMUX_MODE1) ? (0x01 << 1) : 0)
    _RVM1(0x1ede, CONFIG_MIIC_MODE5_MODE1, BIT1),
#endif
#endif

// TCON0
#ifdef PADS_TCONCONFIG0
#if (PADS_TCONCONFIG0 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG0_MODE1 ((PADS_TCONCONFIG0 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                  (PADS_TCONCONFIG0 == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e40, CONFIG_TCONCONFIG0_MODE1, BITMASK(1:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// TCON1
#ifdef PADS_TCONCONFIG1
#if (PADS_TCONCONFIG1 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG1_MODE1 ((PADS_TCONCONFIG1 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                  (PADS_TCONCONFIG1 == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e40, CONFIG_TCONCONFIG1_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// TCON2
#ifdef PADS_TCONCONFIG2
#if (PADS_TCONCONFIG2 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG2_MODE1 ((PADS_TCONCONFIG2 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                  (PADS_TCONCONFIG2 == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e41, CONFIG_TCONCONFIG2_MODE1, BITMASK(1:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// TCON3
#ifdef PADS_TCONCONFIG3
#if (PADS_TCONCONFIG3 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG3_MODE1 ((PADS_TCONCONFIG3 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                                  (PADS_TCONCONFIG3 == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e41, CONFIG_TCONCONFIG3_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// TCON4
#ifdef PADS_TCONCONFIG4
#if (PADS_TCONCONFIG4 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG4_MODE1 ((PADS_TCONCONFIG4 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                                  (PADS_TCONCONFIG4 == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e42, CONFIG_TCONCONFIG4_MODE1, BITMASK(1:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// TCON5
#ifdef PADS_TCONCONFIG5
#if (PADS_TCONCONFIG5 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG5_MODE1 ((PADS_TCONCONFIG5 == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1ea0, CONFIG_TCONCONFIG5_MODE1, BIT5),
#endif
#endif

// TCON6
#ifdef PADS_TCONCONFIG6
#if (PADS_TCONCONFIG6 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG6_MODE1 ((PADS_TCONCONFIG6 == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1ea0, CONFIG_TCONCONFIG6_MODE1, BIT6),
#endif
#endif

// TCON7
#ifdef PADS_TCONCONFIG7
#if (PADS_TCONCONFIG7 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG7_MODE1 ((PADS_TCONCONFIG7 == CONFIG_PADMUX_MODE1) ? (0x01 << 7) : 0)
    _RVM1(0x1ea0, CONFIG_TCONCONFIG7_MODE1, BIT7),
#endif
#endif

// TCON8
#ifdef PADS_TCONCONFIG8
#if (PADS_TCONCONFIG8 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG8_MODE1 ((PADS_TCONCONFIG8 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1ea1, CONFIG_TCONCONFIG8_MODE1, BIT0),
#endif
#endif

// TCON9
#ifdef PADS_TCONCONFIG9
#if (PADS_TCONCONFIG9 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG9_MODE1 ((PADS_TCONCONFIG9 == CONFIG_PADMUX_MODE1) ? (0x01 << 1) : 0)
    _RVM1(0x1ea1, CONFIG_TCONCONFIG9_MODE1, BIT1),
#endif
#endif

// TCON10
#ifdef PADS_TCONCONFIG10
#if (PADS_TCONCONFIG10 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_TCONCONFIG10_MODE1 ((PADS_TCONCONFIG10 == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
    _RVM1(0x1ea1, CONFIG_TCONCONFIG10_MODE1, BIT2),
#endif
#endif

// EXT INT0
#ifdef PADS_EXTINT0
#if (PADS_EXTINT0 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT0_MODE1 ((PADS_EXTINT0 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e44, CONFIG_EXTINT0_MODE1, BIT0),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT1
#ifdef PADS_EXTINT1
#if (PADS_EXTINT1 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT1_MODE1 ((PADS_EXTINT1 == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : \
                              (PADS_EXTINT1 == CONFIG_PADMUX_MODE2) ? (0x02 << 2) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e44, CONFIG_EXTINT1_MODE1, BITMASK(3:2)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT2
#ifdef PADS_EXTINT2
#if (PADS_EXTINT2 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT2_MODE1 ((PADS_EXTINT2 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                              (PADS_EXTINT2 == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e44, CONFIG_EXTINT2_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT3
#ifdef PADS_EXTINT3
#if (PADS_EXTINT3 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT3_MODE1 ((PADS_EXTINT3 == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                              (PADS_EXTINT3 == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e44, CONFIG_EXTINT3_MODE1, BITMASK(7:6)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT4
#ifdef PADS_EXTINT4
#if (PADS_EXTINT4 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT4_MODE1 ((PADS_EXTINT4 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : \
                              (PADS_EXTINT4 == CONFIG_PADMUX_MODE2) ? (0x02 << 0) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e45, CONFIG_EXTINT4_MODE1, BITMASK(1:0)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT5
#ifdef PADS_EXTINT5
#if (PADS_EXTINT5 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT5_MODE1 ((PADS_EXTINT5 == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e45, CONFIG_EXTINT5_MODE1, BIT2),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT6
#ifdef PADS_EXTINT6
#if (PADS_EXTINT6 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT6_MODE1 ((PADS_EXTINT6 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : \
                              (PADS_EXTINT6 == CONFIG_PADMUX_MODE2) ? (0x02 << 4) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e45, CONFIG_EXTINT6_MODE1, BITMASK(5:4)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EXT INT7
#ifdef PADS_EXTINT7
#if (PADS_EXTINT7 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EXTINT7_MODE1 ((PADS_EXTINT7 == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                              (PADS_EXTINT7 == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : 0)
_MEMMAP_REGBANK_15_,
    _RVM1(0x1e45, CONFIG_EXTINT7_MODE1, BITMASK(7:6)),
_MEMMAP_REGBANK_10_,
#endif
#endif

// PM51 GPIO0
#ifdef PADS_P1_ENABLE_B0
#if (PADS_P1_ENABLE_B0 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B0_MODE1 ((PADS_P1_ENABLE_B0 == CONFIG_PADMUX_MODE1) ? (0x01 << 0) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B0_MODE1, BIT0),
#endif
#endif

// PM51 GPIO1
#ifdef PADS_P1_ENABLE_B1
#if (PADS_P1_ENABLE_B1 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B1_MODE1 ((PADS_P1_ENABLE_B1 == CONFIG_PADMUX_MODE1) ? (0x01 << 1) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B1_MODE1, BIT1),
#endif
#endif

// PM51 GPIO2
#ifdef PADS_P1_ENABLE_B2
#if (PADS_P1_ENABLE_B2 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B2_MODE1 ((PADS_P1_ENABLE_B2 == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B2_MODE1, BIT2),
#endif
#endif

// PM51 GPIO3
#ifdef PADS_P1_ENABLE_B3
#if (PADS_P1_ENABLE_B3 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B3_MODE1 ((PADS_P1_ENABLE_B3 == CONFIG_PADMUX_MODE1) ? (0x01 << 3) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B3_MODE1, BIT3),
#endif
#endif

// PM51 GPIO4
#ifdef PADS_P1_ENABLE_B4
#if (PADS_P1_ENABLE_B4 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B4_MODE1 ((PADS_P1_ENABLE_B4 == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B4_MODE1, BIT4),
#endif
#endif

// PM51 GPIO5
#ifdef PADS_P1_ENABLE_B5
#if (PADS_P1_ENABLE_B5 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B5_MODE1 ((PADS_P1_ENABLE_B5 == CONFIG_PADMUX_MODE1) ? (0x01 << 5) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B5_MODE1, BIT5),
#endif
#endif

// PM51 GPIO6
#ifdef PADS_P1_ENABLE_B6
#if (PADS_P1_ENABLE_B6 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B6_MODE1 ((PADS_P1_ENABLE_B6 == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B6_MODE1, BIT6),
#endif
#endif

// PM51 GPIO7
#ifdef PADS_P1_ENABLE_B7
#if (PADS_P1_ENABLE_B7 != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_P1_ENABLE_B7_MODE1 ((PADS_P1_ENABLE_B7 == CONFIG_PADMUX_MODE1) ? (0x01 << 7) : 0)
    _RVM1(0x1ea4, CONFIG_P1_ENABLE_B7_MODE1, BIT7),
#endif
#endif

// SDIO
#ifdef PADS_SDIO_CONFIG
#if (PADS_SDIO_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_SDIO_CONFIG_MODE1 ((PADS_SDIO_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
    _RVM1(0x1ef6, CONFIG_SDIO_CONFIG_MODE1, BIT4),
#endif
#endif

#if 0
// EMMC RST
#ifdef PADS_EMMC_RSTN_EN_PM
#if (PADS_EMMC_RSTN_EN_PM != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EMMC_RSTN_EN_PM_MODE1 ((PADS_EMMC_RSTN_EN_PM == CONFIG_PADMUX_MODE1) ? (0x01 << 4) : 0)
_MEMMAP_REGBANK_00_,
    _RVM1(0x0e62, CONFIG_EMMC_RSTN_EN_PM_MODE1, BIT4),
_MEMMAP_REGBANK_10_,
#endif
#endif

// EMMC RST
#ifdef PADS_EMMC_RSTN_EN
#if (PADS_EMMC_RSTN_EN != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EMMC_RSTN_EN_MODE1 ((PADS_EMMC_RSTN_EN == CONFIG_PADMUX_MODE1) ? (0x01 << 2) : 0)
    _RVM1(0x1e9e, CONFIG_EMMC_RSTN_EN_MODE1, BIT2),
#endif
#endif

// EMMC MODE
#ifdef PADS_EMMC_CONFIG
#if (PADS_EMMC_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_EMMC_CONFIG_MODE1 ((PADS_EMMC_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : 0)
    _RVM1(0x1edc, CONFIG_EMMC_CONFIG_MODE1, BITMASK(7:6)),
#endif
#endif
#endif

// 3DFLAG mode
#ifdef PADS_3DFLAGCONFIG
#if (PADS_3DFLAGCONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_3DFLAGCONFIG_MODE1 ((PADS_3DFLAGCONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                   (PADS_3DFLAGCONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                   (PADS_3DFLAGCONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1eb3, CONFIG_3DFLAGCONFIG_MODE1, BITMASK(7:6)),
#endif
#endif

// 3DFLAG mode
#ifdef PADS_OSD3DFLAG_CONFIG
#if (PADS_OSD3DFLAG_CONFIG != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_OSD3DFLAG_CONFIG_MODE1 ((PADS_OSD3DFLAG_CONFIG == CONFIG_PADMUX_MODE1) ? (0x01 << 6) : \
                                       (PADS_OSD3DFLAG_CONFIG == CONFIG_PADMUX_MODE2) ? (0x02 << 6) : \
                                       (PADS_OSD3DFLAG_CONFIG == CONFIG_PADMUX_MODE3) ? (0x03 << 6) : 0)
    _RVM1(0x1ef6, CONFIG_OSD3DFLAG_CONFIG_MODE1, BITMASK(7:6)),
#endif
#endif

// ARC Mode
#ifdef PADS_ARC_MODE
#if (PADS_ARC_MODE != CONFIG_PADMUX_UNKNOWN)
#define CONFIG_ARC_MODE_MODE1 ((PADS_ARC_MODE == CONFIG_PADMUX_MODE1) ? (0x01 << 3) : 0)
_MEMMAP_REGBANK_11_,
    _RVM1(0x0320, CONFIG_ARC_MODE_MODE1, BIT3),
_MEMMAP_REGBANK_10_,
#endif
#endif

//=============================================================================
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),             //reg[101ea1]#7 = 0b
    _END_OF_TBL_,
};

const U8 analogPadInitTbl[] =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
    1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
    0x00,                       // nor
    0x00,                       // nand
    0x00,                       // reserved
    0x00,                       // reserved
    0x00,                       // reserved

//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------
    _MEMMAP_REGBANK_10_,

    #if (PADA_RIN1P_IS_GPIO != GPIO_NONE)
        #define PADA_RIN1P_OEN (PADA_RIN1P_IS_GPIO == GPIO_IN ? 0: BIT1)
        #define PADA_RIN1P_OUT (PADA_RIN1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x2537, PADA_RIN1P_OUT, BIT1),
        _RVM1(0x2536, PADA_RIN1P_OEN, BIT1),
        //reg_settings
        _RVM1(0x2534, BIT1, BIT1),   //reg[102534]#1 = 1b
        _RVM1(0x2503, 0, BIT6),      //reg[102503]#6 = 0b
    #endif

    #if (PADA_GIN1P_IS_GPIO != GPIO_NONE)
        #define PADA_GIN1P_OEN (PADA_GIN1P_IS_GPIO == GPIO_IN ? 0: BIT1)
        #define PADA_GIN1P_OUT (PADA_GIN1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x2539, PADA_GIN1P_OUT, BIT1),
        _RVM1(0x2538, PADA_GIN1P_OEN, BIT1),
        //reg_settings
        _RVM1(0x2534, BIT1, BIT1),   //reg[102534]#1 = 1b
        _RVM1(0x2503, 0, BIT6),      //reg[102503]#6 = 0b
    #endif

    #if (PADA_GIN1M_IS_GPIO != GPIO_NONE)
        #define PADA_GIN1M_OEN (PADA_GIN1M_IS_GPIO == GPIO_IN ? 0: BIT5)
        #define PADA_GIN1M_OUT (PADA_GIN1M_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
        _RVM1(0x2539, PADA_GIN1M_OUT, BIT5),
        _RVM1(0x2538, PADA_GIN1M_OEN, BIT5),
        //reg_settings
        _RVM1(0x2534, BIT1, BIT1),   //reg[102534]#1 = 1b
        _RVM1(0x2503, 0, BIT6),      //reg[102503]#6 = 0b
    #endif

    #if (PADA_BIN1P_IS_GPIO != GPIO_NONE)
        #define PADA_BIN1P_OEN (PADA_BIN1P_IS_GPIO == GPIO_IN ? 0: BIT1)
        #define PADA_BIN1P_OUT (PADA_BIN1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x253B, PADA_BIN1P_OUT, BIT1),
        _RVM1(0x253A, PADA_BIN1P_OEN, BIT1),
        //reg_settings
        _RVM1(0x2534, BIT1, BIT1),   //reg[102534]#1 = 1b
        _RVM1(0x2503, 0, BIT6),      //reg[102503]#6 = 0b
    #endif

    #if (PADA_HSYNC1_IS_GPIO != GPIO_NONE)
        #define PADA_HSYNC1_OEN (PADA_HSYNC1_IS_GPIO == GPIO_IN ? 0: BIT1)
        #define PADA_HSYNC1_OUT (PADA_HSYNC1_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
        _RVM1(0x255a, PADA_HSYNC1_OUT, BIT7),
        _RVM1(0x255a, PADA_HSYNC1_OEN, BIT1),
        //reg_settings
        _RVM1(0x2558, BIT1, BIT1),   //reg[102558]#1 = 1b
    #endif

    #if (PADA_VSYNC1_IS_GPIO != GPIO_NONE)
        #define PADA_VSYNC1_OEN (PADA_VSYNC1_IS_GPIO == GPIO_IN ? 0: BIT4)
        #define PADA_VSYNC1_OUT (PADA_VSYNC1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
        _RVM1(0x255b, PADA_VSYNC1_OUT, BIT2),
        _RVM1(0x255a, PADA_VSYNC1_OEN, BIT4),
        //reg_settings
        _RVM1(0x2558, BIT4, BIT4),   //reg[102558]#4 = 1b
    #endif

    _MEMMAP_REGBANK_11_,
    #if (PADA_LINEIN_L0_IS_GPIO != GPIO_NONE)
        #define PADA_LINEIN_L0_OEN (PADA_LINEIN_L0_IS_GPIO == GPIO_IN ? 0: BIT2)
        #define PADA_LINEIN_L0_OUT (PADA_LINEIN_L0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
        _RVM1(0x2EDA, PADA_LINEIN_L0_OUT, BIT0),
        _RVM1(0x2EDA, PADA_LINEIN_L0_OEN, BIT2),
        //reg_settings
        _RVM1(0x2EDA, BIT1, BIT1),   //reg[112EDA]#1 = 1b
    #endif

    #if (PADA_LINEIN_R0_IS_GPIO != GPIO_NONE)
        #define PADA_LINEIN_R0_OEN (PADA_LINEIN_R0_IS_GPIO == GPIO_IN ? 0: BIT6)
        #define PADA_LINEIN_R0_OUT (PADA_LINEIN_R0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
        _RVM1(0x2EDA, PADA_LINEIN_R0_OUT, BIT4),
        _RVM1(0x2EDA, PADA_LINEIN_R0_OEN, BIT6),
        //reg_settings
        _RVM1(0x2EDA, BIT5, BIT5),   //reg[112EDA]#5 = 1b
    #endif
    _MEMMAP_REGBANK_10_,

//=============================================================================
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),             //reg[101ea1]#7 = 0b
    _END_OF_TBL_,
};
