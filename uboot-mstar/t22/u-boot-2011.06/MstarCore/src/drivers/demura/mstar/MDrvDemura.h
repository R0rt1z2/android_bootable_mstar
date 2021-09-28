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
#ifndef _MDRV_DeMura_H_
#define _MDRV_DeMura_H_

#define DBG_DEMURA                    1

#if (DBG_DEMURA == 1)
#define DEBUG_DEMURA(msg...)           UBOOT_DEBUG(msg)
#else
#define DEBUG_DEMURA(msg...)           
#endif

#define DEMURA_PACKET_SIZE(x)           (1 << (7 + x))


typedef struct
{
    // Offset: 0x00 ~ 0x03
    MS_U32 u32AllBinCRC;

    // Offset: 0x04 ~ 0x07
    MS_U32 u32HeaderCRC;

    // Offset: 0x08 ~ 0x0B
    MS_U32 u32HeaderSize;

    // Offset: 0x0C ~ 0x0F
    MS_U32 u32AllBinSize;

    // Offset: 0x10 ~ 0x1F
    MS_U8 u8DemuraID[16];

    // Offset: 0x20 ~ 0x21
    MS_U16 u16BinVersion;

    // Offset: 0x22
    MS_U8 u8LayerDataFomrat; // 1: IC Dram format
                          // 2: Compresses IC Dram format

    // Offset: 0x23
    MS_U8 bR_ch_Enable : 1;
    MS_U8 bG_ch_Enable : 1;
    MS_U8 bB_ch_Enable : 1;
    MS_U8 Reserved : 5;

    // Offset: 0x24 ~ 0x25
    MS_U16 u16HNodeCount;

    // Offset: 0x26 ~ 0x27
    MS_U16 u16VNodeCount;

    // Offset: 0x28
    MS_U8 u4StartLayer : 4;
    MS_U8 u4EndLayer   : 4;

    // Offset: 0x29
    MS_U8 bSeperateRGB : 1;
    MS_U8 dummy        : 7;

    // Offset: 0x2A ~ 0x2D
    MS_U32 u32LayerDataAddr;

    // Offset: 0x2E ~ 0x31:
    //  Layer data original (uncompressed) size in bytes.
    //  For layer data format 0 and 1, layer data original size is equal to layer data flash size.
    MS_U32 u32LayerDataOriSize;

    // Offset: 0x32 ~ 0x35:
    //  Size (in bytes) of layer data that stored on flash.
    //  If layer data format is 2 (compressed by 'gzip -9'), it keeps the size of compressed layer data.
    MS_U32 u32LayerDataFlashSize;

    // Offset: 0x36 ~ 0x39
    MS_U32 u32RegDataAddr;

    // Offset: 0x3A ~ 0x3D
    MS_U32 u32RegDataCount; // Count of register

    // Offset: 0x3E ~ 0x3F
    MS_U16 u16DLLVersion;

    // Offset: 0x40 ~ 0x43
    MS_U32 u32ProjectId;

    // Offset: 0x44 ~ 0x47 Customer data start address
    MS_U32 u32CustomerDataAddr;

    // Offset: 0x48 ~ 0x4B: Customer data size in bytes
    MS_U32 u32CustomerDataSize;

    MS_U32   nDate;           // 0x4C~0x4F
    MS_U8    nHBlockSize;     // 0x50
    MS_U8    nVBlockSize;     // 0x51
    MS_U8    nPacketSize;     // 0x52
}  __attribute__((__packed__)) DeMuraBinHeader;


typedef enum
{
    E_DEMURA_TOOL_FORMAT          = 0,  // tool format
    E_DEMURA_UNCOMPRESS_FORMAT    = 1,  // uncompress format
    E_DEMURA_COMPRESS_FORMAT      = 2,  // compress format
} DEMURA_LAYER_FORMAT;

typedef enum
{
    E_DEMURA_RGB_PANEL            = 0,
    E_DEMURA_RGBW_PANEL           = 1,
} DEMURA_PANEL_TYPE;


typedef enum
{
    AUTO_DL_TRIG_MODE             = 0,  // Download One time
    AUTO_DL_ENABLE_MODE           = 1,  // Repeated download data
}AUTODOWNLOAD_TRIG_e;


MS_BOOL MDrv_DEMURA_SetIOMapBase(void);
MS_BOOL MDrv_DEMURA_Check_HeaderCRC(MS_U8 *pBinBuf);
MS_BOOL MDrv_DEMURA_Check_AllBinCRC(MS_U8 *pBinBuf);
MS_BOOL MDrv_DEMURA_On_Resume(DeMuraBinHeader *pHeader);

MS_BOOL MDrv_DEMURA_Is_Support(DeMuraBinHeader *pHdr, PanelType* panel_data);
void MDrv_DEMURA_Dump_BinInfo(DeMuraBinHeader *pHdr);
MS_BOOL MDrv_DEMURA_HandleData(DeMuraBinHeader* pHdr, MS_U8 *dst_start, MS_U8 *src_addr);
MS_BOOL MDrv_DEMURA_RegisterTableLoad(DeMuraBinHeader* pHdr, MS_U32 RegDataBuf);
void MDrv_DEMURA_AutoDownload_Setup(DeMuraBinHeader* pHdr, MS_U32 pLayerDataAddr, MS_BOOL bEnable);

MS_U32 MDrv_DEMURA_DMA_Addr_Unit(void);
void MDrv_DEMURA_TurnOn(MS_BOOL bOnOff);

extern int zunzip(void *dst, int dstlen, unsigned char *src, unsigned long *lenp, int stoponerr, int offset);

#endif
