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
#include <MsTypes.h>
#include <stdio.h>
#include <common.h>
#include "CommonDataType.h"
#include <malloc.h>
#include <dmalloc.h>
#include <ms_vector.h>
#include <demura.h>
#include <demura_common.h>


MS_BOOL demura_core_ver00(const int base_address, const int lut_h_size, const int lut_v_size, 
                       const int block_h_size, const int block_v_size, const int y_mode_en, 
                       interface_info *pInfo, m_vector *pComRegVector, m_vector *pRegVector, const int packet_size)
{
    int reg_bank = base_address;  // 0x11200

    _08bits demura_y_mode_en    = { {reg_bank+0x47,0x02,0x00,"y_mode_en"} };
    _16bits demura_table_h_size = { {reg_bank+0x48,0xFF,0x00,"table_h_size"}, {reg_bank+0x49,0x03,0x00,"table_h_size"} };
    _16bits demura_table_v_size = { {reg_bank+0x4A,0xFF,0x00,"table_v_size"}, {reg_bank+0x4B,0x01,0x00,"table_v_size"} };

    _08bits demura_block_h_size = { {reg_bank+0x46,0x0F,0x03,"block_h_size"} };        // reg_block_h_size
    _08bits demura_block_v_size = { {reg_bank+0x46,0xF0,0x30,"block_v_size"} };        // reg_block_v_size

    _08bits demura_out_dither_en      = { {reg_bank+0xE1,0x08,0x00,"out_dither_en"} };
    //_08bits demura_internal_dither_en = { {reg_bank+0xE1,0x04,0x00,"internal_dither_en"} };

    registers r_channel = {
        { {reg_bank+0x51,0x80,0x00,"r_layer0_level_en"      }                                                                                     }, // reg_layer0_level_en
        { {reg_bank+0x50,0xFF,0x00,"r_layer0_level"         }, {reg_bank+0x51,0x03,0x00,"r_layer0_level"}                                         }, // reg_layer0_level
        { {reg_bank+0x53,0x80,0x00,"r_layer1_level_en"      }                                                                                     }, // reg_layer1_level_en
        { {reg_bank+0x52,0xFF,0x00,"r_layer1_level"         }, {reg_bank+0x53,0x03,0x00,"r_layer1_level"}                                         }, // reg_layer1_level
        { {reg_bank+0x55,0x80,0x00,"r_layer2_level_en"      }                                                                                     }, // reg_layer2_level_en
        { {reg_bank+0x54,0xFF,0x00,"r_layer2_level"         }, {reg_bank+0x55,0x03,0x00,"r_layer2_level"}                                         }, // reg_layer2_level
        { {reg_bank+0x57,0x80,0x00,"r_layer3_level_en"      }                                                                                     }, // reg_layer3_level_en
        { {reg_bank+0x56,0xFF,0x00,"r_layer3_level"         }, {reg_bank+0x57,0x03,0x00,"r_layer3_level"}                                         }, // reg_layer3_level
        { {reg_bank+0x59,0x80,0x00,"r_layer4_level_en"      }                                                                                     }, // reg_layer4_level_en
        { {reg_bank+0x58,0xFF,0xFF,"r_layer4_level"         }, {reg_bank+0x59,0x03,0x03,"r_layer4_level"}                                         }, // reg_layer4_level
        { {reg_bank+0x5B,0x80,0x00,"r_layer5_level_en"      }                                                                                     }, // reg_layer5_level_en
        { {reg_bank+0x5A,0xFF,0xFF,"r_layer5_level"         }, {reg_bank+0x5B,0x03,0x03,"r_layer5_level"}                                         }, // reg_layer5_level
        { {reg_bank+0x80,0xFF,0xFF,"r_int0_inv"             }, {reg_bank+0x81,0xFF,0xFF,"r_int0_inv"    }, {reg_bank+0x82,0x0F,0x0F,"r_int0_inv"} }, // reg_int0_inv
        { {reg_bank+0x84,0xFF,0xFF,"r_int1_inv"             }, {reg_bank+0x85,0xFF,0xFF,"r_int1_inv"    }, {reg_bank+0x86,0x0F,0x0F,"r_int1_inv"} }, // reg_int1_inv
        { {reg_bank+0x88,0xFF,0xFF,"r_int2_inv"             }, {reg_bank+0x89,0xFF,0xFF,"r_int2_inv"    }, {reg_bank+0x8A,0x0F,0x0F,"r_int2_inv"} }, // reg_int2_inv
        { {reg_bank+0x8C,0xFF,0xFF,"r_int3_inv"             }, {reg_bank+0x8D,0xFF,0xFF,"r_int3_inv"    }, {reg_bank+0x8E,0x0F,0x0F,"r_int3_inv"} }, // reg_int3_inv
        { {reg_bank+0x90,0xFF,0xFF,"r_int4_inv"             }, {reg_bank+0x91,0xFF,0xFF,"r_int4_inv"    }, {reg_bank+0x92,0x0F,0x0F,"r_int4_inv"} }, // reg_int4_inv
        { {reg_bank+0x94,0xFF,0xFF,"r_int5_inv"             }, {reg_bank+0x95,0xFF,0xFF,"r_int5_inv"    }, {reg_bank+0x96,0x0F,0x0F,"r_int5_inv"} }, // reg_int5_inv
        { {reg_bank+0x98,0xFF,0xFF,"r_int6_inv"             }, {reg_bank+0x99,0xFF,0xFF,"r_int6_inv"    }, {reg_bank+0x9A,0x0F,0x0F,"r_int6_inv"} }, // reg_int6_inv
        //{ {reg_bank+0xE0,0x04,0x04,"r_en"                   }                                                                                       }, // reg_en
        { {reg_bank+0xDC,0x07,0x00,"r_gain_y0"              }                                                                                     }, // reg_gain_y0
        { {reg_bank+0xF8,0x07,0x00,"r_gain_y1"              }                                                                                     }, // reg_gain_y1
        { {reg_bank+0xF8,0x70,0x00,"r_gain_y2"              }                                                                                     }, // reg_gain_y2
        { {reg_bank+0xF9,0x07,0x00,"r_gain_y3"              }                                                                                     }, // reg_gain_y3
        { {reg_bank+0xF9,0x70,0x00,"r_gain_y4"              }                                                                                     }, // reg_gain_y4
        { {reg_bank+0xDA,0x07,0x00,"r_gain_y5"              }                                                                                     }, // reg_gain_y5
        { {reg_bank+0xED,0x80,0x80,"r_low_luma_en"          }                                                                                     }, // reg_low_luma_en
        { {reg_bank+0xED,0x7F,0x00,"r_low_luma_thrd"        }                                                                                     }, // reg_low_luma_thrd
        { {reg_bank+0xEC,0xF0,0x40,"r_low_luma_slope"       }                                                                                     }, // reg_low_luma_slope
        { {reg_bank+0xEC,0x0F,0x00,"r_low_luma_min_weight"  }                                                                                     }, // reg_low_luma_min_weight
        { {reg_bank+0xEF,0x80,0x80,"r_high_luma_en"         }                                                                                     }, // reg_high_luma_en
        { {reg_bank+0xEF,0x7F,0x7F,"r_high_luma_thrd"       }                                                                                     }, // reg_high_luma_thrd
        { {reg_bank+0xEE,0xF0,0x40,"r_high_luma_slope"      }                                                                                     }, // reg_high_luma_slope
        { {reg_bank+0xEE,0x0F,0x00,"r_high_luma_min_weight" }                                                                                     }, // reg_high_luma_min_weight
    };


    registers g_channel = {
        { {reg_bank+0x61,0x80,0x00,"g_layer0_level_en"      }                                                                                     }, // reg_layer0_level_en
        { {reg_bank+0x60,0xFF,0x00,"g_layer0_level"         }, {reg_bank+0x61,0x03,0x00,"g_layer0_level"}                                         }, // reg_layer0_level
        { {reg_bank+0x63,0x80,0x00,"g_layer1_level_en"      }                                                                                     }, // reg_layer1_level_en
        { {reg_bank+0x62,0xFF,0x00,"g_layer1_level"         }, {reg_bank+0x63,0x03,0x00,"g_layer1_level"}                                         }, // reg_layer1_level
        { {reg_bank+0x65,0x80,0x00,"g_layer2_level_en"      }                                                                                     }, // reg_layer2_level_en
        { {reg_bank+0x64,0xFF,0x00,"g_layer2_level"         }, {reg_bank+0x65,0x03,0x00,"g_layer2_level"}                                         }, // reg_layer2_level
        { {reg_bank+0x67,0x80,0x00,"g_layer3_level_en"      }                                                                                     }, // reg_layer3_level_en
        { {reg_bank+0x66,0xFF,0x00,"g_layer3_level"         }, {reg_bank+0x67,0x03,0x00,"g_layer3_level"}                                         }, // reg_layer3_level
        { {reg_bank+0x69,0x80,0x00,"g_layer4_level_en"      }                                                                                     }, // reg_layer4_level_en
        { {reg_bank+0x68,0xFF,0xFF,"g_layer4_level"         }, {reg_bank+0x69,0x03,0x03,"g_layer4_level"}                                         }, // reg_layer4_level
        { {reg_bank+0x6B,0x80,0x00,"g_layer5_level_en"      }                                                                                     }, // reg_layer5_level_en
        { {reg_bank+0x6A,0xFF,0xFF,"g_layer5_level"         }, {reg_bank+0x6B,0x03,0x03,"g_layer5_level"}                                         }, // reg_layer5_level
        { {reg_bank+0xA0,0xFF,0xFF,"g_int0_inv"             }, {reg_bank+0xA1,0xFF,0xFF,"g_int0_inv"    }, {reg_bank+0xA2,0x0F,0x0F,"g_int0_inv"} }, // reg_int0_inv
        { {reg_bank+0xA4,0xFF,0xFF,"g_int1_inv"             }, {reg_bank+0xA5,0xFF,0xFF,"g_int1_inv"    }, {reg_bank+0xA6,0x0F,0x0F,"g_int1_inv"} }, // reg_int1_inv
        { {reg_bank+0xA8,0xFF,0xFF,"g_int2_inv"             }, {reg_bank+0xA9,0xFF,0xFF,"g_int2_inv"    }, {reg_bank+0xAA,0x0F,0x0F,"g_int2_inv"} }, // reg_int2_inv
        { {reg_bank+0xAC,0xFF,0xFF,"g_int3_inv"             }, {reg_bank+0xAD,0xFF,0xFF,"g_int3_inv"    }, {reg_bank+0xAE,0x0F,0x0F,"g_int3_inv"} }, // reg_int3_inv
        { {reg_bank+0xB0,0xFF,0xFF,"g_int4_inv"             }, {reg_bank+0xB1,0xFF,0xFF,"g_int4_inv"    }, {reg_bank+0xB2,0x0F,0x0F,"g_int4_inv"} }, // reg_int4_inv
        { {reg_bank+0xB4,0xFF,0xFF,"g_int5_inv"             }, {reg_bank+0xB5,0xFF,0xFF,"g_int5_inv"    }, {reg_bank+0xB6,0x0F,0x0F,"g_int5_inv"} }, // reg_int5_inv
        { {reg_bank+0xB8,0xFF,0xFF,"g_int6_inv"             }, {reg_bank+0xB9,0xFF,0xFF,"g_int6_inv"    }, {reg_bank+0xBA,0x0F,0x0F,"g_int6_inv"} }, // reg_int6_inv
        //{ {reg_bank+0xE0,0x02,0x02,"g_en"                   }                                                                                     }, // reg_en
        { {reg_bank+0xDC,0x70,0x00,"g_gain_y0"              }                                                                                     }, // reg_gain_y0
        { {reg_bank+0xFA,0x07,0x00,"g_gain_y1"              }                                                                                     }, // reg_gain_y1
        { {reg_bank+0xFA,0x70,0x00,"g_gain_y2"              }                                                                                     }, // reg_gain_y2
        { {reg_bank+0xFB,0x07,0x00,"g_gain_y3"              }                                                                                     }, // reg_gain_y3
        { {reg_bank+0xFB,0x70,0x00,"g_gain_y4"              }                                                                                     }, // reg_gain_y4
        { {reg_bank+0xDA,0x70,0x00,"g_gain_y5"              }                                                                                     }, // reg_gain_y5
        { {reg_bank+0xF1,0x80,0x80,"g_low_luma_en"          }                                                                                     }, // reg_low_luma_en
        { {reg_bank+0xF1,0x7F,0x00,"g_low_luma_thrd"        }                                                                                     }, // reg_low_luma_thrd
        { {reg_bank+0xF0,0xF0,0x40,"g_low_luma_slope"       }                                                                                     }, // reg_low_luma_slope
        { {reg_bank+0xF0,0x0F,0x00,"g_low_luma_min_weight"  }                                                                                     }, // reg_low_luma_min_weight
        { {reg_bank+0xF3,0x80,0x80,"g_high_luma_en"         }                                                                                     }, // reg_high_luma_en
        { {reg_bank+0xF3,0x7F,0x7F,"g_high_luma_thrd"       }                                                                                     }, // reg_high_luma_thrd
        { {reg_bank+0xF2,0xF0,0x40,"g_high_luma_slope"      }                                                                                     }, // reg_high_luma_slope
        { {reg_bank+0xF2,0x0F,0x00,"g_high_luma_min_weight" }                                                                                     }, // reg_high_luma_min_weight
    };


    registers b_channel = {
        { {reg_bank+0x71,0x80,0x00,"b_layer0_level_en"      }                                                                                     }, // reg_layer0_level_en
        { {reg_bank+0x70,0xFF,0x00,"b_layer0_level"         }, {reg_bank+0x71,0x03,0x00,"b_layer0_level"}                                         }, // reg_layer0_level
        { {reg_bank+0x73,0x80,0x00,"b_layer1_level_en"      }                                                                                     }, // reg_layer1_level_en
        { {reg_bank+0x72,0xFF,0x00,"b_layer1_level"         }, {reg_bank+0x73,0x03,0x00,"b_layer1_level"}                                         }, // reg_layer1_level
        { {reg_bank+0x75,0x80,0x00,"b_layer2_level_en"      }                                                                                     }, // reg_layer2_level_en
        { {reg_bank+0x74,0xFF,0x00,"b_layer2_level"         }, {reg_bank+0x75,0x03,0x00,"b_layer2_level"}                                         }, // reg_layer2_level
        { {reg_bank+0x77,0x80,0x00,"b_layer3_level_en"      }                                                                                     }, // reg_layer3_level_en
        { {reg_bank+0x76,0xFF,0x00,"b_layer3_level"         }, {reg_bank+0x77,0x03,0x00,"b_layer3_level"}                                         }, // reg_layer3_level
        { {reg_bank+0x79,0x80,0x00,"b_layer4_level_en"      }                                                                                     }, // reg_layer4_level_en
        { {reg_bank+0x78,0xFF,0xFF,"b_layer4_level"         }, {reg_bank+0x79,0x03,0x03,"b_layer4_level"}                                         }, // reg_layer4_level
        { {reg_bank+0x7B,0x80,0x00,"b_layer5_level_en"      }                                                                                     }, // reg_layer5_level_en
        { {reg_bank+0x7A,0xFF,0xFF,"b_layer5_level"         }, {reg_bank+0x7B,0x03,0x03,"b_layer5_level"}                                         }, // reg_layer5_level
        { {reg_bank+0x20,0xFF,0xFF,"b_int0_inv"             }, {reg_bank+0x21,0xFF,0xFF,"b_int0_inv"    }, {reg_bank+0x22,0x0F,0x0F,"b_int0_inv"} }, // reg_int0_inv
        { {reg_bank+0x24,0xFF,0xFF,"b_int1_inv"             }, {reg_bank+0x25,0xFF,0xFF,"b_int1_inv"    }, {reg_bank+0x26,0x0F,0x0F,"b_int1_inv"} }, // reg_int1_inv
        { {reg_bank+0x28,0xFF,0xFF,"b_int2_inv"             }, {reg_bank+0x29,0xFF,0xFF,"b_int2_inv"    }, {reg_bank+0x2A,0x0F,0x0F,"b_int2_inv"} }, // reg_int2_inv
        { {reg_bank+0x2C,0xFF,0xFF,"b_int3_inv"             }, {reg_bank+0x2D,0xFF,0xFF,"b_int3_inv"    }, {reg_bank+0x2E,0x0F,0x0F,"b_int3_inv"} }, // reg_int3_inv
        { {reg_bank+0x30,0xFF,0xFF,"b_int4_inv"             }, {reg_bank+0x31,0xFF,0xFF,"b_int4_inv"    }, {reg_bank+0x32,0x0F,0x0F,"b_int4_inv"} }, // reg_int4_inv
        { {reg_bank+0x34,0xFF,0xFF,"b_int5_inv"             }, {reg_bank+0x35,0xFF,0xFF,"b_int5_inv"    }, {reg_bank+0x36,0x0F,0x0F,"b_int5_inv"} }, // reg_int5_inv
        { {reg_bank+0x38,0xFF,0xFF,"b_int6_inv"             }, {reg_bank+0x39,0xFF,0xFF,"b_int6_inv"    }, {reg_bank+0x3A,0x0F,0x0F,"b_int6_inv"} }, // reg_int6_inv
        //{ {reg_bank+0xE0,0x01,0x01,"b_en"                   }                                                                                     }, // reg_en
        { {reg_bank+0xDD,0x07,0x00,"b_gain_y0"              }                                                                                     }, // reg_gain_y0
        { {reg_bank+0xFC,0x07,0x00,"b_gain_y1"              }                                                                                     }, // reg_gain_y1
        { {reg_bank+0xFC,0x70,0x00,"b_gain_y2"              }                                                                                     }, // reg_gain_y2
        { {reg_bank+0xFD,0x07,0x00,"b_gain_y3"              }                                                                                     }, // reg_gain_y3
        { {reg_bank+0xFD,0x70,0x00,"b_gain_y4"              }                                                                                     }, // reg_gain_y4
        { {reg_bank+0xDB,0x07,0x00,"b_gain_y5"              }                                                                                     }, // reg_gain_y5
        { {reg_bank+0xF5,0x80,0x80,"b_low_luma_en"          }                                                                                     }, // reg_low_luma_en
        { {reg_bank+0xF5,0x7F,0x00,"b_low_luma_thrd"        }                                                                                     }, // reg_low_luma_thrd
        { {reg_bank+0xF4,0xF0,0x40,"b_low_luma_slope"       }                                                                                     }, // reg_low_luma_slope
        { {reg_bank+0xF4,0x0F,0x00,"b_low_luma_min_weight"  }                                                                                     }, // reg_low_luma_min_weight
        { {reg_bank+0xF7,0x80,0x80,"b_high_luma_en"         }                                                                                     }, // reg_high_luma_en
        { {reg_bank+0xF7,0x7F,0x7F,"b_high_luma_thrd"       }                                                                                     }, // reg_high_luma_thrd
        { {reg_bank+0xF6,0xF0,0x40,"b_high_luma_slope"      }                                                                                     }, // reg_high_luma_slope
        { {reg_bank+0xF6,0x0F,0x00,"b_high_luma_min_weight" }                                                                                     }, // reg_high_luma_min_weight
    };

    registers Channel;

    switch((*pInfo).iChannel)
    {
    case 0:   // R channel
        Channel = r_channel;
        break;
    case 1:   // G channel
        Channel = g_channel;
        break;
    case 2:   // B channel
    default:
        Channel = b_channel;
        break;
    }


    //---------------- HW register start ----------------//
    int reg_ufc_layer_level_en[6];
    int reg_ufc_layer_level[6];
    int reg_ufc_gain_y[6];
    int reg_ufc_int_inv[7];

    int idx;
    for (idx = 0; idx < 6; idx++)
    {
        reg_ufc_layer_level_en[idx] = (((*pInfo).bLayer_level_en[idx]==TRUE) ? 1 : 0);
        reg_ufc_layer_level[idx]    = (*pInfo).iLayer_level[idx];                      // 10 bit
        reg_ufc_gain_y[idx]         = 0x0;                                         // default                                         
        reg_ufc_int_inv[idx]        = 0xFFFFF;                                     // default     
    }

    reg_ufc_int_inv[6]        = 0xFFFFF;                                     // default

    // int reg_ufc_low_luma_thrd  = minmax_((*pInfo).iLow_luma_thrd,0x0,0x7F);            // 8bit
    // int reg_ufc_high_luma_thrd = minmax_(((*pInfo).iHigh_luma_thrd - 0x80),0x0,0x7F);  // 8bit
    //---------------- HW register end ----------------//

    //---------------- layerX level pre-process start ----------------//
    if(reg_ufc_layer_level_en[0]==0)
        reg_ufc_layer_level[0] = 0;

    for (idx = 1; idx <= 4; idx++)
    {
        if(reg_ufc_layer_level_en[idx]==0)
        {

            switch((reg_ufc_layer_level_en[idx-1]<<1)+reg_ufc_layer_level_en[idx+1])
            {
            case 0:
            case 1:
            case 3:
                reg_ufc_layer_level[idx] = reg_ufc_layer_level[idx-1];
                break;
            case 2:
                reg_ufc_layer_level[idx] = 1023;
                break;
            }

        }
    }

    if(reg_ufc_layer_level_en[5]==0)
        reg_ufc_layer_level[5] = 1023;

    //---------------- layerX level pre-process end   ----------------//


    //---------------- LUT value process start ----------------//
    
    // alloc space
    (*pInfo).Lut_out = malloc(sizeof(int *) * 6);
    (*pInfo).Lut_out_inv = malloc(sizeof(double *) * 6);
    
    for (idx = 0; idx < 6; idx++)
    {
        if(idx==3)
            idx = idx;
        
        MS_U32 alloc_size = 0;
        MS_U32 TotalNode  = lut_h_size*lut_v_size;
        
        // pInfo ouput buffer
        alloc_size = TotalNode * sizeof(int);
        char * ptr = dmalloc(alloc_size);
        CHECK_DMALLOC_SPACE(ptr, (unsigned int)alloc_size);
        (*pInfo).Lut_out[idx] = (int *)ptr;
        
    #if (Get_Demura_Inv == 1)
        alloc_size = TotalNode * sizeof(double);
        ptr = dmalloc(alloc_size);
        CHECK_DMALLOC_SPACE(ptr, (unsigned int)alloc_size);
        (*pInfo).Lut_out_inv[idx] = (double *)ptr;
    #endif
        
        // Temp buffer
        alloc_size = TotalNode * sizeof(double);
        ptr = dmalloc(alloc_size);
        CHECK_DMALLOC_SPACE(ptr, (unsigned int)alloc_size);
        double *lut_in = (double *)ptr;
        
        int count_idx;
        for (count_idx = 0; count_idx < lut_h_size*lut_v_size; count_idx++)
        {

            if(reg_ufc_layer_level_en[idx]==0)
                lut_in[count_idx] = 0;
            else
                lut_in[count_idx] = (*pInfo).Lut_in[idx][count_idx];
        }

        demura_table_normalization( lut_h_size*lut_v_size, lut_in, (*pInfo).Lut_out[idx], &(reg_ufc_gain_y[idx]));
    #if (Get_Demura_Inv == 1)
        demura_table_normalization_inv( lut_h_size*lut_v_size, reg_ufc_gain_y[idx], (*pInfo).Lut_out[idx], (*pInfo).Lut_out_inv[idx] );
    #endif
        
        // free temp buffer
        dfree(lut_in);
        
        // free pinfo->Lut_in buffer;
        dfree( (*pInfo).Lut_in[idx] );
        (*pInfo).Lut_in[idx] = NULL;
    }
    
    //---------------- LUT value process end ----------------//

    //---------------- cal layer distance start ----------------//     
    reg_ufc_int_inv[0] = demura_layer_distance_div(                      0, reg_ufc_layer_level[0]);
    reg_ufc_int_inv[1] = demura_layer_distance_div( reg_ufc_layer_level[0], reg_ufc_layer_level[1]);
    reg_ufc_int_inv[2] = demura_layer_distance_div( reg_ufc_layer_level[1], reg_ufc_layer_level[2]);
    reg_ufc_int_inv[3] = demura_layer_distance_div( reg_ufc_layer_level[2], reg_ufc_layer_level[3]);
    reg_ufc_int_inv[4] = demura_layer_distance_div( reg_ufc_layer_level[3], reg_ufc_layer_level[4]);
    reg_ufc_int_inv[5] = demura_layer_distance_div( reg_ufc_layer_level[4], reg_ufc_layer_level[5]);
    reg_ufc_int_inv[6] = demura_layer_distance_div( reg_ufc_layer_level[5],                   1023); 
    //---------------- cal layer distance end ----------------//

    //---------------- write register start ----------------//

    //++++++++++++++++++++++++ demura y mode enable start +++++++++++++++++++++++++//
    if(y_mode_en!=-1)
       demura_y_mode_en.Bit07_00.value                         = register_shift(demura_y_mode_en.Bit07_00.mask,y_mode_en);
    //++++++++++++++++++++++++ demura y mode enable end   +++++++++++++++++++++++++//

    //++++++++++++++++++++++++ demura table h/v size start ++++++++++++++++++++++++//

    int demura_table_h_size_tmp07_00;
    int demura_table_h_size_tmp15_08;

    int new_lut_h_size = 0;

    if (y_mode_en == 1)
    {
        new_lut_h_size = (lut_h_size/(packet_size * 3)) + (((lut_h_size%(packet_size *3)) != 0)?1:0);
    }
    else
    {
        new_lut_h_size = (lut_h_size/packet_size) + (((lut_h_size%packet_size) != 0)?1:0);;
    }
    
    part_bit15_00( new_lut_h_size, 10, &demura_table_h_size_tmp15_08, &demura_table_h_size_tmp07_00);

    demura_table_h_size.Bit07_00.value                       = register_shift(demura_table_h_size.Bit07_00.mask, demura_table_h_size_tmp07_00);
    demura_table_h_size.Bit15_08.value                       = register_shift(demura_table_h_size.Bit15_08.mask, demura_table_h_size_tmp15_08);


    int demura_table_v_size_tmp07_00;
    int demura_table_v_size_tmp15_08;

    part_bit15_00( lut_v_size, 10, &demura_table_v_size_tmp15_08, &demura_table_v_size_tmp07_00);

    demura_table_v_size.Bit07_00.value                       = register_shift(demura_table_v_size.Bit07_00.mask, demura_table_v_size_tmp07_00);
    demura_table_v_size.Bit15_08.value                       = register_shift(demura_table_v_size.Bit15_08.mask, demura_table_v_size_tmp15_08);
    //++++++++++++++++++++++++ demura table h/v size end ++++++++++++++++++++++++//

    //++++++++++++++++++++++++ demura block h/v size start ++++++++++++++++++++++++//
    int demura_block_size_tmp07_00;

    switch(block_h_size)
    {
    case 8:
        demura_block_size_tmp07_00 = 0x3;
        break;
    case 16:
        demura_block_size_tmp07_00 = 0x4;
        break;
    case 32:
        demura_block_size_tmp07_00 = 0x5;
        break;
    case 64:
        demura_block_size_tmp07_00 = 0x6;
        break;
    case 128:
        demura_block_size_tmp07_00 = 0x7;
        break;
    case 256:
        demura_block_size_tmp07_00 = 0x8;
        break;
    default:
        demura_block_size_tmp07_00 = 0x3;
        break;
    }

    demura_block_h_size.Bit07_00.value     = register_shift(demura_block_h_size.Bit07_00.mask,demura_block_size_tmp07_00);


    switch(block_v_size)
    {
    case 8:
        demura_block_size_tmp07_00 = 0x3;
        break;
    case 16:
        demura_block_size_tmp07_00 = 0x4;
        break;
    case 32:
        demura_block_size_tmp07_00 = 0x5;
        break;
    case 64:
        demura_block_size_tmp07_00 = 0x6;
        break;
    case 128:
        demura_block_size_tmp07_00 = 0x7;
        break;
    case 256:
        demura_block_size_tmp07_00 = 0x8;
        break;
    default:
        demura_block_size_tmp07_00 = 0x3;
        break;
    }

    demura_block_v_size.Bit07_00.value      = register_shift(demura_block_v_size.Bit07_00.mask,demura_block_size_tmp07_00);

    //++++++++++++++++++++++++ demura block h/v size end   ++++++++++++++++++++++++//

    //++++++++++++++++++++++ layerX level enable start ++++++++++++++++++++++//
    Channel.reg_layer0_level_en.Bit07_00.value                   = register_shift(Channel.reg_layer0_level_en.Bit07_00.mask,reg_ufc_layer_level_en[0]);
    Channel.reg_layer1_level_en.Bit07_00.value                   = register_shift(Channel.reg_layer1_level_en.Bit07_00.mask,reg_ufc_layer_level_en[1]);
    Channel.reg_layer2_level_en.Bit07_00.value                   = register_shift(Channel.reg_layer2_level_en.Bit07_00.mask,reg_ufc_layer_level_en[2]);
    Channel.reg_layer3_level_en.Bit07_00.value                   = register_shift(Channel.reg_layer3_level_en.Bit07_00.mask,reg_ufc_layer_level_en[3]);
    Channel.reg_layer4_level_en.Bit07_00.value                   = register_shift(Channel.reg_layer4_level_en.Bit07_00.mask,reg_ufc_layer_level_en[4]);
    Channel.reg_layer5_level_en.Bit07_00.value                   = register_shift(Channel.reg_layer5_level_en.Bit07_00.mask,reg_ufc_layer_level_en[5]);
    //++++++++++++++++++++++ layerX level enable end ++++++++++++++++++++++//

    //++++++++++++++++++++++ layerX level start ++++++++++++++++++++++//
    int layer_level_value_tmp07_00;
    int layer_level_value_tmp15_08;

    part_bit15_00(reg_ufc_layer_level[0], 10, &layer_level_value_tmp15_08, &layer_level_value_tmp07_00);

    Channel.reg_layer0_level.Bit07_00.value                      = register_shift(Channel.reg_layer0_level.Bit07_00.mask,layer_level_value_tmp07_00);
    Channel.reg_layer0_level.Bit15_08.value                      = register_shift(Channel.reg_layer0_level.Bit15_08.mask,layer_level_value_tmp15_08);


    part_bit15_00(reg_ufc_layer_level[1], 10, &layer_level_value_tmp15_08, &layer_level_value_tmp07_00);

    Channel.reg_layer1_level.Bit07_00.value                      = register_shift(Channel.reg_layer1_level.Bit07_00.mask,layer_level_value_tmp07_00);
    Channel.reg_layer1_level.Bit15_08.value                      = register_shift(Channel.reg_layer1_level.Bit15_08.mask,layer_level_value_tmp15_08);


    part_bit15_00(reg_ufc_layer_level[2], 10, &layer_level_value_tmp15_08, &layer_level_value_tmp07_00);

    Channel.reg_layer2_level.Bit07_00.value                      = register_shift(Channel.reg_layer2_level.Bit07_00.mask,layer_level_value_tmp07_00);
    Channel.reg_layer2_level.Bit15_08.value                      = register_shift(Channel.reg_layer2_level.Bit15_08.mask,layer_level_value_tmp15_08);


    part_bit15_00(reg_ufc_layer_level[3], 10, &layer_level_value_tmp15_08, &layer_level_value_tmp07_00);

    Channel.reg_layer3_level.Bit07_00.value                      = register_shift(Channel.reg_layer3_level.Bit07_00.mask,layer_level_value_tmp07_00);
    Channel.reg_layer3_level.Bit15_08.value                      = register_shift(Channel.reg_layer3_level.Bit15_08.mask,layer_level_value_tmp15_08);


    part_bit15_00(reg_ufc_layer_level[4], 10, &layer_level_value_tmp15_08, &layer_level_value_tmp07_00);

    Channel.reg_layer4_level.Bit07_00.value                      = register_shift(Channel.reg_layer4_level.Bit07_00.mask,layer_level_value_tmp07_00);
    Channel.reg_layer4_level.Bit15_08.value                      = register_shift(Channel.reg_layer4_level.Bit15_08.mask,layer_level_value_tmp15_08);


    part_bit15_00(reg_ufc_layer_level[5], 10, &layer_level_value_tmp15_08, &layer_level_value_tmp07_00);

    Channel.reg_layer5_level.Bit07_00.value                      = register_shift(Channel.reg_layer5_level.Bit07_00.mask,layer_level_value_tmp07_00);
    Channel.reg_layer5_level.Bit15_08.value                      = register_shift(Channel.reg_layer5_level.Bit15_08.mask,layer_level_value_tmp15_08);
    //++++++++++++++++++++++ layerX level end ++++++++++++++++++++++//



    //++++++++++++++++++++++ gain layerX start ++++++++++++++++++++++//
    Channel.reg_gain_y0.Bit07_00.value                           = register_shift(Channel.reg_gain_y0.Bit07_00.mask,reg_ufc_gain_y[0]);

    Channel.reg_gain_y1.Bit07_00.value                           = register_shift(Channel.reg_gain_y1.Bit07_00.mask,reg_ufc_gain_y[1]);

    Channel.reg_gain_y2.Bit07_00.value                           = register_shift(Channel.reg_gain_y2.Bit07_00.mask,reg_ufc_gain_y[2]);

    Channel.reg_gain_y3.Bit07_00.value                           = register_shift(Channel.reg_gain_y3.Bit07_00.mask,reg_ufc_gain_y[3]);

    Channel.reg_gain_y4.Bit07_00.value                           = register_shift(Channel.reg_gain_y4.Bit07_00.mask,reg_ufc_gain_y[4]);

    Channel.reg_gain_y5.Bit07_00.value                           = register_shift(Channel.reg_gain_y5.Bit07_00.mask,reg_ufc_gain_y[5]);
    //++++++++++++++++++++++ gain layerX end ++++++++++++++++++++++//



    //++++++++++++++++++++++ intX inv start ++++++++++++++++++++++//
    int inv_value_tmp07_00;
    int inv_value_tmp15_08;
    int inv_value_tmp23_16;

    part_bit23_00( reg_ufc_int_inv[0], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int0_inv.Bit07_00.value                          = register_shift(Channel.reg_int0_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int0_inv.Bit15_08.value                          = register_shift(Channel.reg_int0_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int0_inv.Bit23_16.value                          = register_shift(Channel.reg_int0_inv.Bit23_16.mask,inv_value_tmp23_16);


    part_bit23_00( reg_ufc_int_inv[1], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int1_inv.Bit07_00.value                          = register_shift(Channel.reg_int1_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int1_inv.Bit15_08.value                          = register_shift(Channel.reg_int1_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int1_inv.Bit23_16.value                          = register_shift(Channel.reg_int1_inv.Bit23_16.mask,inv_value_tmp23_16);


    part_bit23_00( reg_ufc_int_inv[2], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int2_inv.Bit07_00.value                          = register_shift(Channel.reg_int2_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int2_inv.Bit15_08.value                          = register_shift(Channel.reg_int2_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int2_inv.Bit23_16.value                          = register_shift(Channel.reg_int2_inv.Bit23_16.mask,inv_value_tmp23_16);


    part_bit23_00( reg_ufc_int_inv[3], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int3_inv.Bit07_00.value                          = register_shift(Channel.reg_int3_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int3_inv.Bit15_08.value                          = register_shift(Channel.reg_int3_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int3_inv.Bit23_16.value                          = register_shift(Channel.reg_int3_inv.Bit23_16.mask,inv_value_tmp23_16);


    part_bit23_00( reg_ufc_int_inv[4], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int4_inv.Bit07_00.value                          = register_shift(Channel.reg_int4_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int4_inv.Bit15_08.value                          = register_shift(Channel.reg_int4_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int4_inv.Bit23_16.value                          = register_shift(Channel.reg_int4_inv.Bit23_16.mask,inv_value_tmp23_16);


    part_bit23_00( reg_ufc_int_inv[5], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int5_inv.Bit07_00.value                          = register_shift(Channel.reg_int5_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int5_inv.Bit15_08.value                          = register_shift(Channel.reg_int5_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int5_inv.Bit23_16.value                          = register_shift(Channel.reg_int5_inv.Bit23_16.mask,inv_value_tmp23_16);


    part_bit23_00( reg_ufc_int_inv[6], 20, &inv_value_tmp23_16, &inv_value_tmp15_08, &inv_value_tmp07_00);

    Channel.reg_int6_inv.Bit07_00.value                          = register_shift(Channel.reg_int6_inv.Bit07_00.mask,inv_value_tmp07_00);
    Channel.reg_int6_inv.Bit15_08.value                          = register_shift(Channel.reg_int6_inv.Bit15_08.mask,inv_value_tmp15_08);
    Channel.reg_int6_inv.Bit23_16.value                          = register_shift(Channel.reg_int6_inv.Bit23_16.mask,inv_value_tmp23_16);
    //++++++++++++++++++++++ intX inv end ++++++++++++++++++++++//


    //++++++++++++++++++++++ low luma weight start ++++++++++++++++++++++//
    Channel.reg_low_luma_en.Bit07_00.value                       = register_shift(Channel.reg_low_luma_en.Bit07_00.mask, (((*pInfo).bLow_luma_en==TRUE) ? 1 : 0));
    Channel.reg_low_luma_thrd.Bit07_00.value                     = register_shift(Channel.reg_low_luma_thrd.Bit07_00.mask, (*pInfo).iLow_luma_thrd);
    Channel.reg_low_luma_slope.Bit07_00.value                    = register_shift(Channel.reg_low_luma_slope.Bit07_00.mask, (*pInfo).iLow_luma_slope);
    Channel.reg_low_luma_min_weight.Bit07_00.value               = register_shift(Channel.reg_low_luma_min_weight.Bit07_00.mask, (*pInfo).iLow_luma_min_weight);
    //++++++++++++++++++++++ low luma weight end   ++++++++++++++++++++++//


    //++++++++++++++++++++++ high luma weight start ++++++++++++++++++++++//
    Channel.reg_high_luma_en.Bit07_00.value                       = register_shift(Channel.reg_high_luma_en.Bit07_00.mask, (((*pInfo).bHigh_luma_en==TRUE) ? 1 : 0));
    Channel.reg_high_luma_thrd.Bit07_00.value                     = register_shift(Channel.reg_high_luma_thrd.Bit07_00.mask, (*pInfo).iHigh_luma_thrd);
    Channel.reg_high_luma_slope.Bit07_00.value                    = register_shift(Channel.reg_high_luma_slope.Bit07_00.mask, (*pInfo).iHigh_luma_slope);
    Channel.reg_high_luma_min_weight.Bit07_00.value               = register_shift(Channel.reg_high_luma_min_weight.Bit07_00.mask, (*pInfo).iHigh_luma_min_weight);
    //++++++++++++++++++++++ high luma weight end   ++++++++++++++++++++++//

    //++++++++++++++++++++++ dither start ++++++++++++++++++++++//
    demura_out_dither_en.Bit07_00.value                           = register_shift(demura_out_dither_en.Bit07_00.mask, (((*pInfo).bOut_dither_en==TRUE) ? 1 : 0));
    //demura_internal_dither_en.Bit07_00.value                      = register_shift(demura_internal_dither_en.Bit07_00.mask, (((*pInfo).bInternal_dither_en==TRUE) ? 1 : 0));
    //++++++++++++++++++++++ dither end   ++++++++++++++++++++++//
    //---------------- write register end ----------------//


    //---------------- dump channel register to vector start ----------------//
    int reg_idx = 0;
    reg_struct *regList = (reg_struct *)pRegVector->pbuf + pRegVector->dat_num;
    
    regList[reg_idx++] = (Channel.reg_layer0_level_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer0_level.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer0_level.Bit15_08);
    regList[reg_idx++] = (Channel.reg_layer1_level_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer1_level.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer1_level.Bit15_08);
    regList[reg_idx++] = (Channel.reg_layer2_level_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer2_level.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer2_level.Bit15_08);
    regList[reg_idx++] = (Channel.reg_layer3_level_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer3_level.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer3_level.Bit15_08);
    regList[reg_idx++] = (Channel.reg_layer4_level_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer4_level.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer4_level.Bit15_08);
    regList[reg_idx++] = (Channel.reg_layer5_level_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer5_level.Bit07_00);
    regList[reg_idx++] = (Channel.reg_layer5_level.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int0_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int0_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int0_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_int1_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int1_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int1_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_int2_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int2_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int2_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_int3_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int3_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int3_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_int4_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int4_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int4_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_int5_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int5_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int5_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_int6_inv.Bit07_00);
    regList[reg_idx++] = (Channel.reg_int6_inv.Bit15_08);
    regList[reg_idx++] = (Channel.reg_int6_inv.Bit23_16);
    regList[reg_idx++] = (Channel.reg_gain_y0.Bit07_00);
    regList[reg_idx++] = (Channel.reg_gain_y1.Bit07_00);
    regList[reg_idx++] = (Channel.reg_gain_y2.Bit07_00);
    regList[reg_idx++] = (Channel.reg_gain_y3.Bit07_00);
    regList[reg_idx++] = (Channel.reg_gain_y4.Bit07_00);
    regList[reg_idx++] = (Channel.reg_gain_y5.Bit07_00);
    regList[reg_idx++] = (Channel.reg_low_luma_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_low_luma_thrd.Bit07_00);
    regList[reg_idx++] = (Channel.reg_low_luma_slope.Bit07_00);
    regList[reg_idx++] = (Channel.reg_low_luma_min_weight.Bit07_00);
    regList[reg_idx++] = (Channel.reg_high_luma_en.Bit07_00);
    regList[reg_idx++] = (Channel.reg_high_luma_thrd.Bit07_00);
    regList[reg_idx++] = (Channel.reg_high_luma_slope.Bit07_00);
    regList[reg_idx++] = (Channel.reg_high_luma_min_weight.Bit07_00);
    
    pRegVector->dat_num += reg_idx;
    //---------------- dump channel register to vector end   ----------------//

    //---------------- dump Common register to vector start ----------------//
    int com_reg_idx = 0;
    reg_struct *Common_regList = (reg_struct *)pComRegVector->pbuf + pComRegVector->dat_num;

    Common_regList[com_reg_idx++] = (demura_table_h_size.Bit07_00);
    Common_regList[com_reg_idx++] = (demura_table_h_size.Bit15_08);
    Common_regList[com_reg_idx++] = (demura_table_v_size.Bit07_00);
    Common_regList[com_reg_idx++] = (demura_table_v_size.Bit15_08);

    Common_regList[com_reg_idx++] = (demura_block_h_size.Bit07_00);
    Common_regList[com_reg_idx++] = (demura_block_v_size.Bit07_00);

    if(y_mode_en != -1)
        Common_regList[com_reg_idx++] = (demura_y_mode_en.Bit07_00);

    Common_regList[com_reg_idx++] = (demura_out_dither_en.Bit07_00);
    pComRegVector->dat_num += com_reg_idx;
    //---------------- dump Common register to vector end   ----------------//
    return TRUE;
}


void write_demuraLUT_to_HW_DRAM(stlayer_info_structInfo layer_info, strgb_structInfo *layer_blk_img[8], m_vector *pLutOutVector)
{
    int out_idx = 0; // For Lut_out arrary
    unsigned char *Lut_out = (unsigned char *)(pLutOutVector->pbuf) + pLutOutVector->dat_num;

    if(layer_info.dmc_rgb_mode==1)
    {
        int H_offset = 1;
        switch(layer_info.dmc_plane_num)
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
            default:
                break;
        }

        int lut_vidx, lut_hidx;
        for (lut_vidx = 0; lut_vidx < layer_info.real_dram_v_size; lut_vidx++)
        {
            for (lut_hidx = 0; lut_hidx < layer_info.real_dram_h_size; lut_hidx++)
            {
                strgb_structInfo L[9];
                strgb_structInfo null_val;
                null_val.r = 0;
                null_val.g = 0;
                null_val.b = 0;
                null_val.dbr = 0.0;
                null_val.dbg = 0.0;
                null_val.dbb = 0.0;

                int lut_idx0 = min_(lut_hidx*H_offset + 0, layer_info.dram_h_size);
                int lut_idx1 = min_(lut_hidx*H_offset + 1, layer_info.dram_h_size);
                int lut_idx2 = min_(lut_hidx*H_offset + 2, layer_info.dram_h_size);
                int lut_idx3 = min_(lut_hidx*H_offset + 3, layer_info.dram_h_size);

                lut_idx0 = (lut_idx0==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx0);
                lut_idx1 = (lut_idx1==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx1);
                lut_idx2 = (lut_idx2==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx2);
                lut_idx3 = (lut_idx3==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx3);


                switch(layer_info.dmc_plane_num)
                {
                case 1:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = null_val;
                    L[3] = (lut_idx1==-1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[4] = null_val;
                    L[5] = (lut_idx2==-1) ? null_val : layer_blk_img[0][lut_idx2];
                    L[6] = null_val;
                    L[7] = (lut_idx3==-1) ? null_val : layer_blk_img[0][lut_idx3];
                    L[8] = null_val;
                    break;
                case 2:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx1==-1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[4] = (lut_idx1==-1) ? null_val : layer_blk_img[1][lut_idx1];
                    L[5] = (lut_idx2==-1) ? null_val : layer_blk_img[0][lut_idx2];
                    L[6] = (lut_idx2==-1) ? null_val : layer_blk_img[1][lut_idx2];
                    L[7] = (lut_idx3==-1) ? null_val : layer_blk_img[0][lut_idx3];
                    L[8] = (lut_idx3==-1) ? null_val : layer_blk_img[1][lut_idx3];
                    break;
                case 3:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0==-1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = null_val;
                    L[5] = (lut_idx1==-1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[6] = (lut_idx1==-1) ? null_val : layer_blk_img[1][lut_idx1];
                    L[7] = (lut_idx1==-1) ? null_val : layer_blk_img[2][lut_idx1];
                    L[8] = null_val;
                    break;
                case 4:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0==-1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0==-1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx1==-1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[6] = (lut_idx1==-1) ? null_val : layer_blk_img[1][lut_idx1];
                    L[7] = (lut_idx1==-1) ? null_val : layer_blk_img[2][lut_idx1];
                    L[8] = (lut_idx1==-1) ? null_val : layer_blk_img[3][lut_idx1];
                    break;
                case 5:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0==-1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0==-1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0==-1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = null_val;
                    L[7] = null_val;
                    L[8] = null_val;
                    break;
                case 6:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0==-1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0==-1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0==-1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = (lut_idx0==-1) ? null_val : layer_blk_img[5][lut_idx0];
                    L[7] = null_val;
                    L[8] = null_val;
                    break;
                case 7:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0==-1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0==-1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0==-1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = (lut_idx0==-1) ? null_val : layer_blk_img[5][lut_idx0];
                    L[7] = (lut_idx0==-1) ? null_val : layer_blk_img[6][lut_idx0];
                    L[8] = null_val;
                    break;
                case 8:
                    L[1] = (lut_idx0==-1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0==-1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0==-1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0==-1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0==-1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = (lut_idx0==-1) ? null_val : layer_blk_img[5][lut_idx0];
                    L[7] = (lut_idx0==-1) ? null_val : layer_blk_img[6][lut_idx0];
                    L[8] = (lut_idx0==-1) ? null_val : layer_blk_img[7][lut_idx0];
                    break;
                }


                int idx;
                for (idx = 1; idx < 9; idx++)
                {
                    char rr = (char)((L[idx].r < 0.0) ? (256 + L[idx].r) : (L[idx].r));
                    char gg = (char)((L[idx].g < 0.0) ? (256 + L[idx].g) : (L[idx].g));
                    char bb = (char)((L[idx].b < 0.0) ? (256 + L[idx].b) : (L[idx].b));
                    char zero = 0;
                    
                    Lut_out[out_idx++] = bb;
                    Lut_out[out_idx++] = gg;
                    Lut_out[out_idx++] = rr;

                    if((idx-1)%4==3)
                    {
                        int zero_idx;
                        for (zero_idx = 0; zero_idx < 4; zero_idx++)
                            Lut_out[out_idx++] = zero;
                    }

                }

            }
        }

    }
    else
    {
        int H_offset = 4;
        switch(layer_info.dmc_plane_num)
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
            default:
                break;
        }  


        int lut_vidx, lut_hidx;
        for (lut_vidx = 0; lut_vidx < layer_info.real_dram_v_size; lut_vidx++)
        {
            for (lut_hidx = 0; lut_hidx < layer_info.real_dram_h_size; lut_hidx++)
            {
                int L[32];
                int null_val = 0;

                int lut_idx00 = min_(lut_hidx*H_offset +  0, layer_info.dram_h_size );
                int lut_idx01 = min_(lut_hidx*H_offset +  1, layer_info.dram_h_size );
                int lut_idx02 = min_(lut_hidx*H_offset +  2, layer_info.dram_h_size );
                int lut_idx03 = min_(lut_hidx*H_offset +  3, layer_info.dram_h_size );
                int lut_idx04 = min_(lut_hidx*H_offset +  4, layer_info.dram_h_size );
                int lut_idx05 = min_(lut_hidx*H_offset +  5, layer_info.dram_h_size );
                int lut_idx06 = min_(lut_hidx*H_offset +  6, layer_info.dram_h_size );
                int lut_idx07 = min_(lut_hidx*H_offset +  7, layer_info.dram_h_size );
                int lut_idx08 = min_(lut_hidx*H_offset +  8, layer_info.dram_h_size );
                int lut_idx09 = min_(lut_hidx*H_offset +  9, layer_info.dram_h_size );
                int lut_idx10 = min_(lut_hidx*H_offset + 10, layer_info.dram_h_size );
                int lut_idx11 = min_(lut_hidx*H_offset + 11, layer_info.dram_h_size );
                int lut_idx12 = min_(lut_hidx*H_offset + 12, layer_info.dram_h_size );
                int lut_idx13 = min_(lut_hidx*H_offset + 13, layer_info.dram_h_size );
                int lut_idx14 = min_(lut_hidx*H_offset + 14, layer_info.dram_h_size );
                int lut_idx15 = min_(lut_hidx*H_offset + 15, layer_info.dram_h_size );

                lut_idx00 = (lut_idx00==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx00);
                lut_idx01 = (lut_idx01==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx01);
                lut_idx02 = (lut_idx02==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx02);
                lut_idx03 = (lut_idx03==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx03);
                lut_idx04 = (lut_idx04==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx04);
                lut_idx05 = (lut_idx05==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx05);
                lut_idx06 = (lut_idx06==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx06);
                lut_idx07 = (lut_idx07==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx07);
                lut_idx08 = (lut_idx08==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx08);
                lut_idx09 = (lut_idx09==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx09);
                lut_idx10 = (lut_idx10==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx10);
                lut_idx11 = (lut_idx11==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx11);
                lut_idx12 = (lut_idx12==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx12);
                lut_idx13 = (lut_idx13==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx13);
                lut_idx14 = (lut_idx14==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx14);
                lut_idx15 = (lut_idx15==layer_info.dram_h_size) ? -1 : (lut_vidx*layer_info.dram_h_size + lut_idx15);


                switch(layer_info.dmc_plane_num)
                {
                case 1:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = null_val;
                    L[ 2] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 3] = null_val;
                    L[ 4] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 5] = null_val;
                    L[ 6] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx04==-1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[ 9] = null_val;
                    L[10] = (lut_idx05==-1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[11] = null_val;
                    L[12] = (lut_idx06==-1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[13] = null_val;
                    L[14] = (lut_idx07==-1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[15] = null_val;
                    L[16] = (lut_idx08==-1) ? null_val : layer_blk_img[0][lut_idx08].g;
                    L[17] = null_val;
                    L[18] = (lut_idx09==-1) ? null_val : layer_blk_img[0][lut_idx09].g;
                    L[19] = null_val;
                    L[20] = (lut_idx10==-1) ? null_val : layer_blk_img[0][lut_idx10].g;
                    L[21] = null_val;
                    L[22] = (lut_idx11==-1) ? null_val : layer_blk_img[0][lut_idx11].g;
                    L[23] = null_val;
                    L[24] = (lut_idx12==-1) ? null_val : layer_blk_img[0][lut_idx12].g;
                    L[25] = null_val;
                    L[26] = (lut_idx13==-1) ? null_val : layer_blk_img[0][lut_idx13].g;
                    L[27] = null_val;
                    L[28] = (lut_idx14==-1) ? null_val : layer_blk_img[0][lut_idx14].g;
                    L[29] = null_val;
                    L[30] = (lut_idx15==-1) ? null_val : layer_blk_img[0][lut_idx15].g;
                    L[31] = null_val;
                    break;
                case 2:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 3] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[ 4] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 5] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[ 6] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[ 7] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[ 8] = (lut_idx04==-1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[ 9] = (lut_idx04==-1) ? null_val : layer_blk_img[1][lut_idx04].g;
                    L[10] = (lut_idx05==-1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[11] = (lut_idx05==-1) ? null_val : layer_blk_img[1][lut_idx05].g;
                    L[12] = (lut_idx06==-1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[13] = (lut_idx06==-1) ? null_val : layer_blk_img[1][lut_idx06].g;
                    L[14] = (lut_idx07==-1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[15] = (lut_idx07==-1) ? null_val : layer_blk_img[1][lut_idx07].g;
                    L[16] = (lut_idx08==-1) ? null_val : layer_blk_img[0][lut_idx08].g;
                    L[17] = (lut_idx08==-1) ? null_val : layer_blk_img[1][lut_idx08].g;
                    L[18] = (lut_idx09==-1) ? null_val : layer_blk_img[0][lut_idx09].g;
                    L[19] = (lut_idx09==-1) ? null_val : layer_blk_img[1][lut_idx09].g;
                    L[20] = (lut_idx10==-1) ? null_val : layer_blk_img[0][lut_idx10].g;
                    L[21] = (lut_idx10==-1) ? null_val : layer_blk_img[1][lut_idx10].g;
                    L[22] = (lut_idx11==-1) ? null_val : layer_blk_img[0][lut_idx11].g;
                    L[23] = (lut_idx11==-1) ? null_val : layer_blk_img[1][lut_idx11].g;
                    L[24] = (lut_idx12==-1) ? null_val : layer_blk_img[0][lut_idx12].g;
                    L[25] = (lut_idx12==-1) ? null_val : layer_blk_img[1][lut_idx12].g;
                    L[26] = (lut_idx13==-1) ? null_val : layer_blk_img[0][lut_idx13].g;
                    L[27] = (lut_idx13==-1) ? null_val : layer_blk_img[1][lut_idx13].g;
                    L[28] = (lut_idx14==-1) ? null_val : layer_blk_img[0][lut_idx14].g;
                    L[29] = (lut_idx14==-1) ? null_val : layer_blk_img[1][lut_idx14].g;
                    L[30] = (lut_idx15==-1) ? null_val : layer_blk_img[0][lut_idx15].g;
                    L[31] = (lut_idx15==-1) ? null_val : layer_blk_img[1][lut_idx15].g;
                    break;
                case 3:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = null_val;
                    L[ 4] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 5] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[ 6] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 9] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[10] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[11] = null_val;
                    L[12] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[13] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[14] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[15] = null_val;
                    L[16] = (lut_idx04==-1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[17] = (lut_idx04==-1) ? null_val : layer_blk_img[1][lut_idx04].g;
                    L[18] = (lut_idx04==-1) ? null_val : layer_blk_img[2][lut_idx04].g;
                    L[19] = null_val;
                    L[20] = (lut_idx05==-1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[21] = (lut_idx05==-1) ? null_val : layer_blk_img[1][lut_idx05].g;
                    L[22] = (lut_idx05==-1) ? null_val : layer_blk_img[2][lut_idx05].g;
                    L[23] = null_val;
                    L[24] = (lut_idx06==-1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[25] = (lut_idx06==-1) ? null_val : layer_blk_img[1][lut_idx06].g;
                    L[26] = (lut_idx06==-1) ? null_val : layer_blk_img[2][lut_idx06].g;
                    L[27] = null_val;
                    L[28] = (lut_idx07==-1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[29] = (lut_idx07==-1) ? null_val : layer_blk_img[1][lut_idx07].g;
                    L[30] = (lut_idx07==-1) ? null_val : layer_blk_img[2][lut_idx07].g;
                    L[31] = null_val;
                    break;            
                case 4:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 5] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[ 6] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[ 7] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[ 8] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 9] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[10] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[11] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[12] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[13] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[14] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[15] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[16] = (lut_idx04==-1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[17] = (lut_idx04==-1) ? null_val : layer_blk_img[1][lut_idx04].g;
                    L[18] = (lut_idx04==-1) ? null_val : layer_blk_img[2][lut_idx04].g;
                    L[19] = (lut_idx04==-1) ? null_val : layer_blk_img[3][lut_idx04].g;
                    L[20] = (lut_idx05==-1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[21] = (lut_idx05==-1) ? null_val : layer_blk_img[1][lut_idx05].g;
                    L[22] = (lut_idx05==-1) ? null_val : layer_blk_img[2][lut_idx05].g;
                    L[23] = (lut_idx05==-1) ? null_val : layer_blk_img[3][lut_idx05].g;
                    L[24] = (lut_idx06==-1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[25] = (lut_idx06==-1) ? null_val : layer_blk_img[1][lut_idx06].g;
                    L[26] = (lut_idx06==-1) ? null_val : layer_blk_img[2][lut_idx06].g;
                    L[27] = (lut_idx06==-1) ? null_val : layer_blk_img[3][lut_idx06].g;
                    L[28] = (lut_idx07==-1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[29] = (lut_idx07==-1) ? null_val : layer_blk_img[1][lut_idx07].g;
                    L[30] = (lut_idx07==-1) ? null_val : layer_blk_img[2][lut_idx07].g;
                    L[31] = (lut_idx07==-1) ? null_val : layer_blk_img[3][lut_idx07].g;
                    break;
                case 5:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = null_val;
                    L[ 6] = null_val;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = null_val;
                    L[14] = null_val;
                    L[15] = null_val;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = null_val;
                    L[22] = null_val;
                    L[23] = null_val;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = null_val;
                    L[30] = null_val;
                    L[31] = null_val;
                    break;
                case 6:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = (lut_idx00==-1) ? null_val : layer_blk_img[5][lut_idx00].g;
                    L[ 6] = null_val;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = (lut_idx01==-1) ? null_val : layer_blk_img[5][lut_idx01].g;
                    L[14] = null_val;
                    L[15] = null_val;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = (lut_idx02==-1) ? null_val : layer_blk_img[5][lut_idx02].g;
                    L[22] = null_val;
                    L[23] = null_val;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = (lut_idx03==-1) ? null_val : layer_blk_img[5][lut_idx03].g;
                    L[30] = null_val;
                    L[31] = null_val;
                    break;
                case 7:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = (lut_idx00==-1) ? null_val : layer_blk_img[5][lut_idx00].g;
                    L[ 6] = (lut_idx00==-1) ? null_val : layer_blk_img[6][lut_idx00].g;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = (lut_idx01==-1) ? null_val : layer_blk_img[5][lut_idx01].g;
                    L[14] = (lut_idx01==-1) ? null_val : layer_blk_img[6][lut_idx01].g;
                    L[15] = null_val;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = (lut_idx02==-1) ? null_val : layer_blk_img[5][lut_idx02].g;
                    L[22] = (lut_idx02==-1) ? null_val : layer_blk_img[6][lut_idx02].g;
                    L[23] = null_val;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = (lut_idx03==-1) ? null_val : layer_blk_img[5][lut_idx03].g;
                    L[30] = (lut_idx03==-1) ? null_val : layer_blk_img[6][lut_idx03].g;
                    L[31] = null_val;
                    break;
                case 8:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = (lut_idx00==-1) ? null_val : layer_blk_img[5][lut_idx00].g;
                    L[ 6] = (lut_idx00==-1) ? null_val : layer_blk_img[6][lut_idx00].g;
                    L[ 7] = (lut_idx00==-1) ? null_val : layer_blk_img[7][lut_idx00].g;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = (lut_idx01==-1) ? null_val : layer_blk_img[5][lut_idx01].g;
                    L[14] = (lut_idx01==-1) ? null_val : layer_blk_img[6][lut_idx01].g;
                    L[15] = (lut_idx01==-1) ? null_val : layer_blk_img[7][lut_idx01].g;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = (lut_idx02==-1) ? null_val : layer_blk_img[5][lut_idx02].g;
                    L[22] = (lut_idx02==-1) ? null_val : layer_blk_img[6][lut_idx02].g;
                    L[23] = (lut_idx02==-1) ? null_val : layer_blk_img[7][lut_idx02].g;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = (lut_idx03==-1) ? null_val : layer_blk_img[5][lut_idx03].g;
                    L[30] = (lut_idx03==-1) ? null_val : layer_blk_img[6][lut_idx03].g;
                    L[31] = (lut_idx03==-1) ? null_val : layer_blk_img[7][lut_idx03].g;
                    break;
                }

                int idx;
                for (idx = 0; idx < 32; idx++)
                {
                    char rr = (char)((L[idx] < 0.0) ? (256.0 + L[idx]) : (L[idx]));
                    Lut_out[out_idx++]= rr;
                }
            }
        }
    }

    pLutOutVector->dat_num += out_idx;
    
    if (pLutOutVector->dat_num > pLutOutVector->max_num)
    {
        printf("\n\033[31;1mError !\033[0m\n");
        printf("Lut_out_vector data num  : 0x%x\n", (unsigned int)pLutOutVector->dat_num);
        printf("Lut_out_vector max  num  : 0x%x\n", (unsigned int)pLutOutVector->max_num);
    }

}

