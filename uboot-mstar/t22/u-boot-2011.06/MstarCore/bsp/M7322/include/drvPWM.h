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
/// @brief  Pulse Width Modulation driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_PWM PWM interface
    \ingroup  G_PERIPHERAL

    \brief 
 
    Pulse-width modulation (PWM), or pulse-duration modulation (PDM), is a technique used to encode 
	a message into a pulsing signal. It is a type of modulation. Although this modulation technique 
	can be used to encode information for transmission, its main use is to allow the control of the 
	power supplied to electrical devices, especially to inertial loads such as motors.
    
    <b>Features</b>
    - PWM Initialize & De-initialize function
    - PWM Duty Cycle function
    - PWM Periodic function

    <b> Operation Code Flow: </b> \n
    -# PWM initialize with configuration.
    -# PWM channel enable.
    -# Setup duty cycle and periodic.

    \defgroup G_PWM_INIT Initialization Task relative
    \ingroup  G_PWM
    \defgroup G_PWM_COMMON Common Task relative
    \ingroup  G_PWM
    \defgroup G_PWM_CONTROL Control relative
    \ingroup  G_PWM
    \defgroup G_PWM_OTHER  other relative
    \ingroup  G_PWM
    \defgroup G_PWM_ToBeModified PWM api to be modified
    \ingroup  G_PWM
    \defgroup G_PWM_ToBeRemove PWM api to be removed
    \ingroup  G_PWM
*/

#ifndef _DRV_TEMP_H_
#define _DRV_TEMP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "UFO.h"
////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_PWM_LIB_CODE                     {'P','W','M','_'}    //Lib code
#define MSIF_PWM_LIBVER                       {'0','1'}            //LIB version
#define MSIF_PWM_BUILDNUM                     {'0','6'}            //Build Number
#define MSIF_PWM_CHANGELIST                   {'0','0','3','4','3','8','2','1'} //P4 ChangeList Number
#define MSIF_PWM_OS                           '0'                  //OS

#define          PWM_DRV_VERSION                  /* Character String for DRV/API version             */  \
            MSIF_TAG,                         /* 'MSIF'                                           */  \
            MSIF_CLASS,                       /* '00'                                             */  \
            MSIF_CUS,                         /* 0x0000                                           */  \
            MSIF_MOD,                         /* 0x0000                                           */  \
            MSIF_CHIP,                                                                              \
            MSIF_CPU,                                                                               \
            MSIF_PWM_LIB_CODE,                    /* IP__                                             */  \
            MSIF_PWM_LIBVER,                      /* 0.0 ~ Z.Z                                        */  \
            MSIF_PWM_BUILDNUM,                    /* 00 ~ 99                                          */  \
            MSIF_PWM_CHANGELIST,                  /* CL#                                              */  \
            MSIF_OS

#define PWM_DIV (4)

typedef enum _PWM_DbgLv
{
    E_PWM_DBGLV_NONE,          // no debug message
    E_PWM_DBGLV_ERR_ONLY,      // show error only
    E_PWM_DBGLV_INFO,          // show error & informaiton
    E_PWM_DBGLV_ALL            // show error, information & funciton name
}PWM_DbgLv;

typedef enum _PWM_ChNum
{
    E_PWM_CH0,
    E_PWM_CH1,
    E_PWM_CH2,
    E_PWM_CH3,
    E_PWM_CH4,
    E_PWM_CH5,
    E_PWM_CH6,
    E_PWM_CH7,
    E_PWM_CH8,
    E_PWM_CH9
}PWM_ChNum;

typedef enum _PWM_Property
{
    E_PWM_GetPeriod,
    E_PWM_GetDutyCycle,
    E_PWM_GetShift,
    E_PWM_GetDiv,
    E_PWM_GetPolarity,
    E_PWM_GetVdben,
    E_PWM_GetRsten,
    E_PWM_GetDben,
    E_PWM_GetOen
}PWM_Property;

typedef enum _PWM_CntMode
{
    E_PWM_Normal = 0x00,
    E_PWM_01Mode = 0x10,
    E_PWM_10Mode = 0x11
}PWM_CntMode;

typedef enum _PWM_Result
{
    E_PWM_OK,
    E_PWM_FAIL
} PWM_Result;

typedef struct _PWM_Info
{
    MS_U8   u8ChNum;           // number of this channel
    MS_U32  u32IOMap;          // base of this channel's registers
    MS_U32  mck;               // base clock rate from mcu
}PWM_Info;

/* This structure is full the same with hal for now. */
typedef struct _PWM_Arg
{
    MS_U16  index;
    MS_U8   div;
    MS_U16  period;
    MS_U16  duty;
    MS_BOOL polarity;
    MS_BOOL vdben;
    MS_BOOL rsten;
    MS_BOOL dben;
} PWM_Arg;

typedef struct _PWM_DrvStatus
{
    MS_BOOL PWM0;
    MS_BOOL PWM1;
    MS_BOOL PWM2;
    MS_BOOL PWM3;
    MS_BOOL PWM4;
    MS_BOOL PWM5;
    MS_BOOL PWM6;
    MS_BOOL PWM7;
    MS_BOOL PWM8;
} PWM_DrvStatus;

typedef struct _PWM_3D_Arg
{
	MS_U32 Shift;
	MS_U32 Duty;
} PWM_3D_Arg;

typedef enum _N_LR_SYNC_SEL
{
    E_VSYNC             = 0,
    E_LR_RISING         = 1,
    E_LR_FALLING        = 2,
    E_LR_RISING_FALLING = 3
} N_LR_SYNC_SEL;


typedef enum _PM_PWM_ChNum
{
    E_PM_PWM_CH0,
    E_PM_PWM_CH1,
    E_PM_PWM_CH2,
}PM_PWM_ChNum;

#if defined(UFO_PUBLIC_HEADER_500_3)
typedef enum _PM_PWM_PAD_CFG
{
    PAD_PM_VID0,
    PAD_PM_GPIO_1,
    PAD_PM_VID1,
    PAD_PM_GPIO_3,
    PAD_GPIO_2,
    PAD_GPIO_4,
}PM_PWM_PAD_CFG;
#endif

#define NON_INVERSE     0
#define INVERSE         1

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvPWM_v2.h"
#include "MsDevice.h"
////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Description: Enable the specific pm pwm
/// @ingroup G_PWM_CONTROL
/// @param index    \b which pwm is setting
/// @param b_en    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Enable(PM_PWM_ChNum index, PM_PWM_PAD_CFG Pad_Sel, MS_BOOL  b_en);
#elif defined(UFO_PUBLIC_HEADER_700)
//In future, MDrv_PM_PWM_Enable will be replace with MDrv_PM_PWM_Enable_Ex.
//For now, this solution is in UTPA2-700 that will be sync to other code line step by step.
void MDrv_PM_PWM_Enable_Ex(PM_PWM_ChNum index, MS_BOOL  b_en);
#define MDrv_PM_PWM_Enable() MDrv_PM_PWM_Enable_Ex(0, TRUE)
#else
void MDrv_PM_PWM_Enable(void);
#endif
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pm pwm
/// @ingroup G_PWM_CONTROL
/// @param index    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Period(PM_PWM_ChNum index, MS_U16 u16PeriodPWM);
#elif defined(UFO_PUBLIC_HEADER_700)
//In future, MDrv_PM_PWM_Period will be replace with MDrv_PM_PWM_Period_Ex.
//For now, this solution is in UTPA2-700 that will be sync to other code line step by step.
void MDrv_PM_PWM_Period_Ex(PM_PWM_ChNum index, MS_U16 u16PeriodPWM);
#define MDrv_PM_PWM_Period(x) MDrv_PM_PWM_Period_Ex(0, x)
#else
void MDrv_PM_PWM_Period(MS_U16 u16PeriodPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// Description: Set the Duty of the specific pm pwm
/// @ingroup G_PWM_CONTROL
/// @param index    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Duty value
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_DutyCycle(PM_PWM_ChNum index,MS_U16 u16DutyPWM);
#elif defined(UFO_PUBLIC_HEADER_700)
//In future, MDrv_PM_PWM_DutyCycle will be replace with MDrv_PM_PWM_DutyCycle_Ex.
//For now, this solution is in UTPA2-700 that will be sync to other code line step by step.
void MDrv_PM_PWM_DutyCycle_Ex(PM_PWM_ChNum index,MS_U16 u16DutyPWM);
#define MDrv_PM_PWM_DutyCycle(x) MDrv_PM_PWM_DutyCycle_Ex(0, x)
#else
void MDrv_PM_PWM_DutyCycle(MS_U16 u16DutyPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// Description: Set the Div of the specific pm pwm
/// @ingroup G_PWM_CONTROL
/// @param index    \b which pwm is setting
/// @param u8DivPWM    \b the 16-bit div value
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Div(PM_PWM_ChNum index,MS_U8 u8DivPWM);
#elif defined(UFO_PUBLIC_HEADER_700)
//In future, MDrv_PM_PWM_Div will be replace with MDrv_PM_PWM_Div_Ex.
//For now, this solution is in UTPA2-700 that will be sync to other code line step by step.
void MDrv_PM_PWM_Div_Ex(PM_PWM_ChNum index,MS_U8 u8DivPWM);
#define MDrv_PM_PWM_Div(x) MDrv_PM_PWM_Div_Ex(0, x)
#else
void MDrv_PM_PWM_Div(MS_U8 u8DivPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Polarity of the specific pm pwm
/// @ingroup G_PWM_CONTROL
/// @param index    \b which pwm is setting
/// @param bPolPWM    \b True/False for Inverse/Non-inverse
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Polarity(PM_PWM_ChNum index, MS_BOOL bPolPWM);
#elif defined(UFO_PUBLIC_HEADER_700)
//In future, MDrv_PM_PWM_Polarity will be replace with MDrv_PM_PWM_Polarity_Ex.
//For now, this solution is in UTPA2-700 that will be sync to other code line step by step.
void MDrv_PM_PWM_Polarity_Ex(PM_PWM_ChNum index,MS_BOOL bPolPWM);
#define MDrv_PM_PWM_Polarity(x) MDrv_PM_PWM_Polarity_Ex(0, x)
#else
void MDrv_PM_PWM_Polarity(MS_BOOL bPolPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Double buffer of the specific pm pwm
/// @ingroup G_PWM_CONTROL
/// @param index    \b which pwm is setting
/// @param bDbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Dben(PM_PWM_ChNum index,MS_BOOL bdbenPWM);
#elif defined(UFO_PUBLIC_HEADER_700)
//In future, MDrv_PM_PWM_Dben will be replace with MDrv_PM_PWM_Dben_Ex.
//For now, this solution is in UTPA2-700 that will be sync to other code line step by step.
void MDrv_PM_PWM_Dben_Ex(PM_PWM_ChNum index, MS_BOOL bdbenPWM);
#define MDrv_PM_PWM_Dben(x) MDrv_PM_PWM_Dben_Ex(0, x)
#else
void MDrv_PM_PWM_Dben(MS_BOOL bDbenPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PWM_SetPowerState(EN_POWER_MODE u16PowerState);

//------------------------------------------------------------------------------
/// Description : Show the PWM driver version
/// @ingroup G_PWM_COMMON
/// @param  ppVersion \b OUT: output PWM driver version
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetLibVer(const MSIF_Version **ppVersion);

//------------------------------------------------------------------------------
/// Description : Show the PWM_PAD is PWM(True) or GPIO(false)
/// @ingroup G_PWM_COMMON
/// @param  pStatus \b OUT: output PWM driver status
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetStatus(PWM_DrvStatus *pStatus);

//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @ingroup G_PWM_INIT
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Init(PWM_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// Description: IOutput enable_bar of PWM pads
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Oen(PWM_ChNum u8IndexPWM, MS_BOOL bOenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Period(PWM_ChNum u8IndexPWM, MS_U32 u32PeriodPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @return u16PeriodPWM
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetProperty(PWM_Property eProperty, PWM_ChNum u8IndexPWM, MS_U32 *u32PWMVar);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Duty of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Duty value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_DutyCycle(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Unit_Div of the pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u16UnitDivPWM    \b the Unit Div value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_UnitDiv(MS_U16 u16UnitDivPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Div of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DivPWM    \b the 16-bit div value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Div(PWM_ChNum u8IndexPWM, MS_U16 u16DivPWM);
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Polarity of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bPolPWM    \b True/False for Inverse/Non-inverse
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Polarity(PWM_ChNum u8IndexPWM, MS_BOOL bPolPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Vsync Double buffer of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param bVdbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Vdben(PWM_ChNum u8IndexPWM, MS_BOOL bVdbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Hsync reset of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bRstPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_ResetEn(PWM_ChNum u8IndexPWM, MS_BOOL bRstPWM);
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Double buffer of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param bDbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Dben(PWM_ChNum u8IndexPWM, MS_BOOL bDbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: IMPULSE ENABLE
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_IMPULSE_EN(PWM_ChNum index, MS_BOOL bdbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: ODDEVEN_SYNC setting
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_ODDEVEN_SYNC(PWM_ChNum index, MS_BOOL bdbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Rst Mux of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bMuxPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstMux(PWM_ChNum u8IndexPWM, MS_BOOL bMuxPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Rst_Cnt of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param u8RstCntPWM    \b the Rst_Cnt value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstCnt(PWM_ChNum u8IndexPWM, MS_U8 u8RstCntPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Bypass Unit of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bBypassPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_BypassUnit(PWM_ChNum u8IndexPWM, MS_BOOL bBypassPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM0 and PWM1
/// @ingroup G_PWM_CONTROL
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM1 donate internal divider to PWM0    \n
///     10: PWM0 donate internal divider to PWM1    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM01_CntMode(PWM_CntMode u8CntModePWM);
//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM2 and PWM3
/// @ingroup G_PWM_ToBeRemove
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM3 donate internal divider to PWM2    \n
///     10: PWM2 donate internal divider to PWM3    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM23_CntMode(PWM_CntMode u8CntModePWM);
//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM6 and PWM7
/// @ingroup G_PWM_ToBeRemove
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM7 donate internal divider to PWM6    \n
///     10: PWM6 donate internal divider to PWM7    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM67_CntMode(PWM_CntMode u8CntModePWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Shift of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Shift value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Shift(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Nvsync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Align(PWM_ChNum u8IndexPWM, MS_BOOL bAliPWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PWM_Set3DMode(PWM_ChNum u8IndexPWM, PWM_3D_Arg *pArraySeting, MS_U8 u8ArraySize);

//-------------------------------------------------------------------------------------------------
/// Description: Set debug level for debug message
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_SetDbgLevel(PWM_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_INV_3D_Flag(MS_BOOL bInvPWM);

void MDrv_PWM_3D_LR_Sync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM, N_LR_SYNC_SEL LrSyncSel, MS_U8 u8RstCnt);

#ifdef __cplusplus
}
#endif

#endif // _DRV_TEMP_H_

