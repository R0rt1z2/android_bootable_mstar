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
/// @brief  Tuner Common Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TUNER_H_
#define _DRV_TUNER_H_

#include "Board.h"
#include "MsCommon.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//============ debug setting ========================
#define TUNER_EN_ERR 0x01
#define TUNER_EN_DBG 0x02
#define TUNER_EN_API 0x04
#define TUNER_DEBUG_OPTIONS  0xf


#if (TUNER_DEBUG_OPTIONS & TUNER_EN_ERR)
#define TUNER_ERR(x) printf("[TUNER ERR]");printf x
#else
#define TUNER_ERR(x) printf("[TUNER ERR]");printf x
#endif

#if (TUNER_DEBUG_OPTIONS & TUNER_EN_DBG)
#define TUNER_DBG(x) printf("[TUNER DBG]");printf x
#else
#define TUNER_DBG(x) printf("[TUNER ERR]");printf x
#endif



#if (TUNER_DEBUG_OPTIONS & TUNER_EN_API)
#define TUNER_ENTERAPI(x) printf x
#define TUNER_ENTERAPISTR(devId) printf("+++ %s [%d] \n\r", __FUNCTION__, devId)
#define TUNER_EXITAPI(x) printf x
#define TUNER_EXITAPISTR(devId, retcode) printf("--- %s [%d] (retcode: %d)\n\r", __FUNCTION__, devId, retcode)
#else
#define TUNER_ENTERAPI(x)
#define TUNER_ENTERAPISTR(devId)
#define TUNER_EXITAPI(x)
#define TUNER_EXITAPISTR(devId, retcode)
#endif

//===============================================


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define _GET_TUNER_ENTRY_NODE(var, Name) var##Name
#define GET_TUNER_ENTRY_NODE(Name) _GET_TUNER_ENTRY_NODE(tuner_entry_,Name)

#define DDIBLD_ATTRIB_TUNER_SECTION(__sect__) __attribute__((section (__sect__)))

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

#ifndef DDI_DRV_TUNER_TABLE_ENTRY
#define DDI_DRV_TUNER_TABLE_ENTRY( _name ) \
        DDIBLD_ATTRIB_TUNER_SECTION(".ecos.tuner." __xstring(_name) ".data")
#endif

/// Define extension function types
typedef enum
{
    TUNER_EXT_FUNC_STANDBY,
    TUNER_EXT_FUNC_WAKEUP,
    TUNER_EXT_FUNC_LOOP_THROUGH,
    TUNER_EXT_FUNC_POWER_ON_OFF,
    TUNER_EXT_FUNC_GET_POWER_LEVEL,
    TUNER_EXT_FUNC_DECIDE_LNB_LO,
    TUNER_EXT_FUNC_FINALIZE,
    TUNER_EXT_FUNC_BLIND_SCAN_MODE
} TUNER_EXT_FUNCTION_TYPE;

typedef enum
{
    TUNER_BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
    TUNER_BW_MODE_7MHZ,                                                       ///< 7 MHz
    TUNER_BW_MODE_8MHZ,                                                       ///< 8 MHz
    TUNER_BW_MODE_1_7MHZ
} TUNER_EN_TER_BW_MODE;

typedef struct
{
    MS_U32* pCur_Broadcast_type;
    MS_U8 u8SlaveID;
} TUNER_MS_INIT_PARAM;

typedef struct
{
    MS_U32 u32RF_FREQ;
    MS_U16 u16LoLOF; // low LOF value,unit MHz
    MS_U16 u16HiLOF; // high LOF value,unit MHz
    MS_BOOL* pbIsHiLOF;
} TUNER_MS_SAT_PARAM;


typedef MS_BOOL     drv_tunerop_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam);
typedef MS_BOOL     drv_tunerop_SetFreq(MS_U8 u8TunerIndex, MS_U32 u32Freq);
typedef MS_BOOL     drv_tunerop_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U32 u32SymbolRate);
typedef MS_BOOL     drv_tunerop_GetLock(MS_U8 u8TunerIndex);
typedef MS_BOOL     drv_tunerop_SetTuner(MS_U8 u8TunerIndex, MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
typedef MS_BOOL     drv_tunerop_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt);
typedef MS_BOOL     drv_tunerop_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq);
typedef MS_BOOL     drv_tunerop_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data);


typedef struct
drv_tunertab_entry
{
    const char                     *name;          // demod system name
    MS_U32                         data;            // private data value
    drv_tunerop_Init               *Init;
    drv_tunerop_SetFreq            *SetFreq;
    drv_tunerop_SetFreq_S2         *SetFreqS2;
    drv_tunerop_GetLock            *GetLock;
    drv_tunerop_SetTuner           *SetTuner;
    drv_tunerop_CheckExist         *CheckExist;
    drv_tunerop_GetTunerIF            *GetTunerIF;
    drv_tunerop_Extension_Function *Extension_Function;
} DRV_TUNER_TABLE_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

#endif // _DRV_TUNER_H_

