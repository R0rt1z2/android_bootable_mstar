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
/// file   MsIRQ.h
/// @brief  MStar IRQ
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MS_IRQ_H_
#define _MS_IRQ_H_



#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
#define MS_IRQ_MAX          (256) //64 IRQs + 64 FIQs
#define ENABLE_USB_PORT0
#define E_IRQ_FIQ_INVALID   0xFFFF

// Interrupt related
typedef enum
{
    // IRQ
    E_INT_IRQ_0x00_START                = 0x00,
    E_INT_IRQ_UART0                     = E_INT_IRQ_0x00_START+0,
    E_INT_IRQ_BDMA_CH0                  = E_INT_IRQ_0x00_START+1,
    E_INT_IRQ_BDMA_CH1                  = E_INT_IRQ_0x00_START+2,
    E_INT_IRQ_MVD                       = E_INT_IRQ_0x00_START+3,
    E_INT_IRQ_PS                        = E_INT_IRQ_0x00_START+4,
    E_INT_IRQ_NFIE                      = E_INT_IRQ_0x00_START+5,
    E_INT_IRQ_USB                       = E_INT_IRQ_0x00_START+6,
    E_INT_IRQ_UHC                       = E_INT_IRQ_0x00_START+7,
    E_INT_IRQ_EC_BRIDGE                 = E_INT_IRQ_0x00_START+8,
    E_INT_IRQ_EMAC                      = E_INT_IRQ_0x00_START+9,
    E_INT_IRQ_DISP                      = E_INT_IRQ_0x00_START+10,
    E_INT_IRQ_DHC                       = E_INT_IRQ_0x00_START+11,
    E_INT_IRQ_PMSLEEP                   = E_INT_IRQ_0x00_START+12,
    E_INT_IRQ_SBM                       = E_INT_IRQ_0x00_START+13,
    E_INT_IRQ_COMB                      = E_INT_IRQ_0x00_START+14,
    E_INT_IRQ_ECC_DERR                  = E_INT_IRQ_0x00_START+15,
    E_INT_IRQ_0x00_END                  = 0x0F,

    E_INT_IRQ_0x10_START                = 0x10,
    E_INT_IRQ_TSP2HK                    = E_INT_IRQ_0x10_START+0,
    E_INT_IRQ_VE                        = E_INT_IRQ_0x10_START+1,
    E_INT_IRQ_CIMAX2MCU                 = E_INT_IRQ_0x10_START+2,
    E_INT_IRQ_DC                        = E_INT_IRQ_0x10_START+3,
    E_INT_IRQ_GOP                       = E_INT_IRQ_0x10_START+4,
    E_INT_IRQ_PCM                       = E_INT_IRQ_0x10_START+5,
    E_INT_IRQ_IIC0                      = E_INT_IRQ_0x10_START+6,
    E_INT_IRQ_RTC                       = E_INT_IRQ_0x10_START+7,
    E_INT_IRQ_KEYPAD                    = E_INT_IRQ_0x10_START+8,
    E_INT_IRQ_PM                        = E_INT_IRQ_0x10_START+9,
    E_INT_IRQ_DDC2BI                    = E_INT_IRQ_0x10_START+10,
    E_INT_IRQ_SCM                       = E_INT_IRQ_0x10_START+11,
    E_INT_IRQ_VBI                       = E_INT_IRQ_0x10_START+12,
    E_INT_IRQ_M4VD                      = E_INT_IRQ_0x10_START+13,
    E_INT_IRQ_FCIE2RIU                  = E_INT_IRQ_0x10_START+14,
    E_INT_IRQ_ADCDVI2RIU                = E_INT_IRQ_0x10_START+15,
    E_INT_IRQ_0x10_END                  = 0x1F,

    // FIQ
    E_INT_FIQ_0x20_START                = 0x20,
    E_INT_FIQ_EXTIMER0                  = E_INT_FIQ_0x20_START+0,
    E_INT_FIQ_EXTIMER1                  = E_INT_FIQ_0x20_START+1,
    E_INT_FIQ_WDT                       = E_INT_FIQ_0x20_START+2,
    E_INT_FIQ_AEON_TO_8051              = E_INT_FIQ_0x20_START+3,
    E_INT_FIQ_8051_TO_AEON              = E_INT_FIQ_0x20_START+4,
    E_INT_FIQ_8051_TO_BEON              = E_INT_FIQ_0x20_START+5,
    E_INT_FIQ_BEON_TO_8051              = E_INT_FIQ_0x20_START+6,
    E_INT_FIQ_BEON_TO_AEON              = E_INT_FIQ_0x20_START+7,
    E_INT_FIQ_AEON_TO_BEON              = E_INT_FIQ_0x20_START+8,
    E_INT_FIQ_JPD                       = E_INT_FIQ_0x20_START+9,
    E_INT_FIQ_MENULOAD                  = E_INT_FIQ_0x20_START+10,
    E_INT_FIQ_HDMI_NON_PCM              = E_INT_FIQ_0x20_START+11,
    E_INT_FIQ_SPDIF_IN_NON_PCM          = E_INT_FIQ_0x20_START+12,
    E_INT_FIQ_EMAC                      = E_INT_FIQ_0x20_START+13,
    E_INT_FIQ_SE_DSP2UP                 = E_INT_FIQ_0x20_START+14,
    E_INT_FIQ_TSP2AEON                  = E_INT_FIQ_0x20_START+15,
    E_INT_FIQ_0x20_END                  = 0x2F,

    E_INT_FIQ_0x30_START                = 0x30,
    E_INT_FIQ_VIVALDI_STR               = E_INT_FIQ_0x30_START+0,
    E_INT_FIQ_VIVALDI_PTS               = E_INT_FIQ_0x30_START+1,
    E_INT_FIQ_DSP_MIU_PROT              = E_INT_FIQ_0x30_START+2,
    E_INT_FIQ_XIU_TIMEOUT               = E_INT_FIQ_0x30_START+3,
    E_INT_FIQ_DMA_DONE                  = E_INT_FIQ_0x30_START+4,
    E_INT_FIQ_VSYNC_VE4VBI              = E_INT_FIQ_0x30_START+5,
    E_INT_FIQ_FIELD_VE4VBI              = E_INT_FIQ_0x30_START+6,
    E_INT_FIQ_VDMCU2HK                  = E_INT_FIQ_0x30_START+7,
    E_INT_FIQ_VE_DONE_TT                = E_INT_FIQ_0x30_START+8,
    E_INT_FIQ_INT_CCFL                  = E_INT_FIQ_0x30_START+9,
    E_INT_FIQ_INT                       = E_INT_FIQ_0x30_START+10,
    E_INT_FIQ_IR                        = E_INT_FIQ_0x30_START+11,
    E_INT_FIQ_AFEC_VSYNC                = E_INT_FIQ_0x30_START+12,
    E_INT_FIQ_DEC_DSP2UP                = E_INT_FIQ_0x30_START+13,
    E_INT_FIQ_MIPS_WDT                  = E_INT_FIQ_0x30_START+14,  //U3
    E_INT_FIQ_DEC_DSP2MIPS              = E_INT_FIQ_0x30_START+15,
    E_INT_FIQ_0x30_END                  = 0x3F,

    E_INT_IRQ_0x40_START                = 0x40,
    E_INT_IRQ_SVD_HVD                   = E_INT_IRQ_0x40_START+0,
    E_INT_IRQ_USB2                      = E_INT_IRQ_0x40_START+1,
    E_INT_IRQ_UHC2                      = E_INT_IRQ_0x40_START+2,
    E_INT_IRQ_MIU                       = E_INT_IRQ_0x40_START+3,
    E_INT_IRQ_GDMA                      = E_INT_IRQ_0x40_START+4,   //U3
    E_INT_IRQ_UART2                     = E_INT_IRQ_0x40_START+5,   //U3
    E_INT_IRQ_UART1                     = E_INT_IRQ_0x40_START+6,   //U3
    E_INT_IRQ_DEMOD                     = E_INT_IRQ_0x40_START+7,   //U3
    E_INT_IRQ_MPIF                      = E_INT_IRQ_0x40_START+8,   //U3
    E_INT_IRQ_JPD                       = E_INT_IRQ_0x40_START+9,   //U3
    E_INT_IRQ_AEON2HI                   = E_INT_IRQ_0x40_START+10,  //U3
    E_INT_IRQ_BDMA0                     = E_INT_IRQ_0x40_START+11,  //U3
    E_INT_IRQ_BDMA1                     = E_INT_IRQ_0x40_START+12,  //U3
    E_INT_IRQ_OTG                       = E_INT_IRQ_0x40_START+13,  //U3
    E_INT_IRQ_MVD_CHECKSUM_FAIL         = E_INT_IRQ_0x40_START+14,  //U3
    E_INT_IRQ_TSP_CHECKSUM_FAIL         = E_INT_IRQ_0x40_START+15,  //U3
    E_INT_IRQ_0x40_END                  = 0x4F,

    E_INT_IRQ_0x50_START                = 0x50,
    E_INT_IRQ_CA_I3                     = E_INT_IRQ_0x50_START+0,   //U3
    E_INT_IRQ_HDMI_LEVEL                = E_INT_IRQ_0x50_START+1,   //U3
    E_INT_IRQ_MIPS_WADR_ERR             = E_INT_IRQ_0x50_START+2,   //U3
    E_INT_IRQ_RASP                      = E_INT_IRQ_0x50_START+3,   //U3
    E_INT_IRQ_CA_SVP                    = E_INT_IRQ_0x50_START+4,   //U3
    E_INT_IRQ_UART2MCU                  = E_INT_IRQ_0x50_START+5,   //U3
    E_INT_IRQ_URDMA2MCU                 = E_INT_IRQ_0x50_START+6,   //U3
    E_INT_IRQ_IIC1                      = E_INT_IRQ_0x50_START+7,   //U3
    E_INT_IRQ_HDCP                      = E_INT_IRQ_0x50_START+8,   //U3
    E_INT_IRQ_DMA_WADR_ERR              = E_INT_IRQ_0x50_START+9,   //U3
    E_INT_IRQ_UP_IRQ_UART_CA            = E_INT_IRQ_0x50_START+10,  //U3
    E_INT_IRQ_UP_IRQ_EMM_ECM            = E_INT_IRQ_0x50_START+11,  //U3
    E_INT_IRQ_ONIF                      = E_INT_IRQ_0x50_START+12,  //T8
    E_INT_IRQ_USB1                      = E_INT_IRQ_0x50_START+13,  //T8
    E_INT_IRQ_UHC1                      = E_INT_IRQ_0x50_START+14,  //T8
    E_INT_IRQ_MFE                       = E_INT_IRQ_0x50_START+15,  //T8
    E_INT_IRQ_0x50_END                  = 0x5F,

    E_INT_FIQ_0x60_START                = 0x60,
    E_INT_FIQ_IR_INT_RC                 = E_INT_FIQ_0x60_START+0,   //U3
    E_INT_FIQ_HDMITX_IRQ_EDGE           = E_INT_FIQ_0x60_START+1,   //U3
    E_INT_FIQ_UP_IRQ_UART_CA            = E_INT_FIQ_0x60_START+2,   //U3
    E_INT_FIQ_UP_IRQ_EMM_ECM            = E_INT_FIQ_0x60_START+3,   //U3
    E_INT_FIQ_PVR2MI_INT0               = E_INT_FIQ_0x60_START+4,   //U3
    E_INT_IRQ_CA_RSA_INT0               = E_INT_FIQ_0x60_START+4,   //Keltic / Kaiser / keres
    E_INT_FIQ_PVR2MI_INT1               = E_INT_FIQ_0x60_START+5,   //U3
    E_INT_IRQ_FIQ_INT                   = E_INT_FIQ_0x60_START+6,   //Kappa
    E_INT_IRQ_UART3                     = E_INT_FIQ_0x60_START+7,   //Kappa
    E_INT_FIQ_AEON_TO_MIPS_VPE0         = E_INT_FIQ_0x60_START+8,   //T3,
    E_INT_FIQ_AEON_TO_MIPS_VPE1         = E_INT_FIQ_0x60_START+9,   //T3, E_INT_FIQ_AEON_TO_BEON
    E_INT_FIQ_SECEMAC                   = E_INT_FIQ_0x60_START+10,  //Kaiser
    E_INT_FIQ_IR2_INT                   = E_INT_FIQ_0x60_START+11,  //Kaiser
    E_INT_FIQ_MIPS_VPE1_TO_MIPS_VPE0    = E_INT_FIQ_0x60_START+12,  //T3
    E_INT_FIQ_MIPS_VPE1_TO_AEON         = E_INT_FIQ_0x60_START+13,  //T3
    E_INT_FIQ_MIPS_VPE1_TO_8051         = E_INT_FIQ_0x60_START+14,  //T3
    E_INT_FIQ_IR2_INT_RC                = E_INT_FIQ_0x60_START+15,  //Kaiser
    E_INT_FIQ_0x60_END                  = 0x6F,

    E_INT_FIQ_0x70_START                = 0x70,
    E_INT_FIQ_MIPS_VPE0_TO_MIPS_VPE1    = E_INT_FIQ_0x70_START+0,   //T3
    E_INT_FIQ_MIPS_VPE0_TO_AEON         = E_INT_FIQ_0x70_START+1,   //T3, E_INT_FIQ_AEON_TO_BEON
    E_INT_FIQ_MIPS_VPE0_TO_8051         = E_INT_FIQ_0x70_START+2,   //T3, E_INT_FIQ_BEON_TO_8051
    E_INT_FIQ_IR_IN                     = E_INT_FIQ_0x70_START+3,   //T8
    E_INT_FIQ_DMDMCU2HK                 = E_INT_FIQ_0x70_START+4,
    E_INT_FIQ_R2TOMCU_INT0              = E_INT_FIQ_0x70_START+5,   //T8
    E_INT_FIQ_R2TOMCU_INT1              = E_INT_FIQ_0x70_START+6,   //T8
    E_INT_FIQ_DSPTOMCU_INT0             = E_INT_FIQ_0x70_START+7,   //T8
    E_INT_FIQ_DSPTOMCU_INT1             = E_INT_FIQ_0x70_START+8,   //T8
    E_INT_FIQ_USB                       = E_INT_FIQ_0x70_START+9,   //T8
    E_INT_FIQ_UHC                       = E_INT_FIQ_0x70_START+10,  //T8
    E_INT_FIQ_USB1                      = E_INT_FIQ_0x70_START+11,  //T8
    E_INT_FIQ_UHC1                      = E_INT_FIQ_0x70_START+12,  //T8
    E_INT_FIQ_USB2                      = E_INT_FIQ_0x70_START+13,  //T8
    E_INT_FIQ_UHC2                      = E_INT_FIQ_0x70_START+14,  //T8
    E_INT_FIQ_EXTIMER2                  = E_INT_FIQ_0x70_START+15,  //Munich
    //Not Used                          = E_INT_FIQ_0x70_START+15,
    E_INT_FIQ_0x70_END                  = 0x7F,


    // Add IRQ from 0x80 ~ 0xBF,
    // if IRQ enum from 0x00 ~ 0x1F, and 0x40 ~ 0x5F is occupied
    E_INT_IRQ_0x80_START                = 0x80,
    E_INT_IRQ_MLINK                     = E_INT_IRQ_0x80_START+0,   //U3
    E_INT_IRQ_AFEC                      = E_INT_IRQ_0x80_START+1,   //T3
    E_INT_IRQ_DPTX                      = E_INT_IRQ_0x80_START+2,   //T3
    E_INT_IRQ_TMDDRLINK                 = E_INT_IRQ_0x80_START+3,   //T3
    E_INT_IRQ_DISPI                     = E_INT_IRQ_0x80_START+4,   //T3
    E_INT_IRQ_EXP_MLINK                 = E_INT_IRQ_0x80_START+5,   //T3
    E_INT_IRQ_M4VE                      = E_INT_IRQ_0x80_START+6,   //T3
    E_INT_IRQ_DVI_HDMI_HDCP             = E_INT_IRQ_0x80_START+7,   //T3
    E_INT_IRQ_G3D2MCU                   = E_INT_IRQ_0x80_START+8,   //T3
    E_INT_IRQ_VP6                       = E_INT_IRQ_0x80_START+9,   //A3
    E_INT_IRQ_INT                       = E_INT_IRQ_0x80_START+10,  //M12
    E_INT_IRQ_CEC                       = E_INT_IRQ_0x80_START+11,  //T8
    E_INT_IRQ_HDCP_IIC                  = E_INT_IRQ_0x80_START+12,  //T8
    E_INT_IRQ_HDCP_X74                  = E_INT_IRQ_0x80_START+13,  //T8
    E_INT_IRQ_WADR_ERR                  = E_INT_IRQ_0x80_START+14,  //T8
    E_INT_IRQ_DCSUB                     = E_INT_IRQ_0x80_START+15,  //T8
    E_INT_IRQ_0x80_END                  = 0x8F,

    E_INT_IRQ_0x90_START                = 0x90,
    E_INT_IRQ_GE                        = E_INT_IRQ_0x90_START+0,   //T8
    E_INT_IRQ_SYNC_DET                  = E_INT_IRQ_0x90_START+1,   //M10
    E_INT_IRQ_FSP                       = E_INT_IRQ_0x90_START+2,   //M10
    E_INT_IRQ_PWM_RP_L                  = E_INT_IRQ_0x90_START+3,   //M10
    E_INT_IRQ_PWM_FP_L                  = E_INT_IRQ_0x90_START+4,   //M10
    E_INT_IRQ_PWM_RP_R                  = E_INT_IRQ_0x90_START+5,   //M10
    E_INT_IRQ_PWM_FP_R                  = E_INT_IRQ_0x90_START+6,   //M10
    E_INT_IRQ_FRC_SC                    = E_INT_IRQ_0x90_START+7,   //A5
    E_INT_IRQ_FRC_INT_FIQ2HST0          = E_INT_IRQ_0x90_START+8,   //A5
    E_INT_IRQ_SMART                     = E_INT_IRQ_0x90_START+9,   //A5
    E_INT_IRQ_MVD2MIPS                  = E_INT_IRQ_0x90_START+10,  //A5
    E_INT_IRQ_GPD                       = E_INT_IRQ_0x90_START+11,  //A5
    E_INT_IRQ_DS                       	= E_INT_IRQ_0x90_START+12,  //Kappa
    E_INT_IRQ_FRC_INT_IRQ2HST0          = E_INT_IRQ_0x90_START+13,  //A5
    E_INT_IRQ_MIIC_DMA_INT3             = E_INT_IRQ_0x90_START+14,  //A5
    E_INT_IRQ_MIIC_INT3                 = E_INT_IRQ_0x90_START+15,  //A5
    E_INT_IRQ_0x90_END                  = 0x9F,

    E_INT_IRQ_0xA0_START                = 0xA0,
    E_INT_IRQ_IIC2                      = E_INT_IRQ_0xA0_START+0,   //A1
    E_INT_IRQ_MIIC_DMA0                 = E_INT_IRQ_0xA0_START+1,   //A1
    E_INT_IRQ_MIIC_DMA1                 = E_INT_IRQ_0xA0_START+2,   //A1
    E_INT_IRQ_MIIC_DMA2                 = E_INT_IRQ_0xA0_START+3,   //A1
    E_INT_IRQ_MSPI0                     = E_INT_IRQ_0xA0_START+4,   //A1
    E_INT_IRQ_MSPI1                     = E_INT_IRQ_0xA0_START+5,   //A1
    E_INT_IRQ_EXT_GPIO0                 = E_INT_IRQ_0xA0_START+6,   //A1
    E_INT_IRQ_EXT_GPIO1                 = E_INT_IRQ_0xA0_START+7,   //A1
    E_INT_IRQ_EXT_GPIO2                 = E_INT_IRQ_0xA0_START+8,   //A1
    E_INT_IRQ_EXT_GPIO3                 = E_INT_IRQ_0xA0_START+9,   //A1
    E_INT_IRQ_EXT_GPIO4                 = E_INT_IRQ_0xA0_START+10,  //A1
    E_INT_IRQ_EXT_GPIO5                 = E_INT_IRQ_0xA0_START+11,  //A1
    E_INT_IRQ_EXT_GPIO6                 = E_INT_IRQ_0xA0_START+12,  //A1
    E_INT_IRQ_EXT_GPIO7                 = E_INT_IRQ_0xA0_START+13,  //A1
    E_INT_IRQ_MIIC_DMA_INT2             = E_INT_IRQ_0xA0_START+14,  //A5
    E_INT_IRQ_MIIC_INT2                 = E_INT_IRQ_0xA0_START+15,  //A5
    E_INT_IRQ_0xA0_END                  = 0xAF,

    E_INT_IRQ_0xB0_START                = 0xB0,
    E_INT_IRQ_MIIC_DMA_INT1             = E_INT_IRQ_0xB0_START+0,   //A5
    E_INT_IRQ_MIIC_INT1                 = E_INT_IRQ_0xB0_START+1,   //A5
    E_INT_IRQ_MIIC_DMA_INT0             = E_INT_IRQ_0xB0_START+2,   //A5
    E_INT_IRQ_MIIC_INT0                 = E_INT_IRQ_0xB0_START+3,   //A5
    E_INT_IRQ_UHC30                     = E_INT_IRQ_0xB0_START+4,   //Agate
    E_INT_IRQ_AU_DMA                    = E_INT_IRQ_0xB0_START+5,   //Agate
    E_INT_IRQ_DIPW                      = E_INT_IRQ_0xB0_START+6,   //Agate
    E_INT_IRQ_HDMITX                    = E_INT_IRQ_0xB0_START+7,   //Agate
    E_INT_IRQ_U3_DPHY                   = E_INT_IRQ_0xB0_START+8,   //Agate
    E_INT_IRQEXPL_TSO                   = E_INT_IRQ_0xB0_START+9,   //Agate
    E_INT_IRQ_TSP_TSO0                  = E_INT_IRQ_0xB0_START+9,   //Keltic , Kiaser Add
    E_INT_IRQEXPH_CEC1                  = E_INT_IRQ_0xB0_START+10,  //Agate
    E_INT_IRQ_TSP_TSO1                  = E_INT_IRQ_0xB0_START+10,  //Keltic , Kiaser Add
    E_INT_IRQ_BT_DMA                    = E_INT_IRQ_0xB0_START+11,  //Kaiser
    E_INT_IRQ_BT_TAB                    = E_INT_IRQ_0xB0_START+12,  //Kaiser
    E_INT_IRQ_SATA                      = E_INT_IRQ_0xB0_START+13,  //Kaiser
    E_INT_IRQ_MHL_CBUS_PM               = E_INT_IRQ_0xB0_START+14,  //Emerald, Eden
    E_INT_IRQ_MHL_CBUS_WAKEUP           = E_INT_IRQ_0xB0_START+15,  //Eden
    E_INT_IRQ_0xB0_END                  = 0xBF,


    // Add FIQ from 0xC0 ~ 0xFD,
    // if FIQ enum from 0x20 ~ 0x4F, and 0x60 ~ 0x7F is occupied
    E_INT_FIQ_0xC0_START                = 0xC0,
    E_INT_FIQ_DMARD                     = E_INT_FIQ_0xC0_START+0,   //U3
    E_INT_FIQ_AU_DMA_BUF_INT            = E_INT_FIQ_0xC0_START+1,   //T3
    E_INT_FIQ_8051_TO_MIPS_VPE1         = E_INT_FIQ_0xC0_START+2,   //T3
    E_INT_FIQ_DVI_DET                   = E_INT_FIQ_0xC0_START+3,   //M10
    E_INT_FIQ_PM_GPIO0                  = E_INT_FIQ_0xC0_START+4,   //M10
    E_INT_FIQ_PM_GPIO1                  = E_INT_FIQ_0xC0_START+5,   //M10
    E_INT_FIQ_PM_GPIO2                  = E_INT_FIQ_0xC0_START+6,   //M10
    E_INT_FIQ_PM_GPIO3                  = E_INT_FIQ_0xC0_START+7,   //M10
    E_INT_FIQ_PM_XIU_TIMEOUT            = E_INT_FIQ_0xC0_START+8,   //M10
    E_INT_FIQ_PWM_RP_RP_L               = E_INT_FIQ_0xC0_START+9,   //M10
    E_INT_FIQ_PWM_RP_FP_L               = E_INT_FIQ_0xC0_START+10,  //M10
    E_INT_FIQ_PWM_RP_RP_R               = E_INT_FIQ_0xC0_START+11,  //M10
    E_INT_FIQ_PWM_RP_FP_R               = E_INT_FIQ_0xC0_START+12,  //M10
    E_INT_FIQ_8051_TO_MIPS_VPE0         = E_INT_FIQ_0xC0_START+13,  //A5
    E_INT_FIQ_FRC_R2_TO_MIPS            = E_INT_FIQ_0xC0_START+14,
    E_INT_FIQ_VP6                       = E_INT_FIQ_0xC0_START+15,  //A3
    E_INT_FIQ_0xC0_END                  = 0xCF,

    E_INT_FIQ_0xD0_START                = 0xD0,
    E_INT_FIQ_STRETCH                   = E_INT_FIQ_0xD0_START+0,
    E_INT_FIQ_GPIO0                     = E_INT_FIQ_0xD0_START+1,   //T12
    E_INT_FIQ_GPIO1                     = E_INT_FIQ_0xD0_START+2,   //T12
    E_INT_FIQ_GPIO2                     = E_INT_FIQ_0xD0_START+3,   //T12
    E_INT_FIQ_GPIO3                     = E_INT_FIQ_0xD0_START+4,   //T12
    E_INT_FIQ_GPIO4                     = E_INT_FIQ_0xD0_START+5,   //T12
    E_INT_FIQ_GPIO5                     = E_INT_FIQ_0xD0_START+6,   //T12
    E_INT_FIQ_GPIO6                     = E_INT_FIQ_0xD0_START+7,   //T12
    E_INT_FIQ_GPIO7                     = E_INT_FIQ_0xD0_START+8,   //T12
    E_INT_FIQ_VE_VSYNC_IN               = E_INT_FIQ_0xD0_START+9,   //Agate
    E_INT_FIQEXPL_HST0_TO_3             = E_INT_FIQ_0xD0_START+10,  //Agate
    E_INT_FIQEXPL_HST1_TO_3             = E_INT_FIQ_0xD0_START+11,  //Agate
    E_INT_FIQEXPL_HST2_TO_3             = E_INT_FIQ_0xD0_START+12,  //Agate
    E_INT_FIQEXPH_CMDQ                  = E_INT_FIQ_0xD0_START+13,  //Agate
    E_INT_FIQEXPH_HDMITX_EDGE           = E_INT_FIQ_0xD0_START+14,  //Agate
    E_INT_FIQEXPH_UHC30                 = E_INT_FIQ_0xD0_START+15,  //Agate
    E_INT_INT_FIQ_0xD0_END              = 0xDF,

    E_INT_FIQ_0xE0_START                = 0xE0,
    E_INT_FIQ_LDM_DMA0                  = E_INT_FIQ_0xE0_START+0,   //A1
    E_INT_IRQ_RASP1                     = E_INT_FIQ_0xE0_START+0,   //Kaiser
    E_INT_FIQ_LDM_DMA1                  = E_INT_FIQ_0xE0_START+1,   //A1
    E_INT_IRQ_SECEMAC                   = E_INT_FIQ_0xE0_START+1,   //Kaiser
    E_INT_IRQ_SDIO                      = E_INT_FIQ_0xE0_START+2,   //K2
    E_INT_IRQ_UHC3                      = E_INT_FIQ_0xE0_START+3,   //K2
    E_INT_IRQ_USB3                      = E_INT_FIQ_0xE0_START+4,   //K2
    E_INT_FIQ_GPIO8                     = E_INT_FIQ_0xE0_START+5,   //Eagle
    E_INT_FIQ_GPIO9                     = E_INT_FIQ_0xE0_START+6,   //Eagle
    E_INT_FIQ_DISP_TGEN0                = E_INT_FIQ_0xE0_START+7,   //Eagle
    E_INT_FIQ_CA_CRYPTO_DMA             = E_INT_FIQ_0xE0_START+7,   //Keltic , Kaiser Add
    E_INT_FIQ_DISP_TGEN1                = E_INT_FIQ_0xE0_START+8,   //Eagle
    E_INT_IRQ_CA_PROG_PVR               = E_INT_FIQ_0xE0_START+8,   //Keltic , Kaiser Add
    E_INT_FIQ_DISP_TGEN2                = E_INT_FIQ_0xE0_START+9,   //Eagle
    E_INT_IRQ_CA_NSK_INT                = E_INT_FIQ_0xE0_START+9,   //Keltic , Kaiser Add
    E_INT_FIQ_DISP_TGEN3                = E_INT_FIQ_0xE0_START+10,  //Eagle
    E_INT_IRQ_TSP_ECM_FLT               = E_INT_FIQ_0xE0_START+10,  //Kaiser
    E_INT_IRQ_ERROR_RESP                = E_INT_FIQ_0xE0_START+11,  //Edison
    E_INT_IRQ_MIU_SECURITY              = E_INT_FIQ_0xE0_START+12,  //Edison
    E_INT_FIQ_TEMPERATURE_FLAG_FALL     = E_INT_FIQ_0xE0_START+13,  //Eiffel
    E_INT_IRQ_DISP1                     = E_INT_FIQ_0xE0_START+13,  //Kaiser
    E_INT_FIQ_TEMPERATURE_FLAG_RISE     = E_INT_FIQ_0xE0_START+14,  //Eiffel
    E_INT_IRQ_RTC1                      = E_INT_FIQ_0xE0_START+14,  //Kaiser
    E_INT_FIQ_U3_DPHY                   = E_INT_FIQ_0xE0_START+15,  //Eiffel
    E_INT_IRQ_GPU2MCU                   = E_INT_FIQ_0xE0_START+15,  //Kaiser
    E_INT_FIQ_0xE0_END                  = 0xEF,

    E_INT_FIQ_0xF0_START                = 0xF0,
    E_INT_FIQ_DEC_DSP2R2M               = E_INT_FIQ_0xF0_START+0,
    E_INT_FIQ_AEON_TO_R2M               = E_INT_FIQ_0xF0_START+1,
    E_INT_FIQ_R2M_TO_AEON               = E_INT_FIQ_0xF0_START+2,
    E_INT_FIQ_R2M_TO_8051               = E_INT_FIQ_0xF0_START+3,
    E_INT_IRQ_VIVALDI_DMA_INTR2         = E_INT_FIQ_0xF0_START+4,
    E_INT_FIQ_AU_DMA_INT                = E_INT_FIQ_0xF0_START+4,  //Kaiser
    E_INT_IRQ_VIVALDI_DMA_INTR1         = E_INT_FIQ_0xF0_START+5,
    E_INT_FIQ_AU_PCM_DMA_INT            = E_INT_FIQ_0xF0_START+5,  //Kaiser
    E_INT_IRQ_AFEC_INT                  = E_INT_FIQ_0xF0_START+6,
    E_INT_FIQ_AU_SPDIF_TX_CS0           = E_INT_FIQ_0xF0_START + 7,
    E_INT_FIQ_AU_SPDIF_TX_CS1           = E_INT_FIQ_0xF0_START + 8, //Eiffel
    E_INT_FIQ_PCM_DMA                   = E_INT_FIQ_0xF0_START + 9, //Eiffel
    E_INT_FIQ_DMDMCU2HK_1               = E_INT_FIQ_0xF0_START+9,   //Kaiser
    E_INT_FIQ_VE_SW_WR2BUF              = E_INT_FIQ_0xF0_START+10,  //Kaiser
    E_INT_IRQ_FRM_PM                    = E_INT_FIQ_0xF0_START+11,
    E_INT_FIQ_FRM_PM                    = E_INT_FIQ_0xF0_START+12,
    E_INT_FIQ_SATA_PHY                  = E_INT_FIQ_0xF0_START+13,
    E_INT_IRQ_FIQ_NONE                  = E_INT_FIQ_0xF0_START+14,
    E_INT_IRQ_FIQ_ALL                   = E_INT_FIQ_0xF0_START+15,
    E_INT_FIQ_0xF0_END                  = 0xFF,

    E_INT_IRQ_0x100_START               = 0x100,
    E_INT_IRQ_RIU_ERROR                 = E_INT_IRQ_0x100_START+0,  //Nugget
    E_INT_IRQ_EVD                       = E_INT_IRQ_0x100_START+1,  //Einstein
    E_INT_IRQ_SWCD                      = E_INT_IRQ_0x100_START+1,  //Nugget
    E_INT_IRQ_MIU_SECURE                = E_INT_IRQ_0x100_START+2,  //Nugget
    E_INT_IRQ_TIMER2                    = E_INT_IRQ_0x100_START+3,  //Nugget
    E_INT_FIQ_8051_TO_SECURER2          = E_INT_IRQ_0x100_START+4,  //Nugget
    E_INT_FIQ_AEON_TO_SECURER2          = E_INT_IRQ_0x100_START+5,  //Nugget
    E_INT_FIQ_BEON_TO_SECURER2          = E_INT_IRQ_0x100_START+6,  //Nugget
    E_INT_FIQ_SECURER2_TO_BEON          = E_INT_IRQ_0x100_START+7,  //Nugget
    E_INT_FIQ_SECURER2_TO_AEON          = E_INT_IRQ_0x100_START+8,  //Nugget
    E_INT_FIQ_SECURER2_TO_8051          = E_INT_IRQ_0x100_START+9, //Nugget
    E_INT_IRQ_SAR1                      = E_INT_IRQ_0x100_START+10, //keres
    E_INT_IRQ_IDAC_PLUG_DET             = E_INT_IRQ_0x100_START+11, //keres
    E_INT_FIQ_AU_SPDIF_TX_CS2           = E_INT_IRQ_0x100_START+12, //keres
    E_INT_IRQ_CA_IP_INT                 = E_INT_IRQ_0x100_START+13, //keres
    E_INT_IRQ_AKL_INT                   = E_INT_IRQ_0x100_START+14, //keres
    E_INT_FIQ_MB_A2M_INT0               = E_INT_IRQ_0x100_START+15, //keres
    E_INT_FIQ_0x100_END                 = 0x10F,

    E_INT_FIQ_0x110_START               = 0x110,
    E_INT_FIQ_MB_D2M_INT0               = E_INT_FIQ_0x110_START+0, //keres
    E_INT_FIQ_MB_D2M_INT1               = E_INT_FIQ_0x110_START+1, //keres
    E_INT_FIQ_MB_A2M_INT1               = E_INT_FIQ_0x110_START+2, //keres
    E_INT_FIQ_MB_A2M_INT2               = E_INT_FIQ_0x110_START+3, //keres
    E_INT_FIQ_MB_A2M_INT3               = E_INT_FIQ_0x110_START+4, //keres
    E_INT_IRQ_FIQ_OTG                   = E_INT_FIQ_0x110_START+5, //clippers
    E_INT_IRQ_VP9_HK2VD_R2              = E_INT_FIQ_0x110_START+6, //maonco vp9_hk2vd_r2_int
    E_INT_FIQ_8051_TO_SECURE51          = E_INT_FIQ_0x110_START+7,  //keres
    E_INT_FIQ_SECURE51_TO_8051          = E_INT_FIQ_0x110_START+8,  //keres
    E_INT_FIQ_BEON_TO_SECURE51          = E_INT_FIQ_0x110_START+9,  //keres
    E_INT_FIQ_SECURE51_TO_BEON          = E_INT_FIQ_0x110_START+10,  //keres
    E_INT_FIQ_SECURER2_TO_SECURE51      = E_INT_FIQ_0x110_START+11,  //keres
    E_INT_FIQ_SECURE51_TO_SECURER2      = E_INT_FIQ_0x110_START+12,  //keres
    E_INT_FIQ_PM_SD_CDZ0                = E_INT_FIQ_0x110_START + 13,   //Miami
    E_INT_FIQ_PM_SD_CDZ1                = E_INT_FIQ_0x110_START + 14,   //Miami
    E_INT_FIQ_TSO_INT                   = E_INT_FIQ_0x110_START + 15,   //Milan add
    E_INT_FIQ_0x110_END                 = 0x11F,

    E_INT_IRQ_0x120_START               = 0x120,
    E_INT_IRQ_SPI2FCIE                  = E_INT_IRQ_0x120_START + 0,    //Milan
    E_INT_IRQ_USB30_SS_INT              = E_INT_IRQ_0x120_START + 1,    //Miami
    E_INT_IRQ_USB30_HS_UHC_INT0         = E_INT_IRQ_0x120_START + 2,    //Miami
    E_INT_IRQ_USB30_HS_UHC_INT1         = E_INT_IRQ_0x120_START + 3,    //Miami
    E_INT_IRQ_USB30_HS_USB_INT          = E_INT_IRQ_0x120_START + 4,    //Miami
    E_INT_IRQ_MIIC_INT4                 = E_INT_IRQ_0x120_START + 5,    //Miami
    E_INT_IRQ_MIIC_INT5                 = E_INT_IRQ_0x120_START + 6,    //Miami
    E_INT_IRQ_UART4                     = E_INT_IRQ_0x120_START + 7,    //Miami
    E_INT_IRQ_BDMA                      = E_INT_IRQ_0x120_START + 8,    //Miami
    E_INT_IRQ_ZDEC                      = E_INT_IRQ_0x120_START + 9,    //Miami
    E_INT_IRQ_FRC                       = E_INT_IRQ_0x120_START + 10,   //Miami
    E_INT_FIQ_USB3                      = E_INT_IRQ_0x120_START + 11,   //Miami
    E_INT_FIQ_UHC3                      = E_INT_IRQ_0x120_START + 12,   //Miami
    E_INT_FIQ_R2TOMCU_INT2              = E_INT_IRQ_0x120_START + 13,   //Miami
    E_INT_FIQ_R2TOMCU_INT3              = E_INT_IRQ_0x120_START + 14,   //Miami
    E_INT_IRQ_PM_ERROR_RESP             = E_INT_IRQ_0x120_START + 15,    //Milan
    E_INT_IRQ_0x120_END                 = 0x12F,

    E_INT_IRQ_0x130_START               = 0x130,
    E_INT_IRQ_AUDMA_V2_INT              = E_INT_IRQ_0x130_START + 1,    //Muji
    E_INT_IRQ_EMMC_OSP_INT              = E_INT_IRQ_0x130_START + 2,    //Muji
    E_INT_IRQ_MHL_ECBUS_INT             = E_INT_IRQ_0x130_START + 3,    //Muji
    E_INT_IRQ_SDIO_OSP_INT              = E_INT_IRQ_0x130_START + 4,    //Muji
    E_INT_IRQ_DISP_FE_INT               = E_INT_IRQ_0x130_START + 7,    //Manhattan
    E_INT_IRQ_SCDC_PM_INT               = E_INT_IRQ_0x130_START + 8,    //Manhattan
    E_INT_IRQ_USB30_HS1_USB_INT         = E_INT_IRQ_0x130_START + 9,    //Manhattan
    E_INT_IRQ_USB30_HS1_UHC_INT         = E_INT_IRQ_0x130_START + 10,   //Manhattan
    E_INT_IRQ_USB30_HS_UHC_INT          = E_INT_IRQ_0x130_START + 11,   //Manhattan
    E_INT_IRQ_TSP_FI_QUEUE_INT          = E_INT_IRQ_0x130_START + 12,   //Manhattan
    E_INT_IRQ_DISP_SC2_INT              = E_INT_IRQ_0x130_START + 13,   //Manhattan
    E_INT_IRQ_MSPI_MCARD_INT            = E_INT_IRQ_0x130_START + 14,   //Manhattan
    E_INT_IRQ_CFKTKS_NONSEC_INT         = E_INT_IRQ_0x130_START + 15,   //Manhattan
    E_INT_IRQ_0x130_END                 = 0x13F,

    E_INT_IRQ_0x140_START               = 0x140,
    E_INT_IRQ_CFKTKS_INT              	= E_INT_IRQ_0x140_START + 0,    //Manhattan
    E_INT_IRQ_CFDONE_INT              	= E_INT_IRQ_0x140_START + 1,    //Manhattan
    E_INT_IRQ_MIU_TLB_INT              	= E_INT_IRQ_0x140_START + 2,    //Manhattan
    E_INT_IRQ_PAS_PTS_COMBINE_INT       = E_INT_IRQ_0x140_START + 3,    //Manhattan
    E_INT_IRQ_AESDMA_S_INT              = E_INT_IRQ_0x140_START + 4,    //Manhattan
    E_INT_IRQ_VD_EVD_R22HI_INT          = E_INT_IRQ_0x140_START + 5,    //Manhattan
    E_INT_IRQ_MOD_DET_INT               = E_INT_IRQ_0x140_START + 6,     //Maserati
    E_INT_HDCP_ICC_INT                  = E_INT_IRQ_0x140_START + 7,     //Maserati
    E_INT_CMDQ_SUB_INT                  = E_INT_IRQ_0x140_START + 8,     //Maserati
    E_INT_OTG_INIT                      = E_INT_IRQ_0x140_START + 9,     //Maserati
    E_INT_VP9_HK2VD_R2_INT              = E_INT_IRQ_0x140_START + 10,    //Maserati
    E_INT_KG1_INT                       = E_INT_IRQ_0x140_START + 11,    //Maserati
    E_INT_PKA_ALL_INT                   = E_INT_IRQ_0x140_START + 12,    //Maserati
    E_INT_AESDMA_PROT_2_INT             = E_INT_IRQ_0x140_START + 13,    //Maserati
    E_INT_CERT_KTKS_INI_NONSEC_INT      = E_INT_IRQ_0x140_START + 14,    //Maserati
    E_INT_CERT_KTKS_INT                 = E_INT_IRQ_0x140_START + 15,    //Maserati
    E_INT_IRQ_0x140_END                 = 0x14F,

    E_INT_FIQ_0x150_START               = 0x150,
    E_INT_FIQ_LAN_ESD_INT               = E_INT_FIQ_0x150_START + 0, 	//Manhattan
    E_INT_FIQ_TIMER2_INT                = E_INT_FIQ_0x150_START + 1, 	//Manhattan
    E_INT_FIQ_SPI2FCIE                  = E_INT_FIQ_0x150_START + 2, 	//Macan
    E_INT_IRQ_EXT_GPIO_MERGE            = E_INT_FIQ_0x150_START + 3,    //Mustang
    E_INT_IRQ_DIMOND_L3_BRIDGE_INT      = E_INT_FIQ_0x150_START + 4,    //Mustang
    E_INT_IRQ_PAS_PTS_INTRL_COMBINE     = E_INT_FIQ_0x150_START + 5,    //Mustang
    E_INT_FIQ_GPIO22_INT                = E_INT_FIQ_0x150_START + 6,
    E_INT_IRQ_PAS_PTS_INT_COMBINE       = E_INT_FIQ_0x150_START + 7,
    E_INT_IRQ_GPIO22_INT_INV            = E_INT_FIQ_0x150_START + 8,
    E_INT_IRQ_AESDMA                    = E_INT_FIQ_0x150_START + 9,
    E_INT_FIQ_MIU_CMA_CLR               = E_INT_FIQ_0x150_START + 10,
    E_INT_FIQ_TSP_SPARE_CA2PM_8         = E_INT_FIQ_0x150_START + 11,
    E_INT_FIQ_MIU_NS                    = E_INT_FIQ_0x150_START + 12,
    E_INT_FIQ_0x150_END                 = 0x15F,


    E_INT_IRQ_0x160_START               = 0x160,
    E_INT_IRQ_AESDMA2_S_INT             = E_INT_IRQ_0x160_START + 0,    //Maserati
    E_INT_IRQ_AESMDA2_INT               = E_INT_IRQ_0x160_START + 1,    //Maserati
    E_INT_IRQ_RXIU_TIMEOUT_NODEF_INT    = E_INT_IRQ_0x160_START + 2,    //Maxim
    E_INT_IRQ_EVD_LITE                  = E_INT_IRQ_0x160_START + 3,    //Kano
    E_INT_IRQ_PCIE                      = E_INT_IRQ_0x160_START + 4,    //Kano
    E_INT_IRQ_PCIE_PHY                  = E_INT_IRQ_0x160_START + 5,    //Kano
    E_INT_IRQ_SMART1                    = E_INT_IRQ_0x160_START + 6,    //Kano
    E_INT_IRQ_TSIO_INT                  = E_INT_IRQ_0x160_START + 7,    //Kastor
    E_INT_IRQ_SATA_PHY                  = E_INT_IRQ_0x160_START + 8,    //Kastor
    E_INT_IRQ_TSIO_LOC_DEC              = E_INT_IRQ_0x160_START + 9,    //Kastor
    E_INT_IRQ_PKA_ALL_INT               = E_INT_IRQ_0x160_START + 10,   //Kastor
    E_INT_IRQ_VIVALDI_V9_MIU_AL         = E_INT_IRQ_0x160_START + 11,   //Kastor
    E_INT_IRQ_PWD_STATUS_INT            = E_INT_IRQ_0x160_START + 12,   //Kastor
    E_INT_IRQ_PM_IRQ_OUT                = E_INT_IRQ_0x160_START + 13,   //Mooney
    E_INT_IRQ_RXIU_TIMEOUT_NODEFINE_INT = E_INT_IRQ_0x160_START + 14,   //Mooney
    E_INT_IRQ_0x160_END                 = 0x16F,

    E_INT_IRQ_0x170_START               = 0x170,
    E_INT_IRQ_GMAC                      = E_INT_IRQ_0x170_START + 0,    //Curry
    E_INT_FIQ_SEC_TIMER0                = E_INT_IRQ_0x170_START + 1,    //Kastor
    E_INT_FIQ_SEC_TIMER1                = E_INT_IRQ_0x170_START + 2,    //Kastor
    E_INT_FIQ_SEC_GUARD_INT             = E_INT_IRQ_0x170_START + 3,    //Kastor
    E_INT_FIQ_USB_INT_P0                = E_INT_IRQ_0x170_START + 4,    //Kastor
    E_INT_FIQ_UHC_INT_P0                = E_INT_IRQ_0x170_START + 5,    //Kastor
    E_INT_FIQ_USB30_SS_INT              = E_INT_IRQ_0x170_START + 6,    //Kastor
    E_INT_FIQ_OTG_INT_P0                = E_INT_IRQ_0x170_START + 7,    //Kastor
    E_INT_FIQ_USB_INT_P1                = E_INT_IRQ_0x170_START + 8,    //Kastor
    E_INT_FIQ_UHC_INT_P1                = E_INT_IRQ_0x170_START + 9,    //Kastor
    E_INT_FIQ_USB31_HS_USB_INT	        = E_INT_IRQ_0x170_START + 10,   //Kastor
    E_INT_FIQ_USB30_HS_UHC_INT	        = E_INT_IRQ_0x170_START + 11,   //Kastor
    E_INT_IRQ_UART5                     = E_INT_IRQ_0x170_START + 12,   //Munich
    E_INT_IRQ_DIAMOND_L3_BRIDGE         = E_INT_IRQ_0x170_START + 13,   //Macan
    E_INT_IRQ_MOD_DETECT_INT            = E_INT_IRQ_0x170_START + 14,   //Macan
    E_INT_IRQ_IMI_TOP_INT               = E_INT_IRQ_0x170_START + 15,   //M5621
    E_INT_IRQ_0x170_END                 = 0x17F,

    E_INT_IRQ_0x180_START               = 0x180,
    E_INT_IRQ_GPIO12_INT_INV            = E_INT_IRQ_0x180_START + 1,    //kiwi
    E_INT_IRQ_GPIO12_INT                = E_INT_IRQ_0x180_START + 2,    //kiwi
    E_INT_FIQ_GPIO12_INT_INV            = E_INT_IRQ_0x180_START + 3,    //kiwi
    E_INT_FIQ_GPIO12_INT                = E_INT_IRQ_0x180_START + 4,    //kiwi
    E_INT_FIQ_GPIO22_INT_INV            = E_INT_IRQ_0x180_START + 5,    //kiwi
    E_INT_IRQ_GPIO22_INT                = E_INT_IRQ_0x180_START + 6,    //kiwi
    E_INT_IRQ_TS0_VLD                   = E_INT_IRQ_0x180_START + 7,    //k5tn
    E_INT_IRQ_TS1_VLD                   = E_INT_IRQ_0x180_START + 8,    //k5tn
    E_INT_IRQ_TSIO                      = E_INT_IRQ_0x180_START + 9,    //k5tn
    E_INT_IRQ_GPIO_TRIGGER              = E_INT_IRQ_0x180_START + 10,   //k5tn
    E_INT_IRQ_RTC2                      = E_INT_IRQ_0x180_START + 11,   //k5tn
    E_INT_IRQ_VDR2_ADDR_LIMIT           = E_INT_IRQ_0x180_START + 12,   //k5tn
    E_INT_IRQ_VIVALDI_AL_L              = E_INT_IRQ_0x180_START + 13,   //k5tn
    E_INT_FIQ_SEC_GUARD                 = E_INT_IRQ_0x180_START + 14,   //k5tn
    E_INT_IRQ_DCSUB_INT                 = E_INT_IRQ_0x180_START + 15,   //M5621
    E_INT_IRQ_0x180_END                 = 0x18F,

    E_INT_FIQ_0x190_START               = 0x190,
    E_INT_FIQ_TS0_VLD                   = E_INT_FIQ_0x190_START + 1,    //kyoto
    E_INT_FIQ_TS1_VLD                   = E_INT_FIQ_0x190_START + 2,    //kyoto
    E_INT_FIQ_TS2_VLD                   = E_INT_FIQ_0x190_START + 3,    //kyoto
    E_INT_FIQ_TS3_VLD                   = E_INT_FIQ_0x190_START + 4,    //kyoto
    E_INT_FIQ_TS4_VLD                   = E_INT_FIQ_0x190_START + 5,    //kyoto
    E_INT_FIQ_TS5_VLD                   = E_INT_FIQ_0x190_START + 6,    //kyoto
    E_INT_FIQ_TS3_CLK                   = E_INT_FIQ_0x190_START + 7,    //kyoto
    E_INT_FIQ_TS4_CLK                   = E_INT_FIQ_0x190_START + 8,    //kyoto
    E_INT_FIQ_TS5_CLK                   = E_INT_FIQ_0x190_START + 9,    //kyoto
    E_INT_FIQ_TS6_CLK                   = E_INT_FIQ_0x190_START + 10,   //kyoto
    E_INT_FIQ_EMAC_MAN_INT              = E_INT_FIQ_0x190_START + 11,   //Mooney
    E_INT_IRQ_GMAC_TX                   = E_INT_FIQ_0x190_START + 12,   //K6U02
    E_INT_IRQ_FCIE_TEE_INIT             = E_INT_FIQ_0x190_START + 13,   //k7u
    E_INT_IRQ_USB30ML_SS_INT            = E_INT_FIQ_0x190_START + 14,   //k7u
    E_INT_IRQ_USB30ML_HS_UHC_INIT       = E_INT_FIQ_0x190_START + 15,   //k7u
    E_INT_FIQ_0x190_END                 = 0x19F,

    E_INT_IRQ_0x1a0_START               = 0x1a0,
    E_INT_IRQ_USB30ML_HS_USB_INIT       = E_INT_IRQ_0x1a0_START + 1,    //k7u
    E_INT_IRQ_NOE_IRQ0                  = E_INT_IRQ_0x1a0_START + 2,    //k7u
    E_INT_FIQ_HST_FOSSIL_TO_SECR2_INT   = E_INT_IRQ_0x1a0_START + 3,    //k7u
    E_INT_FIQ_HST_FOSSIL_TO_SEC51_INT   = E_INT_IRQ_0x1a0_START + 4,    //k7u
    E_INT_FIQ_HST_FOSSIL_TO_DIAMOND_INT = E_INT_IRQ_0x1a0_START + 5,    //k7u
    E_INT_FIQ_HST_DIAMOND_TO_SECR2_INT  = E_INT_IRQ_0x1a0_START + 6,    //k7u
    E_INT_FIQ_HST_DIAMOND_TO_SEC51_INT  = E_INT_IRQ_0x1a0_START + 7,    //k7u
    E_INT_FIQ_HST_DIAMOND_TO_FOSSIL_INT = E_INT_IRQ_0x1a0_START + 8,    //k7u
    E_INT_FIQ_HST_SECR2_TO_SEC51_INT    = E_INT_IRQ_0x1a0_START + 9,    //k7u
    E_INT_FIQ_HST_SECR2_TO_FOSSIL_INT   = E_INT_IRQ_0x1a0_START + 10,   //k7u
    E_INT_FIQ_HST_SECR2_TO_DIAMOND_INT  = E_INT_IRQ_0x1a0_START + 11,   //k7u
    E_INT_FIQ_HST_SEC51_TO_SECR2_INT    = E_INT_IRQ_0x1a0_START + 12,   //k7u
    E_INT_FIQ_HST_SEC51_TO_FOSSIL_INT   = E_INT_IRQ_0x1a0_START + 13,   //k7u
    E_INT_FIQ_HST_SEC51_TO_DIAMOND_INT  = E_INT_IRQ_0x1a0_START + 14,   //k7u
    E_INT_IRQ_NFIE_TOP                  = E_INT_IRQ_0x1a0_START + 15,   //M5621
    E_INT_IRQ_0x1a0_END                 = 0x1af,

    E_INT_IRQ_0x1B0_START               = 0x1b0,
    E_INT_FIQ_PM_GPIO7                  = E_INT_IRQ_0x1B0_START + 0,   //M5621
    E_INT_IRQ_CA_CERT                   = E_INT_IRQ_0x1B0_START + 1,   //k7u
    E_INT_IRQ_DIAMOND_RXIU_TIMEOUT_NODEFINE_INT = E_INT_IRQ_0x1B0_START + 2,   //k7u
    E_INT_IRQ_IM_INVLD_INT              = E_INT_IRQ_0x1B0_START + 3,   //k7u
    E_INT_IRQ_PE2_RC_1_INT              = E_INT_IRQ_0x1B0_START + 4,   //k7u
    E_INT_IRQ_FOSSIL_REG_RXIU_TIMEOUT_NODEFINE_INT = E_INT_IRQ_0x1B0_START + 5,   //k7u
    E_INT_IRQ_COMPA_INT                 = E_INT_IRQ_0x1B0_START + 6,   //k7u
    E_INT_IRQ_NOE_IRQ1                  = E_INT_IRQ_0x1B0_START + 7,   //k7u
    E_INT_IRQ_WED0_IRQ                  = E_INT_IRQ_0x1B0_START + 8,   //k7u
    E_INT_IRQ_WED1_IRQ                  = E_INT_IRQ_0x1B0_START + 9,   //k7u
    E_INT_IRQ_AU_VREC_INT               = E_INT_IRQ_0x1B0_START +10,   //k7u
    E_INT_IRQ_LUT2MI_INT_PVR_PTS_INT    = E_INT_IRQ_0x1B0_START +11,   //k7u
    E_INT_IRQ_EMM_INT_P0_P1             = E_INT_IRQ_0x1B0_START +12,   //k7u
    E_INT_IRQ_MBOX_1_ACPU_TYPE5_IRQ     = E_INT_IRQ_0x1B0_START +13,   //k7u
    E_INT_IRQ_MBOX_2_ACPU_IRQ           = E_INT_IRQ_0x1B0_START +14,   //k7u
    E_INT_IRQ_MVD2_INT                  = E_INT_IRQ_0x1B0_START +15,   //k7u
    E_INT_IRQ_0x1B0_END                 = 0x1bf,

    E_INT_IRQ_0x1C0_START               = 0x1c0,
    E_INT_IRQ_MVD22MIPS_INT             = E_INT_IRQ_0x1C0_START + 0,   //k7u
    E_INT_IRQ_CILINK_SOC_IF_INT         = E_INT_IRQ_0x1C0_START + 1,   //k7u
    E_INT_IRQ_SECGMAC_TX_INT            = E_INT_IRQ_0x1C0_START + 2,   //k7u
    E_INT_FIQ_R2TOMCU_INT4              = E_INT_IRQ_0x1C0_START + 3,   //k7u
    E_INT_FIQ_R2TOMCU_INT5              = E_INT_IRQ_0x1C0_START + 4,   //k7u
    E_INT_FIQ_RTC2_SEC_INT              = E_INT_IRQ_0x1C0_START + 5,   //k7u
    E_INT_FIQ_USB_INT_P2                = E_INT_IRQ_0x1C0_START + 6,   //k7u
    E_INT_FIQ_UHC_INT_P2                = E_INT_IRQ_0x1C0_START + 7,   //k7u
    E_INT_IRQ_MHE                       = E_INT_IRQ_0x1C0_START + 8,   //c2p
    E_INT_IRQ_MIU_CMA_CLR               = E_INT_IRQ_0x1C0_START + 9,   //M7621
    E_INT_FIQ_8051_TO_CM4               = E_INT_IRQ_0x1C0_START +10,   //M7221
    E_INT_FIQ_BEON_TO_CM4               = E_INT_IRQ_0x1C0_START +11,   //M7221
    E_INT_FIQ_MIPS_VPE1_TO_CM4          = E_INT_IRQ_0x1C0_START +12,   //M7221
    E_INT_FIQ_CM4_TO_8051               = E_INT_IRQ_0x1C0_START +13,   //M7221
    E_INT_FIQ_CM4_TO_BEON               = E_INT_IRQ_0x1C0_START +14,   //M7221
    E_INT_FIQ_CM4_TO_MIPS_VPE1          = E_INT_IRQ_0x1C0_START +15,   //M7221
    E_INT_IRQ_0x1C0_END                 = 0x1cf,

    E_INT_IRQ_0x1D0_START               = 0x1d0,
    E_INT_IRQ_MEMC_PHASE_ERR_INT        = E_INT_IRQ_0x1D0_START + 0,   //M7622
    E_INT_IRQ_TSEN_OV_INT               = E_INT_IRQ_0x1D0_START + 1,   //M7622
    E_INT_IRQ_DISP_MFDEC_INT            = E_INT_IRQ_0x1D0_START + 2,   //M7622
    E_INT_FIQ_FO_INT                    = E_INT_IRQ_0x1D0_START + 3,   //M7622
    E_INT_FIQ_MB_A2M_INT4               = E_INT_IRQ_0x1D0_START + 4,   //M7622
    E_INT_FIQ_MB_A2M_INT5               = E_INT_IRQ_0x1D0_START + 5,   //M7622
    E_INT_IRQ_LPLL_INTR                 = E_INT_IRQ_0x1D0_START + 6,   //M7322
    E_INT_IRQ_MSPI_CILINK_INT           = E_INT_IRQ_0x1D0_START + 7,   //M7322
    E_INT_IRQ_CI_GPIO_INT               = E_INT_IRQ_0x1D0_START + 8,   //M7322
    E_INT_IRQ_TSEN_1_OV_INT             = E_INT_IRQ_0x1D0_START + 9,   //M7322
    E_INT_FIQ_USB_INT_MERGE3            = E_INT_IRQ_0x1D0_START +10,   //M7322

    E_INT_IRQ_MAX,
} InterruptNum;

typedef enum
{
    // IRQ
    E_FRCINT_IRQ_0x00_START             = 0x00,
    E_FRCINT_IRQ_ERROR_RESP_INT         = E_FRCINT_IRQ_0x00_START+ 6, //manhattan
    E_FRCINT_IRQ_MC2D_MEDONE_INT3       = E_FRCINT_IRQ_0x00_START+ 7, //manhattan
    E_FRCINT_IRQ_MC2D_MEDONE_INT2       = E_FRCINT_IRQ_0x00_START+ 8, //manhattan
    E_FRCINT_IRQ_MC2D_MEDONE_INT1       = E_FRCINT_IRQ_0x00_START+ 9, //manhattan
    E_FRCINT_IRQ_MC2D_MEDONE_INT0       = E_FRCINT_IRQ_0x00_START+10, //manhattan
    E_FRCINT_IRQ_FSC_INT1               = E_FRCINT_IRQ_0x00_START+11, //manhattan
    E_FRCINT_IRQ_FSC_INT0               = E_FRCINT_IRQ_0x00_START+12, //manhattan
    E_FRCINT_IRQ_FO_INT_CPU0_OP_INT     = E_FRCINT_IRQ_0x00_START+13, //maserati
    E_FRCINT_IRQ_FO_INT_CPU1_OP_INT     = E_FRCINT_IRQ_0x00_START+14, //maserati
    E_FRCINT_IRQ_0x00_END               = 0x0F,

    E_FRCINT_IRQ_0x10_START             = 0x10,
    E_FRCINT_IRQ_FRC_XIU_TIMEOUT        = E_FRCINT_IRQ_0x10_START+5,
    E_FRCINT_IRQ_PWM_RP_L               = E_FRCINT_IRQ_0x10_START+6,
    E_FRCINT_IRQ_PWM_FP_L               = E_FRCINT_IRQ_0x10_START+7,
    E_FRCINT_IRQ_PWM_RP_R               = E_FRCINT_IRQ_0x10_START+8,
    E_FRCINT_IRQ_PWM_FP_R               = E_FRCINT_IRQ_0x10_START+9,
    E_FRCINT_IRQ_SC                     = E_FRCINT_IRQ_0x10_START+10,
    E_FRCINT_IRQ_D2B                    = E_FRCINT_IRQ_0x10_START+11,
    E_FRCINT_IRQ_MSPI3                  = E_FRCINT_IRQ_0x10_START+12, // agate
    E_FRCINT_IRQ_MSPI2                  = E_FRCINT_IRQ_0x10_START+13, // agate
    E_FRCINT_IRQ_MSPI1                  = E_FRCINT_IRQ_0x10_START+14,
    E_FRCINT_IRQ_MSPI0                  = E_FRCINT_IRQ_0x10_START+15,
    E_FRCINT_IRQ_0x10_END               = 0x1F,

    // FIQ
    E_FRCINT_FIQ_0x20_START             = 0x20,
    E_FRCINT_FIQ_HST0_TO_HST1           = E_FRCINT_FIQ_0x20_START+ 0, //manhattan
    E_FRCINT_FIQ_HST0_TO_HST2           = E_FRCINT_FIQ_0x20_START+ 1, //manhattan
    E_FRCINT_FIQ_HST0_TO_HST3           = E_FRCINT_FIQ_0x20_START+ 2, //manhattan
    E_FRCINT_FIQ_HST1_TO_HST0           = E_FRCINT_FIQ_0x20_START+ 3, //manhattan
    E_FRCINT_FIQ_HST1_TO_HST2           = E_FRCINT_FIQ_0x20_START+ 4, //manhattan
    E_FRCINT_FIQ_HST1_TO_HST3           = E_FRCINT_FIQ_0x20_START+ 5, //manhattan
    E_FRCINT_FIQ_HST2_TO_HST0           = E_FRCINT_FIQ_0x20_START+ 6, //manhattan
    E_FRCINT_FIQ_HST2_TO_HST1           = E_FRCINT_FIQ_0x20_START+ 7, //manhattan
    E_FRCINT_FIQ_HST2_TO_HST3           = E_FRCINT_FIQ_0x20_START+ 8, //manhattan
    E_FRCINT_FIQ_HST3_TO_HST0           = E_FRCINT_FIQ_0x20_START+ 9, //manhattan
    E_FRCINT_FIQ_HST3_TO_HST1           = E_FRCINT_FIQ_0x20_START+10, //manhattan
    E_FRCINT_FIQ_HST3_TO_HST2           = E_FRCINT_FIQ_0x20_START+11, //manhattan
    E_FRCINT_FIQ_FRC_TIMER0             = E_FRCINT_FIQ_0x20_START+12, //manhattan
    E_FRCINT_FIQ_FRC_TIMER1             = E_FRCINT_FIQ_0x20_START+13, //manhattan

    //Special definition for FRC
    E_FRCINT_FIQ_HKCPU_TO_FRCR2         = E_FRCINT_FIQ_HST0_TO_HST1,
    E_FRCINT_FIQ_FRCR2_TO_HKCPU         = E_FRCINT_FIQ_HST1_TO_HST0,
    E_FRCINT_FIQ_HKIPVS_TO_FRCR2        = E_FRCINT_FIQ_HST0_TO_HST2,
    E_FRCINT_FIQ_HKOPVS_TO_FRCR2        = E_FRCINT_FIQ_HST0_TO_HST3,
    E_FRCINT_FIQ_MIPS_TO_FRCR2          = E_FRCINT_FIQ_HST0_TO_HST1,
    E_FRCINT_FIQ_FRCR2_TO_MIPS          = E_FRCINT_FIQ_HST1_TO_HST2,

    E_FRCINT_FIQ_0x20_END               = 0x2F,

    E_FRCINT_FIQ_0x30_START             = 0x30,
    E_FRCINT_FIQ_FRC_XIU_TIMEOUT        = E_FRCINT_FIQ_0x30_START+ 2, //manhattan
    E_FRCINT_FIQ_FRC_TO_MCU             = E_FRCINT_FIQ_0x30_START+ 4, //manhattan
    E_FRCINT_FIQ_MCU_TO_FRC             = E_FRCINT_FIQ_0x30_START+ 5, //manhattan

    E_FRCINT_FIQ_PWM_RP_L               = E_FRCINT_FIQ_0x30_START+ 6,
    E_FRCINT_FIQ_PWM_FP_L               = E_FRCINT_FIQ_0x30_START+ 7,
    E_FRCINT_FIQ_PWM_RP_R               = E_FRCINT_FIQ_0x30_START+ 8,
    E_FRCINT_FIQ_PWM_FP_R               = E_FRCINT_FIQ_0x30_START+ 9,
    E_FRCINT_FIQ_LDM_DMA_DONE3          = E_FRCINT_FIQ_0x30_START+10,
    E_FRCINT_FIQ_LDM_DMA_DONE2          = E_FRCINT_FIQ_0x30_START+11,
    E_FRCINT_FIQ_LDM_DMA_DONE1          = E_FRCINT_FIQ_0x30_START+12,
    E_FRCINT_FIQ_LDM_DMA_DONE0          = E_FRCINT_FIQ_0x30_START+13,
    E_FRCINT_FIQ_SC                     = E_FRCINT_FIQ_0x30_START+14,
    E_FRCINT_FIQ_OP2_VS                 = E_FRCINT_FIQ_0x30_START+15,
    E_FRCINT_FIQ_0x30_END               = 0x3F,

    E_FRCINT_FIQ_0x40_START		= 0x40,
    E_FRCINT_FIQ_MC2D_MEDONE_INT3	= E_FRCINT_FIQ_0x40_START+ 1,
    E_FRCINT_FIQ_MC2D_MEDONE_INT2	= E_FRCINT_FIQ_0x40_START+ 2,
    E_FRCINT_FIQ_FSC_INIT1	 	= E_FRCINT_FIQ_0x40_START+ 3,
    E_FRCINT_FIQ_FSC_INIT0		= E_FRCINT_FIQ_0x40_START+ 4,
    E_FRCINT_FIQ_FO_INT_CPU0_OP		= E_FRCINT_FIQ_0x40_START+ 5,
    E_FRCINT_FIQ_FO_INT_CPU1_OP         = E_FRCINT_FIQ_0x40_START+ 6,

    E_FRCINT_FIQ_0x40_END		= E_FRCINT_FIQ_0x40_START+15,

    // END
    E_FRCINT_FIQ_0xF0_START             = 0xF0,
    E_FRCINT_IRQ_FIQ_NONE               = E_FRCINT_FIQ_0xF0_START+14,
    E_FRCINT_IRQ_FIQ_ALL                = E_FRCINT_FIQ_0xF0_START+15,
    E_FRCINT_FIQ_0xF0_END               = 0xFF,

} InterruptNum_Frc;

#ifdef __cplusplus
}
#endif

#endif // _MS_IRQ_H_
