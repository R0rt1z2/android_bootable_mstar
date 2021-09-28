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

/* READ ME
Keys:
1. The detect list implies the demod/tuner checking order   
   You can exchange the devices' order in below list to change the detecting orders
   EX:  if #define TUNER 04TH_SCAN_DEVICE TUNER AV2012
        => It implies that the TUNER AV2012 will 4th be checked whether is onto the PCB

2. If a demod/tuner is not in the detection list (full as DEMOD_NULL/TUNER_NULL),
   the demod's/tuner's driver will be compiled as empty codes 

3. The demod and tuner devices' name can be referenced in    
   \\THEALE\Uranus\DDI_Misc\cus_mstar\project\mstar_demo\Frontend_Enum_Device.h

Note: AV2018,AV2028 can not be distinguished by SW
      So it does not work if av2018 and av2028 are put into the list at the same time
*/

//------------------------------------------------------------------------------------
//  Frontend external demod detection list
//------------------------------------------------------------------------------------
#define DEMOD_00TH_SCAN_DEVICE DEMOD_MSB1236C
#define DEMOD_01ST_SCAN_DEVICE DEMOD_MSB123X
#define DEMOD_02ND_SCAN_DEVICE DEMOD_MSB131X
#define DEMOD_03RD_SCAN_DEVICE DEMOD_NULL
#define DEMOD_04TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_05TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_06TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_07TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_08TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_09TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_10TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_11TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_12TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_13TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_14TH_SCAN_DEVICE DEMOD_NULL

/* Supported external demod devices
DEMOD_MSB1236C          DVBT/DVBT2 (Share driver with DEMOD_MSB1233C)
DEMOD_MSB123X           DVBT/DVBT2
DEMOD_MSB131X           DVBS/DVBS2
DEMOD_MSB1237           ATSC
*/

//------------------------------------------------------------------------------------
//  Frontend tuner detection list
//------------------------------------------------------------------------------------
#define TUNER_00TH_SCAN_DEVICE TUNER_MXL603
#define TUNER_01ST_SCAN_DEVICE TUNER_TDA18250A //if crystal is 16,set this to TUNER_TDA18250B
#define TUNER_02ND_SCAN_DEVICE TUNER_PHILIPS_TDA18250HN
#define TUNER_03RD_SCAN_DEVICE TUNER_TDA18260
#define TUNER_04TH_SCAN_DEVICE TUNER_AV2012
#define TUNER_05TH_SCAN_DEVICE TUNER_NULL
#define TUNER_06TH_SCAN_DEVICE TUNER_NULL
#define TUNER_07TH_SCAN_DEVICE TUNER_NULL
#define TUNER_08TH_SCAN_DEVICE TUNER_NULL
#define TUNER_09TH_SCAN_DEVICE TUNER_NULL
#define TUNER_10TH_SCAN_DEVICE TUNER_NULL
#define TUNER_11TH_SCAN_DEVICE TUNER_NULL
#define TUNER_12TH_SCAN_DEVICE TUNER_NULL
#define TUNER_13TH_SCAN_DEVICE TUNER_NULL
#define TUNER_14TH_SCAN_DEVICE TUNER_NULL

/* Supported external demod devices
TUNER_MXL603              DVBC/DVBT/DVBT2/ATSC/ISDBT
TUNER_TDA18250A           DVBC                       (Crystal_27, same ID with TUNER_TDA18250B)
TUNER_TDA18250B           DVBC                       (Crystal_16, same ID with TUNER_TDA18250A)      
TUNER_PHILIPS_TDA18250HN  DVBC
TUNER_TDA18260            DVBC
TUNER_AV2012              DVBS/DVBS2                 (Share driver with AV2011)
TUNER_AV2018              DVBS/DVBS2                 (Same ID with TUNER_AV2028)      
TUNER_AV2028              DVBS                       (Same ID with TUNER_AV2018) 
*/

