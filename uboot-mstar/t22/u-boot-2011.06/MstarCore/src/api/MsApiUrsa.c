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

#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <MsApiUrsa.h>
#include <MsApiPanel.h>
#include <panel/MsDrvPanel.h>
#include <bootlogo/MsPoolDB.h>

#if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_6m38.h>
#endif

#if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_9.h>
#endif

#if defined(CONFIG_URSA_10)
#include <ursa/ursa_10.h>
#endif

#if defined(CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_20.h>
#endif

#include <ursa/ursa_vb1.h>

#if defined (CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa6(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if(0 != IsPanelReady())
    {
        UBOOT_ERROR("Panel is not ready\n");
        return -1;
    }
    MDrv_Ursa_6M38_SWI2C_Init();
    MDrv_Ursa_6M38_Mute(TRUE);
    MDrv_Ursa_6M38_4K2K60_init();
    udelay(30000);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    udelay(40000);
    MDrv_Ursa_6M38_4K2K_DisableAutoMute();
    MDrv_Ursa_6M38_Mute(FALSE);
    return 0;
}
#endif

#if defined (CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa9(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    PanelType panel_data;
    MDrv_Ursa_9_SWI2C_Init();
    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(2, 2);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(4, 4);
    }
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(4, 8);
    }

    memset(&panel_data, 0, sizeof(panel_data));
    if(Read_PanelParaFromflash(&panel_data)!=0)
    {
        UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
        return -1;
    }

    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(1920, panel_data.m_wPanelWidth);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
    }
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
    }
    mdelay(30);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    mdelay(40);
    return 0;
}
#endif

#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa20(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    PanelType panel_data;
    st_sys_misc_setting misc_setting;
    MS_U16 u16OSDResolution = 0;

    memset(&panel_data, 0, sizeof(panel_data));

    if(Read_PanelParaFromflash(&panel_data)!=0)
    {
        UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
        return -1;
    }

    if(Read_MiscSetting_ToFlash(&misc_setting) == 0)
    {
        u16OSDResolution = misc_setting.m_u16OsdWidth;
    }

    if((misc_setting.m_u16OsdWidth == 0) || (misc_setting.m_u16OsdHeight == 0))
    {
        u16OSDResolution = 0;
    }

    MDrv_Ursa_20_SWI2C_Init();
#if defined (CONFIG_URSA20_SYSBIN)
    UBOOT_TRACE("%s: CONFIG_URSA20_SYSBIN = y %d\n", __func__, __LINE__);
    if(IsTVOled())
    {
        UBOOT_TRACE("%s: OLED panel, turn MFC mod on %d\n", __func__, __LINE__);
        MDrv_Ursa_20_SetModPowerOn();
    }
    else
    {
        UBOOT_TRACE("%s: MFC LED panel, turn panel VDD and MFC mod on %d\n", __func__, __LINE__);
        MDrv_Ursa_20_PanelOn();
    }

#else
    UBOOT_INFO("%s: CONFIG_URSA20_SYSBIN = n %d\n", __func__, __LINE__);

    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_20_Set_Lane_VB1_per_init(2, 2);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_20_Set_Lane_VB1_per_init(4, 4);
    }
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_20_Set_Lane_VB1_per_init(4, 8);
    }

    MDrv_Ursa_20_Panel_Initialize();



    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        if(u16OSDResolution == 0)
            MDrv_Ursa_20_Set_Lane_VB1(1920, panel_data.m_wPanelWidth);
        else
            MDrv_Ursa_20_Set_Lane_VB1(u16OSDResolution, panel_data.m_wPanelWidth);
    }

    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        if(u16OSDResolution == 0)
            MDrv_Ursa_20_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
        else
            MDrv_Ursa_20_Set_Lane_VB1(u16OSDResolution, panel_data.m_wPanelWidth);
    }

    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        if(u16OSDResolution == 0)
            MDrv_Ursa_20_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
        else
            MDrv_Ursa_20_Set_Lane_VB1(u16OSDResolution, panel_data.m_wPanelWidth);
    }
#endif

    mdelay(30);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    mdelay(40);

    return 0;
}
#endif

#if (CONFIG_TV_CHIP == 1)
int do_send_I2C_cmd_unlock_transfer_board(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char* opt_unlock = NULL;
    opt_unlock = getenv("unlock_cmi");
    if(opt_unlock != NULL)
    {
       if(strcmp(opt_unlock,"1")==0)
        {
            UBOOT_TRACE("IN\n");
            MDrv_Cmi_Transfer_Unlock();
            UBOOT_TRACE("OK\n");
        }
    }
    return 0;
}
#endif

#if defined(CONFIG_URSA_10)
int do_send_I2C_cmd_to_ursa10(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MDrv_Ursa_10_Lvds_init();
    return 0;
}
#endif

#if defined(CONFIG_URSA6_VB1) ||  defined(CONFIG_URSA_10) || defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined (CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa6(cmdtp,flag,argc,argv);
#endif
#if defined (CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa9(cmdtp,flag,argc,argv);
#endif
#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa20(cmdtp,flag,argc,argv);
#endif
#if defined(CONFIG_URSA_10)
    do_send_I2C_cmd_to_ursa10(cmdtp,flag,argc,argv);
#endif
#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa20(cmdtp,flag,argc,argv);
#endif
    return 0;
}
#endif
