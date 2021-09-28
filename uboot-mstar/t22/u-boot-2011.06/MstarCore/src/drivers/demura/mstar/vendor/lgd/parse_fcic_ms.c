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
#include "demura_config.h"

#undef ALIGN
#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))


MS_BOOL set_u13_interface(void *pdat_info, BinOutputInfo *pbin_info)
{
    //inital data
    int n_Hnode     = pbin_info->HNode;
    int n_Vnode     = pbin_info->VNode;
    int level_count = pbin_info->LevelCount;
    //int n_Layer     = 8;
    interface_info *pDataInfo = (interface_info *)pdat_info;

    pDataInfo->reg_dmc_plane_num              = level_count;     //  4 bit
    pDataInfo->reg_dmc_h_block                = 0x3;             //  3 bit
    pDataInfo->reg_dmc_v_block                = 0x3;             //  3 bit
    pDataInfo->reg_dmc_rgb_mode               = 0x0;             //  1 bit
    pDataInfo->reg_dmc_panel_h_size           = 0xF00;           // 13 bit
    pDataInfo->reg_dmc_black_limit            = 0x000;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level1           = 0x200;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level2           = 0x400;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level3           = 0x600;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level4           = 0xC00;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level5           = 0;               // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level6           = 0;               // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level7           = 0;               // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level8           = 0;               // 12 bit , format 10.2
    pDataInfo->reg_dmc_white_limit            = 0xFFF;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_dither_en              = 0x1;             //  2 bit , 0 : rounding , 1 : dither , 2~3 : truncate
    pDataInfo->reg_dmc_dither_rst_e_4_frame   = 0x0;             //  1 bit
    pDataInfo->reg_dmc_dither_pse_rst_num     = 0x0;             //  2 bit

    // Set if Sep_type
    pbin_info->Sep_type                       = pDataInfo->reg_dmc_rgb_mode;

    int idx;
    for (idx = 0; idx < level_count; idx++)
    {
        //get level
        int nLayer = 0;
        switch (idx)
        {
            case 0:
                nLayer = pDataInfo->reg_dmc_plane_level1 >> 2;
                break;
            case 1:
                nLayer = pDataInfo->reg_dmc_plane_level2 >> 2;
                break;
            case 2:
                nLayer = pDataInfo->reg_dmc_plane_level3 >> 2;
                break;
            case 3:
                nLayer = pDataInfo->reg_dmc_plane_level4 >> 2;
                break;
            default:
                nLayer = 0xFF;
                break;
        }

        int i, j;
        for (j = 0; j < n_Vnode; j++)
        {
            for (i = 0; i < n_Hnode; i++)
            {                
                //red
                int sign = 0;
                int value = pDataInfo->Lut_in[idx][j * n_Hnode + i].r & 0x7f;
                if ((pDataInfo->Lut_in[idx][j * n_Hnode + i].r & 0x80) == 0x80)
                    sign = -1;
                else
                    sign = 1;

                pDataInfo->Lut_in[idx][j * n_Hnode + i].dbr = (sign * value) + nLayer;
                pDataInfo->Lut_in[idx][j * n_Hnode + i].r   = (sign * value) + nLayer;
                pDataInfo->Lut_in[idx][j * n_Hnode + i].dbg = (sign * value) + nLayer;
                pDataInfo->Lut_in[idx][j * n_Hnode + i].g   = (sign * value) + nLayer;
                pDataInfo->Lut_in[idx][j * n_Hnode + i].dbb = (sign * value) + nLayer;
                pDataInfo->Lut_in[idx][j * n_Hnode + i].b   = (sign * value) + nLayer;
            }
        }
    }
    return TRUE;
}


MS_BOOL Load_LGD_Bin(char **pbuf, int flash_start_addr, int bin_size)
{
    MS_BOOL  bRet;
    char *buffer;
    MS_U32 fsize;

    int start   = flash_start_addr;

    fsize = ALIGN(bin_size, 32);
    buffer = (char *)dmalloc(fsize + 32);
    CHECK_DMALLOC_SPACE(buffer, (uint)fsize + 32);

    // read binary file
    bRet = read_spi_flash((MS_U8 *)buffer, start, fsize);  // optimize,
    if (bRet == FALSE)
    {
        UBOOT_ERROR("read LGD Demura binary error\n");
        dfree(buffer);
        return NULL;
    }

    *pbuf = buffer;
    return TRUE;
}


MS_BOOL parse_LG_bin(void *pdat_info, BinOutputInfo *pbin_info)
{
    // These code waiting to be fix, should be parsed by header !
    pbin_info->HNode          = 481;
    pbin_info->VNode          = 271;
    pbin_info->LevelCount     = 4;
    pbin_info->Blk_h_size     = 8;
    pbin_info->Blk_v_size     = 8;
    
    char *fbuf      = NULL;
    int n_Hnode     = pbin_info->HNode;
    int n_Vnode     = pbin_info->VNode;
    int level_count = pbin_info->LevelCount;
    #if defined (CONFIG_DEMURA_URSA13)
    interface_info *pDataInfo = (interface_info *)pdat_info;
    #else
    #error "Unkown DEMURA_URSA_TYPE !"
    #endif

    UBOOT_DEBUG("Parsing LGD input binary file ...\n");
    MS_BOOL bRet = Load_LGD_Bin(&fbuf, LGD_MURA_START_ADR, (LGD_CHECKSUM_ADR - start));
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Load_LGD_Bin Error\n");
        return FALSE;
    }

    bRet = Alloc_LutIn_Space(pdat_info, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Alloc_LutIn_Space error\n");
        return FALSE;
    }

    int start_addr    = LGD_MURA_START_ADR;    // must be times of 32
    int data_str_addr = LGD_MURA_DAT_OFFSET;
    int checksum_addr = LGD_CHECKSUM_ADR;
    int total_length  = ALIGN(checksum_addr - start_addr, 32);

    //! Start Parsing
    int byte_address = 0;
    MS_BOOL end_flag = FALSE;
    unsigned char check_sum = 0;
    int total_sum = 0;
    int h_cnt = 0, v_cnt = 0, c_cnt = 0, t_cnt = 0;

    while (byte_address <= total_length)
    {
        char *line = fbuf + byte_address;

        int i;
        for (i = 0; i < 32; i++)
        {
            if (byte_address + i < data_str_addr) // Skip bulk detect bytes
                continue;

            //! End after get Checksum byte
            if (byte_address + i >= (checksum_addr - start_addr)) // Optimise
            {
                check_sum = line[i] & 0xff;
                UBOOT_DEBUG("checksum_addr = 0x%x, line[i] = 0x%x\n", byte_address + i, *(fbuf + byte_address + i));
                end_flag = TRUE;
                break;
            }

            if (c_cnt == 3)
            {
                pDataInfo->Lut_in[t_cnt][v_cnt * n_Hnode + h_cnt].r = line[i] & 0xff;  // + nLayer;
                pDataInfo->Lut_in[t_cnt][v_cnt * n_Hnode + h_cnt].g = line[i] & 0xff;  // + nLayer;
                pDataInfo->Lut_in[t_cnt][v_cnt * n_Hnode + h_cnt].b = line[i] & 0xff;  // + nLayer;
            }

            //! (total_sum % 256) to verify check_sum
            total_sum += (int)(line[i] & 0xff);
            total_sum %= 256;

            //! Add index of data matrix
            h_cnt++;
            if (h_cnt == n_Hnode)
            {
                h_cnt = 0;
                v_cnt++;
                if (v_cnt == n_Vnode)
                {
                    v_cnt = 0;
                    t_cnt++;
                    if (t_cnt == level_count)
                    {
                        t_cnt = 0;
                        c_cnt++;
                    }
                }
            }
        }
        if (end_flag == TRUE)
            break;
        byte_address += 32;
    }

    UBOOT_DEBUG("Checksum = %x\n", check_sum);
    UBOOT_DEBUG("2's complement(total_sum %% 256) = %x\n", (unsigned char)(-1 * total_sum));
    if ((unsigned char)(-1 * total_sum) == check_sum)
    {
        UBOOT_DEBUG("Checksum PASS !!!\n");
    }
    else
    {
        UBOOT_ERROR("Checksum FAIL !!!\n");
        dfree(fbuf);
        return FALSE;
    }

    // Sign spi flash data
    MS_U32 sample_crc32;
    char  strbuf[1024];
    
    sample_crc32 = MDrv_CRC32_Cal_DeMura((MS_U8 *)(fbuf+SAMPLE_DAT_START), SAMPLE_DAT_LEN);
    if (sample_crc32 == 0xffffFFFF)
    {
        UBOOT_ERROR("MDrv_CRC32_Cal_DeMura error\n");
    }
    memset(strbuf, 0, sizeof(strbuf));
    snprintf(strbuf, sizeof(strbuf)-1, "0x%x:0x%x", check_sum, sample_crc32);
    setenv(ENV_DEMURA_SIG, strbuf);
    saveenv();

    dfree(fbuf);
    return TRUE;
}


MS_BOOL If_Need_Decode(void)
{
    UBOOT_TRACE("IN\n");
    if (init_spi_flash() != TRUE)
    {
        UBOOT_DEBUG("init_spi_flash error!\n");
        return TRUE;
    }

    char *sig_str = getenv(ENV_DEMURA_SIG);
    if (sig_str == NULL)
    {
        UBOOT_DEBUG("Can not find env(%s)\n", ENV_DEMURA_SIG);
        return TRUE;
    }
    else
    {
        MS_U8  checksum_sf;
        MS_U32 checksum_bd;
        MS_U32 sample_crc32, cal_crc32;
        MS_U8  buf[SAMPLE_DAT_LEN];

        if (read_spi_flash(&checksum_sf, LGD_CHECKSUM_ADR, sizeof(checksum_sf)) == FALSE)
        {
            UBOOT_ERROR("Read LGD Demura Checksum Failed !\n");
            return TRUE;
        }
        UBOOT_DEBUG("checksum_sf  = 0x%x\n", (uint)checksum_sf);

        UBOOT_DEBUG("sig_str = %s\n", sig_str);
        if(2 == sscanf(sig_str, "0x%x:0x%x", &checksum_bd, &sample_crc32))
        {
            UBOOT_DEBUG("checksum_bd  = 0x%x\n", (uint)checksum_bd);
            UBOOT_DEBUG("sample_crc32 = 0x%x\n", (uint)sample_crc32);

            if (read_spi_flash(buf, (LGD_MURA_START_ADR + SAMPLE_DAT_START), SAMPLE_DAT_LEN) == TRUE)
            {
                cal_crc32 = MDrv_CRC32_Cal_DeMura(buf, SAMPLE_DAT_LEN);
                UBOOT_DEBUG("cal_crc32 = 0x%x\n", (uint)cal_crc32);
                if ((sample_crc32 == cal_crc32) && (checksum_bd == checksum_sf))
                {
                    UBOOT_DEBUG("Data Match, No Need to decode again\n");
                    UBOOT_TRACE("OK\n");
                    return FALSE;
                }
            }
        }
        UBOOT_TRACE("OK\n");
        return TRUE;
    }

    UBOOT_TRACE("OK\n");
    return FALSE;
}

MS_BOOL Check_MuraTable(void)
{
    if (init_spi_flash() != TRUE)
    {
        UBOOT_DEBUG("init_spi_flash error!\n");
        return FALSE;
    }
    return TRUE;
}

