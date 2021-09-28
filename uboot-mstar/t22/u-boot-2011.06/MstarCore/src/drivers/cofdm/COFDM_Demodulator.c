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
#include "COFDM_Demodulator.h"
#include <MsTypes.h>
#include <apiSWI2C.h>
#include <drvHWI2C.h>
#include <drvGPIO.h>


#if ((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
#include <cofdm/MSB123x.c>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)
#include <cofdm/MSB123xc.c>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD)
#include <cofdm/MSB1400.c>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
#include <cofdm/MSB122x.c>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD)
#include <cofdm/MSB1233C.c>
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
#if defined(CONFIG_KENYA)
#include <cofdm/drvDemodKenyaDVBC.c>
#elif defined(CONFIG_KERES) || defined(CONFIG_KIRIN) || defined(CONFIG_K1C)
#include <cofdm/drvDemodKeresDVBC.c>
#elif defined(CONFIG_KRATOS) || defined(CONFIG_KAYLA)
#include <cofdm/drvDemodMstarDVBC.c>
#endif
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD)
#if defined(CONFIG_K5TN) || defined(CONFIG_K5AP)
#include <cofdm/drvDemodMstarDVBT2.c>
#endif
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD)
#if defined(CONFIG_KRIS)
#include <cofdm/drvDemodKrisDVBS.c>
#elif defined(CONFIG_KRATOS)|| defined(CONFIG_KAYLA)
#include <cofdm/drvDemodKratosDVBS.c>
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)
#include <cofdm/device_demodulator_msb1238.c>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)
#include <cofdm/device_demodulator_msb124x_dvbt2.c>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)
#include <cofdm/device_demodulator_msb1245_dvbs2.c>
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define COFDM_DBG(x)  //x

extern unsigned char bCardAInside;
//extern void msAPI_Timer_Delayms(unsigned long u32DelayTime); //unit = ms
//jerry-cp
#define msAPI_Timer_Delayms(x) udelay(1000*x)

int devCOFDM_Init(void)
{
    MS_BOOL bDemodInit = TRUE;
// init demod
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
        || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    Connect(DTV_Get_Demod_TYPE());
    bDemodInit = (MS_BOOL) Power_On_Initialization();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    bDemodInit = (MS_BOOL) Power_On_Initialization();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD) || (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD) || (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD))
    bDemodInit = MDrv_Demod_Init();
#endif
    if(!bDemodInit)
    {
        return -1;
    }
// init tuner
#if ((FRONTEND_DEMOD_TYPE != MSTAR_MSB1238_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB124x_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBC_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBT_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBT2_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBS_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB1245_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB123x_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB123xc_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB1400_DEMOD))//MDrv_Demod_Init has included tuner init
    devDigitalTuner_Init();
#elif(FRONTEND_TUNER_TYPE == TUNER_MXL608 || FRONTEND_TUNER_TYPE == TUNER_R850)
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD))
    IIC_Bypass_Mode(1);
#else
    MDrv_Demod_I2C_ByPass(TRUE);
#endif
    if (!MDrv_Tuner_Init())
    {
        printf("TUNER INIT FAIL\n");
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD))
        IIC_Bypass_Mode(0);
#else
        MDrv_Demod_I2C_ByPass(FALSE);
#endif
        return -1;
    }
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)  || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD))
    IIC_Bypass_Mode(0);
#else
    MDrv_Demod_I2C_ByPass(FALSE);
#endif
#elif((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_RT710))
    if(!MDrv_Tuner_Init())
    {
        printf("TUNER INIT FAIL\n");
        return -1;
    }
#endif
    return 0;
}

void devCOFDM_ControlPowerOnOff(BOOLEAN bPowerOn)
{

#if (FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF || FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF_MSB1210 || FRONTEND_IF_DEMODE_TYPE == MSTAR_INTERN_VIF)
    if (bPowerOn == ENABLE) // DTV mode
    {
        EXT_RF_AGC_OFF();
        //printf("KKK: (Temp)EXT_RF_AGC_OFF...for DTV\n");
    }
    else
    {
        EXT_RF_AGC_ON();
        //printf("KKK: (Temp)EXT_RF_AGC_On...for ATV\n");
    }
#endif




#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    ZL10353_Power_ON_OFF(bPowerOn);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    MSB1200_Power_ON_OFF(bPowerOn);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    MSB1210_Power_ON_OFF(bPowerOn);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    //printf("\ndevCOFDM_ControlPowerOnOff %d\n",(int) bPowerOn);
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    bPowerOn = bPowerOn;
//    printf("\nThis is MTK solution\n");
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    CE6355_InitRegister(FALSE);
    if(FALSE == bPowerOn)
        CE6355_MCLOKEnable(bPowerOn);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    //TC90512XBG_Power_ON_OFF(bPowerOn);
    bPowerOn = bPowerOn;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    //TC90517_Power_ON_OFF(bPowerOn);
    bPowerOn = bPowerOn;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    bPowerOn = bPowerOn;
//    printf("\nThis is MONTAGE solution\n");
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    bPowerOn = bPowerOn;
//    printf("\nThis is MONTAGE solution\n");
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    bPowerOn = bPowerOn;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    INTERN_DVBT_Power_ON_OFF(bPowerOn);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD))
    MDrv_Demod_PowerOnOff(bPowerOn);
#endif

}

FUNCTION_RESULT devCOFDM_PowerSave(void)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    if (MSB1200_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    if (MSB1210_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    if (INTERN_DVBT_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    return E_RESULT_SUCCESS;
#endif
}

void devCOFDM_TunerProgramming(U8 *cTunerData)
{
    #if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
        ZL10353_Tuner_Programming( cTunerData );
    #elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
        MSB1200_Tuner_Programming( cTunerData );
    #elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
        *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    *cTunerData=*cTunerData;
    #else
    *cTunerData=*cTunerData;
    #endif
}

#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devCOFDM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetFrequencyOffset(pFreqOff, u8BW);
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetFrequencyOffset(pFreqOff, u8BW);
#else
    *pFreqOff = 0;  //Temp Solution;;
    u8BW = u8BW; //To avoid compiler warning
    return 1;
#endif
}
BOOLEAN devCOFDM_SetAfcEnable(BOOLEAN bEnable)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    bEnable = bEnable;
    return 1;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_SetAfcEnable(bEnable);
#else
    bEnable = bEnable; //To avoid compiler warning
    return 1;
#endif
}

#endif

void devCOFDM_SetFrequency(MS_U32 dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth, RF_CHANNEL_HP_LP eHpLp, U8 u8PlpID)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    BOOLEAN                            b_pal_bg;
#endif
    UNUSED(u8PlpID);
#if ((FRONTEND_DEMOD_TYPE != EMBEDDED_DVBT_DEMOD) && (FRONTEND_DEMOD_TYPE != MSTAR_MSB1210_DEMOD))
    UNUSED(eHpLp);
#endif
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    MSB1200_Active(DISABLE);
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    MT5131_SAMSUNG_NIM_Initialize();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    devDigitalTuner_Init();

    TC90512XBG_Channel_Selection();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)

#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    Active(DISABLE);
#endif

    //dwFrequency = 562000;                   // << Ken 20090629
    //eBandWidth = E_RF_CH_BAND_8MHz;         // << Ken 20090629

    //printf("====dwFrequency = %lu \n", dwFrequency/1000);
    //printf("====eBandWidth = %bx\n", eBandWidth);

#if ((FRONTEND_DEMOD_TYPE != MSTAR_MSB1238_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB124x_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBC_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBT2_DEMOD)&&(FRONTEND_DEMOD_TYPE != EMBEDDED_DVBS_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB1245_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB123x_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB123xc_DEMOD)&&(FRONTEND_DEMOD_TYPE != MSTAR_MSB1400_DEMOD))
    devDigitalTuner_SetFreq((double)(dwFrequency/1000.0), eBandWidth);
#endif


#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_FSM_Restart();
    ZL10353_BW_Programming(eBandWidth);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

  #if (FRONTEND_TUNER_TYPE == MSTAR_MSR1200_TUNER)
    MSB1200_SetFreq((U32)dwFrequency);    //FRA_DBG_071120_Canopus
  #endif

    MsOS_DelayTask(10); //waiting for tuner write

    MSB1200_Config(eBandWidth);

    #if TS_PARALLEL_OUTPUT
        #if ENABLE_CI
        if(!msAPI_Tuner_IsParallelMode())
        {
            MSB1200_Serial_Control(ENABLE);
        }
        else
        #endif
        {
            MSB1200_Serial_Control(DISABLE);
        }
    #else
        #if ENABLE_CI
        if ( msAPI_Tuner_IsParallelMode())
        {
            MSB1200_Serial_Control(DISABLE);
        }
        else
        #endif
        {
            MSB1200_Serial_Control(ENABLE);
        }
    #endif

    //MsOS_DelayTask( 50 );

    MSB1200_Active(ENABLE);

    //MsOS_DelayTask( 5 );

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    MsOS_DelayTask(10); //waiting for tuner write

    //printf("msb1210 config\n");

    b_pal_bg = false;

    #if ENABLE_CI
    if ( msAPI_Tuner_IsParallelMode())
        MSB1210_Config(eBandWidth, false, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE);
    else
    #endif
    MSB1210_Config(eBandWidth, !TS_PARALLEL_OUTPUT, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE);

    //printf("msb1210 active\n");

    MSB1210_Active(ENABLE);

#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)

//    MsOS_DelayTask(100);
    MT5131_SetBW(eBandWidth);
//    MsOS_DelayTask(100);
/*
    if(MT5131_GetLockStatus()==0)
        MT5131_Reset();
*/
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD)
      //SWIIC_SetIICSpeed(150);  //2
               if(LGS_SetAutoMode()== LGS_NO_ERROR)
               {
                   if(LGS_AutoDetect() == LGS_NO_ERROR)
                   {
                        if(LGS_SetManualMode()== LGS_NO_ERROR)
                {
                    //printf("\n\n--------------------------Auto detect done, carrier mode ---------------------\n\n");
                }
                        else
                {
                    //printf("\n\n------------Set ManualMode error ----------------\n\n");
                }
                   }
                   else
                    {
               //printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
               //printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
      //SWIIC_SetIICSpeed(150);  //2
               if(LGS8G52_SetAutoMode()== LGS_NO_ERROR)
               {
                   if(LGS8G52_AutoDetect() == LGS_NO_ERROR)
                   {
                        if(LGS_SetManualMode()== LGS_NO_ERROR)
                {
                    //printf("\n\n--------------------------Auto detect done, carrier mode ---------------------\n\n");
                }
                        else
                {
                    //printf("\n\n------------Set ManualMode error ----------------\n\n");
                }
                   }
                   else
                    {
               //printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
               //printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
	//SWIIC_SetIICSpeed(150);  //2
       	if(LGS8G75_SetAutoMode()== LGS_NO_ERROR)
              {
                   if(LGS8G75_AutoDetect() == LGS_NO_ERROR)
                   {
                        if(LGS8G75_SetManualMode()== LGS_NO_ERROR)
                	   {
                    		printf("\n\n--------------------------Auto detect done, carrier mode ---------------------\n\n");
                	   }
                        else
                	   {
                    		printf("\n\n------------Set ManualMode error ----------------\n\n");
                        }
                    }
                    else
                    {
                       printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
                printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
      //SWIIC_SetIICSpeed(150);  //2
               if(LGS8G80_SetAutoMode()== LGS_NO_ERROR)
               {
                   if(LGS8G80_AutoDetect() == LGS_NO_ERROR)
                   {
                   }
                   else
                    {
               printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
               //printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2


#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    //Demod reset
     TC90512XBG_OFDM_demod_reset();
     TC90512XBG_OFDM_fft_reset();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    //Demod reset
    TC90517_Reset();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    MsOS_DelayTask(5); //waiting for tuner write
    M88DC2800_Config();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    MsOS_DelayTask(5); //waiting for tuner write
    M88DD2000_Config();

#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    MsOS_DelayTask(10); //waiting for tuner write

    //printf("msb1210 config\n");

    b_pal_bg = false;

    #if ENABLE_CI
    if ( msAPI_Tuner_IsParallelMode())// || (0 == TS_SERIAL_OUTPUT_IF_CI_REMOVED))
        INTERN_DVBT_Config(eBandWidth, false, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE);
    else
    #endif
     INTERN_DVBT_Config(eBandWidth, !TS_PARALLEL_OUTPUT, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE); // << ken 2009.06.29 serial
     //INTERN_DVBT_Config(eBandWidth, false, b_pal_bg);
    //printf("msb1210 active\n");

    INTERN_DVBT_Active(ENABLE);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
        ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    DTV_SetFrequency((double)(dwFrequency/1000.0), eBandWidth);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    DTV_SetFrequency(dwFrequency,eBandWidth);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)
	DEMOD_MS_FE_CARRIER_PARAM pParam;
	pParam.u32Frequency = (MS_U32)dwFrequency;
	pParam.TerParam.eBandWidth = eBandWidth;
	MDrv_Demod_Restart(&pParam);
#elif ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD))
    DEMOD_MS_FE_CARRIER_PARAM pParam;
    pParam.u32Frequency = (MS_U32)dwFrequency;
    switch(eBandWidth)
    {
        case E_RF_CH_BAND_6MHz:
            pParam.TerParam.eBandWidth = DEMOD_BW_MODE_6MHZ;
            break;
        case E_RF_CH_BAND_7MHz:
            pParam.TerParam.eBandWidth = DEMOD_BW_MODE_7MHZ;
            break;
        case E_RF_CH_BAND_8MHz:
            pParam.TerParam.eBandWidth = DEMOD_BW_MODE_8MHZ;
    }

    pParam.TerParam.u8PlpID = MDrv_Demod_GetNextPlpID(0);
    pParam.TerParam.u8ScanType = MDrv_Demod_GetCurrentDemodType();
    MDrv_Demod_Restart(&pParam);
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD)
    MDrv_Tuner_SetTuner(pParam.u32Frequency, pParam.TerParam.eBandWidth);
#endif
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD) || (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD))
    MS_SAT_PARAM stSATParam;
    DEMOD_MS_FE_CARRIER_PARAM pParam;
    DEMOD_MS_FE_CARRIER_PARAM pdmdParam;
    extern MS_U16 u16SymbolRate;

    stSATParam.u16LoLOF = 9750;
    stSATParam.u16HiLOF = 10600;
    stSATParam.bPorInvert = 0;
    pParam.SatParam.u8Polarity = 0;
    pParam.u32Frequency = dwFrequency;
    extern BOOLEAN _DigiTuner_CalculateIF(MS_SAT_PARAM , DEMOD_MS_FE_CARRIER_PARAM *, DEMOD_MS_FE_CARRIER_PARAM* );
    _DigiTuner_CalculateIF(stSATParam, &pParam, &pdmdParam);
    pdmdParam.SatParam.u32SymbolRate = u16SymbolRate * 1000;
#if(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD)
    MDrv_Tuner_SetFreq(pdmdParam.u32Frequency, u16SymbolRate);
#endif
    MDrv_Demod_Restart(&pdmdParam);
#endif
}

BOOLEAN devCOFDM_GetLockStatus(COFDM_LOCK_STATUS eStatus)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    return ZL10353_Lock_Indicate(eStatus);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    return MSB1200_Lock(eStatus);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    return MSB1210_Lock(eStatus);

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    U8 u8Result;
    eStatus=eStatus;
    //SWIIC_SetIICSpeed(150);
    u8Result = LGS_CheckLocked();
    //SWIIC_SetIICSpeed(10);
    if(u8Result == 1)
        return TRUE;
    else
        return FALSE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    U8 u8Result;
    U8 u8Temp;
    eStatus=eStatus;
    //SWIIC_SetIICSpeed(150);
    u8Result = LGS8G75_CheckLocked(&u8Temp);
    //SWIIC_SetIICSpeed(10);
    if(u8Temp == 1)
        return TRUE;
    else
        return FALSE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    eStatus=eStatus;
    if(MT5131_GetLockStatus()==FALSE)
    {
        MT5131_Reset();
        return FALSE;
    }else
        return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_Lock_Indicate(eStatus);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return(TC90512XBG_Lock(eStatus));
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    eStatus=eStatus;
    return(TC90517_Lock());
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    eStatus=eStatus;
    return(M88DC2800_GetLock());
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    eStatus=eStatus;
    return(M88DD2000_GetLock());
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    eStatus=eStatus;
    return ATBMLockedFlag();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_Lock(eStatus);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
        ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD))
    return DTV_DVB_T_GetLockStatus();
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)
    EN_LOCK_STATUS eLockStatus = E_DEMOD_UNLOCK;
    eLockStatus = DTV_DVB_T_GetLockStatus();
    if(E_DEMOD_LOCK == eLockStatus)
    {
        //printf("LOCK!!\n");
        return TRUE;
    }
    else
    {
        //printf("eLockStatus = %d\n",eLockStatus);
        return FALSE;
    }

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD)
    EN_LOCK_STATUS eLockStatus = E_DEMOD_UNLOCK;
    eLockStatus = DTV_ISDB_GetLockStatus();
    if(E_DEMOD_LOCK == eLockStatus)
    {
        //printf("LOCK!!\n");
        return TRUE;
    }
    else
    {
        //printf("eLockStatus = %d\n",eLockStatus);
        return FALSE;
    }

#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    return MDrv_Demod_GetLock();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD))
    EN_LOCK_STATUS eLockStatus = E_DEMOD_UNLOCK;
    if(MDrv_Demod_GetLock(&eLockStatus) != TRUE)
    {
        printf("Demod get lock fail\n");
    }
    if(E_DEMOD_LOCK == eLockStatus)
    {
        printf("LOCK!!\n");
        return TRUE;
    }
    else
    {
        printf("eLockStatus = %d\n",eLockStatus);
        return FALSE;
    }
#else
    eStatus = eStatus;
    return TRUE;
#endif
}

U8 devCOFDM_GetSignalToNoiseRatio(void)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    return (U8)ZL10353_GetSNR();

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    return (U8)MSB1200_GetSNR();

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    return (U8)MSB1210_GetSNR();

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return (U8)CE6355_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return (U8)TC90512XBG_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    return (U8)TC90517_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    return (U8)M88DC2800_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    return (U8)ATBMSignalNoiseRatio();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return (U8)INTERN_DVBT_GetSNR();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
    ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    return (U8)DTV_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    return (U8)MDrv_Demod_GetSNR();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD))
    MS_U32 u32SNR;
    MDrv_Demod_GetSNR(&u32SNR);
    return (U8)u32SNR;
#else
    return 1;
#endif
}

#if (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
U8 devCOFDM_GetSignalMode(void)
{
    return LGS8G52_ReadMode();
}
U8 devCOFDM_GetSignalPN(void)
{
        return LGS8G52_ReadPN();
}
U8 devCOFDM_GetSignalPNFLIP(void)
{
    return LGS8G52_ReadPNFLIP();
}
U8 devCOFDM_GetSignalCarrier(void)
{
    return LGS8G52_ReadCarrier();
}
U8 devCOFDM_SetSignalMode(U8 Value)
{
    return LGS8G52_SetMode(Value);
}
U8 devCOFDM_SetSignalPN(U8 Value)
{
    return LGS8G52_SetPN(Value);
}
U8 devCOFDM_SetSignalPNFLIP(U8 Value)
{
    return LGS8G52_SetPNFLIP(Value);
}
U8 devCOFDM_SetSignalCarrier(U8 Value)
{
    return LGS_SetCarrierMode(Value);
}
#endif

void devCOFDM_PassThroughI2C(BOOLEAN bEnable)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_RepeatControl(bEnable);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    bEnable++; //dummy code
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    if(bEnable)
        ATBM_I2CByPassOn();
    else
        ATBM_I2CByPassOff();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    bEnable=bEnable;
#else
    bEnable=bEnable;
#endif
}

#if (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface, RF_CHANNEL_BANDWIDTH BandWidth)
{
    BOOLEAN bEnable;

    if ( interface == E_SERIAL_INTERFACE )
    {
        bEnable = TRUE;
    }
    else
    {
    #if 0
        #if (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD)
        XBYTE[0x1558] = (XBYTE[0x1558]&0xfb) | 0x02;
        #else
        XBYTE[0x1558] = XBYTE[0x1558] | 0x02;
        #endif
     #endif
        bEnable = FALSE;
    }

    CE6355_SwitchTransportSteamInferface(interface, BandWidth);
}
#else
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface)
{
    BOOLEAN bEnable;

    if ( interface == E_SERIAL_INTERFACE )
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }

#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    ZL10353_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    MSB1200_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    MSB1210_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD)
    LGS_SetCarrierMode(interface);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    LGS8G75_SetMpegMode(1,1,0);//bEnable);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    LGS8G80_SetMpegMode(1,1,0);
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
//    MT5131_Serial_Control(bEnable);
    if(interface==E_SERIAL_INTERFACE)
    {
//        printf("Serial\n");
        MT5131_SetTsIf(0x0A);
    }else if(interface==E_PARALLEL_INTERFACE)
    {
//        printf("Parallel\n");
        MT5131_SetTsIf(0x0E);
    }
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)

#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD ||\
       FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    interface=interface;
     //set Demod to clk invert to make sure the TSP clk invert is correct
     //XBYTE[0x1E26] |= 0x02;
     {
        U8 u8Data;

        u8Data = MDrv_ReadByte(0x1E26);
        u8Data|=0x02;
        MDrv_WriteByte(0x1E26,u8Data);
      }
    //printf("---> Do not implement devCOFDM_SwitchTransportSteamInferface???\n");
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    M88DC2800_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    M88DD2000_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    INTERN_DVBT_Serial_Control(bEnable);
    INTERN_DVBT_PAD_TS1_Enable(~bEnable);

#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    {
        struct MPEG_TS_mode_t ts_mode;

        ts_mode.output_edge = TS_OUTPUT_FALLING_EDGE;
        ts_mode.SPI_clock_constant_output = TS_CLOCK_CONST_OUTPUT;
        if(bEnable)
        {
            //printf("set to serial mode\n");
            ts_mode.TS_Transfer_Type = TS_SERIAL_MODE;
        }
        else
        {
            //printf("set to parallel mode\n");
            ts_mode.TS_Transfer_Type = TS_PARALLEL_MODE;
            MDrv_WriteByte(0x101558,0x06);  //add patch here to change TS mode to parallel
        }
        ATBMSetTSMode(ts_mode);
    }
#endif

}
#endif

FUNCTION_RESULT devCOFDM_GetSignal_Vit( MS_U32 *vit )
{

#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetSignal_Vit(vit);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    float ber;
    *vit = *vit;
    return MSB1200_GetPostViterbiBer(&ber);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    float ber;
    *vit = *vit;
    if (MSB1210_GetPostViterbiBer(&ber)==TRUE)
       return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_GetSignal_Vit(vit);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return TC90512XBG_GetSignal_BER(vit);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    float ber;
    *vit = *vit;
    if (INTERN_DVBT_GetPostViterbiBer(&ber)==TRUE)
       return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    *vit = *vit;
    return E_RESULT_FAILURE;
#endif
}

FUNCTION_RESULT devCOFDM_GetSignalStrength(MS_U16 *strength)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
	*strength=LGS8G52_ReadIntension();
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
     *strength=*strength;
    return E_RESULT_SUCCESS;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
     *strength=LGS8G80_ReadIntension();
    return E_RESULT_SUCCESS;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
     *strength=*strength;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return TC90512XBG_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    return TC90517_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    return M88DC2800_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
     *strength=*strength;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    *strength=*strength;
    return (FUNCTION_RESULT)ATBMSignalStrength();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_GetSignalStrength(strength);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
        ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    *strength = DTV_GetSignalStrength();
    return E_RESULT_SUCCESS;
#else
    *strength=*strength;
    return E_RESULT_FAILURE;
#endif

}

BOOLEAN devCOFDM_Get_CELL_ID(MS_U16 *wCell_id)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_Get_CELL_ID(wCell_id);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_Get_CELL_ID(wCell_id);
#else
    *wCell_id = *wCell_id;
    return FALSE;
#endif

}

FUNCTION_RESULT devCOFDM_GetSignalQuality(MS_U16 *quality)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return  TC90512XBG_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    return  TC90517XBG_GetSignalQuality(quality);

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    *quality = *quality;
    return (FUNCTION_RESULT)ATBMSignalQuality();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_GetSignalQuality(quality);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
        ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    *quality = (MS_U16)DTV_GetSignalQuality();
    return E_RESULT_SUCCESS;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    return (U8)DTV_GetSignalQuality();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD))
    MS_U32 u32SNR;
    MDrv_Demod_GetSNR(&u32SNR);
    return (U8)u32SNR;
#else
    *quality = *quality;
    return E_RESULT_FAILURE;
#endif
}

BOOLEAN devCOFDM_GetBER(float *ber)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetPostViterbiBer(ber);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_GetPostViterbiBer(ber);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_GetPostViterbiBer(ber);
#else
    *ber=*ber;
    return FALSE;
#endif
}

BOOLEAN devCOFDM_Get_Modul_Mode(MS_U16 *Modul_Mode)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_Get_TSP_parameter_Mode(Modul_Mode);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;//MSB1200_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_Get_TPS_Parameter_Const(Modul_Mode);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;//CE6355_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_Get_TPS_Parameter_Const(Modul_Mode);
#else
    *Modul_Mode=*Modul_Mode;
    return 0;
#endif

}


BOOLEAN devCOFDM_Is_Hierarchy_On(void)
{
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_Is_HierarchyOn();
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_Is_HierarchyOn();
#else
    return FALSE;
#endif

}

BOOLEAN devCOFDM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
#ifdef USE_SEC_IIC
	U16 U16SlaveID=u8SlaveID;
	U16SlaveID=(U16)((E_I2C_BUS_SYS2<<8)|u8SlaveID);
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
    ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    IIC_Bypass_Mode(1);

    MApi_SWI2C_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data);

    IIC_Bypass_Mode(0);

    return TRUE;
#endif
    return TRUE;
}

BOOLEAN devCOFDM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
	U16 U16SlaveID=u8SlaveID;
	#ifdef USE_SEC_IIC
	U16SlaveID=(U16)((E_I2C_BUS_SYS2<<8)|u8SlaveID);
	#endif
#if (OTHER_TUNER_DEBUG==1)
    int i;
    printf("===> COFDM(ID=0x%bx)(Size=0x%x) ",U16SlaveID,u16size);
    printf("  Data =  ");
    for (i=0;i<u16size;i++)
        printf("0x%bx  ",*(pu8data+i));
    printf("\n");
#endif

#if(FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H||FRONTEND_TUNER_TYPE == XUGUANG_STM_6F_V236H_TUNER \
    ||FRONTEND_TUNER_TYPE==XUGUANG_HFT_8B1_V116H_TUNER || FRONTEND_TUNER_TYPE==XUGUANG_HFT_8B_115CW_TUNER \
    || FRONTEND_TUNER_TYPE == LINGHUA_TDP_1H1_E_TUNER)
    U16SlaveID |= (U16)((TUNER_IIC_BUS<<8)|u8SlaveID);
    return MApi_SWI2C_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) ;

#else // (FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)


#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
#ifdef BD_MST072A_D01A_WITH_ONBOARD_TUNER_ATV
    printf("---> ATV Only, I2c skip DTV-Demod ...\n");
#else
    ZL10353_RepeatControl(ENABLE);
#endif
#if ((MS_BOARD_TYPE_SEL == BD_MST087A_D01A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087B_S7M_D01A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087B_S7_D01A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087C_D02A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087D_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST105A_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST105B_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST106A_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST106B_D01A_S))

if ( FALSE == MApi_SWI2C_WriteBytes((U32)(U16SlaveID|0x0200), u8AddrNum, paddr, u16size, pu8data) )
{
    printf("---> devCOFDM_PassThroughI2C_WriteBytes Error-2 \n");
    return FALSE;
}


#else
    #if (CHIP_FAMILY_TYPE==CHIP_FAMILY_S7J) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARIA10)
    U16SlaveID=(U16)((DEMOD_IIC_BUS<<8)|U16SlaveID);
    #endif
    if ( FALSE == MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }
#endif
    MsOS_DelayTask(5); //waiting for tuner write
#ifdef BD_MST072A_D01A_WITH_ONBOARD_TUNER_ATV
    printf("---> ATV Only, I2c skip DTV-Demod ...\n");
#else
    ZL10353_RepeatControl(DISABLE);
#endif

    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
        return MSB1200_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    MDrv_1210_IIC_Bypass_Mode(TRUE);        // IIC by-pass mode on

    if (MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) == FALSE)
    {
        printf("read write fail---%x\n",U16SlaveID);
        return FALSE;
    }

    MDrv_1210_IIC_Bypass_Mode(FALSE);        // IIC by-pass mode off
        return TRUE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
  #if (MS_BOARD_TYPE_SEL == BD_MST087F_D01A_T)
	U16SlaveID=(U16)((E_I2C_BUS_DDCD0<<8)|u8SlaveID);

    if (MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) == FALSE)
        printf("devCOFDM_PassThroughI2C_WriteBytes error-- %x-\n",U16SlaveID);
  #else
    //SWIIC_SetIICSpeed( 150 );
    LGS_OpenTunerI2C(u8SlaveID)  ;
   	if(MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data)==FALSE)
		printf("devCOFDM_PassThroughI2C_WriteBytes error------error---error-- %x-\n",U16SlaveID);
    LGS_CloseTunerI2C() ;
    //SWIIC_SetIICSpeed( 10 );
  #endif
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
	U16SlaveID=(U16)((E_I2C_BUS_DDCA0<<8)|u8SlaveID);

    if (MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) == FALSE)
    {
        printf("\r\n 20091027--write fail\n");
    }
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    MDrv_LDELCK_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    if ( FALSE == MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }

    msAPI_Timer_Delayms(1); // delay 1ms

    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD ||\
       FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    {
        U8 u8data[10],i;

        u8data[0] = 0xFE;//through start address
        u8data[1] = U16SlaveID;//through start address
        if (u16size > 9)
        {
            printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
            return FALSE;
        }

        for (i=0;i<u16size;i++)
            u8data[2+i] = *(pu8data+i);

#if 0
        printf("--->Demod write data 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx \n",
                  u8data[0],u8data[1],u8data[2],u8data[3],u8data[4],u8data[5],u8data[6],u8data[7]);
#endif
        MApi_SWI2C_WriteBytes(COFDM_DEM_I2C_ID, 0, paddr, (u16size+2), u8data) ;
        u8AddrNum = u8AddrNum;//Remove Compiler Warning
            return TRUE;
    }

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    M88DC2800_RepeatControl(ENABLE);
    if ( FALSE == MApi_SWI2C_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }
    M88DC2800_RepeatControl(DISABLE);

    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    M88DD2000_WriteBytesRepeat(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    ATBM_I2CByPassOn();
    //printf("bypass write addr 0x%02x\n",U16SlaveID);
    MApi_SWI2C_WriteBytes(U16SlaveID|0x0200,u8AddrNum,paddr,u16size,pu8data);
    ATBM_I2CByPassOff();
    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    UNUSED(U16SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
        ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
    UNUSED(U16SlaveID);
    IIC_Bypass_Mode(1);

    if ( FALSE == MApi_SWI2C_WriteBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        IIC_Bypass_Mode(0);
        return FALSE;
    }

    IIC_Bypass_Mode(0);
    msAPI_Timer_Delayms(1); // delay 1ms

    return TRUE;
#else
    UNUSED(U16SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
#endif

#endif  //(FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)

}

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

U16 devCOFDM_ReadReg(U16 RegAddr)
{
    U8 RegData;
    if((RegAddr >= 0xC000) && (RegAddr <= 0xCEFF))
        MSB1200_ReadRaptor(RegAddr, &RegData);
    else
        MSB1200_ReadReg(RegAddr, &RegData);
    return (U16) RegData;
}

BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData)
{
    if((RegAddr >= 0xC000) && (RegAddr <= 0xCEFF))
        return MSB1200_WriteRaptor( RegAddr, (U8)RegData);
    else
        return MSB1200_WriteReg( RegAddr, (U8)RegData);
}
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

U16 devCOFDM_ReadReg(U16 RegAddr)
{
    U8 RegData;
    MSB1210_ReadReg(RegAddr, &RegData);
    return (U16) RegData;
}

BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData)
{
    return MSB1210_WriteReg( RegAddr, (U8)RegData);
}
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD || FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
U16 devCOFDM_ReadReg(U16 RegAddr)
{
    U8 RegData;
    INTERN_DVBT_ReadReg(RegAddr, &RegData);
    return (U16) RegData;
}

BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData)
{
    return INTERN_DVBT_WriteReg( RegAddr,(U8) RegData);
}
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) \
    ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    #if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD)
    return SetCurrentDemodulatorType(enDemodType);
    #else
    Disconnect();
    SetCurrentDemodulatorType(enDemodType);
    return Connect(enDemodType);
    #endif
}
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void)
{
    return DTV_Get_Demod_TYPE();
}
#endif

BOOLEAN devCOFDM_InitFrontend(EN_DEVICE_DEMOD_TYPE enDemodType, MS_U32 u32Frequency, MS_U8 u8BandWidth)
{
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD))
    Connect(enDemodType);

    Power_On_Initialization();

#if 1 // Set Frequency and test lock status
#if (FRONTEND_TUNER_TYPE == TUNER_MXL608 || FRONTEND_TUNER_TYPE == TUNER_R850)
    IIC_Bypass_Mode(1);

    MXL608_BW_E MsBandWidth;
    switch(u8BandWidth)
    {
        case E_RF_CH_BAND_6MHz:
        {
            MsBandWidth=MXL608_TERR_BW_6MHz;
        }
        break;
        case E_RF_CH_BAND_7MHz:
        {
            MsBandWidth=MXL608_TERR_BW_7MHz;
        }
        break;
        case E_RF_CH_BAND_8MHz:
        {
            MsBandWidth=MXL608_TERR_BW_8MHz;
        }
        break;
        default:
            MsBandWidth=MXL608_TERR_BW_8MHz;
    }
//    printf("[%s][%s][%d] MDrv_Tuner_SetTuner \n", __FILE__, __FUNCTION__, __LINE__);

    MDrv_Tuner_SetTuner(u32Frequency, MsBandWidth);

    IIC_Bypass_Mode(0);
#endif
    if(u32Plp != 0x0000FFFF)
    {
        DTV_SetPlpGroupID((U8) (u32Plp & 0x00FF), 0xff);
    }

    BOOLEAN bRet = DTV_SetFrequency(u32Frequency, u8BandWidth);
    if( bRet != TRUE)
    {
        printf("[%s][%s][%d] DTV_SetFrequency fail\n", __FILE__, __FUNCTION__, __LINE__);
        return FALSE;
    }

    int i=0;
    EN_LOCK_STATUS status = E_DEMOD_UNLOCK;
    do
    {
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)
        status = DTV_DVB_T_GetLockStatus();
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD)
        status = DTV_ISDB_GetLockStatus();
#endif
        printf("[%s][%s][%d]  GetLockStatus:%d\n", __FILE__, __FUNCTION__, __LINE__, status);
        mdelay(100);
        i++;
//    } while(status == E_DEMOD_CHECKING);
    }while (i<10);
    printf("[%s][%s][%d]  GetLockStatus:%d\n", __FILE__, __FUNCTION__, __LINE__, status);
#endif

    return TRUE;
#else
    printf("demod tuner is not configured\n");
    return FALSE;
#endif

}


#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD) ||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD))
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    EN_DEMOD_TYPE eType = E_DEMOD_TYPE_T;
    if(enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        eType = E_DEMOD_TYPE_T;
    }
    else
    {
        eType = E_DEMOD_TYPE_T2;
    }

    return MDrv_Demod_SetCurrentDemodType(eType);

}
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void)
{
   return MSB124X_GetCurrentDemodulatorType();
}
#endif

#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD)
MS_BOOL devCOFDM_SetDemodBuffer(MS_U32 u32Addr)
{
   return DEMOD_MSDVBT2_Extension_Function(0, DEMOD_EXT_FUNC_SET_BUFFER_ADDR, &u32Addr);
}
#endif


#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
BOOLEAN devCOFDM_DTV_Serial_Control(BOOLEAN bEnable)
{
    return DTV_Serial_Control(bEnable);
}
#endif

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD || (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD))
BOOLEAN _DigiTuner_CalculateIF(MS_SAT_PARAM stSATParam, DEMOD_MS_FE_CARRIER_PARAM *pParam, DEMOD_MS_FE_CARRIER_PARAM* pdmdParam)
{
    MS_S32 s32Freq;

    // calc Mid Freq & LNB control(22K on/off)
    if (stSATParam.u16LoLOF == stSATParam.u16HiLOF)
    {
        s32Freq  = pParam->u32Frequency;
        s32Freq -= stSATParam.u16LoLOF;
        if (s32Freq < 0) s32Freq = -s32Freq;
        pdmdParam->u32Frequency = s32Freq;
    }
    else // 2LOF
    {
        if (stSATParam.u16LoLOF < MAX_C_LOF_FREQ) // c band
        {
            s32Freq  = pParam->u32Frequency;
            if (stSATParam.bPorInvert)
            {
                if (pParam->SatParam.u8Polarity == 1) //V ---> use low LOF
                {
                    pdmdParam->u32Frequency = stSATParam.u16LoLOF - s32Freq;
                }
                else   //H --->use high LOF
                {
                    pdmdParam->u32Frequency =  stSATParam.u16HiLOF - s32Freq;
                }
            }
            else
            {
                if (pParam->SatParam.u8Polarity == 0) //H ---> use low LOF
                {
                    pdmdParam->u32Frequency = stSATParam.u16LoLOF - s32Freq;
                }
                else   //V --->use high LOF
                {
                    pdmdParam->u32Frequency = stSATParam.u16HiLOF - s32Freq;
                }
            }
        }
        else //Ku band
        {
            MS_U32 u32MidFreq;
            MS_S32 s32Offset = 0;
            s32Freq  = pParam->u32Frequency;
            u32MidFreq =  s32Freq - stSATParam.u16LoLOF;
            s32Offset = stSATParam.u16LoLOF + 2150 - stSATParam.u16HiLOF - 950;
            if (s32Offset < 0)
            {
                s32Offset = 0;
            }
            else
            {
                s32Offset /= 2;
            }
            if (u32MidFreq <= (2150 - s32Offset))
            {
                pdmdParam->u32Frequency = u32MidFreq;
            }
            else
            {
                pdmdParam->u32Frequency = s32Freq - stSATParam.u16HiLOF;
            }
        }
    }
	if(pdmdParam->u32Frequency < 0)
	{
		pdmdParam->u32Frequency *= -1;
	}

    if ((pdmdParam->u32Frequency < 950) || (pdmdParam->u32Frequency > 2150))
    {
        pdmdParam->u32Frequency = 945;
    }
    return TRUE;
}

#endif

#if ENABLE_DVB_T2_SYS
MS_U8 devCOFDM_GetPlpIDList(void)
{
     return MDrv_Demod_GetPlpIDList();
}

MS_U8 devCOFDM_GetNextPlpID(MS_U8 u8Index)
{
    return MDrv_Demod_GetNextPlpID(u8Index);
}

MS_BOOL devCOFDM_SetPlpID(MS_U8 u8PlpID)
{
    return MDrv_Demod_SetPlpID(u8PlpID);
}
#endif

#endif
