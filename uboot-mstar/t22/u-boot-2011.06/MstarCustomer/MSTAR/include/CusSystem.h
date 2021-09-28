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

#ifndef __CMD_MS_CUS_SYSTEM__
#define __CMD_MS_CUS_SYSTEM__


#define BOOT_STATUS_CUSTOMER_ACTIONS_STR    "customer-actions\0"
#define BOOT_STATUS_ACTION_RELOADENV_BIT    (char)(0x01 << 0)
#define BOOT_STATUS_ACTION_RELOADPANEL_BIT  (char)(0x01 << 1)

int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_setmbootver (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#if defined(CONFIG_MTK_WIFI_7668)
int do_MTK_wifi_reset (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
int do_MTK_ursa_reset (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if defined(CONFIG_ENABLE_MIU_SSC)
int do_MIU_SSC (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
int do_envload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_initenv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_loadenv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_setFRC(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_lvds_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ir_delay(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_lvds_off(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_2k_mode_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_osd_unmute (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_panel_set_init (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_panel_set_fhd (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_panel_set_4k2k (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_nova_set_4k2k_2division (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa8_lvds_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa8_set_osd_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_panel_inx_vb1_init (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int Set_4K2K_OP0(void);
int do_ursa_set_osd_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
int do_frc_bringup(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_frc_send(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_frc_recv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if defined(CONFIG_URSA20_VB1) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
int do_oledLGOnRFDoneGPIO(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#endif

