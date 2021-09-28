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
#include <uboot_mmap.h>
#include <MsMmap.h>
#include <MsDebug.h>
#include <MsSysUtility.h>

#include <apiXC.h>
#include <apiXC_Adc.h>

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
#if(ENABLE_OSDC_Init==1)

static void MsDrv_OSDC_init(void)
{
    UBOOT_TRACE("IN\n");

    MS_XC_OSDC_CTRL_INFO pstOC_Ctrl;

    pstOC_Ctrl.bOC_ClK_En = TRUE;
    pstOC_Ctrl.bOC_Mixer_Bypass_En = DISABLE;
    pstOC_Ctrl.bOC_Mixer_InvAlpha_En = ENABLE;
    pstOC_Ctrl.bOC_Mixer_Hsync_Vfde_Out = DISABLE;
    pstOC_Ctrl.bOC_Mixer_Hfde_Vfde_Out = ENABLE;
    //pstOC_Ctrl.u16OC_Lpll_type=getLinkExtType();
    pstOC_Ctrl.u16OC_Lpll_type=LINK_VBY1_10BIT_4LANE;
#if defined (CONFIG_URSA7_VB1) && (CONFIG_URSA7_VB1 == 1)
    pstOC_Ctrl.u8OC_OutputFormat = 1;
#else
    pstOC_Ctrl.u8OC_OutputFormat = 0;
#endif

    if( LINK_HS_LVDS == pstOC_Ctrl.u16OC_Lpll_type)
    {
        UBOOT_DEBUG("\n OC Set to LINK_HS_LVDS (1920x1080)\n");
        MApi_XC_OSDC_InitSetting(E_XC_OSDC_TGEN_1920x1080, NULL, &pstOC_Ctrl);
    }
    else if(LINK_VBY1_10BIT_2LANE == pstOC_Ctrl.u16OC_Lpll_type)
    {
        UBOOT_DEBUG("\n OC Set to 2LANE (1920x1080)\n");
        MApi_XC_OSDC_InitSetting(E_XC_OSDC_TGEN_1920x1080, NULL, &pstOC_Ctrl);
        UBOOT_DEBUG("setup OSDC Vfreqx10 %u\n",600);
        MApi_XC_OSDC_SetOutVfreqx10(600);
    }
    else if(LINK_VBY1_10BIT_4LANE == pstOC_Ctrl.u16OC_Lpll_type)
    {
        UBOOT_DEBUG("\n OC Set to 4LANE (3840x2160)\n");
        MApi_XC_OSDC_InitSetting(E_XC_OSDC_TGEN_3840x2160, NULL, &pstOC_Ctrl);
        UBOOT_DEBUG("setup OSDC Vfreqx10 %u\n",300);
        MApi_XC_OSDC_SetOutVfreqx10(300);
    }
    MApi_XC_OSDC_Control(E_XC_OSDC_INIT);
    //PATCH FOR 3D LR, All TV Chip need this patch
    WriteRegBit(0x103260,BIT14,TRUE);
    UBOOT_TRACE("OUT\n");

}
#endif
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
///Entry function
//--------------------------------------------------------------------------------------------------
#if(ENABLE_XC_Init==1)
int MsDrv_XC_init(void)
{

    MS_PHYADDR u32Addr=0;
    U32 u32Size=0;
	XC_INITDATA sXC_InitData;
    XC_INITDATA *pstXC_InitData= &sXC_InitData;
    UBOOT_TRACE("IN\n");

    // reset to zero

    memset(&sXC_InitData, 0, sizeof(sXC_InitData));

    // Init XC
		// (OBA2!=1) // remove when XC driver update
    // Check library version. Do not modify this statement please.
    pstXC_InitData->u32XC_version = XC_INITDATA_VERSION;

    pstXC_InitData->u32XTAL_Clock = MST_XTAL_CLOCK_HZ;
    if(get_addr_from_mmap("E_MMAP_ID_XC_MAIN_FB", &u32Addr)!=0)
    {
        if(get_addr_from_mmap("E_MMAP_ID_XC_SELF", &u32Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_XC_MAIN_FB and E_MMAP_ID_XC_SELF mmap fail\n");
            return  -1;
        }
        else
        {
            get_length_from_mmap("E_MMAP_ID_XC_SELF", (U32 *)&u32Size);
            UBOOT_DEBUG("E_MMAP_ID_XC_SELF 0x%llx \n",u32Addr);
        }
    }
    else
    {
        get_length_from_mmap("E_MMAP_ID_XC_MAIN_FB", (U32 *)&u32Size);
        UBOOT_DEBUG("E_MMAP_ID_XC_MAIN_FB 0x%llx \n",u32Addr);
    }

    pstXC_InitData->u32Main_FB_Size = u32Size; //SCALER_DNR_BUF_LEN;
    pstXC_InitData->u32Main_FB_Start_Addr = u32Addr;//((SCALER_DNR_BUF_MEMORY_TYPE & MIU1) ? (SCALER_DNR_BUF_ADR | MIU_INTERVAL) : (SCALER_DNR_BUF_ADR));

    // Init DNR Address in Main & Sub channel. Keep the same. If project support FB PIP mode, set Sub DNR Address in AP layer (eg. mapp_init).
    pstXC_InitData->u32Sub_FB_Size = pstXC_InitData->u32Main_FB_Size;
    pstXC_InitData->u32Sub_FB_Start_Addr = pstXC_InitData->u32Main_FB_Start_Addr;

    // Chip related.
    pstXC_InitData->bIsShareGround = ENABLE;
    // (OBA2!=1) // remove when XC driver update
    // Board related
    pstXC_InitData->eScartIDPort_Sel = SCART_ID_SEL;//SCART_ID_SEL | SCART2_ID_SEL ;

    pstXC_InitData->bCEC_Use_Interrupt = FALSE;

    pstXC_InitData->bEnableIPAutoCoast = 0;

    pstXC_InitData->bMirror = FALSE;


    // panel info
    pstXC_InitData->stPanelInfo.u16HStart = g_IPanel.HStart();      // DE H start
    pstXC_InitData->stPanelInfo.u16VStart = g_IPanel.VStart();
    pstXC_InitData->stPanelInfo.u16Width  = g_IPanel.Width();
    pstXC_InitData->stPanelInfo.u16Height = g_IPanel.Height();
    pstXC_InitData->stPanelInfo.u16HTotal = g_IPanel.HTotal();
    pstXC_InitData->stPanelInfo.u16VTotal = g_IPanel.VTotal();

    pstXC_InitData->stPanelInfo.u16DefaultVFreq = g_IPanel.DefaultVFreq();

    pstXC_InitData->stPanelInfo.u8LPLL_Mode = g_IPanel.LPLL_Mode();
    pstXC_InitData->stPanelInfo.enPnl_Out_Timing_Mode = (E_XC_PNL_OUT_TIMING_MODE)(g_IPanel.OutTimingMode());

    pstXC_InitData->stPanelInfo.u16DefaultHTotal = g_IPanel.HTotal();
    pstXC_InitData->stPanelInfo.u16DefaultVTotal = g_IPanel.VTotal();
    pstXC_InitData->stPanelInfo.u32MinSET = g_IPanel.MinSET();
    pstXC_InitData->stPanelInfo.u32MaxSET = g_IPanel.MaxSET();
    pstXC_InitData->stPanelInfo.eLPLL_Type = (E_XC_PNL_LPLL_TYPE) g_IPanel.LPLL_Type();
    pstXC_InitData->bDLC_Histogram_From_VBlank = FALSE;

    if(MApi_XC_Init(pstXC_InitData, sizeof(XC_INITDATA)) == FALSE)
    {
        UBOOT_ERROR("L:%d, XC_Init failed because of InitData wrong, please update header file and compile again\n", __LINE__);
        return -1;
    }
#if (CONFIG_HDMITX_MSTAR_ROCKET==1 || CONFIG_HDMITX_MSTAR_ROCKET2==1)
  #if (CONFIG_TV_CHIP==1)
    //set display window
    MApi_XC_SetDispWindowColor(0x0,MAIN_WINDOW);  //TV chip is RGB color, black should set to 0x0.
  #else
    MApi_XC_SetDispWindowColor(0x82,MAIN_WINDOW); //STB chip is YUV color, black should set to 0x82.
  #endif
    MApi_XC_SetFrameColor(0x00801080);
#endif

#if(ENABLE_OSDC_Init==1)
    MsDrv_OSDC_init();
#endif

    UBOOT_TRACE("OUT\n");
    return 1;

}
#endif



