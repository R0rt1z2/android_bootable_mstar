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


/*@ <Include> @*/
/*
#include "device_tuner.h"
#include "device_tuner_NXP_TD1611ALF.h"
#include "mapi_i2c_devTable.h"

//#include "apiSWI2C.h"
#include "mapi_i2c.h"
#include "mapi_interface.h" */
#include <cofdm/COFDM_Demodulator.h>
#include <MsOS.h>


#define tuner_I2c 0xC0

#define NXP_TD1611ALF_INT_AGC 1
#define TUNER_IF_FREQ                   36160

#define DBG_TUNER(x)  // x

#if 0
U8 u8DeviceBusy=0;

MAPI_BOOL DeviceTunerCreate()
{
    mapi_tuner *pMapiTuner;

    pMapiTuner = mapi_interface::Get_mapi_tuner();

    pMapiTuner->stMapiTunerFun.Connect = Connect;

    pMapiTuner->stMapiTunerFun.Disconnect = Disconnect;

    pMapiTuner->stMapiTunerFun.ATV_SetTune = ATV_SetTune;

    pMapiTuner->stMapiTunerFun.DTV_SetTune = DTV_SetTune;

    pMapiTuner->stMapiTunerFun.ExtendCommand = ExtendCommand;

    return TRUE;

}

BOOL Connect(void)
{
    DBG_TUNER(printf("device_tuner_CD1616LF_GIGH_4: Connect\n"));
    if (u8DeviceBusy==1)
        return FALSE;

    u8DeviceBusy=1;

    return TRUE;
}

BOOL Disconnect()
{
    DBG_TUNER(printf("device_tuner_CD1616LF_GIGH_4: Disconnect\n"));

    u8DeviceBusy=0;
    return TRUE;
}

BOOL ATV_SetTune( U32 u32FreqKHz,RFBAND eBand, EN_TUNER_MODE eMode)
{
    u32FreqKHz = u32FreqKHz;
    eBand = eBand;
    eMode = eMode;

    return TRUE;
}

BOOL DTV_SetTune( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth, EN_TUNER_MODE eMode)
{
    U8 CONFIG[5];
    U8 CB0,CB1;
    BOOL dwError = FALSE;
    U8    ucfreq[2];
    U16    wFrequency;
    U8    ucDividerRatio = 6;
    eMode = eMode;
    // Use critical section to protect Tuner frequency setting
    //SysEnterCriticalSection();

    CB0 = 0xC8;
    CB1 = 0;
    //P5 P4
    if(eBandWidth == E_RF_CH_BAND_8MHz)
    {
          CB0 |= 0x01;
          CB1 |= 0x08;//according to -4 spec only.
          ucDividerRatio = 7;
    }
    else
    {
         CB0 |= 0x02; //R2R1R0
         ucDividerRatio = 6;
    }
    Freq*=1000;
    DBG_TUNER(printf("sttest tune %d ,%d \n",(int)Freq,(int)eBandWidth));

    wFrequency = (U16)(((Freq + TUNER_IF_FREQ) * ucDividerRatio + 500) / 1000);
    ucfreq[1] = (U8)((wFrequency >> 8) & 0x00FF);
    ucfreq[0] = (U8)(wFrequency & 0x00FF);

    //P3 P2 P1
    if ( Freq <= 174000 ) // VHF low
    {
        CB1 |= 0x00;
    }
    else if ( Freq  <= 470000 ) // VHF high
    {
        CB1 |= 0x02;
    }
    else  // UHF
    {
        CB1 |= 0x04;
    }

    //Charge Pump  CP2 CP1 CP0
    Freq += TUNER_IF_FREQ;
    if(Freq<=200000)
        CB1|= 0x60;// not used
    else if(Freq<=290000)
        CB1|= 0x60;
    else if(Freq<=420000)
        CB1|= 0xA0;
    else if(Freq<=480000)
        CB1|= 0xC0;
    else if(Freq<=620000)
        CB1|= 0x60;
    else if(Freq<=830000)
        CB1|= 0xA0;
    else
        CB1|= 0xE0;

    CONFIG[0]  = ucfreq[1];
    CONFIG[1]  = ucfreq[0];
    CONFIG[2]  = CB0;
    CONFIG[3]  = CB1;
    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(TUNER_DEV_I2C_1);
    dwError = iptr->WriteBytes(0, NULL, 4, CONFIG);

    if (dwError)
    {
        MsOS_DelayTaskUs(150);
        #if (NXP_TD1611ALF_INT_AGC)
           CONFIG[2]  = 0x8C;
        #else
           CONFIG[2]  = 0x8E;
        #endif
        dwError = iptr->WriteBytes(0, NULL, 4, CONFIG);

    #if 1
        if (dwError)
        {
            U8 n=0;
            while(n<20)
            {
                MsOS_DelayTaskUs(100);
                dwError=iptr->ReadBytes(0,NULL,1, CONFIG);
                n++;
                /*if((CONFIG[0]&0x08)==0x08)
                    printf("Internal RF AGC!\n");
                else
                   printf("external RF AGC\n");*/
                if((CONFIG[0]==0xFF)||(dwError==FALSE)) // not by pass
                {
                    dwError=FALSE;
                    break;
                }
                else if((CONFIG[0]&0x40)==0x40)
                {
                    //printf("Tuner locked %d!\n",n);
                    break;
                }
                else
                {
                    //printf("tuner lock failed\n");
                }

            }

            if((n>=20)||(dwError==FALSE))
            {
                printf("tuner config fail; status:%x\n",CONFIG[0]);
                dwError=FALSE;
            }
        }
    #else
        MsOS_DelayTaskUs(100);
    #endif

    }

    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    return (dwError);
}


BOOL ExtendCommand( U8 u8SubCmd, U32 u32Param1, U32 u32Param2, void* pvoidParam3)
{
    u8SubCmd=u8SubCmd;
    u32Param1=u32Param1;
    u32Param2=u32Param2;
    pvoidParam3=pvoidParam3;
    return TRUE;
}

#else

#include <Tuner.h>
#include <MsIRQ.h>
#include <MsOS.h>

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
#include <cofdm/MSB123x.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
#include <cofdm/MSB122x.h>
#endif


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

//-----------------------------------------------------
	MS_U8 CONFIG[5];
    MS_U8 CB0,CB1;
    MS_BOOL dwError = FALSE;
    MS_U8    ucfreq[2];
    MS_U16    wFrequency;
    MS_U8    ucDividerRatio = 6;
    // Use critical section to protect Tuner frequency setting
    //SysEnterCriticalSection();

    CB0 = 0xC8;
    CB1 = 0;
    //P5 P4
    if(eBandWidth == E_RF_CH_BAND_8MHz)
    {
          CB0 |= 0x01;
          CB1 |= 0x08;//according to -4 spec only.
          ucDividerRatio = 7;
    }
    else
    {
         CB0 |= 0x02; //R2R1R0
         ucDividerRatio = 6;
    }
    Freq*=1000;
    DBG_TUNER(printf("sttest tune %d ,%d \n",(int)Freq,(int)eBandWidth));

    wFrequency = (U16)(((Freq + TUNER_IF_FREQ) * ucDividerRatio + 500) / 1000);
    ucfreq[1] = (U8)((wFrequency >> 8) & 0x00FF);
    ucfreq[0] = (U8)(wFrequency & 0x00FF);

    //P3 P2 P1
    if ( Freq <= 174000 ) // VHF low
    {
        CB1 |= 0x00;
    }
    else if ( Freq  <= 470000 ) // VHF high
    {
        CB1 |= 0x02;
    }
    else  // UHF
    {
        CB1 |= 0x04;
    }

    //Charge Pump  CP2 CP1 CP0
    Freq += TUNER_IF_FREQ;
    if(Freq<=200000)
        CB1|= 0x60;// not used
    else if(Freq<=290000)
        CB1|= 0x60;
    else if(Freq<=420000)
        CB1|= 0xA0;
    else if(Freq<=480000)
        CB1|= 0xC0;
    else if(Freq<=620000)
        CB1|= 0x60;
    else if(Freq<=830000)
        CB1|= 0xA0;
    else
        CB1|= 0xE0;

    CONFIG[0]  = ucfreq[1];
    CONFIG[1]  = ucfreq[0];
    CONFIG[2]  = CB0;
    CONFIG[3]  = CB1;
 /*   mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(TUNER_DEV_I2C_1);
    dwError = iptr->WriteBytes(0, NULL, 4, CONFIG); */

 dwError = devCOFDM_PassThroughI2C_WriteBytes(tuner_I2c, 0, NULL, 4, CONFIG);

    if (dwError)
    {
        MsOS_DelayTaskUs(150);
        #if (NXP_TD1611ALF_INT_AGC)
           CONFIG[2]  = 0x8C;
        #else
           CONFIG[2]  = 0x8E;
        #endif
       // dwError = iptr->WriteBytes(0, NULL, 4, CONFIG);
		dwError = devCOFDM_PassThroughI2C_WriteBytes(tuner_I2c, 0, NULL, 4, CONFIG);

    #if 1
        if (dwError)
        {
            U8 n=0;
            while(n<200)
            {
                MsOS_DelayTaskUs(100);
               // dwError=iptr->ReadBytes(0,NULL,1, CONFIG);
				dwError = devCOFDM_PassThroughI2C_ReadBytes(tuner_I2c, 0, NULL, 1, CONFIG);

                n++;
                /*if((CONFIG[0]&0x08)==0x08)
                    printf("Internal RF AGC!\n");
                else
                   printf("external RF AGC\n");*/
                if((CONFIG[0]==0xFF)||(dwError==FALSE)) // not by pass
                {
                    dwError=FALSE;
                    break;
                }
                else if((CONFIG[0]&0x40)==0x40)
                {
                    //printf("Tuner locked %d!\n",n);
                    break;
                }
                else
                {
                    //printf("tuner lock failed\n");
                }

            }

            if((n>=200)||(dwError==FALSE))
            {
                printf("tuner config fail; status:%x\n",CONFIG[0]);
                dwError=FALSE;
            }
        }
    #else
        MsOS_DelayTaskUs(100);
    #endif

    }

    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    // return (dwError);

}


#endif

