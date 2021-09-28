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
#ifndef device_tuner_NXP_TDA18275_H
#define device_tuner_NXP_TDA18275_H
/*@ </IncludeGuard> @*/

#ifdef __TUNER_H__
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifdef VIF_TUNER_TYPE
#undef VIF_TUNER_TYPE
#endif
#ifdef VIF_CR_RATE_B
#undef VIF_CR_RATE_B
#endif
#ifdef VIF_CR_INVERT_B
#undef VIF_CR_INVERT_B
#endif
#ifdef VIF_CR_RATE_GH
#undef VIF_CR_RATE_GH
#endif
#ifdef VIF_CR_INVERT_GH
#undef VIF_CR_INVERT_GH
#endif
#ifdef VIF_CR_RATE_DK
#undef VIF_CR_RATE_DK
#endif
#ifdef VIF_CR_INVERT_DK
#undef VIF_CR_INVERT_DK
#endif
#ifdef VIF_CR_RATE_I
#undef VIF_CR_RATE_I
#endif
#ifdef VIF_CR_INVERT_I
#undef VIF_CR_INVERT_I
#endif
#ifdef VIF_CR_RATE_L
#undef VIF_CR_RATE_L
#endif
#ifdef VIF_CR_INVERT_L
#undef VIF_CR_INVERT_L
#endif
#ifdef VIF_CR_RATE_LL
#undef VIF_CR_RATE_LL
#endif
#ifdef VIF_CR_INVERT_LL
#undef VIF_CR_INVERT_LL
#endif
#ifdef VIF_CR_RATE_MN
#undef VIF_CR_RATE_MN
#endif
#ifdef VIF_CR_INVERT_MN
#undef VIF_CR_INVERT_MN
#endif

#ifdef  D_DMD_IF_DVBT_8M
#undef D_DMD_IF_DVBT_8M
#endif
#ifdef  D_DMD_IF_DVBT_7M
#undef D_DMD_IF_DVBT_7M
#endif
#ifdef  D_DMD_IF_DVBT_6M
#undef D_DMD_IF_DVBT_6M
#endif
#ifdef  D_DMD_IF_DVBC_8M
#undef D_DMD_IF_DVBC_8M
#endif
#ifdef  D_DMD_IF_DVBC_7M
#undef D_DMD_IF_DVBC_7M
#endif
#ifdef  D_DMD_IF_DVBC_6M
#undef D_DMD_IF_DVBC_6M
#endif
#ifdef  D_DMD_IF_DVBT2_8M
#undef D_DMD_IF_DVBT2_8M
#endif
#ifdef  D_DMD_IF_DVBT2_7M
#undef D_DMD_IF_DVBT2_7M
#endif
#ifdef  D_DMD_IF_DVBT2_6M
#undef D_DMD_IF_DVBT2_6M
#endif
#ifdef  D_DMD_TUNER_IQSWAP
#undef D_DMD_TUNER_IQSWAP
#endif

// tmbslTDA18275_InstanceCustom.h
// Digital IF
#define QAM_6M     4000L
#define QAM_8M     5000L
#define ATSC_6M    5250L
#define ISDBT_6M   3683L
#define DVBT_1p7M   850L
#define DVBT_6M    4000L
#define DVBT_7M    4500L
#define DVBT_8M    5000L
#define DVBT_10M   6000L
#define DMBT_8M    5000L

// Analog picture carrier
#define ATV_VC_MN  5400L
#define ATV_VC_B   6250L
#define ATV_VC_GH  6750L
#define ATV_VC_I   7250L
#define ATV_VC_DK  6850L
#define ATV_VC_L   6750L
#define ATV_VC_LL  1250L
// [21:0], CR_RATE for Video_Carrier_BBB KHz, HEX((Video_Carrier_BBB/43200.0) * (2^22))
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner
#define VIF_CR_RATE_B                   (MS_U32)(ATV_VC_B*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_B                 0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_GH                  (MS_U32)(ATV_VC_GH*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_GH                0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_DK                  (MS_U32)(ATV_VC_DK*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_DK                0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_I                   (MS_U32)(ATV_VC_I*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_I                 0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_L                   (MS_U32)(ATV_VC_L*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_L                 0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_LL                  (MS_U32)(ATV_VC_LL*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_LL                1                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_MN                  (MS_U32)(ATV_VC_MN*4194304.0/43200.0+0.5)
#define VIF_CR_INVERT_MN                0                   // Video carrier position; 0: high side; 1:low side


#define D_DMD_IF_DVBT_8M  DVBT_8M
#define D_DMD_IF_DVBT_7M  DVBT_7M
#define D_DMD_IF_DVBT_6M  DVBT_6M
#define D_DMD_IF_DVBC_8M  QAM_8M
#define D_DMD_IF_DVBC_7M  QAM_8M
#define D_DMD_IF_DVBC_6M  QAM_8M
#define D_DMD_IF_DVBT2_8M  DVBT_8M
#define D_DMD_IF_DVBT2_7M  DVBT_7M
#define D_DMD_IF_DVBT2_6M  DVBT_6M
#define D_DMD_TUNER_IQSWAP 1


#define VHF_LOWMIN_FREQ        46250L    // KHz
#define VHF_LOWMAX_FREQ        142000L    // KHz
#define VHF_HIGHMIN_FREQ    149000L    // KHz
#define VHF_HIGHMAX_FREQ    426000L    // KHz
#define UHF_MIN_FREQ_UK        470000L //470000L    // KHz for UK
#define UHF_MIN_FREQ        434000L    // KHz
#define UHF_MAX_FREQ        865250L    // KHz
#define TUNER_IF_FREQ_KHz            38900L    // KHz
#define TUNER_L_PRIME_IF_FREQ_KHz    33950L    // KHz

/// the tuner mode
typedef enum
{
    /// DTV, DVBT
    E_TUNER_DTV_DVB_T_MODE = 0x00,
    /// DTV, DVBC
    E_TUNER_DTV_DVB_C_MODE ,
    /// DTV, DVBS
    E_TUNER_DTV_DVB_S_MODE ,
    /// DTV, DTMB
    E_TUNER_DTV_DTMB_MODE ,
    /// DTV, ATSC
    E_TUNER_DTV_ATSC_MODE ,
    /// ATV, PAL
    E_TUNER_ATV_PAL_MODE ,
    /// ATV, SECAM-L'
    E_TUNER_ATV_SECAM_L_PRIME_MODE ,
    /// ATV, NTSC
    E_TUNER_ATV_NTSC_MODE ,
    /// DTV, ISDB
    E_TUNER_DTV_ISDB_MODE ,
    /// DTV, DVBT2
    E_TUNER_DTV_DVB_T2_MODE,
    /// ATV, SECAM-L
    E_TUNER_ATV_SECAM_L_MODE ,
    /// ATV, SECAM-L
    E_TUNER_DTV_QAM_US_MODE ,
    /// invalid
    E_TUNER_INVALID
} EN_TUNER_MODE;

/// Audio SIF Standard Type
typedef enum
{
    ///< Audio standard BG
    E_AUDIOSTANDARD_BG_                     = 0x00,
    ///< Audio standard BG A2
    E_AUDIOSTANDARD_BG_A2_                  = 0x01,
    ///< Audio standard BG NICAM
    E_AUDIOSTANDARD_BG_NICAM_               = 0x02,
    ///< Audio standard I
    E_AUDIOSTANDARD_I_                      = 0x03,
    ///< Audio standard DK
    E_AUDIOSTANDARD_DK_                     = 0x04,
    ///< Audio standard DK1 A2
    E_AUDIOSTANDARD_DK1_A2_                 = 0x05,
    ///< Audio standard DK2 A2
    E_AUDIOSTANDARD_DK2_A2_                 = 0x06,
    ///< Audio standard DK3 A2
    E_AUDIOSTANDARD_DK3_A2_                 = 0x07,
    ///< Audio standard DK NICAM
    E_AUDIOSTANDARD_DK_NICAM_               = 0x08,
    ///< Audio standard L
    E_AUDIOSTANDARD_L_                      = 0x09,
    ///< Audio standard M
    E_AUDIOSTANDARD_M_                      = 0x0A,
    ///< Audio standard M BTSC
    E_AUDIOSTANDARD_M_BTSC_                 = 0x0B,
    ///< Audio standard M A2
    E_AUDIOSTANDARD_M_A2_                   = 0x0C,
    ///< Audio standard M EIA J
    E_AUDIOSTANDARD_M_EIA_J_                = 0x0D,
    ///< Not Audio standard
    E_AUDIOSTANDARD_NOTSTANDARD_            = 0x0F
} AUDIOSTANDARD_TYPE_;

/*@ <Definitions> @*/

INTERFACE void devTunerInit(void);
INTERFACE void devDigitalTuner_Init(void);
INTERFACE void devTunerSetFreq(MS_U32 u32Freq);
INTERFACE void devDigitalTuner_SetFreq( double Freq,  RF_CHANNEL_BANDWIDTH eBandWidth);
INTERFACE MS_BOOL devTuner_Std_set(EN_TUNER_MODE eMode, AUDIOSTANDARD_TYPE_ OtherMode);
INTERFACE MS_BOOL mdev_GetSignalStrength(float* f_strength);

#undef INTERFACE

#endif // device_tuner_NuTune_FK16021_H
