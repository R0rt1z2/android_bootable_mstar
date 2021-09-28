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
/// @brief  SAR Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SAR SAR interface
   *  \ingroup  G_PERIPHERAL
     *! \defgroup G_SAR_INIT Initialization Task relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_COMMON Common Task relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_CONTROL Control relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_OTHER  other relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_ToBeModified SAR api to be modified
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_ToBeRemove SAR api to be removed
     *  \ingroup  G_SAR
*/

#ifndef _DRV_CUS_SAR_H_
#define _DRV_CUS_SAR_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// define SAR library version
#define MSIF_CUS_SAR_LIB_CODE               {'C','S','A','R'}
#define MSIF_CUS_SAR_LIBVER                 {'0','3'}
#define MSIF_CUS_SAR_BUILDNUM               {'0','9'}
#define MSIF_CUS_SAR_CHANGELIST             {'0','0','4','3','3','2','5','1'}

#define CUS_SAR_DRV_VERSION             /* Character String for DRV/API version  */  \
    MSIF_TAG,                           /* 'MSIF' */  \
    MSIF_CLASS,                         /* '00' */  \
    MSIF_CUS,                           /* 0x0000 */  \
    MSIF_MOD,                           /* 0x0000 */  \
    MSIF_CHIP,                                       \
    MSIF_CPU,                                        \
    MSIF_CUS_SAR_LIB_CODE,              /* IP__ */  \
    MSIF_CUS_SAR_LIBVER,                /* 0.0 ~ Z.Z */  \
    MSIF_CUS_SAR_BUILDNUM,              /* 00 ~ 99 */  \
    MSIF_CUS_SAR_CHANGELIST,            /* CL# */  \
    MSIF_OS

/// SAR driver
#define CUS_SAR_VERSION_STRING_MAX      33
#define CUS_SAR_DUMMY_KEY               0xFF
#define CUS_SAR_DUMMY_STATUS            0x00
#define CUS_SAR_VERSION            0x00000001

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//##################################
//
//  Access SAR settings by using Mailbox
//  sent commands to PM
//
//##################################


//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad key code function.
/// @ingroup G_SAR_COMMON
/// @param pu8Key: key code pointer
/// @param pu8Repeat: key repeat pointer
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetKeyCode_Custom(MS_U8 *pu8Key, MS_U8 *pu8Repeat);

//-------------------------------------------------------------------------------------------------
/// Get SAR Multi-Keypad key code function with every SAR channel
/// @ingroup G_SAR_COMMON
/// @param SAR_KeyValue: key code struct pointer
/// @param u32Size: Struct Size
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetMultiKeyCode_Custom(SAR_KeyValue *sSarKey, MS_U32 u32Size, MS_U8 u8ReplyKey);


#ifdef __cplusplus
}
#endif


#endif // _DRV_SAR_H_

