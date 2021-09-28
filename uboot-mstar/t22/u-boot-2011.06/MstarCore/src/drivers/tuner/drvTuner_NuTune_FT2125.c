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



#ifndef drvTuner_NuTune_FT2125_C
#define drvTuner_NuTune_FT2125_C


#include <cofdm/COFDM_Demodulator.h>
#include <MsIRQ.h>
#include <MsOS.h>


void devDigitalTuner_Init()
{

}

void devTunerInit(void)
{
}


void devTunerSetFreqStep(FREQSTEP eFreqStep)
{

}

void devTunerSetPLLData(MS_U16 wPLLData, RFBAND eBand)
{
}


void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    MS_BOOL dwError = FALSE;
    MS_U8 config[4];

    MS_U16 divider = (Freq + 36.0) * 6.0;
    MS_U8 u8Control, u8BandSwitch;

    if (Freq <= 228.5)
    {
        u8Control = 0xBC;
        u8BandSwitch = (eBandWidth != E_RF_CH_BAND_8MHz) ? 0x02 : 0x12;
    }
    else if (Freq <= 500.0)
    {
        u8Control = 0xBC;
        u8BandSwitch = (eBandWidth != E_RF_CH_BAND_8MHz) ? 0x04 : 0x14;
    }
    else if (Freq <= 800.0)
    {
        u8Control = 0xF4;
        u8BandSwitch = (eBandWidth != E_RF_CH_BAND_8MHz) ? 0x04 : 0x14;
    }
    else
    {
        u8Control = 0xFC;
        u8BandSwitch = (eBandWidth != E_RF_CH_BAND_8MHz) ? 0x04 : 0x14;
    }

    config[0] = (divider >> 8) & 0xff;
    config[1] = divider & 0xff;
    config[2] = u8Control;
    config[3] = u8BandSwitch;
    //fprintf(stderr, "\033[1;36m[%s:%s][%d] Freq=%lf config=%x %x %x %x\033[0m\n",
    //            __FILE__, __FUNCTION__, __LINE__, Freq, config[0], config[1], config[2], config[3]);
    dwError = devCOFDM_PassThroughI2C_WriteBytes(_TUNER_I2C_ID, 0, NULL, 4, config);
    MsOS_DelayTask(100);
    //usleep(100*1000);

}


#endif
