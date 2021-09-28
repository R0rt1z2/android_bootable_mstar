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
#include <command.h>
#include <common.h>
#include <stdio.h>
#include <linux/string.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <dmalloc.h>
#include <ms_utils.h>
#include <demura.h>
#include <crc_libs.h>
#include <demura_config.h>
#include <CommonDataType.h>
#include <convert_entry.h>

#include <apiPNL.h>
#include <MDrvDemura.h>
#include <halDemura.h>
#include "vendor.h"
#include "parse_fcic.h"


MS_BOOL Decode_To_Mstar_Format(void *pDataInfo, BinOutputInfo *pbin_info)
{
    MS_BOOL bRet;

#ifndef CONFIG_DEMURA_LGD_DEMO
    bRet = parse_LG_bin(pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("parse_LG_bin error\n");
        return FALSE;
    }

    // Decode Vendor data
    bRet = set_u13_interface(pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("set_u13_interface error\n");
        return FALSE;
    }
#else
    bRet = transfer_LGD_Data_1tab(pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("transfer_LGD_Data_1tab error\n");
        return FALSE;
    }
#endif
    
    MS_BOOL demura_enable[3]  = {TRUE, TRUE, TRUE};
    
    pbin_info->Build_Date     =  0x18032315;  // 2018.03.23
    pbin_info->reg_base_addr  =  0x7700;
    pbin_info->project_id     =  ((MSTAR_CHIP_ID << 16) + (1ULL << 15) + ID_CUS_LGD);
    pbin_info->data_type      =  IC_DRAM;
    pbin_info->Gain_type      =  10;
    pbin_info->Vendor_ID      =  EN_DEMURA_ID_LGD;
    
    memcpy(pbin_info->channel_enable, demura_enable, sizeof(demura_enable));
    return TRUE;
}

