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
/// @brief  System Control Interface
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRV_CPU_H_
#define _DRV_CPU_H_

#include "MsTypes.h"
#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_UTOPIA20       (1)

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// #define COPRO_TYPE_DEF
/// Version string.

#define MSIF_COPRO_LIB_CODE                     {'C','O','P','R'}    //Lib code
#define MSIF_COPRO_LIBVER                       {'0','2'}            //LIB version
#define MSIF_COPRO_BUILDNUM                     {'1','0'}            //Build Number
#define MSIF_COPRO_CHANGELIST                   {'0','0','5','5','7','0','9','3'} //P4 ChangeList Number

#define COPRO_DRV_VERSION                       /* Character String for DRV/API version             */  \
    MSIF_TAG,                             /* 'MSIF'                                           */  \
    MSIF_CLASS,                           /* '00'                                             */  \
    MSIF_CUS,                             /* 0x0000                                           */  \
    MSIF_MOD,                             /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_COPRO_LIB_CODE,                        /* IP__                                             */  \
    MSIF_COPRO_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_COPRO_BUILDNUM,                        /* 00 ~ 99                                          */  \
    MSIF_COPRO_CHANGELIST,                      /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_COPRO_OK,
    E_COPRO_FAIL,
} COPRO_Result;

typedef struct _COPRO_DrvInfo
{
    MS_BOOL     Init;
    MS_U32      u32Addr;
} COPRO_DrvInfo;

typedef struct _COPRO_DrvStatus
{
    MS_BOOL     HwBusy;
} COPRO_DrvStatus;

typedef enum _COPRO_DbgLv
{
    E_COPRO_DBGLV_NONE,           //no debug message
    E_COPRO_DBGLV_ERR_ONLY,      //show error only
    E_COPRO_DBGLV_INFO,          //show error & informaiton
    E_COPRO_DBGLV_ALL           //show error, information & funciton name
} COPRO_DbgLv;

typedef enum
{
    Q_PARALLEL,
    Q_STATE,
    Q_MPEG2_FS,
    Q_MPEG2,
    Q_TSPLAYER_FS,
    Q_TSPLAYER,
    Q_VDPLAYER_FS,
    Q_VDPLAYER,
    Q_MBFS,
    Q_IR,
    Q_OBAMA,
    Q_PRINT_MESSAGE,
    Q_CIPLUS,
    Q_DLNA,
    Q_BTPD,
    Q_KTV,
    Q_SHWFS,
    Q_APENGINE,
    Q_MM,
    Q_MHEG5,
    Q_NWDEBUG, //add by Ian
    Q_NUM,
} Q_TYPE;

typedef enum _CPU_CLASS
{
    E_COPRO_MM_AEON,
    E_COPRO_SEC_R2,
    E_COPRO_FRC_R2,
    E_COPRO_SC_R2
} CPU_CLASS;

typedef enum _CPU_CLUSTER
{
    E_HOST_CORE_LITTLE,
    E_HOST_CORE_BIG,
} CPU_CLUSTER;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#if (CPU_UTOPIA20)
#include "drvCPU_v2.h"
#endif


//-------------------------------------------------------------------------------------------------
/// Reset and disable coprocessor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
void MDrv_COPRO_Disable(void);
//-------------------------------------------------------------------------------------------------
/// Enable coprocessor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
void MDrv_COPRO_Enable(MS_U32 u32_ADR);
//void MDrv_BEON_Init(MS_U16 u16BeonBinID, MS_U32 u32BEON_ADR, MS_U32 u32BEON_LEN);
//void MDrv_BEON_Init_ByAddr(MS_U32 u32BEON_ADR, MS_U32 u32BEON_LEN);
void MDrv_SetBEON_Host(MS_BOOL bHost);
//-------------------------------------------------------------------------------------------------
/// Reset and disable coprocessor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
void MDrv_COPRO_Init_Front(void);
//-------------------------------------------------------------------------------------------------
/// Reset and disable coprocessor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
void MDrv_COPRO_Init_End(MS_U32 u32_ADR);
//-------------------------------------------------------------------------------------------------
/// Get the information of coprocessor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
const COPRO_DrvInfo* MDrv_COPRO_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// Get the version of library version
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
COPRO_Result MDrv_COPRO_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// The the status of coprocessor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
COPRO_Result MDrv_COPRO_GetStatus(COPRO_DrvStatus *pStatus);
//-------------------------------------------------------------------------------------------------
/// Setting the debug level
/// @ingroup DRV_CPU
void MDrv_COPRO_SetDbgLevel(MS_U8 u8Level);
//-------------------------------------------------------------------------------------------------
/// Get IO base for H.K.
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_VIRT MDrv_COPRO_GetBase(void);
//-------------------------------------------------------------------------------------------------
/// Get the clock setting of CPU clock
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_U32 MDrv_CPU_QueryClock(void);
//-------------------------------------------------------------------------------------------------
/// Get the clock setting of CPU cluster clock
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_U32 MDrv_CPU_Cluster_QueryClock(CPU_CLUSTER eCluster);
//-------------------------------------------------------------------------------------------------
/// Set the power status of power
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_U32 MDrv_CPU_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// Get DQ infromation
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_U32 MDrv_CPU_GetDqmemInfo(MS_U32 *pu32_DqmemAddr, MS_U32 *pu32_DqmemSize);
#if !defined(CONFIG_FRC)
//-------------------------------------------------------------------------------------------------
/// Disable FRC R2
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
void MDrv_FRCR2_Init_Front(void);
//-------------------------------------------------------------------------------------------------
/// Enable FRC R2
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
void MDrv_FRCR2_Init_End(MS_U32 u32_ADR);
#endif
//-------------------------------------------------------------------------------------------------
/// Disable specified co-processor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_S32 MDrv_CPU_Disable(CPU_CLASS eClass);
//-------------------------------------------------------------------------------------------------
/// Enable specified co-processor
/// @ingroup DRV_CPU
////-----------------------------------------------------------------------------------------------
MS_S32 MDrv_CPU_Enable(CPU_CLASS eClass, MS_PHY phyAddr);

#ifdef __cplusplus
}
#endif

#endif // _DRV_CPU_H_
