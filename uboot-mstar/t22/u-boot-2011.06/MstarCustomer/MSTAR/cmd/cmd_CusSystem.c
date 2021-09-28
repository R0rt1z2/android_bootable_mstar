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
#include <config.h>
#include <CusSystem.h>

#if defined(CONFIG_MTK_WIFI_7668)
U_BOOT_CMD(
    wifi_Reset ,    CONFIG_SYS_MAXARGS,    1,     do_MTK_wifi_reset,
    "wifi_Reset   - wifi Reset for MTK Wifi dongle\n",
    NULL
);
#endif

#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
U_BOOT_CMD(
    ursa_Reset ,    CONFIG_SYS_MAXARGS,    1,     do_MTK_ursa_reset,
    "ursa_Reset   - ursa Reset for MTK Ursa\n",
    NULL
);
#endif

#if defined(CONFIG_ENABLE_MIU_SSC)
U_BOOT_CMD(
    MIU_SSC ,    CONFIG_SYS_MAXARGS,    1,     do_MIU_SSC,
    "MIU_SSC   - ENABLE MIU SSC\n",
    NULL
);
#endif


U_BOOT_CMD(
    envload, CONFIG_SYS_MAXARGS, 1,    do_envload,
    "envload   - reload the nand environment.\n",
    NULL
);

#if defined (CONFIG_MBOOT_VERSION)
U_BOOT_CMD(
    mbootver_set, CONFIG_SYS_MAXARGS, 1,    do_setmbootver,
    "setmbootver   -  for customer show in factory menu.\n",
    NULL
);
#endif

//#if defined(CONFIG_LOAD_ENV_FROM_SN)
U_BOOT_CMD(
    loadenv ,    CONFIG_SYS_MAXARGS,    1,     do_loadenv,
    "loadenv   - load env for nand \n",
    "    -  \n"
);
//#endif

U_BOOT_CMD(
    initenv ,    CONFIG_SYS_MAXARGS,    1,     do_initenv,
    "initenv   - initialize env\n",
    "    -  \n"
);

#if defined (CONFIG_SET_4K2K_MODE)
U_BOOT_CMD(
    set_FRC ,    CONFIG_SYS_MAXARGS,    1,     do_setFRC,
    "set_FRC   - set FRC \n",
    "    -  \n"
);
#endif

#if defined (CONFIG_URSA_6M40)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    ursa_lvds_on,   CONFIG_SYS_MAXARGS, 1,    do_ursa_lvds_on,
    "ursa_lvds_on   - set ursa lvds on\n",
    NULL
);
U_BOOT_CMD(
    ursa_lvds_off,   CONFIG_SYS_MAXARGS, 1,    do_ursa_lvds_off,
    "ursa_lvds_off   - set ursa lvds off\n",
    NULL
);


#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
		ursa_osd_unmute,	CONFIG_SYS_MAXARGS, 1,	  do_ursa_osd_unmute,
		"ursa_osd_unmute	- ursa_osd_unmute\n",
		NULL
	);
U_BOOT_CMD(
		ursa_2k_mode,	CONFIG_SYS_MAXARGS, 1,	  do_ursa_2k_mode_on,
		"ursa_2k_mode	- ursa_2k_mode_on\n",
		NULL
	);

U_BOOT_CMD(
        ursa_set_osd_mode,   CONFIG_SYS_MAXARGS, 1,    do_ursa_set_osd_mode,
        "ursa_set_osd_mode   - do_ursa_set_osd_mode\n",
        NULL
    );
#endif

#if defined (CONFIG_ENABLE_4K2K_PANEL)
#if defined (CONFIG_INX_VB1) || defined (CONFIG_INX_NOVA_VB1)
U_BOOT_CMD(
    inx_panel_set_init,   CONFIG_SYS_MAXARGS, 1,    do_inx_panel_set_init,
    "inx_panel_sete_init   - init inx panel\n",
    NULL
);

U_BOOT_CMD(
    inx_panel_set_fhd,   CONFIG_SYS_MAXARGS, 1,    do_inx_panel_set_fhd,
    "panel_set_fhd   - set panel as fhd mode\n",
    NULL
);

U_BOOT_CMD(
    inx_panel_set_4k2k,   CONFIG_SYS_MAXARGS, 1,    do_inx_panel_set_4k2k,
    "panel_set_4k2k   - set panel as 4k2k mode\n",
    NULL
);
#endif

#if defined (CONFIG_INX_VB1)
U_BOOT_CMD(
    panel_inx_vb1_init,    CONFIG_SYS_MAXARGS, 1,    do_panel_inx_vb1_init,
    "panel_inx_vb1_init   - panel_inx_vb1_init\n",
    NULL
);
#endif

#if defined (CONFIG_INX_NOVA_VB1)
U_BOOT_CMD(
    inx_nova_set_4k2k_2division,   CONFIG_SYS_MAXARGS, 1,    do_inx_nova_set_4k2k_2division,
    "inx_nova_set_4k2k_2division   - set 4k2k in 2 division\n",
    NULL
);
#endif
#endif
#if defined (CONFIG_URSA_8)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    ursa8_lvds_on,   CONFIG_SYS_MAXARGS, 1,    do_ursa8_lvds_on,
    "ursa8_lvds_on   - set ursa8 lvds on\n",
    NULL
);

U_BOOT_CMD(
    ursa8_set_osd_mode,   CONFIG_SYS_MAXARGS, 1,    do_ursa8_set_osd_mode,
    "ursa8_set_osd_mode   - do_ursa8_set_osd_mode\n",
    NULL
);

#endif

#if (ENABLE_MODULE_BOOT_IR == 1)
U_BOOT_CMD(
    ir_delay, CONFIG_SYS_MAXARGS, 1,    do_ir_delay,
    "ir_delay   -  delay 300ms for IR detect.\n",
    NULL
);
#endif

#if(CONFIG_ENABLE_V_I_PWM == 1)
U_BOOT_CMD(
    safemode ,    CONFIG_SYS_MAXARGS,    1,     do_safemode,
    "get safemode from env / set safemode to env",
    " -  safemode get\n"
    " -  safemode set on/off\n"
);
U_BOOT_CMD(
    safemode_check ,    CONFIG_SYS_MAXARGS,    1,     do_check_safemode,
    "get safemode from env / set safemode to env",
    " -  safemode get\n"
    " -  safemode set on/off\n"
);
#endif

#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
U_BOOT_CMD(
    frcup,   CONFIG_SYS_MAXARGS, 1,    do_frc_bringup,
    "frcup   - do_frc_bringup\n",
    NULL
);

U_BOOT_CMD(
    frcsnd,   CONFIG_SYS_MAXARGS, 1,    do_frc_send,
    "frcsnd   - do_frc_send\n",
    "[Usage] - frcsnd [u8CmdIdx] [u32Data]\n"
);

U_BOOT_CMD(
    frcrcv,   CONFIG_SYS_MAXARGS, 1,    do_frc_recv,
    "frcrcv   - do_frc_recv\n",
    "[Usage] - do_frc_recv\n"
);

#endif

#if defined(CONFIG_URSA20_VB1) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
U_BOOT_CMD(
    oledLGOnRFDoneGPIO,   CONFIG_SYS_MAXARGS, 1,    do_oledLGOnRFDoneGPIO,
    "oledLGOnRFDoneGPIO   - do_oledLGOnRFDoneGPIO\n",
    NULL
);
#endif


