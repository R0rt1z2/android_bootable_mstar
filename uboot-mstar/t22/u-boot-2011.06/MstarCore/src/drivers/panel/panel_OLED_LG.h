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
#ifndef _API_XC_PRIVATE_H_
#define _API_XC_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <MsTypes.h>
////////////////////////////////////////////////////
//      Type define
////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_OLED_LG_SetOLED_PanelStatus(MS_BOOL bPanelStatus);
MS_BOOL MDrv_Panel_OLED_LG_GetOLED_PanelStatus(void);
MS_BOOL MDrv_Panel_OLED_LG_ErrorCountCheck(void);
void MDrv_Panel_OLED_LG_TurnOledPanelOnOff(MS_BOOL bOn);
void MDrv_Panel_OLED_LG_IncreaseOledErrorCount(void);
void MDrv_Panel_OLED_LG_TimerOLEDError(MS_U32 u32Tick,MS_U32 u32TimerID);
MS_BOOL MDrv_Panel_OLED_LG_EVDD_DET_Check(void);
MS_BOOL MDrv_Panel_OLED_LG_OnRFCountCheck(void);
void MDrv_Panel_OLED_LG_ErrorTimerID_Reset(void);
void MDrv_Panel_OLED_LG_VDD_ON(void);
void MDrv_Panel_OLED_LG_VDD_OFF(void);
void MDrv_Panel_OLED_LG_AC_DET_ON(void);
void MDrv_Panel_OLED_LG_AC_DET_OFF(void);
void MDrv_Panel_OLED_LG_EVDD_ON(void);
void MDrv_Panel_OLED_LG_EVDD_OFF(void);
MS_BOOL MDrv_Panel_OLED_LG_Get_EVDD_DET_Status(void);
MS_BOOL MDrv_Panel_OLED_LG_Get_ON_RF_DONE_Level(void);
MS_BOOL MDrv_Panel_OLED_LG_Get_ERROR_DET_Level(void);
#endif //__PANEL_OLED_LG_H__
