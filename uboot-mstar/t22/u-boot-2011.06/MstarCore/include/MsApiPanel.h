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

#ifndef _MS_API_PANEL_
#define _MS_API_PANEL_
#include <MsTypes.h>
#include <apiPNL.h>
#include <panel/panel.h>

/// Max order Size  for CH order
#define MAX_ORDER_SIZE      8

typedef enum
{
    //this valuse is offset in chunk header.
    VB1_CH_ORDER_16V=0x00,
    VB1_CH_ORDER_8V,
    VB1_CH_ORDER_4V,
    VB1_CH_ORDER_2V,
    VB1_CH_ORDER_1V,
    VB1_CH_ORDER_4O,
    VB1_CH_ORDER_2O,
    VB1_CH_ORDER_ORTHER=0xFF
} EN_VB1_CH_ORTHER_ITEM;

/// VB1ChannelOrder
typedef struct
{
    // 16V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb116vOrder[MAX_ORDER_SIZE];
    // 8V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb18vOrder[MAX_ORDER_SIZE];
    // 4V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb14vOrder[MAX_ORDER_SIZE];
    // 2V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb12vOrder[MAX_ORDER_SIZE];
    // 1V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb11vOrder[MAX_ORDER_SIZE];
    // 4O VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb14OOrder[MAX_ORDER_SIZE];
    // 2O VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb12OOrder[MAX_ORDER_SIZE];
}pnl_VB1ChannelOrder;

typedef struct
{
    MS_U8 URSA_6M30_LVDS_CHANNEL_SWAP_MODE;
    MS_U8 URSA_6M30_IN_LVDS_TIMODE;
    MS_U8 URSA_6M30_IN_LVDS_TI_BITNUM;
    MS_U8 URSA_6M30_OUT_LVDS_TIMODE;
    MS_U8 URSA_6M30_OUT_LVDS_TI_BITNUM;
    MS_U8 URSA_6M30_SSC_SEL;
    MS_U8 URSA_6M30_SSC_ENABLE;
    MS_U16 URSA_6M30_SSC_FREQ;
    MS_U16 URSA_6M30_SSC_PERCENT;
}ursa_6m30_cmd_table;

typedef struct
{
    MS_U8 URSA_EXTERNAL;
    MS_U8 URSA_MEMC_ENABLE;
    MS_U8 URSA_LVDS_CHANNEL_SWAP_MODE;
    MS_U8 URSA_LVDS_SWAP_POL;
    MS_U8 URSA_IN_LVDS_TIMODE;
    MS_U8 URSA_IN_LVDS_TI_BITNUM;
    MS_U8 URSA_OUT_LVDS_TIMODE;
    MS_U8 URSA_OUT_LVDS_TI_BITNUM;
    MS_U8 URSA_SSC_SEL;
    MS_U8 URSA_SSC_ENABLE;
    MS_U16 URSA_SSC_FREQ;
    MS_U16 URSA_SSC_PERCENT;
	MS_U16 URSA_OSD_PROTECT_MODE;
}ursa_cmd_table;

typedef struct
{
    MS_U8   m_u8BOARD_PWM_PORT;
    MS_U16  m_u16BOARD_LVDS_CONNECT_TYPE;
    MS_U8   m_bPANEL_PDP_10BIT;
    MS_U8   m_bPANEL_SWAP_LVDS_POL;
    MS_U8   m_bPANEL_SWAP_LVDS_CH;
    MS_U8   m_bPANEL_CONNECTOR_SWAP_PORT;
    MS_U16  m_u16LVDS_PN_SWAP_L;
    MS_U16  m_u16LVDS_PN_SWAP_H;
    char    m_sGPIO1_PAD_NAME[32];
    MS_U16  m_u16GPIO1_INDEX;
    MS_U8   m_u8GPIO1_VALUE;
    char    m_sGPIO2_PAD_NAME[32];
    MS_U16  m_u16GPIO2_INDEX;
    MS_U8   m_u8GPIO2_VALUE;
    MS_BOOL m_bTconOutput;
    MS_BOOL m_bODEnable;
}st_board_para;

int do_panel_init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_panel_pre_init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

int do_backLight_on( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_local_dimming( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//-------------------------------------------------------------------------------------------------
/// Query panle status
/// @return                                   \b OUT: 0: panel is ready, -1: panel is not ready
//-------------------------------------------------------------------------------------------------
int IsPanelReady(void);
APIPNL_LINK_EXT_TYPE GetPanelLinkExtType(void);

typedef PANEL_RESOLUTION_TYPE (*GetPnlTypeSet_cb)(void);
void RegisterCBGetPnlTypeSetting(GetPnlTypeSet_cb cb);
#if (ENABLE_HDMI_RESOLUTION_RESET==1)
int Set_BootArgs_Resolution(void);
#endif

#endif

