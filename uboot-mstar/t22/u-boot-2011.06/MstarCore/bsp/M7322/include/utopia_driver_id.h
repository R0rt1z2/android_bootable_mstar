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
#ifndef _UTOPIA_DRIVER_ID_H_
#define _UTOPIA_DRIVER_ID_H_


extern __attribute__((weak)) MS_U32 BDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 IR_TXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 AESDMAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 PCMCIAOpen(void** pInstantTmp, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 GOPOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 GFXOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 TVENCODEROpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 XCOpenFull(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 XCOpenMinimum(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 XCOpenForTEE(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 XCOpenForR2TEE(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 MBXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DIPOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 PNLOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 VBIOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 ACEOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 DLCOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 IROpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 SAROpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 PWSOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 FLASHOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 SEALOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 CMDQOpen(void** pInstance, void* pAttribute);
extern __attribute__((weak)) MS_U32 SEMOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 SWI2COpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 SYSOpen(void** pInstance, void* pAttribute);
extern __attribute__((weak)) MS_U32 VDEC_EX_V2_Open(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 MVOPOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32  DMXOpen(void** pInstance, void* pAttribute);
extern __attribute__((weak)) MS_U32 RTCOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 UARTOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 PWMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 JPEGOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 GPDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 MFEOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 CPUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 JPEG_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 MSPIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DSCMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DACOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 CH34Open(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 GOPSCDOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 ACPOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 AVDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 MIUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 HWI2COpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 LDOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 MHLOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 CECOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 HDMIRxOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 PQOpen(void** ppInstance,MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 AUDIOOpen(void** pInstance, void* pAttribute);
extern __attribute__((weak)) MS_U32 VIFOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 GESTROpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 ATSCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DTMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 ISDBTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DVBTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DVBCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DVBT2Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DVBSOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 HDMITXOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 CAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 WDTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 GPIOOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 CIPHEROpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 URDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 PVR_IFRAMELUTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 DMSOpen(void** ppInstance, const void* const pAttribute);
extern __attribute__((weak)) MS_U32 PMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 MSB124XOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 AKLOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 SCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 PKAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 MFDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
extern __attribute__((weak)) MS_U32 VENCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);

#define MODULE_TYPE_UTOPIA_FULL 1
#define MODULE_TYPE_BDMA_FULL BDMAOpen
#define MODULE_TYPE_IR_TX_FULL IR_TXOpen
#define MODULE_TYPE_AESDMA_FULL AESDMAOpen
#define MODULE_TYPE_DSCMB_FULL DSCMBOpen
#define MODULE_TYPE_CI_FULL PCMCIAOpen
#define MODULE_TYPE_GOP_FULL GOPOpen
#define MODULE_TYPE_GFX_FULL GFXOpen
#define MODULE_TYPE_TVENCODER_FULL TVENCODEROpen
#define MODULE_TYPE_XC_FULL     XCOpenFull
#define MODULE_TYPE_XC_MINIMUM  XCOpenMinimum
#define MODULE_TYPE_XC_TEE XCOpenForTEE
#define MODULE_TYPE_XC_R2TEE XCOpenForR2TEE
#define MODULE_TYPE_MBX_FULL MBXOpen
#define MODULE_TYPE_TSP_FULL NULL
#define MODULE_TYPE_DIP_FULL DIPOpen
#define MODULE_TYPE_PNL_FULL PNLOpen
#define MODULE_TYPE_VBI_FULL VBIOpen
#define MODULE_TYPE_ACE_FULL ACEOpen
#define MODULE_TYPE_DLC_FULL DLCOpen
#define MODULE_TYPE_IR_FULL IROpen
#define MODULE_TYPE_SAR_FULL SAROpen
#define MODULE_TYPE_MIU_FULL MIUOpen
#define MODULE_TYPE_PWS_FULL PWSOpen
#define MODULE_TYPE_FLASH_FULL FLASHOpen
#define MODULE_TYPE_SEAL_FULL NULL
#define MODULE_TYPE_CMDQ_FULL CMDQOpen
#define MODULE_TYPE_MMFI_FULL NULL
#define MODULE_TYPE_SEM_FULL SEMOpen
#define MODULE_TYPE_SWI2C_FULL SWI2COpen
#define MODULE_TYPE_SYS_FULL SYSOpen
#define MODULE_TYPE_VDEC_EX_FULL VDEC_EX_V2_Open
#define MODULE_TYPE_MVOP_FULL MVOPOpen
#define MODULE_TYPE_AVD_FULL AVDOpen
#define MODULE_TYPE_DMX_FULL DMXOpen
#define MODULE_TYPE_RTC_FULL RTCOpen
#define MODULE_TYPE_HWI2C_FULL HWI2COpen
#define MODULE_TYPE_VDEC_FULL NULL
#define MODULE_TYPE_UART_FULL UARTOpen
#define MODULE_TYPE_PWM_FULL PWMOpen
#define MODULE_TYPE_NJPEG_EX_FULL JPEGOpen
#define MODULE_TYPE_GPD_FULL GPDOpen
#define MODULE_TYPE_MFE_FULL MFEOpen
#define MODULE_TYPE_CPU_FULL CPUOpen
#define MODULE_TYPE_JPEG_EX_FULL JPEGOpen
#define MODULE_TYPE_JPEG_FULL NULL
#define MODULE_TYPE_MSPI_FULL MSPIOpen
#define MODULE_TYPE_DAC_FULL DACOpen
#define MODULE_TYPE_CH34_FULL CH34Open
#define MODULE_TYPE_GOPSCD_FULL GOPSCDOpen
#define MODULE_TYPE_ACP_FULL ACPOpen
#define MODULE_TYPE_LD_FULL LDOpen
#define MODULE_TYPE_MHL_FULL MHLOpen
#define MODULE_TYPE_CEC_FULL CECOpen
#define MODULE_TYPE_HDMIRX_FULL HDMIRxOpen
#define MODULE_TYPE_PQ_FULL PQOpen
#define MODULE_TYPE_AUDIO_FULL AUDIOOpen
#define MODULE_TYPE_VIF_FULL VIFOpen
#define MODULE_TYPE_GESTR_FULL GESTROpen
#define MODULE_TYPE_ATSC_FULL ATSCOpen
#define MODULE_TYPE_DTMB_FULL DTMBOpen
#define MODULE_TYPE_ISDBT_FULL ISDBTOpen
#define MODULE_TYPE_DVBT_FULL DVBTOpen
#define MODULE_TYPE_DVBC_FULL DVBCOpen
#define MODULE_TYPE_DVBT2_FULL DVBT2Open
#define MODULE_TYPE_DVBS_FULL DVBSOpen
#define MODULE_TYPE_HDMITX_FULL HDMITXOpen
#define MODULE_TYPE_CA_FULL CAOpen
#define MODULE_TYPE_WDT_FULL WDTOpen
#define MODULE_TYPE_GPIO_FULL GPIOOpen
#define MODULE_TYPE_CIPHER_FULL CIPHEROpen
#define MODULE_TYPE_URDMA_FULL URDMAOpen
#define MODULE_TYPE_PVR_IFRAMELUT_FULL PVR_IFRAMELUTOpen
#define MODULE_TYPE_DMS_FULL DMSOpen
#define MODULE_TYPE_PM_FULL PMOpen
#define MODULE_TYPE_MSB124X_FULL MSB124XOpen
#define MODULE_TYPE_AKL_FULL AKLOpen
#define MODULE_TYPE_SC_FULL SCOpen
#define MODULE_TYPE_PKA_FULL NULL
#define MODULE_TYPE_MFD_FULL MFDOpen
#define MODULE_TYPE_VENC_FULL VENCOpen

// add new module here
#define _INCLUDED_MODULE \
    PREFIX(BDMA) \
    PREFIX(AESDMA) \
    PREFIX(DSCMB) \
    PREFIX(CI) \
    PREFIX(GOP) \
    PREFIX(GFX) \
    PREFIX(TVENCODER) \
    PREFIX(XC) \
    PREFIX(MBX) \
    PREFIX(TSP) \
    PREFIX(DIP) \
    PREFIX(PNL) \
    PREFIX(VBI) \
    PREFIX(ACE) \
    PREFIX(DLC) \
    PREFIX(IR) \
    PREFIX(SAR) \
    PREFIX(MIU) \
    PREFIX(PWS) \
    PREFIX(FLASH) \
    PREFIX(SEAL) \
    PREFIX(CMDQ) \
    PREFIX(MMFI) \
    PREFIX(SEM) \
    PREFIX(SYS) \
    PREFIX(VDEC_EX) \
    PREFIX(MVOP)    \
    PREFIX(AVD)    \
    PREFIX(DMX) \
    PREFIX(RTC) \
    PREFIX(HWI2C) \
    PREFIX(VDEC) \
    PREFIX(UART) \
    PREFIX(PWM) \
    PREFIX(NJPEG_EX) \
    PREFIX(GPD) \
    PREFIX(MFE) \
    PREFIX(CPU) \
    PREFIX(JPEG_EX) \
    PREFIX(JPEG) \
    PREFIX(MSPI) \
    PREFIX(DAC) \
    PREFIX(CH34) \
    PREFIX(GOPSCD) \
    PREFIX(ACP) \
    PREFIX(LD) \
    PREFIX(MHL) \
    PREFIX(CEC) \
    PREFIX(HDMIRX) \
    PREFIX(PQ) \
    PREFIX(AUDIO) \
    PREFIX(VIF)    \
    PREFIX(GESTR) \
    PREFIX(ATSC) \
    PREFIX(DTMB) \
		PREFIX(ISDBT) \
		PREFIX(DVBT) \
		PREFIX(DVBC) \
    PREFIX(DVBT2) \
		PREFIX(DVBS) \
    PREFIX(HDMITX) \
    PREFIX(CA) \
    PREFIX(WDT) \
    PREFIX(GPIO) \
    PREFIX(CIPHER) \
    PREFIX(IR_TX) \
    PREFIX(URDMA) \
    PREFIX(PVR_IFRAMELUT) \
    PREFIX(DMS) \
    PREFIX(MSB124X) \
    PREFIX(PM) \
    PREFIX(SWI2C) \
    PREFIX(AKL) \
    PREFIX(SC) \
    PREFIX(PKA) \
    PREFIX(MFD) \
    PREFIX(VENC) \

#define INCLUDED_MODULE \
    PREFIX(UTOPIA) \
    _INCLUDED_MODULE

typedef enum{
#define PREFIX(MODULE) MODULE_##MODULE,
    INCLUDED_MODULE
#undef PREFIX
    eMODULE_END,
}eMsModule;

#define PREFIX(MODULE) extern MS_U32 MODULE_##MODULE##_OPEN;
INCLUDED_MODULE
#undef PREFIX

#endif
