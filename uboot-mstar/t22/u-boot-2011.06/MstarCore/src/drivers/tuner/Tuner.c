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
#include <common.h>
#include <MsTypes.h>
#include "Tuner.h"

#if (FRONTEND_TUNER_TYPE == NUTUNE_FT2125_TUNER)
#include "drvTuner_NuTune_FT2125.c"
#elif (FRONTEND_TUNER_TYPE == NUTUNE_FJ2207_TUNER)
#include "drvTuner_NuTune_FJ2207.c"
#include "FJ2207/tmbslNT220x/src/tmbslNT220x.c"
#include "FJ2207/tmbslNT220x/src/tmbslNT220xInstance.c"
#include "FJ2207/tmddNT220x/src/tmddNT220x.c"
#include "FJ2207/tmddNT220x/src/tmddNT220x_Advanced.c"
#include "FJ2207/tmddNT220x/src/tmddNT220xInstance.c"
//#elif (FRONTEND_TUNER_TYPE == NUTUNE_FK1602_TUNER)
//#include "MAXLINER_MxL_601.c"
#elif (FRONTEND_TUNER_TYPE == NXP_TD1611ALF_TUNER)
#include "drvTuner_NXP_TD1611ALF.c"
#elif (FRONTEND_TUNER_TYPE == SILAB_2158_TUNER)
#include "SILAB_2158_A20.c"
#include "SILAB_2158_A20/si2158_i2c_api.c"
#include "SILAB_2158_A20/Si2158_L1_API.c"
#include "SILAB_2158_A20/Si2158_L1_Commands.c"
#include "SILAB_2158_A20/Si2158_L1_Properties.c"
#include "SILAB_2158_A20/Si2158_L2_API.c"
#include "SILAB_2158_A20/Si2158_Properties_Strings.c"
#include "SILAB_2158_A20/silabs_L0_TV_Chassis.c"
#elif (FRONTEND_TUNER_TYPE == NXP_TDA18275_TUNER)
#include "NXP_TDA18275.c"
#elif (FRONTEND_TUNER_TYPE == NXP_TDA18250A_TUNER)
#include "tmbslTDA18250A/tmbslTDA18250A.c"
#include "tmbslTDA18250A/tmbslTDA18250A_Advanced.c"
#include "tmbslTDA18250A/drvTuner_TDA18250A.c"
#elif (FRONTEND_TUNER_TYPE == TUNER_MXL603)
#include "MxL603/MxL603_TunerApi.c"
#include "MxL603/MxL603_OEM_Drv.c"
#include "MxL603/MxL603_TunerCfg.c"
#include "MxL603/MxL603_TunerSpurTable.c"
#include "drvTuner_MxL603.c"
#elif (FRONTEND_TUNER_TYPE == TUNER_MXL608)
#include "MxL608/MxL608_TunerApi.c"
#include "MxL608/MxL608_OEM_Drv.c"
#include "MxL608/MxL608_TunerCfg.c"
#include "MxL608/MxL608_TunerSpurTable.c"
#include "drvTuner_MxL608.c"
#elif (FRONTEND_TUNER_TYPE == TUNER_AV2012)
#include "DVBS_Tuner/AV2012/drvTuner_AV2012.c"
#elif (FRONTEND_TUNER_TYPE == TUNER_RT710)
#include "DVBS_Tuner/RT710/drvTuner_RT710.c"
#elif (FRONTEND_TUNER_TYPE == TUNER_R850)
#include "drvTuner_R850.c"
#endif
