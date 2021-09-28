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
#include <MsTypes.h>
#include <apiPNL.h>
#include <panel/panel.h>
#include <ShareType.h>
#include <MsOS.h>
#include <MsBoot.h>
#include <MsSystem.h>
#include <MsStr.h>
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsMmap.h>
#include <miu/MsDrvMiu.h>
#include "demura_config.h"
#include "MsApiDemura.h"


#undef ALIGN
#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))


MS_BOOL MApi_FCIC_Init(void);
MS_BOOL MApi_FCIC_Enable(MS_BOOL bEnable);

MS_BOOL MApi_MsDemura_Init(PanelType* panel_data);
MS_BOOL MApi_MsDemura_Enable(MS_BOOL bOnOff);

// Global value;
static demura_buf_desc *pbuf_desc = NULL;


static void init_dbuf_desc(MS_U64 mmap_start, MS_U32 mmap_len)
{
    MS_U32 u32_mmap_addr  = mmap_start;
    pbuf_desc  = (demura_buf_desc *)u32_mmap_addr;

    if (check_str_resume() == 0)
    {
        // AC ON
        memset(pbuf_desc, 0, DEMURA_BUFFER_DESC_SIZE);
        pbuf_desc->mmap_phy_addr  =  mmap_start;
        pbuf_desc->mmap_length    =  mmap_len;
        pbuf_desc->next_bin_addr  =  mmap_start + DEMURA_BUFFER_DESC_SIZE;
        pbuf_desc->left_space     =  mmap_len   - DEMURA_BUFFER_DESC_SIZE;
    }
}


static void enable_dbuf_desc(void)
{
    memcpy(pbuf_desc->magic_str, DEMURA_BDM_STRING, strlen(DEMURA_BDM_STRING));
}


MS_BOOL push_demura_bin(demura_bin_desc *pbin)
{
    MS_U32 cur_num     =  pbuf_desc->bin_cur_num;
    MS_U32 start_addr  =  pbuf_desc->next_bin_addr;
    MS_U32 len_align   =  ALIGN(pbin->length, DEMURA_BIN_ALIGN_UNIT);

    if (cur_num >= DEMURA_BIN_MAX_NUM)
    {
        UBOOT_ERROR("cur_num(%d) >= DEMURA_BIN_MAX_NUM(%d)", (int)cur_num, DEMURA_BIN_MAX_NUM);
        return FALSE;
    }

    if (pbin->length == 0)
    {
        UBOOT_ERROR("pbin->length = 0\n");
        return FALSE;
    }
    if (len_align > pbuf_desc->left_space)
    {
        UBOOT_ERROR("len_align(0x%x) > pbuf_desc->left_space(0x%x)", \
                    (uint)len_align, (uint)pbuf_desc->left_space);
        return FALSE;
    }

    pbuf_desc->next_bin_addr +=  len_align;
    pbuf_desc->left_space    -=  len_align;
    pbin->phy_addr   =  start_addr;
    pbin->len_align  =  len_align;
    UBOOT_DEBUG("pbin->phy_addr = 0x%x\n", (uint)start_addr);

    memcpy(&(pbuf_desc->bdesc[cur_num]), pbin, sizeof(*pbin));
    pbuf_desc->bin_cur_num ++;

    return TRUE;
}


void pop_demura_bin()
{
    MS_U32 cur_num, len_align;
    demura_bin_desc *pbin;
    
    cur_num = pbuf_desc->bin_cur_num;
    if (cur_num <= 0)
    {
        return;
    }
    pbin      = &(pbuf_desc->bdesc[cur_num-1]);
    len_align = pbin->len_align;

    pbuf_desc->next_bin_addr -=  len_align;
    pbuf_desc->left_space    +=  len_align;

    memset(pbin, 0, sizeof(*pbin));
    pbuf_desc->bin_cur_num --;
}


MS_U64 add_dbin_buf(const char *ip_string, MS_U32 filesize, MS_U32 id_num)
{
    MS_U64  bus_addr;
    MS_U32  final_size;
    demura_bin_desc bin_desc;

    final_size = ALIGN(filesize, DEMURA_BIN_ALIGN_UNIT);
    if (strlen(ip_string) >= sizeof(bin_desc.ip_string))
    {
        UBOOT_ERROR("length of ip_string(%s) > %d", ip_string, sizeof(bin_desc.ip_string));
        return 0x00;
    }

    memset(&bin_desc, 0, sizeof(bin_desc));
    memcpy(bin_desc.ip_string, ip_string, strlen(ip_string));
    bin_desc.length  = final_size;
    bin_desc.id_num  = id_num;

    if (push_demura_bin(&bin_desc) != TRUE)
    {
        UBOOT_ERROR("push_demura_bin Error!\n");
        return 0x00;
    }

    #ifdef CONFIG_MBOOT
    bus_addr = bin_desc.phy_addr;
    #else
    bus_addr = bin_desc.virt_addr;
    #endif
    return bus_addr;
}



demura_bin_desc *get_demura_bin(char *ip_string, MS_U32 id_num)
{
    MS_U32 i = 0;
    MS_U32 num = pbuf_desc->bin_cur_num;
    demura_bin_desc *pbin = NULL;

    for (i = 0; i < num; i++)
    {
        pbin = &(pbuf_desc->bdesc[i]);
        if (strcmp(pbin->ip_string, ip_string) == 0)
        {
            if (pbin->id_num == id_num)
            {
                break;
            }
        }
    }
    if (i == num)
    {
        pbin = NULL;
    }
    return pbin;
}

MS_U32 get_dbuf_length(void)
{
    return pbuf_desc->mmap_length;
}


MS_BOOL MApi_Demura_Init(PanelType* panel_data)
{
    char *str_ip = NULL;
    MS_BOOL ms_ret = FALSE; 
    MS_BOOL fc_ret = FALSE;
    MS_U32 buf_phy_len  = 0;
    MS_PHYADDR phy_addr = 0;
    long init_start_time = 0;

    UBOOT_TRACE("IN\n");
    init_start_time = MsSystemGetBootTime();
    str_ip = getenv(ENV_DEMURA_IP);
    UBOOT_DEBUG("ENV_DEMURA_IP = %s\n", str_ip);

    if (get_addr_from_mmap(DEMURA_MMAP_ID, &phy_addr) == -1)
    {
        UBOOT_ERROR("get addr from %s mmap fail\n",DEMURA_MMAP_ID);
        UBOOT_TRACE("OK\n");
        return FALSE;
    }
    if (get_length_from_mmap(DEMURA_MMAP_ID, (U32 *)&buf_phy_len) == -1)
    {
        UBOOT_ERROR("get length from %s mmap fail\n",DEMURA_MMAP_ID);
        UBOOT_TRACE("OK\n");
        return FALSE;
    }

    init_dbuf_desc(PA2BA(phy_addr), buf_phy_len);

    // Using api(push_demura_bin) to get load buffer
    #ifdef CONFIG_DEMURA_MSTAR
    UBOOT_DEBUG("Init Mstar-Demura IP\n");
    ms_ret = MApi_MsDemura_Init(panel_data);
    if (ms_ret == FALSE)
    {
        UBOOT_ERROR("MApi_MstarDemura_Init Failed\n");
    }
    #endif

    #ifdef CONFIG_DEMURA_FCIC
    UBOOT_DEBUG("Init FCIC-Demura IP\n");
    fc_ret = MApi_FCIC_Init();
    if (fc_ret == FALSE)
    {
        UBOOT_ERROR("MApi_FCIC_Init Failed\n");
    }
    #endif


    /**************************************/
    // After All IP have be inited !
    if ((ms_ret == TRUE) || (fc_ret == TRUE))
    {
        enable_dbuf_desc();
    }

    if (((str_ip == NULL) || (strcmp(str_ip, "mstar") == 0)) && (ms_ret == TRUE))
    {
    #ifdef CONFIG_DEMURA_MSTAR
        UBOOT_DEBUG("Enable Mstar-Demura IP\n");
        MApi_MsDemura_Enable(TRUE);
    #endif
    }
    else if ((strcmp(str_ip, "fcic") == 0)&& (fc_ret == TRUE))
    {
    #ifdef CONFIG_DEMURA_FCIC
        UBOOT_DEBUG("Enable FCIC-Demura IP\n");
        MApi_FCIC_Enable(TRUE);
    #endif
    }

    if ((ms_ret == TRUE) || (fc_ret == TRUE))
    {
        UBOOT_INFO("Demura_Init Cost : [%lu] ms\n", MsSystemGetBootTime() - init_start_time);
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}
