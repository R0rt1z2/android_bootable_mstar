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
#include <malloc.h>
#include <stdio.h>
#include <MsDebug.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsStr.h>

#include <dmalloc.h>
#include <CommonDataType.h>
#include <demura.h>
#include <demuraDll.h>
#include <demura_config.h>
#include <ms_utils.h>
#include <vendor/decoder_input.h>


MS_BOOL init_demura_heap(void)
{
    MS_U32 base_addr = 0;
    MS_U32 buf_len   = CONVERT_BUFFER_SIZE;

    if (get_large_memory(&base_addr, buf_len) == FALSE)
    {
        UBOOT_ERROR("failed to get Demura_Convert_Buf\n");
        return FALSE;
    }

    // init dmalloc buffer
    dmalloc_init((unsigned char *)base_addr, buf_len);
    return TRUE;
}


MS_BOOL Alloc_LutIn_Space(void *pdat_info, BinOutputInfo *pbin_info)
{
    int n_Hnode     = pbin_info->HNode;
    int n_Vnode     = pbin_info->VNode;
    int nTotalNode  = n_Hnode * n_Vnode;
    int level_count = pbin_info->LevelCount;

    if ((n_Hnode == 0) || (n_Vnode == 0) || (level_count == 0))
    {
        UBOOT_ERROR("n_Hnode = %d, n_Vnode = %d, level_count = %d\n", n_Hnode, n_Vnode, level_count);
        UBOOT_ERROR("Alloc_LutIn_Space Failed !\n");
        return FALSE;
    }

    #if defined (CONFIG_DEMURA_URSA11)
        int n_Layer     = 6;     // Wait to fix it.
        int layer_size  = sizeof(double) * nTotalNode;
        interface_info *pChanData = (interface_info *)pdat_info;

        int idx, channel;
        for(channel = 0; channel < 3; channel++)
        {
            double **lut_in  = (double **)malloc(sizeof(double *) * 6);

            for(idx = 0; idx < n_Layer; idx++)
            {
                char *pLut_in = dmalloc(layer_size);
                CHECK_DMALLOC_SPACE(pLut_in, layer_size);
                lut_in[idx] = (double *)pLut_in;
            }
            pChanData[channel].Lut_in = lut_in; 
        }

    #elif defined (CONFIG_DEMURA_URSA13)
        int n_Layer     = level_count > 0 ? level_count : 8;
        int layer_size  = sizeof(strgb_structInfo) * nTotalNode;
        interface_info *pDataInfo = (interface_info *)pdat_info;

        int idx;
        for (idx = 0; idx < n_Layer; idx++)
        {
            char *pLut_in  = (char *)dmalloc(layer_size);
            CHECK_DMALLOC_SPACE(pLut_in, layer_size);
            pDataInfo->Lut_in[idx] = (strgb_structInfo *)pLut_in;
        }
        for (idx = n_Layer; idx < 8; idx++)
        {
            pDataInfo->Lut_in[idx] = NULL;   // Init to NULL value
        }

    #endif
    return TRUE;
}


int do_demura_convert(BinOutputInfo *pbin_info)
{
    MS_BOOL ret;
    //char  cmd[256];

    if (check_str_resume() == 1)
    {
        //UBOOT_DEBUG("Can not covert demura file during STR\n");
        return 0;
    }

    UBOOT_DEBUG("\n=======================================\n");
    UBOOT_DEBUG("DEMURA_URSA_TYPE : U%s\n", CONFIG_DEMURA_URSA_STRING);
    UBOOT_DEBUG("DEMURA_VENDOR    : %s\n",  CONFIG_DEMURA_VENDOR_STRING);
    UBOOT_DEBUG("=======================================\n");

    if (init_spi_flash() != TRUE)
    {
        UBOOT_ERROR("init_spi_flash error!\n");
        return -1;
    }

//================================ Generate Demura Bin ===============================

    #if defined (CONFIG_DEMURA_URSA11)
        interface_info DataInfo[3];
    #elif defined (CONFIG_DEMURA_URSA13)
        interface_info DataInfo;
    #else
        #error "Unkown DEMURA_URSA_TYPE !"
    #endif
    
    memset(&DataInfo, 0, sizeof(DataInfo));

    // Ready to standard covert flow, Now comment .
    // Alloc_LutIn_Space(pdat_info, pbin_info)

    ret = Decode_To_Mstar_Format((void *)&DataInfo, pbin_info);
    if (ret == FALSE)
    {
        printf("Decode_To_Mstar_Format error\n");
        return -1;
    }

    mstar_demura_interface((interface_info *)&DataInfo, pbin_info);

//================================ End Generate Demura Bin ===============================

    MS_U8  *pout = pbin_info->bin_buf;
    MS_U32 fsize = pbin_info->bin_size;
    
    printf("output_addr    = 0x%p\n", pout);
    printf("bin_size       = 0x%x\n", (uint)fsize);
    printf("dmalloc_length = 0x%x\n", get_dmalloc_length());

    // Enable SPI_FLASH - Write protect 
    /*memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "sf protect lock");
    if (run_command(cmd, 0) != 0)
    {
      printf("Enable SPI_FLASH Write protect Fail !\n");
    }
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "sf protect status");
    run_command(cmd, 0);
    */
    printf("\n");
    return 0;
}

