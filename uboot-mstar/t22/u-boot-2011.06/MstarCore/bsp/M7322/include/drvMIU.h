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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_MIU MIU interface
    \ingroup  G_PERIPHERAL

    \brief 
    MIU is an IP used to manage DRAM, and it supports DRAM Protection, IP Selection, and IP Mask.    
    
    <b>Features</b>

    - MIU Protect: Enable/Disable IPs to read/write a specific memory range.
    - IP Select: To change IPs used in MIU0/MIU1
    - IP Mask: To disable IPs using DRAM.    

    <b> MIU Block Diagram: </b> \n
    \image html drvMIU_pic1.png
    
     \defgroup G_MIU_INIT Initialization Task relative
     \ingroup  G_MIU
     \defgroup G_MIU_COMMON Common Task relative
     \ingroup  G_MIU
     \defgroup G_MIU_CONTROL Control relative
     \ingroup  G_MIU
     \defgroup G_MIU_PROTECT Protect Task relative
     \ingroup  G_MIU
     \defgroup G_MIU_ToBeModified MIU api to be modified
     \ingroup  G_MIU
     \defgroup G_MIU_ToBeRemove MIU api to be removed
     \ingroup  G_MIU
*/

#ifndef _DRV_MIU_H_
#define _DRV_MIU_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
#include "UFO.h"

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#ifdef STELLAR
#define _MIU_INTERNEL_USE 1
#endif

#define MIU_MAX_BW_MONITOR_GP_CLIENT_NUMBER_ALL (8)
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MIU_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'M','I','U','_'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','2'},                          /* 00 ~ 99                                          */  \
    {'0','0','2','6','4','8','8','5'},  /* CL#                                              */  \
    MSIF_OS

#define MIU_CLIENT_MAX  0x30


#define bit_check(x,bit_pos)  ((x & bit_pos) != 0) ? TRUE : FALSE


#define set_req_mask_info(reg_addr,base_addr,bEnable,id_pos)  \
    MST_MACRO_START                                                     \
    reg_addr = base_addr;                                               \
    reg_addr = reg_addr + (MS_U16)((id_pos >> 4) * 0x20);                  \
    if(reg_addr == 0x1286)                                              \
        reg_addr = 0x06F6;                                              \
    else if(reg_addr == 0x0686)                                         \
        reg_addr = 0x0628;                                              \
    reg_addr = reg_addr + (MS_U16)(bit_check(id_pos,BIT3));                \
    HAL_MIU_WriteRegBit(reg_addr, bEnable , (1 << (id_pos & (BIT2|BIT1|BIT0)))); \
    MST_MACRO_END

#ifdef ALIGN
#undef ALIGN
#define ALIGN(_val_,_shift_) (((_val_) >> _shift_) << _shift_)
#else
#define ALIGN(_val_,_shift_) (((_val_) >> _shift_) << _shift_)
#endif

//#define _MIU_INTERNEL_USE         //for internel test only
#define _ALIGN(_val_,_shift_) (((_val_) >> _shift_) << _shift_)

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef enum
{
    CLIENT_TSPIWB,          // orz
    CLIENT_STRLD,
    CLIENT_HK51_ICACHE,    // MAU0
    CLIENT_AEON_I,          // MAU0
    CLIENT_FLH_W,           // MAU1
    CLIENT_HK51_XDATA,     // MAU1
    CLIENT_AEON,            // MAU1
    CLIENT_OD_LSB,
    CLIENT_GOP2,
    CLIENT_VE,
    CLIENT_FDSEICH,
    CLIENT_TSP,
    CLIENT_TTXSK,
    CLIENT_SVD_DB,
    CLIENT_FDDECICH,
    CLIENT_OPM,
    CLIENT_DC,
    CLIENT_GOP,
    CLIENT_GOP0,
    CLIENT_GOP1,
    CLIENT_DNRB,    // DNR
    CLIENT_COMB,
    CLIENT_OD,
    CLIENT_MADDMA2,   // DMA2
    CLIENT_MAD,
    CLIENT_SVD_EN,
    CLIENT_GE,
    CLIENT_BDMA,   // MIU_DMA
    CLIENT_NFC,
    CLIENT_USBP1,  // USB20 ??
    CLIENT_USBP2,  //USB20_1 ??
    CLIENT_RVD,
    CLIENT_SVDINTP,
    CLIENT_MVD,
    CLIENT_AESDMA,
    CLIENT_TTX,
    CLIENT_JPD,
    CLIENT_EMAC,
    CLIENT_MAX
} euMIUClientIDTYPE;

typedef struct
{
    MS_U8 miu;
    MS_U8 protect_id;
} stMIUClientID_t;

typedef enum
{
    MIU_CLIENT_NONE,    //none can access
    MIU_CLIENT_DUMMY,
    MIU_CLIENT_ADCDVIPLL_W,
    MIU_CLIENT_AESDMA_RW,
    MIU_CLIENT_AU_R2_RW,
    MIU_CLIENT_BDMA_RW,
    MIU_CLIENT_DC_R,                //same as MIU_CLIENT_MVOP_64BIT_R
    MIU_CLIENT_DISP_IPATH_DI_W,
    MIU_CLIENT_DISP_IPATH_MR_RW,
    MIU_CLIENT_DISP_IPATH_NR_RW,
    MIU_CLIENT_DMA2_RW,
    MIU_CLIENT_DNRA_RW,
    MIU_CLIENT_DSCRMB_RW,
    MIU_CLIENT_DVBC_ADC_RW,
    MIU_CLIENT_EMAC_RW,
    MIU_CLIENT_FCIE_RW,
    MIU_CLIENT_FDDECICH_R,
    MIU_CLIENT_FDSEICH_R,
    MIU_CLIENT_G3D_RW,
    MIU_CLIENT_GE_RW,
    MIU_CLIENT_GOP_W,
    MIU_CLIENT_GOP0_R,
    MIU_CLIENT_GOP1_R,
    MIU_CLIENT_GOP2_R,
    MIU_CLIENT_GOP3_R,
    MIU_CLIENT_HISPEED_UART_RW,
    MIU_CLIENT_HVD_RW,
    MIU_CLIENT_HVD_BBU_R,
    MIU_CLIENT_JPD_RW,
    MIU_CLIENT_M4VE_ME_R,
    MIU_CLIENT_M4VE0_RW,
    MIU_CLIENT_M4VE2_RW,
    MIU_CLIENT_MAU_RW,
    MIU_CLIENT_MAU0_W,
    MIU_CLIENT_MAU1_R,
    MIU_CLIENT_MFE0_W,
    MIU_CLIENT_MFE1_R,
    MIU_CLIENT_MHEG5_DCACHE_RW,
    MIU_CLIENT_MHEG5_ICACHE_R,
    MIU_CLIENT_MHEG5_ICACHE_RW,
    MIU_CLIENT_MHEG5_GDMA_RW,
    MIU_CLIENT_MIPS_R,
    MIU_CLIENT_MIPS_W,
    MIU_CLIENT_MIPS_RW,
    MIU_CLIENT_MOBF_RW,
    MIU_CLIENT_MPIF_RW,
    MIU_CLIENT_MVD_RW,
    MIU_CLIENT_MVD_BBU_RW,
    MIU_CLIENT_MVOP_64BIT_R,
    MIU_CLIENT_MVOP_128BIT_R,
    MIU_CLIENT_NAND_RW,
    MIU_CLIENT_OD_R,
    MIU_CLIENT_OD_W,
    MIU_CLIENT_OD_LSB_W,
    MIU_CLIENT_OD_LSB_R,
    MIU_CLIENT_OPW_W,
    MIU_CLIENT_OTG_RW,
    MIU_CLIENT_PM51_RW,
    MIU_CLIENT_PVR_W,
    MIU_CLIENT_PVR2_W,
    MIU_CLIENT_R2M_R,
    MIU_CLIENT_R2M_W,
    MIU_CLIENT_RASP0_W,
    MIU_CLIENT_RASP1_W,
    MIU_CLIENT_RVD_BBU_R,
    MIU_CLIENT_RVD_RW,
    MIU_CLIENT_SC_DNR_R,
    MIU_CLIENT_SC_DNR_W,
    MIU_CLIENT_SC_IPMAIN_R,
    MIU_CLIENT_SC_IPMAIN_W,
    MIU_CLIENT_SC_IPSUB_R,
    MIU_CLIENT_SC_IPSUB_W,
    MIU_CLIENT_SC_OP_R,
    MIU_CLIENT_SC_OPM_R,
    MIU_CLIENT_SC_TNR_R,
    MIU_CLIENT_SC_TNR_W,
    MIU_CLIENT_STRLD_RW,
    MIU_CLIENT_TSP_R,
    MIU_CLIENT_TSP_W,
    MIU_CLIENT_TSP_ORZ_R,
    MIU_CLIENT_TSP_ORZ_W,
    MIU_CLIENT_USB20_RW,
    MIU_CLIENT_USB_UHC0_RW,
    MIU_CLIENT_USB_UHC1_RW,
    MIU_CLIENT_USB_UHC2_RW,
    MIU_CLIENT_VD_COMB_R,
    MIU_CLIENT_VD_COMB_W,
    MIU_CLIENT_VD_TTX_RW,
    MIU_CLIENT_VD_TTXSL_W,
    MIU_CLIENT_VD_TTXSK_W,
    MIU_CLIENT_VE_W,
    MIU_CLIENT_VE_R,
    MIU_CLIENT_VIF_ADC_W,
    MIU_CLIENT_VIVALDI9_AUDMA_RW,
    MIU_CLIENT_VIVALDI9_DECODER_R,
    MIU_CLIENT_VIVALDI9_DMA_RW,
    MIU_CLIENT_VIVALDI9_LNKLST_R,
    MIU_CLIENT_VIVALDI9_MAD_RW,
    MIU_CLIENT_VIVALDI9_SE_R,
    MIU_CLIENT_MSP_ICACHE_RW,
    MIU_CLIENT_DISP_IPATH_DI_RW,
    MIU_CLIENT_MVOP1_R,
    MIU_CLIENT_LDM_W,
    MIU_CLIENT_LDM_R,
    MIU_CLIENT_T3D_W,
    MIU_CLIENT_T3D_R,
    MIU_CLIENT_MIIC0_RW,
    MIU_CLIENT_MIIC1_RW,
    MIU_CLIENT_MIIC2_W,
    MIU_CLIENT_MAXID,
    MIU_CLIENT_SC_IPMAIN_RW,
    MIU_CLIENT_SC_IPSUB_RW,
    MIU_CLIENT_SC_OPMAIN_RW,
    MIU_CLIENT_FRC_OSD_RW,
    MIU_CLIENT_FRC_IP_R,
    MIU_CLIENT_FRC_IP_W,
    MIU_CLIENT_FRC_OD_R,
    MIU_CLIENT_FRC_OD_W,
    MIU_CLIENT_FRC_OPM_R,
    MIU_CLIENT_FRC_R2_RW,
    MIU_CLIENT_FRC_SC_RW,
    MIU_CLIENT_SC_OP_W,
    MIU_CLIENT_SECURE_R2_RW,
    MIU_CLIENT_SC_2D3D_RW,
    MIU_CLIENT_SC_OD_W,
    MIU_CLIENT_SC_OD_R,
    MIU_CLIENT_SC_LD_RW,
    MIU_CLIENT_GPD_RW,
    MIU_CLIENT_VP6_RW,
    MIU_CLIENT_SDIO_RW,
    MIU_CLIENT_G3D0_RW,
    MIU_CLIENT_G3D1_RW,
    MIU_CLIENT_SECEMAC_RW,
    MIU_CLIENT_USB_UHC3_RW,
    MIU_CLIENT_TSP_PVR0_W,
    MIU_CLIENT_TSP_PVR1_W,
    MIU_CLIENT_MAU0_RW,
    MIU_CLIENT_MAU1_RW,
    MIU_CLIENT_TSP_SEC_W,
    MIU_CLIENT_OPM_R,
    MIU_CLIENT_USB3_RW,
    MIU_CLIENT_SC_DIPW_RW,
    MIU_CLIENT_CMD_QUEUE_RW,
    MIU_CLIENT_TSO_RW,
    MIU_CLIENT_VE_2DMCDI_RW,
    MIU_CLIENT_SC_IPSUB2_R,
    MIU_CLIENT_SC_IPSUB2_W,
    MIU_CLIENT_MIIC_DMA_RW,
    MIU_CLIENT_UART_DMA_RW,
    MIU_CLIENT_NJPD_RW,
    MIU_CLIENT_XD2MIU_RW,
    MIU_CLIENT_VD_R2D_RW,
    MIU_CLIENT_VD_R2I_R,
    MIU_CLIENT_TSP_ORZ_RW,
    MIU_CLIENT_MVOP_SUB_R,
    MIU_CLIENT_SC_DIPW_W,
    MIU_CLIENT_T3D_RW,
    MIU_CLIENT_BT_RW,
    MIU_CLIENT_VE_VBI_R,
    MIU_CLIENT_ARM_RW,
    MIU_CLIENT_SC1_OP_R,
    MIU_CLIENT_SC1_IPMAIN_RW,
    MIU_CLIENT_GOP4_R,
    MIU_CLIENT_GOP5_R,
    MIU_CLIENT_GMAC_RW,
    MIU_CLIENT_SATA_RW,
    MIU_CLIENT_SC_LOCALDIMING_RW,
    MIU_CLIENT_JPD720P_RW,
    MIU_CLIENT_SC_IPM2_R,
    MIU_CLIENT_VIVALDI_DSC_R,
    MIU_CLIENT_TSP_JPD_RW,
    MIU_CLIENT_DEMOD_W,
    MIU_CLIENT_DEMOD_R,
    MIU_CLIENT_DEMOD_ADCDMA_W,
    MIU_CLIENT_GPU_RW,
    MIU_CLIENT_PDW1_RW,
    MIU_CLIENT_GPO0_PDW0_RW,
    MIU_CLIENT_EVD_R,
    MIU_CLIENT_EVD_RW,
    MIU_CLIENT_SC_FRCL_R,
    MIU_CLIENT_SC_FRCR_R,
    MIU_CLIENT_VD_VBI_RW, // do not use this client; use MIU_CLIENT_VD_TTXSL_W instead
    MIU_CLIENT_VD_MHEG5_ICACHE_RW,
    MIU_CLIENT_TSP00_RW,
    MIU_CLIENT_TSP01_RW,
    MIU_CLIENT_TSP02_RW,
    MIU_CLIENT_TSP03_RW,
    MIU_CLIENT_TSP04_RW,
    MIU_CLIENT_TSP05_RW,
    MIU_CLIENT_TSP06_RW,
    MIU_CLIENT_VIVALDI9_COMBINE_RW,
    MIU_CLIENT_TSP07_RW,
    MIU_CLIENT_ISDBT_TDI_R,
    MIU_CLIENT_ISDBT_TDI_W,
    MIU_CLIENT_FI_Queue0_WR,
    MIU_CLIENT_FI_Queue1_WR,
    MIU_CLIENT_SWDC_RW,
    MIU_CLIENT_ISDB1_RW,
    MIU_CLIENT_ISDB2_RW,
    MIU_CLIENT_MIIC3_RW,
    MIU_CLIENT_SECAU_R2_RW,
    MIU_CLIENT_SC_LOCALDIMING_R_RW,
    MIU_CLIENT_SC_LOCALDIMING_L_RW,
    MIU_CLIENT_SC0_L_RW,
    MIU_CLIENT_SC0_R_RW,
    MIU_CLIENT_TSP_FIQ_RW,
    MIU_CLIENT_EVD_R2D_RW,
    MIU_CLIENT_EVD_R2I_R,
    MIU_CLIENT_SECHVD_RW,
    MIU_CLIENT_SECEVD_RW,
    MIU_CLIENT_MFDEC_R,
    MIU_CLIENT_SECMFDEC_R,
    MIU_CLIENT_MIUTEST_R,
    MIU_CLIENT_GOP3_PDW0_RW,
    MIU_CLIENT_SC1_OPMAIN_RW, 
    MIU_CLIENT_SC2_IPSUB_RW, 
    MIU_CLIENT_SC_IPMAIN2_RW, 
    MIU_CLIENT_SC2_OPMAIN_RW, 
    MIU_CLIENT_SC_ODL_RW,
    MIU_CLIENT_SC_ODR_RW,
    MIU_CLIENT_SC1_IPSUB_RW, 
    MIU_CLIENT_EVD_BBU_R,
    MIU_CLIENT_SC_DWIN_W,
    MIU_CLIENT_ZDEC_RW,
    MIU_CLIENT_ZDEC_ACP_RW,
    MIU_CLIENT_USB30_1_RW,
    MIU_CLIENT_USB30_2_RW,
    MIU_CLIENT_3RDHVD_RW,
    MIU_CLIENT_VP9_RW,
    MIU_CLIENT_FRC_R,
    MIU_CLIENT_FRCM_W,
    MIU_CLIENT_SC_OD_RW,
    MIU_CLIENT_SC_OPSUB_W, 
    MIU_CLIENT_FRCS_W,
    MIU_CLIENT_EVD_MTFC_W,
    MIU_CLIENT_EVD_MTFY_W,
    MIU_CLIENT_ZDEC2_RW,
    MIU_CLIENT_SC2_IPMAIN_RW, 
    MIU_CLIENT_MTF_W,
    MIU_CLIENT_DBG_R,
    MIU_CLIENT_DS_R,
    MIU_CLIENT_FRC_R2,
    MIU_CLIENT_FRC_R2I_R = MIU_CLIENT_FRC_R2,
    MIU_CLIENT_MVD_RTO_RW,
    MIU_CLIENT_FRC_FSCM2_RW,
    MIU_CLIENT_FRC_FSCM3_RW,
    MIU_CLIENT_FRC_IPM0_W,
    MIU_CLIENT_FRC_IPM1_W,
    MIU_CLIENT_FRC_OPM0_R,
    MIU_CLIENT_FRC_OPM1_R,
    MIU_CLIENT_FRC_OPME0_R,
    MIU_CLIENT_FRC_OPME1_R,
    MIU_CLIENT_FRC_OPMI0_R,
    MIU_CLIENT_FRC_OPMI1_R,
    MIU_CLIENT_FRC_ME_W,
    MIU_CLIENT_FRC_ME_R,
    MIU_CLIENT_FRC_HR_W,
    MIU_CLIENT_FRC_HR_R,
    MIU_CLIENT_FRC_MI_MERGE_RW,
    MIU_CLIENT_MC2D_RW,
    MIU_CLIENT_CMD_QUEUE1_RW,
    MIU_CLIENT_USB_UHC4_RW,
    MIU_CLIENT_DEMOD_RW,
    MIU_CLIENT_VE_RW,
    MIU_CLIENT_SC_PDW_W,
    MIU_CLIENT_VIVALDI9_R2_ARB_RW,
    MIU_CLIENT_MCU51_DB_TOOL_RW,
    MIU_CLIENT_TSP_RW,
    MIU_CLIENT_TSP_ORZ2_RW,
    MIU_CLIENT_EVD_BBU_RW,
    MIU_CLIENT_DVBC_ADC_W,
    MIU_CLIENT_GMAC1_RW,
    MIU_CLIENT_MFE_RW,
    MIU_CLIENT_VD_R2_L_I_R,
    MIU_CLIENT_VD_R2_L_D_RW,
    MIU_CLIENT_CA_MIU_CROSSBAR_2_RW,
    MIU_CLIENT_TSP08_RW,
    MIU_CLIENT_ZDEC_LZDMA_RW,
    MIU_CLIENT_EVD2_BBU_R,
    MIU_CLIENT_GOP3_DWIN_RW,
    MIU_CLIENT_MVOP_256BIT_R,
    MIU_CLIENT_MFDEC1_R,
    MIU_CLIENT_SC_DYN_SCL_R,
    MIU_CLIENT_SC1_OPM_R,
    MIU_CLIENT_ZDEC_ACP_W,
    MIU_CLIENT_CMD_QUEUE_R,
    MIU_CLIENT_VIVALDI9_DECODER_RW,
    MIU_CLIENT_DEMOD_ADCDMA_RW,
    MIU_CLIENT_MIU_BIST,
    MIU_CLIENT_CA_MIU_CROSSBAR_0_RW,
    MIU_CLIENT_CA_MIU_CROSSBAR_1_RW,
    MIU_CLIENT_SECGMAC_RW,
    MIU_CLIENT_AU_R2_1_RW,
    MIU_CLIENT_TSO_1_RW,
    MIU_CLIENT_TSIO_RW,
    MIU_CLIENT_PCIE_OUTBOUND_RW,
    MIU_CLIENT_PCIE_INBOUND_RW,
    MIU_CLIENT_DDI_0_RW,
    MIU_CLIENT_SC_DIPW_1_RW,
    MIU_CLIENT_EVD_ENGINE1_RW,
    MIU_CLIENT_HVD_ENGINE1_RW,
    MIU_CLIENT_DDI_1_RW,
    MIU_CLIENT_MFDEC0_1_R,
    MIU_CLIENT_MFDEC1_1_R,
    MIU_CLIENT_AUTO_DOWNLOAD_R,
    MIU_CLIENT_MFEH_R,
    MIU_CLIENT_AUDIO_RW,
    MIU_CLIENT_OD_RW,
    MIU_CLIENT_MVOP1_256BIT_R,
    MIU_CLIENT_MVD_256BIT_RW,
    MIU_CLIENT_TSP_FILEIN_RW,
    MIU_CLIENT_TSP_SEC_RW,
    MIU_CLIENT_HDR_L_RW,
    MIU_CLIENT_HDR_R_RW,
    MIU_CLIENT_MIU_CMD_RW,
    MIU_CLIENT_G256_POST_ARB_RW,
    MIU_CLIENT_G128_POST_ARB_RW,
    MIU_CLIENT_G3_PRE_ARB_RW,
    MIU_CLIENT_EVD_2_MIU0_RW,
    MIU_CLIENT_EVD_2_MIU1_RW,
    MIU_CLIENT_DEMOD_MCU51_WR,
    MIU_CLIENT_DEMOD1_WR,
    MIU_CLIENT_DEMOD2_WR,
    MIU_CLIENT_DEMOD3_WR,
    MIU_CLIENT_DEMOD4_WR,
    MIU_CLIENT_DEMOD5_WR,
    MIU_CLIENT_DEMOD6_WR,
    MIU_CLIENT_TSO_TX_RW,
    MIU_CLIENT_TSO_RX_RW,
    MIU_CLIENT_SC_DIP_RW,
    MIU_CLIENT_SC_DIP_DI_RW,
    MIU_CLIENT_ADL_RW,
    MIU_CLIENT_DSP_CACHE_RW,
    MIU_CLIENT_R2_I_RW,
    MIU_CLIENT_R2_D_RW,
    MIU_CLIENT_R2_BDMA_RW,
    MIU_CLIENT_R21_I_R,
    MIU_CLIENT_R21_D_RW,
    MIU_CLIENT_R21_BDMA_RW,
    MIU_CLIENT_DMAL2_RW,
    MIU_CLIENT_MADDMA_RW,
    MIU_CLIENT_SEDMA_RW,
    MIU_CLIENT_DMAL2_W,
    MIU_CLIENT_USB30_MERGE_RW,
    MIU_CLIENT_MVD_R,
    MIU_CLIENT_MIIC0_W,
    MIU_CLIENT_PCIE_RW,
    MIU_CLIENT_VD_RW,
    MIU_CLIENT_AESDMA2_RW,
    MIU_CLIENT_FRC_FRCM2_R,
    MIU_CLIENT_FRC_FRCM3_R,
    MIU_CLIENT_FRC_MI_R,
    MIU_CLIENT_FRC_HVDM0_W,
    MIU_CLIENT_FRC_HVDM1_W,
    MIU_CLIENT_MFDEC2_R,
    MIU_CLIENT_MFDEC3_R,
    MIU_CLIENT_MVOP1_0_R,
    MIU_CLIENT_MVOP1_1_R,
    MIU_CLIENT_SC_MGWIN1,
    MIU_CLIENT_SC_MCDI_R,
    MIU_CLIENT_SC_IPM_R2_R,
    MIU_CLIENT_SC2_OPM_R,
    MIU_CLIENT_SC2_IPM_R,
    MIU_CLIENT_SC_AFBC0_R,
    MIU_CLIENT_SC_DS_R,
    MIU_CLIENT_SC2_DS_R,
    MIU_CLIENT_MVOP0_0_R,
    MIU_CLIENT_MVOP0_1_R,
    MIU_CLIENT_SC_DIPR_0_R,
    MIU_CLIENT_SC_DIPR_1_R,
    MIU_CLIENT_SC_DOLBY_R,
    MIU_CLIENT_SC_CMDQ_R,
    MIU_CLIENT_SC_MGWIN0_R,
    MIU_CLIENT_SC_AFBC1_R,
    MIU_CLIENT_SC_OPMAIN_W,
    MIU_CLIENT_MDWIN_W,
    MIU_CLIENT_SC_DIP_0_W,
    MIU_CLIENT_SC_DIP_1_W,
    MIU_CLIENT_SC_PDW0_W,
    MIU_CLIENT_SC_PDW1_W,
    MIU_CLIENT_SC_MCDI_W,
    MIU_CLIENT_SC_DOLBY_W,
    MIU_CLIENT_SC2_IPM_W,
    MIU_CLIENT_SC_ABC_DMA_W,
    MIU_CLIENT_SC_ADC_DMA_W = MIU_CLIENT_SC_ABC_DMA_W,
    MIU_CLIENT_EVD_0_RW,
    MIU_CLIENT_EVD_1_RW,
    MIU_CLIENT_EVD_2_RW,
    MIU_CLIENT_EVD_3_RW,
    MIU_CLIENT_HVD_0_RW,
    MIU_CLIENT_HVD_1_RW,
    MIU_CLIENT_HVD_2_RW,
    MIU_CLIENT_MIU_TEST_RW,
    MIU_CLIENT_FIQ_RW,  //MIU_CLIENT_TSP_FIQ_RW
    MIU_CLIENT_ADL_R,
    MIU_CLIENT_TSO_2_RW,
    MIU_CLIENT_TSO_3_RW,
    MIU_CLIENT_VD_R2DMA_RW,
    MIU_CLIENT_MVD1_BBU_RW,
    MIU_CLIENT_MVD1_RW,
    MIU_CLIENT_PCIE1_OUTBOUND_RW,
    MIU_CLIENT_PCIE1_INBOUND_RW,
    MIU_CLIENT_EVD_LITE_RW,
    MIU_CLIENT_EVD_LITE_ENGINE1_RW,
    MIU_CLIENT_HVD_LITE_BBU_R,
    MIU_CLIENT_MVD1_RTO_RW,
    MIU_CLIENT_TSP09_RW,
    MIU_CLIENT_USB30_HS_RW,
    MIU_CLIENT_USB30M1_RW,
    MIU_CLIENT_USB30M1_HS_RW,
    MIU_CLIENT_HDR_OPMAIN_RW,
    MIU_CLIENT_HDR_IPMAIN_RW,
    MIU_CLIENT_AU_R2_2_RW,
    MIU_CLIENT_SCPU_MIU_RW,
    MIU_CLIENT_PCIE_1_RW,
    MIU_CLIENT_NOE_RW,
    MIU_CLIENT_WED0_RW,
    MIU_CLIENT_WED1_RW,
    MIU_CLIENT_MVD_SUBSYSY_RW,
    MIU_CLIENT_GE_W_RW,
    MIU_CLIENT_DDI_2_RW,
    MIU_CLIENT_IPM_RW,
    MIU_CLIENT_USB30_SS_RW,
    MIU_CLIENT_PE2_RC_TOP,
    MIU_CLIENT_MHE_R,
    MIU_CLIENT_MHE_W,
    MIU_CLIENT_VIVALDI9_AUBDMA_RW,
    MIU_CLIENT_GE_CMDQ_RW,
    MIU_CLIENT_FRC_R2D_R,
    MIU_CLIENT_FRC_LOGO_R,
    MIU_CLIENT_FRC_LOGO_W,
    MIU_CLIENT_FRC_HVDM0_R,
    MIU_CLIENT_FRC_MV_R,
    MIU_CLIENT_FRC_MV_W,
    MIU_CLIENT_FRC_MCMV_R,
    MIU_CLIENT_EVD1_R,
    MIU_CLIENT_EVD2_R,
    MIU_CLIENT_HVD1_R,
    MIU_CLIENT_SC_OPMAIN_R,
    MIU_CLIENT_SC_LD_R,
    MIU_CLIENT_SC_LD_W,
    MIU_CLIENT_SC_LD1_W,
    MIU_CLIENT_SC_HDR_DMA_R,
    MIU_CLIENT_SC_HDR_DMA_W,
    MIU_CLIENT_CPU_R,
    MIU_CLIENT_CPU_W,
    MIU_CLIENT_GPU_R,
    MIU_CLIENT_GPU_W,
    MIU_CLIENT_TSP_UFSHCI_RW,
    MIU_CLIENT_VREC_RW,
    MIU_CLIENT_PASDMA_W,
    MIU_CLIENT_TSP_ALP_W,
    MIU_CLIENT_TSO_R,
    MIU_CLIENT_SC_ODW_W,
    MIU_CLIENT_SC_AUL_W,
    MIU_CLIENT_MPU_MMU_RW,
    MIU_CLIENT_DDI_3_RW,
    MIU_CLIENT_AUTO_UPLOAD_W,
    MIU_CLIENT_AUTO_DOWNLOAD_RW,
    MIU_CLIENT_SC_DIP_2_W,
    MIU_CLIENT_SC_DIP_3_RW,
    MIU_CLIENT_SC_ABF_W0_RW,
    MIU_CLIENT_SC_ABF_W1_RW,
    MIU_CLIENT_SC2_OP_R,
    MIU_CLIENT_SC_DS_RW,
    MIU_CLIENT_SC_DIPR_R,
    MIU_CLIENT_MONITOR_GROUP0,
    MIU_CLIENT_MONITOR_GROUP1,
    MIU_CLIENT_MONITOR_GROUP2,
    MIU_CLIENT_MONITOR_GROUP3,
    MIU_CLIENT_MONITOR_GROUP4,
    MIU_CLIENT_MONITOR_GROUP5,
    MIU_CLIENT_MONITOR_GROUP6,
    MIU_CLIENT_MONITOR_GROUP7,
    MIU_CLIENT_MONITOR_GROUP8,
    MIU_CLIENT_MHEG_5_RW,
    MIU_CLIENT_R2_BBU_RW,
    MIU_CLIENT_MFDEC0_0_R,
    MIU_CLIENT_MFDEC1_0_R,
    MIU_CLIENT_SC_ODW_R,
    //Add new after here
}eMIUClientID;

typedef enum
{
    //IP Access MIU0 only
    MIU_SELTYPE_MIU0 = 0,
    //IP Access MIU1 only
    MIU_SELTYPE_MIU1,
    //IP can access MIU0 & MIU1
    MIU_SELTYPE_MIU_ALL,
    //IP Access MIU2 only
    MIU_SELTYPE_MIU2,
    //IP Access MIU3 only
    MIU_SELTYPE_MIU3

}eMIU_SelType;

typedef enum
{
    //MIU Select
    MIU_AttrType_Select = 0,
    //MIU Group Priority
    MIU_AttrType_GroupPriority,
    //MIU Dram Size
    MIU_AttrType_DramSize,
    MIU_AttrType_MAX,
}eMIU_ATTR_TYPE;

typedef struct
{
    MS_U8 u8Gp:4;
    MS_U8 u8BitPos:4;
}MIU_ClientInfo;

typedef struct DLL_PACKED
{
    MS_BOOL bHit;
    MS_U8   u8Group;
    MS_U8   u8ClientID;
    MS_U8   u8Block;
}MIU_PortectInfo;

typedef struct
{
    MS_U8   u81st;
    MS_U8   u82nd;
    MS_U8   u83rd;
    MS_U8   u84th;
}MIU_GroupPriority;

typedef enum
{
  E_MIU_0 = 0,
  E_MIU_1,
  E_MIU_2,
  E_MIU_3,
  E_MIU_NUM,
} MIU_ID;

typedef enum
{
  E_MIU_DDR_32MB = 0,
  E_MIU_DDR_64MB,
  E_MIU_DDR_128MB,
  E_MIU_DDR_256MB,
  E_MIU_DDR_512MB,
  E_MIU_DDR_1024MB,
  E_MIU_DDR_2048MB,
  E_MIU_DDR_4096MB,
  E_MIU_DDR_8192MB,
}MIU_DDR_SIZE;

typedef enum
{
  E_MIU_DDR3,
  E_MIU_DDR4
}MIU_DDR_TYPE;

typedef enum
{
  E_MIU_CLIENT_64BIT = 0,
  E_MIU_CLIENT_128BIT,
  E_MIU_CLIENT_256BIT,
}eMIU_ClientWidth;

typedef enum
{
    E_MONITOR_FREQUENCY_1,              //Highest Frequency (First Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_2,              //(Second Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_3,              //(Second Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_4,              //(Second Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_5,              //(Third Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_6,              //(Third Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_7,              //(Third Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_8,              //(Fourth Section for Morton, Merlin)
    E_MONITOR_FREQUENCY_9,              //(Fourth Section for Morton, Merlin)
}MIU_BW_TIME_FREQUENCY;

typedef enum
{
    E_MONITOR_REPORT_IMMED_EFFI = 0,    //Immediate Efficiency
    E_MONITOR_REPORT_MIN_EFFI,          //Min Efficieny
    E_MONITOR_REPORT_IMMED_UTIL,        //Immediate utilization
    E_MONITOR_REPORT_MAX_UTIL,          //Max utilization
    E_MONITOR_REPORT_IMMED_OCCU,        //Immediate Occupancy
    E_MONITOR_REPORT_MAX_OCCU,          //Max Occupancy
}MIU_BW_INFO_TYPE;

typedef enum
{
    E_MONITOR_INDEX_0 = 0,
    E_MONITOR_INDEX_1,
    E_MONITOR_INDEX_2,
    E_MONITOR_INDEX_3,
    E_MONITOR_INDEX_4,
    E_MONITOR_INDEX_5,
    E_MONITOR_INDEX_6,
    E_MONITOR_INDEX_7,
    E_MONITOR_INIT = 0xFE,
    E_MONITOR_NONE = 0xFF,
}MIU_BW_MONITOR_INDEX;

typedef struct
{
    MIU_BW_TIME_FREQUENCY eSamplingSection;         //Select frequency number
    MS_U32 u32UpdateFrequencyValue;                 //Specified the frequency number
}MIU_BandwidthSamplingFrequency;

typedef struct
{
    MS_U32 u32ReportImmediateEfficiency;
    MS_U32 u32ReportMinEfficiency;
    MS_U32 u32ReportImmediateUtilization;
    MS_U32 u32ReportMaxUtilization;
    MS_U32 u32ReportImmediateOccupancy;
    MS_U32 u32ReportMaxOccupancy;
}MIU_BandwidthReportValue;

typedef struct DLL_PACKED
{
    MS_BOOL                         bMonitorAllMIU;                                             //Monitor all MIU Bandwidth or not
    MS_U32                          u32CurrentTimeCount;                                        //Bandwidth value update current time count
    MS_U32                          u32MonitorIndex;                                            //Bandwidth monitor index
    MIU_BW_INFO_TYPE                eReportType;                                                //Bandwidth value report type
    MIU_BandwidthSamplingFrequency  stSamplingFrequency;                                        //Bandwidth value Sampling frequency
    MIU_BandwidthReportValue        stCurrentBandwidth;                                         //Currently get value of Bandwidth (per mill)
    MS_U32                          u32MonitorId[MIU_MAX_BW_MONITOR_GP_CLIENT_NUMBER_ALL];      //Monitor Client ID
}MIU_BandwidthMonitorInfo;

#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)


#define set_client_info(client_data,miu_pos,id_code)         \
        MST_MACRO_START                                      \
        client_data->miu = miu_pos;                          \
        client_data->protect_id = id_code;                   \
        MST_MACRO_END


//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvMIU_v2.h"

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_InitCounter(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_VOP_SwitchMIU(MS_U8 u8MiuID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_PROTECT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_PrintMIUProtectInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_PROTECT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8*  MDrv_MIU_GetDefaultClientID_KernelProtect(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16  MDrv_MIU_GetBusWidth(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_SetSsc(MS_U16 u16Fmodulation, MS_U16 u16FDeviation,MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_SetSscValue(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_GetClientID(euMIUClientIDTYPE client_name, stMIUClientID_t *client_read, stMIUClientID_t *client_write);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Mask(euMIUClientIDTYPE client_name);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Unmask(euMIUClientIDTYPE client_name);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_MaskByPort(euMIUClientIDTYPE client_name,stMIUClientID_t *ByPort);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_UnmaskByPort(euMIUClientIDTYPE client_name,stMIUClientID_t *ByPort);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_SetIOMapBase(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_PROTECT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_ProtectAlign(MS_U32 *u32PageShift);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Dram_ReadSize(MS_U8 MiuID, MIU_DDR_SIZE *pDramSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_MIU_ClinetNumber(MS_U8 u8Blockx);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_PROTECT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Protect( MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY phyStart, MS_PHY phyEnd, MS_BOOL bSetFlag );

MS_BOOL MDrv_MIU_ProtectEx( MS_U8 u8Blockx, MS_U32 *pu32ProtectId, MS_PHY phy64Start, MS_PHY phy64End, MS_BOOL bSetFlag );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_IsSupportMIU1(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_SelMIU(eMIUClientID eClientID, eMIU_SelType eType);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID, MIU_ClientInfo *pInfo);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_PROTECT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_EnableScramble(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_IsScrambleEnabled(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_GetLoading(MS_U8 u8MiuDev, MS_U32 *Loading);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_MIU_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_GetClientWidth(MS_U8 u8MiuDevi, eMIUClientID eClientID, eMIU_ClientWidth *pClientWidth);

MS_BOOL MDrv_MIU_GetDramType(MIU_ID eMiu, MIU_DDR_TYPE* pType);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MIU_BW_Monitor
/// @param u8MiuDev \b IN: miu device id
/// @param pBWInfo \b IN: MIU_BandwidthMonitorInfo
/// @return true : Success
/// @return false : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Set_BW_Monitor(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
//-------------------------------------------------------------------------------------------------
/// @ingroup G_MIU_BW_Monitor
/// @param u8MiuDev \b IN: miu device id
/// @param pBWInfo \b IN: MIU_BandwidthMonitorInfo
/// @return true : Success
/// @return false : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Del_BW_Monitor(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
//-------------------------------------------------------------------------------------------------
/// @ingroup G_MIU_BW_Monitor
/// @param u8MiuDev \b IN: miu device id
/// @param pBWInfo \b IN: MIU_BandwidthMonitorInfo
/// @return true : Success
/// @return false : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Query_BW_Monitor(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
//-------------------------------------------------------------------------------------------------
/// @ingroup G_MIU_BW_Monitor
/// @param u8MiuDev \b IN: miu device id
/// @param pBWInfo \b IN: MIU_BandwidthMonitorInfo
/// @return true : Success
/// @return false : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Get_BW(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
//-------------------------------------------------------------------------------------------------
/// @ingroup G_MIU_BW_Monitor
/// @param u8MiuDev \b IN: miu device id
/// @return true : Success
/// @return false : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Reset_All_BW_Monitor(MS_U8 u8MiuDev);

MS_BOOL MDrv_MIU_ConfigHWAttr(MS_U8 u8MiuDev, eMIU_ATTR_TYPE eAttrType, MS_U16 *u16Value, MS_U16 u16Length);

#ifdef _MIU_INTERNEL_USE
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MIU_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_MIU_GetLoading(MS_U8 u8MiuDev, MS_U32 *Loading);
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_MIU_H_

