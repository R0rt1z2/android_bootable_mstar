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
#ifndef _DRVIRTX_H_
#define _DRVIRTX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#define IRTX_UTOPIA20       (1)

typedef enum _IRTX_Result
{
 
    IRTX_FAIL = 0
    ,IRTX_OK = 1
    ,IRTX_TIMEOUT
    ,IRTX_QUEUE_FULL
    ,IRTX_BUSY
}IRTX_Result;


////////////////////////////////////////////////////////

typedef struct _IRTX_TRIGGER_PARAM
{
	
}IRTX_TRIGGER_PARAM, *PIRTX_TRIGGER_PARAM;

typedef struct _IRTX_SETSTATUS_PARAM
{
    MS_U16 Status;
}IRTX_SETSTATUS_PARAM, *PIRTX_SETSTATUS_PARAM;


typedef struct _IRTX_SETMEMSTATUS_PARAM
{
    MS_U16 Status;
}IRTX_SETMEMSTATUS_PARAM, *PIRTX_SETMEMSTATUS_PARAM;


typedef struct _IRTX_SETCLKDIV_PARAM
{
    MS_U8 Div;
}IRTX_SETCLKDIV_PARAM, *PIRTX_SETCLKDIV_PARAM;


typedef struct _IRTX_SETDELAYCLKTIME_PARAM
{
    MS_U16 CycleTime_H;
	MS_U16 CycleTime_L;
}IRTX_SETDELAYCLKTIME_PARAM, *PIRTX_SETDELAYCLKTIME_PARAM;


typedef struct _IRTX_SETMEMADDR_PARAM
{
    MS_U16 MemAddr;
}IRTX_SETMEMADDR_PARAM, *PIRTX_SETMEMADDR_PARAM;


typedef struct _IRTX_SETMEMDATA_PARAM
{
    MS_U16 MemData;
}IRTX_SETMEMDATA_PARAM, *PIRTX_SETMEMDATA_PARAM;


typedef struct _IRTX_SETUNITVALUE_PARAM
{
    MS_U16 Unit_Value;
	MS_U8 Unit_Number;
}IRTX_SETUNITVALUE_PARAM, *PIRTX_SETUNITVALUE_PARAM;


typedef struct _IRTX_SETSHOTCOUNT_PARAM
{
    MS_U16 H_ShotCount;
    MS_U16 L_ShotCount;
 	MS_U8 Unit_Number;
}IRTX_SETSHOTCOUNT_PARAM, *PIRTX_SETSHOTCOUNT_PARAM;



typedef struct _IRTX_INIT_PARAM
{

}IRTX_INIT_PARAM, *PIRTX_INIT_PARAM;



typedef struct _IRTX_SETCARRIERCOUNT_PARAM
{
    MS_U16 Count;
}IRTX_SETCARRIERCOUNT_PARAM, *PIRTX_SETCARRIERCOUNT_PARAM;




MS_BOOL MDrv_IR_TX_Trigger(void);
MS_BOOL MDrv_IR_TX_SetStatus(MS_U16 Status);
MS_BOOL MDrv_IR_TX_SetMemStatus(MS_U16 Status);
MS_BOOL MDrv_IR_TX_SetClkDiv(MS_U8 Div);
MS_BOOL MDrv_IR_TX_SetDelayCycleTime(MS_U16 CycleTime_H, MS_U16 CycleTime_L);
MS_BOOL MDrv_IR_TX_SetMemAddr(MS_U16 MemAddr);
MS_BOOL MDrv_IR_TX_SetMemData(MS_U16 MemData);
MS_BOOL MDrv_IR_TX_SetUnitValue(MS_U16 Unit_Value, MS_U8 Unit_Number);
MS_BOOL MDrv_IR_TX_SetShotCount(MS_U16 H_ShotCount, MS_U16 L_ShotCount, MS_U8 Unit_Number);
MS_BOOL MDrv_IR_TX_Init(void);
MS_BOOL MDrv_IR_TX_SetCarrierCount(MS_U16 Count);


#ifdef __cplusplus
}
#endif
#endif
