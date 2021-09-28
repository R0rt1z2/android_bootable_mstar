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
/// @brief  UART DMA Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MDRV_URDMA_H__
#define __MDRV_URDMA_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// #define UART_TYPE_DEF
/// Version string.
#define URDMA_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'U','D','M','A'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','1'},                          /* 00 ~ 99                                          */  \
    {'0','0','0','0','0','0','0','0'},  /* CL#                                              */  \
    MSIF_OS


typedef MS_U32                         DRVURDMA_RESULT;

/// @name DRVURDMA_RESULT
/// @ref DRVURDMA_RESULT
/// return value
/// @{
#define DRVURDMA_PASS                 0x00000000
#define DRVURDMA_TX_BUSY              0x00000001
#define DRVURDMA_RX_BUSY			  0x00000002
#define DRVURDMA_INVALID_PARAM        0x00000003
#define DRVURDMA_MIU_ADDR_ERROR       0x00000004
#define DRVURDMA_FATAL_ERROR          0x00000005
/// }@

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum _URDMA_DbgLv
{
     E_URDMA_DBGLV_NONE          //no debug message
    ,E_URDMA_DBGLV_ERR_ONLY      //show error only
    ,E_URDMA_DBGLV_INFO          //show error & informaiton
    ,E_URDMA_DBGLV_ALL           //show error, information & funciton name
}URDMA_DbgLv;

typedef enum
{
    E_URDMA_UART = 0,
    E_URDMA_DMA,
} URDMA_Mode;

typedef enum
{
    E_URDMA_CIRCULAR = 0,
    E_URDMA_LINE,
} URDMA_OPmode;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
DRVURDMA_RESULT MDrv_UrDMA_GetLibVer(const MSIF_Version **ppVersion);
void MDrv_UrDMA_SetDbgLevel(MS_U8 level);
DRVURDMA_RESULT MDrv_UrDMA_TxInit(MS_PHYADDR *tx_buf, MS_U16 tx_size);
DRVURDMA_RESULT MDrv_UrDMA_RxInit(MS_PHYADDR *rx_buf, MS_U16 rx_size);
DRVURDMA_RESULT MDrv_UrDMA_Polling_Tx_Done(void);
void MDrv_UrDMA_ISR(void);
void MDrv_UrDMA_SelMode(URDMA_Mode eMode);
void MDrv_UrDMA_Engine(MS_BOOL bEnable);
void MDrv_UrDMA_Init(void *dma_rx_cb, void *dma_tx_cb, URDMA_OPmode eOp_mode);
#ifdef MS_DEBUG
void MDrv_UrDMA_DumpREG(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MDRV_URDMA_H__ */
