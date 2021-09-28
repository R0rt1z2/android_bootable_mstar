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
/// file    mhal_lth.cpp
/// @brief  HDMITx HDCP HAL
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _MHAL_LTH_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <common.h>
#include <command.h>
#include "MsCommon.h"
#include <MsDebug.h>
#include <apilth.h>
#include <apiSWI2C.h>
#include <apiHDMITx.h>
#include <drvGPIO.h>
#include <apiPNL.h>

#define ROCKET_I2C_WR_SLAVE_ID 0x00A2

#ifndef ROCKET_RESET_GPIO
#define  ROCKET_RESET_GPIO 0
#endif
#if 0
static E_LTH_HDMITX_OUTPUT_TIMING_TYPE getRocket_output_timing(HDMITX_VIDEO_TIMING outputTiming)
{
    E_LTH_HDMITX_OUTPUT_TIMING_TYPE rocketTiming=E_LTH_HDMITX_1080_60P;

    switch(outputTiming)
    {
        case HDMITX_RES_720x480i:
            rocketTiming=E_LTH_HDMITX_480_60I;
            break;
        case HDMITX_RES_720x576i:
            rocketTiming=E_LTH_HDMITX_576_50I;
            break;
        case HDMITX_RES_720x480p:
            rocketTiming=E_LTH_HDMITX_480_60P;
            break;
        case HDMITX_RES_720x576p:
            rocketTiming=E_LTH_HDMITX_576_50P;
            break;
        case HDMITX_RES_1280x720p_50Hz:
            rocketTiming=E_LTH_HDMITX_720_50P;
            break;
        case HDMITX_RES_1280x720p_60Hz:
            rocketTiming=E_LTH_HDMITX_720_60P;
            break;
        case HDMITX_RES_1920x1080i_50Hz:
            rocketTiming=E_LTH_HDMITX_1080_50I;
            break;
        case HDMITX_RES_1920x1080i_60Hz:
            rocketTiming=E_LTH_HDMITX_1080_60I;
            break;
        case HDMITX_RES_1920x1080p_24Hz:
            rocketTiming=E_LTH_HDMITX_1080_24P;
            break;
        case HDMITX_RES_1920x1080p_25Hz:
            rocketTiming=E_LTH_HDMITX_1080_25P;
            break;
        case HDMITX_RES_1920x1080p_30Hz:
            rocketTiming=E_LTH_HDMITX_1080_30P;
            break;
        case HDMITX_RES_1920x1080p_50Hz:
            rocketTiming=E_LTH_HDMITX_1080_50P;
            break;
        case HDMITX_RES_1920x1080p_60Hz:
            rocketTiming=E_LTH_HDMITX_1080_60P;
            break;
        case HDMITX_RES_4K2Kp_30Hz:
            rocketTiming=E_LTH_HDMITX_4K2K_30P;
            break;
        case HDMITX_RES_1280x1470p_50Hz:
            rocketTiming=E_LTH_HDMITX_1470_50P;
            break;
        case HDMITX_RES_1280x1470p_60Hz:
            rocketTiming=E_LTH_HDMITX_1470_60P;
            break;
        case HDMITX_RES_1280x1470p_24Hz:
            rocketTiming=E_LTH_HDMITX_1470_24P;
            break;
        case HDMITX_RES_1280x1470p_30Hz:
            rocketTiming=E_LTH_HDMITX_1470_30P;
            break;
        case HDMITX_RES_1920x2205p_24Hz:
            rocketTiming=E_LTH_HDMITX_2250_24P;
            break;
        case HDMITX_RES_1920x2205p_30Hz:
            rocketTiming=E_LTH_HDMITX_2250_30P;
            break;
        case HDMITX_RES_4K2Kp_25Hz:
            rocketTiming=E_LTH_HDMITX_4K2K_25P;
            break;
        case HDMITX_RES_MAX:
            rocketTiming=E_LTH_HDMITX_TIMING_MAX;
            break;

        default:
            UBOOT_ERROR("getRocket_output_timing: Type Error !!!\n");
            break;
    }

    UBOOT_DEBUG(" SWITCH HDMI TIMING TABLE [%d]--> ROCKET[%d] \n",outputTiming,rocketTiming);
    return rocketTiming;

}
#endif

MS_BOOL is_Interlace(HDMITX_VIDEO_TIMING outputTiming)
{
    MS_BOOL rel=false;

    switch(outputTiming)
    {
        case HDMITX_RES_720x480i:
        case HDMITX_RES_720x576i:
        case HDMITX_RES_1920x1080i_50Hz:
        case HDMITX_RES_1920x1080i_60Hz:
            rel=true;
            UBOOT_DEBUG("interlaced   !!!\n");
            break;


        default:
            UBOOT_DEBUG("progressive  !!!\n");
            break;
    }

    return rel;

}

static MS_BOOL Rocket_HPD_Read(void)
{
    return TRUE;
}

MS_BOOL Rocket_IIC_WriteBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_BOOL bResult = TRUE;
    if(MApi_SWI2C_WriteBytes(u16SlaveCfg ,uAddrCnt, pRegAddr, uSize, pData) == FALSE)
    {
        UBOOT_ERROR("IIC Write fail\n");
        bResult = MApi_SWI2C_WriteBytes(ROCKET_I2C_WR_SLAVE_ID ,0, NULL, 4, pData);
    }
    return bResult;

}

MS_BOOL Rocket_IIC_ReadBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_BOOL bResult = TRUE;
    if(MApi_SWI2C_ReadBytes(u16SlaveCfg ,uAddrCnt, pRegAddr, uSize, pData) == FALSE)
    {
        UBOOT_ERROR("IIC Read fail 2\n");
        bResult = (MApi_SWI2C_WriteBytes(u16SlaveCfg ,uAddrCnt, pRegAddr, uSize, pData));
    }

    return bResult;
}

MS_BOOL msHdmitx_Rocket_Init(HDMITX_VIDEO_TIMING outPutType )
{
    UBOOT_TRACE("IN\n");
    MS_BOOL                     _bIDAC_En = true;
	HDMITX_OUTPUT_MODE          genHDMITX_Mode = HDMITX_HDMI;
#if(CONFIG_MSTAR_NAPOLI_BD_MST051D_20ANV == 1)
	HDMITX_VIDEO_COLOR_FORMAT	genHDMITx_InColor = HDMITX_VIDEO_COLOR_RGB444;
	HDMITX_VIDEO_COLOR_FORMAT	genHDMITx_OutColor = HDMITX_VIDEO_COLOR_RGB444;
#else
	HDMITX_VIDEO_COLOR_FORMAT	genHDMITx_InColor = HDMITX_VIDEO_COLOR_YUV444;
	HDMITX_VIDEO_COLOR_FORMAT	genHDMITx_OutColor = HDMITX_VIDEO_COLOR_YUV444;
#endif
	HDMITX_VIDEO_COLORDEPTH_VAL genHDMITx_ColorDepth = HDMITX_VIDEO_CD_24Bits;
	HDMITX_AUDIO_FREQUENCY      genAudio_Freq = HDMITX_AUDIO_48K;
	HDMITX_AUDIO_CHANNEL_COUNT  genAudio_Ch_Cnt = HDMITX_AUDIO_CH_2;
	HDMITX_AUDIO_CODING_TYPE    genAudio_CodeType = HDMITX_AUDIO_PCM;
	HDMITX_VIDEO_TIMING         genHDMITx_VideoTiming = outPutType;
	E_LTH_HDMITX_AUDIO_TYPE     genAudio_LthType = E_LTH_HDMITX_AUDIO_I2S;
	HDMITX_VIDEO_3D_STRUCTURE   genHDMITx_3D = HDMITx_VIDEO_3D_Not_in_Use;
	HDMITX_VIDEO_4k2k_VIC       genHDMITx_4K2K_VIC = HDMITx_VIDEO_4k2k_Reserved;



    LTH_IIC_CB_CONFIG stIIC_Cfg;
    LTH_HWIIC_CONFIG stHWIIC_Cfg;
    LTH_HPD_CONFIG stHPD_Cfg;

#if(ROCKET_RESET_GPIO>0)
    mdrv_gpio_set_low(ROCKET_RESET_GPIO);
    mdelay(1);
#endif

#if 0
    mdrv_gpio_set_low(PAD_SPI1_DI);
    mdelay(1);
    if(is_Interlace(outPutType))
    {
        MApi_PNL_SetOutputInterlaceTiming(TRUE);

    }
#endif
#if(CONFIG_MSTAR_NAPOLI_BD_MST051D_20ANV == 1)
    WRITE_WORD(0x1F206514,0x003F);
#endif
    // 1. set callback function
    stIIC_Cfg.IIC_WriteBytes = Rocket_IIC_WriteBytes;
    stIIC_Cfg.IIC_ReadBytes = Rocket_IIC_ReadBytes;
    if(MApi_Lth_Init_IIC_CB(stIIC_Cfg) == FALSE)
    {
        UBOOT_ERROR("Init IIC Fail \n");
        return FALSE;
    }

    // 2. init HWI2C driver & HPD
    stHWIIC_Cfg.bUseInternal = TRUE;
    stHWIIC_Cfg.HWI2C_ReadBytes = NULL;
    stHWIIC_Cfg.HWI2C_WriteBytes = NULL;
    if(MApi_Lth_Init_HWIIC(stHWIIC_Cfg) == FALSE)
    {
        UBOOT_ERROR("Init HWI2C Fail \n");
        return FALSE;
    }

    stHPD_Cfg.bUseInternal = FALSE;
    stHPD_Cfg.HPD_Read = Rocket_HPD_Read;
    if(MApi_Lth_Init_HPD(stHPD_Cfg) == FALSE)
    {
        UBOOT_ERROR("HPD Init Fail \n");
        return FALSE;
    }

    if(MApi_Lth_Init() == FALSE)
    {
        UBOOT_ERROR("MApi_Lth_Init Fail \n");
        return FALSE;
    }

    //MApi_Lth_Set_Dbg_Level(3);
    //MApi_HDMITx_SetDbgLevel(3);

    if(MApi_HDMITx_EdidChecking())
    {
        MS_U8 edid_data[128];
        MApi_HDMITx_GetEDIDData(edid_data, 1);
        UBOOT_TRACE("EDID_DATA[3]=%x\n", edid_data[3]);
        if(edid_data[3] & 0x30)
        {
            genHDMITx_OutColor = HDMITX_VIDEO_COLOR_YUV444;
        }
        else
        {
            genHDMITx_OutColor = HDMITX_VIDEO_COLOR_RGB444;
        }
    }
    else
    {
        UBOOT_ERROR("MApi_HDMITx_EdidChecking FAIL \n");
        genHDMITx_OutColor = HDMITX_VIDEO_COLOR_RGB444;
    }

    MApi_HDMITx_TurnOnOff(TRUE);

    MApi_HDMITx_SetHDMITxMode(genHDMITX_Mode);

    MApi_HDMITx_SetHDMITxMode_CD(genHDMITX_Mode, genHDMITx_ColorDepth);

    MApi_HDMITx_SetVideoOutputTiming(genHDMITx_VideoTiming);

    MApi_HDMITx_SetColorFormat(genHDMITx_InColor, genHDMITx_OutColor);
#if 0
    if( genHDMITx_4K2K_VIC != HDMITx_VIDEO_4k2k_Reserved &&
       (genHDMITx_VideoTiming == HDMITX_RES_4K2Kp_30Hz  ||
        genHDMITx_VideoTiming == HDMITX_RES_4K2Kp_25Hz))
    {
        MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, genHDMITx_4K2K_VIC);
    }
    else if( genHDMITx_3D != HDMITx_VIDEO_3D_Not_in_Use &&
            (genHDMITx_VideoTiming >= HDMITX_RES_1280x1470p_50Hz  &&
             genHDMITx_VideoTiming <= HDMITX_RES_1920x2205p_30Hz)    )
    {
        MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_3D, genHDMITx_3D, HDMITx_VIDEO_4k2k_Reserved);
    }
    else
    {
        MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_No_Addition, HDMITx_VIDEO_3D_Not_in_Use, HDMITx_VIDEO_4k2k_Reserved);
    }
#else
    if( genHDMITx_4K2K_VIC != HDMITx_VIDEO_4k2k_Reserved &&
       (genHDMITx_VideoTiming == HDMITX_RES_3840x2160p_30Hz  ||
        genHDMITx_VideoTiming == HDMITX_RES_3840x2160p_25Hz))
    {
        MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, genHDMITx_4K2K_VIC);
    }
    else if( genHDMITx_3D != HDMITx_VIDEO_3D_Not_in_Use &&
            (genHDMITx_VideoTiming >= HDMITX_RES_1920x2205p_24Hz  &&
             genHDMITx_VideoTiming <= HDMITX_RES_1280X1470p_60Hz)    )
    {
        MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_3D, genHDMITx_3D, HDMITx_VIDEO_4k2k_Reserved);
    }
    else
    {
        MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_No_Addition, HDMITx_VIDEO_3D_Not_in_Use, HDMITx_VIDEO_4k2k_Reserved);
    }
#endif
    MApi_HDMITx_SetAudioConfiguration(genAudio_Freq, genAudio_Ch_Cnt, genAudio_CodeType);

    MApi_Lth_Set_Audio(genAudio_LthType);

    MApi_HDMITx_SetVideoOnOff(TRUE);

    MApi_HDMITx_SetAudioOnOff(TRUE);

    MApi_HDMITx_SetTMDSOnOff(TRUE);
    
    MApi_Lth_Set_IDAC_BlackColor( false );

    MApi_Lth_Set_IDAC(_bIDAC_En);

    UBOOT_TRACE("OUT\n");
    return TRUE;
}
