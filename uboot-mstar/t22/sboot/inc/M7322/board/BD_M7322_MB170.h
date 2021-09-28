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

#ifndef _MSBOARD_H_
#define _MSBOARD_H_

#define BOARD_NAME                              "MST148B_10NQAT_18115_M7322"

//=============================================================================
#define PIN_SPI_CZ1                             0
#define PIN_SPI_CZ2                             0
#define PIN_SPI_CZ3                             0
#define PIN_FLASH_WP0                           0
#define PIN_FLASH_WP1                           0

//=============================================================================
#define CONFIG_PADMUX_MODE0                     0x00
#define CONFIG_PADMUX_MODE1                     0x01
#define CONFIG_PADMUX_MODE2                     0x02
#define CONFIG_PADMUX_MODE3                     0x03
#define CONFIG_PADMUX_MODE4                     0x04
#define CONFIG_PADMUX_MODE5                     0x05
#define CONFIG_PADMUX_MODE6                     0x06
#define CONFIG_PADMUX_MODE7                     0x07
#define CONFIG_PADMUX_MODE8                     0x08
#define CONFIG_PADMUX_MODE9                     0x09
#define CONFIG_PADMUX_UNKNOWN                   0xFF

//=============================================================================
// PADMUX Configuration
#if defined(CONFIG_NAND_FLASH)
#define PADS_NAND_MODE                          CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: SDR/DDR  1.8V/3.3V, DQS at PAD_EMMC_IO8
                                                                        //CONFIG_PADMUX_MODE2: NAND ((3D NAND BGA), NV-DDR2, (1.8V), DQS at PAD_EMMC_IO8
                                                                        //CONFIG_PADMUX_MODE3: SDR 3.3V

#define PADS_NAND_CS1_EN                        DISABLE                 //ENABLE / DISABLE
#define PADS_EMMC_CONFIG                        DISABLE                 //CONFIG_PADMUX_MODE1 / DISABLE
#define PADS_EMMC_RSTN_EN                       DISABLE                 //ENABLE / DISABLE
#elif defined(CONFIG_MMC)
#define PADS_NAND_MODE                          DISABLE                 //CONFIG_PADMUX_MODE1: SDR/DDR  1.8V/3.3V, DQS at PAD_EMMC_IO8
                                                                        //CONFIG_PADMUX_MODE2: NAND ((3D NAND BGA), NV-DDR2, (1.8V), DQS at PAD_EMMC_IO8
                                                                        //CONFIG_PADMUX_MODE3: SDR 3.3V

#define PADS_NAND_CS1_EN                        DISABLE                 //ENABLE / DISABLE
#define PADS_EMMC_CONFIG                        CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1 / DISABLE
#define PADS_EMMC_RSTN_EN                       ENABLE                  //ENABLE / DISABLE
#else
#define PADS_NAND_MODE                          DISABLE                 //CONFIG_PADMUX_MODE1: SDR/DDR  1.8V/3.3V, DQS at PAD_EMMC_IO8
                                                                        //CONFIG_PADMUX_MODE2: NAND ((3D NAND BGA), NV-DDR2, (1.8V), DQS at PAD_EMMC_IO8
                                                                        //CONFIG_PADMUX_MODE3: SDR 3.3V
#define PADS_EMMC_CONFIG                        DISABLE                 //CONFIG_PADMUX_MODE1 / DISABLE
#endif

#define PADS_EJ_CONFIG                          DISABLE                 //CONFIG_PADMUX_MODE1 / CONFIG_PADMUX_MODE2 / CONFIG_PADMUX_MODE3 / DISABLE
#define PADS_AGC_DBG                            DISABLE                 //CONFIG_PADMUX_MODE1 / DISABLE
#define PADS_TSERROUT                           DISABLE                 //CONFIG_PADMUX_MODE1 /CONFIG_PADMUX_MODE2 / DISABLE
#define PADS_VSYNC_VIF_OUT_EN                   DISABLE                 //CONFIG_PADMUX_MODE1 / DISABLE
#define PADS_DISEQC_OUT_CONFIG                  DISABLE                 //CONFIG_PADMUX_MODE1 / DISABLE
#define PADS_DISEQC_IN_CONFIG                   DISABLE                 //CONFIG_PADMUX_MODE1 / DISABLE
#define PADS_FREEZE_TUNER                       DISABLE                 //CONFIG_PADMUX_MODE1 / CONFIG_PADMUX_MODE2 / CONFIG_PADMUX_MODE3 / DISABLE //Freeze Tuner
#define PADS_TS0CONFIG                          CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: ts0 = 1p in
                                                                        //CONFIG_PADMUX_MODE2: ts0 = 1s in
                                                                        //CONFIG_PADMUX_MODE3: ts0 = mspi
                                                                        //CONFIG_PADMUX_MODE4: ts0 = 1s in (3 wire)

#define PADS_TS1CONFIG                          CONFIG_PADMUX_MODE2     //CONFIG_PADMUX_MODE1: ts1 = 1p in
                                                                        //CONFIG_PADMUX_MODE2: ts1 = 1p out
                                                                        //CONFIG_PADMUX_MODE3: ts1 = 1s in
                                                                        //CONFIG_PADMUX_MODE4: ts1 = 1s in (3 wire)
                                                                        //CONFIG_PADMUX_MODE5: ts1 = mspi
#define PADS_TS_OUT_MODE                        DISABLE                 //CONFIG_PADMUX_MODE1: None
                                                                        //CONFIG_PADMUX_MODE2: ts1 = 1p out
                                                                        //CONFIG_PADMUX_MODE3: ts1 = 1p out
                                                                        //CONFIG_PADMUX_MODE4: ts1 = 1p out (s2p)
                                                                        //CONFIG_PADMUX_MODE5: ts1 = 1p out (s2p1)
#define PADS_TS2CONFIG                          DISABLE                 //CONFIG_PADMUX_MODE1: ts2 = 1s in
                                                                        //CONFIG_PADMUX_MODE2: ts2 = 1s in
                                                                        //CONFIG_PADMUX_MODE3: ts2 = mspi

#define PADS_SM_CONFIG                          DISABLE                 //CONFIG_PADMUX_MODE1: SMART Card
                                                                        //CONFIG_PADMUX_MODE2: SMART Card
#define PADS_PCMCTRLCONFIG                      CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: PCM1 CTRL
#define PADS_PCMADCONFIG                        DISABLE                 //CONFIG_PADMUX_MODE1: PCM1 AD
#define PADS_PCM2CTRLCONFIG                     DISABLE                 //CONFIG_PADMUX_MODE1: PCM2 CTRL
#define PADS_PCM2_CDN_CONFIG                    DISABLE                 //CONFIG_PADMUX_MODE1: PCM2 CTRL
#define PADS_SPDIFOUTCONFIG                     CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: SPDIF OUT
#define PADS_SPDIFINCONFIG                      DISABLE                 //CONFIG_PADMUX_MODE1: SPDIF IN
#define PADS_SPDIFOUTCONFIG2                    DISABLE                 //CONFIG_PADMUX_MODE1: 2nd SPDIF OUT
#define PADS_I2SINCONFIG                        DISABLE                 //CONFIG_PADMUX_MODE1: I2S Rx 2ch
                                                                        //CONFIG_PADMUX_MODE2: 2nd I2S Rx 2ch
                                                                        //CONFIG_PADMUX_MODE3: I2S Rx 6ch
                                                                        //CONFIG_PADMUX_MODE4: 2nd I2S Rx 6ch
                                                                        //CONFIG_PADMUX_MODE5: 3rd I2S Rx 2ch
                                                                        //CONFIG_PADMUX_MODE6: 3rd I2S Rx 6ch
#define PADS_I2SIN_SD1                          DISABLE                 //CONFIG_PADMUX_MODE1: 3rd I2S Rx 6ch
#define PADS_I2S_PDM_MD                         DISABLE                 //CONFIG_PADMUX_MODE1: Dig Mic 8ch
                                                                        //CONFIG_PADMUX_MODE2: Dig Mic 4ch
                                                                        //CONFIG_PADMUX_MODE3: Dig Mic 2ch
#define PADS_I2S_TDM_MD                         DISABLE                 //CONFIG_PADMUX_MODE1: I2S Rx 4ch
                                                                        //CONFIG_PADMUX_MODE2: TDM Rx 8ch or I2S Rx 2ch
#define PADS_I2S_BT_MD                          DISABLE                 //CONFIG_PADMUX_MODE1: BT TRx 2ch
                                                                        //CONFIG_PADMUX_MODE2: 2nd BT TRx 2ch
                                                                        //CONFIG_PADMUX_MODE3: 3rd BT TRx 2ch
#define PADS_I2SMUTEMODE                        DISABLE                 //CONFIG_PADMUX_MODE1: I2S MUTE
                                                                        //CONFIG_PADMUX_MODE2: I2S MUTE
#define PADS_I2SOUT_IN_TCON                     DISABLE                 //CONFIG_PADMUX_MODE1: I2S OUT at TS1
#define PADS_I2SOUTCONFIG0                      CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: I2S OUT
#define PADS_I2SOUTCONFIG1                      CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: I2S OUT SD
#define PADS_I2SOUTCONFIG2                      DISABLE     //CONFIG_PADMUX_MODE1: I2S OUT SD1
#define PADS_I2SOUTCONFIG3                      CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: I2S OUT SD2
#define PADS_I2SOUTCONFIG4                      CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: I2S OUT SD3
#define PADS_ET_MODE                            DISABLE                 //CONFIG_PADMUX_MODE1: External ETH PHY
#define PADS_LED_MODE                           DISABLE                 //CONFIG_PADMUX_MODE1: LED Mode
                                                                        //CONFIG_PADMUX_MODE2: LED Mode
                                                                        //CONFIG_PADMUX_MODE3: LED Mode
#define PADS_VSYNC_LIKE_CONFIG                  DISABLE                 //CONFIG_PADMUX_MODE1: LDM_SPI0
                                                                        //CONFIG_PADMUX_MODE2: VSYNC_LIKE_FROM_LD
                                                                        //CONFIG_PADMUX_MODE3: LDM_SPI0
                                                                        //CONFIG_PADMUX_MODE4: VSYNC_LIKE_FROM_LD
#define PADS_LD_SPI1_CONFIG                     DISABLE                 //CONFIG_PADMUX_MODE1: 2-wired MSPI
                                                                        //CONFIG_PADMUX_MODE2: 2-wired MSPI at PM
#define PADS_LD_SPI2_CONFIG                     DISABLE                 //CONFIG_PADMUX_MODE1: 2-wired MSPI
                                                                        //CONFIG_PADMUX_MODE2: 2-wired MSPI at PM
#define PADS_LD_SPI3_CONFIG                     DISABLE                 //CONFIG_PADMUX_MODE1: 4wired MSPI for LXX FCIC/DEMURA or CHN Local Dimming
                                                                        //CONFIG_PADMUX_MODE2: 4wire mspi for CHN Demura
                                                                        //CONFIG_PADMUX_MODE3: 4-wired MSPI at PM
#define PADS_MSPI0_CONFIG                       DISABLE                 //CONFIG_PADMUX_MODE1: MSPI0
#define PADS_MSPI1_CONFIG                       DISABLE                 //CONFIG_PADMUX_MODE1: MSPI1
                                                                        //CONFIG_PADMUX_MODE2: MSPI1
#define PADS_MSPI_TSLINK_CONFIG                 DISABLE                 //CONFIG_PADMUX_MODE1: MSPI_TS_LINK
                                                                        //CONFIG_PADMUX_MODE2: MSPI_TS_LINK
#define PADS_SECONDUARTMODE                     DISABLE                 //CONFIG_PADMUX_MODE1: UART2
                                                                        //CONFIG_PADMUX_MODE2: UART2
                                                                        //CONFIG_PADMUX_MODE3: UART2
                                                                        //CONFIG_PADMUX_MODE4: UART2
                                                                        //CONFIG_PADMUX_MODE5: UART2
#define PADS_OD2NDUART                          DISABLE                 //CONFIG_PADMUX_MODE1: OD UART2
                                                                        //CONFIG_PADMUX_MODE2: OD UART2
                                                                        //CONFIG_PADMUX_MODE3: OD UART2
                                                                        //CONFIG_PADMUX_MODE4: OD UART2
                                                                        //CONFIG_PADMUX_MODE5: OD UART2
#define PADS_THIRDUARTMODE                      DISABLE                 //CONFIG_PADMUX_MODE1: UART3
                                                                        //CONFIG_PADMUX_MODE2: UART3
                                                                        //CONFIG_PADMUX_MODE3: UART3
                                                                        //CONFIG_PADMUX_MODE4: UART3
                                                                        //CONFIG_PADMUX_MODE5: UART3
#define PADS_OD3RDUART                          DISABLE                 //CONFIG_PADMUX_MODE1: OD UART3
                                                                        //CONFIG_PADMUX_MODE2: OD UART3
                                                                        //CONFIG_PADMUX_MODE3: OD UART3
                                                                        //CONFIG_PADMUX_MODE4: OD UART3
                                                                        //CONFIG_PADMUX_MODE5: OD UART3
#define PADS_FOURTHUARTMODE                     DISABLE                 //CONFIG_PADMUX_MODE1: UART4
                                                                        //CONFIG_PADMUX_MODE2: UART4
                                                                        //CONFIG_PADMUX_MODE3: UART4
#define PADS_OD4THUART                          DISABLE                 //CONFIG_PADMUX_MODE1: OD UART4
                                                                        //CONFIG_PADMUX_MODE2: OD UART4
                                                                        //CONFIG_PADMUX_MODE3: OD UART4
#define PADS_FIFTHUARTMODE                      DISABLE                 //CONFIG_PADMUX_MODE1: UART5
                                                                        //CONFIG_PADMUX_MODE2: UART5
                                                                        //CONFIG_PADMUX_MODE3: UART5
#define PADS_OD5THUART                          DISABLE                 //CONFIG_PADMUX_MODE1: OD UART5
                                                                        //CONFIG_PADMUX_MODE2: OD UART5
                                                                        //CONFIG_PADMUX_MODE3: OD UART5
#define PADS_SIXTHUARTMODE                      DISABLE                 //CONFIG_PADMUX_MODE1: UART6
#define PADS_OD6THUART                          DISABLE                 //CONFIG_PADMUX_MODE1: OD UART6
#define PADS_FASTUARTMODE                       DISABLE                 //CONFIG_PADMUX_MODE1: Fast UART
                                                                        //CONFIG_PADMUX_MODE2: Fast UART
                                                                        //CONFIG_PADMUX_MODE3: Fast UART
#define PADS_ODFASTUART                         DISABLE                 //CONFIG_PADMUX_MODE1: Fast UART
                                                                        //CONFIG_PADMUX_MODE2: Fast UART
                                                                        //CONFIG_PADMUX_MODE3: Fast UART
#define PADS_PWM0_MODE                          CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: PWM0
#define PADS_PWM1_MODE                          CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: PWM1
#define PADS_PWM2_MODE                          CONFIG_PADMUX_MODE1     //CONFIG_PADMUX_MODE1: PWM2
                                                                        //CONFIG_PADMUX_MODE2: 2nd PWM2
#define PADS_IRE_MODE                           DISABLE                 //CONFIG_PADMUX_MODE1: IRE
                                                                        //CONFIG_PADMUX_MODE2: IRE
                                                                        //CONFIG_PADMUX_MODE3: IRE
#define PADS_DDCRMODE                           DISABLE                 //CONFIG_PADMUX_MODE1: DDCR
                                                                        //CONFIG_PADMUX_MODE2: MIIC3
#define PADS_MIIC_MODE0                         DISABLE                 //CONFIG_PADMUX_MODE1: MIIC0
#define PADS_MIIC_MODE1                         DISABLE                 //CONFIG_PADMUX_MODE1: MIIC1
#define PADS_MIIC_MODE2                         DISABLE                 //CONFIG_PADMUX_MODE1: MIIC2
                                                                        //CONFIG_PADMUX_MODE2: MIIC2
#define PADS_MIIC_MODE4                         DISABLE                 //CONFIG_PADMUX_MODE1: MIIC4
#define PADS_MIIC_MODE5                         DISABLE                 //CONFIG_PADMUX_MODE1: MIIC5
#define PADS_TCONCONFIG0                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON0
                                                                        //CONFIG_PADMUX_MODE2: TCON0
#define PADS_TCONCONFIG1                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON1
                                                                        //CONFIG_PADMUX_MODE2: TCON1
#define PADS_TCONCONFIG2                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON2
                                                                        //CONFIG_PADMUX_MODE2: TCON2
#define PADS_TCONCONFIG3                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON3
                                                                        //CONFIG_PADMUX_MODE2: TCON3
#define PADS_TCONCONFIG4                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON4
                                                                        //CONFIG_PADMUX_MODE2: TCON4
#define PADS_TCONCONFIG5                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON5
#define PADS_TCONCONFIG6                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON6
#define PADS_TCONCONFIG7                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON7
#define PADS_TCONCONFIG8                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON8
#define PADS_TCONCONFIG9                        DISABLE                 //CONFIG_PADMUX_MODE1: TCON9
#define PADS_TCONCONFIG10                       DISABLE                 //CONFIG_PADMUX_MODE1: TCON10
#define PADS_EXTINT0                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT0
#define PADS_EXTINT1                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT1
                                                                        //CONFIG_PADMUX_MODE2: EXT INT1
#define PADS_EXTINT2                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT2
                                                                        //CONFIG_PADMUX_MODE2: EXT INT2
#define PADS_EXTINT3                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT3
                                                                        //CONFIG_PADMUX_MODE2: EXT INT3
#define PADS_EXTINT4                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT4
                                                                        //CONFIG_PADMUX_MODE2: EXT INT4
#define PADS_EXTINT5                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT5
#define PADS_EXTINT6                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT6
                                                                        //CONFIG_PADMUX_MODE2: EXT INT6
#define PADS_EXTINT7                            DISABLE                 //CONFIG_PADMUX_MODE1: EXT INT7
                                                                        //CONFIG_PADMUX_MODE2: EXT INT7
#define PADS_P1_ENABLE_B0                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO0
#define PADS_P1_ENABLE_B1                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO1
#define PADS_P1_ENABLE_B2                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO2
#define PADS_P1_ENABLE_B3                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO3
#define PADS_P1_ENABLE_B4                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO4
#define PADS_P1_ENABLE_B5                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO5
#define PADS_P1_ENABLE_B6                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO6
#define PADS_P1_ENABLE_B7                       DISABLE                 //CONFIG_PADMUX_MODE1: PM51 GPIO7
#define PADS_SDIO_CONFIG                        DISABLE                 //CONFIG_PADMUX_MODE1: SDIO
#define PADS_3DFLAGCONFIG                       DISABLE                 //CONFIG_PADMUX_MODE1: 3DFLAG mode
                                                                        //CONFIG_PADMUX_MODE2: 3DFLAG mode
                                                                        //CONFIG_PADMUX_MODE3: 3DFLAG mode
#define PADS_OSD3DFLAG_CONFIG                   DISABLE                 //CONFIG_PADMUX_MODE1: 3DFLAG mode
                                                                        //CONFIG_PADMUX_MODE2: 3DFLAG mode
                                                                        //CONFIG_PADMUX_MODE3: 3DFLAG mode
#define PADS_ARC_MODE                           DISABLE                 //CONFIG_PADMUX_MODE1: ARC Mode


//=============================================================================
// GPIO Configuration
//Panel
#define PAD_LD_SPI_CK_IS_GPIO                   GPIO_OUT_LOW            //(R1/R2) PANEL_ON/OFF: Panel power switch (Low: Off / High: On)
#define PAD_I2S_IN_MCK_IS_GPIO                  GPIO_OUT_LOW            //(R3) PANEL_ON/OFF   : Panel power switch (Low: Off / High: On)

//Backlight
#define PAD_GPIO0_PM_IS_GPIO                    GPIO_OUT_HIGH           //VBL_CTRL (Low: On / High : Off)

//Audio
#define PADA_LINEIN_L0_IS_GPIO                  GPIO_OUT_LOW            //AMP Reset(R3) (Low: On / High : Off)

#define PAD_SAR2_IS_GPIO                        GPIO_OUT_LOW            //AMP Mute (Low: Off / High: Mute)
#define PAD_GPIO0_IS_GPIO                       GPIO_OUT_LOW            //HP AMP Mute (Low: Off / High: Mute)
#define PAD_GPIO18_PM_IS_GPIO                   GPIO_IN                 //HP_DET
#define PAD_TGPIO2_PM_IS_GPIO                   GPIO_IN                 //I2CM_SCL_AMP
#define PAD_TGPIO3_PM_IS_GPIO                   GPIO_IN                 //I2CM_SDA_AMP

//VID
#ifndef CONFIG_MSTAR_VID_ENABLE
#define PAD_VID0_IS_GPIO                        GPIO_OUT_LOW            //VID0: CORE POWER CONTROL
#define PAD_VID1_IS_GPIO                        GPIO_IN                 //VID1: CORE POWER CONTROL
#endif

//SWI2C
#define PAD_DDCR_DA_IS_GPIO                     GPIO_IN                 //DDCR_DA
#define PAD_DDCR_CK_IS_GPIO                     GPIO_IN                 //DDCR_CK

//PCMCIA Power
#define PAD_LD_SPI_MISO_IS_GPIO                 GPIO_OUT_HIGH           //(R1/R2) PCM_PWR_CTL (Low: On / High: Off)
#define PADA_BIN1P_IS_GPIO                      GPIO_OUT_HIGH           //(R3)    PCM_PWR_CTL (Low: On / High: Off)

//WIFI_RESET
#define PAD_GPIO9_PM_IS_GPIO                    GPIO_OUT_HIGH            //WIFI_reset (Low reset)

//WIFI_ENABLE
#define PAD_TCON1_IS_GPIO                       GPIO_OUT_HIGH            //(R1/R2) Wifi Enable (High: enable / Low : disable)
#define PAD_USB_CTRL_IS_GPIO                    GPIO_OUT_HIGH            //(R3)    Wifi Enable (High: enable / Low : disable)

//USB power enable
#define PAD_I2S_IN_DIN0_IS_GPIO			GPIO_OUT_HIGH		// USB-3 port
#define PAD_I2S_IN_DIN1_IS_GPIO			GPIO_OUT_HIGH		// USB-0 port 
//BT_WAKE detect
#define PAD_SAR0_IS_GPIO                        GPIO_IN                 // Wake detect from BT

//Power detect
#define PAD_SAR3_IS_GPIO                        GPIO_IN                 // Power detect

//Product mode
#define PAD_LD_SPI_CS_IS_GPIO                   GPIO_IN                 // Product mode (0: 1:)

//HW_ID
#define PADA_RIN1P_GPIO                         GPIO_IN                 // (R3) HW_ID_0
#define PADA_GIN1M_GPIO                         GPIO_IN                 // (R3) HW_ID_1
#define PADA_GIN1P_GPIO                         GPIO_IN                 // (R3) HW_ID_2

//LNB
#define PADA_HSYNC1_GPIO                        GPIO_OUT_HIGH           // (R3) LNB_VOLTAGE
#define PADA_VSYNC1_GPIO                        GPIO_OUT_HIGH           // (R3) LNB_ENABLE

//IR blaster ctrl
#define PAD_I2S_OUT_SD1_IS_GPIO			GPIO_OUT_LOW 		//LOW is disabe

//=============================================================================
#ifdef CONFIG_MSTAR_VID_ENABLE
#define CONFIG_CHIP_PACKAGE                     0                       //Chip Package: 0: BGA / 1: QFP / Otherwise: Reserved

#define CONFIG_SIDD_THRESHOLD_CPU               28      // SIDD(CPU) * 5 = 5.6 * 5 = 28
#define CONFIG_OSC_THRESHOLD_CPU                101     // OSC(CPU) * 10 = 10.1 * 10 = 101
#define CONFIG_SIDD_THRESHOLD                   124     // SIDD(CPU+CORE) * 5 = 24.7 * 5 = 124
#define CONFIG_OSC_THRESHOLD_GPU                162     // OSC(CPU+CORE) * 10 = 16.2 * 10 = 162
#endif

//=============================================================================
#define EMMC_TYPE_HS200                         0x00
#define EMMC_TYPE_HS400                         0x01
#define EMMC_TYPE_UNKNOWN                       0xFF

#if defined(CONFIG_MMC)
#define EMMC_TYPE_SEL                           ENABLE
#define EMMC_TYPE                               EMMC_TYPE_HS400
#else
#define EMMC_TYPE_SEL                           DISABLE
#define EMMC_TYPE                               EMMC_TYPE_UNKNOWN
#endif

//=============================================================================
#define ENABLE_MHL                              DISABLE
#define MHL_TYPE                                MHL_TYPE_INTERNAL
#define HDMI_PORT_FOR_MHL                       UI_INPUT_SOURCE_HDMI3

//For Support MHL chip
#if (ENABLE_MHL == ENABLE)
#define INPUT_SUPPORT_MHL_PATH                  E_INPUT_SUPPORT_MHL_PORT_DVI0
#else
#define INPUT_SUPPORT_MHL_PATH                  E_INPUT_NOT_SUPPORT_MHL
#endif

// HDMI switch Setting
#define HDMI_SWITCH_SELECT                      HDMI_SWITCH_NONE

//=============================================================================
#define MST_XTAL_CLOCK_HZ                       FREQ_12MHZ
#define MST_XTAL_CLOCK_KHZ                      (MST_XTAL_CLOCK_HZ / 1000UL)
#define MST_XTAL_CLOCK_MHZ                      (MST_XTAL_CLOCK_KHZ / 1000UL)

// WIFI Reset
#define Wifi_Reset_ON()                         mdrv_gpio_set_low(PAD_GPIO9_PM)
#define Wifi_Reset_OFF()                        mdrv_gpio_set_high(PAD_GPIO9_PM)

// Audio Amplifier
#define I2C_BUS_IDX                             2 // Bus2: PAD_TGPIO2, PAD_TGPIO3

#define Audio_Amplifier_ON()                    _FUNC_NOT_USED()
#define Audio_Amplifier_OFF()                   _FUNC_NOT_USED()

#define Adj_Volume_On()                         mdrv_gpio_set_low(PAD_SAR2)
#define Adj_Volume_Off()                        mdrv_gpio_set_high(PAD_SAR2)

//------ PANEL RELATED ---------------------------------------------------------
#define Panel_VCC_ON()                          {mdrv_gpio_set_high(PAD_LD_SPI_CK); mdrv_gpio_set_high(PAD_I2S_IN_MCK);}
#define Panel_VCC_OFF()                         {mdrv_gpio_set_low(PAD_LD_SPI_CK); mdrv_gpio_set_low(PAD_I2S_IN_MCK);}
#define Panel_Backlight_VCC_ON()                mdrv_gpio_set_low(PAD_GPIO0_PM)
#define Panel_Backlight_VCC_OFF()               mdrv_gpio_set_high(PAD_GPIO0_PM)

#define Panel_Backlight_PWM_ADJ(x)              MDrv_PWM_DutyCycle(E_PWM_CH0, x)
#define Panel_Backlight_Max_Current(x)          MDrv_PWM_DutyCycle(E_PWM_CH0, x)

#define Panel_VG_HL_CTL_ON()                    _FUNC_NOT_USED()
#define Panel_VG_HL_CTL_OFF()                   _FUNC_NOT_USED()
#define PANEL_CONNECTOR_SWAP_LVDS_CH            0
#define PANEL_CONNECTOR_SWAP_LVDS_POL           1

#define PANEL_PDP_10BIT                         1
#define PANEL_CONNECTOR_SWAP_PORT               1
#define PANEL_SWAP_LVDS_POL                     0

// PCMCIA power control
#define PCMCIA_VCC_ON()                         _FUNC_NOT_USED()
#define PCMCIA_VCC_OFF()                        _FUNC_NOT_USED()
#define TunerOffPCMCIA()                        PCMCIA_VCC_OFF()
#define TunerOnPCMCIA()                         PCMCIA_VCC_ON()

// LED Control
#define CONFIG_LED_IS_PWM 1
#if (CONFIG_LED_IS_PWM == 1)
// Adjust MDrv_PM_PWM_DutyCycle to 100%
#define LED_RED_ON(_PERCENTAGE)                 run_command("led_pwm 100", 0)
// Adjust MDrv_PM_PWM_DutyCycle to 0%
#define LED_RED_OFF(_PERCENTAGE)                run_command("led_pwm 0", 0)

#else
#define LED_RED_ON()                            mdrv_gpio_set_high(PAD_PWM_PM)
#define LED_RED_OFF()                           mdrv_gpio_set_low(PAD_PWM_PM)
#endif
#define LED_GREEN_ON()                          _FUNC_NOT_USED()
#define LED_GREEN_OFF()                         _FUNC_NOT_USED()

#define PANEL_CTL_Off()                         Panel_VCC_OFF()
#define PANEL_CTL_On()                          Panel_VCC_ON()
#define INV_CTL_Off()                           Panel_Backlight_VCC_OFF()
#define INV_CTL_On()                            Panel_Backlight_VCC_ON()
#define POWER_ON_OFF1_On()                      Power_On()
#define POWER_ON_OFF1_Off()                     Power_Off()
#define MUTE_On()                               Adj_Volume_Off()    //(XBYTE[0x1e63] |= BIT7)
#define MUTE_Off()                              Adj_Volume_On()     //(XBYTE[0x1e63] &= ~BIT7)
#define EEPROM_WP_On()                          _FUNC_NOT_USED()
#define EEPROM_WP_Off()                         _FUNC_NOT_USED()
#define LED_GRN_Off()                           LED_GREEN_OFF()
#define LED_GRN_On()                            LED_GREEN_ON()
#define LED_RED_Off()                           LED_GRN_On()
#define LED_RED_On()                            LED_GRN_Off()

//------MST Keypad definition---------------------------------------------------
#define KEYPAD_CHANNEL_SUPPORT                  4 //Maximun supported keypad channels
#define ADC_KEY_CHANNEL_NUM                     2 //Real supported keypad channels
#define ADC_KEY_LAST_CHANNEL                    ADC_KEY_CHANNEL_NUM - 1

//config which keypad channel enabled
#define ENABLE_KPDCHAN_1                        DISABLE
#define ENABLE_KPDCHAN_2                        ENABLE
#define ENABLE_KPDCHAN_3                        DISABLE
#define ENABLE_KPDCHAN_4                        DISABLE

#define KEYPAD_KEY_VALIDATION                   3
#define KEYPAD_REPEAT_KEY_CHECK                 KEYPAD_KEY_VALIDATION + 2
#define KEYPAD_REPEAT_KEY_CHECK_1               KEYPAD_KEY_VALIDATION + 3
#define KEYPAD_STABLE_NUM                       10
#define KEYPAD_STABLE_NUM_MIN                   9
#define KEYPAD_REPEAT_PERIOD                    2 // 6
#define KEYPAD_REPEAT_PERIOD_1                  KEYPAD_REPEAT_PERIOD/2

#define KEYPAD_POWER_KEY			0x74
#define KEYPAD_POWER_KEY1			0
#define KEYPAD_USBUPGRADE_KEY                  KEYPAD_POWER_KEY
//------------------------------------------------------------------------------
// SAR boundary define
//------------------------------------------------------------------------------
#define KEYPAD_CH1_UB                           0xFF
#define KEYPAD_CH1_LB                           0xD0
#define KEYPAD_CH2_UB                           0xFF
#define KEYPAD_CH2_LB                           0xD0
#define KEYPAD_CH3_UB                           0xFF
#define KEYPAD_CH3_LB                           0xD0
#define KEYPAD_CH4_UB                           0xFF
#define KEYPAD_CH4_LB                           0xD0

#define ADC_KEY_LEVEL                           4
#define ADC_KEY_L0                              0x12
#define ADC_KEY_L1                              0x36
#define ADC_KEY_L2                              0x56
#define ADC_KEY_L3                              0x7B
#define ADC_KEY_L4                              0x92
#define ADC_KEY_L5                              0xAB
#define ADC_KEY_L6                              0xC3
#define ADC_KEY_L7                              0xE7
#define ADC_KEY_LEVEL_MAX                       ADC_KEY_L7

//### Currently Only Support 2 keypad Channels
//### Support un-balanced levels for each channel by spec. requirement
#define ADC_CH1_LEVELS                          4       //### must be <= ADC_KEY_LEVEL
#define ADC_CH2_LEVELS                          4       //### must be <= ADC_KEY_LEVEL
#define ADC_CH3_LEVELS                          4       //### must be <= ADC_KEY_LEVEL
#define ADC_CH4_LEVELS                          4       //### must be <= ADC_KEY_LEVEL

#define ADC_KEY_1_L0                            0x22    //0x27
#define ADC_KEY_1_L1                            0x5A    //0x47
#define ADC_KEY_1_L2                            0x81    //0x63
#define ADC_KEY_1_L3                            0xC9    //0x7B
#define ADC_KEY_1_L4                            0x00    //RFU
#define ADC_KEY_1_L5                            0x00    //RFU
#define ADC_KEY_1_L6                            0x00    //RFU
#define ADC_KEY_1_L7                            0x00    //RFU

#define ADC_KEY_2_L0                            0x22    //0x27
#define ADC_KEY_2_L1                            0x5A    //0x47
#define ADC_KEY_2_L2                            0x81    //0x63
#define ADC_KEY_2_L3                            0xC9    //0x7B
#define ADC_KEY_2_L4                            0x00    //RFU
#define ADC_KEY_2_L5                            0x00    //RFU
#define ADC_KEY_2_L6                            0x00    //RFU
#define ADC_KEY_2_L7                            0x00    //RFU

#define PWM0_PERIOD                             0xff
//#define PWM1_PERIOD                           0xff
//#define PWM2_PERIOD                           0xff    //PWM2 Period=( PWM2_PERIOD+1 ) *( 1/ Xtal)
//#define PWM3_PERIOD                           0xff

#define INIT_PWM0_DUTY                          0x7e
//#define INIT_PWM1_DUTY                        0x7e
//#define INIT_PWM2_DUTY                        0x7e    //PWM2_duty= (Init_Pwm2_DUTY +1 ) * (1/XTAL)
//#define INIT_PWM3_DUTY                        0x7e

#define BACKLITE_INIT_SETTING                   ENABLE
#define PWM2_MUX_SEL                            0x00

#define ENABLE_PIU_UART0                        DISABLE
#define ENABLE_PIU_UART0_INTERRUPT              DISABLE

//------ HDMI RELATED ---------------------------------------------------------
#define HDCP_HPD_INVERSE                        ENABLE

///////////////////////////////////////////////////////////////////////////////////////////////////
// FCIE - SD realted configuration
#define ENABLE_FCIE_GPIO_CONTROL        0
///////////////////////////////////////////////////////////////////////////////////////////////////
// reg_sdio_config
#define FCIE_PAD_CONFIG     0           // pad mux for sd

// card detection GPIO input setting
#define FCIE_CD_OPT         0           // 0: always remove, 1: insert low, 2: insert high, 3: always insert
#define FCIE_CD_BANK        0x0000      // IN register bank
#define FCIE_CD_OFFSET      0x00        // 8 bits address offset
#define FCIE_CD_BIT         0           // IN value bit

// card power control GPIO output setting
#define FCIE_PC_OPT         0           // 0: always has power, 1: low turn on power, 2: high turn on power
#define FCIE_PC_BANK        0x0000      // OUT register bank
#define FCIE_PC_OFFSET      0x00        // 8 bits address offset
#define FCIE_PC_BIT         0           // OUT value bit

// write protect GPIO input setting
#define FCIE_WP_OPT         0           // 0: not support, 1: low write protect, 2: high write protect
#define FCIE_WP_BANK        0x0000      // IN register bank
#define FCIE_WP_OFFSET      0x00        // 8 bits address offset
#define FCIE_WP_BIT         0           // IN value bit

///////////////////////////////////////////////////////////////////////////////////////////////////
// SDIO - SD realted configuration
#define ENABLE_SDIO_GPIO_CONTROL        1
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
#define						BIT00		1
#define						BIT01		2
#define						BIT02		4
// card detection GPIO input setting
#define SDIO_CD_OPT         1           // 0: always remove, 1: insert low, 2: insert high, 3: always insert
#define	SDIO_CD_BANK		0x1103		// IN register bank
#define	SDIO_CD_OFFSET		0x4A		// 8 bits address offset
#define SDIO_CD_BIT			BIT01		// IN value bit

// card power control GPIO output setting
#define	SDIO_PC_OPT			0			// 0: always has power, 1: low turn on power, 2: high turn on power
#define	SDIO_PC_BANK		0			// OUT register bank
#define	SDIO_PC_OFFSET		0			// 8 bits address offset
#define SDIO_PC_BIT			0			// OUT value bit

// write protect GPIO input setting
#define	SDIO_WP_OPT			0			// 0: not support, 1: low write protect, 2: high write protect
#define	SDIO_WP_BANK		0		// IN register bank
#define	SDIO_WP_OFFSET		0		// 8 bits address offset
#define SDIO_WP_BIT 		0		// IN value bit

// avdd_sdio_3318 voltage control type
#define SDIO_IO_VOLT_TYPE   0           // 1: SW switch IO for SD card 3.0, 2: SW switch IO for WiFi, 3: HW fix AVDD_SDIO
///////////////////////////////////////////////////////////////////////////////////////////////////

#define	SD_SIGNATURE				0x3800
#define SDIO_OPT					( (SDIO_CD_OPT<<8) | (SDIO_PC_OPT<<10) | (SDIO_WP_OPT<<12) )


#if defined(CONFIG_ENABLE_MIU_SSC)
#define MIU0_SSC_ENABLE ENABLE
#define MIU1_SSC_ENABLE ENABLE
#define MIU2_SSC_ENABLE DISABLE
#define MIU_SSC_MODULATION                      25
#define MIU_SSC_DEVIATION                       10
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

#define CHECK_IF_MBOOT_DEFAULT_ENV
#define FORCE_ENV_RELOAD
#define CONFIG_MBOOT_VERSION
#define USB_MBOOT_VERSION                 "MUSB-02.05"
#define USB_MBOOT_CURRENT_VERSION         "CL565126""(" __DATE__" - "__TIME__ ")"

//------ CMI UNLOCK--------

#define CMI_UNLOCK_CK PAD_TGPIO2
#define CMI_UNLOCK_DA PAD_TGPIO3

#include "chip/MSD95NTGW4.h"

#endif // _MSBOARD_H_
