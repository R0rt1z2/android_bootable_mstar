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
/// @brief  MMIO SYS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_MMIO_H__
#define __DRV_MMIO_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define MSIF_MMIO_LIB_CODE              {'M','M','I','O'}    //Lib code
#define MSIF_MMIO_LIBVER                {'0','1'}            //LIB version
#define MSIF_MMIO_BUILDNUM              {'0','2'}            //Build Number
#define MSIF_MMIO_CHANGELIST            {'0','0','4','0','9','9','0','8'} //P4 ChangeList Number

#define MMIO_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_MMIO_LIB_CODE,                  /* IP__                                             */  \
    MSIF_MMIO_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_MMIO_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_MMIO_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// HW register space
#define DRV_MMIO_PM_BANK            0x0001
#define DRV_MMIO_NONPM_BANK         0x0002
#define DRV_MMIO_FLASH_BANK0        0x0003
#define DRV_MMIO_FRC_BANK           0x0004
#define DRV_MMIO_EXTRIU_BANK        0x0005
#define DRV_MMIO_OTP_BANK           0x0010
#define DRV_MMIO_SPRAM_BANK         0x0011
#define DRV_MMIO_OTP_BANK2          0x0012
#define DRV_MMIO_XC_EXT_BANK        0x0013
#define DRV_MMIO_VDEC_BANK	    0x0014


// Specific HW IP base
#define DRV_MMIO_SC_BANK            0x0100
#define DRV_MMIO_TSP_BANK           0x0101
#define DRV_MMIO_NDS_BANK           0x0102
#define DRV_MMIO_NSK_BANK           0x0103
#define DRV_MMIO_CAM_BANK           0x0104
#define DRV_MMIO_NGA_BANK           0x0105
#define DRV_MMIO_SC1_BANK           0x0106


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  MMIO
/// @version 1.0
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup SYS SYS modules
 *
 * \defgroup MMIO MMIO interface (drvMMIO.h)
 *  \ingroup SYS

	\brief

	MMIO maps the RIU to userspace so that drivers are capable to access it directly.

	<b> Diagram </b>
	\image html mmio_1.png

	<b> Code Flow </b>
	-# Init(setup mapping)
	-# Each driver get its own base
	\image html mmio_2.png

 * \defgroup MMIO_BASIC basic mmio interface
 *  \ingroup MMIO
*/

//-------------------------------------------------------------------------------------------------
/// Init MMIO driver
/// @ingroup MMIO_BASIC
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MMIO_Init(void);
//-------------------------------------------------------------------------------------------------
/// Get the virtual address of iomap for register
/// @ingroup MMIO_BASIC
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MMIO_GetBASE(MS_VIRT *ptrBaseaddr, MS_PHY *u32Basesize, MS_U32 u32Module);
//-------------------------------------------------------------------------------------------------
/// DeInit MMIO driver
/// @ingroup MMIO_BASIC
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MMIO_Close(void);


#ifdef __cplusplus
}
#endif


#endif // __DRV_MMIO_H__
