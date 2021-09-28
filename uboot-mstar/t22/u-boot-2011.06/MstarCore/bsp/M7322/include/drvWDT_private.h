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
#ifndef _DRVWDT_PRIV_H_
#define _DRVWDT_PRIV_H_

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
#include "drvWDT.h"

typedef enum {
    MDrv_CMD_WDT_Init,
    MDrv_CMD_WDT_Stop,
    MDrv_CMD_WDT_Clear,
    MDrv_CMD_WDT_ClearRstFlag,
    MDrv_CMD_WDT_IsReset,
    MDrv_CMD_WDT_IsEnable,
    MDrv_CMD_WDT_SetTimer,
    MDrv_CMD_WDT_SetTimer_ms,
    MDrv_CMD_WDT_SetTimer_us,
    MDrv_CMD_WDT_SetIntTimer,
    MDrv_CMD_TIMER_Init,
    MDrv_CMD_TIMER_Count,
    MDrv_CMD_TIMER_Rst,
    MDrv_CMD_TIMER_SetMaxMatch,
    MDrv_CMD_TIMER_HitMaxMatch,
    MDrv_CMD_TIMER_GetMaxMatch,
    MDrv_CMD_TIMER_GetCounter,
    MDrv_CMD_TIMER_GetSecond,
    MDrv_CMD_TIMER_GetMs,
    MDrv_CMD_TIMER_GetUs,
    MDrv_CMD_TIMER_Delay,
    MDrv_CMD_TIMER_DelayMs,
    MDrv_CMD_TIMER_DelayUs,
    MDrv_CMD_TIMER_Initial,
    MDrv_CMD_TIMER_Exit,
    MDrv_CMD_WDT_GetConfig,
} eWdtIoctlOpt;


typedef struct _WDT_RESOURCE_PRIVATE
{
   WDT_DbgLv eLevel;
}WDT_RESOURCE_PRIVATE;

typedef struct _WDT_INIT_PARAM
{
    WDT_DbgLv eLevel;
}WDT_INIT_PARAM, *PWDT_INIT_PARAM;

typedef struct _WDT_IS_RST_PARAM
{
    WDT_Result status;
}WDT_IS_RST_PARAM, *PWDT_IS_RST_PARAM;

typedef struct _WDT_IS_EN_PARAM
{
    WDT_Result status;
}WDT_IS_EN_PARAM, *PWDT_IS_EN_PARAM;

typedef struct _WDT_STOP_PARAM
{
    WDT_DbgLv eLevel;
}WDT_STOP_PARAM, *PWDT_STOP_PARAM;

typedef struct _WDT_SET_S_PARAM
{
    WDT_DbgLv eLevel;
    MS_U16 sec;
}WDT_SET_S_PARAM, *PWDT_SET_S_PARAM;

typedef struct _WDT_SET_MS_PARAM
{
    WDT_DbgLv eLevel;
    MS_U16 msec;
}WDT_SET_MS_PARAM, *PWDT_SET_MS_PARAM;

typedef struct _WDT_SET_US_PARAM
{
    WDT_DbgLv eLevel;
    MS_U16 usec;
}WDT_SET_US_PARAM, *PWDT_SET_US_PARAM;

typedef struct _WDT_SETTIMER_PARAM
{
    WDT_DbgLv eLevel;
    MS_U16 sec;
}WDT_SETTIMER_PARAM, *PWDT_SETTIMER_PARAM;

typedef struct _TIMER_COUNT_PARAM
{
	E_PIU_Timer eTimer;
	MS_BOOL bEnable;
}TIMER_COUNT_PARAM, *PTIMER_COUNT_PARAM;

typedef struct _TIMER_INT_PARAM
{
	E_PIU_Timer eTimer;
	MS_BOOL bEnable;
}TIMER_INT_PARAM, *PTIMER_INT_PARAM;

typedef struct _TIMER_HIT_MAX_MATCH_PARAM
{
	E_PIU_Timer eTimer;
}TIMER_HIT_MAX_MATCH_PARAM, *PTIMER_HIT_MAX_MATCH_PARAM;

typedef struct _TIMER_RST_PARAM
{
	E_PIU_Timer eTimer;
}TIMER_RST_PARAM, *PTIMER_RST_PARAM;

typedef struct _TIMER_SET_MAX_MATCH_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32MaxTimer;
}TIMER_SET_MAX_MATCH_PARAM, *PTIMER_SET_MAX_MATCH_PARAM;

typedef struct _TIMER_GET_MAX_MATCH_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32MaxMatchVal;
}TIMER_GET_MAX_MATCH_PARAM, *PTIMER_GET_MAX_MATCH_PARAM;

typedef struct _TIMER_GET_COUNTER_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32Cnt;
}TIMER_GET_COUNTER_PARAM, *PTIMER_GET_COUNTER_PARAM;

typedef struct _TIMER_GET_SEC_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32Sec;
}TIMER_GET_SEC_PARAM, *PTIMER_GET_SEC_PARAM;

typedef struct _TIMER_GET_MS_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32Msec;
}TIMER_GET_MS_PARAM, *PTIMER_GET_MS_PARAM;

typedef struct _TIMER_GET_US_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32Usec;
}TIMER_GET_US_PARAM, *PTIMER_GET_US_PARAM;

typedef struct _TIMER_DELAY_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32Second;
}TIMER_DELAY_PARAM, *PTIMER_DELAY_PARAM;

typedef struct _TIMER_DELAY_MS_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32ms;
}TIMER_DELAY_MS_PARAM, *PTIMER_DELAY_MS_PARAM;

typedef struct _TIMER_DELAY_US_PARAM
{
	E_PIU_Timer eTimer;
	MS_U32 u32us;
}TIMER_DELAY_US_PARAM, *PTIMER_DELAY_US_PARAM;

typedef union _WDT_PRIVATE_PARAM
{
	 WDT_INIT_PARAM					privateWdtInit;
	 WDT_IS_RST_PARAM				privateWdtIsRst;
	 WDT_IS_EN_PARAM				privateWdtIsEn;
	 WDT_STOP_PARAM				privateWdtStop;
	 WDT_SET_S_PARAM				privateWdtSetSec;
	 WDT_SET_MS_PARAM				privateWdtSetMSec;
	 WDT_SET_US_PARAM				privateWdtSetUSec;
	 WDT_SETTIMER_PARAM			privateWdtSetTimer;
	 TIMER_COUNT_PARAM				privateTimerCnt;
	 TIMER_INT_PARAM				privateTimerInit;
	 TIMER_HIT_MAX_MATCH_PARAM		privateTimerHitMaxMatch;
	 TIMER_RST_PARAM				privateTimerRst;
	 TIMER_SET_MAX_MATCH_PARAM		privateTimerSetMaxMatch;
	 TIMER_GET_MAX_MATCH_PARAM		privateTimerGetMaxMatch;
	 TIMER_GET_COUNTER_PARAM		privateTimerGetCunter;
	 TIMER_GET_SEC_PARAM			privateTimerGetSec;
	 TIMER_GET_MS_PARAM			privateTimerGetMs;
	 TIMER_GET_US_PARAM				privateTimerGetUs;
	 TIMER_DELAY_PARAM				privateTimerDelay;
	 TIMER_DELAY_MS_PARAM			privateTimerDelayMs;
	 TIMER_DELAY_US_PARAM			privateTimerDelayUs;
} WDT_PRIVATE_PARAM;

typedef WDT_Result (*IOCTL_WDT_IS_ENABLE)(void);
typedef WDT_Result (*IOCTL_WDT_INIT)(WDT_DbgLv);
typedef WDT_Result (*IOCTL_WDT_STOP)(WDT_DbgLv);
typedef WDT_Result (*IOCTL_WDT_CLEAR)(void);
typedef WDT_Result (*IOCTL_WDT_CLEAR_RST_FLAG)(void);
typedef WDT_Result (*IOCTL_WDT_IS_RST)(void);
typedef WDT_Result (*IOCTL_WDT_SET_TIMER)(WDT_DbgLv , MS_U16);
typedef WDT_Result (*IOCTL_WDT_SET_MS)(WDT_DbgLv , MS_U16);
typedef WDT_Result (*IOCTL_WDT_SET_US)(WDT_DbgLv , MS_U16);
typedef WDT_Result (*IOCTL_WDT_SET_INT_TIMER)(WDT_DbgLv , MS_U16);
typedef void (*IOCTL_TIMER_COUNT)(E_PIU_Timer , MS_BOOL);
typedef void (*IOCTL_TIMER_INT)(E_PIU_Timer , MS_BOOL);
typedef MS_BOOL (*IOCTL_TIMER_MAX_MATCH)(E_PIU_Timer);
typedef void (*IOCTL_TIMER_RST)(E_PIU_Timer);
typedef void (*IOCTL_TIMER_SET_MAX_MATCH)(E_PIU_Timer , MS_U32 );
typedef MS_U32 (*IOCTL_TIMER_GET_MAX_MATCH) (E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GET_COUNT) (E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GET_SEC) (E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GET_MS) (E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GET_US) (E_PIU_Timer);
typedef void (*IOCTL_TIMER_DELAY) (E_PIU_Timer,MS_U32);
typedef void (*IOCTL_TIMER_DELAY_MS) (E_PIU_Timer,MS_U32);
typedef void (*IOCTL_TIMER_DELAY_US) (E_PIU_Timer,MS_U32);
typedef void (*IOCTL_TIMER_INITIAL) (void);
typedef void (*IOCTL_TIMER_EXIT) (void);

typedef struct _WDT_INSTANT_PRIVATE
{
	IOCTL_WDT_IS_ENABLE 		fpWdtIsEn;
	IOCTL_WDT_INIT  				fpWdtInit;
	IOCTL_WDT_STOP				fpWdtStop;
	IOCTL_WDT_CLEAR			fpWdtClear;
	IOCTL_WDT_CLEAR_RST_FLAG	fpWdtClrRstFlg;
	IOCTL_WDT_IS_RST			fpWdtIsRst;
	IOCTL_WDT_SET_TIMER		fpWdtSetTimer;
	IOCTL_WDT_SET_MS			fpWdtSetMs;
	IOCTL_WDT_SET_US			fpWdtSetUs;
	IOCTL_WDT_SET_INT_TIMER		fpWdtSetIntTimer;
	IOCTL_TIMER_COUNT			fpTimerCount;
	IOCTL_TIMER_INT				fpTimerInt;
	IOCTL_TIMER_MAX_MATCH		fpTimerMaxMatch;
	IOCTL_TIMER_RST				fpTimerRST;
	IOCTL_TIMER_SET_MAX_MATCH	fpTimerSetMaxMatch;
	IOCTL_TIMER_GET_MAX_MATCH	fpTimerGetMaxMatch;
	IOCTL_TIMER_GET_COUNT		fpTimerGetCount;
	IOCTL_TIMER_GET_SEC			fpTimerGetSec;
	IOCTL_TIMER_GET_MS			fpTimerGetMs;
	IOCTL_TIMER_GET_US			fpTimerGetUs;
	IOCTL_TIMER_DELAY			fpTimerDelay;
	IOCTL_TIMER_DELAY_MS		fpTimerDelayMs;
	IOCTL_TIMER_DELAY_US		fpTimerDelayUs;
	IOCTL_TIMER_INITIAL			fpTimerInitial;
	IOCTL_TIMER_EXIT				fpTimerExit;
}WDT_INSTANT_PRIVATE;

void WDTRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 WDTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 WDTClose(void* pInstance);
MS_U32 WDTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 WDTStr(MS_U32 u32PowerState, void* pModule);

WDT_Result MDrv_WDT_Init_U2K(WDT_DbgLv eLevel);
WDT_Result MDrv_WDT_Stop_U2K(WDT_DbgLv eLevel);
WDT_Result MDrv_WDT_Clear_U2K(void);
WDT_Result MDrv_WDT_ClearRstFlag_U2K(void);
WDT_Result MDrv_WDT_IsReset_U2K(void);
WDT_Result MDrv_WDT_IsEnable_U2K(void);
WDT_Result MDrv_WDT_SetTimer_U2K(WDT_DbgLv eLevel, MS_U16 sec);
WDT_Result MDrv_WDT_SetTimer_ms_U2K(WDT_DbgLv eLevel, MS_U16 msec);
WDT_Result MDrv_WDT_SetTimer_us_U2K(WDT_DbgLv eLevel, MS_U16 usec);
WDT_Result MDrv_WDT_SetIntTimer_U2K(WDT_DbgLv eLevel, MS_U16 sec);
void MDrv_TIMER_Init_U2K(void);
void MDrv_TIMER_Count_U2K(E_PIU_Timer eTimer, MS_BOOL bEnable);
void MDrv_TIMER_INT_U2K(E_PIU_Timer eTimer, MS_BOOL bEnable);
void MDrv_TIMER_Rst_U2K(E_PIU_Timer eTimer);
void MDrv_TIMER_SetMaxMatch_U2K(E_PIU_Timer eTimer, MS_U32 u32MaxTimer);
MS_BOOL MDrv_TIMER_HitMaxMatch_U2K(E_PIU_Timer eTimer);
MS_U32 MDrv_TIMER_GetMaxMatch_U2K(E_PIU_Timer eTimer);
MS_U32 MDrv_TIMER_GetCounter_U2K(E_PIU_Timer eTimer);
MS_U32 MDrv_TIMER_GetSecond_U2K(E_PIU_Timer eTimer);
MS_U32 MDrv_TIMER_GetMs_U2K(E_PIU_Timer eTimer);
MS_U32 MDrv_TIMER_GetUs_U2K(E_PIU_Timer eTimer);
void MDrv_TIMER_Delay_U2K(E_PIU_Timer eTimer, MS_U32 u32Second);
void MDrv_TIMER_DelayMs_U2K(E_PIU_Timer eTimer, MS_U32 u32MS);
void MDrv_TIMER_DelayUs_U2K(E_PIU_Timer eTimer, MS_U32 u32US);
void MDrv_TIMER_Exit_U2K(void);


#ifdef __cplusplus
}
#endif
#endif // _DRVWDT_PRIV_H_
