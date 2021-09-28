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
#include <linux/string.h>
#include <CommonDataType.h>
#include <crc_libs.h>
#include <ms_vector.h>
#include <dmalloc.h>
#include <demura.h>
#include <demura_common.h>
#include <demuraDll.h>

#include <apiPNL.h>
#include <MDrvDemura.h>
#include <halDemura.h>


unsigned char *bin_buf;
unsigned char *bin_cur_pos;
MstarSubBinHeader m_MstarSubHeader;
DemuraHeader m_Header;

//---------------------------------------------------------------
MS_U32 StrToHex(const char *psHex, int startIndex)
{
  MS_U32 dwHex = 0;

  if (startIndex == 0)
  {
    if (psHex[0] == '0' && (psHex[1] == 'x' || psHex[1] == 'X'))
    {
      return atoi(psHex);
    }
  }

    int count;

  while (*psHex != '\0')
  {
    char c = *psHex;

    if (c >= '0' && c <= '9')
    {
      c -= '0';
    }
    else if (c >= 'A' && c <= 'F')
    {
      c = c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f')
    {
      c = c - 'a' + 10;
    }

    dwHex = dwHex * 16 + c;
    psHex++;
    count++;
  }

  return dwHex;
}

//---------------------------------------------------------------
int get_block_index(int n_block_size)
{
     switch(n_block_size)
     {
        case 4:
            return 2;

        case 8:
            return 3;

        case 16:
            return 4;

        case 32:
            return 5;

        case 64:
            return 6;

        case 128:
            return 7;

        case 256:
            return 8;

        default:
            return 3;
    }
}
//---------------------------------------------------------------
int get_block_value(int n_block_idx)
{
     switch(n_block_idx)
     {
        case 2:
            return 4;

        case 3:
            return 8;

        case 4:
            return 16;

        case 5:
            return 32;

        case 6:
            return 64;

        case 7:
            return 128;

        case 8:
            return 256;

        default:
            return 8;
    }
}


static MS_U32 calculate_bin_size(BinOutputInfo *pbin)
{
    #define BIN_SIZE_ALIGNMENT  0x1000
    #define BIN_SIZE_ALIGNMASK  (BIN_SIZE_ALIGNMENT - 1)

    MS_U32 bin_size, header_size;
    MS_U32 reg_size, layer_size;

    header_size = sizeof(MstarSubBinHeader);
    layer_size  = HAL_DEMURA_Cal_LutSize(pbin->LevelCount, pbin->HNode, pbin->VNode, pbin->Sep_type);
    reg_size    = HAL_DEMURA_Max_RegSize();
    bin_size    = header_size + layer_size + reg_size;

    bin_size   += ((bin_size * 20)/100);    // redundancy space
    if (bin_size & BIN_SIZE_ALIGNMASK)
    {
        bin_size +=  BIN_SIZE_ALIGNMENT;
        bin_size &= (~BIN_SIZE_ALIGNMASK);
    }
    printf("calculate bin size = 0x%x\n", (unsigned int)bin_size);
    return bin_size;
}


//---------------------------------------------------------------
MS_BOOL mstar_demura_interface (interface_info *Info1, BinOutputInfo *pbin_info)
{
    //Inital Value
    int i;
    int m_alignment;
    MS_BOOL ret;

    MS_U32  project_id, reg_base_addr;
    LAYER_DATA_TYPE lay_dat_type;
    MS_U32  lut_h_size, lut_v_size;
    MS_BOOL *channel_enable;

    project_id      =  pbin_info->project_id;
    reg_base_addr   =  pbin_info->reg_base_addr;
    lay_dat_type    =  pbin_info->data_type;
    lut_h_size      =  pbin_info->HNode;
    lut_v_size      =  pbin_info->VNode;
    channel_enable  =  pbin_info->channel_enable;

    m_alignment = Aligned_Value - (sizeof(MstarSubBinHeader)%Aligned_Value);    
    if (m_alignment == Aligned_Value)
    {
        m_alignment = 0;    
    }

    memset((void *)&m_MstarSubHeader, 0, sizeof(MstarSubBinHeader));
    memset((void *)&m_Header, 0, sizeof(DemuraHeader));
    
    /******************************* begin to covert *****************************************/

    int lut_out_size = calculate_bin_size(pbin_info);
    m_vector  Lut_out_vector;
    
    ret = alloc_vector(1, lut_out_size, &Lut_out_vector);
    if (ret == FALSE)
    {
        printf("alloc_vector for Lut_out_vector failed\n");
        return FALSE;
    }
    
    int reg_num  = HAL_DEMURA_Max_RegCount();
    m_vector  Reg_vector;
    ret = alloc_vector(sizeof(reg_struct), reg_num, &Reg_vector);
    if (ret == FALSE)
    {
        printf("alloc_vector for Reg_vector failed\n");
        return FALSE;
    }
    
    demura_core_ver10(reg_base_addr, lut_h_size, lut_v_size, Info1, &Reg_vector, &Lut_out_vector);
    /***************************************************************************************/


    /***************************** prepare to output data ************************************/
    int bin_size = calculate_bin_size(pbin_info);
    m_vector Bin_vector;
    ret = alloc_vector(1, bin_size, &Bin_vector);
    if (ret == FALSE)
    {
        printf("alloc_vector for Bin_vector failed\n");
        return FALSE;
    }
    bin_buf = Bin_vector.pbuf;
    bin_cur_pos = bin_buf;
    /***************************************************************************************/


    int packet_type = 0;  // 0:128bits, 1:256bits
    // packect size
    switch (project_id >> 16)
    {
        case ID_CHIP_U9:
            packet_type = 1;
            break;

        case ID_CHIP_U10:
            packet_type = 0;
            break;

        case ID_CHIP_U11:
            packet_type = 1;
            break;

        case ID_CHIP_U12:
            packet_type = 1;
            break;

        case ID_CHIP_TERRA:
            packet_type = 0;
            break;
        default:
            break;
    }
    packet_type = HAL_DEMURA_Get_PacketType();
    //int packet_size = 1;

    /********************************** layer data **********************************/
    // align to 16 byte
    bin_cur_pos = bin_buf + (sizeof(MstarSubBinHeader) + m_alignment + CusDataSize);
    memset(bin_buf, 0, (MS_U32)(bin_cur_pos - bin_buf));

    int DataLength = Lut_out_vector.dat_num;     // Write Data length
    MS_U8* Lut_data = Lut_out_vector.pbuf;
    memcpy(bin_cur_pos, Lut_data, DataLength);
    bin_cur_pos += DataLength;

    /*******************************************************************************/

    //check format, if compressed than do it
    int gzip_size = DataLength;
    
    /****************************** write Register ********************************/
    RegData RegisterData;
    int registerCount = Reg_vector.dat_num;
    reg_struct *reg_data  = (reg_struct *)Reg_vector.pbuf;
    
    for (i = 0 ; i < registerCount; i++)
    {        
        RegisterData.RegAddr  = reg_data[i].address;
        RegisterData.RegValue = reg_data[i].value;
        RegisterData.RegMask  = reg_data[i].mask;
        
        memcpy(bin_cur_pos, &RegisterData, sizeof(RegData));
        bin_cur_pos += sizeof(RegData);
    }
    /*******************************************************************************/

    //write SubHeader info
    m_MstarSubHeader.nAllBinCheckSum = 0;
    m_MstarSubHeader.nHeaderCheckSum = 0;
    
    m_MstarSubHeader.nHeaderSize = sizeof(MstarSubBinHeader);        
    m_MstarSubHeader.nAllBinSize = sizeof(MstarSubBinHeader) + m_alignment + gzip_size + (registerCount * sizeof(RegisterData));

    m_MstarSubHeader.nDemuraIdH = sw8Byte(0x6D73746172206465);  //Identification for a demura bin: "mstar demura", not be little endian
    m_MstarSubHeader.nDemuraIdL = sw8Byte(0x6D75726100000000);  //Hex values: 6D 73 74 61 72 20 64 65 6D 75 72 61 00 00 00 00
    m_MstarSubHeader.nBinVersion = BinVersion;
 
    m_MstarSubHeader.nDataFormat = lay_dat_type;
    m_MstarSubHeader.bR_ch_Enable = channel_enable[0];
    m_MstarSubHeader.bG_ch_Enable = channel_enable[1];
    m_MstarSubHeader.bB_ch_Enable = channel_enable[2];

    m_MstarSubHeader.nReserved = 0;
    
    //calculate H node
    int Reg_H_Node = lut_h_size;
    int H_offset = 1;;
        
    // reg_dmc_rgb_mode = 0 => ymode
    // reg_dmc_rgb_mode = 1 => RGB Mode
    if (Info1->reg_dmc_rgb_mode == 1)
    {        
        switch(Info1->reg_dmc_plane_num)
        {
        case 1:
        case 2:
            H_offset = 4;
            break;
        case 3:
        case 4:
            H_offset = 2;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            H_offset = 1;
            break;
        } 
    }
    else
    {
        switch(Info1->reg_dmc_plane_num)
        {
        case 1:
        case 2:
            H_offset = 16;
            break;
        case 3:
        case 4:
            H_offset = 8;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            H_offset = 4;
            break;
        } 
    }

    Reg_H_Node = (lut_h_size / H_offset) + (((lut_h_size % H_offset) == 0)?0:1);

    m_MstarSubHeader.nHNodeCount = Reg_H_Node;
    m_MstarSubHeader.nVNodeCount = lut_v_size;

/*
    //Get Start Layer
    int nLayerStart = 0;
    int nLayerEnd = 0;
    
    int color = 0;
    for (color = 0 ; color < 3; color++)
    {
        if (channel_enable[color] == TRUE)
        {
            break;
        }
    }
*/

    m_MstarSubHeader.nLayerStart = 1;
    m_MstarSubHeader.nLayerEnd = Info1->reg_dmc_plane_num;
    m_MstarSubHeader.bSeparate = Info1->reg_dmc_rgb_mode;
        
    m_MstarSubHeader.nLDataStartAddr = sizeof(MstarSubBinHeader) + m_alignment + CusDataSize;
    m_MstarSubHeader.nLDataOriginalSize = DataLength;
    m_MstarSubHeader.nLDataFlashSize = gzip_size;

    m_MstarSubHeader.nRegisterStartAddr = sizeof(MstarSubBinHeader) + m_alignment + gzip_size + CusDataSize;
    m_MstarSubHeader.nRegisterCount = registerCount;
       
    m_MstarSubHeader.nDllVersion = DllVersion;
    m_MstarSubHeader.nProjectID = project_id;
        
    m_MstarSubHeader.nCusDataStartAddr = 0x00000000;
    m_MstarSubHeader.nCusDataSize = CusDataSize;//0x00000000;

    m_MstarSubHeader.nHBlockSize = Info1->reg_dmc_h_block;
    m_MstarSubHeader.nVBlockSize = Info1->reg_dmc_v_block;
    m_MstarSubHeader.nPacketSize = packet_type;

    //get date
    if (pbin_info->Build_Date == 0x00)
    {
        char NowDate[20];
        sprintf(NowDate,"%02d%02d%02d%02d", 17, 5, 5, 10);
        m_MstarSubHeader.nDate = sw4Byte(StrToHex(NowDate, 0));
    }
    else
    {
        m_MstarSubHeader.nDate = sw4Byte(pbin_info->Build_Date);
    }
    
    // update Demura_bin_size
    m_MstarSubHeader.nAllBinSize = (int)(bin_cur_pos - bin_buf);
    
    // CRC32 Demura_bin_header
    m_MstarSubHeader.nHeaderCheckSum = MDrv_CRC32_Cal_DeMura((MS_U8*)&m_MstarSubHeader + 8, sizeof(MstarSubBinHeader)-8);
    
    //write Mstar SubHeader, to make sure 'CRC32 Demura_all_bin' OK
    memcpy(bin_buf, &m_MstarSubHeader, sizeof(m_MstarSubHeader));

    // CRC32 Demura_all_bin
    m_MstarSubHeader.nAllBinCheckSum = MDrv_CRC32_Cal_DeMura(bin_buf + 4, m_MstarSubHeader.nAllBinSize-4);
    
    //write Mstar SubHeader
    memcpy(bin_buf, &m_MstarSubHeader, sizeof(m_MstarSubHeader));
    
    pbin_info->bin_buf  = bin_buf;
    pbin_info->bin_size = m_MstarSubHeader.nAllBinSize;
    
    return TRUE;
}
//---------------------------------------------------------------
