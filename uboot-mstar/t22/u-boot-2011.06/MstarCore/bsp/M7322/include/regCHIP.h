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
/// file    regSystem.h
/// @brief  System Chip Top Registers Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SYSTEM_H_
#define _REG_SYSTEM_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define REG_TOP_BASE                0xBF803C00
// Register access
#define TOP_READ(addr)              READ_WORD(REG_TOP_BASE + ((addr)<<2))
#define TOP_WRITE(addr, val)        WRITE_WORD((REG_TOP_BASE + ((addr)<<2)), (val))
// Register access utility
#define TOP_OR(addr, val)           TOP_WRITE(addr, TOP_READ(addr) | (val))
#define TOP_AND(addr, val)          TOP_WRITE(addr, TOP_READ(addr) & (val))
#define TOP_XOR(addr, val)          TOP_WRITE(addr, TOP_READ(addr) ^ (val))

#define REG_TOP_DEVICE_ID           0x00DD
#define REG_TOP_CHIP_VERSION        0x00DE
#define CHIP_VERSION_SHFT           0
#define CHIP_VERSION_MASK           BMASK(7:0)
#define CHIP_REVISION_SHFT          8
#define CHIP_REVISION_MASK          BMASK(15:8)

#ifdef MCU_AEON
    #define REG_IRQ_BASE            0xA0200000+(0x0c80<<2) // 0xBF805600

    #define REG_FIQ_MASK_L          0x0024
    #define REG_FIQ_MASK_H          0x0025
    #define REG_FIQEXP_MASK_L       0x0026
    #define REG_FIQEXP_MASK_H       0x0027
    #define REG_FIQ_CLEAR_L         0x002c
    #define REG_FIQ_CLEAR_H         0x002d
    #define REG_FIQEXP_CLEAR_L      0x002e
    #define REG_FIQEXP_CLEAR_H      0x002f
    #define REG_FIQ_PENDING_L       0x002c
    #define REG_FIQ_PENDING_H       0x002d
    #define REG_FIQEXP_PENDING_L    0x002e
    #define REG_FIQEXP_PENDING_H    0x002f

    #define REG_IRQ_MASK_L          0x0034
    #define REG_IRQ_MASK_H          0x0035
    #define REG_IRQEXP_MASK_L       0x0036
    #define REG_IRQEXP_MASK_H       0x0037
    #define REG_IRQ_PENDING_L       0x003c
    #define REG_IRQ_PENDING_H       0x003d
    #define REG_IRQEXP_PENDING_L    0x003e
    #define REG_IRQEXP_PENDING_H    0x003f
#else
// for MIPS VPE 0
#define REG_IRQ_BASE            0xBF200000+(0x0c80<<2) // 0xBF800A80

//[CHIP][HAL][001] Set byte address of FIQ [START]
#define REG_FIQ_MASK_L          0x0024
#define REG_FIQ_MASK_H          0x0025
#define REG_FIQEXP_MASK_L       0x0026
#define REG_FIQEXP_MASK_H       0x0027
#define REG_FIQ_CLEAR_L         0x002c
#define REG_FIQ_CLEAR_H         0x002d
#define REG_FIQEXP_CLEAR_L      0x002e
#define REG_FIQEXP_CLEAR_H      0x002f
#define REG_FIQ_PENDING_L       0x002c
#define REG_FIQ_PENDING_H       0x002d
#define REG_FIQEXP_PENDING_L    0x002e
#define REG_FIQEXP_PENDING_H    0x002f
//[CHIP][HAL][001] Set byte address of FIQ [END]
//[CHIP][HAL][002] Set byte address of IRQ [START]
#define REG_IRQ_MASK_L          0x0034
#define REG_IRQ_MASK_H          0x0035
#define REG_IRQEXP_MASK_L       0x0036
#define REG_IRQEXP_MASK_H       0x0037
#define REG_IRQ_PENDING_L       0x003c
#define REG_IRQ_PENDING_H       0x003d
#define REG_IRQEXP_PENDING_L    0x003e
#define REG_IRQEXP_PENDING_H    0x003f
//[CHIP][HAL][002] Set byte address of IRQ [END]
#endif

#define IRQ_REG(addr)           (*((volatile MS_U16*)(REG_IRQ_BASE + ((addr)<<2))))

//[CHIP][HAL][003] Update bit address of FIQ source [START]
    // REG_FIQ_MASK_L
    //FIQ Low 16 bits
    #define FIQL_MASK                           0xFFFF
    #define FIQ_EXTIMER0                        (0x1 << (E_FIQ_EXTIMER0         - E_FIQL_START) )
    #define FIQ_EXTIMER1                        (0x1 << (E_FIQ_EXTIMER1         - E_FIQL_START) )
    #define FIQ_WDT                             (0x1 << (E_FIQ_WDT              - E_FIQL_START) )
    #define FIQ_MB_auR2toMCU_INT0               (0x1 << (E_FIQ_AEON_MB2_MCU0    - E_FIQL_START) )
    #define FIQ_MB_auR2toMCU_INT1               (0x1 << (E_FIQ_AEON_MB2_MCU1    - E_FIQL_START) )
    #define FIQ_MB_DSP2toMCU_INT0               (0x1 << (E_FIQ_DSP2_MB2_MCU0    - E_FIQL_START) )
    #define FIQ_MB_DSP2toMCU_INT1               (0x1 << (E_FIQ_DSP2_MB2_MCU1    - E_FIQL_START) )
    #define FIQ_USB_INT                         (0x1 << (E_FIQ_USB              - E_FIQL_START) )
    #define FIQ_UHC_INT                         (0x1 << (E_FIQ_UHC              - E_FIQL_START) )
    #define FIQ_HDMI_NON_PCM                    (0x1 << (E_FIQ_HDMI_NON_PCM     - E_FIQL_START) )
    #define FIQ_SPDIF_IN_NON_PCM                (0x1 << (E_FIQ_SPDIF_IN_NON_PCM - E_FIQL_START) )
    #define FIQ_EMAC                            (0x1 << (E_FIQ_EMAC             - E_FIQL_START) )
    #define FIQ_SE_DSP2UP                       (0x1 << (E_FIQ_SE_DSP2UP        - E_FIQL_START) )
    #define FIQ_TSP2AEON                        (0x1 << (E_FIQ_TSP2AEON         - E_FIQL_START) )


    // REG_FIQ_MASK_H
    //FIQ High 16 bits
    #define FIQH_MASK                           0xFFFF
    #define FIQ_VIVALDI_STR                     (0x1 << (E_FIQ_VIVALDI_STR  - E_FIQH_START) )
    #define FIQ_VIVALDI_PTS                     (0x1 << (E_FIQ_VIVALDI_PTS  - E_FIQH_START) )
    #define FIQ_DSP_MIU_PROT                    (0x1 << (E_FIQ_DSP_MIU_PROT - E_FIQH_START) )
    #define FIQ_XIU_TIMEOUT                     (0x1 << (E_FIQ_XIU_TIMEOUT  - E_FIQH_START) )
    #define FIQ_DMDMCU2HK                       (0x1 << (E_FIQ_DMDMCU2HK    - E_FIQH_START) )
    #define FIQ_VSYNC_VE4VBI                    (0x1 << (E_FIQ_VSYNC_VE4VBI - E_FIQH_START) )
    #define FIQ_FIELD_VE4VBI                    (0x1 << (E_FIQ_FIELD_VE4VBI - E_FIQH_START) )
    #define FIQ_VDMCU2HK                        (0x1 << (E_FIQ_VDMCU2HK     - E_FIQH_START) )
    #define FIQ_VE_DONE_TT                      (0x1 << (E_FIQ_VE_DONE_TT   - E_FIQH_START) )
    #define FIQ_INT_CCFL                        (0x1 << (E_FIQ_INT_CCFL     - E_FIQH_START) )
    #define FIQ_INT                             (0x1 << (E_FIQ_INT          - E_FIQH_START) )
    #define FIQ_IR                              (0x1 << (E_FIQ_IR           - E_FIQH_START) )
    #define FIQ_AFEC_VSYNC                      (0x1 << (E_FIQ_AFEC_VSYNC   - E_FIQH_START) )
    #define FIQ_DEC_DSP2UP                      (0x1 << (E_FIQ_DEC_DSP2UP   - E_FIQH_START) )
    #define FIQ_FRC_R2_TO_MIPS                  (0x1 << (E_FIQ_FRC_R2_TO_MIPS  - E_FIQH_START) )
    #define FIQ_DSP2MIPS                        (0x1 << (E_FIQ_DSP2MIPS     - E_FIQH_START) )
//[CHIP][HAL][003] Update bit address of FIQ source [END]

//[CHIP][HAL][004] Update bit address of IRQ source [START]
    // #define REG_IRQ_PENDING_L
    #define IRQ_UART0                           (0x1 << (E_IRQ_UART0        - E_IRQL_START) )
    #define IRQ_PM_SLEEP                        (0x1 << (E_IRQ_PM_SLEEP     - E_IRQL_START) )
    #define IRQ_ONIF                            (0x1 << (E_IRQ_ONIF         - E_IRQL_START) )
    #define IRQ_MVD                             (0x1 << (E_IRQ_MVD          - E_IRQL_START) )
    #define IRQ_PS                              (0x1 << (E_IRQ_PS           - E_IRQL_START) )
    #define IRQ_NFIE                            (0x1 << (E_IRQ_NFIE         - E_IRQL_START) )
    #define IRQ_USB                             (0x1 << (E_IRQ_USB          - E_IRQL_START) )
    #define IRQ_UHC                             (0x1 << (E_IRQ_UHC          - E_IRQL_START) )
    #define IRQ_EC_BRIDGE                       (0x1 << (E_IRQ_EC_BRIDGE    - E_IRQL_START) )
    #define IRQ_EMAC                            (0x1 << (E_IRQ_EMAC         - E_IRQL_START) )
    #define IRQ_DISP                            (0x1 << (E_IRQ_DISP         - E_IRQL_START) )
    #define IRQ_FRC_SC                          (0x1 << (E_IRQ_FRC_SC       - E_IRQL_START) )
    #define IRQ_IIC_DMA_INT3                    (0x1 << (E_IRQ_MIIC_DMA_INT3 - E_IRQL_START) )
    #define IRQ_MIIC_INT3                       (0x1 << (E_IRQ_MIIC_INT3     - E_IRQL_START) )
    #define IRQ_COMB                            (0x1 << (E_IRQ_COMB         - E_IRQL_START) )
    #define IRQ_FRC_INT_FIQ2HST0                (0x1 << (E_IRQ_FRC_INT_FIQ2HST0 - E_IRQL_START) )


    // #define REG_IRQ_PENDING_H
    #define IRQH_MASK                           0xFFFF
    #define IRQ_TSP2HK                          (0x1 << (E_IRQ_TSP2HK       - E_IRQH_START) )
    #define IRQ_VE                              (0x1 << (E_IRQ_VE           - E_IRQH_START) )
    #define IRQ_CIMAX2MCU                       (0x1 << (E_IRQ_CIMAX2MCU    - E_IRQH_START) )
    #define IRQ_DC                              (0x1 << (E_IRQ_DC           - E_IRQH_START) )
    #define IRQ_GOP                             (0x1 << (E_IRQ_GOP          - E_IRQH_START) )
    #define IRQ_PCM                             (0x1 << (E_IRQ_PCM          - E_IRQH_START) )
    #define IRQ_IIC0                            (0x1 << (E_IRQ_IIC0         - E_IRQH_START) )
    #define IRQ_SMART                           (0x1 << (E_IRQ_SMART        - E_IRQH_START) )
    #define IRQ_DDC2BI                          (0x1 << (E_IRQ_DDC2BI       - E_IRQH_START) )
    #define IRQ_SCM                             (0x1 << (E_IRQ_SCM          - E_IRQH_START) )
    #define IRQ_VBI                             (0x1 << (E_IRQ_VBI          - E_IRQH_START) )    //#define IRQ_MLINK                           (0x1 << (E_IRQ_MLINK        - E_IRQH_START) )
    #define IRQ_MVD2MIPS                        (0x1 << (E_IRQ_MVD2MIPS     - E_IRQH_START) )
    #define IRQ_GPD                             (0x1 << (E_IRQ_GPD          - E_IRQH_START) )
    #define IRQ_ADCDVI2RIU                      (0x1 << (E_IRQ_ADCDVI2RIU   - E_IRQH_START) )

    //
    #define IRQEXPL_MASK                        0xFFFF
    #define IRQEXPL_HVD                         (0x1 << (E_IRQEXPL_HVD              - E_IRQEXPL_START) )
    #define IRQEXPL_USB1                        (0x1 << (E_IRQEXPL_USB1             - E_IRQEXPL_START) )
    #define IRQEXPL_UHC1                        (0x1 << (E_IRQEXPL_UHC1             - E_IRQEXPL_START) )
    #define IRQEXPL_MIU                         (0x1 << (E_IRQEXPL_MIU              - E_IRQEXPL_START) )
    #define IRQEXPL_USB2                        (0x1 << (E_IRQEXPL_USB2             - E_IRQEXPL_START) )
    #define IRQEXPL_UHC2                        (0x1 << (E_IRQEXPL_UHC2             - E_IRQEXPL_START) )
    #define IRQEXPL_AEON2HI                     (0x1 << (E_IRQEXPL_AEON2HI          - E_IRQEXPL_START) )
    #define IRQEXPL_UART1                       (0x1 << (E_IRQEXPL_UART1            - E_IRQEXPL_START) )
    #define IRQEXPL_UART2                       (0x1 << (E_IRQEXPL_UART2            - E_IRQEXPL_START) )
    #define IRQEXPL_FRC_INT_IRQ2HST0            (0x1 << (E_IRQEXPL_FRC_INT_IRQ2HST0 - E_IRQEXPL_START) )
    #define IRQEXPL_MPIF                        (0x1 << (E_IRQEXPL_MPIF             - E_IRQEXPL_START) )
    #define IRQ_IIC_DMA_INT2                    (0x1 << (E_IRQEXPL_MIIC_DMA_INT2    - E_IRQEXPL_START) )
    #define IRQ_MIIC_INT2                       (0x1 << (E_IRQEXPL_MIIC_INT2        - E_IRQEXPL_START) )
    #define IRQEXPL_JPD                         (0x1 << (E_IRQEXPL_JPD              - E_IRQEXPL_START) )
    #define IRQEXPL_DISPI                       (0x1 << (E_IRQEXPL_DISPI            - E_IRQEXPL_START) )
    #define IRQEXPL_MFE                         (0x1 << (E_IRQEXPL_MFE              - E_IRQEXPL_START) )

    #define IRQEXPH_MASK                        0xFFFF
    #define IRQEXPH_BDMA0                       (0x1 << (E_IRQEXPH_BDMA0            - E_IRQEXPH_START) )
    #define IRQEXPH_BDMA1                       (0x1 << (E_IRQEXPH_BDMA1            - E_IRQEXPH_START) )
    #define IRQEXPH_UART2MCU                    (0x1 << (E_IRQEXPH_UART2MCU         - E_IRQEXPH_START) )
    #define IRQEXPH_URDMA2MCU                   (0x1 << (E_IRQEXPH_URDMA2MCU        - E_IRQEXPH_START) )
    #define IRQEXPH_DVI_HDMI_HDCP               (0x1 << (E_IRQEXPH_DVI_HDMI_HDCP    - E_IRQEXPH_START) )
    #define IRQEXPH_G3D2MCU                     (0x1 << (E_IRQEXPH_G3D2MCU          - E_IRQEXPH_START) )
    #define IRQEXPH_CEC_INT_PM                  (0x1 << (E_IRQEXPH_CEC              - E_IRQEXPH_START) )
    #define IRQEXPH_HDCP_IIC                    (0x1 << (E_IRQEXPH_HDCP_IIC         - E_IRQEXPH_START) )
    #define IRQEXPH_HDCP_X74                    (0x1 << (E_IRQEXPH_HDCP_X74         - E_IRQEXPH_START) )
    #define IRQEXPH_WADR_ERR                    (0x1 << (E_IRQEXPH_WADR_ERR         - E_IRQEXPH_START) )
    #define IRQEXPH_DCSUB                       (0x1 << (E_IRQEXPH_DCSUB            - E_IRQEXPH_START) )
    #define IRQEXPH_GE                          (0x1 << (E_IRQEXPH_GE               - E_IRQEXPH_START) )
    #define IRQEXPH_MIIC_DMA_INT1               (0x1 << (E_IRQEXPH_MIIC_DMA_INT1    - E_IRQEXPH_START) )
    #define IRQEXPH_MIIC_INT1                   (0x1 << (E_IRQEXPH_MIIC_INT1        - E_IRQEXPH_START) )
    #define IRQEXPH_MIIC_DMA_INT0               (0x1 << (E_IRQEXPH_MIIC_DMA_INT0    - E_IRQEXPH_START) )
    #define IRQEXPH_MIIC_INT0                   (0x1 << (E_IRQEXPH_MIIC_INT0        - E_IRQEXPH_START) )

    #define FIQEXPL_MASK                        0xFFFF
    #define FIQEXPL_IR_INT_RC                   (0x1 << (E_FIQEXPL_IR_INT_RC                 - E_FIQEXPL_START) )
    #define FIQEXPL_AU_DMA_BUF_INT              (0x1 << (E_FIQEXPL_AU_DMA_BUF_INT            - E_FIQEXPL_START) )
    #define FIQEXPL_IR_IN                       (0x1 << (E_FIQEXPL_IR_IN                     - E_FIQEXPL_START) )
    #define FIQEXPL_8051_TO_MIPS_VPE0           (0x1 << (E_FIQEXPL_8051_TO_MIPS_VPE0         - E_FIQEXPL_START) )
    #define FIQEXPL_EXT_GPIO_INT0               (0x1 << (E_FIQEXPL_EXT_GPIO_INT0             - E_FIQEXPL_START) )
    #define FIQEXPL_MIPS_VPE0_TO_8051           (0x1 << (E_FIQEXPL_MIPS_VPE1_TO_8051         - E_FIQEXPL_START) )
    #define FIQEXPL_EXT_GPIO_INT1               (0x1 << (E_FIQEXPL_EXT_GPIO_INT1             - E_FIQEXPL_START) )
    #define FIQEXPL_EXT_GPIO_INT2               (0x1 << (E_FIQEXPL_EXT_GPIO_INT2             - E_FIQEXPL_START) )

    #define FIQEXPH_MASK                        0xFFFF
    #define FIQEXPH_USB_INT1                    (0x1 << (E_FIQEXPH_USB1                      - E_FIQEXPH_START) )
    #define FIQEXPH_UHC_INT1                    (0x1 << (E_FIQEXPH_UHC1                      - E_FIQEXPH_START) )
    #define FIQEXPH_USB_INT2                    (0x1 << (E_FIQEXPH_USB2                      - E_FIQEXPH_START) )
    #define FIQEXPH_UHC_INT2                    (0x1 << (E_FIQEXPH_UHC2                      - E_FIQEXPH_START) )
    #define FIQEXPH_EXT_GPIO_INT3               (0x1 << (E_FIQEXPH_EXT_GPIO_INT3             - E_FIQEXPH_START) )
    #define FIQEXPH_EXT_GPIO_INT4               (0x1 << (E_FIQEXPH_EXT_GPIO_INT4             - E_FIQEXPH_START) )
    #define FIQEXPH_EXT_GPIO_INT5               (0x1 << (E_FIQEXPH_EXT_GPIO_INT5             - E_FIQEXPH_START) )
    #define FIQEXPH_EXT_GPIO_INT6               (0x1 << (E_FIQEXPH_EXT_GPIO_INT6             - E_FIQEXPH_START) )
    #define FIQEXPH_PWM_RP_L                    (0x1 << (E_FIQEXPH_PWM_RP_L                  - E_FIQEXPH_START) )
    #define FIQEXPH_PWM_FP_L                    (0x1 << (E_FIQEXPH_PWM_FP_L                  - E_FIQEXPH_START) )
    #define FIQEXPH_PWM_RP_R                    (0x1 << (E_FIQEXPH_PWM_RP_R                  - E_FIQEXPH_START) )
    #define FIQEXPH_PWM_FP_R                    (0x1 << (E_FIQEXPH_PWM_FP_R                  - E_FIQEXPH_START) )
    #define FIQEXPH_EXT_GPIO_INT7               (0x1 << (E_FIQEXPH_EXT_GPIO_INT7             - E_FIQEXPH_START) )
//[CHIP][HAL][004] Update bit address of IRQ source [END]

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define INTERFACE extern

INTERFACE MS_U32 u32_ge0_mmio_base;


//extern MS_U32 u32_bdma_mmio_base;
//extern MS_U32 u32_scaler_mmio_base;

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#define     REG_GE0_BASE     u32_ge0_mmio_base
//#define     REG_BDMA_BASE     u32_bdma_mmio_base
//#define     REG_SCALER_BASE   u32_scaler_mmio_base


//-------------------------------------------------------------------------------------------------

// Macros

//-------------------------------------------------------------------------------------------------

#define MReg_Write2Byte(u32Base, u32Reg, u16Val )                                                 \
    do {((volatile MS_U16*)(u32Base))[((u32Reg))] = u16Val;} while(0)

#define MReg_Read2Byte( u32Base, u32Reg )                                                         \
    ((volatile MS_U16*)(u32Base))[((u32Reg) )]

#define MReg_WriteByte(u32Base, u32Reg, u8Val )                                                 \
    do{((volatile MS_U8*)(u32Base))[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;} while(0)

#define MReg_ReadByte( u32Base, u32Reg )                                                         \
    ((volatile MS_U8*)(u32Base))[((u32Reg) * 2) - ((u32Reg) & 1)]

 #define MReg_Write3Byte(u32Base, u32Reg, u32Val )   \
    do {                                                                     \
        if ((u32Reg) & 0x01)                                                                \
        {                                                                                               \
            MReg_WriteByte(u32Base, u32Reg , u32Val);                                    \
            MReg_Write2Byte(u32Base, (u32Reg + 1) , ((u32Val) >> 8));                                      \
        }                                                                                           \
        else                                                                                        \
        {                                                                                               \
            MReg_Write2Byte(u32Base, (u32Reg) ,  u32Val);                                                         \
            MReg_WriteByte(u32Base, (u32Reg + 2) ,  ((u32Val) >> 16));                             \
        }   \
    } while(0)

#define MReg_Write4Byte( u32Base, u32Reg, u32Val )                                               \
    do {                                                                     \
        if ((u32Reg) & 0x01)                                                      \
        {                                                                                               \
            MReg_WriteByte( u32Base, u32Reg ,  u32Val);                                         \
            MReg_Write2Byte( u32Base, (u32Reg + 1) , ( (u32Val) >> 8));                                      \
            MReg_WriteByte( u32Base, (u32Reg + 3) ,  ((u32Val) >> 24));                           \
        }                                                                                               \
        else                                                                                                \
        {                                                                                                   \
            MReg_Write2Byte(u32Base, u32Reg ,  u32Val);                                                             \
            MReg_Write2Byte(u32Base,  (u32Reg + 2) ,  ((u32Val) >> 16));                                             \
        }                                                                     \
    } while(0)

#define MReg_WriteByteMask(u32Base, u32Reg, u8Val, u8Msk )                                      \
    do {                                                                     \
        MReg_WriteByte( u32Base, u32Reg, (MReg_ReadByte(u32Base, ((u32Reg) )) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    } while(0)

#define MReg_Write2ByteMask( u32Base, u32Reg, u16Val , u16Msk)                                               \
    do {                                                                     \
        if ( ((u32Reg) & 0x01) )                                                        \
        {                                                                                           \
            MReg_WriteByteMask( u32Base, ((u32Reg)+1) , (((u16Val) & 0xff00)>>8) , (((u16Msk)&0xff00)>>8) );                                                                          \
            MReg_WriteByteMask( u32Base, (u32Reg) , ((u16Val) & 0x00ff) , ((u16Msk)&0x00ff) );                                                                          \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            MReg_Write2Byte(u32Base,  u32Reg ,  (((u16Val) & (u16Msk))  | (MReg_Read2Byte(u32Base,  u32Reg  ) & (~( u16Msk ))))  );                                                       \
        }      \
    } while(0)



#endif // _REG_SYSTEM_H_

