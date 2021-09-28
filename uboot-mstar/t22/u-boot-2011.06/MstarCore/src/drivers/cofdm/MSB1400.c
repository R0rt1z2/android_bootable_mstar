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



#include "drvGPIO.h"
#include "MSB1400.h"

#if (ATSC_SYSTEM_ENABLE == 1)
#include "drvDMD_INTERN_ATSC.h"
#endif

#if (DVBT_SYSTEM_ENABLE == 1)
#include "drvDMD_INTERN_DVBT.h"
#endif

#if (DVBC_SYSTEM_ENABLE == 1)
#include "drvDMD_INTERN_DVBC.h"
#endif

//#if (ISDB_SYSTEM_ENABLE == 1)
#include "drvDMD_ISDBT.h"       //dan add for integrate ISDBT utopia driver
//#endif

#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"

//dan add for integrate ISDBT utopia driver[begin]
#include "apiSWI2C.h"
#include "drvHWI2C.h"
//dan add for integrate ISDBT utopia driver[end]

//dan add for integrate ISDBT utopia driver[begin]
#include "apiSWI2C.h"
#include "drvHWI2C.h"

//dan add for integrate ISDBT utopia driver[end]

#define DEMOD_MSB140X_SLAVE_ID  0xD2
#define ISDBT_IQ_SWAP   0x01          // IQ Swap                    //this define will depend on tuner

#define TS_PARALLEL_OUTPUT 1
#if TS_PARALLEL_OUTPUT
#define ISDBT_TS_SERIAL		  0x00
#else
#define ISDBT_TS_SERIAL		  0x01
#endif
// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_isdbt_ts_clk_divnum
// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#define ISDBT_TS_CLK_DIVNUM_SERIAL_MODE   4
#define ISDBT_TS_CLK_DIVNUM_PARALLEL_MODE  31

#if (ISDBT_TS_SERIAL)
#define ISDBT_TS_CLK_DIVNUM	  ISDBT_TS_CLK_DIVNUM_SERIAL_MODE
#else
#define ISDBT_TS_CLK_DIVNUM	  ISDBT_TS_CLK_DIVNUM_PARALLEL_MODE
#endif

//#define ISDBT_TS_DATA_SWAP  0x01  // TS data inversion
static MAPI_BOOL _bISDBT_TS_Data_Swap = MAPI_TRUE;
#define ISDBT_TS_CLK_INV      0x00  // TS clock inverseion          //dan note: this define will depend on main board
#define ISDBT_IFAGC_REF       0x38                                  //dan note: this define will depend on tuner

//VIF handler patch for Tuning, or Tuning may lose channels
//Need VIF team's help to remove me.
#define VIF_HANDLER_PATCH         1
#define DMD_INIT_PARAM_LEN_MAX 256
#if VIF_HANDLER_PATCH
#ifdef __cplusplus
extern "C"
{
#endif
extern void DRV_VIF_Handler(MS_BOOL bAutoScan);
#ifdef __cplusplus
}
#endif
#endif


#if 0
#define DEMOD_DBG(fmt, arg...)           printf((char *)fmt, ##arg)
#define DEMOD_FLOW_DBG(fmt, arg...)      printf((char *)fmt, ##arg)
#define DEMOD_IFO(fmt, arg...)           printf((char *)fmt, ##arg)
#else
#define DEMOD_DBG(fmt, arg...)
#define DEMOD_FLOW_DBG(fmt, arg...)
#define DEMOD_IFO(fmt, arg...)
#endif

#define DBG_DEMOD_MSB(x)       //   x
#define DBG_DUMP_LOAD_DSP_TIME 0

#define MDrv_Timer_Delayms OS_DELAY_TASK
#define MDrv_Timer_GetTime0 OS_SYSTEM_TIME

MAPI_BOOL IIC_Bypass_Mode(MAPI_BOOL enable);

//#if (STB_ENABLE == 0)
//MAPI_U8 u8ATVDeviceBusy=0;
//#endif
MAPI_U8 u8DTVDeviceBusy=0;

MAPI_BOOL m_bSerialOut=MAPI_TRUE;

#if (STB_ENABLE == 0)
MAPI_U8 gVifTop = VIF_TOP;
MAPI_U8 gVifSoundSystem = 0;//(MAPI_U8)mapi_vif_datatype::E_VIF_SOUND_DK2;
MAPI_U8 gVifIfFreq = (MAPI_U8)E_IF_FREQ_3890;
#endif

MAPI_U8 u8MsbData[6];
MAPI_U8 gu8ChipRevId = 0;
MAPI_U8 gCalIdacCh0 = 0;
MAPI_U8 gCalIdacCh1 = 0;
//S_CMDPKTREG gsCmdPacket;
MAPI_U8 gu8DemoDynamicI2cAddress=0x32;//Default
MAPI_BOOL     FECLock = MAPI_FALSE;
MAPI_BOOL    gbTVAutoScanEn=MAPI_FALSE;//init value=MAPI_FALSE, follow with auto/manual scan
EN_DEVICE_DEMOD_TYPE m_enCurrentDemodulator_Type = E_DEVICE_DEMOD_NULL;

//@@++
IF_FREQ m_eIF_Freq = IF_FREQ_INVALID;

MAPI_U32 Tuner_TOP_Setting = 0xFF;
MAPI_U32 Tuner_Top_Setting_LPrime=0x05;
MAPI_U32 Tuner_Top_Setting_SECAM_VHF=0x04;
MAPI_U32 Tuner_Top_Setting_SECAM_UHF=0x03;
MAPI_U32 Tuner_Top_Setting_PAL_VHF=0x02;
MAPI_U32 Tuner_Top_Setting_PAL_UHF=0x01;

MAPI_U16 IfFreq = 0;
MAPI_BOOL IFDM_Initialized = FALSE;
MAPI_U8 Scanning_Active=0;
//@@++

MAPI_U16 g_u16_symbol_rate_list[] =
{
    1500, 2000, 2500, 3000, 3500,
    4000, 4500, 5000, 5500, 6000,
    6500, 7000,
};


#if 1
#define DemodCmd         0x110500L
#define DemodAdrL        0x110501L
#define DemodAdrH        0x110502L
#define DemodData        0x110503L
#define DemodCmdWrReg    0x01    // write register.
#define DemodCmdRdReg    0x02    // read register.

MAPI_BOOL MSB1400_Data_Swap_Mode(MAPI_BOOL enable)
{
    _bISDBT_TS_Data_Swap = enable;
    return MAPI_TRUE;
}

MAPI_U8 MDrv_T3VIF_ReadByte(MAPI_U32 u32Reg)
{
    #if 0
    MDrv_WriteByte(DemodAdrL, u32Reg&0xFF);
    MDrv_WriteByte(DemodAdrH, (u32Reg>>8)&0xFF);
    MDrv_WriteByte(DemodCmd, DemodCmdRdReg);
    usleep(100000);
    return MDrv_ReadByte(DemodData);
    #endif
    UNUSED(u32Reg);
    return 0;
}


void MDrv_T3VIF_WriteByte(MAPI_U32 u32Reg, MAPI_U8 u8Val)
{
    #if 0
    usleep(100000);
    MDrv_WriteByte(DemodAdrL, u32Reg&0xFF);
    MDrv_WriteByte(DemodAdrH, (u32Reg>>8)&0xFF);
    MDrv_WriteByte(DemodData, u8Val);
    MDrv_WriteByte(DemodCmd, DemodCmdWrReg);
    #endif
    UNUSED(u32Reg);
    UNUSED(u8Val);
}
#endif


//##########################################################################################################
//##########################################################################################################
//###############################  Public:Common Function Implementation ###################################
//##########################################################################################################
//##########################################################################################################

#if 0
MAPI_BOOL device_demodulator::DeviceDemodCreate()
{
    mapi_demodulator *pMapiDemod;


    pMapiDemod = mapi_interface::Get_mapi_demod();

    if (pMapiDemod == NULL)
    {
        printf("%s(),%d Init Fail\n",__func__,__LINE__);
        return MAPI_FALSE;
    }

    //Public:Common
    pMapiDemod->stMapiDemodulatorFun.Connect = Connect;
    pMapiDemod->stMapiDemodulatorFun.Disconnect = Disconnect;
    pMapiDemod->stMapiDemodulatorFun.Reset = Reset;
    pMapiDemod->stMapiDemodulatorFun.IIC_Bypass_Mode = IIC_Bypass_Mode;
    pMapiDemod->stMapiDemodulatorFun.Data_Swap_Mode = MSB1400_Data_Swap_Mode;
    pMapiDemod->stMapiDemodulatorFun.Power_On_Initialization = Power_On_Initialization;
    pMapiDemod->stMapiDemodulatorFun.Set_PowerOn = Set_PowerOn;
    pMapiDemod->stMapiDemodulatorFun.Set_PowerOff = Set_PowerOff;
    pMapiDemod->stMapiDemodulatorFun.Active = Active;
    pMapiDemod->stMapiDemodulatorFun.GetCurrentDemodulatorType = GetCurrentDemodulatorType;
    pMapiDemod->stMapiDemodulatorFun.SetCurrentDemodulatorType = SetCurrentDemodulatorType;
    pMapiDemod->stMapiDemodulatorFun.ExtendCmd = ExtendCmd;

#if (STB_ENABLE == 0)
    //Public:ATV
    pMapiDemod->stMapiDemodulatorFun.ATV_GetAFC_Distance = ATV_GetAFC_Distance;
    pMapiDemod->stMapiDemodulatorFun.ATV_SetVIF_SoundSystem = ATV_SetVIF_SoundSystem;
    pMapiDemod->stMapiDemodulatorFun.ATV_SetAudioSawFilter = ATV_SetAudioSawFilter;
    pMapiDemod->stMapiDemodulatorFun.ATV_SetVIF_IfFreq = ATV_SetVIF_IfFreq;
    pMapiDemod->stMapiDemodulatorFun.ATV_VIF_Init = ATV_VIF_Init;
    pMapiDemod->stMapiDemodulatorFun.ATV_VIF_Handler = ATV_VIF_Handler;
    pMapiDemod->stMapiDemodulatorFun.ATV_GetVIF_Type = ATV_GetVIF_Type;
	pMapiDemod->stMapiDemodulatorFun.ExtendCmd = ExtendCmd;
//@@++
	pMapiDemod->stMapiDemodulatorFun.ATV_GetIF = ATV_GetIF;
    pMapiDemod->stMapiDemodulatorFun.ATV_SetIF = ATV_SetIF;
	pMapiDemod->stMapiDemodulatorFun.ATV_GetIFAGC = ATV_GetIFAGC;
	pMapiDemod->stMapiDemodulatorFun.ATV_GetVifIfFreq = ATV_GetVifIfFreq;
	pMapiDemod->stMapiDemodulatorFun.ATV_Initialized = ATV_Initialized;
	pMapiDemod->stMapiDemodulatorFun.ATV_ScanningStatus = ATV_ScanningStatus;
	pMapiDemod->stMapiDemodulatorFun.ATV_SetPeakingParameters = ATV_SetPeakingParameters;
	pMapiDemod->stMapiDemodulatorFun.ATV_SetAGCParameters = ATV_SetAGCParameters;
    pMapiDemod->stMapiDemodulatorFun.ATV_SetAudioNotch = ATV_SetAudioNotch;
    pMapiDemod->stMapiDemodulatorFun.ATV_GetVIF_InitailValue = ATV_GetVIF_InitailValue;
//@@++
#endif

    //Public:DTV
    pMapiDemod->stMapiDemodulatorFun.DTV_SetFrequency = DTV_SetFrequency;

    pMapiDemod->stMapiDemodulatorFun.DTV_GetSNR = DTV_GetSNR;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetBER = DTV_GetBER;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetSignalQuality = DTV_GetSignalQuality;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetPostBER = DTV_GetPostBER;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetSignalStrength = DTV_GetSignalStrength;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetCellID = DTV_GetCellID;
    pMapiDemod->stMapiDemodulatorFun.DTV_Serial_Control = DTV_Serial_Control;
    pMapiDemod->stMapiDemodulatorFun.DTV_IsHierarchyOn = DTV_IsHierarchyOn;

    //dan add for integrate ISDBT utopia driver[begin]
    //Public:DTV-ISDB
    pMapiDemod->stMapiDemodulatorFun.DTV_ISDB_GetLockStatus = DTV_ISDB_GetLockStatus;
    //dan add for integrate ISDBT utopia driver[end]

    MDrv_DMD_PreInit();
    return MAPI_TRUE;
}
#endif

//dan add for integrate ISDBT utopia driver[begin]
#define resetDemodTime 50
#define waitFlashTime 50
//dan add for integrate ISDBT utopia driver[end]
MS_BOOL WriteBytes(MS_U16 u16SlaveCfg, MS_U8 uAddrCnt, MS_U8* pRegAddr, MS_U16 uSize, MS_U8* pData)
{
    return MDrv_HWI2C_WriteBytes(u16SlaveCfg, uAddrCnt, pRegAddr, uSize, pData);
}

MS_BOOL ReadBytes(MS_U16 u16SlaveCfg, MS_U8 uAddrCnt, MS_U8* pRegAddr, MS_U16 uSize, MS_U8* pData)
{
    return MDrv_HWI2C_ReadBytes(u16SlaveCfg, uAddrCnt, pRegAddr, uSize, pData);
}

extern MS_BOOL MDrv_Tuner_Init(void);
MAPI_BOOL Connect(EN_DEVICE_DEMOD_TYPE enDemodType)
{
//#if (STB_ENABLE == 0)
//    if ((u8ATVDeviceBusy==1)&&(enDemodType == E_DEVICE_DEMOD_ATV))
//    {
//        return MAPI_FALSE;
//    }
//#endif
    if ((u8DTVDeviceBusy==1)&&(enDemodType == E_DEVICE_DEMOD_ISDB))
    {
        return MAPI_FALSE;
    }

/*
#if (STB_ENABLE == 0)
    SarChannel eSARChannel = E_SAR_NC;
    SawArchitecture eSAWType = SystemInfo::GetInstance()->GetSAWType();

    if(eSAWType < DUAL_SAW || eSAWType >= SAW_NUMS)
    {
        printf("\r\n Warning!!!!! Abnormal SAW type");
        ASSERT(0);
    }
    eSARChannel = SystemInfo::GetInstance()->GetSARChannel();

    if (!((eSARChannel < E_SAR_MAX_NUMS) || (eSARChannel == E_SAR_NC)))
    {
        eSARChannel=E_SAR_NC;
        printf("\r\n Warning!!!!! Abnormal SAR channel");
        ASSERT(0);
    }
#endif
*/

    switch (enDemodType)
    {
//#if (STB_ENABLE == 0)
//        case E_DEVICE_DEMOD_ATV:
//            u8ATVDeviceBusy = 1;
//            u8DTVDeviceBusy = 0;
//            break;
//#endif
        //dan add for integrate ISDBT utopia driver[begin]
        case E_DEVICE_DEMOD_ISDB:
        {
            //mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);
            //mapi_gpio *gptr = mapi_gpio::GetGPIO_Dev(Demodulator_Reset);

            DMD_ISDBT_InitData sDMD_ISDBT_InitData;
            static MS_U8 u8DMD_ISDBT_InitExt[]={1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
            sDMD_ISDBT_InitData.u16IF_KHZ = 5000; // u16DtvIFFreqKHz;
            sDMD_ISDBT_InitData.bIQSwap = 1; // u8IqSwap;
            sDMD_ISDBT_InitData.u16ISDBTFECLockCheckTime = 2000;
            sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime = 500;
            sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt = u8DMD_ISDBT_InitExt;
            sDMD_ISDBT_InitData.u16AgcReferenceValue = ISDBT_IFAGC_REF;
            sDMD_ISDBT_InitData.bIsUseSspiLoadCode=0; //make it load code by hw i2c

            //TS[begin]
            //Parallel mode
            sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = ISDBT_TS_CLK_DIVNUM;
            sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = ISDBT_TS_CLK_INV;
            sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = (_bISDBT_TS_Data_Swap)?0x01:0x00;
            sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = ISDBT_TS_SERIAL;
            //Serial mode
            //sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = 4;
            //sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = 0;
            //sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = 0;
            //sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = 1;
            //TS[end]

            //I2C[begin]
            sDMD_ISDBT_InitData.u8I2CSlaveAddr = DEMOD_SLAVE_ID;//iptr->GetSlaveAddr();
            sDMD_ISDBT_InitData.u8I2CSlaveBus = 0;
            sDMD_ISDBT_InitData.bIsExtDemod = TRUE;
            //printf("u8I2CSlaveAddr:%d\n", sDMD_ISDBT_InitData.u8I2CSlaveAddr);

            //sDMD_ISDBT_InitData.I2C_WriteBytes = MApi_SWI2C_WriteBytes;   //dan note: must check I2C function
            //sDMD_ISDBT_InitData.I2C_ReadBytes = MApi_SWI2C_ReadBytes;     //dan note: must check I2C function
            sDMD_ISDBT_InitData.I2C_WriteBytes = WriteBytes;
            sDMD_ISDBT_InitData.I2C_ReadBytes = ReadBytes;
            //I2C[end]

/*
            if(gptr != NULL)
            {
                gptr->SetOff();
            }
*/
            mdrv_gpio_set_low(DEMODULATOR_RESET_GPIO);

            mdelay(resetDemodTime);

/*
            if(gptr != NULL)
            {
                gptr->SetOn();
            }
*/
            mdrv_gpio_set_high(DEMODULATOR_RESET_GPIO);

            mdelay(waitFlashTime);
            //MDrv_DMD_ISDBT_Initial_Hal_Interface();
            MDrv_DMD_ISDBT_Initial_External_Hal_Interface();
            MDrv_DMD_ISDBT_Init(&sDMD_ISDBT_InitData, sizeof(sDMD_ISDBT_InitData));
            u8DTVDeviceBusy = 1;
//#if (STB_ENABLE == 0)
//            u8ATVDeviceBusy = 0;
//#endif
        }
        break;
        //dan add for integrate ISDBT utopia driver[end]
        default:
           break;
    }

    m_enCurrentDemodulator_Type = enDemodType;

    IIC_Bypass_Mode(TRUE);
    if (MDrv_Tuner_Init() == FALSE)
    {
        IIC_Bypass_Mode(FALSE);
        printf("\n MXL603 INIT FAILED\n");
        return FALSE;
    }
    IIC_Bypass_Mode(FALSE);
    printf("\n MXL603 INIT OK\n");

    return TRUE;
}


MAPI_BOOL Disconnect(void)
{
    MAPI_BOOL bReturn=MAPI_FALSE;
    printf("device_demodulator_msb1400: Disconnect\n");
    u8DTVDeviceBusy = 0;
//#if (STB_ENABLE == 0)
//    u8ATVDeviceBusy = 0;
//#endif
    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
      	 case E_DEVICE_DEMOD_ISDB:
            MDrv_DMD_ISDBT_Exit();
            break;
         //dan add for integrate ISDBT utopia driver[end]
         default:
            break;
    }
    return bReturn;
}



void Reset()
{
    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
      	case E_DEVICE_DEMOD_ISDB:
            MDrv_DMD_ISDBT_SetReset();
            break;
         //dan add for integrate ISDBT utopia driver[end]
         default:
            break;
    }
}

MAPI_BOOL IIC_Bypass_Mode(MAPI_BOOL enable)
{
    return MDrv_DMD_ISDBT_IIC_BYPASS_MODE(enable);
}

MAPI_BOOL Power_On_Initialization(void)
{
    MAPI_BOOL bReturn = MAPI_TRUE;

    switch (m_enCurrentDemodulator_Type)
    {
//#if (STB_ENABLE == 0)
//        case E_DEVICE_DEMOD_ATV:
//                ATV_SetVIF_IfFreq(38900L);
//            break;
//#endif
        //dan add for integrate ISDBT utopia driver[begin]
	case E_DEVICE_DEMOD_ISDB:
            // _UTOPIA
            break;
       //dan add for integrate ISDBT utopia driver[end]
       default:
            //ASSERT(0);
            printf("invalidate demod type:%d\n", m_enCurrentDemodulator_Type);
            break;
    }
    return bReturn;
}

MAPI_BOOL Set_PowerOn(void)
{


    return MAPI_TRUE;
}

MAPI_BOOL Set_PowerOff(void)
{


    return MAPI_TRUE;
}

MAPI_BOOL Active(MAPI_BOOL bEnable)
{
     MAPI_BOOL bReturn=MAPI_FALSE;
     printf("%s()\n",__func__);


     switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
        case E_DEVICE_DEMOD_ISDB:
            return MDrv_DMD_ISDBT_SetConfig(bEnable);
            break;
        //dan add for integrate ISDBT utopia driver[end]
         default:
            break;
    }

    return bReturn;
}

EN_DEVICE_DEMOD_TYPE GetCurrentDemodulatorType(void)
{
    return m_enCurrentDemodulator_Type;
}


MAPI_BOOL SetCurrentDemodulatorType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    if(enDemodType == m_enCurrentDemodulator_Type)
    {
        return MAPI_TRUE;
    }

    m_enCurrentDemodulator_Type = enDemodType;
    return MAPI_TRUE;
}


MAPI_BOOL ExtendCmd(MAPI_U8 SubCmd, MAPI_U32 u32Param1, MAPI_U32 u32Param2, void *pvParam3)
{

   //1 => Write , 2 =>  Read
    if (SubCmd == 1)
    {//1 => Write
        printf("---> Write VIF Register...\n");
        //MDrv_T3VIF_WriteByte((MAPI_U32)u32Param1,(U8)u32Param2);
        //return MAPI_TRUE;
        //return MDrv_SYS_DMD_VD_MBX_WriteReg((MS_U32) u32Param1,(MS_U8) u32Param2);
        return MDrv_DMD_WriteReg((MS_U32) u32Param1,(MS_U8) u32Param2);
    }

    if (SubCmd == 2)
    { //2 =>  Read

        //U8 *pU8Data;

        printf("---> Read VIF Register...\n");
        //pU8Data = (U8 *)pvParam3;
        //*pU8Data = MDrv_T3VIF_ReadByte((MAPI_U32)u32Param1);
        //return MAPI_TRUE;
        //return MDrv_SYS_DMD_VD_MBX_ReadReg((MS_U16)u32Param1,(MS_U8*)pvParam3);
        return MDrv_DMD_ReadReg((MS_U32)u32Param1,(MS_U8*)pvParam3);
    }

#if (STB_ENABLE == 0)
    if (SubCmd ==5)
    {//Read VIF_CR_KI1 from VIF define
        MS_U8 *pU8Data;
        pU8Data = (MS_U8 *)pvParam3;
        *pU8Data = VIF_CR_KI1;
        return MAPI_TRUE;
    }

    if (SubCmd ==6)
    {//Read VIF_CR_KP1 from VIF define
        MS_U8 *pU8Data;
        pU8Data = (MS_U8 *)pvParam3;
        *pU8Data = VIF_CR_KP1;
        return MAPI_TRUE;
    }
#endif

    // write symbol rate list.
    if(SubCmd == 0x50)
    {
        MS_U8 index = 0;
        MS_U8 length = (MAPI_U8)u32Param1;
        MS_U16 *pArrary = (MS_U16*)pvParam3;


        if(length > (sizeof(g_u16_symbol_rate_list)/sizeof(MS_U16)))
          return MAPI_FALSE;

        for(index = 0; index < (sizeof(g_u16_symbol_rate_list)/sizeof(MS_U16)); index++)
          g_u16_symbol_rate_list[index] = 0;

        index = 0;

        for(index = 0; index < length; index++)
          g_u16_symbol_rate_list[index] = pArrary[index];

        return MAPI_TRUE;
    }

    // read symbol rate list.
    if(SubCmd == 0x51)
    {

        // return format, pArrary,
        // - pArrary[0], total symbol rate list number
        // - pArrary[1], 1st symbol rate.
        // - pArrary[2], 2nd symbol rate.
        // - ......
        // - pArrary[N], (N-1)th symbol rate.
        // available_length,
        // - pArrary length

        MS_U8 index = 0;

        // available_length = 1 + x, x is maximam allocated slot for symbol rate.
        MS_U8  available_length = (MAPI_U8)u32Param1;
        MS_U8  slot_available_length = available_length - 1;
        MS_U16 *pArrary = (MS_U16*)pvParam3;


        if(pArrary == NULL)
          return MAPI_FALSE;

        if(available_length == 0)
          return MAPI_FALSE;

        if( slot_available_length > (sizeof(g_u16_symbol_rate_list)/sizeof(MS_U16)) )
          slot_available_length = sizeof(g_u16_symbol_rate_list)/sizeof(MS_U16);

        for(index = 0;index < slot_available_length; index++)
        {
          pArrary[index + 1] = g_u16_symbol_rate_list[index];
        }

        pArrary[0] = (MS_U16)index;

        return MAPI_TRUE;

    }


    if(SubCmd == 0x80)  // temp solution, to turn on/off IF AGC
    {
        if(u32Param1 == 1)  // turn on
        {
            MDrv_DMD_RFAGC_Tristate(TRUE);
            MDrv_DMD_IFAGC_Tristate(FALSE);
        }
        else if(u32Param1 == 0)     // turn off
        {
            MDrv_DMD_RFAGC_Tristate(TRUE);
            MDrv_DMD_IFAGC_Tristate(TRUE);
        }
    }

    if(SubCmd == 0x82)  // to turn on/off null packet insert
    {
        MS_U8 u8Value;
        if(u32Param1 == 1)  // null packet on
        {
            MDrv_SYS_DMD_VD_MBX_ReadReg(0x1F20, &u8Value);
            MDrv_SYS_DMD_VD_MBX_WriteReg(0x1F20, u8Value | 0x04);
        }
        else if(u32Param1 == 0)     // null packet off
        {
            MDrv_SYS_DMD_VD_MBX_ReadReg(0x1F20, &u8Value);
            MDrv_SYS_DMD_VD_MBX_WriteReg(0x1F20, u8Value & (~0x04));
        }
    }

    return MAPI_TRUE;
}



//##########################################################################################################
//##########################################################################################################
//########################################  Public:ATV VIF Implementation ##################################
//##########################################################################################################
//##########################################################################################################
#if 0 //ATV functions
#if (STB_ENABLE == 0)
AFC ATV_GetAFC_Distance(void)
{

     MAPI_U8 u8Value;

    u8Value=mapi_interface::Get_mapi_vif()->Read_CR_LOCK_STATUS();

    if (!(u8Value&0x01))
    {
        DEMOD_DBG("KKK: E_AFC_OUT_OF_AFCWIN \n");

        return E_AFC_OUT_OF_AFCWIN;
     }

    u8Value=mapi_interface::Get_mapi_vif()->Read_CR_FOE();//For Eris one byte, AFC_FOE=register address

   // VIFDBG(printf("CR_FOE=%bx ",u8Value));

// FREQ_STEP_62_5KHz
    switch(u8Value)
    {
    case 0x00:
    case 0x01:
        return E_AFC_PLUS_12p5KHz;//afcPLUS_12p5KHz;
    case 0x02:
        return E_AFC_PLUS_37p5KHz;//afcPLUS_37p5KHz;
    case 0x03:
    case 0x04:
        return E_AFC_PLUS_62p5KHz;//afcPLUS_62p5KHz;
    case 0x05:
        return E_AFC_PLUS_87p5KHz;//afcPLUS_87p5KHz;
    case 0x06:
    case 0x07:
        return E_AFC_PLUS_112p5KHz;//afcPLUS_112p5KHz;
    case 0x08:
        return E_AFC_PLUS_137p5KHz;//afcPLUS_137p5KHz;
    case 0x09:
    case 0x0A:
        return E_AFC_PLUS_162p5KHz;//afcPLUS_162p5KHz;
    case 0x0B:
    case 0x0C:
        return E_AFC_ABOVE_PLUS_187p5KHz;//afcABOVE_PLUS_187p5KHz;

    case 0xFF:
        return E_AFC_MINUS_12p5KHz;//afcMINUS_12p5KHz;
    case 0xFE:
        return E_AFC_MINUS_37p5KHz;//afcMINUS_37p5KHz;
    case 0xFD:
    case 0xFC:
        return E_AFC_MINUS_62p5KHz;//afcMINUS_62p5KHz;
    case 0xFB:
        return E_AFC_MINUS_87p5KHz;//afcMINUS_87p5KHz;
    case 0xFA:
    case 0xF9:
        return E_AFC_MINUS_112p5KHz;//afcMINUS_112p5KHz;
    case 0xF8:
        return E_AFC_MINUS_137p5KHz;//afcMINUS_137p5KHz;
    case 0xF7:
    case 0xF6:
        return E_AFC_MINUS_162p5KHz;//afcMINUS_162p5KHz;
    case 0xF5:
    case 0xF4:
        return E_AFC_BELOW_MINUS_187p5KHz;//afcBELOW_MINUS_187p5KHz;
    default:
        break;
    }

    if(u8Value >= 0x0C && u8Value <= 0x40)
    {
        return E_AFC_ABOVE_PLUS_187p5KHz;//afcABOVE_PLUS_187p5KHz;
    }
    if(u8Value >= 0xC0 && u8Value <= 0xF4)
    {
        return E_AFC_BELOW_MINUS_187p5KHz;//afcBELOW_MINUS_187p5KHz;
    }

    return E_AFC_OUT_OF_AFCWIN;
}

MAPI_BOOL ATV_SetVIF_SoundSystem(DEMOD_AUDIOSTANDARD_TYPE_ eIF_Freq)//B-ok
{
    DBG_DEMOD_MSB(printf("\r\n    Set SoundSystem:%x\n", (int)eIF_Freq));
    //DRV_VIF_SetSoundSystem((VIFSoundSystem)5);
    //return MAPI_TRUE;

    ATV_SetAudioSawFilter(SAW_FILTER_OTHERS_MODE);

    switch(eIF_Freq)
    {
        case E_DEMOD_AUDIOSTANDARD_BG_:
        case E_DEMOD_AUDIOSTANDARD_BG_A2_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_B_STEREO_A2);
            break;
        case E_DEMOD_AUDIOSTANDARD_BG_NICAM_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_B_MONO_NICAM);
            break;
        case E_DEMOD_AUDIOSTANDARD_I_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_I);
            break;
        case E_DEMOD_AUDIOSTANDARD_DK1_A2_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_DK1_STEREO_A2);
            break;
        case E_DEMOD_AUDIOSTANDARD_DK_:
        case E_DEMOD_AUDIOSTANDARD_DK2_A2_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_DK2_STEREO_A2);
            break;
        case E_DEMOD_AUDIOSTANDARD_DK3_A2_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_DK3_STEREO_A2);
            break;
        case E_DEMOD_AUDIOSTANDARD_DK_NICAM_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_DK_MONO_NICAM);
            break;
        case E_DEMOD_AUDIOSTANDARD_L_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_L);
            break;
        case E_DEMOD_AUDIOSTANDARD_LP_:
            ATV_SetAudioSawFilter(SAW_FILTER_SECAML_MODE);
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_LL);
            break;
        case E_DEMOD_AUDIOSTANDARD_M_:
        case E_DEMOD_AUDIOSTANDARD_M_BTSC_:
        case E_DEMOD_AUDIOSTANDARD_M_A2_:
        case E_DEMOD_AUDIOSTANDARD_M_EIA_J_:
            mapi_interface::Get_mapi_vif()->SetSoundSystem(mapi_vif_datatype::E_VIF_SOUND_MN);
            break;

        default:
            break;
    }

    return MAPI_TRUE;

}

//@@++
IF_FREQ ATV_GetIF(void)
{
	return m_eIF_Freq;
}

MAPI_BOOL ATV_SetIF(IF_FREQ eIF_Freq)
{
    if(m_eIF_Freq == eIF_Freq)
    {
        return MAPI_TRUE;
    }

    m_eIF_Freq = eIF_Freq;
    return MAPI_TRUE;
}

MAPI_U32 ATV_GetIFAGC(void)
{
	return Tuner_TOP_Setting;
}

MAPI_U16 ATV_GetVifIfFreq(void)
{
	return IfFreq;
}

MAPI_BOOL ATV_Initialized(void)
{
	return IFDM_Initialized;
}

MAPI_U8 ATV_ScanningStatus(MAPI_BOOL status)
{
    DBG_DEMOD_MSB(printf(" >> MDrv_IFDM_ScanningStatus "));
    if (status)
    {
        DBG_DEMOD_MSB(printf(" Active \r\n"));
        Scanning_Active=1;
    }
    else
    {
        DBG_DEMOD_MSB(printf(" Passive \r\n"));
        Scanning_Active=0;
    }
	return Scanning_Active;
}

MAPI_BOOL ATV_SetPeakingParameters(RFBAND eBand)
{
		mapi_vif_datatype::stVIFUserFilter VIF_UserFilter;

		memset((MAPI_U8*)&VIF_UserFilter, 0, sizeof(mapi_vif_datatype::stVIFUserFilter));

		switch(eBand)
		{
			case E_RFBAND_VHF_LOW:
				mapi_interface::Get_mapi_vif()->SetFreqBand(mapi_vif_datatype::E_FREQ_VHF_L);
				if((m_eIF_Freq==IF_FREQ_B)||(m_eIF_Freq==IF_FREQ_G))
				{
					//VIF_InitData.VifClampgainGainOvNegative=0X0600;
					VIF_UserFilter.VifUserPeakingFilterSelect=(m_eIF_Freq==IF_FREQ_B)?mapi_vif_datatype::PK_B_VHF_L:mapi_vif_datatype::PK_GH_VHF_L;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::YC_B_VHF_L : mapi_vif_datatype::YC_GH_VHF_L;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::GP_B_VHF_L : mapi_vif_datatype::GP_GH_VHF_L;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
					VIF_UserFilter.VifSos32FilterC4  = 0x0200;
	            }
				else if(m_eIF_Freq==IF_FREQ_DK)
				{
					//VIF_InitData.VifClampgainGainOvNegative=0X0600;
					VIF_UserFilter.VifUserPeakingFilterSelect=mapi_vif_datatype::PK_DK_VHF_L;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;

                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_DK_VHF_L;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_DK_VHF_L;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
					VIF_UserFilter.VifSos32FilterC4  = 0x0200;
				}
				else if(m_eIF_Freq==IF_FREQ_I)
				{
					//VIF_InitData.VifClampgainGainOvNegative=0X0690;
					VIF_UserFilter.VifUserPeakingFilterSelect=mapi_vif_datatype::PK_I_VHF_L;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_I_VHF_L;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_I_VHF_L;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_L)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_L_VHF_L;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_L_VHF_L;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_L_VHF_L;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_L_PRIME)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_LL_VHF_L;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_LL_VHF_L;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_LL_VHF_L;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            break;

        case E_RFBAND_VHF_HIGH:
            mapi_interface::Get_mapi_vif()->SetFreqBand(mapi_vif_datatype::E_FREQ_VHF_H);
            if((m_eIF_Freq == IF_FREQ_B) || (m_eIF_Freq == IF_FREQ_G))
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::PK_B_VHF_H : mapi_vif_datatype::PK_GH_VHF_H;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::YC_B_VHF_H : mapi_vif_datatype::YC_GH_VHF_H;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::GP_B_VHF_H : mapi_vif_datatype::GP_GH_VHF_H;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_DK)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_DK_VHF_H;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_DK_VHF_H;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_DK_VHF_H;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_I)
            {
                //VIF_InitData.VifClampgainGainOvNegative=0X0690;
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_I_VHF_H;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_I_VHF_H;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_I_VHF_H;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_L)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_L_VHF_H;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_L_VHF_H;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_L_VHF_H;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_L_PRIME)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_LL_VHF_H;
                VIF_UserFilter.VifSos21FilterC0  = 0x02F8;
                VIF_UserFilter.VifSos21FilterC1  = 0x0620;
                VIF_UserFilter.VifSos21FilterC2  = 0x0207;
                VIF_UserFilter.VifSos21FilterC3  = 0x0508;
                VIF_UserFilter.VifSos21FilterC4  = 0x01DA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_LL_VHF_H;
                VIF_UserFilter.VifSos22FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos22FilterC1  = 0x06C4;
                VIF_UserFilter.VifSos22FilterC2  = 0x01F5;
                VIF_UserFilter.VifSos22FilterC3  = 0x0512;
                VIF_UserFilter.VifSos22FilterC4  = 0x0146;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_LL_VHF_H;
                VIF_UserFilter.VifSos31FilterC0  = 0x028B;
                VIF_UserFilter.VifSos31FilterC1  = 0x06A8;
                VIF_UserFilter.VifSos31FilterC2  = 0x0158;
                VIF_UserFilter.VifSos31FilterC3  = 0x0576;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02B8;
                VIF_UserFilter.VifSos32FilterC1  = 0x0713;
                VIF_UserFilter.VifSos32FilterC2  = 0x00ED;
                VIF_UserFilter.VifSos32FilterC3  = 0x0548;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;

            }
            break;

        case E_RFBAND_UHF:
            mapi_interface::Get_mapi_vif()->SetFreqBand(mapi_vif_datatype::E_FREQ_UHF);
            if((m_eIF_Freq == IF_FREQ_B) || (m_eIF_Freq == IF_FREQ_G))
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::PK_B_UHF : mapi_vif_datatype::PK_GH_UHF;
                VIF_UserFilter.VifSos21FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos21FilterC1  = 0x062D;
                VIF_UserFilter.VifSos21FilterC2  = 0x0209;
                VIF_UserFilter.VifSos21FilterC3  = 0x0512;
                VIF_UserFilter.VifSos21FilterC4  = 0x01CA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::YC_B_UHF : mapi_vif_datatype::YC_GH_UHF;
                VIF_UserFilter.VifSos22FilterC0  = 0x0291;
                VIF_UserFilter.VifSos22FilterC1  = 0x070A;
                VIF_UserFilter.VifSos22FilterC2  = 0x01D9;
                VIF_UserFilter.VifSos22FilterC3  = 0x056F;
                VIF_UserFilter.VifSos22FilterC4  = 0x011D;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = (m_eIF_Freq == IF_FREQ_B) ? mapi_vif_datatype::GP_B_UHF : mapi_vif_datatype::GP_GH_UHF;
                VIF_UserFilter.VifSos31FilterC0  = 0x02D3;
                VIF_UserFilter.VifSos31FilterC1  = 0x0685;
                VIF_UserFilter.VifSos31FilterC2  = 0x017B;
                VIF_UserFilter.VifSos31FilterC3  = 0x052D;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02E1;
                VIF_UserFilter.VifSos32FilterC1  = 0x06F7;
                VIF_UserFilter.VifSos32FilterC2  = 0x010A;
                VIF_UserFilter.VifSos32FilterC3  = 0x051F;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_DK)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_DK_UHF;
                VIF_UserFilter.VifSos21FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos21FilterC1  = 0x062D;
                VIF_UserFilter.VifSos21FilterC2  = 0x0209;
                VIF_UserFilter.VifSos21FilterC3  = 0x0512;
                VIF_UserFilter.VifSos21FilterC4  = 0x01CA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_DK_UHF;
                VIF_UserFilter.VifSos22FilterC0  = 0x0291;
                VIF_UserFilter.VifSos22FilterC1  = 0x070A;
                VIF_UserFilter.VifSos22FilterC2  = 0x01D9;
                VIF_UserFilter.VifSos22FilterC3  = 0x056F;
                VIF_UserFilter.VifSos22FilterC4  = 0x011D;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_DK_UHF;
                VIF_UserFilter.VifSos31FilterC0  = 0x02D3;
                VIF_UserFilter.VifSos31FilterC1  = 0x0685;
                VIF_UserFilter.VifSos31FilterC2  = 0x017B;
                VIF_UserFilter.VifSos31FilterC3  = 0x052D;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02E1;
                VIF_UserFilter.VifSos32FilterC1  = 0x06F7;
                VIF_UserFilter.VifSos32FilterC2  = 0x010A;
                VIF_UserFilter.VifSos32FilterC3  = 0x051F;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_I)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_I_UHF;
                VIF_UserFilter.VifSos21FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos21FilterC1  = 0x062D;
                VIF_UserFilter.VifSos21FilterC2  = 0x0209;
                VIF_UserFilter.VifSos21FilterC3  = 0x0512;
                VIF_UserFilter.VifSos21FilterC4  = 0x01CA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_I_UHF;
                VIF_UserFilter.VifSos22FilterC0  = 0x0291;
                VIF_UserFilter.VifSos22FilterC1  = 0x070A;
                VIF_UserFilter.VifSos22FilterC2  = 0x01D9;
                VIF_UserFilter.VifSos22FilterC3  = 0x056F;
                VIF_UserFilter.VifSos22FilterC4  = 0x011D;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_I_UHF;
                VIF_UserFilter.VifSos31FilterC0  = 0x02D3;
                VIF_UserFilter.VifSos31FilterC1  = 0x0685;
                VIF_UserFilter.VifSos31FilterC2  = 0x017B;
                VIF_UserFilter.VifSos31FilterC3  = 0x052D;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02E1;
                VIF_UserFilter.VifSos32FilterC1  = 0x06F7;
                VIF_UserFilter.VifSos32FilterC2  = 0x010A;
                VIF_UserFilter.VifSos32FilterC3  = 0x051F;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_L)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_L_UHF;
                VIF_UserFilter.VifSos21FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos21FilterC1  = 0x062D;
                VIF_UserFilter.VifSos21FilterC2  = 0x0209;
                VIF_UserFilter.VifSos21FilterC3  = 0x0512;
                VIF_UserFilter.VifSos21FilterC4  = 0x01CA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_L_UHF;
                VIF_UserFilter.VifSos22FilterC0  = 0x0291;
                VIF_UserFilter.VifSos22FilterC1  = 0x070A;
                VIF_UserFilter.VifSos22FilterC2  = 0x01D9;
                VIF_UserFilter.VifSos22FilterC3  = 0x056F;
                VIF_UserFilter.VifSos22FilterC4  = 0x011D;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_L_UHF;
                VIF_UserFilter.VifSos31FilterC0  = 0x02D3;
                VIF_UserFilter.VifSos31FilterC1  = 0x0685;
                VIF_UserFilter.VifSos31FilterC2  = 0x017B;
                VIF_UserFilter.VifSos31FilterC3  = 0x052D;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02E1;
                VIF_UserFilter.VifSos32FilterC1  = 0x06F7;
                VIF_UserFilter.VifSos32FilterC2  = 0x010A;
                VIF_UserFilter.VifSos32FilterC3  = 0x051F;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            else if(m_eIF_Freq == IF_FREQ_L_PRIME)
            {
                VIF_UserFilter.VifUserPeakingFilterSelect = mapi_vif_datatype::PK_LL_UHF;
                VIF_UserFilter.VifSos21FilterC0  = 0x02EE;
                VIF_UserFilter.VifSos21FilterC1  = 0x062D;
                VIF_UserFilter.VifSos21FilterC2  = 0x0209;
                VIF_UserFilter.VifSos21FilterC3  = 0x0512;
                VIF_UserFilter.VifSos21FilterC4  = 0x01CA;
                VIF_UserFilter.VifUserYcDelayFilterSelect = mapi_vif_datatype::YC_LL_UHF;
                VIF_UserFilter.VifSos22FilterC0  = 0x0291;
                VIF_UserFilter.VifSos22FilterC1  = 0x070A;
                VIF_UserFilter.VifSos22FilterC2  = 0x01D9;
                VIF_UserFilter.VifSos22FilterC3  = 0x056F;
                VIF_UserFilter.VifSos22FilterC4  = 0x011D;
                VIF_UserFilter.VifUserGroupDelayFilterSelect = mapi_vif_datatype::GP_LL_UHF;
                VIF_UserFilter.VifSos31FilterC0  = 0x02D3;
                VIF_UserFilter.VifSos31FilterC1  = 0x0685;
                VIF_UserFilter.VifSos31FilterC2  = 0x017B;
                VIF_UserFilter.VifSos31FilterC3  = 0x052D;
                VIF_UserFilter.VifSos31FilterC4  = 0x0200;
                VIF_UserFilter.VifSos32FilterC0  = 0x02E1;
                VIF_UserFilter.VifSos32FilterC1  = 0x06F7;
                VIF_UserFilter.VifSos32FilterC2  = 0x010A;
                VIF_UserFilter.VifSos32FilterC3  = 0x051F;
                VIF_UserFilter.VifSos32FilterC4  = 0x0200;
            }
            break;

        default:
            break;
    }
    mapi_interface::Get_mapi_vif()->VifSetParameter(mapi_vif_datatype::E_VIF_PARA_USER_FILTER, &VIF_UserFilter, sizeof(mapi_vif_datatype::stVIFUserFilter));

    return MAPI_TRUE;
}

MAPI_BOOL ATV_SetAudioNotch(void)
{
    // no use, notch will be set by sound system in mapi_vif::SetSoundSystem
    return MAPI_TRUE;
}

MAPI_BOOL ATV_SetAudioSawFilter(MAPI_U8 u8SawFilterMode)
{
    mapi_gpio *gptr = mapi_gpio::GetGPIO_Dev(SECAM_L_PRIME);

    DBG_DEMOD_MSB(printf(">>>J2: Enter ATV_SetAudioSawFilter() = 0x%x\n", u8SawFilterMode));

    if (gptr == NULL)
    {
        return MAPI_FALSE;
    }

    if(u8SawFilterMode > SAW_FILTER_OTHERS_MODE)
        return MAPI_FALSE;


    if(u8SawFilterMode == SAW_FILTER_SECAML_MODE)
    {
        gptr->SetOn();
        //printf("---> Set Saw = 1\n");
        DBG_DEMOD_MSB(printf(">>>J2: Set Saw = 1\n"));
    }
    else
    {
        gptr->SetOff();
        DBG_DEMOD_MSB(printf(">>>J2: Set Saw = 0\n"));
    }

    return MAPI_TRUE;
}

MAPI_BOOL ATV_SetAGCParameters(EN_EMC_IF_AGC_SETTINGS par,MAPI_U32 val)
{
    switch (par)
    {
        case EMC_IF_AGC_LPRIME_SETTING:
            Tuner_Top_Setting_LPrime=val;
            DBG_DEMOD_MSB(printf("LPRIME AGC Setting %ld",Tuner_Top_Setting_LPrime));
        break;
        case EMC_IF_AGC_SECAM_VHF_SETTING:
            Tuner_Top_Setting_SECAM_VHF=val;
            DBG_DEMOD_MSB(printf("SECAM VHF AGC Setting %ld \r\n",Tuner_Top_Setting_SECAM_VHF));
        break;
        case EMC_IF_AGC_SECAM_UHF_SETTING:
            Tuner_Top_Setting_SECAM_UHF=val;
            DBG_DEMOD_MSB(printf("SECAM UHF AGC Setting %ld \r\n",Tuner_Top_Setting_SECAM_UHF));
        break;
        case EMC_IF_AGC_PAL_VHF_SETTING:
            Tuner_Top_Setting_PAL_VHF=val;
            DBG_DEMOD_MSB(printf("PAL VHF  AGC Setting %ld \r\n",Tuner_Top_Setting_PAL_VHF));
        break;
        case EMC_IF_AGC_PAL_UHF_SETTING:
            Tuner_Top_Setting_PAL_UHF=val;
            DBG_DEMOD_MSB(printf("PAL UHF AGC Setting %ld \r\n",Tuner_Top_Setting_PAL_UHF));
        break;
        default :
            DBG_DEMOD_MSB(printf("???? AGC ERROR   \r\n"));
		break;

    }

    return MAPI_TRUE;
}
//@@++



//static MS_U32 mtime;
MAPI_BOOL ATV_SetVIF_IfFreq(MAPI_U16 u16IfFreq)
{
    mapi_vif_datatype::EN_IF_FREQ_TYPE eIF_FreqTmp;


    DBG_DEMOD_MSB(printf("\n\n>>>A7: Enter ATV_SetVIF_IfFreq() = 0x%x\r\n", u16IfFreq));


    if ((u16IfFreq == 33950L)||(u16IfFreq == 33900L)||(u16IfFreq == 33625L))
        eIF_FreqTmp = mapi_vif_datatype::E_IF_FREQ_3395;   // SECAM-L'
    else if (u16IfFreq == 38000L)
        eIF_FreqTmp = mapi_vif_datatype::E_IF_FREQ_3800;   // PAL
    else if (u16IfFreq == 38900L)
        eIF_FreqTmp = mapi_vif_datatype::E_IF_FREQ_3890;   // PAL
    else if (u16IfFreq == 39500L)
        eIF_FreqTmp = mapi_vif_datatype::E_IF_FREQ_3950;   // only for PAL-I
    else if (u16IfFreq == 45750L)
        eIF_FreqTmp = mapi_vif_datatype::E_IF_FREQ_4575;   // NTSC-M/N
    else if (u16IfFreq == 58750L)
        eIF_FreqTmp = mapi_vif_datatype::E_IF_FREQ_5875;   // NTSC-M/N
    else
        //eIF_FreqTmp = IF_FREQ_3890;   // PAL
        return MAPI_FALSE;


    //printf("MsVifIffre=%bX\n",eIF_FreqTmp);

    gVifIfFreq = (MAPI_U8)eIF_FreqTmp;
    DBG_DEMOD_MSB(printf("MsVifIffre=%d\n",(int)eIF_FreqTmp));
    mapi_interface::Get_mapi_vif()->SetIfFreq(eIF_FreqTmp);

    #if 0//unuseful codes.
    {
        gsCmdPacket.cmd_code = CMD_VIF_CTRL;
        gsCmdPacket.param[0] = SET_IF_FREQ;
        gsCmdPacket.param[pc_if_freq] = eIF_FreqTmp;

        Cmd_Packet_Send(&gsCmdPacket, VIF_PARAM_MAX_NUM);
    }


    if ((u16IfFreq == 33950L)||(u16IfFreq == 33900L)||(u16IfFreq == 33625L))
        ATV_SetAudioSawFilter(VIF_FLAT_SAW);
    else
        ATV_SetAudioSawFilter(VIF_FLAT_SAW);
    #endif


    if((u16IfFreq == 33950L) || (u16IfFreq == 33900L) || (u16IfFreq == 33625L))
        ATV_SetAudioSawFilter(SAW_FILTER_SECAML_MODE);
    else
        ATV_SetAudioSawFilter(SAW_FILTER_OTHERS_MODE);

    #if VIF_HANDLER_PATCH
    //printf("\ndiff time= %lu\n",mapi_interface::Get_mapi_system()->TimeDiffFromNow0(mtime));
    //mtime=mapi_interface::Get_mapi_system()->GetTime0();
    DRV_VIF_Handler(true);
    #else
    mapi_interface::Get_mapi_vif()->Handler(true);
    #endif
    return MAPI_TRUE;

}


MAPI_BOOL ATV_GetVIF_InitailValue(mapi_vif_datatype::stVIFInitialIn *VIFInitialIn_inst)
{
    //mapi_vif_datatype::stVIFInitialIn VIFInitialIn_inst;
    SawArchitecture eSAWType;
    MAPI_U32 u32VifIfBaseFreq = 0;

    //printf("MDrv_VIF_Init\r\n");
    VIFInitialIn_inst->VifTop = VIF_TOP;
    //VIFInitialIn_inst->VifIfBaseFreq = VIF_IF_BASE_FREQ;
    //VIFInitialIn_inst->VifTunerStepSize = VIF_TUNER_STEP_SIZE;

    mapi_tuner* pTuner = mapi_interface::Get_mapi_pcb()->GetAtvTuner(0);
    ASSERT(pTuner);

    pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_IF_FREQ, 0, 0, &u32VifIfBaseFreq);
    VIFInitialIn_inst->VifIfBaseFreq = ((u32VifIfBaseFreq == 38000L)? IF_FREQ_38_00 : ((u32VifIfBaseFreq == 45750L)? IF_FREQ_45_75 : IF_FREQ_38_90));
    pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_FREQ_STEP, 0, 0, &VIFInitialIn_inst->VifTunerStepSize);

    eSAWType = SystemInfo::GetInstance()->GetSAWType();

    VIFInitialIn_inst->VifSawArch = eSAWType;

    VIFInitialIn_inst->VifAgcRefNegative = VIF_AGC_REF_NEGATIVE;
    VIFInitialIn_inst->VifDagc1GainOv = VIF_DAGC1_GAIN_OV;
    VIFInitialIn_inst->VifDagc2GainOv = VIF_DAGC2_GAIN_OV;
    VIFInitialIn_inst->VifClampgainGainOvNegative = VIF_CLAMPGAIN_GAIN_OV_NEGATIVE;
    VIFInitialIn_inst->VifClampgainGainOvPositive = VIF_CLAMPGAIN_GAIN_OV_POSITIVE;

    VIFInitialIn_inst->VifVgaMaximum = VIF_VGA_MAXIMUM;
    VIFInitialIn_inst->VifVgaMinimum = VIF_VGA_MINIMUM;
    VIFInitialIn_inst->GainDistributionThr = GAIN_DISTRIBUTION_THR;

    VIFInitialIn_inst->VifAgcVgaBase=VIF_AGC_VGA_BASE;
    VIFInitialIn_inst->VifAgcVgaOffs=VIF_AGC_VGA_OFFS;

    VIFInitialIn_inst->VifAgcRefPositive = VIF_AGC_REF_POSITIVE;
    VIFInitialIn_inst->VifDagc1Ref = VIF_DAGC1_REF;
    VIFInitialIn_inst->VifDagc2Ref = VIF_DAGC2_REF;
    VIFInitialIn_inst->VifCrKf1=VIF_CR_KF1;
    VIFInitialIn_inst->VifCrKp1=VIF_CR_KP1;
    VIFInitialIn_inst->VifCrKi1=VIF_CR_KI1;
    VIFInitialIn_inst->VifCrKp2=VIF_CR_KP2;
    VIFInitialIn_inst->VifCrKi2=VIF_CR_KI2;

    VIFInitialIn_inst->VifCrKp = VIF_CR_KP;
    VIFInitialIn_inst->VifCrKi = VIF_CR_KI;
    VIFInitialIn_inst->VifCrLockThr = VIF_CR_LOCK_THR;

    VIFInitialIn_inst->VifCrThr= VIF_CR_THR;
    VIFInitialIn_inst->VifCrLockNum=VIF_CR_LOCK_NUM;
    VIFInitialIn_inst->VifCrUnlockNum=VIF_CR_UNLOCK_NUM;
    VIFInitialIn_inst->VifCrPdErrMax=VIF_CR_PD_ERR_MAX;
    VIFInitialIn_inst->VifCrLockLeakySel=VIF_CR_LOCK_LEAKY_SEL;
    VIFInitialIn_inst->VifCrPdX2=VIF_CR_PD_X2;
    VIFInitialIn_inst->VifCrLpfSel=VIF_CR_LPF_SEL;
    VIFInitialIn_inst->VifCrPdModeSel=VIF_CR_PD_MODE_SEL;
    VIFInitialIn_inst->VifCrKpKiAdjust=VIF_CR_KP_KI_ADJUST;
    VIFInitialIn_inst->VifCrKpKiAdjustGear=VIF_CR_KP_KI_ADJUST_GEAR;
    VIFInitialIn_inst->VifCrKpKiAdjustThr1=VIF_CR_KP_KI_ADJUST_THR1;
    VIFInitialIn_inst->VifCrKpKiAdjustThr2=VIF_CR_KP_KI_ADJUST_THR2;
    VIFInitialIn_inst->VifCrKpKiAdjustThr3=VIF_CR_KP_KI_ADJUST_THR3;
    VIFInitialIn_inst->VifDynamicTopAdjust=VIF_DYNAMIC_TOP_ADJUST;
    VIFInitialIn_inst->VifDynamicTopMin=VIF_DYNAMIC_TOP_MIN;
    VIFInitialIn_inst->VifAmHumDetection=VIF_AM_HUM_DETECTION;
    VIFInitialIn_inst->VifClampgainClampSel=VIF_CLAMPGAIN_CLAMP_SEL;
    VIFInitialIn_inst->VifClampgainSyncbottRef=VIF_CLAMPGAIN_SYNCBOTT_REF;
    VIFInitialIn_inst->VifClampgainSyncheightRef=VIF_CLAMPGAIN_SYNCHEIGHT_REF;
    VIFInitialIn_inst->VifClampgainKc=VIF_CLAMPGAIN_KC;
    VIFInitialIn_inst->VifClampgainKg=VIF_CLAMPGAIN_KG;
    VIFInitialIn_inst->VifClampgainClampOren=VIF_CLAMPGAIN_CLAMP_OREN;
    VIFInitialIn_inst->VifClampgainGainOren=VIF_CLAMPGAIN_GAIN_OREN;
    VIFInitialIn_inst->VifClampgainClampOvNegative=VIF_CLAMPGAIN_CLAMP_OV_NEGATIVE;
    VIFInitialIn_inst->VifClampgainClampOvPositive=VIF_CLAMPGAIN_CLAMP_OV_POSITIVE;
    VIFInitialIn_inst->VifClampgainClampMin=VIF_CLAMPGAIN_CLAMP_MIN;
    VIFInitialIn_inst->VifClampgainClampMax=VIF_CLAMPGAIN_CLAMP_MAX;
    VIFInitialIn_inst->VifClampgainGainMin=VIF_CLAMPGAIN_GAIN_MIN;
    VIFInitialIn_inst->VifClampgainGainMax=VIF_CLAMPGAIN_GAIN_MAX;
    VIFInitialIn_inst->VifClampgainPorchCnt=VIF_CLAMPGAIN_PORCH_CNT;

    VIFInitialIn_inst->VifPeakingFilterB_VHF_L=VIF_PEAKING_FILTER_B_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterB_VHF_L=VIF_YC_DELAY_FILTER_B_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterB_VHF_L=VIF_GROUP_DELAY_FILTER_B_VHF_L;
    VIFInitialIn_inst->VifPeakingFilterGH_VHF_L=VIF_PEAKING_FILTER_GH_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterGH_VHF_L=VIF_YC_DELAY_FILTER_GH_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterGH_VHF_L=VIF_GROUP_DELAY_FILTER_GH_VHF_L;
    VIFInitialIn_inst->VifPeakingFilterDK_VHF_L=VIF_PEAKING_FILTER_DK_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterDK_VHF_L=VIF_YC_DELAY_FILTER_DK_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterDK_VHF_L=VIF_GROUP_DELAY_FILTER_DK_VHF_L;
    VIFInitialIn_inst->VifPeakingFilterI_VHF_L=VIF_PEAKING_FILTER_I_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterI_VHF_L=VIF_YC_DELAY_FILTER_I_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterI_VHF_L=VIF_GROUP_DELAY_FILTER_I_VHF_L;
    VIFInitialIn_inst->VifPeakingFilterL_VHF_L=VIF_PEAKING_FILTER_L_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterL_VHF_L=VIF_YC_DELAY_FILTER_L_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterL_VHF_L=VIF_GROUP_DELAY_FILTER_L_VHF_L;
    VIFInitialIn_inst->VifPeakingFilterLL_VHF_L=VIF_PEAKING_FILTER_LL_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterLL_VHF_L=VIF_YC_DELAY_FILTER_LL_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterLL_VHF_L=VIF_GROUP_DELAY_FILTER_LL_VHF_L;
    VIFInitialIn_inst->VifPeakingFilterMN_VHF_L=VIF_PEAKING_FILTER_MN_VHF_L;
    VIFInitialIn_inst->VifYcDelayFilterMN_VHF_L=VIF_YC_DELAY_FILTER_MN_VHF_L;
    VIFInitialIn_inst->VifGroupDelayFilterMN_VHF_L=VIF_GROUP_DELAY_FILTER_MN_VHF_L;

    VIFInitialIn_inst->VifPeakingFilterB_VHF_H=VIF_PEAKING_FILTER_B_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterB_VHF_H=VIF_YC_DELAY_FILTER_B_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterB_VHF_H=VIF_GROUP_DELAY_FILTER_B_VHF_H;
    VIFInitialIn_inst->VifPeakingFilterGH_VHF_H=VIF_PEAKING_FILTER_GH_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterGH_VHF_H=VIF_YC_DELAY_FILTER_GH_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterGH_VHF_H=VIF_GROUP_DELAY_FILTER_GH_VHF_H;
    VIFInitialIn_inst->VifPeakingFilterDK_VHF_H=VIF_PEAKING_FILTER_DK_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterDK_VHF_H=VIF_YC_DELAY_FILTER_DK_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterDK_VHF_H=VIF_GROUP_DELAY_FILTER_DK_VHF_H;
    VIFInitialIn_inst->VifPeakingFilterI_VHF_H=VIF_PEAKING_FILTER_I_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterI_VHF_H=VIF_YC_DELAY_FILTER_I_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterI_VHF_H=VIF_GROUP_DELAY_FILTER_I_VHF_H;
    VIFInitialIn_inst->VifPeakingFilterL_VHF_H=VIF_PEAKING_FILTER_L_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterL_VHF_H=VIF_YC_DELAY_FILTER_L_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterL_VHF_H=VIF_GROUP_DELAY_FILTER_L_VHF_H;
    VIFInitialIn_inst->VifPeakingFilterLL_VHF_H=VIF_PEAKING_FILTER_LL_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterLL_VHF_H=VIF_YC_DELAY_FILTER_LL_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterLL_VHF_H=VIF_GROUP_DELAY_FILTER_LL_VHF_H;
    VIFInitialIn_inst->VifPeakingFilterMN_VHF_H=VIF_PEAKING_FILTER_MN_VHF_H;
    VIFInitialIn_inst->VifYcDelayFilterMN_VHF_H=VIF_YC_DELAY_FILTER_MN_VHF_H;
    VIFInitialIn_inst->VifGroupDelayFilterMN_VHF_H=VIF_GROUP_DELAY_FILTER_MN_VHF_H;

    VIFInitialIn_inst->VifPeakingFilterB_UHF=VIF_PEAKING_FILTER_B_UHF;
    VIFInitialIn_inst->VifYcDelayFilterB_UHF=VIF_YC_DELAY_FILTER_B_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterB_UHF=VIF_GROUP_DELAY_FILTER_B_UHF;
    VIFInitialIn_inst->VifPeakingFilterGH_UHF=VIF_PEAKING_FILTER_GH_UHF;
    VIFInitialIn_inst->VifYcDelayFilterGH_UHF=VIF_YC_DELAY_FILTER_GH_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterGH_UHF=VIF_GROUP_DELAY_FILTER_GH_UHF;
    VIFInitialIn_inst->VifPeakingFilterDK_UHF=VIF_PEAKING_FILTER_DK_UHF;
    VIFInitialIn_inst->VifYcDelayFilterDK_UHF=VIF_YC_DELAY_FILTER_DK_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterDK_UHF=VIF_GROUP_DELAY_FILTER_DK_UHF;
    VIFInitialIn_inst->VifPeakingFilterI_UHF=VIF_PEAKING_FILTER_I_UHF;
    VIFInitialIn_inst->VifYcDelayFilterI_UHF=VIF_YC_DELAY_FILTER_I_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterI_UHF=VIF_GROUP_DELAY_FILTER_I_UHF;
    VIFInitialIn_inst->VifPeakingFilterL_UHF=VIF_PEAKING_FILTER_L_UHF;
    VIFInitialIn_inst->VifYcDelayFilterL_UHF=VIF_YC_DELAY_FILTER_L_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterL_UHF=VIF_GROUP_DELAY_FILTER_L_UHF;
    VIFInitialIn_inst->VifPeakingFilterLL_UHF=VIF_PEAKING_FILTER_LL_UHF;
    VIFInitialIn_inst->VifYcDelayFilterLL_UHF=VIF_YC_DELAY_FILTER_LL_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterLL_UHF=VIF_GROUP_DELAY_FILTER_LL_UHF;
    VIFInitialIn_inst->VifPeakingFilterMN_UHF=VIF_PEAKING_FILTER_MN_UHF;
    VIFInitialIn_inst->VifYcDelayFilterMN_UHF=VIF_YC_DELAY_FILTER_MN_UHF;
    VIFInitialIn_inst->VifGroupDelayFilterMN_UHF=VIF_GROUP_DELAY_FILTER_MN_UHF;

    VIFInitialIn_inst->ChinaDescramblerBox = CHINA_DESCRAMBLER_BOX;
    VIFInitialIn_inst->VifDelayReduce= VIF_DELAY_REDUCE;
    VIFInitialIn_inst->VifOverModulation = VIF_OVER_MODULATION;

    VIFInitialIn_inst->VifOverModulationDetect = VIF_OM_DETECTOR;
    VIFInitialIn_inst->VifACIDetect = VIF_ACI_DETECTOR;
    VIFInitialIn_inst->VifACIAGCREF=VIF_ACI_AGC_REF;
    VIFInitialIn_inst->VifADCOverflowAGCREF=VIF_ADC_OVERFLOW_AGC_REF;
    VIFInitialIn_inst->VifChanelScanAGCREF=VIF_CHANEL_SCAN_AGC_REF;
    VIFInitialIn_inst->VifACIDetTHR1 = VIF_ACIDET_THR1;
    VIFInitialIn_inst->VifACIDetTHR2 = VIF_ACIDET_THR2;
    VIFInitialIn_inst->VifACIDetTHR3 = VIF_ACIDET_THR3;
    VIFInitialIn_inst->VifACIDetTHR4 = VIF_ACIDET_THR4;

    VIFInitialIn_inst->VifSos21FilterC0= VIF_SOS21_FILTER_C0;
    VIFInitialIn_inst->VifSos21FilterC1= VIF_SOS21_FILTER_C1;
    VIFInitialIn_inst->VifSos21FilterC2= VIF_SOS21_FILTER_C2;
    VIFInitialIn_inst->VifSos21FilterC3= VIF_SOS21_FILTER_C3;
    VIFInitialIn_inst->VifSos21FilterC4= VIF_SOS21_FILTER_C4;
    VIFInitialIn_inst->VifSos22FilterC0= VIF_SOS22_FILTER_C0;
    VIFInitialIn_inst->VifSos22FilterC1= VIF_SOS22_FILTER_C1;
    VIFInitialIn_inst->VifSos22FilterC2= VIF_SOS22_FILTER_C2;
    VIFInitialIn_inst->VifSos22FilterC3= VIF_SOS22_FILTER_C3;
    VIFInitialIn_inst->VifSos22FilterC4= VIF_SOS22_FILTER_C4;
    VIFInitialIn_inst->VifSos31FilterC0= VIF_SOS31_FILTER_C0;
    VIFInitialIn_inst->VifSos31FilterC1= VIF_SOS31_FILTER_C1;
    VIFInitialIn_inst->VifSos31FilterC2= VIF_SOS31_FILTER_C2;
    VIFInitialIn_inst->VifSos31FilterC3= VIF_SOS31_FILTER_C3;
    VIFInitialIn_inst->VifSos31FilterC4= VIF_SOS31_FILTER_C4;
    VIFInitialIn_inst->VifSos32FilterC0= VIF_SOS32_FILTER_C0;
    VIFInitialIn_inst->VifSos32FilterC1= VIF_SOS32_FILTER_C1;
    VIFInitialIn_inst->VifSos32FilterC2= VIF_SOS32_FILTER_C2;
    VIFInitialIn_inst->VifSos32FilterC3= VIF_SOS32_FILTER_C3;
    VIFInitialIn_inst->VifSos32FilterC4= VIF_SOS32_FILTER_C4;


    pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_VIF_TUNER_TYPE, 0, 0, &VIFInitialIn_inst->VifTunerType);
    pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_VIF_PARA, 0, 0, VIFInitialIn_inst);

    VIFInitialIn_inst->VifReserve = 0;


    MS_Factory_NS_VIF_SET NSVIFValue;
    MSrv_Control::GetMSrvSystemDatabase()->GetFactoryExtSetting((&NSVIFValue), EN_FACTORY_EXT_NSVIF);
    if(NSVIFValue.VifAsiaSignalOption == 1)
    {
        VIFInitialIn_inst->VifCrKf1=VIF_CR_KF1_ASIA;
        VIFInitialIn_inst->VifCrKp1=VIF_CR_KP1_ASIA;
        VIFInitialIn_inst->VifCrKi1=VIF_CR_KI1_ASIA;
        VIFInitialIn_inst->VifCrKp2=VIF_CR_KP2_ASIA;
        VIFInitialIn_inst->VifCrKi2=VIF_CR_KI2_ASIA;
        VIFInitialIn_inst->VifCrLpfSel=VIF_CR_LPF_SEL_ASIA;
        VIFInitialIn_inst->VifCrPdModeSel=VIF_CR_PD_MODE_SEL_ASIA;
        VIFInitialIn_inst->VifCrKpKiAdjustGear=VIF_CR_KP_KI_ADJUST_GEAR_ASIA;
        VIFInitialIn_inst->VifCrKpKiAdjustThr1=VIF_CR_KP_KI_ADJUST_THR1_ASIA;
        VIFInitialIn_inst->VifCrKpKiAdjustThr2=VIF_CR_KP_KI_ADJUST_THR2_ASIA;
        VIFInitialIn_inst->VifCrKpKiAdjustThr3=VIF_CR_KP_KI_ADJUST_THR3_ASIA;
        VIFInitialIn_inst->VifCrKp = VIF_CR_KP_ASIA;
        VIFInitialIn_inst->VifCrKi = VIF_CR_KI_ASIA;
        VIFInitialIn_inst->VifCrKpKiAdjust=VIF_CR_KP_KI_ADJUST_ASIA;
    }

#if 0
       printf("\r\n define VifTop:%x",(U16)VIFInitialIn_inst.VifTop);
       printf("\r\n define VIF_VGA_MAXIMUM:%x",(U16)VIFInitialIn_inst.VifVgaMaximum);
       printf("\r\n define GAIN_DISTRIBUTION_THR:%x",(U16)VIFInitialIn_inst.GainDistributionThr);
       printf("\r\n define VIF_AGC_VGA_BASE:%x",(U16)VIFInitialIn_inst.VifAgcVgaBase);
       printf("\r\n define CHINA_DESCRAMBLER_BOX:%x",(U16)VIFInitialIn_inst.ChinaDescramblerBox);
       printf("\r\n define VIF_CR_KP1:%x",(U16)VIFInitialIn_inst.VifCrKp1);
       printf("\r\n define VIF_CR_KI1:%x",(U16)VIFInitialIn_inst.VifCrKi1);
       printf("\r\n define VIF_CR_KP2:%x",(U16)VIFInitialIn_inst.VifCrKp2);
       printf("\r\n define VIF_CR_KI2:%x",(U16)VIFInitialIn_inst.VifCrKi2);
       printf("\r\n define VIF_CR_KP:%x",(U16)VIFInitialIn_inst.VifCrKp);
       printf("\r\n define VIF_CR_KI:%x",(U16)VIFInitialIn_inst.VifCrKi);
       printf("\r\n define VIF_CR_LOCK_THR:%x",(U16)VIFInitialIn_inst.VifCrLockThr);
       printf("\r\n define VIF_CR_THR:%x",(U16)VIFInitialIn_inst.VifCrKpKiAdjust);
       printf("\r\n define VIF_CR_KP_KI_ADJUST:%x",(U16)VIFInitialIn_inst.VifCrKpKiAdjust);
       printf("\r\n define VIF_DELAY_REDUCE:%x",(U16)VIFInitialIn_inst.VifDelayReduce);
       printf("\r\n define VIF_OVER_MODULATION:%x",(U16)VIFInitialIn_inst.VifOverModulation);
       printf("\r\n define VIF_CLAMPGAIN_CLAMP_OV_NEGATIVE:%x",(U16)VIFInitialIn_inst.VifClampgainClampOvNegative);
       printf("\r\n define VIF_ACI_AGC_REF:%x",(U16)VIFInitialIn_inst.VifACIAGCREF);
#endif


    return MAPI_TRUE;
}
MAPI_BOOL ATV_VIF_Init(void)
{
    //Init VIF Parameter

    mapi_vif_datatype::stVIFInitialIn VIFInitialIn_inst;

    m_eIF_Freq=IF_FREQ_INVALID;

    // Set VIF data to 0, before init VIF variable
    memset(&VIFInitialIn_inst, 0x00, sizeof(VIFInitialIn_inst));

    ATV_GetVIF_InitailValue(&VIFInitialIn_inst);

    //update data from factory setting
    MS_Factory_NS_VIF_SET NSVIFValue;
    MSrv_Control::GetMSrvSystemDatabase()->GetFactoryExtSetting((&NSVIFValue), EN_FACTORY_EXT_NSVIF);
    VIFInitialIn_inst.VifTop                    =NSVIFValue.VifTop;
    VIFInitialIn_inst.VifVgaMaximum             =NSVIFValue.VifVgaMaximum;
    VIFInitialIn_inst.GainDistributionThr       =NSVIFValue.GainDistributionThr;
    VIFInitialIn_inst.VifAgcVgaBase             =NSVIFValue.VifAgcVgaBase;
    VIFInitialIn_inst.ChinaDescramblerBox       =NSVIFValue.ChinaDescramblerBox;
    //VIFInitialIn_inst.VifCrKp1                  =NSVIFValue.VifCrKp1;
    //VIFInitialIn_inst.VifCrKi1                  =NSVIFValue.VifCrKi1;
    //VIFInitialIn_inst.VifCrKp2                  =NSVIFValue.VifCrKp2;
    //VIFInitialIn_inst.VifCrKi2                  =NSVIFValue.VifCrKi2;
    VIFInitialIn_inst.VifCrKp                   =NSVIFValue.VifCrKp;
    VIFInitialIn_inst.VifCrKi                   =NSVIFValue.VifCrKi;
    VIFInitialIn_inst.VifCrLockThr              =NSVIFValue.VifCrLockThr ;
    VIFInitialIn_inst.VifCrThr                  =NSVIFValue.VifCrThr;
    VIFInitialIn_inst.VifCrKpKiAdjust           =NSVIFValue.VifCrKpKiAdjust;
    VIFInitialIn_inst.VifDelayReduce            =NSVIFValue.VifDelayReduce;
    VIFInitialIn_inst.VifOverModulation         =NSVIFValue.VifOverModulation;
    VIFInitialIn_inst.VifClampgainClampOvNegative=NSVIFValue.VifClampgainClampOvNegative;
    //VIFInitialIn_inst.VifClampgainGainOvNegative  =NSVIFValue.VifClampgainGainOvNegative;
    VIFInitialIn_inst.VifACIAGCREF = NSVIFValue.VifACIAGCREF;

    mapi_interface::Get_mapi_vif()->Init(&VIFInitialIn_inst,sizeof(VIFInitialIn_inst));

    return MAPI_TRUE;
}

MAPI_BOOL ATV_VIF_Handler(MAPI_BOOL bSet)
{
//    static MAPI_U32 u32Temp=0;
//    printf(" VIF Handler ---> Counter =%d \n",(int)u32Temp++);
     mapi_interface::Get_mapi_vif()->Handler(bSet);
    return MAPI_TRUE;
}
#endif
#endif
//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ##################################
//##########################################################################################################
//##########################################################################################################


MAPI_BOOL DTV_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth)//, MAPI_BOOL bPalBG, MAPI_BOOL bLPsel)
{
    //dan add for integrate ISDBT utopia driver[begin]
    switch (m_enCurrentDemodulator_Type)
    {
        case E_DEVICE_DEMOD_ISDB:
        {
            MDrv_DMD_ISDBT_SetConfig(ENABLE);
	 }
        break;
	 default:
	 break;
    }
    //dan add for integrate ISDBT utopia driver[end]
    return MAPI_TRUE;

}

EN_FRONTEND_SIGNAL_CONDITION DTV_GetSNR(void)
{
    switch (m_enCurrentDemodulator_Type)
    {
         default:
            break;
    }
    return E_FE_SIGNAL_NO;

}

MAPI_U32 DTV_GetBER(void)
{

    return 0;

}

MAPI_BOOL DTV_GetPreBER(float *p_preBer)
{
    EN_ISDBT_Layer 	LayerIndex;
    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
        case E_DEVICE_DEMOD_ISDB:
	     LayerIndex = E_ISDBT_Layer_A;
	     MDrv_DMD_ISDBT_GetPreViterbiBer(LayerIndex, p_preBer);
            //MDrv_DMD_ISDBT_GetPreViterbiBer(E_ISDBT_Layer_A, p_preBer);
            //MDrv_DMD_ISDBT_GetPreViterbiBer(E_ISDBT_Layer_B, p_preBer);
            //MDrv_DMD_ISDBT_GetPreViterbiBer(E_ISDBT_Layer_C, p_preBer);
            break;
        //dan add for integrate ISDBT utopia driver[end]
        default:
            break;

    }
    return MAPI_TRUE;
}

MAPI_BOOL DTV_GetPostBER(float *p_postBer)
{
    EN_ISDBT_Layer 	LayerIndex;
    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
        case E_DEVICE_DEMOD_ISDB:
	     LayerIndex = E_ISDBT_Layer_A;
	     MDrv_DMD_ISDBT_GetPostViterbiBer(LayerIndex, p_postBer);
            //MDrv_DMD_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_A, p_postBer); // _UTOPIA
            //MDrv_DMD_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_B, p_postBer); // _UTOPIA
            //MDrv_DMD_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_C, p_postBer); // _UTOPIA
            break;
        //dan add for integrate ISDBT utopia driver[end]
        default:
            break;
    }
    return MAPI_TRUE;
}

MAPI_BOOL DTV_GetPacketErr(void)
{
    MAPI_U16 pktErrA, pktErrB, pktErrC;
    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
        case E_DEVICE_DEMOD_ISDB:
            MDrv_DMD_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_A, &pktErrA); // _UTOPIA
            MDrv_DMD_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_B, &pktErrB); // _UTOPIA
            MDrv_DMD_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_C, &pktErrC); // _UTOPIA
            break;
        //dan add for integrate ISDBT utopia driver[end]
        default:
            break;
    }
    return MAPI_TRUE;
}

MAPI_U16 DTV_GetSignalQuality(void)
{

    MAPI_U16 quality;
//    mapi_tuner *pTuner=NULL;
//    float fRFLevel=0.0f;


    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
      case E_DEVICE_DEMOD_ISDB:
            quality = MDrv_DMD_ISDBT_GetSignalQuality(); // _UTOPIA
            //quality = MDrv_DMD_ISDBT_GetSignalQualityCombine(); // _UTOPIA
            break;
        //dan add for integrate ISDBT utopia driver[end]
        default:
            printf("%s Line:%d Error!!\n",__func__, __LINE__);
            quality=0;
            break;
    }
    //printf("dan_SQI in DTV_GetSignalQuality[%d]\n",quality);

    return quality;

}

MAPI_U16 DTV_GetSignalStrength(void)
{
    MAPI_U16 strength;
    switch(m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
      case E_DEVICE_DEMOD_ISDB:
        {
//       #if (VIF_TUNER_TYPE != 0) || (STB_ENABLE == 1)
            MDrv_DMD_ISDBT_GetSignalStrength(&strength); // _UTOPIA
//       #else
//            mapi_tuner* pTuner = mapi_interface::Get_mapi_pcb()->GetAtvTuner(0);
//            ASSERT(pTuner);
//            pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_RF_LEVEL, 0, 0, &strength);
//       #endif
        }
            break;
        //dan add for integrate ISDBT utopia driver[end]
       default:
            printf("%s Line:%d Error!!\n",__func__, __LINE__);
            strength=0;
            break;
    }

    return strength;
}

MAPI_U16 DTV_GetCellID(void)
{

    MAPI_U16 u16CellID =0;

    //MDrv_DMD_ISDBT_GetCellID(&u16CellID); // _UTOPIA

    return u16CellID;

}

MAPI_BOOL DTV_Serial_Control(MAPI_BOOL bEnable)
{
    m_bSerialOut=bEnable;

    switch (m_enCurrentDemodulator_Type)
    {
        //dan add for integrate ISDBT utopia driver[begin]
        case E_DEVICE_DEMOD_ISDB:
            {
                printf("Set DTV_Serial_Control...ISDBT %d\r\n\n", bEnable);
                MS_U8  u8TsConfigData;
                u8TsConfigData = 0;
                if(bEnable == TRUE)  //serial mode
                {
                    u8TsConfigData |= 0x01;
                    u8TsConfigData |= ISDBT_TS_CLK_DIVNUM_SERIAL_MODE << 3;  //0x04
                }
                else
                {
                    u8TsConfigData &= ~0x01;
                    u8TsConfigData |= ISDBT_TS_CLK_DIVNUM_PARALLEL_MODE << 3; //0x31
                }

                u8TsConfigData |= ((_bISDBT_TS_Data_Swap)?0x01:0x00) << 1;
                u8TsConfigData |= ISDBT_TS_CLK_INV << 2;

                MDrv_DMD_ISDBT_SetSerialControl(u8TsConfigData);        // _UTOPIA
            }
            break;
        //dan add for integrate ISDBT utopia driver[end]
        default:
                break;
    }
    return MAPI_TRUE;

}

MAPI_BOOL DTV_IsHierarchyOn(void)
{
    return FALSE;

}

//dan add for integrate ISDBT utopia driver[begin]
EN_LOCK_STATUS DTV_ISDB_GetLockStatus(void)
{
    DMD_ISDBT_LOCK_STATUS eLockStatus;
    eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK);
    switch (eLockStatus) // _UTOPIA
    {
        case DMD_ISDBT_LOCK:
            //printf("ISDBT demod E_DEMOD_LOCK!!!\n");
            return E_DEMOD_LOCK;
            break;
        case DMD_ISDBT_CHECKING:
            //printf("ISDBT demod E_DEMOD_CHECKING!!!\n");
            return E_DEMOD_CHECKING;
            break;
        case DMD_ISDBT_CHECKEND:
            //printf("ISDBT demod E_DEMOD_CHECKEND!!!\n");
            return E_DEMOD_CHECKEND;
            break;
        case DMD_ISDBT_UNLOCK:
            //printf("ISDBT demod E_DEMOD_UNLOCK!!!\n");
            return E_DEMOD_UNLOCK;
            break;
        default:
            //printf("ISDBTT_GetLockStatus error\n");
            break;
    }
    return E_DEMOD_UNLOCK;
}
//dan add for integrate ISDBT utopia driver[end]





//##########################################################################################################
//##########################################################################################################
//########################################  Private Function Implementation ################################
//##########################################################################################################
//##########################################################################################################



MAPI_BOOL WriteReg(MAPI_U16 u16Addr, MAPI_U8 u8Data)
{

    MDrv_DMD_WriteReg(u16Addr, u8Data);
    return MAPI_TRUE;

}



MAPI_BOOL ReadReg(MAPI_U16 u16Addr, MAPI_U8 *pu8Data)
{

    MDrv_DMD_ReadReg(u16Addr, pu8Data);
    return MAPI_TRUE;

}


/*
MAPI_BOOL Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MAPI_U8 param_cnt)
{
    UNUSED(pCmdPacket);
    UNUSED(param_cnt);

    return MAPI_TRUE;

}



MAPI_BOOL Cmd_Packet_Exe_Check(MAPI_BOOL* cmd_done)
{
    UNUSED(cmd_done);

    return MAPI_TRUE;

}
*/


MAPI_BOOL LoadDSPCode(void)
{


    return MAPI_TRUE;

}



MAPI_BOOL I2C_Address_Polling(void)
{
    return MAPI_TRUE;


}


MAPI_BOOL DTV_Config (RF_CHANNEL_BANDWIDTH BW, MAPI_BOOL bSerialTS, MAPI_BOOL bPalBG)
{
    UNUSED(BW);
    UNUSED(bSerialTS);
    UNUSED(bPalBG);

    return MAPI_TRUE;
}


void Driving_Control(MAPI_BOOL bEnable)
{
    UNUSED(bEnable);

    return;
}





#if 0 // ATV functions
MAPI_BOOL ATV_VIF_SetHandler(MAPI_BOOL bAutoScan, MAPI_U16 GainDistributionThr)
{
    UNUSED(bAutoScan);
    UNUSED(GainDistributionThr);

    return MAPI_TRUE;

}



MAPI_BOOL ATV_VIF_IfInitial(MAPI_U16 VifVgaMaximum)
{
    UNUSED(VifVgaMaximum);

    return MAPI_TRUE;

}



MAPI_BOOL ATV_VIF_TopAdjust(MAPI_U8 ucVifTop)
{
    UNUSED(ucVifTop);

    return MAPI_TRUE;

}


MAPI_BOOL ATV_VIF_SoftReset()
{
    return MAPI_TRUE;

}



E_VIF_TYPE ATV_GetVIF_Type()
{
    return E_VIF_INTERNAL;
}
#endif



