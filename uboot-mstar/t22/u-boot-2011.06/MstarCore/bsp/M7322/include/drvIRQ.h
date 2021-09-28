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
/// @brief  IRQ Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_IRQ IRQ interface
   *  \ingroup  G_PERIPHERAL
     *! \defgroup G_IRQ_INIT Initialization Task relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_EnDecrypt En/Decrypt Task relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_SHA Hash relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_OTHER  other relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_ToBeModified IRQ api to be modified
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_ToBeRemove IRQ api to be removed
     *  \ingroup  G_IRQ
*/

#ifndef _DRV_IRQ_H_
#define _DRV_IRQ_H_

#include"MsIRQ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define MSIF_DRVIRQ_LIB_CODE              {'I','R','Q',' '}    //Lib code
#define MSIF_DRVIRQ_LIBVER                {'0','1'}            //LIB version
#define MSIF_DRVIRQ_BUILDNUM              {'0','3'}            //Build Number
#define MSIF_DRVIRQ_CHANGELIST            {'0','0','5','5','5','6','7','9'} //P4 ChangeList Number

#define DRVIRQ_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DRVIRQ_LIB_CODE,                  /* IP__                                             */  \
    MSIF_DRVIRQ_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_DRVIRQ_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_DRVIRQ_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// TICK_TIMER
#define MDRV_RISC32_TIMER   0x80
// PIC
#define MDRV_RISC32_UART    0x81

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
///Define CPU to CPU interrupt type
typedef enum
{
    E_IRQ_CPU0_2_CPU1,      ///< 8051 to AEON
    E_IRQ_CPU0_2_CPU2,      ///< 8051 to BEON
    E_IRQ_CPU1_2_CPU0,      ///< AEON to 8051
    E_IRQ_CPU1_2_CPU2,      ///< AEON to BEON
    E_IRQ_CPU2_2_CPU0,      ///< BEON to 8051
    E_IRQ_CPU2_2_CPU1,      ///< BEON to AEON
}IRQ_CPU_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_Detech(MS_U32 u32Vector);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_Attach(MS_U32 u32Vector, void *pIntCb);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_Restore(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_MaskAll(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_UnMaskAll(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_Mask(MS_U32 u32Vector);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_UnMask(MS_U32 u32Vector);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_IRQ_NotifyCpu(IRQ_CPU_TYPE eType);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IRQ_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IRQ_InISR(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_IRQ_SetPowerState(EN_POWER_MODE u16PowerState);


#ifdef __cplusplus
}
#endif

#endif // _DRV_IRQ_H_

