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
#ifndef _DRVPWM_PRIV_H_
#define _DRVPWM_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvPWM.h"

typedef PWM_Result (*IOCTL_PWM_OEN)(PWM_ChNum, MS_BOOL);
typedef PWM_Result (*IOCTL_PWM_PERIOD)(PWM_ChNum, MS_U32);
typedef PWM_Result (*IOCTL_PWM_DUTYCYCLE)(PWM_ChNum, MS_U32);
typedef PWM_Result (*IOCTL_PWM_UNITDIV)(MS_U16);
typedef void (*IOCTL_PWM_DIV)(PWM_ChNum, MS_U16);
typedef void (*IOCTL_PWM_POLARITY)(PWM_ChNum, MS_BOOL);
typedef void (*IOCTL_PWM_VDBEN)(PWM_ChNum, MS_BOOL);
typedef void (*IOCTL_PWM_RESETEN)(PWM_ChNum, MS_BOOL);
typedef void (*IOCTL_PWM_DBEN)(PWM_ChNum, MS_BOOL);
typedef PWM_Result (*IOCTL_PWM_SHIFT)(PWM_ChNum, MS_U32);
typedef void (*IOCTL_PWM_NVSYNC)(PWM_ChNum, MS_BOOL);
typedef PWM_Result (*IOCTL_PWM_GET_PROPERTY)(PWM_Property,PWM_ChNum,MS_U32 *);
typedef PWM_Result (*IOCTL_PWM_INIT)(PWM_DbgLv);

typedef struct _PWM_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}PWM_RESOURCE_PRIVATE;

typedef struct _PWM_INSTANT_PRIVATE
{
    IOCTL_PWM_INIT              fpPWMInit;
    IOCTL_PWM_OEN		fpPWMOen;
    IOCTL_PWM_PERIOD		fpPWMPeriod;
    IOCTL_PWM_DUTYCYCLE		fpPWMDutyCycle;
    IOCTL_PWM_UNITDIV		fpPWMUnitDiv;
    IOCTL_PWM_DIV		fpPWMDiv;
    IOCTL_PWM_POLARITY		fpPWMPolarity;
    IOCTL_PWM_VDBEN		fpPWMVdben;
    IOCTL_PWM_RESETEN		fpPWMResetEn;
    IOCTL_PWM_DBEN		fpPWMDben;
    IOCTL_PWM_SHIFT		fpPWMShift;
    IOCTL_PWM_NVSYNC		fpPWMNvsync;
    IOCTL_PWM_GET_PROPERTY      fpPWMGetPro;
}PWM_INSTANT_PRIVATE;

void PWMRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 PWMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 PWMClose(void* pInstance);
MS_U32 PWMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U16 PWMStr(MS_U32 u32PowerState, void* pModule);
PWM_Result MDrv_PWM_Init_U2K(PWM_DbgLv eLevel);
PWM_Result MDrv_PWM_Oen_U2K(PWM_ChNum u8IndexPWM, MS_BOOL bOenPWM);
PWM_Result MDrv_PWM_Period_U2K(PWM_ChNum u8IndexPWM, MS_U32 u32PeriodPWM);
PWM_Result MDrv_PWM_GetProperty_U2K(PWM_Property eProperty, PWM_ChNum u8IndexPWM, MS_U32 *u32PWMVar);
PWM_Result MDrv_PWM_DutyCycle_U2K(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM);
PWM_Result MDrv_PWM_Shift_U2K(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM);
void MDrv_PWM_Vdben_U2K(PWM_ChNum u8IndexPWM, MS_BOOL bVdbenPWM);
void MDrv_PWM_ResetEn_U2K(PWM_ChNum u8IndexPWM, MS_BOOL bRstPWM);
void MDrv_PWM_Dben_U2K(PWM_ChNum u8IndexPWM, MS_BOOL bDbenPWM);


#ifdef __cplusplus
}
#endif
#endif // _DRVPWM_PRIV_H_
