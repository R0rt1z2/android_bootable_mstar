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
#include <stdio.h>
//#include <string.h>

//#include "mapi_i2c_devTable.h"
#include "MsTypes.h"
//#include "mapi_i2c.h"
//#include "mapi_interface.h"

#define MsOS_DelayTask(ms) usleep(ms * 1000)

#define DBG_TUNER(x)  //x

#include <stdio.h>
//#include "error.h"
#include "MsCommon.h"
#include "Board.h"
//#include "apiDigiTuner.h"
//#include "drvTuner.h"


//#if 1//(FRONTEND_TUNER_TYPE == TUNER_MXL608)

#include "MxL608/MxL608_TunerApi.h"
#include "MxL608/MxL608_OEM_Drv.h"
//#include "device_tuner_MXL608.h"
#define EXAMPLE_DEV_MAX   2
#define MXL608_I2C_ADDR   0xC0
#define MXL608_CHIP_ID 0x02

#if (ISDB_SYSTEM_ENABLE == 1)
#define MXL608_INC_CURRENT 0
#else
#define MXL608_INC_CURRENT 1
#endif

#if 0//(MS_DVB_TYPE_SEL == DVBC)
MS_U8 dmdConfig[] = {};
#endif

#if MXL608_INC_CURRENT
/* Increase driving current for IFamp
 */
 static MS_U8 u8Possible_SLAVE_IDs[4] = {0xC0, 0xC2, 0xC4, 0xC6};
 static MS_U8 u8SlaveID =MXL608_I2C_ADDR;

static MXL_STATUS MDrv_Tuner_IncCurrent(void)
{
    UINT8 status = MXL_SUCCESS;

    status |= MxLWare608_OEM_WriteRegister(MXL608_I2C_ADDR, 0x5B, 0x10);
    status |= MxLWare608_OEM_WriteRegister(MXL608_I2C_ADDR, 0x5C, 0xB1);
	
    return (MXL_STATUS) status;
}
#endif

MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
#if 0
    MS_BOOL ret;
    if(bPowerOn==FALSE)
    {
        ret = MxLWare608_API_CfgDevPowerMode(MXL608_I2C_ADDR, MXL608_PWR_MODE_STANDBY);
        if(MxL_ERR_OTHERS == ret)
            return FALSE;
        return TRUE;
    }
    else
    {
        ret = MxLWare608_API_CfgDevPowerMode(MXL608_I2C_ADDR, MXL608_PWR_MODE_ACTIVE);
        if(MxL_OK == ret)
            return TRUE;
        return FALSE;
    }
#endif
    return TRUE;
}
MS_BOOL MXL608_CheckExist(void)
{
    MXL608_VER_INFO_T mxlDevVerInfo;
    MS_U8 i,u8CurID;

    for(i=0; i< sizeof(u8Possible_SLAVE_IDs); i++)
    {
        u8CurID = u8Possible_SLAVE_IDs[i];
        if(MXL_SUCCESS != MxLWare608_API_ReqDevVersionInfo(u8CurID, &mxlDevVerInfo))
        {
            printf("[mxl608] Read chip ID fail with slave ID 0x%x\n", u8CurID);
        }
        else
        {
            u8SlaveID=u8CurID;
            break;
        }
    }

    printf("[mxl608] read id =0x%x\n",mxlDevVerInfo.chipId);

    if(mxlDevVerInfo.chipId == MXL608_CHIP_ID)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
MS_BOOL MXL608_GetTunerIF(MS_U32* u32IF_Freq)
{
    UINT16 ifFcw;
    UINT8 status = MXL_SUCCESS;
    UINT8 readData = 0;

  // Read back register for manual IF Out
  status |= MxLWare608_OEM_ReadRegister(u8SlaveID, IF_FREQ_SEL_REG, &readData);
     // IF out manual setting : bit<5>
  if((readData & 0x20) == 0x20)
  {
      //Get low 8 bit
      status |= MxLWare608_OEM_ReadRegister(u8SlaveID, IF_FCW_LOW_REG, &readData);
      ifFcw = (UINT16)readData;
      // Get high 4 bit
      status |= MxLWare608_OEM_ReadRegister(u8SlaveID, IF_FCW_HIGH_REG, &readData);
      ifFcw |= ((UINT16)(readData) & 0x0f) << 8;
      *u32IF_Freq = (MS_U32)((ifFcw * 216000)/8192);
  }
  else
  {
      // IF Freq <4:0>
      status |= MxLWare608_OEM_ReadRegister(u8SlaveID, IF_FREQ_SEL_REG, &readData);
      readData &= 0x1f;
      switch(readData)
      {
          case MXL608_IF_3_65MHz:
              *u32IF_Freq = 3650;
              break;
          case MXL608_IF_4MHz:
              *u32IF_Freq = 4000;
              break;
          case MXL608_IF_4_1MHz:
              *u32IF_Freq = 4100;
              break;
          case MXL608_IF_4_15MHz:
              *u32IF_Freq = 4150;
              break;
          case MXL608_IF_4_5MHz:
              *u32IF_Freq = 4500;
              break;
          case MXL608_IF_4_57MHz:
              *u32IF_Freq = 4570;
              break;
          case MXL608_IF_5MHz:
              *u32IF_Freq = 5000;
              break;
          case MXL608_IF_5_38MHz:
              *u32IF_Freq = 5380;
              break;
          case MXL608_IF_6MHz:
              *u32IF_Freq = 6000;
              break;
          case MXL608_IF_6_28MHz:
              *u32IF_Freq = 6280;
              break;
          case MXL608_IF_7_2MHz:
               *u32IF_Freq = 7200;
              break;
          case MXL608_IF_8_25MHz:
              *u32IF_Freq = 8250;
              break;
          case MXL608_IF_35_25MHz:
              *u32IF_Freq = 35250;
              break;
          case MXL608_IF_36MHz:
              *u32IF_Freq = 36000;
              break;
          case MXL608_IF_36_15MHz:
              *u32IF_Freq = 36150;
              break;
          case MXL608_IF_36_65MHz:
              *u32IF_Freq = 36650;
              break;
          case MXL608_IF_44MHz:
              *u32IF_Freq = 44000;
              break;
          default:
              *u32IF_Freq = 5000;
              break;
      }
  }

  if(MXL_SUCCESS == status)
    return TRUE;
  else
    return FALSE;
}

int MxL608_init_main(void)
{
    MXL_STATUS status;
    UINT8 devId;
    MXL_BOOL singleSupply_3_3V;
    MXL608_XTAL_SET_CFG_T xtalCfg;
    MXL608_IF_OUT_CFG_T ifOutCfg;
    MXL608_AGC_CFG_T agcCfg;
    MXL608_TUNER_MODE_CFG_T tunerModeCfg;

    //MXL608_CHAN_TUNE_CFG_T chanTuneCfg;
    //MXL_BOOL refLockPtr = MXL_UNLOCKED;
    //MXL_BOOL rfLockPtr = MXL_UNLOCKED;

    /* If OEM data is implemented, customer needs to use OEM data structure
       related operation. Following code should be used as a reference.
       For more information refer to sections 2.5 & 2.6 of
       MxL608_mxLWare_API_UserGuide document.

      for (devId = 0; devId < EXAMPLE_DEV_MAX; devId++)
      {
        // assigning i2c address for  the device
        device_context[devId].i2c_address = GET_FROM_FILE_I2C_ADDRESS(devId);

        // assigning i2c bus for  the device
        device_context[devId].i2c_bus = GET_FROM_FILE_I2C_BUS(devId);

        // create semaphore if necessary
        device_context[devId].sem = CREATE_SEM();

        // sample stat counter
        device_context[devId].i2c_cnt = 0;

        status = MxLWare608_API_CfgDrvInit(devId, (void *) &device_context[devId]);

        // if you don’t want to pass any oem data, just use NULL as a parameter:
        // status = MxLWare608_API_CfgDrvInit(devId, NULL);
      }

    */
    if(MxLWare608_OEM_Init() == MXL_FALSE)
    {
        printf("MXL608 Init Fail\n");
        return MXL_FAILED;
    }

    /* If OEM data is not required, customer should treat devId as
     I2C slave Address */
    devId = MXL608_I2C_ADDR;

    //Step 1 : Soft Reset MxL608
    status = MxLWare608_API_CfgDevSoftReset(devId);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgDevSoftReset\n");
        return status;
    }

    //Step 2 : Overwrite Default
    singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;
    status = MxLWare608_API_CfgDevOverwriteDefaults(devId, singleSupply_3_3V);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgDevOverwriteDefaults\n");
        return status;
    }

    //Step 3 : XTAL Setting
    xtalCfg.xtalFreqSel = MXL608_XTAL_24MHz;
#if (ISDB_SYSTEM_ENABLE == 1)
    xtalCfg.xtalCap = 0;  //12;
#else
    xtalCfg.xtalCap = 22;  //12;
#endif
    xtalCfg.clkOutEnable = MXL_DISABLE;
    xtalCfg.clkOutDiv = MXL_DISABLE;
    xtalCfg.clkOutExt = MXL_DISABLE;
    xtalCfg.singleSupply_3_3V = MXL_ENABLE;
    xtalCfg.XtalSharingMode = MXL_DISABLE;
    status = MxLWare608_API_CfgDevXtal(devId, xtalCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgDevXtal\n");
        return status;
    }

    //Step 4 : IF Out setting
    ifOutCfg.ifOutFreq = MXL608_IF_5MHz;
    ifOutCfg.manualIFOutFreqInKHz = 5000;
#if (ISDB_SYSTEM_ENABLE == 1)
    ifOutCfg.ifInversion = MXL_ENABLE;
#else
    ifOutCfg.ifInversion = MXL_DISABLE;
#endif
    ifOutCfg.gainLevel = 11;
    ifOutCfg.manualFreqSet = MXL_DISABLE;

    status = MxLWare608_API_CfgTunerIFOutParam(devId, ifOutCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgTunerIFOutParam\n");
        return status;
    }

    //Step 5 : AGC Setting
    agcCfg.agcType = MXL608_AGC_EXTERNAL;
    agcCfg.setPoint = 66;
    agcCfg.agcPolarityInverstion = MXL_DISABLE;
    status = MxLWare608_API_CfgTunerAGC(devId, agcCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgTunerAGC\n");
        return status;
    }

    //Step 6 : Application Mode setting
#if (ISDB_SYSTEM_ENABLE == 0)
//#if (MS_DVB_TYPE_SEL == DVBC)
    //tunerModeCfg.signalMode = MXL608_DIG_DVB_C;
//#elif (MS_DVB_TYPE_SEL == DVBT || MS_DVB_TYPE_SEL == DVBT2)
    tunerModeCfg.signalMode = MXL608_DIG_DVB_T_DTMB;
#else
//#elif (MS_DVB_TYPE_SEL == ISDBT)
    tunerModeCfg.signalMode = MXL608_DIG_ISDBT_ATSC;
//#endif
#endif

#if MXL608_INC_CURRENT
    status = MDrv_Tuner_IncCurrent();
    if(status != MXL_SUCCESS)
    {
        printf("Error! MDrv_Tuner_IncCurrent\n");
        return status;
    }
#endif
    tunerModeCfg.ifOutFreqinKHz = 5000;//4570;//6000;//4000;
    tunerModeCfg.xtalFreqSel = MXL608_XTAL_24MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    status = MxLWare608_API_CfgTunerMode(devId, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgTunerMode\n");
        return status;
    }

    return MXL_SUCCESS;
}

#ifdef DVBT2_STYLE
FEMode TunerMode;
void MDrv_Tuner_SetTunerMode(FEMode mode)
{
    TunerMode.Fetype = mode.Fetype;
}
#endif

MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MXL608_BW_E ucBw /*MHz*/)
{
    MXL_STATUS status = MXL_FALSE;
    MXL608_BW_E eBw = ucBw;
    MXL608_CHAN_TUNE_CFG_T chanTuneCfg;
    MXL_BOOL refLockPtr = MXL_UNLOCKED;
    MXL_BOOL rfLockPtr = MXL_UNLOCKED;
    UINT8 regData = 0;

    //Step 7 : Channel mode & bandwidth & frequency setting
#if (ISDB_SYSTEM_ENABLE == 0)
//#if (MS_DVB_TYPE_SEL == DVBC)
    //chanTuneCfg.signalMode = MXL608_DIG_DVB_C;
//#elif ((MS_DVB_TYPE_SEL == DVBT) || (MS_DVB_TYPE_SEL == DVBT2))
    chanTuneCfg.signalMode = MXL608_DIG_DVB_T_DTMB;
//#elif (MS_DVB_TYPE_SEL == ISDBT)
#else
    chanTuneCfg.signalMode = MXL608_DIG_ISDBT_ATSC;
//#endif
#endif

    switch(chanTuneCfg.signalMode)
    {
        case MXL608_DIG_DVB_C:
            switch(eBw)
            {
                case MXL608_CABLE_BW_6MHz:
                case MXL608_CABLE_BW_7MHz:
                case MXL608_CABLE_BW_8MHz:
                    chanTuneCfg.bandWidth = eBw;
                    break;
                default:
                    chanTuneCfg.bandWidth = MXL608_CABLE_BW_8MHz;
            }
            break;
        case MXL608_DIG_DVB_T_DTMB:
            eBw = (MXL608_BW_E)(eBw + MXL608_TERR_BW_6MHz);
            switch(eBw)
            {
                case MXL608_TERR_BW_6MHz:
                case MXL608_TERR_BW_7MHz:
                case MXL608_TERR_BW_8MHz:
                    chanTuneCfg.bandWidth = eBw;
                    break;
                default:
                    chanTuneCfg.bandWidth = MXL608_TERR_BW_8MHz;
            }
            break;
        case MXL608_DIG_ISDBT_ATSC:
            //eBw = eBw + MXL608_TERR_BW_6MHz;
            switch(eBw)
            {
                case MXL608_TERR_BW_6MHz:
                case MXL608_TERR_BW_7MHz:
                case MXL608_TERR_BW_8MHz:
                    chanTuneCfg.bandWidth = eBw;
                    break;
                default:
                    chanTuneCfg.bandWidth = MXL608_TERR_BW_6MHz;
            }
            break;
        default:
            chanTuneCfg.bandWidth = MXL608_TERR_BW_8MHz;
            break;
    }
#if MXL608_INC_CURRENT
    status = MDrv_Tuner_IncCurrent();
    if(status != MXL_SUCCESS)
    {
        printf("Error! MDrv_Tuner_IncCurrent\n");
        return status;
    }
#endif
    chanTuneCfg.freqInHz = dwFreq * 1000;
    chanTuneCfg.xtalFreqSel = MXL608_XTAL_24MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
//#if (MS_DVB_TYPE_SEL == DVBC)
    //status |= MxLWare608_OEM_WriteRegister(MXL608_I2C_ADDR, 0xC2, 0xA8);
//#endif
    status = MxLWare608_API_CfgTunerChanTune(MXL608_I2C_ADDR, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare608_API_CfgTunerChanTune\n");
    }

    // Wait 15 ms
    MxLWare608_OEM_Sleep(15);

    // Read back Tuner lock status
    status = MxLWare608_API_ReqTunerLockStatus(MXL608_I2C_ADDR, &rfLockPtr, &refLockPtr);
    if (status == MXL_TRUE)
    {
        if ((MXL_LOCKED == rfLockPtr) && (MXL_LOCKED == refLockPtr))
        {
            printf("Tuner locked\n");
            MxLWare608_OEM_ReadRegister(MXL608_I2C_ADDR, 0x5B, &regData);
            printf ("MxL608_0x5B : %x\n", regData);
            MxLWare608_OEM_ReadRegister(MXL608_I2C_ADDR, 0x5C, &regData);
            printf ("MxL608_0x5C : %x\n", regData);
        }
        else
        {
            printf("Tuner unlocked\n");
        }
    }

    return TRUE;
}

MS_BOOL MDrv_Tuner_Init(void)
{
     if(MXL608_CheckExist() != TRUE)
    {
       printf("MXL608 INIT FAILED:Chip ID is incorrect\n");
       return FALSE;
    }
    if(MxL608_init_main() == 0)  //0 is success
    {
        printf("\n MXL608 INIT OK\n");
        return TRUE;
    }
    else
    {
        printf("\n MXL608 INIT FAILED\n");
        return FALSE;
    }
}
MS_BOOL Mdrv_GetTunerIF(MS_U32* u32IF_Freq)
{
    if(MXL608_GetTunerIF(u32IF_Freq))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************5007 API End*******************************/

/*****************************************C++ Interface*******************************/
#if 0
device_tuner_MXL608::device_tuner_MXL608(void)
{
    u8DeviceBusy = 0;
}

#if (STR_ENABLE == 1)
MAPI_BOOL device_tuner_MXL608::TunerReset()
{
    return MAPI_TRUE;
}
#endif

//init the tuner.
MAPI_BOOL device_tuner_MXL608::Connect(void)
{
    return MDrv_Tuner_Init();
}

MAPI_BOOL device_tuner_MXL608::Disconnect()
{
    DBG_TUNER(printf("device_tuner_CD1616LF_GIGH_4: Disconnect\n"));

    u8DeviceBusy=0;
    return MAPI_TRUE;
}

MAPI_BOOL device_tuner_MXL608::ATV_SetTune(MAPI_U32 u32FreqKHz, RFBAND eBand, EN_TUNER_MODE eMode,MAPI_U8 u8OtherMode)
{
    return MAPI_TRUE;
}

MAPI_BOOL device_tuner_MXL608::DTV_SetTune( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth, EN_TUNER_MODE eMode)
{
    //UINT32 dwError=0;
    //MAPI_BOOL bLock;
    //if(InitTuner() == MAPI_FALSE)
    //{
    //  printf("mxl InitTuner error \n");
    //}
    DBG_TUNER(printf("[device_tuner_MXL_5007t] (%d): Connect\n",__LINE__));
    MXL608_BW_E MsBandWidth;
    switch(eBandWidth)
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
    if(MAPI_FALSE == MDrv_Tuner_SetTuner(Freq*1000,MsBandWidth))
    {
        DBG_TUNER(printf("\r\n SetTuner failed"));
        return MAPI_FALSE;
    }
    return MAPI_TRUE;
}


MAPI_BOOL device_tuner_MXL608::ExtendCommand( MAPI_U8 u8SubCmd, MAPI_U32 u32Param1, MAPI_U32 u32Param2, void* pvoidParam3)
{
    if (u8DeviceBusy == 0)
        return MAPI_FALSE;
    /*
    if (u8SubCmd == mapi_tuner::E_TUNER_SUBCMD_POWER_OFF)
    {
        MxL5007_TunerConfigS myTuner;
        //Standby Tuner;
        MxL_Stand_By(&myTuner);
    }
    */
    return MAPI_TRUE;
}
#endif
