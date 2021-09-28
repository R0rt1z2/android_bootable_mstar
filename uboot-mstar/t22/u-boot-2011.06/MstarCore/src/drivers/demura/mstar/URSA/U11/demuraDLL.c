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


typedef union
{
    unsigned char byte;
    struct
    {
        unsigned char b0:7;
        unsigned char b1:1;        
    }bit;    
}DATA;

unsigned char *bin_buf;
unsigned char *bin_cur_pos;
MstarSubBinHeader m_MstarSubHeader;
DemuraHeader m_Header;


//---------------------------------------------------------------
void GetSELayer(int *nLStart, int *nLEnd, int layer0_en, int layer1_en, int layer2_en, int layer3_en, int layer4_en, int layer5_en)
{
    int LayerEn[6] = {layer0_en, layer1_en, layer2_en, layer3_en, layer4_en, layer5_en};

    int number;
    for (number = 0; number < 6; number++)
    {
        if(LayerEn[number] == 1)
        {
            *nLStart = number;
            break;
        }
    }

    if (LayerEn[5] == 1)
    {
        *nLEnd = 5;
    }
    else
    {
        for (number = *nLStart; number < 6; number++)
        {
            if(LayerEn[number] == 0)
            {
                *nLEnd = number - 1;
                break;
            }
        }
    }    
}
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
int cal_luma_slope(const int compensate_value, const int cutoff_value, const int bits)
{
     int diff = abs( compensate_value - cutoff_value);

     diff = (diff == 0) ? 1 : diff;
     
     int numerator = (1<<(bits - 2));

     int rounding = ((numerator%diff) != 0) ? 1 : 0;

     int slope = (int)(numerator/diff) + rounding;

     slope = minmax_( slope, 1, 15);

     return slope;
}
//---------------------------------------------------------------
int get_block_index(int n_block_size)
{
     switch(n_block_size)
     {
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


MS_BOOL mstar_demura_interface (interface_info *Info, BinOutputInfo *pbin_info)
{
    //Inital Value
    int i, channel;
    int m_alignment;
    MS_BOOL ret;

    MS_U32  project_id, reg_base_addr;
    LAYER_DATA_TYPE lay_dat_type;
    MS_U32  lut_h_size, lut_v_size;
    MS_U32  block_h_size, block_v_size;
    MS_BOOL *channel_enable;
    MS_U32  gain_type;
    int y_mode_en = -1;
    
    if (pbin_info->Sep_type == 0)
    {
        y_mode_en = 1;
    }
    else if (pbin_info->Sep_type == 1)
    {
        y_mode_en = 0;
    }

    project_id      =  pbin_info->project_id;
    reg_base_addr   =  pbin_info->reg_base_addr;
    lay_dat_type    =  pbin_info->data_type;
    lut_h_size      =  pbin_info->HNode;
    lut_v_size      =  pbin_info->VNode;
    block_h_size    =  pbin_info->Blk_h_size;
    block_v_size    =  pbin_info->Blk_v_size;
    channel_enable  =  pbin_info->channel_enable;
    gain_type       =  pbin_info->Gain_type;
    int y_mode      = (y_mode_en > 0)?1:y_mode_en;

    m_alignment = Aligned_Value - (sizeof(MstarSubBinHeader)%Aligned_Value);
    if (m_alignment == Aligned_Value)
    {
        m_alignment = 0;
    }

    memset((void *)&m_MstarSubHeader, 0, sizeof(MstarSubBinHeader));
    memset((void *)&m_Header, 0, sizeof(DemuraHeader));

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

        case ID_CHIP_MASERATI:
            packet_type = 1;
            break;

        case ID_CHIP_TERRA:
            packet_type = 0;
            break;
        default:
            break;
    }
    packet_type = HAL_DEMURA_Get_PacketType();

    int packet_size = (packet_type == 0)?1:2;
    int y_mode_size = 3;


    /******************************* begin to covert *****************************************/
    
    // prepare space
    int reg_num  = HAL_DEMURA_Max_RegCount();  //Common_regList
    m_vector  ComRegVector, RegVector, TotalRegVector;
    
    ret = alloc_vector(sizeof(reg_struct), reg_num, &ComRegVector);
    if (ret == FALSE)
    {
        printf("alloc_vector for ComRegVector failed\n");
        return FALSE;
    }
    ret = alloc_vector(sizeof(reg_struct), reg_num, &RegVector);
    if (ret == FALSE)
    {
        printf("alloc_vector for RegVector failed\n");
        return FALSE;
    }
    ret = alloc_vector(sizeof(reg_struct), reg_num, &TotalRegVector);
    if (ret == FALSE)
    {
        printf("alloc_vector for TotalRegVector failed\n");
        return FALSE;
    }
    
    
    for (channel = 0; channel < 3; channel++)
    {
        int nMin_Layer_Level = 0;
        int nMax_Layer_Level = 0;
                
        GetSELayer(&nMin_Layer_Level, &nMax_Layer_Level, Info[channel].bLayer_level_en[0], Info[channel].bLayer_level_en[1], Info[channel].bLayer_level_en[2], 
                   Info[channel].bLayer_level_en[3], Info[channel].bLayer_level_en[4], Info[channel].bLayer_level_en[5]);
        
        int nLow_luma = (Info[channel].iLow_luma_thrd == 255)?1023:(Info[channel].iLow_luma_thrd * 4);
        int nHigh_luma = (Info[channel].iHigh_luma_thrd == 255)?1023:(Info[channel].iHigh_luma_thrd * 4);
        
        // transfer 8 bit to 10 bit            
        if (gain_type == 8)
        {
            int layer;
            for (layer = 0; layer < 6; layer++)
            {
                Info[channel].iLayer_level[layer] = (Info[channel].iLayer_level[layer] == 255)?1023:(Info[channel].iLayer_level[layer] << 2);
            }
        }
        
        Info[channel].iLow_luma_slope  = cal_luma_slope(Info[channel].iLayer_level[nMin_Layer_Level], nLow_luma, 10);
        Info[channel].iHigh_luma_slope = cal_luma_slope(Info[channel].iLayer_level[nMax_Layer_Level], nHigh_luma, 10);
        
        clear_vector(&ComRegVector);    // Register Table for Common
        clear_vector(&RegVector);       // Register Table for R, G, B
        
        demura_core_ver00(reg_base_addr, lut_h_size, lut_v_size, block_h_size, block_v_size, y_mode, &(Info[channel]), &ComRegVector, &RegVector, packet_size);
        merge_vector(&TotalRegVector, &RegVector);
    }
    
    // copy common register list to total regList
    merge_vector(&TotalRegVector, &ComRegVector);
    free_vector(&ComRegVector);
    free_vector(&RegVector);
    
    
    /***************************** prepare to output data ************************************/
    MS_U32 bin_size = calculate_bin_size(pbin_info);
    bin_buf = dmalloc(bin_size);
    CHECK_DMALLOC_SPACE(bin_buf, (unsigned int)bin_size);
    
    bin_cur_pos = bin_buf;
    /***************************************************************************************/
    
    
    /********************************** layer data **********************************/
    // align to 16 byte
    bin_cur_pos = bin_buf + (sizeof(MstarSubBinHeader) + m_alignment + CusDataSize);
    memset(bin_buf, 0, (MS_U32)(bin_cur_pos - bin_buf));
    
    DATA temp;
    MS_U16 data = 0;
    int DataLength = 0;            // Write Data length
    
    
    if (y_mode_en == 1)
    {
        int y_channel = -1;
        int channel;
        for (channel = 0; channel < 3; channel++)
        {
            if (channel_enable[channel] == TRUE)
            {
                y_channel = channel;
                break;
            }
        }

        if (y_channel == -1)
        {
            return FALSE;
        }

        unsigned int v, h;
        for (v = 0; v < lut_v_size; v++)
        {        
            data = 0;                
            int bitCount = 0;
            int y_Node = 0;
            
            for (h = 0 ; h < lut_h_size; h++)
            {    
                //layer n
                int layer;
                for (layer = 0; layer < 6; layer++)
                {
                    temp.byte = (char)(Info[y_channel].Lut_out[layer][v*lut_h_size + h]);
                    data = data + (temp.bit.b0 << bitCount);
                    
                    bitCount = bitCount + 7;
                    
                    if (bitCount >= 8)
                    {
                        *bin_cur_pos++ = (char)data;
                        data = data >> 8;
                        bitCount = bitCount - 8;
                        DataLength++;
                    }
                }
                 
                y_Node++;

                if ((y_Node % y_mode_size) == 0)
                {
                    //C_n
                    temp.byte = 2;
                    data = data + (temp.bit.b0 << bitCount);

                    bitCount = bitCount + 2;
    
                    if (bitCount >= 8)
                    {
                        *bin_cur_pos++ = (char)data;
                        data = data >> 8;
                        bitCount = bitCount - 8;
                        DataLength++;
                    }
                }
            }

            while ((y_Node % (packet_size * y_mode_size)) != 0)
            {
                int nLayer = 0;
    
                while(nLayer < 6)
                {
                    //layer n
                    bitCount = bitCount + 7;
    
                    if (bitCount >= 8)
                    {
                        *bin_cur_pos++ = (char)data;
                        data = data >> 8;
                        bitCount = bitCount - 8;
                        DataLength++;
                    }
                
                    nLayer++;
                }

                y_Node++;

                if ((y_Node % y_mode_size) == 0)
                {
                    //C_n
                    temp.byte = 2;
                    data = data + (temp.bit.b0 << bitCount);
    
                    bitCount = bitCount + 2;
        
                    if (bitCount >= 8)
                    {
                        *bin_cur_pos++ = (char)data;
                        data = data >> 8;
                        bitCount = bitCount - 8;
                        DataLength++;
                    }
                }
            }
        }
    }
    else if (y_mode_en == 0)
    {
        unsigned int v;
        for (v = 0; v < lut_v_size; v++)
        {
            data = 0;                
            int bitCount = 0;    
            int n_Node = 0;
            
            unsigned int h;
            for (h = 0 ; h < lut_h_size; h++)
            {
                int channel, layer;
                for (channel = 2; channel >= 0; channel--)
                {
                    for (layer = 0; layer < 6; layer++)
                    {
                        //layer n
                        temp.byte = (char)(Info[channel].Lut_out[layer][v*lut_h_size + h]);
                        data = data + (temp.bit.b0 << bitCount);
                        
                        bitCount = bitCount + 7;
                        
                        if (bitCount >= 8)
                        {
                            *bin_cur_pos++ = (char)data;
                            data = data >> 8;
                            bitCount = bitCount - 8;
                            DataLength++;
                        }
                    }
                }
                
                n_Node++;

                //C_n
                temp.byte = 2;
                data = data + (temp.bit.b0 << bitCount);
                bitCount = bitCount + 2;

                if (bitCount >= 8)
                {
                    *bin_cur_pos++ = (char)data;
                    data = data >> 8;
                    bitCount = bitCount - 8;
                    DataLength++;
                }    
            }

            while ((n_Node % packet_size) != 0)
            {
                int channel;
                for (channel = 0; channel < 3; channel++)
                {
                    int nLayer = 0;
    
                    while(nLayer < 6)
                    {
                        //layer n
                        bitCount = bitCount + 7;
        
                        if (bitCount >= 8)
                        {
                            *bin_cur_pos++ = (char)data;
                            data = data >> 8;
                            bitCount = bitCount - 8;
                            DataLength++;
                        }
                        nLayer++;
                    }
                }

                n_Node++;

                //C_n
                temp.byte = 2;
                data = data + (temp.bit.b0 << bitCount);
                bitCount = bitCount + 2;
        
                if (bitCount >= 8)
                {
                    *bin_cur_pos++ = (char)data;
                    data = data >> 8;
                    bitCount = bitCount - 8;
                    DataLength++;
                }
            }
        }
    }
    else
    {
        printf("can not support y_mode_en(%d)\n", y_mode_en);
        return FALSE;
    }
    
    //check format, if compressed than do it
    int gzip_size = DataLength;

    //write Register
    RegData RegisterData;
    int registerCount     = TotalRegVector.dat_num;
    reg_struct *reg_data  = (reg_struct *)TotalRegVector.pbuf;
    
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
    
    if (y_mode == 1)
    {
        m_MstarSubHeader.nHNodeCount = (lut_h_size/(packet_size * y_mode_size)) + (((lut_h_size%(packet_size *y_mode_size)) != 0)?1:0);
    }
    else if (y_mode == -1)
    {
        m_MstarSubHeader.nHNodeCount = lut_h_size;
    }
    else
    {
        m_MstarSubHeader.nHNodeCount = (lut_h_size/packet_size) + (((lut_h_size%packet_size) != 0)?1:0);
    }
    m_MstarSubHeader.nVNodeCount = lut_v_size;

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

    GetSELayer(&nLayerStart, &nLayerEnd, Info[color].bLayer_level_en[0], Info[color].bLayer_level_en[1], Info[color].bLayer_level_en[2],
               Info[color].bLayer_level_en[3], Info[color].bLayer_level_en[4], Info[color].bLayer_level_en[5]);

    m_MstarSubHeader.nLayerStart = nLayerStart;
    m_MstarSubHeader.nLayerEnd = nLayerEnd;
    if (y_mode == 1)
    {
        m_MstarSubHeader.bSeparate = 0;
    }
    else
    {
        m_MstarSubHeader.bSeparate = 1;
    }
    
    m_MstarSubHeader.nLDataStartAddr = sizeof(MstarSubBinHeader) + m_alignment + CusDataSize;
    m_MstarSubHeader.nLDataOriginalSize = DataLength;
    m_MstarSubHeader.nLDataFlashSize = gzip_size;

    m_MstarSubHeader.nRegisterStartAddr = sizeof(MstarSubBinHeader) + m_alignment + gzip_size + CusDataSize;
    m_MstarSubHeader.nRegisterCount = registerCount;
       
    m_MstarSubHeader.nDllVersion = DllVersion;
    m_MstarSubHeader.nProjectID = project_id;
        
    m_MstarSubHeader.nCusDataStartAddr = 0x00000000;
    m_MstarSubHeader.nCusDataSize = CusDataSize;//0x00000000;

    m_MstarSubHeader.nHBlockSize = get_block_index(block_h_size);
    m_MstarSubHeader.nVBlockSize = get_block_index(block_v_size);
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