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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  TSIO API
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __API_DMX_TSIO_H__
#define __API_DMX_TSIO_H__

#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum
{
    TSIO_CC_RESULT_OK,
    TSIO_CC_RESULT_NORESULT,
    TSIO_CC_RESULT_TXCMD_TOOSMALL,
    TSIO_CC_RESULT_TXCMD_TOOBIG,
    TSIO_CC_RESULT_TXCMD_TIMEOUT,
    TSIO_CC_RESULT_RXCMD_TOOSMALL,
    TSIO_CC_RESULT_RXCMD_TOOBIG,
    TSIO_CC_RESULT_RXCMD_TIMEOUT,
    TSIO_CC_RESULT_ERR_FLAG,
    TSIO_CC_RESULT_ERROR,
}TSIO_CC_RESULT;

typedef enum
{
    TSIO_CLK_200M = 0,
    TSIO_CLK_100M,
    TSIO_CLK_50M,
    TSIO_CLK_25M,
}TSIO_CLK;

typedef enum
{
    TSIO_CMD_BYPASSSMC,         //bypass SMC setting, 0: disable, 1: direct8, 2: direct 16, 3: loopback.
    TSIO_CMD_STUFFINGSID,       //stuffing id (from 0 ~ 0x3F)
    TSIO_CMD_LOCDECRYPT,        //1: enable loc decrypt, 0: disable
    TSIO_CMD_SYNCTHRESHOLD,     //sync threshold (3 ~ 8)
    TSIO_CMD_DBGLEVEL,          //3: trace, 2: info, 1: error, 0: release


    TSIO_CMD_PHASE_ADJUST,      //kastor U01 use
    TSIO_CMD_BITTRAINING_ADJUST,//kastor U01 use
}TSIO_CMD;

typedef enum
{
    TSIO_INT_TYPE_NONE = 0,
    TSIO_INT_TYPE_SGDMAOUT_LASTNODE,
    TSIO_INT_TYPE_SGDMAOUT_NODEINT,
    TSIO_INT_TYPE_SGDMAOUT_DROP,
    TSIO_INT_TYPE_SGDMAOUT_DMAEND,
    TSIO_INT_TYPE_SGDMAIN_DONE,
    TSIO_INT_TYPE_CC_TX_DONE,
    TSIO_INT_TYPE_CC_RX_DONE,
    TSIO_INT_TYPE_LIVE_TSO_OVERFLOW,
    TSIO_INT_TYPE_OPERATION_START,
}TSIO_INT_TYPE;

typedef enum
{
    TSIO_INFO_LOSELOCKCNT = 0,      //lose locked count
    TSIO_INFO_USEBUFLEN = 1,        //get use buffer size, arg is service handleid from 0 to 63
}TSIO_INFO;

/// Pid filter source for TSIO
typedef enum
{
    E_TSIO_FLT_SRC_NONE         = 0x00000000UL,
    E_TSIO_FLT_SRC_TSO_LIVE0    = 0x00000001UL,
    E_TSIO_FLT_SRC_TSO_LIVE1    = 0x00000002UL,
    E_TSIO_FLT_SRC_TSO_LIVE2    = 0x00000003UL,
    E_TSIO_FLT_SRC_TSO_LIVE3    = 0x00000004UL,
    E_TSIO_FLT_SRC_TSO_FILE0    = 0x00000005UL,
    E_TSIO_FLT_SRC_TSO_FILE1    = 0x00000006UL,
    E_TSIO_FLT_SRC_DMAIN        = 0x00000010UL,
    E_TSIO_FLT_SRC_MAX          = 0xFFFFFFFFUL,
}TSIO_FLT_SRC;

typedef struct
{
    MS_PHY phyLastpktBufAddr; // 192 bytes (pkt size) x 64 (service num)
    MS_U32 u32LastpktBufSize;
    MS_PHY phyDMAVQBufAddr;
    MS_U32 u32DMAVQBufSize;
    MS_PHY phyDMAinNodeAddr; // 1 node need 16 bytes.
    MS_U32 u32DMAinNodeSize;
    MS_PHY phyDMAoutNodeAddr; // 1 node need 16 bytes.
    MS_U32 u32DMAoutNodeSize;
}TSIO_Init_Set;

typedef struct
{
    MS_U8* pIn;
    MS_U16 u16InLen;
    MS_U8* pOut;
    MS_U16* pOutLen;
    MS_U32 u32Timeout;
}TSIO_CC_Set;

typedef enum
{
    E_TSIO_SERVICEDEST_LIVE = 0,
    E_TSIO_SERVICEDEST_DMA,
}TSIO_SERVICEDEST;

typedef struct
{
    MS_PHY phyBufAddr;
    MS_U32 u32BufSize;
    MS_BOOL bTSmode;
    MS_BOOL bMidPriority;
    MS_BOOL bLastNodeFlush;
    MS_BOOL bNewPkt;
    MS_BOOL bBypassAll;
}TSIO_DMAIN_SET;

typedef struct
{
    MS_PHY phyBufAddr;
    MS_U32 u32BufSize;
    MS_BOOL bSecurity;
}TSIO_DMAOUT_SET;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bEvenkeyenable;
    MS_U8 *u8Evenkey;
    MS_U16 u16Evenkeylen;
    MS_BOOL bOddkeyenable;
    MS_U8 *u8Oddkey;
    MS_U16 u16Oddkeylen;
}TSIO_LOCKEY_SET;

typedef struct
{
    TSIO_FLT_SRC    eFltSrc;
    MS_U16          u16Pid;
    MS_U32          *pu32DmxId;
    MS_BOOL         bBypassTSIO;
}TSIO_FLT_CFG;

typedef void(*TSIOIntTypeCB)(TSIO_INT_TYPE eType, MS_U32 u32Info, void *ext1, void *ext2);

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#ifdef DMX_UTOPIA_20
#include "apiDMX_tsio_v2.h"
#endif

//-------------------------------------------------------------------------------------------------
/// Register TSIO callback function
/// @param TSIOIntTypeCB                                  \b IN: Callback function pointer
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_RegisterIntCb(TSIOIntTypeCB efn);

//-------------------------------------------------------------------------------------------------
/// Init TSIO
/// @param eParm                         \b IN: init param
/// @return DMX_FILTER_STATUS
/// @note
/// It should be called before calling any TSIO API.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Init(TSIO_Init_Set *eParm);

//-------------------------------------------------------------------------------------------------
/// Open TSIO
/// @param eClk                         \b IN: smc clk speed
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Open(TSIO_CLK eClk);

//-------------------------------------------------------------------------------------------------
/// Exit TSIO
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Close TSIO
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Close(void);

//-------------------------------------------------------------------------------------------------
/// Send ctrl command to TSIO
/// @param eCmd                                     \b IN: Command
/// @param u32Value                                \b IN: Commnad's param
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Cmd(TSIO_CMD eCmd, MS_U32 u32Value);

//-------------------------------------------------------------------------------------------------
/// Send and Recv C&C haed-based cmd
/// @param Parm                                        \b IN: Pointer to store C&C cmd address
/// @param pResult                                     \b IN: Pointer to store result status
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_CC(TSIO_CC_Set *Parm, TSIO_CC_RESULT* pResult);

//-------------------------------------------------------------------------------------------------
/// Get TSIO informaiton
/// @param eInfo                                        \b IN: Info type
/// @param u32Arg                                      \b IN: detail value
/// @param u32Retinfo                                 \b OUT: Pointer to store Info address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_GetInfo(TSIO_INFO eInfo , MS_U32 u32Arg, MS_U32 *u32Retinfo);

//-------------------------------------------------------------------------------------------------
/// Set DMAOUT Virture channel to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetDMAoutVC(MS_U16 u16Handle, TSIO_DMAOUT_SET *eSet);

//-------------------------------------------------------------------------------------------------
/// Get DMAOUT VC  write address
/// @param u16Handle                                    \b IN: Service handle ID
/// @param pphyWrite                                    \b OUT: dma out write address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_DMAout_WriteGet(MS_U16 u16Handle, MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set destination to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma out param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetDestination(MS_U16 u16Handle, TSIO_SERVICEDEST eDest);

//-------------------------------------------------------------------------------------------------
/// Set DMAIN Virture channel to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma in param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetDMAinVC(MS_U16 u16Handle, TSIO_DMAIN_SET *eSet);

//-------------------------------------------------------------------------------------------------
/// start DMAIN Virture channel
/// @param u16Handle                                    \b IN: Service handle ID
/// @param u32Pacing                                     \b IN: Sgdma in pacing rate
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_DMAinVC_Start(MS_U16 u16Handle, MS_U32 u32Pacing);

//-------------------------------------------------------------------------------------------------
/// Set Locdecrypt key to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                            \b IN: Pointer to store loc decrypt key address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetLocdecKey(MS_U16 u16Handle, TSIO_LOCKEY_SET *eSet);

//-------------------------------------------------------------------------------------------------
/// Enable/disable locdec key
/// @param u16Handle                                    \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable locdec key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_LocdecKeyEnable(MS_U16 u16Handle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO service
/// @param u8Sid                                            \b IN: Service ID
/// @param u16Handle                                    \b OUT: pointer of service handle for return
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_Alloc(MS_U8 u8Sid, MS_U16 *u16Handle);

//-------------------------------------------------------------------------------------------------
/// Free TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_Free(MS_U16 u16Handle);

//-------------------------------------------------------------------------------------------------
/// open pid filter for TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param TSIO_FLT_CFG                                 \b IN: TSIO PID filter configuration
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_PidOpen(MS_U16 u16Handle, TSIO_FLT_CFG *pstFltCfg);

//-------------------------------------------------------------------------------------------------
/// close pid filter for TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eFltSrc                                      \b IN: TSIO PID filter source
/// @param u32DmxId                                     \b IN: TSIO PID filter ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_PidClose(MS_U16 u16Handle, MS_U32 u32DmxId);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_H__
