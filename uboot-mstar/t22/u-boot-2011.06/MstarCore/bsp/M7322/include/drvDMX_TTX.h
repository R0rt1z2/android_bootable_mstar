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
/// @brief  MStar DVT TTX Driver Interface
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMX_TTX_H
#define _DRV_DMX_TTX_H

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define VBI_TTX_UTOPIA20 (1)

#define VBI_TTX_DATA_LENGTH 48

typedef struct
{
    MS_BOOL _bTTXInit;
    MS_BOOL _bSuspend;
    MS_PHY bufferAddr;
    MS_U16 packetCount;
    MS_PHY _TTXBufAddr;
    MS_U16 _TTXBufLen;
    MS_U8 _FilterID;
} DMX_TTX_StoreInfo;

typedef enum
{
    DMX_TTX_INIT,
    DMX_TTX_EXIT,
    DMX_TTX_SUSPEND,
    DMX_TTX_RESUME,
} EN_DMX_TTX_CMD;

typedef MS_U32(*DMX_CB_FN)(MS_U8 SecFltNo);
//-------------------------------------------------------------------------------------------------
/// Reset TTX ring buffer
/// @ingroup TTX_Task
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_RingBuffer_Reset(void);
//-------------------------------------------------------------------------------------------------
/// Initialize DMX TTX module
/// @ingroup TTX_General
/// @param  bufferAddr       \b IN: Set TTX ring buffer address
/// @param  packetCount    \b IN: Set TTX ring buffer size
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_Init(MS_PHY bufferAddr, MS_U16 packetCount);
//-------------------------------------------------------------------------------------------------
/// Finalize DMX TTX module
/// @ingroup TTX_General
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_Exit(void);

//-----------------
// TTX_Task
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set TTX callback function.
/// @ingroup TTX_Task
/// @param  fn         \b IN: callback sunction
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_SetCB(DMX_CB_FN fn);
//-------------------------------------------------------------------------------------------------
/// Set TTX PID filter index to use callback function get the write address of section buffer.
/// @ingroup TTX_Task
/// @param  fid         \b IN: PID filter index
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_SetFilterID(MS_U8 fid);
//-------------------------------------------------------------------------------------------------
/// Check TTX buffer is empty or not.
/// @ingroup TTX_Task
/// @return TRUE  : Buffer is empty.
/// @return FALSE : Buffer is not empty.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_PacketBufferIsEmpty(void);
//-------------------------------------------------------------------------------------------------
/// Get TTX data (Get all of the packets in the TTX buffer (if the input buffer is big enough))
/// @ingroup TTX_Task
/// @param  dataAddr       \b IN: Set TTX buffer address
/// @param  length           \b OUT: Get total packt length
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length);
//-------------------------------------------------------------------------------------------------
/// Get TTX data (Get 1 of the packets in the TTX buffer)
/// @ingroup TTX_Task
/// @param  packetAddress \b OUT: Get TTX buffer address
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_GetPacket(MS_PHY *packetAddress);

//-------------------------------------------------------------------------------------------------
/// Check if there is a packet buffer overflow. If there is an overflow,
/// the the packet buffer should be cleared from the reading task.
/// @ingroup TTX_Task
/// @return TRUE  : If there is packet buffer overflow
/// @return FALSE : Otherwise
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_PacketBufferIsOverflow( void );

//-------------------------------------------------------------------------------------------------
/// Return the nomber of packet buffer overflows since the last reset or creation.
/// @ingroup TTX_Task
/// @return : The number of packet buffer overflows.
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_DMX_TTX_PacketBufferGetNoOfOverflows( void );

//-----------------
// TTX_STR
//-------------------------------------------------------------------------------------------------
/// Save DMX_TTX driver states to DRAM.
/// @ingroup TTX_STR
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_Suspend(void);

//-------------------------------------------------------------------------------------------------
/// Restore DMX_TTX driver states from DRAM
/// @ingroup TTX_STR
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_Resume(void);

//-------------------------------------------------------------------------------------------------
/// Control TTX STR functions.
/// @ingroup TTX_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
// MS_U32  MDrv_DMX_TTX_SetPowerState(EN_POWER_MODE u16PowerState);

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif

