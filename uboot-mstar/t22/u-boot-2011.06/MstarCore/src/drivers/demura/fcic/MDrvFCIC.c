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
#include <stdio.h>
#include <common.h>
#include <MsTypes.h>
#include <drvBDMA.h>
#include <MsSystem.h>
#include <miu/MsDrvMiu.h>
#include <MsDebug.h>
#include <halFCIC.h>
#include <drvMMIO.h>
#include <halRegOp.h>
#include "MDrvFCIC.h"


MS_BOOL MDrv_FCIC_SetIOMapBase(void)
{
    MS_VIRT NonPM_Addr = 0;
    MS_PHY  NonPM_Size = 0;

    if (FALSE == MDrv_MMIO_GetBASE(&NonPM_Addr, &NonPM_Size, MS_MODULE_BDMA)) {  // Wait for utopia to fix it
        UBOOT_ERROR("Get IOMAP Base faill!\n");
        return FALSE;
    }
    UBOOT_DEBUG("Get IOMAP ID:%u Base:%lx!\n", MS_MODULE_BDMA, (ulong)NonPM_Addr);
    HAL_DEMURA_SetIOMapBase(NonPM_Addr);

    return TRUE;
}


MS_BOOL MDrv_FCIC_Checksum(MS_U8 *lut_buf, MS_U32 lut_offset, MS_U32 lut_len)
{
    MS_U32 i = 0;
    MS_U32 totalsum = 0;
    MS_U32 check_start = lut_offset;
    MS_U32 check_end   = lut_len;

    unsigned char *pLut = (unsigned char *)lut_buf;

    for (i = check_start; i < check_end; i++)
    {
        totalsum = totalsum + pLut[i];
        totalsum = totalsum % 256;
    }
    UBOOT_DEBUG("totalsum = 0x%x\n", (uint)totalsum);
    UBOOT_DEBUG("checksum ptr = 0x%p\n", pLut+i);

    unsigned char checksum = pLut[i];
    unsigned char calcusum = (unsigned char)(-1 * totalsum);    // Two's complement, don't care "+/-"

    if (calcusum != checksum)
    {
        UBOOT_ERROR("calcusum = 0x%x\n", calcusum);
        UBOOT_ERROR("checksum = 0x%x\n", checksum);
        return FALSE;
    }
    return TRUE;
}


MS_BOOL MDrv_FCIC_Set_BDMA(MS_PHYADDR lut_phyAddr, MS_U32 lut_size)
{
    MS_PHYADDR src_addr, dst_addr;
    BDMA_CpyType eCpyType;
    BDMA_Result  bRet;
    EN_MIUID miu_no;

    // MDrv_BDMA_Init(0);  MstarSysInit() has call it !

    miu_no = MsApi_Miu_GetID(lut_phyAddr);

    if (miu_no == E_MIU_ID_0)
    {
        eCpyType = E_BDMA_SDRAM2FCIC;
    }
    else if (miu_no == E_MIU_ID_1)
    {
        eCpyType = E_BDMA_SDRAM12FCIC;
    }
    else
    {
        eCpyType = E_BDMA_SDRAM22FCIC;
    }
    src_addr = lut_phyAddr;

    dst_addr = FCIC_SRAM_START_ADDR;

    UBOOT_DEBUG("src_addr = 0x%x, dst_addr = 0x%x, lut_size = 0x%x, eCpyType = 0x%x\n", \
                (uint)src_addr, (uint)dst_addr, (uint)lut_size, eCpyType);

    bRet = MDrv_BDMA_CopyHnd(src_addr, dst_addr, lut_size, eCpyType, BDMA_OPCFG_DEF);
    if (bRet != E_BDMA_OK)
    {
        UBOOT_ERROR("MDrv_BDMA_CopyHnd Error, ret = %d\n", (int)bRet);
        return FALSE;
    }
    return TRUE;
}


void MDrv_FCIC_TurnOn(MS_BOOL bOnOff)
{
    HAL_FCIC_TurnOn(bOnOff);
}

