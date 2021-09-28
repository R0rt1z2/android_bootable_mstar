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
#include <ms_array.h>

#include <apiPNL.h>
#include <MDrvDemura.h>
#include <halDemura.h>
#include <CommonDataType.h>
#include <convert_entry.h>
#include "decompress_8206.h"
#include "vendor.h"
#include "parse_header.h"
#include "crc_libs.h"
#include "demura_config.h"


#define HEADER_MAX_SIZE     0x200
#define SAMPLE_DAT_START    1024     // Byte
#define SAMPLE_DAT_LEN      4096     // Byte

#undef ALIGN
#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))

static Vendor2Mstar_Info dec_info;


static MS_BOOL set_u13_interface(Vendor2Mstar_Info *pv2m, void *pDataInfo, BinOutputInfo *pbin_info)
{
    int n_Hnode     = pv2m->hblock_num;       // 481;
    int n_Vnode     = pv2m->vblock_num;       // 271;
    int hblock_size = pv2m->hblock_size;      //   8;
    int vblock_size = pv2m->vblock_size;      //   8;
    int level_count = pv2m->layer_count;
    interface_info *pInfo_Out = (interface_info *)pDataInfo;

    pInfo_Out->reg_dmc_plane_num              = level_count;                    //  4 bit
    pInfo_Out->reg_dmc_h_block                = hblock_size > 4 ? 0x03 : 0x02;  //  3 bit
    pInfo_Out->reg_dmc_v_block                = vblock_size > 4 ? 0x03 : 0x02;  //  3 bit
    pInfo_Out->reg_dmc_rgb_mode               = 0x1;         //  1 bit
    pInfo_Out->reg_dmc_panel_h_size           = 0xF00;       // 13 bit
    pInfo_Out->reg_dmc_black_limit            = pv2m->bot_limit * 16;       // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level1           = pv2m->layer_levels[0] * 16;     // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level2           = pv2m->layer_levels[1] * 16;     // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level3           = pv2m->layer_levels[2] * 16;     // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level4           = pv2m->layer_levels[3] * 16;     // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level5           = pv2m->layer_levels[4] * 16;     // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level6           = 0;           // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level7           = 0;           // 12 bit , format 10.2
    pInfo_Out->reg_dmc_plane_level8           = 0;           // 12 bit , format 10.2
    pInfo_Out->reg_dmc_white_limit            = pv2m->top_limit * 16;       // 12 bit , format 10.2
    pInfo_Out->reg_dmc_dither_en              = 0x1;         //  2 bit , 0 : rounding , 1 : dither , 2~3 : truncate
    pInfo_Out->reg_dmc_dither_rst_e_4_frame   = 0x0;         //  1 bit
    pInfo_Out->reg_dmc_dither_pse_rst_num     = 0x0;         //  2 bit

    // Remember to set !
    pbin_info->Sep_type                       = pInfo_Out->reg_dmc_rgb_mode;

    int idx;
    for (idx = 0; idx < level_count; idx++)
    {
        int nLayer = 0;
        switch (idx)
        {
            case 0:
                nLayer = pInfo_Out->reg_dmc_plane_level1 / 4;
                break;
            case 1:
                nLayer = pInfo_Out->reg_dmc_plane_level2 / 4;
                break;
            case 2:
                nLayer = pInfo_Out->reg_dmc_plane_level3 / 4;
                break;
            case 3:
                nLayer = pInfo_Out->reg_dmc_plane_level4 / 4;
                break;
            case 4:
                nLayer = pInfo_Out->reg_dmc_plane_level5 / 4;
                break;
            case 5:
                nLayer = pInfo_Out->reg_dmc_plane_level6 / 4;
                break;
            case 6:
                nLayer = pInfo_Out->reg_dmc_plane_level7 / 4;
                break;
            case 7:
                nLayer = pInfo_Out->reg_dmc_plane_level8 / 4;
                break;
            default:
                break;
        }
        int **layer_value_lut = pv2m->LutIn[idx];

        int i, j, idx_image_size = 0;
        for (i = 0; i < n_Vnode; i++)
        {
            for (j = 0; j < n_Hnode ; j++)
            {
                (*pInfo_Out).Lut_in[idx][idx_image_size].dbr   = layer_value_lut[i][j] + nLayer;
                (*pInfo_Out).Lut_in[idx][idx_image_size].dbg   = layer_value_lut[i][j] + nLayer;
                (*pInfo_Out).Lut_in[idx][idx_image_size].dbb   = layer_value_lut[i][j] + nLayer;

                (*pInfo_Out).Lut_in[idx][idx_image_size].r   = (int)(layer_value_lut[i][j] + nLayer);
                (*pInfo_Out).Lut_in[idx][idx_image_size].g   = (int)(layer_value_lut[i][j] + nLayer);
                (*pInfo_Out).Lut_in[idx][idx_image_size].b   = (int)(layer_value_lut[i][j] + nLayer);
                idx_image_size += 1;
            }
        }
    }
    return TRUE;
}


MS_BOOL INX_Decode_To_Mstar_Format(Vendor2Mstar_Info *pv2m, void *pDataInfo, BinOutputInfo *pbin_info)
{
    MS_BOOL bRet = FALSE;
    DeCmpxInfo cmpx;
    MS_U32 sample_crc32;
    char  strbuf[1024];

    int    n_Hnode         =  pv2m->hblock_num;
    int    n_Vnode         =  pv2m->vblock_num;
    MS_U32 length          =  pv2m->data_length;
    MS_U32 *buffer         =  pv2m->input_buf;

    pbin_info->HNode       =  n_Hnode;
    pbin_info->VNode       =  n_Vnode;
    pbin_info->Blk_h_size  =  pv2m->hblock_size;
    pbin_info->Blk_v_size  =  pv2m->vblock_size;
    pbin_info->LevelCount  =  pv2m->layer_count;
    
    if (buffer == NULL)
    {
        UBOOT_ERROR("Error : INX Compress data input buffer == NULL !\n");
        return FALSE;
    }

    // ***************** Prepare Ouput Buffer *****************
    DEFINE_ARRAY_DIM2(_LutOut1, sizeof(int), n_Vnode, n_Hnode);
    DEFINE_ARRAY_DIM2(_LutOut2, sizeof(int), n_Vnode, n_Hnode);
    DEFINE_ARRAY_DIM2(_LutOut3, sizeof(int), n_Vnode, n_Hnode);
    DEFINE_ARRAY_DIM2(_LutOut4, sizeof(int), n_Vnode, n_Hnode);
    DEFINE_ARRAY_DIM2(_LutOut5, sizeof(int), n_Vnode, n_Hnode);

    // Enter decompress
    memset(&cmpx, 0, sizeof(cmpx));
    cmpx.Height      = n_Vnode;
    cmpx.Width       = n_Hnode;
    cmpx.ByteCmpxIn  = buffer;
    cmpx.CmpxSize    = length;
    cmpx.LutOut[0]   = (int **)PTR_ARRAY_DIM2(_LutOut1);
    cmpx.LutOut[1]   = (int **)PTR_ARRAY_DIM2(_LutOut2);
    cmpx.LutOut[2]   = (int **)PTR_ARRAY_DIM2(_LutOut3);
    cmpx.LutOut[3]   = (int **)PTR_ARRAY_DIM2(_LutOut4);
    cmpx.LutOut[4]   = (int **)PTR_ARRAY_DIM2(_LutOut5);

    DeCmpx(&cmpx);
    printf("Decompress Done !\n");
    memcpy(pv2m->LutIn, cmpx.LutOut, sizeof(cmpx.LutOut));

    // Sign spi flash data
    sample_crc32 = MDrv_CRC32_Cal_DeMura((MS_U8 *)buffer + SAMPLE_DAT_START, SAMPLE_DAT_LEN);
    if (sample_crc32 == 0xffffFFFF)
    {
        UBOOT_ERROR("MDrv_CRC32_Cal_DeMura error\n");
        dfree(buffer);
        goto done;
    }
    memset(strbuf, 0, sizeof(strbuf));
    snprintf(strbuf, sizeof(strbuf)-1, "0x%x:0x%x", pv2m->data_checksum, sample_crc32);
    setenv(ENV_DEMURA_SIG, strbuf);
    saveenv();
    dfree(buffer);
    pv2m->input_buf = NULL;

    // alloc Lut_in buffer
    bRet = Alloc_LutIn_Space(pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Alloc_LutIn_Space error\n");
        goto done;
    }

    // set interface_info
    bRet = set_u13_interface(pv2m, pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("set_u13_interface error\n");
    }

done:
    FREE_ARRAY_DIM2(_LutOut1);
    FREE_ARRAY_DIM2(_LutOut2);
    FREE_ARRAY_DIM2(_LutOut3);
    FREE_ARRAY_DIM2(_LutOut4);
    FREE_ARRAY_DIM2(_LutOut5);
    memset(pv2m->LutIn, 0, sizeof(pv2m->LutIn));    // Clean up

    return bRet;
}


MS_BOOL get_demura_header(INX_Demura_Header *phdr)
{
    MS_BOOL bRet;
    MS_U8 buf[HEADER_MAX_SIZE];

    bRet = read_spi_flash(buf, 0, HEADER_MAX_SIZE);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Read INX Demura from spi flash failed !\n");
        return FALSE;
    }

    bRet = parse_vendor_header(buf, phdr);
    if (bRet != TRUE)
    {
        UBOOT_ERROR("parse_vendor_header\n");
        return FALSE;
    }

    UBOOT_DEBUG("\nBuild Date : %x/%02x/%02x\n", (uint)((phdr->date>>16)&0xFFFF), \
                 (uint)((phdr->date>>8)&0xFF), (uint)(phdr->date & 0xFF));
    return TRUE;
}


static MS_BOOL Load_Decode_Info(void)
{
    INX_Demura_Header header;
    static MS_BOOL has_init = FALSE;

    UBOOT_TRACE("IN\n");
    if (has_init == TRUE)
    {
        UBOOT_TRACE("OK\n");
        return TRUE;
    }

    memset(&dec_info, 0, sizeof(dec_info));
    if (init_spi_flash() != TRUE)
    {
        UBOOT_DEBUG("init_spi_flash error!\n");
        return FALSE;
    }
    if (get_demura_header(&header) != TRUE)
    {
        UBOOT_DEBUG("get_demura_header error!\n");
        return FALSE;
    }
    if (fetch_decode_info(&header, &dec_info) != TRUE)
    {
        UBOOT_DEBUG("fetch_decode_info error\n");
        return FALSE;
    }

    has_init = TRUE;
    UBOOT_TRACE("OK\n");
    return TRUE;
}


static void Dump_Decode_Info(void)
{
    uint i;
    // Print data information
    UBOOT_INFO("%s Layer count : %d, Bot Limit : %d, Top Limit : %d\n", CONFIG_DEMURA_VENDOR_STRING, \
                (int)(dec_info.layer_count), (int)dec_info.bot_limit, (int)dec_info.top_limit);
    UBOOT_INFO("%s Layer value :", CONFIG_DEMURA_VENDOR_STRING);
    for (i = 0; i < dec_info.layer_count; i++)
    {
        printf(" %d", (int)(dec_info.layer_levels[i]));
    }
    printf("\n");
}


MS_BOOL Decode_To_Mstar_Format(void *pDataInfo, BinOutputInfo *pbin_info)
{
    MS_BOOL bRet;

    //=============================== read setting ===============================
    if (Load_Decode_Info() != TRUE)
    {
        UBOOT_DEBUG("Load_Decode_Info error, Unable to Decode_To_Mstar_Format\n");
        return FALSE;
    }
    Dump_Decode_Info();

    // Decode Vendor data
    bRet = INX_Decode_To_Mstar_Format(&dec_info, pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("INX_Decode_To_Mstar_Format error\n");
        return FALSE;
    }
    
    MS_BOOL demura_enable[3]  = {TRUE, TRUE, TRUE};
    
    pbin_info->Build_Date     =  dec_info.build_date;
    pbin_info->reg_base_addr  =  0x7700;
    pbin_info->project_id     =  ((MSTAR_CHIP_ID << 16) + (1ULL << 15) + ID_CUS_MSTAR);
    pbin_info->data_type      =  IC_DRAM;
    pbin_info->Gain_type      =  10;
    
    memcpy(pbin_info->channel_enable, demura_enable, sizeof(demura_enable));
    
    return TRUE;
}


MS_BOOL If_Need_Decode(void)
{
    UBOOT_TRACE("IN\n");
    if (Load_Decode_Info() != TRUE)
    {
        UBOOT_DEBUG("Load_Decode_Info error, Unable to decode data\n");
        dfree(dec_info.input_buf);   // No need to decode, dfree the input file buffer;
        return FALSE;
    }

    char *sig_str = getenv(ENV_DEMURA_SIG);
    if (sig_str == NULL)
    {
        Dump_Decode_Info();
        return TRUE;
    }
    else
    {
        MS_U32 blk_checksum;
        MS_U32 sample_crc32, cal_crc32;
        MS_U8  buf[SAMPLE_DAT_LEN];

        UBOOT_DEBUG("sig_str = %s\n", sig_str);
        if(2 == sscanf(sig_str, "0x%x:0x%x", &blk_checksum, &sample_crc32))
        {
            UBOOT_DEBUG("blk_checksum = 0x%x\n", (uint)blk_checksum);
            UBOOT_DEBUG("sample_crc32 = 0x%x\n", (uint)sample_crc32);

            if (read_spi_flash(buf, (dec_info.data_start + SAMPLE_DAT_START), SAMPLE_DAT_LEN) == TRUE)
            {
                cal_crc32 = MDrv_CRC32_Cal_DeMura(buf, SAMPLE_DAT_LEN);
                UBOOT_DEBUG("cal_crc32 = 0x%x\n", (uint)cal_crc32);
                if ((sample_crc32 == cal_crc32) && (dec_info.data_checksum == blk_checksum))
                {
                    UBOOT_DEBUG("Data Match, No Need to decode again\n");
                    UBOOT_TRACE("OK\n");
                    dfree(dec_info.input_buf);
                    return FALSE;
                }
            }
        }

        Dump_Decode_Info();
        UBOOT_TRACE("OK\n");
        return TRUE;
    }

    dfree(dec_info.input_buf);
    UBOOT_TRACE("OK\n");
    return FALSE;
}


MS_BOOL Check_MuraTable(void)
{
    MS_U32 length, offset;
    MS_U32 cal_sum;
    MS_U8  *bufTemp;
    MS_U32 *buffer;
    MS_BOOL bRet;

    UBOOT_TRACE("IN\n");
    if (Load_Decode_Info() != TRUE)
    {
        UBOOT_ERROR("Load_Decode_Info error, \033[31mCheck_MuraTable --> Failed\033[0m\n");
        UBOOT_TRACE("OK\n");
        return FALSE;
    }
    length = dec_info.data_length;
    offset = dec_info.data_start;

    bufTemp = (MS_U8 *)dmalloc(length + 4);
    CHECK_DMALLOC_SPACE(bufTemp, (uint)(length + 4));
    buffer  = (MS_U32 *)ALIGN((size_t)bufTemp, 4);

    bRet = read_spi_flash((MS_U8 *)buffer, offset, length);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Read Data block from spi flash failed !\n");
        dfree(buffer);
        return FALSE;
    }
    
    // Calculate checksum
    cal_sum = inx_crc8((MS_U8 *)buffer, INX_CRC8_SEED, 0, length);
    UBOOT_DEBUG("Block calculate checksum = 0x%x, checksum = 0x%x\n", (uint)cal_sum, (uint)dec_info.data_checksum);
    if (cal_sum != dec_info.data_checksum)
    {
        UBOOT_ERROR("Data Checksum Error. Calculate(0x%x) != Checksum(0x%x)\n", (uint)cal_sum, (uint)dec_info.data_checksum);
        dfree(buffer);
        return FALSE;
    }

    // Keep block data, avoid to read it from spi_flash again.
    dec_info.input_buf = buffer;
    UBOOT_TRACE("OK\n");
    return TRUE;
}

