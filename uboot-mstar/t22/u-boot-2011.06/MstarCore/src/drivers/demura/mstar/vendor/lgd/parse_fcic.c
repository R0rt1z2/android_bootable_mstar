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
#include <ms_array.h>
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


MS_BOOL Load_LGD_Bin(char **pbuf, int flash_start_addr, int bin_size)
{
    MS_BOOL  bRet;
    char *buffer;
    MS_U32 fsize;

    UBOOT_TRACE("IN\n");
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
        return FALSE;
    }

    *pbuf = buffer;
    UBOOT_TRACE("OK\n");
    return TRUE;
}



MS_BOOL parse_LG_bin_1tab(int n_Hnode, int n_Vnode, interface_info *info, MS_BOOL bFCIC)
{
    // Create data array
    DEFINE_ARRAY_DIM4(_data, sizeof(int), 271, 492, 4, 4);
    int **** data = (int ****)PTR_ARRAY_DIM4(_data);

    int fcic_gain[4];
    int fcic_ofst[4];

    UBOOT_TRACE("IN\n");;

    int start_addr = 0; // must be times of 32 (0x20)
    int data_str_addr = 0;
    int chroma_4tab_addr = 0;
    int empty_dummy_addr = 0;
    int gain_offset_addr = 0;
    int checksum_addr = 0;
    char *fbuf = NULL;

    if (bFCIC == TRUE)
    {
        start_addr = 0x0; // must be times of 32 (0x20)
        data_str_addr = 0x4;
        chroma_4tab_addr = 0x208d8;
        empty_dummy_addr = 0x39d24;
        gain_offset_addr = 0x5c304;
        checksum_addr = 0x5c310;
    }
    else
    {
        start_addr = 0x60000;        // must be times of 32 (0x20)
        data_str_addr = 0x00004;     // 0x60004 , For optimize
        gain_offset_addr = 0x085e4;  // 0x685e4
        checksum_addr = 0x085f0;     // 0x685f0
    }

    MS_BOOL bRet = Load_LGD_Bin(&fbuf, start_addr, (checksum_addr + 16));
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Load_LGD_Bin Error\n");
        FREE_ARRAY_DIM4(_data);
        return FALSE;
    }

    //! Start Parsing
    int byte_address = 0;
    MS_BOOL end_flag = FALSE;
    int check_sum = 0;
    int total_sum = 0;
    int h_cnt = 0, v_cnt = 0, c_cnt = 0, t_cnt = 0;

    while (end_flag != TRUE)
    {
        int i;
        char *line;  // char line[32];
        line = fbuf + byte_address;

        for (i = 0; i < 32; i++) // Parse 32 bytes in one time
        {
            if (byte_address + i < data_str_addr) // Skip bulk detect bytes
                 continue;

            //! End after get Checksum byte
            if (byte_address + i >= checksum_addr)
            {
                check_sum = line[i] & 0xff;
                end_flag = TRUE;
                break;
            }

            if (bFCIC == TRUE)
            {
                if (byte_address + i >= gain_offset_addr) // 12 bytes Gain & Offset
                {
                    if (byte_address + i == gain_offset_addr)
                        fcic_gain[2] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 1)
                        fcic_gain[1] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 2)
                        fcic_gain[0] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 4)
                        fcic_ofst[0] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 7)
                        fcic_gain[3] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 9)
                        fcic_ofst[3] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 10)
                        fcic_ofst[2] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 11)
                        fcic_ofst[1] = line[i] & 0xff;
                }
                else if (byte_address + i < chroma_4tab_addr) // luma (3:W, 1:Gray64)
                {
                    data[v_cnt][h_cnt / 4 * 4 + (3 - i % 4)][3][1] = line[i];
                    //! Add index of data matrix
                    h_cnt++;
                    if (h_cnt == 492)
                    {
                        h_cnt = 0;
                        v_cnt++;
                    }
                }
                else if (byte_address + i < empty_dummy_addr) // chroma (0~2:RGB)
                {
                    if (byte_address + i == chroma_4tab_addr)
                    {
                        UBOOT_DEBUG("Start parsing chroma, reset counters\n");
                        v_cnt = 0;
                        h_cnt = 0;
                        c_cnt = 0;
                        t_cnt = 0;
                    }

                    data[v_cnt][h_cnt][c_cnt][t_cnt] = line[i];

                    //! Add index of data matrix
                    t_cnt++;
                    if (t_cnt == 4)
                    {
                        t_cnt = 0;
                        h_cnt++;
                        if (h_cnt == 125)
                        {
                            h_cnt = 0;
                            v_cnt++;
                            if (v_cnt == 69)
                            {
                                v_cnt = 0;
                                c_cnt++;
                            }
                         }
                    }
                }
                //! (total_sum % 256) to verify check_sum
                total_sum += line[i];
                total_sum %= 256;
            }
            else
            {
                if (byte_address + i >= gain_offset_addr) // 12 bytes Gain & Offset
                {
                    if (byte_address + i == gain_offset_addr)
                        fcic_gain[2] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 1)
                        fcic_gain[1] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 2)
                        fcic_gain[0] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 4)
                        fcic_ofst[0] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 7)
                        fcic_gain[3] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 9)
                        fcic_ofst[3] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 10)
                        fcic_ofst[2] = line[i] & 0xff;
                    else if (byte_address + i == gain_offset_addr + 11)
                        fcic_ofst[1] = line[i] & 0xff;
                }
                else
                {
                    data[v_cnt][h_cnt / 4 * 4 + (3 - i % 4)][3][1] = line[i];
                    //! Add index of data matrix
                    h_cnt++;
                    if (h_cnt == 252)
                    {
                        h_cnt = 0;
                        v_cnt++;
                    }
                }
                //! (total_sum % 256) to verify check_sum
                total_sum += (int)(line[i]);
                total_sum %= 256;
            }
        }
        byte_address += 32;
    }

    UBOOT_DEBUG("Checksum = %x\n", check_sum);
    UBOOT_DEBUG("2's complement(total_sum %% 256) = %x\n", ((-1 * total_sum) & 0xff));
    if (((-1 * total_sum) & 0xff) == check_sum)
    {
        UBOOT_DEBUG("\nChecksum PASS !!!\n");
    }
    else
    {
        UBOOT_ERROR("\nChecksum FAIL !!!\n");
        FREE_ARRAY_DIM4(_data);
        dfree(fbuf);
        return FALSE;
    }

    int i, j, t, c;
    for (t = 0; t < 4; t++)
    {
        UBOOT_DEBUG("Tab %d: gain = %d, ofst = %d\n", t, fcic_gain[t], fcic_ofst[t]);
    }

    //! The data provided by LGD is 1's complement
    //! Convert 1's complement to 2's complement
    //! For Full&Simple FCIC, firstly convert G64 where the 1-layer data provided for "W" channel only
    for (j = 0; j < n_Vnode; j++)
    {
        for (i = 0; i < n_Hnode; i++)
        {
            int sign = 0;
            int value = data[j][i][3][1] & 0x7f;
            //value = (value) / 2; // s6.1 to s7(s0_6.0)
            if ((data[j][i][3][1] & 0x80) == 0x80)
                sign = -1;
            else
                sign = 1;

            data[j][i][3][1] = sign * value;
        }
    }
    
    for (c = 0; c < 4; c++)
    {
        for (t = 0; t < 4; t++)
        {
            for (j = 0; j < n_Vnode; j++)
            {
                for (i = 0; i < n_Hnode; i++)
                {                
                    int sign = 0;
                    int value = data[j][i][c][t] & 0x7f;
                    if ((data[j][i][c][t] & 0x80) == 0x80)
                        sign = -1;
                    else
                        sign = 1;
                
                    //! For Full&Simple FCIC, calculate G64 with gain&offset to generate the other tabs for "W" channel only
                    if (c == 3)
                    {
                        if (t != 1)
                        {
                            int final_out = ((int)(data[j][i][c][1]) * fcic_gain[t]) / 64 + fcic_ofst[t];
                            if (final_out < -128 || final_out > 127)
                            {
                                UBOOT_DEBUG("FCIC: Overflow according to data with the gains & offsets\n");
                                //FREE_ARRAY_DIM4(_data);
                                //dfree(fbuf);
                                //return FALSE;
                            }
                            data[j][i][c][t] = final_out;
                        }
                    }
                    else
                    {
                        data[j][i][c][t] = sign * value;
                    }
                }                
            }
        }
    }

    int nLayer = 0;
    for (t = 0; t < 4; t++)
    {
        //get level 
        switch (t)
        {
            case 0:
                nLayer = info->reg_dmc_plane_level1 >> 2;
                break;
            case 1:
                nLayer = info->reg_dmc_plane_level2 >> 2;
                break;
            case 2:
                nLayer = info->reg_dmc_plane_level3 >> 2;
                break;
            case 3:
                nLayer = info->reg_dmc_plane_level4 >> 2;
                break;
        }
            
        for (j = 0; j < n_Vnode; j++)
        {
            for (i = 0; i < n_Hnode; i++)
            {
                info->Lut_in[t][j * n_Hnode + i].dbr = (double)(data[j][i][3][t] >> 1) + nLayer;
                info->Lut_in[t][j * n_Hnode + i].r   = (data[j][i][3][t] >> 1) + nLayer;
                info->Lut_in[t][j * n_Hnode + i].dbg = (double)(data[j][i][3][t] >> 1) + nLayer;
                info->Lut_in[t][j * n_Hnode + i].g   = (data[j][i][3][t] >> 1) + nLayer;
                info->Lut_in[t][j * n_Hnode + i].dbb = (double)(data[j][i][3][t] >> 1) + nLayer;
                info->Lut_in[t][j * n_Hnode + i].b   = (data[j][i][3][t] >> 1) + nLayer;
            }
        }
    }

    FREE_ARRAY_DIM4(_data);
    dfree(fbuf);
    UBOOT_TRACE("OK\n");
    return TRUE;
}


MS_BOOL transfer_LGD_Data_1tab(void *pdat_info, BinOutputInfo *pbin_info)
{
    int n_Hnode = 481;
    int n_Vnode = 271;

    //inital data
    MS_BOOL bFCIC = pbin_info->Vendor_ARG1;
    if (bFCIC == FALSE)
    {
        n_Hnode = 241;
        n_Vnode = 136;
    }

    #if defined (CONFIG_DEMURA_URSA13)
    interface_info *pDataInfo = (interface_info *)pdat_info;
    #else
    #error "Unkown DEMURA_URSA_TYPE !"
    #endif

    if (bFCIC == TRUE)
    {
        pDataInfo->reg_dmc_h_block            = 0x3;         //  3 bit
        pDataInfo->reg_dmc_v_block            = 0x3;         //  3 bit
    }
    else
    {
        pDataInfo->reg_dmc_h_block            = 0x4;         //  3 bit
        pDataInfo->reg_dmc_v_block            = 0x4;         //  3 bit
    }
    
    pDataInfo->reg_dmc_plane_num              = 0x4;         //  4 bit
    pDataInfo->reg_dmc_rgb_mode               = 0x0;         //  1 bit
    pDataInfo->reg_dmc_panel_h_size           = 0xF00;       // 13 bit
    pDataInfo->reg_dmc_black_limit            = 0x000;       // 12 bit , format 10.2        
    pDataInfo->reg_dmc_plane_level1           = 0x200;       // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level2           = 0x400;       // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level3           = 0x800;       // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level4           = 0xC00;       // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level5           = 0;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level6           = 0;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level7           = 0;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level8           = 0;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_white_limit            = 0xFFF;       // 12 bit , format 10.2
    pDataInfo->reg_dmc_dither_en              = 0x1;         //  2 bit , 0 : rounding , 1 : dither , 2~3 : truncate
    pDataInfo->reg_dmc_dither_rst_e_4_frame   = 0x0;         //  1 bit
    pDataInfo->reg_dmc_dither_pse_rst_num     = 0x0;         //  2 bit

    // Init Output message
    pbin_info->HNode          = n_Hnode;
    pbin_info->VNode          = n_Vnode;
    pbin_info->LevelCount     = pDataInfo->reg_dmc_plane_num;
    pbin_info->Blk_h_size     = 1 << (pDataInfo->reg_dmc_h_block);
    pbin_info->Blk_v_size     = 1 << (pDataInfo->reg_dmc_v_block);

    MS_BOOL bRet = Alloc_LutIn_Space(pdat_info, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Alloc_LutIn_Space error\n");
        return FALSE;
    }

    bRet = parse_LG_bin_1tab(n_Hnode, n_Vnode, pDataInfo, bFCIC);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("parse_LG_bin_1tab error\n");
        return FALSE;
    }

    return TRUE;
}

