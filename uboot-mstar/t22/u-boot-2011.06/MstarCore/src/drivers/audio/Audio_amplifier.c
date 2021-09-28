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

#include <drvGPIO.h>
#include <config.h>
#include <common.h>
#include <MsTypes.h>

#include <apiSWI2C.h>
#include <audio/Audio_amplifier.h>
#include <CusI2C.h>

#define msAPI_Timer_Delayms(x) udelay(1000*x)

#if (CONFIG_DIGITAL_TI5707) //(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5707)
#include "DevTAS5707.c"
#elif (CONFIG_DIGITAL_STA333)//(DIGITAL_I2S_SELECT==AUDIO_I2S_STA333)
#include "DevI2S_STA333.c"
#elif (CONFIG_DIGITAL_TI5708) //(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5708)
#include "Amplifier_TI5708.c"
#elif (CONFIG_DIGITAL_TI5805) //(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5805)
#include "Amplifier_TI5805.c"
#elif (CONFIG_DIGITAL_TI5711)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5711)
#include "Amplifier_TI5711.c"
#elif (CONFIG_DIGITAL_TI5727_HISENSE)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5727_HISENSE)
#include "Amplifier_TI5727_hisense.c"
#elif (CONFIG_DIGITAL_RT9113B)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5727_HISENSE)
#include "Amplifier_RT9113B.c"
#elif (CONFIG_DIGITAL_RT9114B)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5727_HISENSE)
#include "Amplifier_RT9114B.c"
#endif


#if 0// move all this to msI2C.c
#define MIIC_BUS_SYS    PAD_DDCR_CK,    PAD_DDCR_DA,    SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCA0  PAD_DDCA_CK,    PAD_DDCA_DA,    SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCD0  PAD_DDCDA_CK,   PAD_DDCDA_DA,   SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCD1  PAD_DDCDB_CK,   PAD_DDCDB_DA,   SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCD2  PAD_DDCDC_CK,   PAD_DDCDC_DA,   SWI2C_SPEED_MAPPING_100K

#define __I2C_BUS(scl, sda, dly)    scl, sda, dly
#define I2C_BUS( bus )      __I2C_BUS( bus )

typedef struct
{
    MS_U16 padSCL;
    MS_U16 padSDA;
    MS_U16 defDelay;
} I2C_Bus;

static MS_U8 u8BusSel = 0;

SWI2C_BusCfg g_I2CBus[] =
{
    { I2C_BUS( MIIC_BUS_SYS   ) },
	{ I2C_BUS( MIIC_BUS_DDCA0 ) },
	{ I2C_BUS( MIIC_BUS_DDCD0 ) },
	{ I2C_BUS( MIIC_BUS_DDCD1 ) },
	{ I2C_BUS( MIIC_BUS_DDCD2 ) },
};
#endif
void drvAudio_AMP_Init(void)
{

#if (CONFIG_DIGITAL_TI5707)
    //MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    drvTAS5707_SW_Init();
#elif (CONFIG_DIGITAL_STA333)
    //MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    Audio_Amplifier_ON();
    msAPI_Timer_Delayms(20);
    Audio_Amplifier_OFF();
    msAPI_Timer_Delayms(100);
    DevI2s_STA333_Init();
#elif (CONFIG_DIGITAL_TI5708)
    //MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    Amplifier_TI5708_Init();
    Amplifier_TI5708_VolumeForSource();
#elif (CONFIG_DIGITAL_TI5805)
    Amplifier_TI5805_Init();
    //Amplifier_TI5805_VolumeForSource();
#elif (CONFIG_DIGITAL_TI5711)
    msI2C_init();
    Amplifier_TI5711_Init();
    Amplifier_TI5711_Mute(0);
    Amplifier_TI5711_SWMute(0);
#elif (CONFIG_DIGITAL_TI5727_HISENSE)
   // MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    Amplifier_TI5727_hisense_Init();
    Amplifier_TI5727_hisense_Mute(0);
    Amplifier_TI5727_hisense_SWMute(0);
#elif (CONFIG_DIGITAL_RT9113B)
    Amplifier_RT9113_Init();
#elif (CONFIG_DIGITAL_RT9114B)
    Amplifier_RT9114_Init();
#endif
}
