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
#ifndef _DEMURA_H_
#define _DEMURA_H_

#define max_(a,b)        ((a>b) ? a : b)
#define min_(a,b)        ((a<b) ? a : b)
#define minmax_(a,b,c)   (max_((min_(a,c)),b))
#define abs(a)           ((a) >= 0 ? (a) : (-(a)))
#define posi_blk(CurV,CurH)     (CurV*block_width+CurH)

#define Get_Demura_Inv             0
#define DEMURA_MAX_LAYER_NUM       6
#define DEMURA_MAX_CHANNEL_NUM     3    // R/G/B


#define sw8Byte(x) \
    ((MS_U64)( \
        (((MS_U64)(x) & (MS_U64)0x00000000000000ff) << 56) | \
        (((MS_U64)(x) & (MS_U64)0x000000000000ff00) << 40) | \
        (((MS_U64)(x) & (MS_U64)0x0000000000ff0000) << 24) | \
        (((MS_U64)(x) & (MS_U64)0x00000000ff000000) << 8) | \
        (((MS_U64)(x) & (MS_U64)0x000000ff00000000) >> 8) | \
        (((MS_U64)(x) & (MS_U64)0x0000ff0000000000) >> 24) | \
        (((MS_U64)(x) & (MS_U64)0x00ff000000000000) >> 40) | \
        (((MS_U64)(x) & (MS_U64)0xff00000000000000) >> 56)))

#define sw5Byte(x) \
    ((MS_U64)( \
        (((MS_U64)(x) & (MS_U64)0x00000000ff) << 32) | \
        (((MS_U64)(x) & (MS_U64)0x000000ff00) << 16) | \
        ((MS_U64)(x) & (MS_U64)0x0000ff0000) | \
        (((MS_U64)(x) & (MS_U64)0x00ff000000) >> 16) | \
        (((MS_U64)(x) & (MS_U64)0xff00000000) >> 32)))

#define sw4Byte(x) \
    ((MS_U32)( \
        (((MS_U32)(x) & (MS_U32)0x000000ff) << 24) | \
        (((MS_U32)(x) & (MS_U32)0x0000ff00) << 8) | \
        (((MS_U32)(x) & (MS_U32)0x00ff0000) >> 8) | \
        (((MS_U32)(x) & (MS_U32)0xff000000) >> 24)))

#define sw2Byte(x) \
    ((MS_U16)( \
        (((MS_U16)(x) & (MS_U16)0x00ff) << 8) | \
        (((MS_U16)(x) & (MS_U16)0xff00) >> 8)))

#define BinVersion           ((0 << 8) + 6)
#define DllVersion           ((0 << 8) + 2)
#define HNodeCount           481
#define VNodeCount           271
#define HNodeCount_U10       241
#define VNodeCount_U10       136
#define MAX_INX_BIN_LENGTH   (512 * 1024) //512k

#define MAX_BIN_LENGTH       (4 *1024 * 1024) // 4MB

#define Aligned_Value        16

#define BlockHSize  8
#define BlockVSize  8

#define CusDataSize 0 //0x100

//----------------------- Chip device ID start -----------------------//
#define ID_CHIP_U9          0x003D  // Ursa  9 (Unicorn)
#define ID_CHIP_U10         0x0090  // Ursa 10 (Uniqlo)
#define ID_CHIP_U11         0x0094  // Ursa 11 (Ultrasonic)
#define ID_CHIP_U12         0x008F  // Ursa 12 (UBike)
#define ID_CHIP_TERRA       0x005F  // Tcon Terra
#define ID_CHIP_MASERATI    0x00A4  // Soc Maserati
//----------------------- Chip device ID end -----------------------//

//----------------------- Customer ID start -----------------------//
#define ID_CUS_MSTAR        0x0000
#define ID_CUS_AUO          0x0100
#define ID_CUS_INL          0x0200
#define ID_CUS_HISENSE      0x0300
#define ID_CUS_BOE          0x1100
#define ID_CUS_PANDA        0x1200
#define ID_CUS_HKC          0x1E00
//----------------------- Customer ID end -----------------------//


typedef struct rgb_struct 
{
    int r;
    int g;
    int b;
    double dbr;
    double dbg;
    double dbb;
}strgb_structInfo, *LPrgb_structInfo;

typedef struct layer_info_struct 
{
    int dram_h_size;
    int dram_v_size;
    int block_h_size;
    int block_v_size;
    int dmc_plane_num;
    int dmc_rgb_mode;
    int real_dram_h_size;
    int real_dram_v_size;
}stlayer_info_structInfo, *LPlayer_info_structInfo;

typedef struct
{
    int address;
    int mask;
    int value;
    char *description;
}reg_struct;

typedef struct
{
    reg_struct Bit07_00;
}_08bits;

typedef struct
{
    reg_struct Bit07_00;
    reg_struct Bit15_08;
}_16bits;

typedef struct
{
    reg_struct Bit07_00;
    reg_struct Bit15_08;
    reg_struct Bit23_16;
}_24bits;

typedef struct
{
    
    _08bits reg_layer0_level_en;     //  1 bit
    _16bits reg_layer0_level;        // 10 bits

    _08bits reg_layer1_level_en;     //  1 bit
    _16bits reg_layer1_level;        // 10 bits

    _08bits reg_layer2_level_en;     //  1 bit
    _16bits reg_layer2_level;        // 10 bits

    _08bits reg_layer3_level_en;     //  1 bit
    _16bits reg_layer3_level;        // 10 bits

    _08bits reg_layer4_level_en;     //  1 bit
    _16bits reg_layer4_level;        // 10 bits

    _08bits reg_layer5_level_en;     //  1 bit
    _16bits reg_layer5_level;        // 10 bits

    _24bits reg_int0_inv;            // 20 bits
    _24bits reg_int1_inv;            // 20 bits
    _24bits reg_int2_inv;            // 20 bits
    _24bits reg_int3_inv;            // 20 bits
    _24bits reg_int4_inv;            // 20 bits
    _24bits reg_int5_inv;            // 20 bits
    _24bits reg_int6_inv;            // 20 bits

    //_08bits reg_en;                  //  1 bit

    _08bits reg_gain_y0;             //  3 bits
    _08bits reg_gain_y1;             //  3 bits
    _08bits reg_gain_y2;             //  3 bits
    _08bits reg_gain_y3;             //  3 bits
    _08bits reg_gain_y4;             //  3 bits
    _08bits reg_gain_y5;             //  3 bits

    _08bits reg_low_luma_en;         //  1 bit
    _08bits reg_low_luma_thrd;       //  7 bits
    _08bits reg_low_luma_slope;      //  4 bits
    _08bits reg_low_luma_min_weight; //  4 bits

    _08bits reg_high_luma_en;         //  1 bit
    _08bits reg_high_luma_thrd;       //  7 bits
    _08bits reg_high_luma_slope;      //  4 bits
    _08bits reg_high_luma_min_weight; //  4 bits
} registers;


typedef struct
{

    _16bits reg_dmc_black_limit;     // 12 bit
    _16bits reg_dmc_plane_level1;    // 12 bit
    _16bits reg_dmc_plane_level2;    // 12 bit
    _16bits reg_dmc_plane_level3;    // 12 bit
    _16bits reg_dmc_plane_level4;    // 12 bit
    _16bits reg_dmc_plane_level5;    // 12 bit
    _16bits reg_dmc_plane_level6;    // 12 bit
    _16bits reg_dmc_plane_level7;    // 12 bit
    _16bits reg_dmc_plane_level8;    // 12 bit
    _16bits reg_dmc_white_limit;     // 12 bit
    _08bits reg_dmc_data_r_mag4;     //  3 bit
    _08bits reg_dmc_data_r_mag3;     //  3 bit
    _08bits reg_dmc_data_r_mag2;     //  3 bit
    _08bits reg_dmc_data_r_mag1;     //  3 bit
    _08bits reg_dmc_data_r_mag8;     //  3 bit
    _08bits reg_dmc_data_r_mag7;     //  3 bit
    _08bits reg_dmc_data_r_mag6;     //  3 bit
    _08bits reg_dmc_data_r_mag5;     //  3 bit
    _08bits reg_dmc_data_g_mag4;     //  3 bit
    _08bits reg_dmc_data_g_mag3;     //  3 bit
    _08bits reg_dmc_data_g_mag2;     //  3 bit
    _08bits reg_dmc_data_g_mag1;     //  3 bit
    _08bits reg_dmc_data_g_mag8;     //  3 bit
    _08bits reg_dmc_data_g_mag7;     //  3 bit
    _08bits reg_dmc_data_g_mag6;     //  3 bit
    _08bits reg_dmc_data_g_mag5;     //  3 bit
    _08bits reg_dmc_data_b_mag4;     //  3 bit
    _08bits reg_dmc_data_b_mag3;     //  3 bit
    _08bits reg_dmc_data_b_mag2;     //  3 bit
    _08bits reg_dmc_data_b_mag1;     //  3 bit
    _08bits reg_dmc_data_b_mag8;     //  3 bit
    _08bits reg_dmc_data_b_mag7;     //  3 bit
    _08bits reg_dmc_data_b_mag6;     //  3 bit
    _08bits reg_dmc_data_b_mag5;     //  3 bit
    _16bits reg_dmc_plane_b1_coef;   // 14 bit
    _16bits reg_dmc_plane_12_coef;   // 14 bit
    _16bits reg_dmc_plane_23_coef;   // 14 bit
    _16bits reg_dmc_plane_34_coef;   // 14 bit
    _16bits reg_dmc_plane_45_coef;   // 14 bit
    _16bits reg_dmc_plane_56_coef;   // 14 bit
    _16bits reg_dmc_plane_67_coef;   // 14 bit
    _16bits reg_dmc_plane_78_coef;   // 14 bit
    _16bits reg_dmc_plane_8w_coef;   // 14 bit
    _08bits reg_dmc_plane_8w_ks22;   //  1 bit
    _08bits reg_dmc_plane_78_ks22;   //  1 bit
    _08bits reg_dmc_plane_67_ks22;   //  1 bit
    _08bits reg_dmc_plane_56_ks22;   //  1 bit
    _08bits reg_dmc_plane_45_ks22;   //  1 bit
    _08bits reg_dmc_plane_34_ks22;   //  1 bit
    _08bits reg_dmc_plane_23_ks22;   //  1 bit
    _08bits reg_dmc_plane_12_ks22;   //  1 bit
    _08bits reg_dmc_plane_b1_ks22;   //  1 bit
    _08bits reg_dmc_h_block;         //  3 bit
    _08bits reg_dmc_v_block;         //  3 bit
    _08bits reg_dmc_rgb_mode;        //  1 bit
    _08bits reg_dmc_block_size;      //  2 bit
    _08bits reg_dmc_plane_num;       //  4 bit
    _16bits reg_dmc_h_lut_num;       // 11 bit
    _16bits reg_dmc_v_lut_num;       // 11 bit
    _08bits reg_dmc_flow_ctrl_en;    //  1 bit
    _16bits reg_dmc_flow_len;        // 10 bit
    _08bits reg_dmc_grid_line_en;    //  1 bit
    _08bits reg_dmc_grid_line_size;  //  3 bit
    _08bits reg_dmc_grid_line_color; //  8 bit
    _08bits reg_dmc_dither_rst_e_4_frame; //  1 bit
    _08bits reg_dmc_dither_pse_rst_num;   //  2 bit
    _08bits reg_dmc_dither_en;            //  2 bit
    _08bits reg_dmc_r_en;             //  1 bit
    _08bits reg_dmc_g_en;             //  1 bit
    _08bits reg_dmc_b_en;             //  1 bit
    _16bits reg_dmc_data_r_offset1;   // 14 bit
    _16bits reg_dmc_data_r_offset2;   // 14 bit
    _16bits reg_dmc_data_r_offset3;   // 14 bit
    _16bits reg_dmc_data_r_offset4;   // 14 bit
    _16bits reg_dmc_data_r_offset5;   // 14 bit
    _16bits reg_dmc_data_r_offset6;   // 14 bit
    _16bits reg_dmc_data_r_offset7;   // 14 bit
    _16bits reg_dmc_data_r_offset8;   // 14 bit
    _16bits reg_dmc_data_g_offset1;   // 14 bit
    _16bits reg_dmc_data_g_offset2;   // 14 bit
    _16bits reg_dmc_data_g_offset3;   // 14 bit
    _16bits reg_dmc_data_g_offset4;   // 14 bit
    _16bits reg_dmc_data_g_offset5;   // 14 bit
    _16bits reg_dmc_data_g_offset6;   // 14 bit
    _16bits reg_dmc_data_g_offset7;   // 14 bit
    _16bits reg_dmc_data_g_offset8;   // 14 bit
    _16bits reg_dmc_data_b_offset1;   // 14 bit
    _16bits reg_dmc_data_b_offset2;   // 14 bit
    _16bits reg_dmc_data_b_offset3;   // 14 bit
    _16bits reg_dmc_data_b_offset4;   // 14 bit
    _16bits reg_dmc_data_b_offset5;   // 14 bit
    _16bits reg_dmc_data_b_offset6;   // 14 bit
    _16bits reg_dmc_data_b_offset7;   // 14 bit
    _16bits reg_dmc_data_b_offset8;   // 14 bit
    _16bits reg_dmc_panel_h_size;     // 13 bit
    _16bits reg_dmc_preload_line;     // 12 bit
    _08bits reg_dmc_mono_mode_q_h_r;  //  6 bit
    _08bits reg_dmc_mono_mode_r_h_r;  //  3 bit
} registersTerr2HW;


typedef struct
{
    int iChannel;               // 0: R channel, 1 : G channel, 2 : B channel 
    
    MS_BOOL bLayer_level_en[6];    //  1 bit
    int  iLayer_level[6];       // 10 bits
    
    MS_BOOL bLow_luma_en;          //  1 bit
    int  iLow_luma_thrd;        //  8 bits
    int  iLow_luma_slope;       //  4 bits
    int  iLow_luma_min_weight;  //  4 bits

    MS_BOOL bHigh_luma_en;         //  1 bit
    int  iHigh_luma_thrd;       //  8 bits
    int  iHigh_luma_slope;      //  4 bits
    int  iHigh_luma_min_weight; //  4 bits

    MS_BOOL bOut_dither_en;
    
    double **Lut_in;
    int    **Lut_out;
    double **Lut_out_inv;

} interface_info;


typedef enum
{
    TYPE_MSTAR = 0, 
    TYPE_INX = 256,
} DEMURA_TYPE;

typedef enum
{
    CRC16 = sizeof(MS_U16), 
    CRC32 = sizeof(MS_U32),
    CRC64 = sizeof(MS_U64),
} CRC_TYPE;

typedef struct
{
  MS_U16   nBlackType;
  MS_U16   nBlackChecksum;
  MS_U32   nBlackStartAddr;
  MS_U32   nBlackDataLength;
} BlackHeader;  

typedef struct
{  
  MS_U64   nIdentification : 8;
  MS_U64   nASIC : 40;
  MS_U64   nVersion : 8;
  MS_U64   nAlgorithm : 8;
  MS_U32   nDate;      
  MS_U8    nBlackStructure;
  MS_U8    nDefination;
  MS_U16   nReserved;
  BlackHeader nBlack1;
  BlackHeader nBlack2;
  BlackHeader nBlack3;
  BlackHeader nBlack4;
  MS_U64   nReserved1;
  MS_U64   nReserved2;
  MS_U64   nReserved3;
  MS_U64   nReserved4;
  MS_U64   nReserved5;
  MS_U64   nReserved6;
  MS_U64   nReserved7;
  MS_U32   nReserved8;
  MS_U16   nReserved9;
  MS_U16   nCheckSum;
} DemuraHeader;

// BIN header structure (37 bytes)
typedef struct
{  
  MS_U32   nAllBinCheckSum;                                               // 4
  MS_U32   nHeaderCheckSum;                                               // 8
  MS_U32   nHeaderSize;                                                   // 12
  MS_U32   nAllBinSize;                                                   // 16
  MS_U64   nDemuraIdH;                                                    // 24
  MS_U64   nDemuraIdL;                                                    // 32
  MS_U16   nBinVersion;                                                   // 34
  MS_U8    nDataFormat;                                                   // 35
  MS_U8    bR_ch_Enable : 1;                                              // 36
  MS_U8    bG_ch_Enable : 1;
  MS_U8    bB_ch_Enable : 1;
  MS_U8    nReserved : 5;
  MS_U16   nHNodeCount;                                                   // 38
  MS_U16   nVNodeCount;                                                   // 40
  MS_U8    nLayerStart : 4; //Bits[3:0]: Start layer number               // 41
  MS_U8    nLayerEnd : 4;   //Bits[7:4]: End layer number
  MS_U8    bSeparate;       //0: Y channel, 1: Separate R/G/B channels    // 42
  MS_U32   nLDataStartAddr;                                               // 46
  MS_U32   nLDataOriginalSize;                                            // 50
  MS_U32   nLDataFlashSize;                                               // 54
  MS_U32   nRegisterStartAddr;                                            // 58
  MS_U32   nRegisterCount;                                                // 62
  MS_U16   nDllVersion;                                                   // 64
  MS_U32   nProjectID;                                                    // 68
  MS_U32   nCusDataStartAddr;                                             // 72
  MS_U32   nCusDataSize;                                                  // 76
  MS_U32   nDate;                                                         // 80
  MS_U8    nHBlockSize;                                                   // 81
  MS_U8    nVBlockSize;                                                   // 82
  MS_U8    nPacketSize;                                                   // 83
} __attribute__((packed)) MstarSubBinHeader;  //83 byte


typedef struct
{
  MS_U32   RegAddr : 24;
  MS_U32   RegValue : 8;
  MS_U8    RegMask;
}  __attribute__((packed)) RegData;


#endif