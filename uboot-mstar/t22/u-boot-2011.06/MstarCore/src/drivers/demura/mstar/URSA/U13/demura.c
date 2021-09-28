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
#include <linux/string.h>
#include "CommonDataType.h"
#include <dmalloc.h>
#include <ms_vector.h>
#include <demura.h>
#include <demura_common.h>


MS_BOOL demura_core_ver10(int base_address, int lut_h_size, int lut_v_size, interface_info *pInfo, \
                        m_vector *pRegVector, m_vector *pLut_out_vector)
{
    int reg_bank = base_address;  // 0x11200

    registers _reg = {
        {{reg_bank+0x20,0xFF,0x00,"reg_dmc_black_limit"          }, {reg_bank+0x21,0x0F,0x00,"reg_dmc_black_limit"          }},
        {{reg_bank+0x22,0xFF,0xff,"reg_dmc_plane_level1"         }, {reg_bank+0x23,0x0F,0x0f,"reg_dmc_plane_level1"         }},
        {{reg_bank+0x24,0xFF,0xff,"reg_dmc_plane_level2"         }, {reg_bank+0x25,0x0F,0x0f,"reg_dmc_plane_level2"         }},
        {{reg_bank+0x26,0xFF,0xff,"reg_dmc_plane_level3"         }, {reg_bank+0x27,0x0F,0x0f,"reg_dmc_plane_level3"         }},
        {{reg_bank+0x28,0xFF,0xff,"reg_dmc_plane_level4"         }, {reg_bank+0x29,0x0F,0x0f,"reg_dmc_plane_level4"         }},
        {{reg_bank+0x2A,0xFF,0xff,"reg_dmc_plane_level5"         }, {reg_bank+0x2B,0x0F,0x0f,"reg_dmc_plane_level5"         }},
        {{reg_bank+0x2C,0xFF,0xff,"reg_dmc_plane_level6"         }, {reg_bank+0x2D,0x0F,0x0f,"reg_dmc_plane_level6"         }},
        {{reg_bank+0x2E,0xFF,0xff,"reg_dmc_plane_level7"         }, {reg_bank+0x2F,0x0F,0x0f,"reg_dmc_plane_level7"         }},
        {{reg_bank+0x30,0xFF,0xff,"reg_dmc_plane_level8"         }, {reg_bank+0x31,0x0F,0x0f,"reg_dmc_plane_level8"         }},
        {{reg_bank+0x32,0xFF,0xff,"reg_dmc_white_limit"          }, {reg_bank+0x33,0x0F,0x0f,"reg_dmc_white_limit"          }},
        {{reg_bank+0x35,0x70,0x00,"reg_dmc_data_r_mag4"          }},
        {{reg_bank+0x35,0x07,0x00,"reg_dmc_data_r_mag3"          }},
        {{reg_bank+0x34,0x70,0x00,"reg_dmc_data_r_mag2"          }},
        {{reg_bank+0x34,0x07,0x00,"reg_dmc_data_r_mag1"          }},
        {{reg_bank+0x37,0x70,0x00,"reg_dmc_data_r_mag8"          }},
        {{reg_bank+0x37,0x07,0x00,"reg_dmc_data_r_mag7"          }},
        {{reg_bank+0x36,0x70,0x00,"reg_dmc_data_r_mag6"          }},
        {{reg_bank+0x36,0x07,0x00,"reg_dmc_data_r_mag5"          }},
        {{reg_bank+0x39,0x70,0x00,"reg_dmc_data_g_mag4"          }},
        {{reg_bank+0x39,0x07,0x00,"reg_dmc_data_g_mag3"          }},
        {{reg_bank+0x38,0x70,0x00,"reg_dmc_data_g_mag2"          }},
        {{reg_bank+0x38,0x07,0x00,"reg_dmc_data_g_mag1"          }},
        {{reg_bank+0x3B,0x70,0x00,"reg_dmc_data_g_mag8"          }},
        {{reg_bank+0x3B,0x07,0x00,"reg_dmc_data_g_mag7"          }},
        {{reg_bank+0x3A,0x70,0x00,"reg_dmc_data_g_mag6"          }},
        {{reg_bank+0x3A,0x07,0x00,"reg_dmc_data_g_mag5"          }},
        {{reg_bank+0x3D,0x70,0x00,"reg_dmc_data_b_mag4"          }},
        {{reg_bank+0x3D,0x07,0x00,"reg_dmc_data_b_mag3"          }},
        {{reg_bank+0x3C,0x70,0x00,"reg_dmc_data_b_mag2"          }},
        {{reg_bank+0x3C,0x07,0x00,"reg_dmc_data_b_mag1"          }},
        {{reg_bank+0x3F,0x70,0x00,"reg_dmc_data_b_mag8"          }},
        {{reg_bank+0x3F,0x07,0x00,"reg_dmc_data_b_mag7"          }},
        {{reg_bank+0x3E,0x70,0x00,"reg_dmc_data_b_mag6"          }},
        {{reg_bank+0x3E,0x07,0x00,"reg_dmc_data_b_mag5"          }},
        {{reg_bank+0x40,0xFF,0x00,"reg_dmc_plane_b1_coef"        }, {reg_bank+0x41,0x3F,0x00,"reg_dmc_plane_b1_coef"        }},
        {{reg_bank+0x42,0xFF,0x00,"reg_dmc_plane_12_coef"        }, {reg_bank+0x43,0x3F,0x00,"reg_dmc_plane_12_coef"        }},
        {{reg_bank+0x44,0xFF,0x00,"reg_dmc_plane_23_coef"        }, {reg_bank+0x45,0x3F,0x00,"reg_dmc_plane_23_coef"        }},
        {{reg_bank+0x46,0xFF,0x00,"reg_dmc_plane_34_coef"        }, {reg_bank+0x47,0x3F,0x00,"reg_dmc_plane_34_coef"        }},
        {{reg_bank+0x48,0xFF,0x00,"reg_dmc_plane_45_coef"        }, {reg_bank+0x49,0x3F,0x00,"reg_dmc_plane_45_coef"        }},
        {{reg_bank+0x4A,0xFF,0x00,"reg_dmc_plane_56_coef"        }, {reg_bank+0x4B,0x3F,0x00,"reg_dmc_plane_56_coef"        }},
        {{reg_bank+0x4C,0xFF,0x00,"reg_dmc_plane_67_coef"        }, {reg_bank+0x4D,0x3F,0x00,"reg_dmc_plane_67_coef"        }},
        {{reg_bank+0x4E,0xFF,0x00,"reg_dmc_plane_78_coef"        }, {reg_bank+0x4F,0x3F,0x00,"reg_dmc_plane_78_coef"        }},
        {{reg_bank+0x50,0xFF,0x00,"reg_dmc_plane_8w_coef"        }, {reg_bank+0x51,0x3F,0x00,"reg_dmc_plane_8w_coef"        }},
        {{reg_bank+0x53,0x01,0x00,"reg_dmc_plane_8w_ks22"        }},
        {{reg_bank+0x52,0x80,0x00,"reg_dmc_plane_78_ks22"        }},
        {{reg_bank+0x52,0x40,0x00,"reg_dmc_plane_67_ks22"        }},
        {{reg_bank+0x52,0x20,0x00,"reg_dmc_plane_56_ks22"        }},
        {{reg_bank+0x52,0x10,0x00,"reg_dmc_plane_45_ks22"        }},
        {{reg_bank+0x52,0x08,0x00,"reg_dmc_plane_34_ks22"        }},
        {{reg_bank+0x52,0x04,0x00,"reg_dmc_plane_23_ks22"        }},
        {{reg_bank+0x52,0x02,0x00,"reg_dmc_plane_12_ks22"        }},
        {{reg_bank+0x52,0x01,0x00,"reg_dmc_plane_b1_ks22"        }},
        {{reg_bank+0x55,0x70,0x30,"reg_dmc_h_block"              }},
        {{reg_bank+0x55,0x07,0x03,"reg_dmc_v_block"              }},
        {{reg_bank+0x54,0x80,0x80,"reg_dmc_rgb_mode"             }},
        {{reg_bank+0x54,0x30,0x30,"reg_dmc_block_size"           }},
        {{reg_bank+0x54,0x0F,0x03,"reg_dmc_plane_num"            }},
        {{reg_bank+0x56,0xFF,0xe1,"reg_dmc_h_lut_num"            }, {reg_bank+0x57,0x07,0x01,"reg_dmc_h_lut_num"            }},
        {{reg_bank+0x58,0xFF,0x0f,"reg_dmc_v_lut_num"            }, {reg_bank+0x59,0x07,0x01,"reg_dmc_v_lut_num"            }},
        {{reg_bank+0x99,0x80,0x00,"reg_dmc_flow_ctrl_en"         }},
        {{reg_bank+0x98,0xFF,0x00,"reg_dmc_flow_len"             }, {reg_bank+0x99,0x07,0x00,"reg_dmc_flow_len"             }},
        {{reg_bank+0x97,0x80,0x00,"reg_dmc_grid_line_en"         }},
        {{reg_bank+0x97,0x70,0x00,"reg_dmc_grid_line_size"       }},
        {{reg_bank+0x96,0xFF,0x00,"reg_dmc_grid_line_color"      }},
        {{reg_bank+0x5F,0x80,0x00,"reg_dmc_dither_rst_e_4_frame" }},
        {{reg_bank+0x5F,0x30,0x00,"reg_dmc_dither_pse_rst_num"   }},
        {{reg_bank+0x5E,0x30,0x00,"reg_dmc_dither_en"            }},
        {{reg_bank+0x5E,0x04,0x00,"reg_dmc_r_en"                 }},
        {{reg_bank+0x5E,0x02,0x00,"reg_dmc_g_en"                 }},
        {{reg_bank+0x5E,0x01,0x00,"reg_dmc_b_en"                 }},
        {{reg_bank+0x60,0xFF,0x00,"reg_dmc_data_r_offset1"       }, {reg_bank+0x61,0x3F,0x00,"reg_dmc_data_r_offset1"       }},
        {{reg_bank+0x62,0xFF,0x00,"reg_dmc_data_r_offset2"       }, {reg_bank+0x63,0x3F,0x00,"reg_dmc_data_r_offset2"       }},
        {{reg_bank+0x64,0xFF,0x00,"reg_dmc_data_r_offset3"       }, {reg_bank+0x65,0x3F,0x00,"reg_dmc_data_r_offset3"       }},
        {{reg_bank+0x66,0xFF,0x00,"reg_dmc_data_r_offset4"       }, {reg_bank+0x67,0x3F,0x00,"reg_dmc_data_r_offset4"       }},
        {{reg_bank+0x68,0xFF,0x00,"reg_dmc_data_r_offset5"       }, {reg_bank+0x69,0x3F,0x00,"reg_dmc_data_r_offset5"       }},
        {{reg_bank+0x6A,0xFF,0x00,"reg_dmc_data_r_offset6"       }, {reg_bank+0x6B,0x3F,0x00,"reg_dmc_data_r_offset6"       }},
        {{reg_bank+0x6C,0xFF,0x00,"reg_dmc_data_r_offset7"       }, {reg_bank+0x6D,0x3F,0x00,"reg_dmc_data_r_offset7"       }},
        {{reg_bank+0x6E,0xFF,0x00,"reg_dmc_data_r_offset8"       }, {reg_bank+0x6F,0x3F,0x00,"reg_dmc_data_r_offset8"       }},
        {{reg_bank+0x70,0xFF,0x00,"reg_dmc_data_g_offset1"       }, {reg_bank+0x71,0x3F,0x00,"reg_dmc_data_g_offset1"       }},
        {{reg_bank+0x72,0xFF,0x00,"reg_dmc_data_g_offset2"       }, {reg_bank+0x73,0x3F,0x00,"reg_dmc_data_g_offset2"       }},
        {{reg_bank+0x74,0xFF,0x00,"reg_dmc_data_g_offset3"       }, {reg_bank+0x75,0x3F,0x00,"reg_dmc_data_g_offset3"       }},
        {{reg_bank+0x76,0xFF,0x00,"reg_dmc_data_g_offset4"       }, {reg_bank+0x77,0x3F,0x00,"reg_dmc_data_g_offset4"       }},
        {{reg_bank+0x78,0xFF,0x00,"reg_dmc_data_g_offset5"       }, {reg_bank+0x79,0x3F,0x00,"reg_dmc_data_g_offset5"       }},
        {{reg_bank+0x7A,0xFF,0x00,"reg_dmc_data_g_offset6"       }, {reg_bank+0x7B,0x3F,0x00,"reg_dmc_data_g_offset6"       }},
        {{reg_bank+0x7C,0xFF,0x00,"reg_dmc_data_g_offset7"       }, {reg_bank+0x7D,0x3F,0x00,"reg_dmc_data_g_offset7"       }},
        {{reg_bank+0x7E,0xFF,0x00,"reg_dmc_data_g_offset8"       }, {reg_bank+0x7F,0x3F,0x00,"reg_dmc_data_g_offset8"       }},
        {{reg_bank+0x80,0xFF,0x00,"reg_dmc_data_b_offset1"       }, {reg_bank+0x81,0x3F,0x00,"reg_dmc_data_b_offset1"       }},
        {{reg_bank+0x82,0xFF,0x00,"reg_dmc_data_b_offset2"       }, {reg_bank+0x83,0x3F,0x00,"reg_dmc_data_b_offset2"       }},
        {{reg_bank+0x84,0xFF,0x00,"reg_dmc_data_b_offset3"       }, {reg_bank+0x85,0x3F,0x00,"reg_dmc_data_b_offset3"       }},
        {{reg_bank+0x86,0xFF,0x00,"reg_dmc_data_b_offset4"       }, {reg_bank+0x87,0x3F,0x00,"reg_dmc_data_b_offset4"       }},
        {{reg_bank+0x88,0xFF,0x00,"reg_dmc_data_b_offset5"       }, {reg_bank+0x89,0x3F,0x00,"reg_dmc_data_b_offset5"       }},
        {{reg_bank+0x8A,0xFF,0x00,"reg_dmc_data_b_offset6"       }, {reg_bank+0x8B,0x3F,0x00,"reg_dmc_data_b_offset6"       }},
        {{reg_bank+0x8C,0xFF,0x00,"reg_dmc_data_b_offset7"       }, {reg_bank+0x8D,0x3F,0x00,"reg_dmc_data_b_offset7"       }},
        {{reg_bank+0x8E,0xFF,0x00,"reg_dmc_data_b_offset8"       }, {reg_bank+0x8F,0x3F,0x00,"reg_dmc_data_b_offset8"       }},
        {{reg_bank+0x90,0xFF,0x00,"reg_dmc_panel_h_size"         }, {reg_bank+0x91,0x1F,0x0f,"reg_dmc_panel_h_size"         }},
        {{reg_bank+0x92,0xFF,0x02,"reg_dmc_preload_line"         }, {reg_bank+0x93,0x0F,0x00,"reg_dmc_preload_line"         }},
        {{reg_bank+0x94,0x3F,0x00,"reg_dmc_mono_mode_q_h_r"      }},
        {{reg_bank+0x95,0x07,0x00,"reg_dmc_mono_mode_r_h_r"      }}
    };

    int Bit07_00;
    int Bit15_08;
    
    //----------------------- interface input start -----------------------//
    int reg_dmc_plane_num                 = (*pInfo).reg_dmc_plane_num; // 1 ~ 8
    int reg_dmc_h_block                   = (*pInfo).reg_dmc_h_block; // 2 ~ 4 for U13 , 2 ~ 6 for Odinson , 2 : 4 pixel , 3 : 8 pixel , 4 : 16 pixel , 5 : 32 pixel , 6 : 64 pixel
    int reg_dmc_v_block                   = (*pInfo).reg_dmc_v_block; // 2 ~ 4 for U13 , 2 ~ 6 for Odinson , 2 : 4 pixel , 3 : 8 pixel , 4 : 16 pixel , 5 : 32 pixel , 6 : 64 pixel
    int reg_dmc_rgb_mode                  = (*pInfo).reg_dmc_rgb_mode; // 0 : mono mode , 1 : rgb mode
    int reg_dmc_panel_h_size              = (*pInfo).reg_dmc_panel_h_size; // panel real horizontal size
    //int reg_dmc_r_en                      = 1;
    //int reg_dmc_g_en                      = 1;
    //int reg_dmc_b_en                      = 1;
    int reg_dmc_black_limit               = (*pInfo).reg_dmc_black_limit; // 12 bit , format 10.2     
    int reg_dmc_plane_level1              = (*pInfo).reg_dmc_plane_level1; // 12 bit , format 10.2    
    int reg_dmc_plane_level2              = (*pInfo).reg_dmc_plane_level2; // 12 bit , format 10.2    
    int reg_dmc_plane_level3              = (*pInfo).reg_dmc_plane_level3; // 12 bit , format 10.2   
    int reg_dmc_plane_level4              = (*pInfo).reg_dmc_plane_level4; // 12 bit , format 10.2     
    int reg_dmc_plane_level5              = (*pInfo).reg_dmc_plane_level5; // 12 bit , format 10.2      
    int reg_dmc_plane_level6              = (*pInfo).reg_dmc_plane_level6; // 12 bit , format 10.2   
    int reg_dmc_plane_level7              = (*pInfo).reg_dmc_plane_level7; // 12 bit , format 10.2      
    int reg_dmc_plane_level8              = (*pInfo).reg_dmc_plane_level8; // 12 bit , format 10.2     
    int reg_dmc_white_limit               = (*pInfo).reg_dmc_white_limit; // 12 bit , format 10.2     
    int reg_dmc_dither_rst_e_4_frame      = (*pInfo).reg_dmc_dither_rst_e_4_frame; // 0 : dither frame reset off , 1 : dither frame reset on
    int reg_dmc_dither_pse_rst_num        = (*pInfo).reg_dmc_dither_pse_rst_num; // 0 : 1 frame reset , 1 : 2 frame reset , 2 : 4 frame reset , 3 : 8 frame reset 
    int reg_dmc_dither_en                 = (*pInfo).reg_dmc_dither_en; // 2 bit , 0 : rounding , 1 : dither , 2~3 truncate
    //----------------------- interface input end -----------------------//

    //----------------------- register plane number start -----------------------//
    _reg.reg_dmc_plane_num.Bit07_00.value               = register_shift(_reg.reg_dmc_plane_num.Bit07_00.mask, reg_dmc_plane_num);
    //----------------------- register plane number end -----------------------//

    //----------------------- register H\V block size start -----------------------//
    _reg.reg_dmc_h_block.Bit07_00.value                 = register_shift(_reg.reg_dmc_h_block.Bit07_00.mask, reg_dmc_h_block);
    _reg.reg_dmc_v_block.Bit07_00.value                 = register_shift(_reg.reg_dmc_v_block.Bit07_00.mask, reg_dmc_v_block);
    //----------------------- register H\V block size end -----------------------//

    //----------------------- register mono\rgb mode start -----------------------//
    _reg.reg_dmc_rgb_mode.Bit07_00.value                = register_shift(_reg.reg_dmc_rgb_mode.Bit07_00.mask, reg_dmc_rgb_mode);
    //----------------------- register mono\rgb mode end -----------------------//

    //----------------------- register panel real horizontal size start -----------------------//
    part_bit15_00( reg_dmc_panel_h_size, 13, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_panel_h_size.Bit07_00.value            = register_shift(_reg.reg_dmc_panel_h_size.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_panel_h_size.Bit15_08.value            = register_shift(_reg.reg_dmc_panel_h_size.Bit15_08.mask, Bit15_08);
    //----------------------- register panel real horizontal size end -----------------------//

    //--------------------- register R\G\B enable start ---------------------//
    //_reg.reg_dmc_r_en.Bit07_00.value                    = register_shift(_reg.reg_dmc_r_en.Bit07_00.mask, reg_dmc_r_en);
    //_reg.reg_dmc_g_en.Bit07_00.value                    = register_shift(_reg.reg_dmc_g_en.Bit07_00.mask, reg_dmc_g_en);
    //_reg.reg_dmc_b_en.Bit07_00.value                    = register_shift(_reg.reg_dmc_b_en.Bit07_00.mask, reg_dmc_b_en);
    //--------------------- register R\G\B enable end ---------------------//

    //--------------------- register Layer start ---------------------//
    part_bit15_00( reg_dmc_black_limit, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_black_limit.Bit07_00.value             = register_shift(_reg.reg_dmc_black_limit.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_black_limit.Bit15_08.value             = register_shift(_reg.reg_dmc_black_limit.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level1, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level1.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level1.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level1.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level1.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level2, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level2.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level2.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level2.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level2.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level3, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level3.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level3.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level3.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level3.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level4, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level4.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level4.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level4.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level4.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level5, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level5.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level5.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level5.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level5.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level6, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level6.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level6.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level6.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level6.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level7, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level7.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level7.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level7.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level7.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_plane_level8, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_level8.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_level8.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_level8.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_level8.Bit15_08.mask, Bit15_08);

    part_bit15_00( reg_dmc_white_limit, 12, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_white_limit.Bit07_00.value             = register_shift(_reg.reg_dmc_white_limit.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_white_limit.Bit15_08.value             = register_shift(_reg.reg_dmc_white_limit.Bit15_08.mask, Bit15_08);
    //--------------------- register Layer end ---------------------//

    //--------------------- register Dither start ---------------------//
    _reg.reg_dmc_dither_rst_e_4_frame.Bit07_00.value    = register_shift(_reg.reg_dmc_dither_rst_e_4_frame.Bit07_00.mask, reg_dmc_dither_rst_e_4_frame);
    _reg.reg_dmc_dither_pse_rst_num.Bit07_00.value      = register_shift(_reg.reg_dmc_dither_pse_rst_num.Bit07_00.mask, reg_dmc_dither_pse_rst_num);
    _reg.reg_dmc_dither_en.Bit07_00.value               = register_shift(_reg.reg_dmc_dither_en.Bit07_00.mask, reg_dmc_dither_en);
    //--------------------- register Dither end ---------------------//

    //--------------------- register Layer coeff and ks22 start ---------------------//
    int diff_b1 = MAX(reg_dmc_plane_level1*4 - reg_dmc_black_limit*4,  1); // layer distance is 14 bit , input layer is 12 (10.2) bit , so shift 2 bit keep 14 bit
    int diff_12 = MAX(reg_dmc_plane_level2*4 - reg_dmc_plane_level1*4, 1);
    int diff_23 = MAX(reg_dmc_plane_level3*4 - reg_dmc_plane_level2*4, 1);
    int diff_34 = MAX(reg_dmc_plane_level4*4 - reg_dmc_plane_level3*4, 1);
    int diff_45 = MAX(reg_dmc_plane_level5*4 - reg_dmc_plane_level4*4, 1);
    int diff_56 = MAX(reg_dmc_plane_level6*4 - reg_dmc_plane_level5*4, 1);
    int diff_67 = MAX(reg_dmc_plane_level7*4 - reg_dmc_plane_level6*4, 1);
    int diff_78 = MAX(reg_dmc_plane_level8*4 - reg_dmc_plane_level7*4, 1);
    int diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level8*4, 1);

    switch(reg_dmc_plane_num)
    {
    case 1:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level1*4, 1);
        break;
    case 2:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level2*4, 1);
        break;
    case 3:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level3*4, 1);
        break;
    case 4:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level4*4, 1);
        break;
    case 5:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level5*4, 1);
        break;
    case 6:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level6*4, 1);
        break;
    case 7:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level7*4, 1);
        break;
    case 8:
        diff_8w = MAX(reg_dmc_white_limit*4  - reg_dmc_plane_level8*4, 1);
        break;
    default:
        break;
    }


    int coeff, ks;
    coeff_and_ks( diff_b1, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_b1_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_b1_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_b1_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_b1_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_b1_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_b1_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_12, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_12_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_12_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_12_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_12_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_12_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_12_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_23, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_23_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_23_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_23_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_23_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_23_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_23_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_34, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_34_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_34_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_34_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_34_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_34_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_34_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_45, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_45_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_45_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_45_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_45_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_45_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_45_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_56, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_56_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_56_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_56_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_56_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_56_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_56_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_67, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_67_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_67_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_67_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_67_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_67_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_67_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_78, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_78_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_78_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_78_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_78_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_78_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_78_ks22.Bit07_00.mask, ks);

    coeff_and_ks( diff_8w, &coeff, &ks);
    part_bit15_00( coeff, 14, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_plane_8w_coef.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_8w_coef.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_plane_8w_coef.Bit15_08.value             = register_shift(_reg.reg_dmc_plane_8w_coef.Bit15_08.mask, Bit15_08);
    _reg.reg_dmc_plane_8w_ks22.Bit07_00.value             = register_shift(_reg.reg_dmc_plane_8w_ks22.Bit07_00.mask, ks);
    //--------------------- register Layer coeff and ks22 end ---------------------//

    //--------------------- register H_lut_num/V_lut_num start ---------------------//
    int block_height = lut_v_size;
    int reg_dmc_v_lut_num = block_height;

    int block_width = lut_h_size;
    int reg_dmc_h_lut_num = block_width;


    if(reg_dmc_rgb_mode==1)
    {
        int div_val;
        switch (reg_dmc_plane_num)
        {
        case 1:
        case 2:
            div_val = 4;
            break;
        case 3:
        case 4:
            div_val = 2;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            div_val = 1;
            break;
        }

        int reg_dmc_h_lut_num_tmp = reg_dmc_h_lut_num;

        reg_dmc_h_lut_num = (reg_dmc_h_lut_num_tmp/div_val) + (((reg_dmc_h_lut_num_tmp%div_val) > 0) ? 1 : 0);

    }
    else
    {
        int div_val;
        switch (reg_dmc_plane_num)
        {
        case 1:
        case 2:
            div_val = 16;
            break;
        case 3:
        case 4:
            div_val = 8;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            div_val = 4;
            break;
        }

        int reg_dmc_h_lut_num_tmp = reg_dmc_h_lut_num;

        reg_dmc_h_lut_num = (reg_dmc_h_lut_num_tmp/div_val) + (((reg_dmc_h_lut_num_tmp%div_val) > 0) ? 1 : 0);
    }

    part_bit15_00(reg_dmc_v_lut_num, 11, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_v_lut_num.Bit07_00.value                         = register_shift(_reg.reg_dmc_v_lut_num.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_v_lut_num.Bit15_08.value                         = register_shift(_reg.reg_dmc_v_lut_num.Bit15_08.mask, Bit15_08);

    part_bit15_00(reg_dmc_h_lut_num, 11, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_h_lut_num.Bit07_00.value                         = register_shift(_reg.reg_dmc_h_lut_num.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_h_lut_num.Bit15_08.value                         = register_shift(_reg.reg_dmc_h_lut_num.Bit15_08.mask, Bit15_08);
    //--------------------- register H_lut_num/V_lut_num end ---------------------//

    //--------------------- register demura flow control enable start---------------------//
    part_bit15_00(reg_dmc_h_lut_num, 11, &Bit15_08, &Bit07_00);
    _reg.reg_dmc_flow_len.Bit07_00.value                          = register_shift(_reg.reg_dmc_flow_len.Bit07_00.mask, Bit07_00);
    _reg.reg_dmc_flow_len.Bit15_08.value                          = register_shift(_reg.reg_dmc_flow_len.Bit15_08.mask, Bit15_08);
    
    _reg.reg_dmc_flow_ctrl_en.Bit07_00.value                      = register_shift(_reg.reg_dmc_flow_ctrl_en.Bit07_00.mask, 1);
    //--------------------- register demura flow control enable end---------------------//
    

    //--------------------- layer LUT initial start ---------------------//
    
    strgb_structInfo *layerall_blk_img[8];
    memset(layerall_blk_img, 0, sizeof(layerall_blk_img));
    
    //--------------------- layer LUT initial end ---------------------//


    //--------------------- calculation layer LUT start ---------------------//
    int plane_idx;
    for (plane_idx = 0; plane_idx < reg_dmc_plane_num; plane_idx++)
    {

        int reg_dmc_plane_level = reg_dmc_plane_level1;
        switch(plane_idx)
        {
        case 0:
            reg_dmc_plane_level = reg_dmc_plane_level1;
            break;
        case 1:
            reg_dmc_plane_level = reg_dmc_plane_level2;
            break;
        case 2:
            reg_dmc_plane_level = reg_dmc_plane_level3;
            break;
        case 3:
            reg_dmc_plane_level = reg_dmc_plane_level4;
            break;
        case 4:
            reg_dmc_plane_level = reg_dmc_plane_level5;
            break;
        case 5:
            reg_dmc_plane_level = reg_dmc_plane_level6;
            break;
        case 6:
            reg_dmc_plane_level = reg_dmc_plane_level7;
            break;
        case 7:
            reg_dmc_plane_level = reg_dmc_plane_level8;
            break;
        }

        {
            // Reduce memory-usage  step-1
            int layer_blk_img_size = sizeof(strgb_structInfo) * block_height * block_width;
            char *pblk_img = dmalloc(layer_blk_img_size);
            CHECK_DMALLOC_SPACE(pblk_img, (MS_U32)layer_blk_img_size);
            layerall_blk_img[plane_idx] = (strgb_structInfo *)pblk_img;
            
            int CurV, CurH;
            for (CurV = 0; CurV < block_height; CurV++)
            {
                for (CurH = 0; CurH < block_width; CurH++)
                {
                    layerall_blk_img[plane_idx][posi_blk(CurV,CurH)].r    = 0;
                    layerall_blk_img[plane_idx][posi_blk(CurV,CurH)].g    = 0;
                    layerall_blk_img[plane_idx][posi_blk(CurV,CurH)].b    = 0;
                    layerall_blk_img[plane_idx][posi_blk(CurV,CurH)].dbr  = 0.0;
                    layerall_blk_img[plane_idx][posi_blk(CurV,CurH)].dbg  = 0.0;
                    layerall_blk_img[plane_idx][posi_blk(CurV,CurH)].dbb  = 0.0;
                }
            }
        }

        //calculation LUT¡Bmag©Moffset register
        LUT_AND_SETTING_PROCESSOR(plane_idx, block_width, block_height, reg_dmc_plane_level, &_reg , (*pInfo).Lut_in[plane_idx], layerall_blk_img[plane_idx]);
        
        // Reduce memory-usage  step-2
        dfree( (*pInfo).Lut_in[plane_idx] );
        (*pInfo).Lut_in[plane_idx] = NULL;
    }
    //--------------------- calculation layer LUT end ---------------------//

    stlayer_info_structInfo layer_info;
    layer_info.dram_h_size = block_width;
    layer_info.dram_v_size = block_height;
    layer_info.dmc_rgb_mode = reg_dmc_rgb_mode;
    layer_info.real_dram_h_size = reg_dmc_h_lut_num;
    layer_info.real_dram_v_size = reg_dmc_v_lut_num;
    layer_info.dmc_plane_num = reg_dmc_plane_num;
    write_demuraLUT_to_HW_DRAM_FORMAT(&layer_info, layerall_blk_img, pLut_out_vector);

    
    for (plane_idx = 0; plane_idx < reg_dmc_plane_num; plane_idx++)
    {
        dfree(layerall_blk_img[plane_idx]);     // free layerall_blk_img arrary
    }
    /*
    for (plane_idx = reg_dmc_plane_num; plane_idx < 8; plane_idx++)
    {
        dfree( (*pInfo).Lut_in[plane_idx] );
        (*pInfo).Lut_in[plane_idx] = NULL;
    }
    */

    //---------------- dump register to vector start ----------------//
    int out_idx = 0;
    reg_struct *regList = (reg_struct *)pRegVector->pbuf + pRegVector->dat_num;
    
    regList[out_idx++] = (_reg.reg_dmc_black_limit.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_black_limit.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level1.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level2.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level3.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level4.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level5.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level6.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level7.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_level8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_level8.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_white_limit.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_white_limit.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_mag5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_mag5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_mag5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_b1_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_b1_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_12_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_12_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_23_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_23_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_34_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_34_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_45_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_45_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_56_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_56_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_67_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_67_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_78_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_78_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_8w_coef.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_8w_coef.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_plane_8w_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_78_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_67_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_56_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_45_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_34_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_23_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_12_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_b1_ks22.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_h_block.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_v_block.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_rgb_mode.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_block_size.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_plane_num.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_h_lut_num.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_h_lut_num.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_v_lut_num.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_v_lut_num.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_flow_ctrl_en.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_flow_len.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_flow_len.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_grid_line_en.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_grid_line_size.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_grid_line_color.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_dither_rst_e_4_frame.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_dither_pse_rst_num.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_dither_en.Bit07_00);
    //regList[out_idx++] = (_reg.reg_dmc_r_en.Bit07_00);
    //regList[out_idx++] = (_reg.reg_dmc_g_en.Bit07_00);
    //regList[out_idx++] = (_reg.reg_dmc_b_en.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset1.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset2.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset3.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset4.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset5.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset6.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset7.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_r_offset8.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset1.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset2.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset3.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset4.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset5.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset6.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset7.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_g_offset8.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset1.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset1.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset2.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset2.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset3.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset3.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset4.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset4.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset5.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset5.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset6.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset6.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset7.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset7.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset8.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_data_b_offset8.Bit15_08);
    regList[out_idx++] = (_reg.reg_dmc_panel_h_size.Bit07_00);
    regList[out_idx++] = (_reg.reg_dmc_panel_h_size.Bit15_08);
    //regList[out_idx++] = (_reg.reg_dmc_preload_line.Bit07_00);
    //regList[out_idx++] = (_reg.reg_dmc_preload_line.Bit15_08);
    //regList[out_idx++] = (_reg.reg_dmc_mono_mode_q_h_r.Bit07_00);
    //regList[out_idx++] = (_reg.reg_dmc_mono_mode_r_h_r.Bit07_00);

    pRegVector->dat_num += out_idx;
    //---------------- dump register to vector end   ----------------//
    
    return TRUE;
}