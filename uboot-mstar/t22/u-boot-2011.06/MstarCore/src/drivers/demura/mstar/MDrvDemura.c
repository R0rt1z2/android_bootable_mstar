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
#include <common.h>
#include <MsCommon.h>
#include <MsDebug.h>
#include <apiPNL.h>
#include <linux/string.h>
#include <ms_utils.h>
#include <MDrvDemura.h>
#include <halDemura.h>
#include <drvMMIO.h>
#include <halRegOp.h>
#include <crc_libs.h>

#ifdef ALIGN
#undef ALIGN
#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
#endif


MS_BOOL MDrv_DEMURA_SetIOMapBase(void)
{
    MS_VIRT  NonPM_Addr = 0;
    MS_PHY  NonPM_Size = 0;

    if (FALSE == MDrv_MMIO_GetBASE(&NonPM_Addr, &NonPM_Size, MS_MODULE_BDMA)) {  // Wait for utopia to fix it
        UBOOT_ERROR("Get IOMAP Base faill!\n");
        return FALSE;
    }
    UBOOT_DEBUG("Get IOMAP ID:%u Base:%lx!\n", MS_MODULE_BDMA, (ulong)NonPM_Addr);
    HAL_DEMURA_SetIOMapBase(NonPM_Addr);

    return TRUE;
}


MS_BOOL MDrv_DEMURA_Check_HeaderCRC(MS_U8 *pBinBuf)
{
    MS_U32 calHeaderCRC;
    DeMuraBinHeader *pHdr;

    UBOOT_TRACE("IN\n");
    pHdr = (DeMuraBinHeader *)pBinBuf;
    calHeaderCRC = MDrv_CRC32_Cal_DeMura(pBinBuf+8, sizeof(DeMuraBinHeader)-8);
    if(calHeaderCRC != pHdr->u32HeaderCRC)
    {
        UBOOT_DEBUG("Calculate HeaderCRC = 0x%x\n", (uint)calHeaderCRC);
        UBOOT_DEBUG("Binary    HeaderCRC = 0x%x\n", (uint)pHdr->u32HeaderCRC);
        return FALSE;
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}


MS_BOOL MDrv_DEMURA_Check_AllBinCRC(MS_U8 *pBinBuf)
{
    MS_U32 calAllBinCRC;
    DeMuraBinHeader *pHdr;

    UBOOT_TRACE("IN\n");
    pHdr= (DeMuraBinHeader *)pBinBuf;
    calAllBinCRC = MDrv_CRC32_Cal_DeMura((pBinBuf+4), (pHdr->u32AllBinSize - 4));
    if(calAllBinCRC != pHdr->u32AllBinCRC)
    {
        UBOOT_DEBUG("Calculate AllBinCRC = 0x%x\n", (uint)calAllBinCRC);
        UBOOT_DEBUG("Binary    AllBinCRC = 0x%x\n", (uint)pHdr->u32AllBinCRC);
        return FALSE;
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}


MS_BOOL MDrv_DEMURA_On_Resume(DeMuraBinHeader *pHeader)
{
    if (MDrv_DEMURA_Check_HeaderCRC((MS_U8 *)pHeader) == FALSE)
    {
        UBOOT_ERROR("MDrv_DEMURA_Check_HeaderCRC fail\n");
        return FALSE;
    }
    return TRUE;
}


MS_BOOL MDrv_DEMURA_Is_Support(DeMuraBinHeader *pHdr, PanelType* panel_data)
{
    MS_U16 Demura_LayerCount;

    DEBUG_DEMURA("IN\n");
    if(strncmp((const char *)&(pHdr->u8DemuraID),"mstar demura", 12) != 0)
    {
        DEBUG_DEMURA("Error, Demura ID compare fail.\n");
        DEBUG_DEMURA("Demura ID != 'mstar demura'\n");
        return FALSE;
    }
    if ( pHdr->u4StartLayer    > DEMURA_MAX_LAYER
         ||(pHdr->u4EndLayer   > DEMURA_MAX_LAYER)
         ||(pHdr->u4StartLayer > pHdr->u4EndLayer))
    {
        DEBUG_DEMURA("Error: Layer Counter Start(%d) and End(%d)  is invalid\n", pHdr->u4StartLayer
                     , pHdr->u4EndLayer);
        return FALSE;
    }
    Demura_LayerCount = (pHdr->u4EndLayer - pHdr->u4StartLayer + 1);
    DEBUG_DEMURA("Demura_LayerCount = 0x%x\n", Demura_LayerCount);

    if (HAL_DEMURA_Get_PanelWidth(pHdr) != panel_data->m_wPanelWidth)
    {
        DEBUG_DEMURA("panel_data->m_wPanelWidth = %d\n", panel_data->m_wPanelWidth);
        return FALSE;
    }
    if (HAL_DEMURA_Get_PanelHeight(pHdr) != panel_data->m_wPanelHeight)
    {
        DEBUG_DEMURA("panel_data->m_wPanelHeight = %d\n", panel_data->m_wPanelHeight);
        return FALSE;
    }

    DEBUG_DEMURA("OK\n");
    return TRUE;
}


void MDrv_DEMURA_Dump_BinInfo(DeMuraBinHeader *pHdr)
{
    char *pdate;
    MS_U8 i;
    extern EN_DEBUG_LEVEL dbgLevel;
    DEBUG_DEMURA("IN\n");

    DEBUG_DEMURA("AllBinCRC   = 0x%x\n",  (uint)pHdr->u32AllBinCRC);
    DEBUG_DEMURA("HeaderCRC   = 0x%x\n",  (uint)pHdr->u32HeaderCRC);
    DEBUG_DEMURA("HeaderSize  = 0x%x\n",  (uint)pHdr->u32HeaderSize);
    DEBUG_DEMURA("AllBinSize  = 0x%x\n",  (uint)pHdr->u32AllBinSize);
    DEBUG_DEMURA("DemuraID    = ");
    for(i = 0; i < sizeof(pHdr->u8DemuraID); i++)
    {
        #if (DBG_DEMURA == 1)
        if (dbgLevel & EN_DEBUG_LEVEL_DEBUG)
            printf("%02x ", pHdr->u8DemuraID[i]);
        #endif
    }
    if (dbgLevel & EN_DEBUG_LEVEL_DEBUG)
        printf("\n");
    
    DEBUG_DEMURA("BinVersion   = 0x%x\n",     (uint)pHdr->u16BinVersion);
    DEBUG_DEMURA("LayerDataFomrat = 0x%x\n",  (uint)pHdr->u8LayerDataFomrat);
    DEBUG_DEMURA("bR_ch_Enable = 0x%x\n",     (uint)pHdr->bR_ch_Enable);
    DEBUG_DEMURA("bG_ch_Enable = 0x%x\n",     (uint)pHdr->bG_ch_Enable);
    DEBUG_DEMURA("bB_ch_Enable = 0x%x\n",     (uint)pHdr->bB_ch_Enable);

    DEBUG_DEMURA("HNodeCount  = 0x%x\n",      (uint)pHdr->u16HNodeCount);
    DEBUG_DEMURA("VNodeCount  = 0x%x\n",      (uint)pHdr->u16VNodeCount);
    DEBUG_DEMURA("StartLayer  = 0x%x\n",      (uint)pHdr->u4StartLayer);
    DEBUG_DEMURA("EndLayer    = 0x%x\n",      (uint)pHdr->u4EndLayer);
    DEBUG_DEMURA("SeperateRGB = 0x%x\n",      (uint)pHdr->bSeperateRGB);

    DEBUG_DEMURA("LayerDataAddr    = 0x%x\n",    (uint)pHdr->u32LayerDataAddr);
    DEBUG_DEMURA("LayerDataOriSize = 0x%x\n",    (uint)pHdr->u32LayerDataOriSize);
    DEBUG_DEMURA("LayerDataFlashSize = 0x%x\n",  (uint)pHdr->u32LayerDataFlashSize);
    DEBUG_DEMURA("RegDataAddr      = 0x%x\n",    (uint)pHdr->u32RegDataAddr);
    DEBUG_DEMURA("RegDataCount     = 0x%x\n",    (uint)pHdr->u32RegDataCount);

    DEBUG_DEMURA("DLLVersion  = 0x%x\n",         (uint)pHdr->u16DLLVersion);
    DEBUG_DEMURA("ProjectId   = 0x%x\n",         (uint)pHdr->u32ProjectId);
    DEBUG_DEMURA("CustomerDataAddr = 0x%x\n",    (uint)pHdr->u32CustomerDataAddr);
    DEBUG_DEMURA("CustomerDataSize = 0x%x\n",    (uint)pHdr->u32CustomerDataSize);
    #if (DBG_DEMURA == 1)
    DEBUG_DEMURA("Demura Bin Build date : ");
    pdate = (char *)&(pHdr->nDate);
    if (dbgLevel & EN_DEBUG_LEVEL_DEBUG)
        printf("20%02x/%02x/%02x %02x:00\n", (uint)pdate[0], (uint)pdate[1], (uint)pdate[2], (uint)pdate[3]);
    #endif

    DEBUG_DEMURA("nHBlockSize = 0x%x\n", (uint) pHdr->nHBlockSize);
    DEBUG_DEMURA("nVBlockSize = 0x%x\n", (uint)pHdr->nVBlockSize);
    DEBUG_DEMURA("nPacketSize = 0x%x\n", (uint)pHdr->nPacketSize);

    DEBUG_DEMURA("OK\n\n");
}


MS_BOOL MDrv_DEMURA_HandleData(DeMuraBinHeader* pHdr, MS_U8 *dst_start, MS_U8 *src_addr)
{
    MS_U8  *src, *dst;
    MS_U32 dstlen;
    unsigned long lenp = (~0UL);
    MS_U32 ret, org_len;
    MS_U32 u32CalCRC, pad_num;
    DeMuraBinHeader *pNewHdr;

    DEBUG_DEMURA("IN\n");

    // For STR resuming
    //   Copy the header
    dst = dst_start;
    pNewHdr = (DeMuraBinHeader *)dst;
    memcpy(dst, src_addr, sizeof(DeMuraBinHeader));
    dst = dst + sizeof(DeMuraBinHeader);
    pad_num = ALIGN((size_t)dst, DEMURA_DMA_ADDR_UNIT) - (size_t)dst;
    if (pad_num != 0)
        memset(dst, 0, pad_num);
    dst = dst + pad_num;

    // Load layer data
    pNewHdr->u8LayerDataFomrat = E_DEMURA_UNCOMPRESS_FORMAT;
    pNewHdr->u32LayerDataAddr  = (size_t)dst - (size_t)dst_start;

    if (pHdr->u8LayerDataFomrat == E_DEMURA_COMPRESS_FORMAT)
    {
        MS_U32 distance = (dst_start >= src_addr) ? (dst_start - src_addr) : (src_addr - dst_start);

        if (distance < pHdr->u32AllBinSize)
        {
            DEBUG_DEMURA("For Compress data, dest(0x%x) - src(0x%x) < AllBinSize(0x%x)\n", 
                           (size_t)dst_start, (size_t)src_addr, (size_t)pHdr->u32AllBinSize);
            return FALSE;
        }

        dstlen = pHdr->u32LayerDataFlashSize - 6;    // 4byte  original filesize, 2byte magic value('0xBE', 0x'EF')
        src  = (MS_U8 *)(src_addr + pHdr->u32LayerDataAddr);
        org_len = *(MS_U32 *)(src + dstlen);

        DEBUG_DEMURA("dst = 0x%x, dstlen = 0x%x, src = 0x%x\n", (size_t)dst,(size_t)dstlen,(size_t)src);
        ret = zunzip((void *)dst, dstlen, src, &lenp, 1, 0);
        if (ret != 0)
        {
            DEBUG_DEMURA("zunzip failed!\n");
            return FALSE;
        }
        if ((org_len != lenp) || (lenp != pHdr->u32LayerDataOriSize))
        {
            DEBUG_DEMURA("origal_len = 0x%x, lenp = 0x%x\n", (size_t)org_len, (size_t)lenp);
            DEBUG_DEMURA("zunzip data abort\n");
            return FALSE;
        }
        DEBUG_DEMURA("zunzip done, lenp = 0x%x\n", (size_t)lenp);
    } 
    else if (pHdr->u8LayerDataFomrat == E_DEMURA_UNCOMPRESS_FORMAT)
    {
        memcpy(dst, (src_addr + pHdr->u32LayerDataAddr), pHdr->u32LayerDataOriSize);
    }
    dst = dst +  pHdr->u32LayerDataOriSize;
    pad_num = ALIGN((size_t)dst, DEMURA_DMA_ADDR_UNIT) - (size_t)dst;
    if (pad_num != 0)
        memset(dst, 0, pad_num);
    dst = dst + pad_num;

    //   Copy the register data
    memcpy(dst, (src_addr + pHdr->u32RegDataAddr), (pHdr->u32RegDataCount * 5));
    pNewHdr->u32RegDataAddr = (MS_U32)(dst - dst_start);
    dst = dst + pHdr->u32RegDataCount * 5;
    pad_num = ALIGN((size_t)dst, DEMURA_DMA_ADDR_UNIT) - (size_t)dst;
    if (pad_num != 0)
        memset(dst, 0, pad_num);
    dst = dst + pad_num;

    //   Copy the Customer data
    if (pHdr->u32CustomerDataAddr != 0x00)
    {
        memcpy(dst, (src_addr + pHdr->u32CustomerDataAddr), pHdr->u32CustomerDataSize);
        pNewHdr->u32CustomerDataAddr  = (MS_U32)(dst - dst_start);
        dst = dst + pHdr->u32CustomerDataSize;
    }
    pNewHdr->u32AllBinSize = (MS_U32)(dst - dst_start);

    //   Re generate the CRC of header
    u32CalCRC = MDrv_CRC32_Cal_DeMura((MS_U8*)pNewHdr + 8, sizeof(DeMuraBinHeader)-8);
    pNewHdr->u32HeaderCRC = u32CalCRC;
    memcpy((void *)pHdr, (void *)pNewHdr, sizeof(DeMuraBinHeader));  // Copy back the New Header

    MDrv_DEMURA_Dump_BinInfo(pHdr);
    DEBUG_DEMURA("OK\n");
    return TRUE;
}


MS_BOOL MDrv_DEMURA_RegisterTableLoad(DeMuraBinHeader* pHdr, MS_U32 RegDataBuf)
{
    MS_U32 u32Counter;
    MS_U32 u32RegisterAddress;
    MS_U8  u8Value;
    MS_U8  u8Mask;
    MS_U8 *pu8RegDataBuf = (MS_U8 *)RegDataBuf;

    DEBUG_DEMURA("IN\n");
    if((pHdr->u32RegDataCount == 0) || (pHdr->u32RegDataCount > 256))
    {
        DEBUG_DEMURA("Error: Demura Register Counter = %d, No register value needs to be loaded.\n", (int)pHdr->u32RegDataCount);
        return FALSE;
    }

    UBOOT_DUMP((unsigned int)pu8RegDataBuf, (pHdr->u32RegDataCount * 5));
    for (u32Counter = 0; u32Counter < pHdr->u32RegDataCount; u32Counter++)
    {
        // Littele Endian
        u32RegisterAddress = (pu8RegDataBuf[0]) + REG_DEMURA_BASE;
        u8Value = (pu8RegDataBuf[3]);
        u8Mask = (pu8RegDataBuf[4]);
        
        HAL_DEMURA_WriteByteMask(u32RegisterAddress, u8Value, u8Mask);
        pu8RegDataBuf += 5;
    }

    HAL_DEMURA_Dump_Layerlevel();
    DEBUG_DEMURA("OK\n");
    return TRUE;
}


void MDrv_DEMURA_AutoDownload_Setup(DeMuraBinHeader* pHdr, MS_U32 pLayerDataAddr, MS_BOOL bEnable)
{
    DEBUG_DEMURA("IN\n");

    if (bEnable == TRUE)
    {
        HAL_DEMURA_SetDL_BaseAddr(pLayerDataAddr);
        HAL_DEMURA_SetDL_SramIni_Addr(0);

        HAL_DEMURA_SetDL_Depth(pHdr->u16HNodeCount);
        HAL_DEMURA_SetDL_ReqLenth(DEMURA_DMA_REQ_LENGTH);
        HAL_DEMURA_SetDL_TriggerMode(AUTO_DL_ENABLE_MODE);
        HAL_DEMURA_Enable_DL(TRUE);
    }
    else
    {
        HAL_DEMURA_SetDL_TriggerMode(AUTO_DL_TRIG_MODE);
        HAL_DEMURA_Enable_DL(FALSE);
    }
    DEBUG_DEMURA("OK\n");
}

MS_U32 MDrv_DEMURA_DMA_Addr_Unit(void)
{
    return DEMURA_DMA_ADDR_UNIT;
}

void MDrv_DEMURA_TurnOn(MS_BOOL bOnOff)
{
    DEMURA_PANEL_TYPE ptype;
    DEBUG_DEMURA("IN\n");

    ptype = HAL_DEMURA_Get_PanelType();
    if (ptype == E_DEMURA_RGBW_PANEL)
    {
        UBOOT_INFO("Detected RGBW Panel\n");
    }

    HAL_DEMURA_EnableDemura(bOnOff, ptype);
    DEBUG_DEMURA("OK\n");
}

