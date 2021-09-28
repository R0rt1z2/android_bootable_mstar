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

//--------------------------------------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------------------------------------
#include <common.h>
#include <MsCommon.h>
#include <drvTVEncoder.h>
#include <uboot_mmap.h>
#include <MsMmap.h>
#include <MsDebug.h>
#include <apiPNL.h>
#if (CONFIG_HDMITX_MSTAR_ROCKET==1)
#include <apilth.h>
#endif
#include <apiXC.h>
#include <apiXC_Adc.h>

#include <apiGOP.h>
#include <ve/MsDrvVE.h>
#include <panel/MsDrvPanel.h>


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_VE_VIDEOSYS getVEVideoSys(HDMITX_VIDEO_TIMING outPutType)
{
    switch (outPutType)
    {
        case HDMITX_RES_640x480p:
        case HDMITX_RES_720x480i:
        case HDMITX_RES_720x480p:
        case HDMITX_RES_1280x720p_60Hz:
        case HDMITX_RES_1920x1080i_60Hz:
        case HDMITX_RES_1920x1080p_24Hz:
        case HDMITX_RES_1920x1080p_30Hz:
        case HDMITX_RES_1920x1080p_60Hz:
        case HDMITX_RES_4K2Kp_30Hz:
        case HDMITX_RES_1280x1470p_60Hz:
        case HDMITX_RES_1280x1470p_24Hz:
        case HDMITX_RES_1280x1470p_30Hz:
        case HDMITX_RES_1920x2205p_24Hz:
        case HDMITX_RES_1920x2205p_30Hz:
        case HDMITX_RES_4K2Kp_25Hz:
        case HDMITX_RES_MAX:
            
            return MS_VE_NTSC;
            
        case HDMITX_RES_720x576i:
        case HDMITX_RES_720x576p:
        case HDMITX_RES_1280x720p_50Hz:
        case HDMITX_RES_1920x1080i_50Hz:
        case HDMITX_RES_1920x1080p_25Hz:
        case HDMITX_RES_1920x1080p_50Hz:
        case HDMITX_RES_1280x1470p_50Hz:
            
            return MS_VE_PAL;
        default:
            UBOOT_ERROR("ERROR: Not support current panel resolution!\n");
            break;
    }

    return MS_VE_VIDEOSYS_NUM;
}

static MS_U16 getFrameRates(HDMITX_VIDEO_TIMING outPutType)
{
    switch (outPutType)
    {
        case HDMITX_RES_640x480p:
        case HDMITX_RES_720x480i:
        case HDMITX_RES_720x480p:
        case HDMITX_RES_1280x720p_60Hz:
        case HDMITX_RES_1920x1080i_60Hz:
        case HDMITX_RES_1920x1080p_24Hz:
        case HDMITX_RES_1920x1080p_30Hz:
        case HDMITX_RES_1920x1080p_60Hz:
        case HDMITX_RES_4K2Kp_30Hz:
        case HDMITX_RES_1280x1470p_60Hz:
        case HDMITX_RES_1280x1470p_24Hz:
        case HDMITX_RES_1280x1470p_30Hz:
        case HDMITX_RES_1920x2205p_24Hz:
        case HDMITX_RES_1920x2205p_30Hz:
        case HDMITX_RES_MAX:
            return 6000 / 2;
        case HDMITX_RES_720x576i:
        case HDMITX_RES_720x576p:
        case HDMITX_RES_1280x720p_50Hz:
        case HDMITX_RES_1920x1080i_50Hz:
        case HDMITX_RES_1920x1080p_25Hz:
        case HDMITX_RES_1920x1080p_50Hz:
        case HDMITX_RES_1280x1470p_50Hz:
        default:
            break;
    }

    return 5000 / 2;
}


MS_BOOL mvideo_sc_is_interlace(void)
{
    return 0;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
///Entry function
//--------------------------------------------------------------------------------------------------
int MsDrv_VE_init(HDMITX_VIDEO_TIMING outPutType)
{

    MS_PHYADDR u32Addr=0;
    U32 u32Size=0;

    UBOOT_TRACE("IN\n");
    
    
    //0. VE initialization
    if(get_addr_from_mmap("E_MMAP_ID_VE", &u32Addr)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_VE fail\n");
        return -1;
    }   
   
    MApi_XC_SetOutputCapture(ENABLE,E_XC_OP2);      // Enable op2 to ve path
    MDrv_VE_Init(u32Addr); 
    UBOOT_DEBUG("E_MMAP_ID_VE 0x%llx \n",u32Addr);
    MDrv_VE_SetOSD(FALSE);
    
    MDrv_VE_SetOutputVideoStd(getVEVideoSys(outPutType));
    //setHDMITxAnalogTuning(outPutType);    //set bank21_0x2e(16bit mode)
    //1. VE configuration
    MS_Switch_VE_Src_Info SwitchInputSrc;
    SwitchInputSrc.InputSrcType = MS_VE_SRC_SCALER;
    
    MDrv_VE_SwitchInputSource(&SwitchInputSrc);

    MS_Switch_VE_Dest_Info SwitchOutputDest;
	SwitchOutputDest.OutputDstType = MS_VE_DEST_SCART;
    
    MDrv_VE_SwitchOuputDest(&SwitchOutputDest);

    MS_VE_Set_Mode_Type SetModeType;
    SetModeType.u16H_CapSize     = g_IPanel.Width();

    {
        SetModeType.bSrcInterlace = FALSE;
        SetModeType.u16V_CapSize     = g_IPanel.Height();
    }
    SetModeType.u16H_CapStart    = g_IPanel.HStart();
    SetModeType.u16V_CapStart    = g_IPanel.VStart();
    SetModeType.u16H_SC_CapSize  = g_IPanel.Width();
    SetModeType.u16V_SC_CapSize  = g_IPanel.Height();
    SetModeType.u16H_SC_CapStart = g_IPanel.HStart();
    SetModeType.u16V_SC_CapStart = g_IPanel.VStart();
    SetModeType.u16InputVFreq = getFrameRates(outPutType)*2/10;
    
    SetModeType.bHDuplicate   = FALSE;
    
    MDrv_VE_SetMode(&SetModeType);

    MS_VE_Output_Ctrl OutputCtrl;
    OutputCtrl.bEnable = TRUE;
    
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    
    MDrv_VE_SetOutputCtrl(&OutputCtrl);
    MDrv_VE_SetRGBIn(FALSE);
    MApi_XC_ADC_SetCVBSOut(ENABLE, OUTPUT_CVBS1, INPUT_SOURCE_DTV, TRUE);
    UBOOT_TRACE("OUT\n");
    return 1;

}



