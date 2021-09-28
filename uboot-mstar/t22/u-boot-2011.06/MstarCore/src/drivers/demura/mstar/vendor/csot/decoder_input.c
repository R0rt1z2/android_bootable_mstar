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
#include <MsDebug.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <malloc.h>

#include <CommonDataType.h>
#include <dmalloc.h>
#include <read_text.h>
#include <demura.h>
#include <ms_utils.h>

#include <apiPNL.h>
#include <MDrvDemura.h>
#include <halDemura.h>
#include <convert_entry.h>
#include "vendor.h"

#define HEADER_MAX_SIZE   1024


typedef union
{
    MS_U32 block_data;
    unsigned char data[4];

    struct
    {
        int nReserved:8;
        int b3:6;
        int b2:6;
        int b1:6;
        int b0:6;
    };    
}DATA;


static void read_ini_file_Setting(CSOT_Demura_Header *pHeader)
{
    int  ret, count;
    int  nFind = 0;
    int  pbuf[11];
    char *pkey;
    line_info linfo;
    
    while (get_line(&linfo))
    {
        /* Find "simple" key word */
        if (nFind == 0)
        {
            pkey = (char *)linfo.pkey_start;
            if ( pkey != NULL)
            {
                DBG_TEXT_PRINT("pkey = %s\n", pkey); // test
                if (strcmp(pkey, "CONTROL") == 0)
                {
                    nFind = 1;
                }
            }
            continue;
        }

        if (nFind == 1)
        {
            count = sizeof(*pHeader)/sizeof(MS_U32);
            ret = split_num_string((char *)linfo.pcont_start, ",", pbuf, count); 
            if (ret != count)
            {
                continue;
            }
            memcpy(pHeader, pbuf, sizeof(*pHeader));
            break;
        }
    }
}


static MS_BOOL read_ini_file_table(CSOT_Demura_Header *pHeader, int idx, int nLevel, int *row_dat, double **file_array)
{
    int color, row_cnt;
    int color_index;
    char *pkey;
    char key_name[3][128];

    /******************* vendor *****************/
    int h_size = pHeader->HNode;
    int v_size = pHeader->VNode;

    if (pHeader->sep_type == 1)
    {
        color_index = 3;
        sprintf(key_name[0], "RL%d", idx);
        sprintf(key_name[1], "GL%d", idx);
        sprintf(key_name[2], "BL%d", idx);
    }
    else
    {
        color_index = 1;
        sprintf(key_name[0], "RL%d", idx);
    }
    
    for (color = 0; color < color_index; color++)
    {
        int nTotalCount = 0;            // Write Data length
        int nFind = 0;
        int n_Vnode_Count = 0;
        line_info linfo;
        
        while (get_line(&linfo))
        {
            /* Find "simple" key word */
            if (nFind == 0)
            {
                pkey = (char *)linfo.pkey_start;
                if (pkey != NULL)
                {
                    DBG_TEXT_PRINT("pkey = %s\n", pkey); // test
                    if (strcmp(pkey, key_name[color]) == 0)
                    {
                        nFind = 1;
                    }

//printf("%s : \n", key_name[color]);

                }
                n_Vnode_Count = 0;
                continue;
            }

            if (nFind == 1)
            {
                row_cnt = split_num_string((char *)linfo.pcont_start, ",", row_dat, 0);
                if (row_cnt <= 0)
                {
                   continue;
                }
                else if (row_cnt > (h_size * 2))
                {
                    printf("error, (row_cnt > (h_size * 2)\n");
                    return FALSE;
                }

                int count;
                int n_Hnode_Count = 0;
                for (count = 0; count < row_cnt; count+=3)
                {
                    DATA temp;
                
                    //block n
                    temp.data[3] = row_dat[count]; 
                    temp.data[2] = row_dat[count + 1];
                    temp.data[1] = row_dat[count + 2];
                    temp.data[0] = 0;

                    int x;
                    for (x = 0; x < 4; x++)
                    {
                        if (n_Hnode_Count < h_size)
                        {
                            switch (x % 4)
                            {
                                case 0:
                                    file_array[color][nTotalCount] = temp.b0 + nLevel;
                                    break;
                            
                                case 1:
                                    file_array[color][nTotalCount] = temp.b1 + nLevel;
                                    break;
                            
                                case 2:
                                    file_array[color][nTotalCount] = temp.b2 + nLevel;
                                    break;
                            
                                case 3:
                                    file_array[color][nTotalCount] = temp.b3 + nLevel;
                                    break;
                            }

//printf("%d ", (int)(file_array[color][nTotalCount]) - nLevel );

                            n_Hnode_Count++;
                            nTotalCount++;
                        }
                    }
                }
//printf("\n");
                n_Vnode_Count++;
            }

            // check total data size
            if (n_Vnode_Count >= v_size)
            {
//printf("\n");
                break;
            }
        }
    }
    return TRUE;
}


MS_U32 cal_max_filesize(CSOT_Demura_Header *phead, MS_U32 flash_size)
{
    #define FILESIZE_ALIGNMENT  0x1000
    #define FILESIZE_ALIGNMASK  (FILESIZE_ALIGNMENT - 1)

    MS_U32 cal_size;
    int row_blk_num, row_len;
    int symbol_size = 0x2000;             // 8KB
    double remain_num;

    row_blk_num = phead->HNode * 0.75;    // CSOT data compress
    remain_num  = (phead->HNode * 0.75) - row_blk_num;
    if (remain_num > 0)
    {
        row_blk_num += 3;
    }
    row_len = row_blk_num * 4 + 10;    // CSOT data format : 8bit_demical
    DBG_TEXT_PRINT("row_len = %d\n", (unsigned int)row_len);

    cal_size = row_len * phead->VNode * phead->LevelCount;
    if (phead->sep_type == 1)
    {
        cal_size = cal_size * 3;
    }
    cal_size += symbol_size;

    if (cal_size & FILESIZE_ALIGNMASK)
    {
        cal_size +=  FILESIZE_ALIGNMENT;
        cal_size &= (~FILESIZE_ALIGNMASK);
    }
    DBG_TEXT_PRINT("calculate max filesize = 0x%x\n", (unsigned int)cal_size);

    if (flash_size > cal_size)
    {
        return cal_size;
    }
    else
    {
        return flash_size;
    }
}


static MS_BOOL get_demura_header(CSOT_Demura_Header *phead)
{
    char  *pIni;

    pIni = dmalloc(HEADER_MAX_SIZE);
    CHECK_DMALLOC_SPACE(pIni, HEADER_MAX_SIZE);
    
    memset(pIni, 0, HEADER_MAX_SIZE);
    if (read_spi_flash((MS_U8 *)pIni, 0, (HEADER_MAX_SIZE - 1)) != TRUE)
    {
        printf("read_spi_flash error\n");
        return FALSE;
    }
    memset(phead, 0, sizeof(*phead));
    
    init_text_info(pIni, HEADER_MAX_SIZE);
    read_ini_file_Setting(phead);
    dfree(pIni);
    
    // dump demura header
    printf("\n");
    printf("block_size    : %s\n", (phead->blk_type == 0 ? "8x8" : (phead->blk_type == 1 ? "16x16" : "Unkown")));
    printf("HNode         : %u\n", (unsigned int)phead->HNode);
    printf("VNode         : %u\n", (unsigned int)phead->VNode);
    printf("RGB_Separate  : %s\n", (phead->sep_type == 0 ? "False" : "True"));
    printf("LevelCount    : %u\n", (unsigned int)phead->LevelCount);
    printf("nLayer        : %u, %u, %u, %u, %u, %u\n", (unsigned int)phead->Layer[0], (unsigned int)phead->Layer[1], \
            (unsigned int)phead->Layer[2], (unsigned int)phead->Layer[3], (unsigned int)phead->Layer[4], (unsigned int)phead->Layer[5]);
    printf("\n");

    if ((phead->HNode == 0) || (phead->VNode == 0) 
         || (phead->LevelCount == 0) || (phead->LevelCount > 6))
    {
       UBOOT_ERROR("demura header content invalid\n");
       return FALSE;
    }
    
    return TRUE;
}

static char *get_demura_body(CSOT_Demura_Header *phead)
{
    // read ini file into dram
    // Try to no read the total size of flash.
    unsigned int flash_size = get_spi_flash_size();
    unsigned int max_size   = cal_max_filesize(phead, flash_size);
    printf("calculate max filesize = 0x%x\n", max_size);

    char *pFile = dmalloc(max_size);
    CHECK_DMALLOC_SPACE(pFile, max_size);

    if (read_spi_flash((MS_U8 *)pFile, 0, max_size) != TRUE)
    {
        printf("read_spi_flash error\n");
        dfree(pFile);
        return NULL;
    }
    
    init_text_info(pFile, max_size);
    return pFile;
}


#if defined (CONFIG_DEMURA_URSA11)

static void init_u11_data(CSOT_Demura_Header *phead, interface_info *pDataInfo)
{
//========================================Demura Data Set================================================
    int channel, level;
    int Max_Channel = 3;         // R/G/B channel

    for(channel = 0; channel < Max_Channel; channel++)
    {
        for(level = 0; level < DEMURA_MAX_LAYER_NUM; level++)
        {
            pDataInfo[channel].iLayer_level[level] = phead->Layer[level] << 2;
        
            if (level < phead->LevelCount)
            {
                pDataInfo[channel].bLayer_level_en[level] = TRUE;
            }
            else
            {
                pDataInfo[channel].bLayer_level_en[level] = FALSE;
            }
        }
        
        pDataInfo[channel].iChannel = channel;
        pDataInfo[channel].bLow_luma_en = FALSE;
        pDataInfo[channel].iLow_luma_thrd = 10; //8bits
        pDataInfo[channel].iLow_luma_slope = 0;
        pDataInfo[channel].iLow_luma_min_weight = 0;
        pDataInfo[channel].bHigh_luma_en = FALSE;
        pDataInfo[channel].iHigh_luma_thrd = 200; //8bits
        pDataInfo[channel].iHigh_luma_slope = 0;
        pDataInfo[channel].iHigh_luma_min_weight = 0;
    }
//========================================End Demura Data Set================================================
}


static MS_BOOL set_u11_interface(CSOT_Demura_Header *phead, interface_info *pinfo)
{
    int  i;
    int  nTotalNode;
    int  channel_size;
    char *pFile;
    int n_Layer, channel;
    unsigned int alloc_size;

    init_u11_data(phead, pinfo);
    
    // do not move, for memory-optimize
    // dmalloc space for pinfo->Lut_in, Can not free When the func return
    n_Layer = DEMURA_MAX_LAYER_NUM;
    nTotalNode = phead->HNode * phead->VNode;

    // read ini file into dram
    pFile = get_demura_body(phead);
    if (pFile == NULL)
    {
        printf("get_demura_body error\n");
        return FALSE;
    }

    // dmalloc space for layer_value_lut
    int idx;
    char *player_lut;
    double *layer_value_lut[3];
    
    alloc_size = sizeof(double) * nTotalNode * 3;   // R/G/B channel data
    player_lut = dmalloc(alloc_size);
    CHECK_DMALLOC_SPACE(player_lut, alloc_size);
    
    for (idx = 0; idx < 3; idx++)
    {
        layer_value_lut[idx] = (double *)(player_lut + (nTotalNode * sizeof(double) * idx) );
    }

    // dmalloc space for row_int_array
    int *row_int_array;
    unsigned int n_Hnode = phead->HNode;
    unsigned int n_Vnode = phead->VNode;
    
    alloc_size = sizeof(int) * n_Hnode * 2;
    row_int_array = dmalloc(alloc_size);
    CHECK_DMALLOC_SPACE(row_int_array, alloc_size);

    // begin to covert ini file table
    for (idx = 0; idx < n_Layer; idx++)
    {
        if (idx < phead->LevelCount)
        {
            if (read_ini_file_table(phead, idx, 0, row_int_array, layer_value_lut) == FALSE)
            {
                return FALSE;
            }
        
            int idx_image_size;
            for (idx_image_size = 0; idx_image_size < (n_Hnode * n_Vnode); idx_image_size++)
            {
                for (channel = 0; channel < 3; channel++)
                {
                    pinfo[channel].Lut_in[idx][idx_image_size]   = layer_value_lut[channel][idx_image_size];
                }
            }
        
        }
        else
        {
            for (channel = 0; channel < 3; channel++)
            {
                int idx_image_size;
                for (idx_image_size = 0; idx_image_size < (n_Hnode * n_Vnode); idx_image_size++)
                {
                    pinfo[channel].Lut_in[idx][idx_image_size] =  0;
                }
            }
        }
    }

    // dump message
/*
    for (channel = 0; channel < 3; channel++)
    {
        int val;
        for (idx = 0; idx < 6; idx++)
        {
            printf("channel_%d_layer_%d :\n", channel, idx);
            int idx_image_size;
            for (idx_image_size = 0; idx_image_size < (n_Hnode * n_Vnode); idx_image_size++)
            {
                val = (int)(pinfo[channel].Lut_in[idx][idx_image_size]);
                printf("%d ", val);
                
                //
                if ((idx_image_size > 0) && ((idx_image_size % n_Hnode) == 0) )
                {
                    printf("\n");
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
*/

    
    // dfree Not necessary memory
    dfree(pFile);
    dfree(player_lut);
    dfree(row_int_array);
    
    return TRUE;
}

#elif defined (CONFIG_DEMURA_URSA13)

static void init_u13_data(CSOT_Demura_Header *phead, interface_info *pDataInfo)
{
    pDataInfo->reg_dmc_plane_num              = phead->LevelCount;   // 4 bit
    if (phead->blk_type == 0)
    {
        pDataInfo->reg_dmc_h_block            = 0x3;         //  3 bit
        pDataInfo->reg_dmc_v_block            = 0x3;         //  3 bit
    }
    else
    {
        pDataInfo->reg_dmc_h_block            = 0x4;         //  3 bit
        pDataInfo->reg_dmc_v_block            = 0x4;         //  3 bit
    }
    pDataInfo->reg_dmc_rgb_mode               = phead->sep_type;           //  1 bit
    pDataInfo->reg_dmc_panel_h_size           = 0xF00;                     // 13 bit
    pDataInfo->reg_dmc_black_limit            = 0x100;                     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level1           = phead->Layer[0] << 4;     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level2           = phead->Layer[1] << 4;     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level3           = phead->Layer[2] << 4;     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level4           = phead->Layer[3] << 4;     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level5           = phead->Layer[4] << 4;     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level6           = phead->Layer[5] << 4;     // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level7           = 0;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_plane_level8           = 0;           // 12 bit , format 10.2
    pDataInfo->reg_dmc_white_limit            = 0xEF0;       // 12 bit , format 10.2
    pDataInfo->reg_dmc_dither_en              = 0x1;         //  2 bit , 0 : rounding , 1 : dither , 2~3 : truncate
    pDataInfo->reg_dmc_dither_rst_e_4_frame   = 0x0;         //  1 bit
    pDataInfo->reg_dmc_dither_pse_rst_num     = 0x0;         //  2 bit
//=============================== End Demura Data Set //===============================
}


static MS_BOOL set_u13_interface(CSOT_Demura_Header *phead, interface_info *pinfo)
{
    int  nTotalNode;
    char *pFile;
    unsigned int alloc_size;

    init_u13_data(phead, pinfo);

    nTotalNode = phead->HNode * phead->VNode;

    // read ini file into dram
    pFile = get_demura_body(phead);
    if (pFile == NULL)
    {
        printf("get_demura_body error\n");
        return FALSE;
    }

    // dmalloc space for layer_value_lut
    unsigned int idx;
    char *player_lut;
    double *layer_value_lut[3];
    
    alloc_size = sizeof(double) * nTotalNode * 3;
    player_lut = dmalloc(alloc_size);
    CHECK_DMALLOC_SPACE(player_lut, alloc_size);
    
    for (idx = 0; idx < 3; idx++)
    {
        layer_value_lut[idx] = (double *)(player_lut + ((nTotalNode * sizeof(double)) * idx) );
    }

    // dmalloc space for row_int_array
    int *row_int_array;
    unsigned int n_Hnode = phead->HNode;
    unsigned int n_Vnode = phead->VNode;
    
    alloc_size = sizeof(int) * n_Hnode * 2;
    row_int_array = dmalloc(alloc_size);
    CHECK_DMALLOC_SPACE(row_int_array, alloc_size);

    #if (MSTAR_DEMURA_DAT_BIT_LEN < CSOT_DEMURA_DAT_BIT_LEN)
        #error "MSTAR_DEMURA_DAT_BIT_LEN < CSOT_DEMURA_DAT_BIT_LEN \nCan not support this type!"
    #endif

    // begin to covert ini file table
    for (idx = 0; idx < phead->LevelCount; idx++)
    {
        int nLevel = phead->Layer[idx] << (MSTAR_DEMURA_DAT_BIT_LEN - CSOT_DEMURA_DAT_BIT_LEN);
        
        if (read_ini_file_table(phead, idx, nLevel, row_int_array, layer_value_lut) == FALSE)
        {
            return FALSE;
        }
        
        unsigned int idx_image_size;
        for (idx_image_size = 0; idx_image_size < (n_Hnode * n_Vnode); idx_image_size++)
        {
            pinfo->Lut_in[idx][idx_image_size].dbr   = layer_value_lut[0][idx_image_size];
            pinfo->Lut_in[idx][idx_image_size].dbg   = layer_value_lut[1][idx_image_size];
            pinfo->Lut_in[idx][idx_image_size].dbb   = layer_value_lut[2][idx_image_size];

            pinfo->Lut_in[idx][idx_image_size].r   = (int)(layer_value_lut[0][idx_image_size]);
            pinfo->Lut_in[idx][idx_image_size].g   = (int)(layer_value_lut[1][idx_image_size]);
            pinfo->Lut_in[idx][idx_image_size].b   = (int)(layer_value_lut[2][idx_image_size]);            
        }
    }
    
    // dfree Not necessary memory
    dfree(pFile);
    dfree(player_lut);
    dfree(row_int_array);
    
    return TRUE;
}

#endif


MS_BOOL Decode_To_Mstar_Format(void *pDataInfo, BinOutputInfo *pbin_info)
{
    MS_BOOL  bRet;
    CSOT_Demura_Header  CSOT_Header;
    
//=============================== read setting ===============================
    bRet = get_demura_header(&CSOT_Header);
    if (bRet == FALSE)
    {
        printf("get_demura_header error\n");
        return FALSE;
    }
//=============================== End read setting ===============================

    pbin_info->HNode       =  CSOT_Header.HNode;
    pbin_info->VNode       =  CSOT_Header.VNode;
    pbin_info->LevelCount  =  CSOT_Header.LevelCount;

    // Alloc space for Lut_in
    bRet = Alloc_LutIn_Space(pDataInfo, pbin_info);
    if (bRet == FALSE)
    {
        UBOOT_ERROR("Alloc_LutIn_Space error\n");
        return FALSE;
    }


//=============================== read ini_file_table ===============================
    #if defined (CONFIG_DEMURA_URSA11)
        bRet = set_u11_interface(&CSOT_Header, (interface_info *)pDataInfo);
    #elif defined (CONFIG_DEMURA_URSA13)
        bRet = set_u13_interface(&CSOT_Header, (interface_info *)pDataInfo);
    #endif
    if (bRet == FALSE)
    {
        printf("set_demura_u%d_interface error\n", DCONFIG_DEMURA_URSA_STRING);
        return FALSE;
    }
//=============================== End read ini_file_table ===============================

    MS_BOOL demura_enable[3] = {TRUE, TRUE, TRUE};

    // fill the struct pbin_info
    pbin_info->project_id     =  ((MSTAR_CHIP_ID << 16) + (1ULL << 15) + ID_CUS_MSTAR);
    pbin_info->reg_base_addr  =  0x7700;
    pbin_info->data_type      =  IC_DRAM;
    
    if (CSOT_Header.blk_type == 0)
    {
        pbin_info->Blk_h_size   =  8;
        pbin_info->Blk_v_size   =  8;
    }
    else if (CSOT_Header.blk_type == 1)
    {
        pbin_info->Blk_h_size   =  16;
        pbin_info->Blk_v_size   =  16;
    }
    
    pbin_info->Sep_type       =  CSOT_Header.sep_type;
    pbin_info->Gain_type      =  10;
    memcpy(pbin_info->channel_enable, demura_enable, sizeof(demura_enable));

    return TRUE;
}

MS_BOOL If_Need_Decode(void)
{
    if (init_spi_flash() != TRUE)
    {
        UBOOT_ERROR("init_spi_flash error!\n");
        return FALSE;
    }
    return FALSE;
}
