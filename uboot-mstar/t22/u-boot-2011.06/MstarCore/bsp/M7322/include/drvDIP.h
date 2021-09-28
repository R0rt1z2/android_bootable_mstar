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
#ifndef _DRVDIP_H_
#define _DRVDIP_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief DIP control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_NR_BUFF_CNT     2UL
#define MAX_DATA_BUFF_CNT   16UL

typedef struct
{
    MS_U8               u8NR_BufCnt;
    MS_PHY              u32NR_RationBuf;
    MS_PHY              u32NR_DataBuf[MAX_NR_BUFF_CNT];
    MS_U32              u32NR_BufWidth;
    MS_U32              u32NR_BufHeight;
} NR_INFO;

typedef struct
{
    MS_U8               u8DI_BufCnt;
    MS_PHY              u32DI_YBuf[MAX_DATA_BUFF_CNT];
    MS_PHY              u32DI_CBuf[MAX_DATA_BUFF_CNT];
    MS_U32              u32DI_BufWidth;
    MS_U32              u32DI_BufHeight;
    MS_U16              u16Valid_DIBuf;
    MS_U32              u32DI_FrameCnt;
} DI_INFO;


typedef struct
{
    MS_BOOL             Init;
    NR_INFO             NR_Info;
    DI_INFO             DI_Info;

} DIP_MGR;

typedef enum
{
    DIP_ERR_OK,
    DIP_ERR_FAIL,
    DIP_ERR_INVALID_BUFFER_CNT,
    DIP_ERR_INVALID_BUFFER_START,
    DIP_ERR_INVALID_BUFFER_WIDTH,
    DIP_ERR_INVALID_BUFFER_HEIGHT,
    DIP_ERR_INVALID_BUFFER_SZIE
}DIP_ERRCODE;

typedef enum
{
    E_DIP_INPUT_UNKNOWN = 0,
    E_DIP_INPUT_NTSC,
    E_DIP_INPUT_PAL,
    E_DIP_INPUT_WEBCAM,
}DIP_INPUT;

typedef enum
{
    E_DIP_YUV_ORDER_UYVY = 0,
    E_DIP_YUV_ORDER_YUYV = 1,
} DIP_YUV_ORDER;

typedef enum
{
    E_DIP_INIT ,
    E_DIP_ENABLE ,
    E_DIP_ONESHOT ,
    E_DIP_SET_INFO ,
    E_DIP_SET_YUORD ,
    E_DIP_GET_BUFSTUS ,    
    E_DIP_CLR_BUFSTUS ,
    E_DIP_SET_SCANTYPE ,
    E_DIP_SET_INPUTMODE ,
    E_DIP_SET_WEBCAM ,
} DIP_CNTL;

typedef enum
{
    //By HW or SW DIP
    E_DIP_CAP_CONFIG,
}
EN_DIP_CAPS;

typedef enum
{
    E_DIP_CFG_SW =0,
    E_DIP_CFG_HW =1,
}
EN_DIP_Config;

typedef struct
{           
    /// dwin dram starting address (unit: Byte).
    MS_PHY   u32FbAddr0;
    ///  dwin access address high bond (unit: Byte).
    MS_PHY   u32FbAddr1;
    /// dwin width (unit: pix).
    MS_U16 u16W;
    /// height (unit: line).
    MS_U16 u16H;
    
    MS_U8  u8BufCnt;
    
} DIP_PROPERTY;

typedef struct
{           
    /// dwin dram starting address (unit: Byte).
    MS_VIRT u32WebCamAddr0;
    ///  dwin access address high bond (unit: Byte).
    MS_VIRT u32WebCamAddr1;
    /// dwin width (unit: pix).
    MS_U16 u16WebCamW;
    /// height (unit: line).
    MS_U16 u16WebCamH;
    
} DIP_WEBCAM_PROPERTY;

DIP_ERRCODE MDrv_DIP_GetChipCaps(EN_DIP_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);

void MDrv_DIP_Init(MS_U32 u32InputMode);
void MDrv_DIP_SetFrameInfo(MS_U32 u32FrameX, MS_U32 u32FrameY,
    MS_U32 u32FrameWidth, MS_U32 u32FrameHeight, MS_BOOL bInterLace);
void MDrv_DIP_InputMode(MS_U32 u32InputMode);
void MDrv_DIP_SetNRBuf(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_PHY u32BufStart, MS_PHY u32BufEnd);
void MDrv_DIP_SetDIBuf(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_PHY u32BufStart, MS_PHY u32BufEnd);
void MDrv_DIP_GetDIInfo(DI_INFO *pDIInfo);
void MDrv_DIP_GetNRInfo(NR_INFO *pNRInfo);
void MDrv_DIP_EnableNRDI(MS_BOOL bEnableNR, MS_BOOL bEnableSNR, MS_BOOL bEnableTNR, MS_BOOL bEnableDI);
void MDrv_DIP_GetDIBufCount(MS_U32 *pu32Count);
void MDrv_DIP_ClearDIBufStatus(MS_U8 u8Arg);
void MDrv_DIP_GetDIFrameCount(MS_U32 *pu32FrameCount);
void MDrv_DIP_GetDIBufStatus(MS_U32 *pu32DIBufStatus);
void MDrv_DIP_GetDIShow(void);
void MDrv_DIP_SetMMIOMapBase(void);
void MDrv_DIP_SetWebCamBuff(MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_VIRT u32BufStart, MS_VIRT u32BufEnd);
void MDrv_DIP_Trigger(void);
void MDrv_DIP_SetYUVOrder(DIP_YUV_ORDER dipOrder);
MS_BOOL MDrv_DIP_OneShot(MS_U32 u32BufWidth, MS_U32 u32BufHeight
    , MS_U32 u32InputBufStart, MS_U32 u32OutputBufStart, MS_U8 u8OutputBufCnt);
MS_BOOL MDrv_DIP_RegisterXCSetDIPInfo(DIP_ERRCODE (*fpDIP_CB)(MS_U16 cmd,MS_U32 *arg,MS_U16 size));

#ifdef __cplusplus
}
#endif

#endif
