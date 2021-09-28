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
#if(FRONTEND_DEMOD_TYPE ==DEMOD_MSB1238)
#ifndef _MSB1238_H_
#define _MSB1238_H_

/* ------------------------------------
    Header Files
   ------------------------------------ */

#include "Board.h"
//#include "datatype.h"
#include "MsTypes.h"
#include "drvIIC.h"

/* ------------------------------------
    Macro Definitions
   ------------------------------------ */

#ifdef _MSB1238_C_
#define INTERFACE
#else
#define INTERFACE  extern
#endif

//typedef enum
//{
//  Samsung_ERR_NO = 0,
//  Samsung_ERR_I2C = 1,
//  Samsung_ERR_INIT = 2,
//  Samsung_ERR_UNLOCK = 4,
//  Samsung_ERR_PARAM = 8,
//  /* Invalid parameter check              */
//  Samsung_ERR_CHECK_FUNC = 16,
//  /* Error returned from LgdtCheck...()   */
//  Samsung_ERR_ALTER_FUNC = 32,
//  /* Error returned from LgdtAlter...()   */
//  Samsung_ERR_SPECTRUM = 64,
//  /* Spectrum status symptom shows when QAM mode  */
//  Samsung_ERR_OTHER = 128,
//  Samsung_ERR_TUNER = 256,
//  /* Reserved for a tuner manufacturer    */
//  Samsung_ERR_SCAN = 512
//}SamsungReturn_t;
//
typedef enum
{
    MSB1238_SIGNAL_NO                   = 0,    /* little or no input power detected    */
    MSB1238_SIGNAL_WEAK                 = 1,    /* some power detected.                 */
    MSB1238_SIGNAL_MODERATE             = 2,    /*  */
    MSB1238_SIGNAL_STRONG               = 4,    /*  */
    MSB1238_SIGNAL_VERY_STRONG        = 8       /*  */
}MSB1238SignalCondition_t;
/*------------------------------------------------------//
// TYPE Definition of MPEG Mode
//------------------------------------------------------*/
//typedef enum
//{
//  Samsung_PARAL = 0x00,
//  /* Samsung_PARAL Mode                   */
//  Samsung_SERIAL = 0x01               /* Samsung_SERIAL Mode                      */
//}SamsungMpegMode_t;
//typedef struct
//{
//  MS_U8 Reg;
//  MS_U16 Value;
//} SamsungChipInit_t;

#define VSB_1238 0x04
#define QAM256_1238  0x02
#define VSB 1
#define QAM64 2
#define QAM256 4
#define VSB_SIGNAL_DISTANCE 5
#define QAM_SIGNAL_DISTANCE 5

INTERFACE MS_BOOL MSB1238_Init(void);
INTERFACE MS_BOOL MSB1238_Download(MS_U8* MSB1238_Download_Address, MS_U16 MSB1238_Download_Size, MS_U8* MSB1238_Download_Table);
INTERFACE MS_BOOL MSB1238_ReadReg(MS_U16 u16Addr, MS_U8* u8Data);
INTERFACE MS_BOOL MSB1238_WriteReg(MS_U16 u16Addr, MS_U8 u8Data);
INTERFACE MS_BOOL MSB1238_RepeaterEnable(void);
INTERFACE MS_BOOL MSB1238_RepeaterDisable(void);
INTERFACE MS_BOOL MSB1238_SoftReset(void);
INTERFACE MS_BOOL MSB1238_VsbMode(void);
INTERFACE MS_BOOL MSB1238_64QamMode(void);
INTERFACE MS_BOOL MSB1238_256QamMode(void);
INTERFACE MS_BOOL MSB1238_Vsb_PreLock(void);
INTERFACE MS_BOOL MSB1238_Vsb_QAM_AGCLock(void);
INTERFACE MS_BOOL MSB1238_QAM_PreLock(void);
INTERFACE MS_BOOL MSB1238_QAM_Main_Lock(void);
INTERFACE MS_BOOL MSB1238_Vsb_CE_Lock(void);
INTERFACE MS_BOOL MSB1238_Vsb_FEC_Lock(void);
INTERFACE void MSB1238_CheckSignalCondition(MSB1238SignalCondition_t* pstatus);
INTERFACE MS_BOOL MSB1238_QAMCheckLock(void);
INTERFACE MS_U8  MSB1238_Check8VSB64_256QAM(void);
INTERFACE MS_U8 MSB1238_ReadSNRPercentage(void);
INTERFACE MS_S16 MSB1238_ReadFrequencyOffset(void);
INTERFACE void MSB1238_FWVERSION(void);
INTERFACE MS_BOOL MSB1238_VSBCheckLock(void);
void MDrv_Demod_1238_SetMode(MS_U8 u8demod_type);
MS_U8 MDrv_Demod_1238_GetMode(void);
//INTERFACE void MSB1238_FWVERSION(void);
#undef INTERFACE
#endif
#endif

