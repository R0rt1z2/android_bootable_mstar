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

/*@ <IncludeGuard> @*/
#ifndef device_demodulator_msb1400_H
#define device_demodulator_msb1400_H
/*@ </IncludeGuard> @*/

/*@ <Include> @*/
//#include "mapi_base.h"
//#include "mapi_demodulator.h"
#include "msAPI_Tuner.h"
/*@ </Include> @*/


//#define     DEMOD_SLAVE_ID          0xD2
//#define     DEMOD_SLAVE_ID_2          0xC0


#ifndef MAPI_BOOL
#define MAPI_BOOL BOOLEAN
#endif
#ifndef MAPI_U8
#define MAPI_U8 U8
#endif
#ifndef MAPI_S8
#define MAPI_S8 S8
#endif
#ifndef MAPI_U16
#define MAPI_U16 U16
#endif
#ifndef MAPI_U32
#define MAPI_U32 U32
#endif
#ifndef MAPI_TRUE
#define MAPI_TRUE TRUE
#endif
#ifndef MAPI_FALSE
#define MAPI_FALSE FALSE
#endif

#define IF_FREQ_45_75                       0x01    //IF_FREQ_MN
#define IF_FREQ_38_90                       0x02    //IF_FREQ_B
#define IF_FREQ_38_00                       0x03    //IF_FREQ_PAL_38

#ifdef ATSC_SYSTEM
#define TUNER_IF_FREQ_KHz                   45750L
#define IF_BASE_FREQ                        IF_FREQ_45_75
#else
#define IF_BASE_FREQ                        IF_FREQ_38_90
#endif


#define SAW_FILTER_SECAML_MODE              1
#define SAW_FILTER_OTHERS_MODE              2

/*------------------------------------------------*/
/*  For Asia area (China & East South Asia),      */
/*  please enable this for non-standard signal !! */
/*------------------------------------------------*/

#ifndef VIF_ASIA_SIGNAL_OPTION
#define VIF_ASIA_SIGNAL_OPTION              0       // for Asia non-standard singal
#endif

#ifndef VIF_TUNER_TYPE
#define VIF_TUNER_TYPE                  0                   // 0: RF Tuner; 1: Silicon Tuner
#endif

#define VIF_IF_BASE_FREQ                    IF_BASE_FREQ    // IF base frequency
#define VIF_TUNER_STEP_SIZE                 TN_FREQ_STEP    // Tuner frequency step size

#define SAW_ARCH_DUAL_SAW                   0x00
#define SAW_ARCH_EXTERNAL_SINGLE_SAW        0x01
#define SAW_ARCH_SILICON_TUNER              0x02
#define SAW_ARCH_INTERNAL_SINGLE_SAW_DIF    0x02
#define SAW_ARCH_NO_SAW                     0x03
#define SAW_ARCH_INTERNAL_SINGLE_SAW_VIF    0x04
#define SAW_ARCH_NO_SAW_DIF                 0x05
#define SAW_ARCH_SILICON_TUNER_VIF          0x06

#if (VIF_TUNER_TYPE == 0)
#ifndef VIF_SAW_ARCH
#define VIF_SAW_ARCH                        SAW_ARCH_NO_SAW_DIF // 0: DUAL_SAW; 1: EXTERNAL_SINGLE_SAW; 2:INTERNAL_SINGLE_SAW_DIF(SILICON_TUNER); 3:NO_SAW; 4:INTERNAL_SINGLE_SAW_VIF
#endif
#else
#ifndef VIF_SAW_ARCH
#define VIF_SAW_ARCH                        SAW_ARCH_SILICON_TUNER // silicon tuner
#endif
#endif

#define VIF_TOP                             3               // [3:0], Take Over Point (TOP) adjustment (range 0~15) //berlin 20090918

#define VIF_TUNER_INTERNAL_RF_AGC           0               // 0: Not Use,1: Use
#define VIF_VGA_MAXIMUM   	 	            0x7000          // [15:0], VGA Maximum value

#if(VIF_TUNER_INTERNAL_RF_AGC == 1)
#define VIF_VGA_MINIMUM   	 	            0x6000          // [15:0], VGA Minimum value
#else
#define VIF_VGA_MINIMUM                     0x8000          // [15:0], VGA Minimum value
#endif

#define GAIN_DISTRIBUTION_THR               (VIF_VGA_MAXIMUM - 0x600)   // [15:0], AGC vga threshold of gain distribution
#define VIF_AGC_VGA_BASE                    (GAIN_DISTRIBUTION_THR>>7)  // [7:0], AGC vga base
#define VIF_AGC_VGA_OFFS                    0x40            // [7:0], AGC vga offset

#if(VIF_SAW_ARCH == SAW_ARCH_EXTERNAL_SINGLE_SAW || VIF_SAW_ARCH == SAW_ARCH_SILICON_TUNER || VIF_SAW_ARCH == SAW_ARCH_SILICON_TUNER_VIF)
#define VIF_AGC_REF_NEGATIVE   	        0x70                // [7:0], AGC reference level for negative modulation
#elif(VIF_SAW_ARCH == SAW_ARCH_DUAL_SAW)
#define VIF_AGC_REF_NEGATIVE   	        0x60                // [7:0], AGC reference level for negative modulation
#else
#define VIF_AGC_REF_NEGATIVE   	        0x60                // [7:0], AGC reference level for negative modulation
#endif

#if(VIF_SAW_ARCH == SAW_ARCH_NO_SAW || VIF_SAW_ARCH == SAW_ARCH_NO_SAW_DIF)
#define VIF_AGC_REF_POSITIVE                0x15            // [7:0], AGC reference level for positive modulation
#else
#define VIF_AGC_REF_POSITIVE   	            0x19            // [7:0], AGC reference level for positive modulation
#endif

#define VIF_DAGC1_REF                       0x26            // [5:0], DAGC1 reference level
#define VIF_DAGC2_REF                       0x26            // [5:0], DAGC2 reference level

#if(VIF_SAW_ARCH == SAW_ARCH_EXTERNAL_SINGLE_SAW || VIF_SAW_ARCH == SAW_ARCH_SILICON_TUNER || VIF_SAW_ARCH == SAW_ARCH_SILICON_TUNER_VIF)
#define VIF_DAGC1_GAIN_OV   	        0x1800              // [13:0], DAGC1 gain for positive modulation  //marco 20090918
#define VIF_DAGC2_GAIN_OV   	        0x1800              // [13:0], DAGC2 gain for positive modulation  //marco 20090918
#elif(VIF_SAW_ARCH == SAW_ARCH_DUAL_SAW)
#define VIF_DAGC1_GAIN_OV   	        0x1000              // [13:0], DAGC1 gain for positive modulation
#define VIF_DAGC2_GAIN_OV   	        0x1000              // [13:0], DAGC2 gain for positive modulation
#else
#define VIF_DAGC1_GAIN_OV   	        0x1800              // [13:0], DAGC1 gain for positive modulation
#define VIF_DAGC2_GAIN_OV   	        0x1800              // [13:0], DAGC2 gain for positive modulation
#endif

#define VIF_CR_LOCK_THR                     0x020       // [9:0], Lock threshold of CR
#define VIF_CR_THR                          0x0500      // [13:0], CR threshold
#define VIF_CR_LOCK_NUM                     0x08000     // [19:0], lock number of CR
#define VIF_CR_UNLOCK_NUM                   0x04000     // [19:0], Unlock number of CR
#define VIF_CR_PD_ERR_MAX                   0x3FFF      // [13:0], Maximum phase error of CR
#define VIF_CR_LOCK_LEAKY_SEL               1           // 0: lock leaky factor (2^-16); 1: squared lock leaky factor
#define VIF_CR_PD_X2                        1           // 0: lock 0 degree; 1: lock 0 or 180 degree
//PARA for VIF_ASIA_SIGNAL_OPTION
#define VIF_CR_KF1_ASIA                     0x03                // [3:0], Hardware frequency detection parameter 1 of CR
#define VIF_CR_KP1_ASIA                     0x03                // [3:0], Hardware proportional loop filter parameter 1 of CR
#define VIF_CR_KI1_ASIA                     0x03                // [3:0], Hardware integral loop filter parameter 1 of CR
#define VIF_CR_KP2_ASIA                     0x06                // [3:0], Hardware proportional loop filter parameter 2 of CR
#define VIF_CR_KI2_ASIA                     0x05                // [3:0], Hardware integral loop filter parameter 2 of CR
#define VIF_CR_KP_ASIA                      0x06                // [3:0], Software proportional loop filter parameter of CR
#define VIF_CR_KI_ASIA                      0x09                // [3:0], Software integral loop filter parameter of CR
#define VIF_CR_LPF_SEL_ASIA                 1                   // 0: LPF1; 1: LPF2
#define VIF_CR_PD_MODE_SEL_ASIA             0                   // CR PD mode select
#define VIF_CR_KP_KI_ADJUST_ASIA            1                   // 0: CR Kp/Ki adjust OFF; 1: CR Kp/Ki adjust ON
#define VIF_CR_KP_KI_ADJUST_GEAR_ASIA       2                    // number of gears for auto adjust, only 1, 2 is valid
#define VIF_CR_KP_KI_ADJUST_THR1_ASIA       0x05                // [7:0] CR Kp/Ki adjust threshold 1
#define VIF_CR_KP_KI_ADJUST_THR2_ASIA       0x10                // [7:0] CR Kp/Ki adjust threshold 2
#define VIF_CR_KP_KI_ADJUST_THR3_ASIA       0x03                // [7:0] CR Kp/Ki adjust threshold 3
//PARA for other case
#define VIF_CR_KF1                          0x08                // [3:0], Hardware frequency detection parameter 1 of CR
#define VIF_CR_KP1                          0x04                // [3:0], Hardware proportional loop filter parameter 1 of CR
#define VIF_CR_KI1                          0x05                // [3:0], Hardware integral loop filter parameter 1 of CR
#define VIF_CR_KP2                          0x05                // [3:0], Hardware proportional loop filter parameter 2 of CR
#define VIF_CR_KI2                          0x07                // [3:0], Hardware integral loop filter parameter 2 of CR
#define VIF_CR_KP                           0x05                // [3:0], Software proportional loop filter parameter of CR
#define VIF_CR_KI                           0x08                // [3:0], Software integral loop filter parameter of CR
#define VIF_CR_LPF_SEL                      1                   // 0: LPF1; 1: LPF2
#define VIF_CR_PD_MODE_SEL                  1                   // CR PD mode select
#define VIF_CR_KP_KI_ADJUST                 0                   // 0: CR Kp/Ki adjust OFF; 1: CR Kp/Ki adjust ON
#define VIF_CR_KP_KI_ADJUST_GEAR            2                    // number of gears for auto adjust, only 1, 2 is valid
#define VIF_CR_KP_KI_ADJUST_THR1            0x05                // [7:0] CR Kp/Ki adjust threshold 1
#define VIF_CR_KP_KI_ADJUST_THR2            0x10                // [7:0] CR Kp/Ki adjust threshold 2
#define VIF_CR_KP_KI_ADJUST_THR3            0x03                // [7:0] CR Kp/Ki adjust threshold 3

#define VIF_DYNAMIC_TOP_ADJUST              0                   // 0: Dynamic TOP adjust OFF; 1: Dynamic TOP adjust ON
#define VIF_DYNAMIC_TOP_MIN                 0                      // [3:0], minimum TOP for Dynamic TOP adjust (range 0~10)
#define VIF_AM_HUM_DETECTION                0                   // 0: not adjust AGC reference level; 1: adjust AGC reference level
#define VIF_CLAMPGAIN_CLAMP_SEL             1                   // 0: clamp select sync bottom; 1: clamp select porch
#define VIF_CLAMPGAIN_SYNCBOTT_REF          0x78                // [7:0], Porch or syncbottom reference level
#define VIF_CLAMPGAIN_SYNCHEIGHT_REF        0x68                // [7:0], Syncheight reference level
#define VIF_CLAMPGAIN_KC                    0x07                // [2:0], Clamp loop filter parameter
#define VIF_CLAMPGAIN_KG                    0x07                // [2:0], Gain loop filter parameter
#define VIF_CLAMPGAIN_CLAMP_OREN            1                   // 0: clamp overwrite disable; 1: clamp overwrite enable
#define VIF_CLAMPGAIN_GAIN_OREN             1                   // 0: gain overwrite disable; 1: gain overwrite enable
#define VIF_CLAMPGAIN_CLAMP_OV_NEGATIVE     0x750               // [10:0], Clamp overwrite value for negative modulation
#define VIF_CLAMPGAIN_CLAMP_OV_POSITIVE     0x5F0                   // [10:0], Clamp overwrite value for positive modulation
    
#if(VIF_SAW_ARCH == SAW_ARCH_EXTERNAL_SINGLE_SAW || VIF_SAW_ARCH == SAW_ARCH_SILICON_TUNER || VIF_SAW_ARCH == SAW_ARCH_SILICON_TUNER_VIF)
#define VIF_CLAMPGAIN_GAIN_OV_NEGATIVE  0x5B0               // [10:0], Gain overwrite value for negative modulation
#define VIF_CLAMPGAIN_GAIN_OV_POSITIVE  0x650               // [10:0], Gain overwrite value for positive modulation
#elif(VIF_SAW_ARCH == SAW_ARCH_DUAL_SAW)
#define VIF_CLAMPGAIN_GAIN_OV_NEGATIVE  0x5F0               // [10:0], Gain overwrite value for negative modulation
#define VIF_CLAMPGAIN_GAIN_OV_POSITIVE  0x4F0               // [10:0], Gain overwrite value for positive modulation
#elif(VIF_SAW_ARCH == SAW_ARCH_NO_SAW_DIF)
#define VIF_CLAMPGAIN_GAIN_OV_NEGATIVE  0x6B0               // [10:0], Gain overwrite value for negative modulation
#define VIF_CLAMPGAIN_GAIN_OV_POSITIVE  0x300               // [10:0], Gain overwrite value for positive modulation
#else
#define VIF_CLAMPGAIN_GAIN_OV_NEGATIVE  0x6B0               // [10:0], Gain overwrite value for negative modulation
#define VIF_CLAMPGAIN_GAIN_OV_POSITIVE  0x650               // [10:0], Gain overwrite value for positive modulation
#endif

#define VIF_CLAMPGAIN_CLAMP_MIN         0x80                // [7:0], Clamp minimum value
#define VIF_CLAMPGAIN_CLAMP_MAX         0x00                // [7:0], Clamp maximum value
#define VIF_CLAMPGAIN_GAIN_MIN          0x40                // [7:0], Gain minimum value
#define VIF_CLAMPGAIN_GAIN_MAX          0xFF                // [7:0], Gain maximum value
#define VIF_CLAMPGAIN_PORCH_CNT         0x0C8               // [8:0], Porch counter
#define VIF_PEAKING_FILTER_B_VHF_L      0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_B_VHF_L     1                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_B_VHF_L  2                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_GH_VHF_L     0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_GH_VHF_L    1                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_GH_VHF_L 2                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_DK_VHF_L     5                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_DK_VHF_L    0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_DK_VHF_L 0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_I_VHF_L      5                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_I_VHF_L     0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_I_VHF_L  0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_L_VHF_L      0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_L_VHF_L     0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_L_VHF_L  0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_LL_VHF_L     0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_LL_VHF_L    0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_LL_VHF_L 0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_MN_VHF_L     3                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_MN_VHF_L    0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_MN_VHF_L 6                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_B_VHF_H      0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_B_VHF_H     1                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_B_VHF_H  2                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_GH_VHF_H     0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_GH_VHF_H    1                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_GH_VHF_H 2                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_DK_VHF_H     5                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_DK_VHF_H    0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_DK_VHF_H 0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_I_VHF_H      5                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_I_VHF_H     0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_I_VHF_H  0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_L_VHF_H      0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_L_VHF_H     0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_L_VHF_H  0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_LL_VHF_H     0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_LL_VHF_H    0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_LL_VHF_H 0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_MN_VHF_H     3                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_MN_VHF_H    0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_MN_VHF_H 6                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_B_UHF        6                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_B_UHF       1                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_B_UHF    2                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_GH_UHF       6                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_GH_UHF      1                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_GH_UHF   2                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_DK_UHF       5                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_DK_UHF      0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_DK_UHF   0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_I_UHF        5                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_I_UHF       0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_I_UHF    0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_L_UHF        0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_L_UHF       0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_L_UHF    0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_LL_UHF       0                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_LL_UHF      0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_LL_UHF   0                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters
#define VIF_PEAKING_FILTER_MN_UHF       3                   // [7:0], 0: (null) peaking filter; 1: 1dB peaking filter; 2: 2dB peaking filter; 3: 3dB peaking filter; 4: 4dB peaking filter; 5: (VSB) 3dB peaking filter; 6: (VSB) 4dB peaking filter; 7: (VSB) 5dB peaking filter; 0x80: User define SOS21 filter
#define VIF_YC_DELAY_FILTER_MN_UHF      0                   // [7:0], 0: (null) Y/C delay filter; 1: (VSB) Y/C delay filter; 0x80: User define SOS22 filter
#define VIF_GROUP_DELAY_FILTER_MN_UHF   6                   // [7:0], 0: (null) group delay filter; 1: (VSB) group delay filter (LG); 2: (VSB) group delay filter (Philips); 3: (Low_R) group delay filter; 4: (Low_L) group delay filter; 5: (High_R) group delay filter; 6: (High_L) group delay filter; 0x80: User define SOS31/SOS32 filters

#if VIF_ASIA_SIGNAL_OPTION
#define CHINA_DESCRAMBLER_BOX           1                   // Use descrambler box in China; 0:not use; 1:IMAGE_IIR(0x99); 2:IMAGE_REJ1(0x8A); 3:ACI_REJ(0x89); 4:MIXER_OUT_I(0x86); 5:IMAGE_REJ2(0x8b)
#else
#define CHINA_DESCRAMBLER_BOX           0                   // Use descrambler box in China; 0:not use; 1:IMAGE_IIR(0x99); 2:IMAGE_REJ1(0x8A); 3:ACI_REJ(0x89); 4:MIXER_OUT_I(0x86); 5:IMAGE_REJ2(0x8b)
#endif

#define VIF_DELAY_REDUCE                0x00                // [7:0], [0]: reduce filter delay; [1]: reduce DAGC1 delay; [2]: reduce DAGC2 delay;
#define VIF_OVER_MODULATION             0                   // Support heavy 200% overmodulation when PD_mode = 1
#define VIF_OM_DETECTOR                 0                   //  Switch PD_mode from 0 to 1 to support >100%
#define VIF_ACI_DETECTOR                0                   // (Not used) 0:Not detect ACI ; 1:detect ACI
#define VIF_ACI_AGC_REF                 0x30                // (Not used) AGC reference level for ACI
#define VIF_ACIDET_THR1                 0x16                // (Not used) Upper threshold for N-1 ACI
#define VIF_ACIDET_THR2                 0x0A                // (Not used) Bottom threshold for N-1 ACI
#define VIF_ACIDET_THR3                 0x08                // (Not used) Upper threshold for N+1 ACI
#define VIF_ACIDET_THR4                 0x03                // (Not used) Bottom threshold for N+1 ACI
#define VIF_ADC_OVERFLOW_AGC_REF        0x20                // AGC reference level for serious ACI
#define VIF_SERIOUS_ACI_DETECTION		0                   // No SAW serious ACI detect enable
#define VIF_CHANEL_SCAN_AGC_REF         0x20                // No SAW AGC reference level for channel scan
#define VIF_SOS21_FILTER_C0             0x0000              // [10:0], User define coefficient of SOS21 filter
#define VIF_SOS21_FILTER_C1             0x0000              // [10:0], User define coefficient of SOS21 filter
#define VIF_SOS21_FILTER_C2             0x0000              // [10:0], User define coefficient of SOS21 filter
#define VIF_SOS21_FILTER_C3             0x0000              // [10:0], User define coefficient of SOS21 filter
#define VIF_SOS21_FILTER_C4             0x0200              // [10:0], User define coefficient of SOS21 filter
#define VIF_SOS22_FILTER_C0             0x0000              // [10:0], User define coefficient of SOS22 filter
#define VIF_SOS22_FILTER_C1             0x0000              // [10:0], User define coefficient of SOS22 filter
#define VIF_SOS22_FILTER_C2             0x0000              // [10:0], User define coefficient of SOS22 filter
#define VIF_SOS22_FILTER_C3             0x0000              // [10:0], User define coefficient of SOS22 filter
#define VIF_SOS22_FILTER_C4             0x0200              // [10:0], User define coefficient of SOS22 filter
#define VIF_SOS31_FILTER_C0             0x0000              // [10:0], User define coefficient of SOS31 filter
#define VIF_SOS31_FILTER_C1             0x0000              // [10:0], User define coefficient of SOS31 filter
#define VIF_SOS31_FILTER_C2             0x0000              // [10:0], User define coefficient of SOS31 filter
#define VIF_SOS31_FILTER_C3             0x0000              // [10:0], User define coefficient of SOS31 filter
#define VIF_SOS31_FILTER_C4             0x0200              // [10:0], User define coefficient of SOS31 filter
#define VIF_SOS32_FILTER_C0             0x0000              // [10:0], User define coefficient of SOS32 filter
#define VIF_SOS32_FILTER_C1             0x0000              // [10:0], User define coefficient of SOS32 filter
#define VIF_SOS32_FILTER_C2             0x0000              // [10:0], User define coefficient of SOS32 filter
#define VIF_SOS32_FILTER_C3             0x0000              // [10:0], User define coefficient of SOS32 filter
#define VIF_SOS32_FILTER_C4             0x0200               // [10:0], User define coefficient of SOS32 filter

#define VIF_CR_RATE_B                   0x00000000          // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_B                 0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_GH                  0x00000000          // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_GH                0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_DK                  0x00000000          // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_DK                0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_I                   0x00000000          // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_I                 0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_L                   0x00000000          // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_L                 0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_LL                  0x00000000          // [21:0], CR_RATE for 7.02 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_LL                0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_MN                  0x00000000          // [21:0], CR_RATE for 1.75 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_MN                0                   // Video carrier position; 0: high side; 1:low side



/*@ <Definitions> @*/
typedef struct
{
    float    power_db;
    MAPI_U8  agc_val;
}S_IFAGC_SSI;

typedef struct
{
    float    power_db;
    MAPI_U8  sar3_val;
}S_RFAGC_SSI;

typedef enum
{
    E_ISDBT_SSIMethod1 = 0x00,
    E_ISDBT_SSIMethod2,
    E_ISDBT_SSIMethod3,
}ISDBT_SSIMethod;

typedef enum
{
    E_ISDBT_HUMDET_EN = 0x00,   //0x00
    E_ISDBT_DCR_EN,             //0x01
    E_ISDBT_IIS_EN,             //0x02
    E_ISDBT_CCI_EN,             //0x03
    E_ISDBT_ACI_EN,             //0x04
    E_ISDBT_IQB_EN,             //0x05
    E_ISDBT_AUTO_IQ_SWAP_EN,    //0x06
    E_ISDBT_FIX_MODE_CP_EN,     //0x07
    E_ISDBT_FIX_TMCC_EN,        //0x08
    E_ISDBT_WINDOWING_EN,    	//0x09 
    E_ISDBT_FORCE_ACI_EN,       //0x0A
    E_ISDBT_RF_MAX,             //0x0B
    E_ISDBT_RSSI,               //0x0C
    E_ISDBT_ZIF,                //0x0D
    E_ISDBT_FS_L,               //0x0E
    E_ISDBT_FS_H,               //0x0F
    E_ISDBT_FIF_L,              //0x10
    E_ISDBT_FIF_H,              //0x11
    E_ISDBT_FC_L,               //0x12
    E_ISDBT_FC_H,               //0x13
    E_ISDBT_MODE,               //0x14
    E_ISDBT_CP,                 //0x15
    E_ISDBT_IQ_SWAP,            //0x16
    E_ISDBT_CCI,                //0x17
    E_ISDBT_IFAGC_REF,          //0x18
    E_ISDBT_IFAGC_REF_ACI,      //0x19
    E_ISDBT_IFAGC_REF_IIS,      //0x1A
    E_ISDBT_TS_SERIAL,          //0x1B
    E_ISDBT_TS_CLK_INV,         //0x1C
    E_ISDBT_TS_DATA_SWAP,       //0x1D
    E_ISDBT_FineSA_LeftA,       //0x1E
    
    ISDBT_PARAM_LEN,
} ISDBT_Param;

typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_DCR_LOCK,
    COFDM_AGC_LOCK,
    COFDM_MODE_DET,

} COFDM_LOCK_STATUS;

typedef enum
{
    ///Freq 3395 SECAM-L'
    E_IF_FREQ_3395, // SECAM-L'
    ///Freq 3800 PAL
    E_IF_FREQ_3800, // PAL
    ///Freq 3890 PAL
    E_IF_FREQ_3890, // PAL
    ///Freq 3950 only for PAL-I
    E_IF_FREQ_3950, // only for PAL-I
    ///Freq 4575 NTSC-M/N
    E_IF_FREQ_4575, // NTSC-M/N
    ///Freq 5875 NTSC-M/N
    E_IF_FREQ_5875, // NTSC-M/N
    ///EN_IF_FREQ_TYPE enum numbers
    E_IF_FREQ_NUMS
} EN_IF_FREQ_TYPE;

/*
typedef enum
{
  ///Layer A
    E_ISDBT_Layer_A = 0x00,
    ///Layer B
    E_ISDBT_Layer_B = 0x01,
    ///Layer C
    E_ISDBT_Layer_C = 0x02,
    ///Invalid
    E_ISDBT_Layer_INVALID,
}EN_ISDBT_Layer;
*/


/*@ <IncludeGuardEnd> @*/
#endif 
/*@ </IncludeGuardEnd> @*/
