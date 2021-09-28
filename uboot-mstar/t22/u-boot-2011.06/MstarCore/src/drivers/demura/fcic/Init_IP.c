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
#include <apiPNL.h>
#include <drvBDMA.h>
#include <MsSystem.h>
#include <MsStr.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
#include <miu/MsDrvMiu.h>
#include <halFCIC.h>
#include <ms_utils.h>
#include "MDrvFCIC.h"
#include "MsApiDemura.h"

//#define DBG_WITHOUT_TCON    1

#undef ALIGN
#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))

static FCICBinHeader *pHeader = NULL;


#ifdef DBG_WITHOUT_TCON
static int fcic_reg_table[][2] = {
    {0x302f00, 0x07},    //0x302f_sub07 bank  == Equal ==> 0x3307
    {0x302f01, 0x00 },
    /* {              }, */
    {0x302f02, 0x00 },
    {0x302f03, 0x00 },
    {0x302f04, 0x00 },
    {0x302f05, 0x00 },
    {0x302f06, 0x00 },
    {0x302f07, 0x00 },
    {0x302f08, 0x00 },
    {0x302f09, 0x00 },
    {0x302f0a, 0x00 },
    {0x302f0b, 0x00 },
    {0x302f0c, 0x00 },
    {0x302f0d, 0x00 },
    {0x302f0e, 0x10 },
    {0x302f0f, 0x20 },
    {0x302f10, 0x40 },
    {0x302f11, 0x80 },
    {0x302f12, 0xC0 },
    {0x302f13, 0xFF },
    {0x302f14, 0x00 },
    {0x302f15, 0x00 },
    {0x302f16, 0x00 },
    {0x302f17, 0x00 },
    {0x302f18, 0x00 },
    {0x302f19, 0x00 },
    {0x302f1a, 0x00 },
    {0x302f1b, 0x00 },
    {0x302f1c, 0x00 },
    {0x302f1d, 0x00 },
    {0x302f1e, 0x00 },
    {0x302f1f, 0x00 },
    {0x302f20, 0x00 },
    {0x302f21, 0x80 },
    {0x302f22, 0x70 },
    {0x302f23, 0x00 },
    {0x302f24, 0x00 },
    {0x302f25, 0x00 },
    {0x302f26, 0x00 },
    {0x302f27, 0x00 },
    {0x302f28, 0x00 },
    {0x302f29, 0x00 },
    {0x302f2a, 0x00 },
    {0x302f2b, 0x00 },
    {0x302f2c, 0x00 },
    {0x302f2d, 0x00 },
    {0x302f2e, 0x00 },
    {0x302f2f, 0x00 },
    {0x302f30, 0x00 },
    {0x302f31, 0x00 },
    {0x302f32, 0x00 },
    {0x302f33, 0x00 },
    /*{      ,     }, */

    {0x302f34, 0x01},
    {0x302fa8, 0x04},
    {0x302fa9, 0x00},
};

// should be set by TCON
static void FCIC_Load_Tcon_RegVal(void)
{
    uint i = 0;
    printf("Using Emebbed FCIC Register table\n");
    for (i = 0; i < ARRAY_SIZE(fcic_reg_table); i++)
    {
        MS_U32 reg = fcic_reg_table[i][0];
        MS_U8  val = fcic_reg_table[i][1];
        WriteByte(reg, val);
    }
    Write2Byte(0x302FA4, 0x217B);  // Flash Data Num
    Write2Byte(0x302FA0, 0x0001);  // Flash Base Addr
}
#endif


static void FCIC_Gen_BinHeader(FCICBinHeader *phdr)
{
    MS_U16  offset, dat_num;
    MS_U32  dat_len;

    UBOOT_TRACE("In\n");
    offset  = HAL_FCIC_Read_Reg(REG_FCIC_FLASH_BASE_ADDR);
    dat_num = HAL_FCIC_Read_Reg(REG_FCIC_FLASH_DATA_NUM);
    dat_len = (offset + dat_num) * FCIC_DATA_UNIT;

    memset(phdr, 0, sizeof(*phdr));
    phdr->LutFlashAddr = sizeof(*phdr);
    phdr->VendorIdSize = offset * FCIC_DATA_UNIT;
    phdr->LutSize      = dat_len;
    phdr->FileSize     = dat_len + FCIC_EXTRA_SIZE;

    UBOOT_DEBUG("phdr->LutFlashAddr = 0x%x\n", (uint)phdr->LutFlashAddr);
    UBOOT_DEBUG("phdr->VendorIdSize = 0x%x\n", (uint)phdr->VendorIdSize);
    UBOOT_DEBUG("phdr->LutSize      = 0x%x\n", (uint)phdr->LutSize);
    UBOOT_DEBUG("phdr->FileSize     = 0x%x\n", (uint)phdr->FileSize);
    UBOOT_TRACE("OK\n");
}


static MS_U32 FCIC_AC_Init(void)
{
    MS_U8   *bin_buf, *ptr;
    MS_U32  alloc_size;
    FCICBinHeader header;

    if (init_spi_flash() != TRUE)
    {
        UBOOT_ERROR("init_spi_flash error\n");
        MDrv_FCIC_TurnOn(FALSE);
        return 0x00;
    }
    FCIC_Gen_BinHeader(&header);

    alloc_size = header.LutSize + sizeof(*pHeader);

    bin_buf = (MS_U8 *)(size_t)add_dbin_buf(DEMURA_FCIC_IP, alloc_size, 0);
    if (bin_buf == NULL)
    {
        UBOOT_ERROR("add_dbin_buf Error!\n");
        return 0x00;
    }
    memcpy(bin_buf, &header, sizeof(header));
    ptr = bin_buf + sizeof(header);

    if (read_spi_flash(ptr, FCIC_DATA_START, header.FileSize) != TRUE)
    {
        UBOOT_ERROR("read_spi_flash error\n");
        pop_demura_bin();
        MDrv_FCIC_TurnOn(FALSE);
        return 0x00;
    }

    return (MS_U32)bin_buf;
}


static MS_U64 FCIC_STR_Init(void)
{
    MS_U32  bus_addr;
    demura_bin_desc *pbin;

    pbin = get_demura_bin(DEMURA_FCIC_IP, 0);
    if (pbin == NULL)
    {
        UBOOT_ERROR("On STR resuming, Can no find FCIC demura_bin_desc!\n");
        return 0x00;
    }
    #ifdef CONFIG_MBOOT
    bus_addr = pbin->phy_addr;
    #else
    bus_addr = pbin->virt_addr;
    #endif

    return bus_addr;
}


MS_BOOL MApi_FCIC_Init(void)
{
    MS_U64 bus_addr;
    MS_U32 lut_virtaddr;
    MS_U32 lut_phyaddr;

    if (MDrv_FCIC_SetIOMapBase() != TRUE)
    {
        UBOOT_ERROR("MDrv_FCIC_SetIOMapBase Failed\n");
        MDrv_FCIC_TurnOn(FALSE);
        return FALSE;
    }

    #ifdef DBG_WITHOUT_TCON
    FCIC_Load_Tcon_RegVal();
    #endif

    if (check_str_resume() == 1)
    {
        bus_addr = FCIC_STR_Init();
        if (bus_addr == 0)
        {
            UBOOT_ERROR("FCIC_STR_Init Error\n");
            return FALSE;
        }
    }
    else
    {
        bus_addr = FCIC_AC_Init();
        if (bus_addr == 0)
        {
            UBOOT_ERROR("FCIC_AC_Init Error\n");
            return FALSE;
        }
    }
    pHeader = (FCICBinHeader *)((size_t)bus_addr);

    lut_virtaddr = (MS_U32)(bus_addr + pHeader->LutFlashAddr);
    lut_phyaddr  = BA2PA(lut_virtaddr);

    if (MDrv_FCIC_Checksum((MS_U8 *)lut_virtaddr, pHeader->VendorIdSize, pHeader->LutSize) != TRUE)
    {
        MDrv_FCIC_TurnOn(FALSE);
        UBOOT_ERROR("MDrv_FCIC_Checksum error\n");
        pHeader = NULL;
        return FALSE;
    }

    MDrv_FCIC_TurnOn(TRUE);
    if (MDrv_FCIC_Set_BDMA(lut_phyaddr, pHeader->LutSize) != TRUE)
    {
        UBOOT_ERROR("MDrv_FCIC_Set_BDMA error\n");
        MDrv_FCIC_TurnOn(FALSE);
        return FALSE;
    }

    MDrv_FCIC_TurnOn(FALSE);
    return TRUE;
}


MS_BOOL MApi_FCIC_Enable(MS_BOOL bEnable)
{
    if (pHeader == NULL)
    {
        UBOOT_ERROR("FCIC is not initlized !\n");
        return FALSE;
    }

    if (bEnable == TRUE)
    {
        MDrv_FCIC_TurnOn(TRUE);
    }
    else
    {
        MDrv_FCIC_TurnOn(FALSE);
    }
    return TRUE;
}

