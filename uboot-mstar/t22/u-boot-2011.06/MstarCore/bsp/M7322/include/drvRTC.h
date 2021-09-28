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
/// @brief  Piu Watch Dog Timer Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! 
	\defgroup G_RTC RTC interface
    \ingroup  G_PERIPHERAL
	
	\brief
	A real-time clock (RTC) is a computer clock (most often in the form of an integrated circuit) 
	that keeps track of the current time. Although the term often refers to the devices in personal computers, 
	servers and embedded systems, RTCs are present in almost any electronic device which needs to keep accurate time.

	<b> Operation Flow </b>
	-# Initialize RTC
	-# Set RTC counter
	-# Get RTC counter
	-# Set RTC match counter
	-# Get RTC match counter
	-# Enable PM IRQ for the event to wake up
	-# Disable PM IRQ for the event to wake up

	<b> Architecture </b>
	\image html RTC.png
	
	
    \defgroup G_RTC_INIT Initialization Task relative
    \ingroup  G_RTC
    \defgroup G_RTC_COMMON Common Task relative
    \ingroup  G_RTC
    \defgroup G_RTC_CONTROL Control relative
    \ingroup  G_RTC
    \defgroup G_RTC_OTHER  other relative
    \ingroup  G_RTC
    \defgroup G_RTC_ToBeModified RTC api to be modified
    \ingroup  G_RTC
    \defgroup G_RTC_ToBeRemove RTC api to be removed
    \ingroup  G_RTC
*/

#ifndef _DRV_RTC_H_
#define _DRV_RTC_H_



#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

////////////////////////////////////////////////////////////////////////////////
//  Type and Structure
////////////////////////////////////////////////////////////////////////////////
#define RTC_DRV_VERSION                 /* Character String for DRV/API version  */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                              */  \
    MSIF_CUS,                           /* 0x0000                                         */  \
    MSIF_MOD,                           /* 0x0000                                         */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'R','T','C',},                     /* IP__                                             */  \
    {'0','0'},                          /* 0.0 ~ Z.Z                                      */  \
    {'0','0'},                          /* 00 ~ 99                                         */  \
    {'0','0','0','0','0','0','0','0'},  /* CL#                                              */  \
    MSIF_OS

#define RTC_VERSION            0x00000001

typedef enum _RTC_DbgLv
{
    E_RTC_DBGLV_NONE,          /* no debug message */
    E_RTC_DBGLV_ERR_ONLY,      /* show error only */
    E_RTC_DBGLV_INFO,          /* show error & informaiton */
    E_RTC_DBGLV_ALL            /* show error, information & funciton name */
}RTC_DbgLv;


typedef enum
{
    E_MS_DRV_RTC_0,
    E_MS_DRV_RTC_2,
    E_MS_DRV_RTC_MAX,
}E_MS_DRV_RTC;

typedef enum
{
    E_MS_DRV_RTC_TIME_UNIT_SEC,
    E_MS_DRV_RTC_TIME_UNIT_MS,
    E_MS_DRV_RTC_TIME_UNIT_US,
}E_MS_DRV_RTC_TIME_UNIT;

typedef enum
{
    E_MS_DRV_RTC_STATUS_OK,
    E_MS_DRV_RTC_STATUS_NOT_INIT,
    E_MS_DRV_RTC_STATUS_IN_USE,
    E_MS_DRV_RTC_STATUS_INVALID_ARGUMENT,
}E_MS_DRV_RTC_STATUS;

E_MS_DRV_RTC_STATUS MDrv_RTC_ResetRTC(E_MS_DRV_RTC eRtc);
E_MS_DRV_RTC_STATUS MDrv_RTC_SetCount(E_MS_DRV_RTC eRtc, MS_U32 u32Value);
E_MS_DRV_RTC_STATUS MDrv_RTC_SetMatchCount(E_MS_DRV_RTC eRtc, MS_U32 u32Value);
MS_U32 MDrv_RTC_GetMatchCount(E_MS_DRV_RTC eRtc);
E_MS_DRV_RTC_STATUS MDrv_RTC_IntClear(E_MS_DRV_RTC eRtc);
E_MS_DRV_RTC_STATUS MDrv_RTC_SetPeriod(E_MS_DRV_RTC eRtc, MS_U32 u32Amount, E_MS_DRV_RTC_TIME_UNIT eUnit);
E_MS_DRV_RTC_STATUS MDrv_RTC_StartCounter(E_MS_DRV_RTC eRtc);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_RTC_Init(E_MS_DRV_RTC eRtc, MS_U32 u32Freq, MS_U32 u32Xtal, MS_U32 u32Sec,InterruptCb pIntCb);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_RTC_GetCount(E_MS_DRV_RTC eRtc);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_RTC_DeInit(E_MS_DRV_RTC eRtc);

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register SAR module
//--------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void RTCRegisterToUtopia(FUtopiaOpen ModuleType);  
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 RTCOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 RTCClose(void* pInstance); 
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_RTC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 RTCIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs); 

//RTC_Private
typedef struct _RTC_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}RTC_RESOURCE_PRIVATE;

typedef struct _RTC_INSTANT_PRIVATE
{
   
}RTC_INSTANT_PRIVATE;

typedef struct _RTC_INIT_PARAM
{
    E_MS_DRV_RTC eRtc;
    MS_U32 u32Freq;
    MS_U32 u32Xtal;
    MS_U32 u32Sec;
    InterruptCb pIntCb;

} RTC_INIT_PARAM;

MS_U16 MDrv_RTC_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef __cplusplus
}
#endif

#endif // _DRV_WDT_H_

