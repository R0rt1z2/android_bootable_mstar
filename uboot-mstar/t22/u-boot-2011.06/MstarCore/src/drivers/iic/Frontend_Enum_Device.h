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

//------------------------------MS_DVB_TYPE_SEL-------------------------------
//
#define BROADCAST_TYPE_NOT_SEETING  0
#define DVBC                        1
#define DVBT                        2
#define DVBT2                       3
#define ISDBT                       4
#define DVBS                        5
#define ATSC                        6
#define J83B                        7
#define DVB_NULL                    255
#define INTERNAL_DVBT               8
#define INTERNAL_ISDBT              9

//------------------------------------------------------------------------------------
//  FRONTEND_DEMOD_TYPE
//------------------------------------------------------------------------------------

//--------------- external demods start ---------------
#define DEMOD_MSB1236C              0
#define DEMOD_MSB1233C              1
#define DEMOD_MSB123X               2
#define DEMOD_MSB131X               3             //DVBS
#define DEMOD_MSB1400               4             //external ISDBT


#define DEMOD_MSB1237               126             //Kappa external ATSC
#define DEMOD_AVL6211               134             //DVBS

#define DIBCOM_DIB8096_DEMOD        136             //ISDBT
//others
#define DEMOD_ZARLINK_ZL10353       127             // DVB COFDM Demodulator
#define DEMOD_MSDVBC                128             // DVBC Demodulator
#define DEMOD_AFA_9013              129
#define DEMOD_MSB1228               130
#define DEMOD_MSB1210               131
#define DEMOD_MSB122x               132             //DVBT Demodulator
#define DEMOD_TC90517               133
//--------------- external demods end ---------------

//--------------- internal demods start---------------
//kenya
#define DEMOD_MSKENYA_DVBC          237              //Kenya internal DVBC
//keltic
#define DEMOD_MSKELTIC_ATSC         238              //Keltic internal ATSC
#define DEMOD_MSKELTIC_DVBS         239              //Keltic internal DVBC
#define DEMOD_MSKELTIC_DVBC         240              //Keltic internal DVBC
//kappa
#define DEMOD_MSKAPPA_ISDBT         241              //Kappa internal ISDBT
#define DEMOD_MSATSC_C              242              //internal ATSC-Cable demod              // Kapa internal demod
#define DEMOD_MSKAPPA_DVBT          243              //Kappa internal DVBT
#define DEMOD_MSKAPPA_DVBC          244              //Kappa internal DVBC
//kaiser
#define DEMOD_MSKAISER_DVBT         245              //Kaiser internal DVBT
#define DEMOD_MSKAISER_DVBC         246              //Kaiser internal DVBC
//kaiserin
#define DEMOD_MSKAISERIN_DVBT       247              //Kaiserin internal DVBT
#define DEMOD_MSKAISERIN_DVBC       248              //Kaiserin internal DVBC
//krnous
#define DEMOD_MSKRONUS_DVBT         249              //Kronus internal DVBT
#define DEMOD_MSKRONUS_DVBC         250              //Kronus internal DVBC
//Kris
#define DEMOD_MSKRIS_DVBS           251             //Kris internal DVBS
//--------------- end internal demods ---------------

//not exist
#define DEMOD_NULL                  253
#define DEMOD_PLAYCARD              254              // DVB Fake Demodulator
#define DEMOD_UNKNWON               255

//------------------------------------------------------------------------------------
//FRONTEND_TUNER_TYPE
//------------------------------------------------------------------------------------
#define TUNER_MXL603	                    0
#define TUNER_TDA18250A                     1
#define TUNER_TDA18260                      2
#define TUNER_PHILIPS_TDA18250HN            3
#define TUNER_AV2011                        4
#define TUNER_AV2012                        5
#define TUNER_AV2028                        6
#define TUNER_AV2018                        7
#define TUNER_TDA18250B                     8

#define TUNER_PHILIPS_TDA1316               128           // DVB, PAL RF Tuner
#define TUNER_THOMSON_FE6640                129           // DVB, PAL RF Tuner
#define TUNER_PHILIPS_FQD1216               130           // DVB, PAL RF Tuner
#define TUNER_PHILIPS_TDA6034               131           // DVB RF Tuner
#define TUNER_PHILIPS_CD1316                132           // DVB Tuner (external RFAGC)
#define TUNER_THOMSON_DCT70707              133               // DVB Tuner (internal RFAGC)
#define TUNER_PHILIPS_CD1616LF_GIGH_4       134               // DVB Tuner (internal RFAGC)
#define TUNER_PHILIPS_CD1616LF_GIH_4        135               // DVB Tuner (external RFAGC)
#define TUNER_MXL_MXL5007                   136
#define TUNER_LG_TDTM_G352                  137
#define TUNER_LG_TDTC_G311D                 138
#define TUNER_PLAYCARD                      139
#define TUNER_NXP_TD1611ALF                 140

#define TUNER_NUTUNE_FJ2217                 141
#define TUNER_NUTUNE_FQD1136                142
#define TUNER_MXL201RF                      143
#define TUNER_NXP_TDA18219                  144

#define TUNER_MXL608	                     145

#define TUNER_MS221                         146
#define TUNER_MS222                         147

//DVBS Tuner
#define TUNER_SHARP_BS2S7HZ6306             148

#define TUNER_TBWU18120IN                   149
#define TUNER_SHARP_BS2S7VZ7801             150
#define TUNER_TDA20152                      151

//ISDBT
#define TUNER_DIBCOM_DIB8096                152

#define TUNER_R850                          153

//NULL Tuner
#define TUNER_NULL                          255

//---------------------------  DISH_TYPE-------------------
#define DISH_NONE                            0
#define DISH_DEMOD                           1
#define DISH_A8293                           2
#define DISH_A8296                           3


//------------------------------------------------------------------------------------
// FRONTEND_IF_DEMODE_TYPE
//------------------------------------------------------------------------------------
#define PHILIPS_TDA9886                 0   // DVB Domodulator for Analog AV
#define PHILIPS_TDA9881                 1
#define RENESAS_R2A10406NP              2
#define MSTAR_VIF                       3
#define MSTAR_VIF_MSB1210               4    // MSB1210 DVB-T + VIF Demodulator
#define TOSHIBA_TC90512XBG_IF           5
#define MSTAR_INTERN_VIF                6
#define MSTAR_VIF_MSB1228               7

//------------------------------------------------------------------------------------
// DMX_DEMOD_INPUT TYPE
//------------------------------------------------------------------------------------
#define DMX_INPUT_DEMOD             0              ///< DMX input from internal demod 0
#define DMX_INPUT_DEMOD1            1              ///< DMX input from internal demod 1
#define DMX_INPUT_MEM               2              ///< DMX input from memory
#define DMX_INPUT_EXT_INPUT0        3              ///< DMX input from external input 0
#define DMX_INPUT_EXT_INPUT1        4              ///< DMX input from external input 1
#define DMX_INPUT_EXT_INPUT2        5              ///< DMX input from external input 2
#define DMX_INPUT_EXT_INPUT3        6              ///< DMX input from external input 3

#include "Frontend_Detect_List.h"


//------------------------------ DTV Standar Type -----------------------------------
#ifndef MS_DVB_TYPE_SEL1
#define MS_DVB_TYPE_SEL1 DVB_NULL
#endif
#ifndef MS_DVB_TYPE_SEL2
#define MS_DVB_TYPE_SEL2 DVB_NULL
#endif
#ifndef MS_DVB_TYPE_SEL3
#define MS_DVB_TYPE_SEL3 DVB_NULL
#endif

//#if((MS_DVB_TYPE_SEL == DVBS) || (MS_DVB_TYPE_SEL1 == DVBS) || (MS_DVB_TYPE_SEL2 == DVBS) || (MS_DVB_TYPE_SEL3 == DVBS))
#if 1
#define MS_DVBS_INUSE 1
#else
#define MS_DVBS_INUSE 0
#endif

#if((MS_DVB_TYPE_SEL == DVBC) || (MS_DVB_TYPE_SEL1 == DVBC) || (MS_DVB_TYPE_SEL2 == DVBC) || (MS_DVB_TYPE_SEL3 == DVBC))
#define MS_DVBC_INUSE 1
#else
#define MS_DVBC_INUSE 0
#endif

#if((MS_DVB_TYPE_SEL == DVBT2) || (MS_DVB_TYPE_SEL1 == DVBT2) || (MS_DVB_TYPE_SEL2 == DVBT2) || (MS_DVB_TYPE_SEL3 == DVBT2))
#define MS_DVBT2_INUSE 1
#else
#define MS_DVBT2_INUSE 0
#endif

#if((MS_DVB_TYPE_SEL == J83B) || (MS_DVB_TYPE_SEL1 == J83B) || (MS_DVB_TYPE_SEL2 == J83B) || (MS_DVB_TYPE_SEL3 == J83B))
#define MS_J83B_INUSE 1
#else
#define MS_J83B_INUSE 0
#endif

//------------------------------ Demod Usage ----------------------------------
#if TUNER_14TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 15
#elif TUNER_13TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 14
#elif TUNER_12TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 13
#elif TUNER_11TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 12
#elif TUNER_10TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 11
#elif TUNER_09TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 10
#elif TUNER_08TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  9
#elif TUNER_07TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  8
#elif TUNER_06TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  7
#elif TUNER_05TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  6
#elif TUNER_04TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  5
#elif TUNER_03RD_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  4
#elif TUNER_02ND_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  3
#elif TUNER_01ST_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  2
#elif TUNER_00TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  1
#else
#define TUNER_SCAN_NUMBER  0
#endif

#ifndef FRONTEND_DEMOD_TYPE1
#define FRONTEND_DEMOD_TYPE1 DEMOD_NULL
#endif
#ifndef FRONTEND_DEMOD_TYPE2
#define FRONTEND_DEMOD_TYPE2 DEMOD_NULL
#endif
#ifndef FRONTEND_DEMOD_TYPE3
#define FRONTEND_DEMOD_TYPE3 DEMOD_NULL
#endif

#define IF_THIS_DEMOD_INUSE(x) ( ((DEMOD_00TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 0) )||\
                                 ((DEMOD_01ST_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 1) ) ||\
                                 ((DEMOD_02ND_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 2) ) ||\
                                 ((DEMOD_03RD_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 3) ) ||\
                                 ((DEMOD_04TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 4) ) ||\
                                 ((DEMOD_05TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 5) ) ||\
                                 ((DEMOD_06TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 6) ) ||\
                                 ((DEMOD_07TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 7) ) ||\
                                 ((DEMOD_08TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 8) ) ||\
                                 ((DEMOD_09TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 9) ) ||\
                                 ((DEMOD_10TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 10) ) ||\
                                 ((DEMOD_11TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 11) ) ||\
                                 ((DEMOD_12TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 12) ) ||\
                                 ((DEMOD_13TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 13) ) ||\
                                 ((DEMOD_14TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 14) ) )

//------------------------------ Tuner Usage ----------------------------------
#if DEMOD_14TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 15
#elif DEMOD_13TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 14
#elif DEMOD_12TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 13
#elif DEMOD_11TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 12
#elif DEMOD_10TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 11
#elif DEMOD_09TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 10
#elif DEMOD_08TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  9
#elif DEMOD_07TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  8
#elif DEMOD_06TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  7
#elif DEMOD_05TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  6
#elif DEMOD_04TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  5
#elif DEMOD_03RD_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  4
#elif DEMOD_02ND_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  3
#elif DEMOD_01ST_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  2
#elif DEMOD_00TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  1
#else
#define DEMOD_SCAN_NUMBER  0
#endif


#ifndef FRONTEND_TUNER_TYPE1
#define FRONTEND_TUNER_TYPE1 TUNER_NULL
#endif
#ifndef FRONTEND_TUNER_TYPE2
#define FRONTEND_TUNER_TYPE2 TUNER_NULL
#endif
#ifndef FRONTEND_TUNER_TYPE3
#define FRONTEND_TUNER_TYPE3 TUNER_NULL
#endif

//#define IF_THIS_TUNER_INUSE(x) 1
#define IF_THIS_TUNER_INUSE(x) (((TUNER_00TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 0)) ||\
                                ((TUNER_01ST_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 1)) ||\
                                ((TUNER_02ND_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 2)) ||\
                                ((TUNER_03RD_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 3)) ||\
                                ((TUNER_04TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 4)) ||\
                                ((TUNER_05TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 5)) ||\
                                ((TUNER_06TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 6)) ||\
                                ((TUNER_07TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 7)) ||\
                                ((TUNER_08TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 8)) ||\
                                ((TUNER_09TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 9)) ||\
                                ((TUNER_10TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 10)) ||\
                                ((TUNER_11TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 11)) ||\
                                ((TUNER_12TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 12)) ||\
                                ((TUNER_13TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 13)) ||\
                                ((TUNER_14TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 14)) )

