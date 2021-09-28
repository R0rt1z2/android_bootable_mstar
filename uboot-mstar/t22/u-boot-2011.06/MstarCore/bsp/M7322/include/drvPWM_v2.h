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

#ifndef _DRV_PWM_V2_H_
#define _DRV_PWM_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_PWM_Init,
    MDrv_CMD_PWM_Oen,
    MDrv_CMD_PWM_Period,
    MDrv_CMD_PWM_DutyCycle,
    MDrv_CMD_PWM_UnitDiv,
    MDrv_CMD_PWM_Div,
    MDrv_CMD_PWM_Polarity,
    MDrv_CMD_PWM_Vdben,
    MDrv_CMD_PWM_ResetEn,
    MDrv_CMD_PWM_Dben,
    MDrv_CMD_PWM_Shift,
    MDrv_CMD_PWM_Nvsync,
    MDrv_CMD_PWM_GetProperty,
} ePwmIoctlOpt;


//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _PWM_INIT_PARAM
{
    PWM_DbgLv dbg_Lv;
}PWM_INIT_PARAM, *PPWM_INIT_PARAM;

typedef struct _PWM_OEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bOenPWM;
}PWM_OEN_PARAM, *PPWM_OEN_PARAM; 

typedef struct _PWM_PERIOD_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32PeriodPWM;
}PWM_PERIOD_PARAM, *PPWM_PERIOD_PARAM; 

typedef struct _PWM_DUTYCYCLE_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32DutyPWM;
}PWM_DUTYCYCLE_PARAM, *PPWM_DUTYCYCLE_PARAM; 

typedef struct _PWM_UNITDIV_PARAM
{
    MS_U16 u16UnitDivPWM;
}PWM_UNITDIV_PARAM, *PPWM_UNITDIV_PARAM; 

typedef struct _PWM_DIV_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U16 u16DivPWM;
}PWM_DIV_PARAM, *PPWM_DIV_PARAM; 

typedef struct _PWM_POLARITY_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bPolPWM;
}PWM_POLARITY_PARAM, *PPWM_POLARITY_PARAM; 

typedef struct _PWM_VDBEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bVdbenPWM;
}PWM_VDBEN_PARAM, *PPWM_VDBEN_PARAM; 

typedef struct _PWM_RESETEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bRstPWM;
}PWM_RESETEN_PARAM, *PPWM_RESETEN_PARAM; 

typedef struct _PWM_DBEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bDbenPWM;
}PWM_DBEN_PARAM, *PPWM_DBEN_PARAM; 

typedef struct _PWM_SHIFT_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32DutyPWM;
}PWM_SHITF_PARAM, *PPWM_SHIFT_PARAM; 

typedef struct _PWM_NVSYNC_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bNvsPWM;
}PWM_NVSYNC_PARAM, *PPWM_NVSYNC_PARAM; 

typedef struct _PWM_GET_PROPERTY_PARAM
{
    PWM_Property eProperty;
    PWM_ChNum u8IndexPWM;
    MS_U32 *u32PWMVar;
}PWM_GET_PROPERTY_PARAM, *PPWM_GET_PROPERTY_PARAM;

typedef union _PWM_PRIVATE_PARAM
{
    PWM_INIT_PARAM         privatePwmInit;
    PWM_OEN_PARAM          privatePwmOen;
    PWM_PERIOD_PARAM       privatePwmPeriod;
    PWM_DUTYCYCLE_PARAM    privatePwmDuty;
    PWM_VDBEN_PARAM        privatePwmVdben;
    PWM_RESETEN_PARAM      privatePwmRstEn;
    PWM_DBEN_PARAM         privatePwmDben;
    PWM_SHITF_PARAM        privatePwmShift;
    PWM_GET_PROPERTY_PARAM privatePwmGetPro;
} PWM_PRIVATE_PARAM;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_BDMA_V2_H_

