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
/// file    asmCPU.h
/// @brief  Inline Assembly wrapper
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _ASM_CPU_H_
#define _ASM_CPU_H_



#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  For NonOS usage driver
/// @version 1.0
/////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#if defined(__MIPS16E__)
#define ATTRIBUTE   __attribute__((nomips16))
#else
#define ATTRIBUTE
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
//

//-------------------------------------------------------------------------------------------------
/// Cache coherence operation
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_Sync(void);
//-------------------------------------------------------------------------------------------------
/// Do NOP for some special purpose
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_Nop(void);
//-------------------------------------------------------------------------------------------------
/// Debug backtrace 
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_SwDbgBp(void);
//-------------------------------------------------------------------------------------------------
/// Power down CPU
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_PowerDown(void);
//-------------------------------------------------------------------------------------------------
/// Get CPU status
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_StatusBEV(MS_BOOL bBEV);
//-------------------------------------------------------------------------------------------------
/// Setting up Program Counter
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_Jump(MS_U32 u32PC);
//-------------------------------------------------------------------------------------------------
/// Get Trail
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
MS_U8   ATTRIBUTE   MAsm_CPU_GetTrailOne(MS_U32 u32Flags);
//-------------------------------------------------------------------------------------------------
/// Enable timer interrupt
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_EnableTimerInterrupt(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Delay by Ms
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_DelayMs(MS_U32 msec);
//-------------------------------------------------------------------------------------------------
/// Delay by us
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_DelayUs(MS_U32 usec);
//-------------------------------------------------------------------------------------------------
/// Setup the address of exception register 
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_SetEBASE(MS_U32 u32addr);
//-------------------------------------------------------------------------------------------------
/// Get the system time 
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
MS_U32  ATTRIBUTE   MAsm_GetSystemTime (void);
//-------------------------------------------------------------------------------------------------
/// Init timer
/// @ingroup ASM_CPU_BASIC
//-------------------------------------------------------------------------------------------------
void    ATTRIBUTE   MAsm_CPU_TimerInit(void);

#ifdef __cplusplus
}
#endif

#endif // _ASM_CPU_H_

