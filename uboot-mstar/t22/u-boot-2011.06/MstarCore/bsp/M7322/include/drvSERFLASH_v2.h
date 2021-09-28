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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SERFLASH_V2_H_
#define _DRV_SERFLASH_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_SERFLASH_GetInfo,
    MDrv_CMD_SERFLASH_GetLibVer,
    MDrv_CMD_SERFLASH_GetStatus,
    MDrv_CMD_SERFLASH_GetDbgLevel,
    MDrv_CMD_SERFLASH_SetDbgLevel,
    MDrv_CMD_SERFLASH_SetWPInfo,
    MDrv_CMD_SERFLASH_SetMcuCSCallBack,
    MDrv_CMD_SERFLASH_SetFlashWPCallBack,
    MDrv_CMD_SERFLASH_DetectType,
    MDrv_CMD_SERFLASH_DetectSize,
    MDrv_CMD_SERFLASH_Set2XRead,
    MDrv_CMD_SERFLASH_SetCKG,
    MDrv_CMD_SERFLASH_ClkDiv,
    MDrv_CMD_SERFLASH_SetMode,
    MDrv_CMD_SERFLASH_ReadUID,
    MDrv_CMD_SERFLASH_ChipSelect,
    MDrv_CMD_SERFLASH_Init,
    MDrv_CMD_SERFLASH_ReadID,
    MDrv_CMD_SERFLASH_Read,
    MDrv_CMD_SERFLASH_EraseChip,
    MDrv_CMD_SERFLASH_AddressToBlock,
    MDrv_CMD_SERFLASH_BlockToAddress,
    MDrv_CMD_SERFLASH_AddressErase,
    MDrv_CMD_SERFLASH_BlockErase,
    MDrv_CMD_SERFLASH_SectorErase,
    MDrv_CMD_SERFLASH_CheckWriteDone,
    MDrv_CMD_SERFLASH_Write,
    MDrv_CMD_SERFLASH_DMA,
    MDrv_CMD_SERFLASH_WriteProtect,
    MDrv_CMD_SERFLASH_WriteProtect_Enable_All_Range,
    MDrv_CMD_SERFLASH_WriteProtect_Disable_All_Range,
    MDrv_CMD_SERFLASH_WriteProtect_Disable_Range_Set,
    MDrv_CMD_SERFLASH_WriteProtect_Area,
    MDrv_CMD_SERFLASH_ReadStatusRegister,
    MDrv_CMD_SERFLASH_ReadStatusRegister2,
    MDrv_CMD_SERFLASH_WriteStatusRegister,
    MDrv_CMD_SERFLASH_CopyHnd,
    MDrv_CMD_SERFLASH_SetGPIO,
} eSerflashIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _SERFLASH_READID
{
    MS_U8 *pu8FlashID;
    MS_U32 u32IDSize;
}SERFLASH_READID, *PSERFLASH_READID;

typedef struct _SERFLASH_READ
{
    MS_U32 u32FlashAddr;
    MS_U32 u32FlashSize;
    MS_U8 *user_buffer;
}SERFLASH_READ, *PSERFLASH_READ;

typedef struct _SERFLASH_ADDRTOBLK
{
    MS_U32 u32FlashAddr;
    MS_U32 *pu32BlockIndex;
}SERFLASH_ADDRTOBLK, *PSERFLASH_ADDRTOBLK;

typedef struct _SERFLASH_BLKTOADDR
{
    MS_U32 u32BlockIndex;
    MS_U32 *pu32FlashAddr;
}SERFLASH_BLKTOADDR, *PSERFLASH_BLKTOADDR;

typedef struct _SERFLASH_ADDRERASE
{
    MS_U32 u32StartAddr;
    MS_U32 u32EraseSize;
    MS_BOOL bWait;
}SERFLASH_ADDRERASE, *PSERFLASH_ADDRERASE;

typedef struct _SERFLASH_BLKERASE
{
    MS_U32 u32StartBlock;
    MS_U32 u32EndBlock;
    MS_BOOL bWait;
}SERFLASH_BLKERASE, *PSERFLASH_BLKERASE;

typedef struct _SERFLASH_SECERASE
{
    MS_U32 u32StartAddr;
    MS_U32 u32EndAddr;
}SERFLASH_SECERASE, *PSERFLASH_SECERASE;

typedef struct _SERFLASH_WRITE
{
    MS_U32 u32FlashAddr;
    MS_U32 u32FlashSize;
    MS_U8 *user_buffer;
}SERFLASH_WRITE, *PSERFLASH_WRITE;

typedef struct _SERFLASH_DMA
{
    MS_U32 u32FlashStart;
    MS_U32 u32DRAMStart;
    MS_U32 u32Size;
}SERFLASH_DMA, *PSERFLASH_DMA;

typedef struct _SERFLASH_WP
{
    MS_BOOL bEnable;
}SERFLASH_WP, *PSERFLASH_WP;

typedef struct _SERFLASH_WPDISABLERANGESET
{
    MS_U32 u32DisableLowerBound;
    MS_U32 u32DisableSize;
}SERFLASH_WPDISABLERANGESET, *PSERFLASH_WPDISABLERANGESET;

typedef struct _SERFLASH_WPAREA
{
    MS_BOOL bEnableAllArea;
    MS_U8 u8BlockProtectBits;
}SERFLASH_WPAREA, *PSERFLASH_WPAREA;

typedef struct _SERFLASH_COPYHND
{
    MS_PHYADDR u32FlashAddr;
    MS_PHYADDR u32DstAddr;
    MS_U32 u32Len;
    SPIDMA_Dev eDstDev;
    MS_U8 u8OpCfg;
}SERFLASH_COPYHND, *PSERFLASH_COPYHND;

typedef struct _SERFLASH_GETSTATUS
{
    SERFLASH_DrvStatus* pDrvStatus;
}SERFLASH_GETSTATUS, *PSERFLASH_GETSTATUS;

typedef struct _SERFLASH_GETLIBVER
{
    const MSIF_Version **ppVersion;
}SERFLASH_GETLIBVER, *PSERFLASH_GETLIBVER;

typedef struct _SERFLASH_GETDBGLEVEL
{
    MS_U8 *u8DbgLevel;
}SERFLASH_GETDBGLEVEL, *PSERFLASH_GETDBGLEVEL;

typedef struct _SERFLASH_SETDBGLEVEL
{
    MS_U8 u8DbgLevel;
}SERFLASH_SETDBGLEVEL, *PSERFLASH_SETDBGLEVEL;

typedef struct _SERFLASH_SETWPINFO
{
    MS_BOOL bWPInfo;
}SERFLASH_SETWPINFO, *PSERFLASH_SETWPINFO;

typedef struct _SERFLASH_SETMCUCSCB
{
    ms_Mcu_ChipSelect_CB ChipSel_cb;
}SERFLASH_SETMCUCSCB, *PSERFLASH_SETMCUCSCB;

typedef struct _SERFLASH_SETFLASHWPCB
{
    ms_Flash_SetHWWP_CB FlashWP_cb;
}SERFLASH_SETFLASHWPCB, *PSERFLASH_SETFLASHWPCB;

typedef struct _SERFLASH_DETECTSIZE
{
    MS_U32 *u32FlashSize;
}SERFLASH_DETECTSIZE, *PSERFLASH_DETECTSIZE;

typedef struct _SERFLASH_SET2XREAD
{
    MS_BOOL b2XMode;
}SERFLASH_SET2XREAD, *PSERFLASH_SET2XREAD;

typedef struct _SERFLASH_SETCKG
{
    SPI_DrvCKG eCKGspi;
}SERFLASH_SETCKG, *PSERFLASH_SETCKG;

typedef struct _SERFLASH_CLKDIV
{
    SPI_DrvClkDiv eClkDivspi;
}SERFLASH_CLKDIV, *PSERFLASH_CLKDIV;

typedef struct _SERFLASH_SETMODE
{
    MS_BOOL bXiuRiu;
}SERFLASH_SETMODE, *PSERFLASH_SETMODE;

typedef struct _SERFLASH_CHIPSELECT
{
    MS_U8 u8FlashIndex;
}SERFLASH_CHIPSELECT, *PSERFLASH_CHIPSELECT;

typedef struct _SERFLASH_READSTATUSREG
{
    MS_U8 *pu8StatusReg;
}SERFLASH_READSTATUSREG, *PSERFLASH_READSTATUSREG;

typedef struct _SERFLASH_READSTATUSREG2
{
    MS_U8 *pu8StatusReg;
}SERFLASH_READSTATUSREG2, *PSERFLASH_READSTATUSREG2;

typedef struct _SERFLASH_WRITESTATUSREG
{
    MS_U16 u16StatusReg;
}SERFLASH_WRITESTATUSREG, *PSERFLASH_WRITESTATUSREG;

typedef struct _SERFLASH_SETGPIO
{
    MS_BOOL bSwitch;
}SERFLASH_SETGPIO, *PSERFLASH_SETGPIO;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_SERFLASH_V2_H_

