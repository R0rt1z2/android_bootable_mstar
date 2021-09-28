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
//#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "MsOS.h"
#include "Board.h"
#include "drvDemod.h"
#include "drvGPIO.h"
//#include "SysBsp.h"
#if (FRONTEND_TUNER_TYPE == TUNER_RT710 || FRONTEND_TUNER_TYPE == TUNER_AV2012 || FRONTEND_TUNER_TYPE == TUNER_AV2011 || FRONTEND_TUNER_TYPE == TUNER_R848)
//#include "drvTuner.h"
#elif (FRONTEND_TUNER_TYPE == TUNER_AV2018)
#include "drvTuner_AV2018.h"
#elif (FRONTEND_TUNER_TYPE == TUNER_RDA5815M)
#include "RDA5815M.h"
#elif (FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7VZ7801)
#include "drvTuner_Sharp7801.h"
#endif

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)
#include "drvDMD_EXTERN_MSB124x.h"
#include "device_demodulator_msb1245_dvbs2.h"
#define MSB124X_DEMOD_WAIT_TIMEOUT    (6000)
#define MSB124X_TUNER_WAIT_TIMEOUT    (50)
#define SAMPLING_RATE                  (108000)
#define MSB124X_TS_INV                  1
#define MSB124X_TS_DATA_SWAP            (0)

#if MSB124X_TS_DATA_SWAP //for DVBS
static MS_BOOL    _bTSDataSwap=FALSE;
#endif
static  MS_BOOL   _bIsDVBS2=FALSE;//DVBS ot DVBS2

#define MS124X_DEBUG 0

#if  MS124X_DEBUG
#define PRINTE(p) printf p
#else
#define PRINTE(p)
#endif

#define DEMOD_MSB124X_SLAVE_ID      (0xF2)//MCP:0xF2 External:0xD2 

#define DEMOD_MSB124X_SLAVE_ID_2    0xA2
#define MSB124X_DEMOD_IIC 1
#define MSB124X_SPI_IIC 2
#define MSB124X_MUTEX_TIMEOUT       (2000)

#if(FRONTEND_TUNER_TYPE == TUNER_AV2018 || FRONTEND_TUNER_TYPE == TUNER_RDA5815M)
#define MSB124X_ADCPLL_IQ_SWAP       0 // //1 is ADCPLL IQ swap enable, for customer issue.
#else
#define MSB124X_ADCPLL_IQ_SWAP       1 // //1 is ADCPLL IQ swap enable, for customer issue.
#endif

//mailbox[
#define REG_MB_CNTL             0x098a //DIG_SWUSE0FL
#define REG_MB_ADDR_L           0x098b //DIG_SWUSE0FH
#define REG_MB_ADDR_H           0x098c //DIG_SWUSE10L
#define REG_MB_DATA             0x098d //DIG_SWUSE10H



#ifdef SYSBSP_SUPPORT_TS_TO_MSPI
#define MSB124X_SPI_INTERFACE FALSE
#endif

typedef enum
{
    E_DMD_S2_ZIF_EN = 0x00,
    E_DMD_S2_RF_AGC_EN,
    E_DMD_S2_DCR_EN,
    E_DMD_S2_IQB_EN,
    E_DMD_S2_IIS_EN,
    E_DMD_S2_CCI_EN,
    E_DMD_S2_FORCE_ACI_SELECT,
    E_DMD_S2_IQ_SWAP,					//For DVBS2
    E_DMD_S2_AGC_REF_EXT_0,
    E_DMD_S2_AGC_REF_EXT_1,
    E_DMD_S2_AGC_K,
    E_DMD_S2_ADCI_GAIN,
    E_DMD_S2_ADCQ_GAIN,
    E_DMD_S2_SRD_SIG_SRCH_RNG,
    E_DMD_S2_SRD_DC_EXC_RNG,
    E_DMD_S2_FORCE_CFO_0,				//0FH
    E_DMD_S2_FORCE_CFO_1,
    E_DMD_S2_DECIMATION_NUM,
    E_DMD_S2_PSD_SMTH_TAP,
    E_DMD_S2_CCI_FREQN_0_L,
    E_DMD_S2_CCI_FREQN_0_H,
    E_DMD_S2_CCI_FREQN_1_L,
    E_DMD_S2_CCI_FREQN_1_H,
    E_DMD_S2_CCI_FREQN_2_L,
    E_DMD_S2_CCI_FREQN_2_H,
    E_DMD_S2_TR_LOPF_KP,
    E_DMD_S2_TR_LOPF_KI,
    E_DMD_S2_FINEFE_KI_SWITCH_0,
    E_DMD_S2_FINEFE_KI_SWITCH_1,
    E_DMD_S2_FINEFE_KI_SWITCH_2,
    E_DMD_S2_FINEFE_KI_SWITCH_3,
    E_DMD_S2_FINEFE_KI_SWITCH_4,		//1FH
    E_DMD_S2_PR_KP_SWITCH_0,
    E_DMD_S2_PR_KP_SWITCH_1,
    E_DMD_S2_PR_KP_SWITCH_2,
    E_DMD_S2_PR_KP_SWITCH_3,
    E_DMD_S2_PR_KP_SWITCH_4,
    E_DMD_S2_FS_GAMMA,
    E_DMD_S2_FS_ALPHA0,
    E_DMD_S2_FS_ALPHA1,
    E_DMD_S2_FS_ALPHA2,
    E_DMD_S2_FS_ALPHA3,
    E_DMD_S2_FS_H_MODE_SEL,
    E_DMD_S2_FS_OBSWIN,
    E_DMD_S2_FS_PEAK_DET_TH_L,
    E_DMD_S2_FS_PEAK_DET_TH_H,
    E_DMD_S2_FS_CONFIRM_NUM,
    E_DMD_S2_EQ_MU_FFE_DA,				//2FH
    E_DMD_S2_EQ_MU_FFE_DD,
    E_DMD_S2_EQ_ALPHA_SNR_DA,
    E_DMD_S2_EQ_ALPHA_SNR_DD,
    E_DMD_S2_FEC_ALFA,					//For DVBS2
    E_DMD_S2_FEC_BETA,					//For DVBS2
    E_DMD_S2_FEC_SCALING_LLR,			//For DVBS2
    E_DMD_S2_TS_SERIAL,
    E_DMD_S2_TS_CLK_RATE,
    E_DMD_S2_TS_OUT_INV,
    E_DMD_S2_TS_DATA_SWAP,
	//------------------------------------------
	E_DMD_S2_FW_VERSION_L,           	//0x3A
	E_DMD_S2_FW_VERSION_H,           	//0x3B
	E_DMD_S2_CHIP_VERSION,
	E_DMD_S2_FS_L,		 				//Frequency
	E_DMD_S2_FS_H,						//Frequency
	E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L,	//0x3F
	E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H,	//
	E_DMD_S2_SYSTEM_TYPE,				//DVBS/S2
	E_DMD_S2_MODULATION_TYPE,			//QPSK/8PSK
	E_DMD_S2_CODERATE,					//1/2

	E_DMD_S2_UNCRT_PKT_NUM_7_0,
	E_DMD_S2_UNCRT_PKT_NUM_8_15,
	E_DMD_S2_STATE_FLAG,
	E_DMD_S2_SUBSTATE_FLAG,
	E_DMD_S2_HUM_DETECT_FLAG,
	E_DMD_S2_CCI_DETECT_FLAG,
	E_DMD_S2_IIS_DETECT_FLAG,
	E_DMD_S2_OPEN_HUM_VLD_IRQ_FLAG,
	E_DMD_S2_SRD_COARSE_DONE_FLAG,
	E_DMD_S2_SRD_FINE_DONE_FLAG,
	E_DMD_S2_FINEFE_DONE_FLAG,
	E_DMD_S2_REV_FRAME_FLAG,			//0x4F
	E_DMD_S2_DUMMY_FRAME_FLAG,
	E_DMD_S2_PLSC_DONE_FLAG,
	E_DMD_S2_GET_INFO_FROM_FRAME_LENGTH_DONE_FLAG,
	E_DMD_S2_IQ_SWAP_DETECT_FLAG,
	E_DMD_S2_FRAME_ACQUISITION_DONE_FLAG,
	E_DMD_S2_OLCFE_DONE_FLAG,
	E_DMD_S2_FSYNC_FOUND_FLAG,
	E_DMD_S2_FSYNC_FAIL_SEARCH_FLAG,
	E_DMD_S2_FALSE_ALARM_FLAG,
	E_DMD_S2_VITERBI_IN_SYNC_FLAG,
	E_DMD_S2_INT_CODE_RATE_SEARCH_FAIL_FLAG,
	E_DMD_S2_VITERBI_INT_PRE_FLAG,
	E_DMD_S2_BER_WINDOW_END_FLAG,
	E_DMD_S2_PASS_WRONG_INT_FLAG,
	E_DMD_S2_CLK_CNT_OVER_FLAG,
	E_DMD_S2_UNCRT_OVER_FLAG,			//0x5F
	E_DMD_S2_DISEQC_RX_LENGTH,
	E_DMD_S2_DISEQC_INTERRUPT_FLAG,
	E_DMD_S2_DISEQC_RX_FLAG,
	E_DMD_S2_DISEQC_INTERRUPT_STATUS,
	E_DMD_S2_DISEQC_STATUS_FLAG,
	E_DMD_S2_ACI_FIR_SELECTED,			//0x65
	//LOCK
	E_DMD_S2_AGC_LOCK_FLAG,
	E_DMD_S2_DCR_LOCK_FLAG,
	E_DMD_S2_DAGC0_LOCK_FLAG,
	E_DMD_S2_DAGC1_LOCK_FLAG,
	E_DMD_S2_DAGC2_LOCK_FLAG,
	E_DMD_S2_DAGC3_LOCK_FLAG,
	E_DMD_S2_TR_LOCK_FLAG,
	E_DMD_S2_CLCFE_LOCK_FLAG,
	E_DMD_S2_EQ_LOCK_FLAG,
	E_DMD_S2_PR_LOCK_FLAG,				//0x6F
	E_DMD_S2_FSYNC_LOCK_FLAG,
	E_DMD_S2_FSYNC_FAIL_LOCK_FLAG,

  	E_DMD_S2_MB_SWUSE12L,				//0x72
  	E_DMD_S2_MB_SWUSE12H,
  	E_DMD_S2_MB_SWUSE13L,
  	E_DMD_S2_MB_SWUSE13H,
  	E_DMD_S2_MB_SWUSE14L,
  	E_DMD_S2_MB_SWUSE14H,
  	E_DMD_S2_MB_SWUSE15L,
  	E_DMD_S2_MB_SWUSE15H,
  	E_DMD_S2_MB_SWUSE16L,
  	E_DMD_S2_MB_SWUSE16H,
	E_DMD_S2_MB_SWUSE17L,
	E_DMD_S2_MB_SWUSE17H,
  	E_DMD_S2_MB_SWUSE18L,
  	E_DMD_S2_MB_SWUSE18H,				//0x7F
  	E_DMD_S2_MB_SWUSE19L,
  	E_DMD_S2_MB_SWUSE19H,
  	E_DMD_S2_MB_SWUSE1AL,
  	E_DMD_S2_MB_SWUSE1AH,
  	E_DMD_S2_MB_SWUSE1BL,
  	E_DMD_S2_MB_SWUSE1BH,
  	E_DMD_S2_MB_SWUSE1CL,
  	E_DMD_S2_MB_SWUSE1CH,
  	E_DMD_S2_MB_SWUSE1DL,
  	E_DMD_S2_MB_SWUSE1DH,
  	E_DMD_S2_MB_SWUSE1EL,
	E_DMD_S2_MB_SWUSE1EH,
	E_DMD_S2_MB_SWUSE1FL,
	E_DMD_S2_MB_SWUSE1FH, 				//0x8D

	E_DMD_S2_MB_DMDTOP_DBG_0,
	E_DMD_S2_MB_DMDTOP_DBG_1,			//0x8F
	E_DMD_S2_MB_DMDTOP_DBG_2,
	E_DMD_S2_MB_DMDTOP_DBG_3,
	E_DMD_S2_MB_DMDTOP_DBG_4,
	E_DMD_S2_MB_DMDTOP_DBG_5,
	E_DMD_S2_MB_DMDTOP_DBG_6,
	E_DMD_S2_MB_DMDTOP_DBG_7,
	E_DMD_S2_MB_DMDTOP_DBG_8,
	E_DMD_S2_MB_DMDTOP_DBG_9,
	E_DMD_S2_MB_DMDTOP_DBG_A,
	E_DMD_S2_MB_DMDTOP_DBG_B,

	E_DMD_S2_MB_DMDTOP_SWUSE00L,
	E_DMD_S2_MB_DMDTOP_SWUSE00H,
	E_DMD_S2_MB_DMDTOP_SWUSE01L,
	E_DMD_S2_MB_DMDTOP_SWUSE01H,
	E_DMD_S2_MB_DMDTOP_SWUSE02L,
	E_DMD_S2_MB_DMDTOP_SWUSE02H,		//0x9F
	E_DMD_S2_MB_DMDTOP_SWUSE03L,
	E_DMD_S2_MB_DMDTOP_SWUSE03H,
	E_DMD_S2_MB_DMDTOP_SWUSE04L,
	E_DMD_S2_MB_DMDTOP_SWUSE04H,
	E_DMD_S2_MB_DMDTOP_SWUSE05L,
	E_DMD_S2_MB_DMDTOP_SWUSE05H,
	E_DMD_S2_MB_DMDTOP_SWUSE06L,
	E_DMD_S2_MB_DMDTOP_SWUSE06H,
	E_DMD_S2_MB_DMDTOP_SWUSE07L,
	E_DMD_S2_MB_DMDTOP_SWUSE07H,

	E_DMD_S2_MB_TOP_WR_DBG_90,
	E_DMD_S2_MB_TOP_WR_DBG_91,
	E_DMD_S2_MB_TOP_WR_DBG_92,
	E_DMD_S2_MB_TOP_WR_DBG_93,
	E_DMD_S2_MB_TOP_WR_DBG_94,
	E_DMD_S2_MB_TOP_WR_DBG_95,			//0xAF
	E_DMD_S2_MB_TOP_WR_DBG_96,
	E_DMD_S2_MB_TOP_WR_DBG_97,
	E_DMD_S2_MB_TOP_WR_DBG_98,
	E_DMD_S2_MB_TOP_WR_DBG_99,

	E_DMD_S2_MB_DUMMY_REG_0,
	E_DMD_S2_MB_DUMMY_REG_1,
	E_DMD_S2_MB_DUMMY_REG_2,
	E_DMD_S2_MB_DUMMY_REG_3,
	E_DMD_S2_MB_DUMMY_REG_4,
	E_DMD_S2_MB_DUMMY_REG_5,
	E_DMD_S2_MB_DUMMY_REG_6,
	E_DMD_S2_MB_DUMMY_REG_7,
	E_DMD_S2_MB_DUMMY_REG_8,
	E_DMD_S2_MB_DUMMY_REG_9,
	E_DMD_S2_MB_DUMMY_REG_A,
	E_DMD_S2_MB_DUMMY_REG_B,			//0xBF
	E_DMD_S2_MB_DUMMY_REG_C,
	E_DMD_S2_MB_DUMMY_REG_D,
	E_DMD_S2_MB_DUMMY_REG_E,
	E_DMD_S2_MB_DUMMY_REG_F,
	E_DMD_S2_MB_DUMMY_REG_10,
	E_DMD_S2_MB_DUMMY_REG_11,

	E_DMD_S2_MB_DMDTOP_INFO_01,
	E_DMD_S2_MB_DMDTOP_INFO_02,
	E_DMD_S2_MB_DMDTOP_INFO_03,
	E_DMD_S2_MB_DMDTOP_INFO_04,
	E_DMD_S2_MB_DMDTOP_INFO_05,
	E_DMD_S2_MB_DMDTOP_INFO_06,
	E_DMD_S2_MB_DMDTOP_INFO_07,
	E_DMD_S2_MB_DMDTOP_INFO_08,

	E_DMD_S2_IDLE_STATE_UPDATED,
	E_DMD_S2_LOG_FLAG,					//0xCF
	E_DMD_S2_LOG_SKIP_INDEX,
	E_DMD_S2_LOCK_COUNT,
	E_DMD_S2_NARROW_STEP_FLAG,
	E_DMD_S2_UNCORRECT_PKT_COUNT,
	E_DMD_S2_DISEQC_INIT_MODE,
	E_DMD_S2_DECIMATE_FORCED,
	E_DMD_S2_SRD_MAX_SRG_FLAG,
	E_DMD_S2_DVBS_OUTER_RETRY,

	E_DMD_S2_FORCED_DECIMATE_FLAG,
	E_DMD_S2_NO_SIGNAL_FLAG,
	E_DMD_S2_SPECTRUM_TRACK_FLAG,
	E_DMD_S2_SRD_LOCAL_SEARCH_FLAG,
	E_DMD_S2_NO_SIGNAL_RATIO_CHECK_FLAG,
	E_DMD_S2_LOW_SR_ACI_FLAG,
	E_DMD_S2_SPECTRUM_TRACKER_TIMEOUT,
	E_DMD_S2_TR_TIMEOUT,				//0xDF
	E_DMD_S2_BALANCE_TRACK,
	E_DMD_S2_GAIN_TILT_FLAG,			//0xE1

	DVBS2_PARAM_LEN
} DVBS_Param_2;
static  MS_S32                       _s32TaskId     = -1;
static  MS_S32                       _s32MutexId;
static  MS_S32                       _s32FunMutexId;
static  MS_BOOL                  _bInited     = FALSE;
static  MS_BOOL                  _bDemodType=FALSE;
static  MS_U16                       _u16BlindScanStartFreq =0;
static  MS_U16                       _u16BlindScanEndFreq   =0;
static  MS_U16                       _u16TunerCenterFreq    =0;
static  MS_U16                      _u16ChannelInfoIndex    =0;
//Debug Only+
static  MS_U16                      _u16NextCenterFreq=0;
static  MS_U16                      _u16LockedSymbolRate=0;
static  MS_U16                      _u16LockedCenterFreq=0;
static  MS_U16                      _u16PreLockedHB=0;
static  MS_U16                      _u16PreLockedLB=0;
static  MS_U16                      _u16CurrentSymbolRate=0;
static  MS_S16                      _s16CurrentCFO=0;
static  MS_U16                      _u16CurrentStepSize=0;
static MS_U8                        _u8ToneBurstFlag=0;
static  MS_U32                      _u32DemodStartTime=0;//claire test
//Debug Only-
static  MS_U16                      _u16ChannelInfoArray[2][1000];
static const MS_U16                       _u16SignalLevel[47][2]=
{
    {8285,    922},{10224,    902},{12538,    882},{14890,    862},{17343,    842},{19767,    822},{22178,    802},{24618,    782},{27006,    762},{29106,    742},
    {30853,    722},{32289,    702},{33577,    682},{34625,    662},{35632,    642},{36552,    622},{37467,    602},{38520,    582},{39643,    562},{40972,    542},
    {42351,    522},{43659,    502},{44812,    482},{45811,    462},{46703,    442},{47501,    422},{48331,    402},{49116,    382},{49894,    362},{50684,    342},
    {51543,    322},{52442,    302},{53407,    282},{54314,    262},{55208,    242},{56000,    222},{56789,    202},{57544,    182},{58253,    162},{58959,    142},
    {59657,    122},{60404,    102},{61181,    82},{62008,    62},{63032,    42},{65483,    22},{65535,    12}
};

MS_U16 u16_INTERN_DVBS_Version;

static void _msb124x_hw_reset(void)
{
#if 0
    int rst_pin = GPIO_DEMOD_RST;

    mdrv_gpio_set_high(rst_pin);
    MsOS_DelayTask(100);
    mdrv_gpio_set_low(rst_pin);
    MsOS_DelayTask(200);
    mdrv_gpio_set_high(rst_pin);
    MsOS_DelayTask(100);
    MDrv_DMD_MSB124X_Exit();
#endif
}

static float Log10Approx(float flt_x)
{
        MS_U32 u32_temp = 1;
        MS_U8 indx = 0;
        do
        {
            u32_temp = u32_temp << 1;
            if (flt_x < (float)u32_temp)
                break;
        }
        while (++indx < 32);
        return (float)0.3 *indx;
}
/*================================================
==                       IIC write/read interface
=================================================*/
#ifdef SYSBSP_SUPPORT_TS_TO_MSPI
    static void msb124x_SPIPAD_En(MS_BOOL bOnOff)
    {
        if (bOnOff)
        {
            SYSBSP_ENABLE_MSPI_PAD();
        }
        else
        {
            SYSBSP_DISABLE_MSPI_PAD();
        }
    }
#endif

MS_BOOL MSB124X_WriteBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Write(DEMOD_MSB124X_SLAVE_ID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        printf("Demod IIC write error\n");
    }
    return bRet;
}

MS_BOOL MSB124X_ReadBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;

    bRet = MDrv_IIC_Read(DEMOD_MSB124X_SLAVE_ID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        printf("Demod IIC read error\n");
    }
    return bRet;
}

MS_BOOL MSB124X_WriteBytes_spi(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Write(DEMOD_MSB124X_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        printf("Demod IIC write spi error\n");
    }
    return bRet;
}

MS_BOOL MSB124X_ReadBytes_spi(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Read(DEMOD_MSB124X_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        printf("Demod IIC read spi error\n");
    }
    return bRet;
}

typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

mapi_i2c DemodI2Chandler, SpiI2Chandler;
mapi_i2c* mapi_i2c_GetI2C_Dev(MS_U32 u32gID)
{
    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        printf("iic device not supported\n");
    case MSB124X_DEMOD_IIC:
        handler = &DemodI2Chandler;
        handler->WriteBytes = MSB124X_WriteBytes_demod;
        handler->ReadBytes = MSB124X_ReadBytes_demod;
        break;
    case MSB124X_SPI_IIC:
        handler = &SpiI2Chandler;
        handler->WriteBytes = MSB124X_WriteBytes_spi;
        handler->ReadBytes = MSB124X_ReadBytes_spi;
        break;
    }
    return handler;
}

static MS_BOOL msb124x_I2C_Access(eDMD_MSB124X_DemodI2CSlaveID eSlaveID, eDMD_MSB124X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
    switch (eSlaveID)
    {
    case E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2:
        i2c_iptr = mapi_i2c_GetI2C_Dev(MSB124X_SPI_IIC);
        break;
    case E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1:
    default:
        i2c_iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
        break;
    }

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB124X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
        default:
            break;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

/*================================================
==               demod  msb124x  write/read interface
=================================================*/
MS_BOOL MSB124X_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_MSB124X_SetReg(u16Addr, u8Data);
}

MS_BOOL MSB124X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    return MDrv_DMD_MSB124X_SetRegs(u16Addr, u8pData, data_size);
}

MS_BOOL MSB124X_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    return MDrv_DMD_MSB124X_SetReg2Bytes(u16Addr, u16Data);
}

MS_BOOL MSB124X_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return MDrv_DMD_MSB124X_GetReg(u16Addr, pu8Data);
}

MS_BOOL  MSB124X_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_MSB124X_SetDSPReg(u16Addr, u8Data);
}

MS_BOOL MSB124X_ReadDspReg(MS_U16 u16Addr, MS_U8* pData)
{
    return MDrv_DMD_MSB124X_GetDSPReg(u16Addr, pData);
}

MS_BOOL MSB124X_TS_Enable(MS_BOOL bTsEnable)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16Address;
    MS_U8   u8Data;
    if (bTsEnable == TRUE)
    {
        bRet&= MSB124X_WriteReg(0x095A, 0xFF);
    }
    else
    {
        bRet&= MSB124X_WriteReg(0x095A, 0x00);
    }

    u16Address=0x0924;
    bRet &= MSB124X_ReadReg(u16Address, &u8Data);
    #if MSB124X_TS_INV
        u8Data |= (0x20);
    #else
        u8Data ~= (0x20);
    #endif
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    return bRet;
}
//MS_BOOL MDrv_Demod_GetFWVer(MS_U16 *ver)
MS_BOOL MSB124X_GPIO_SetLevel(MS_U8 u8Pin, MS_BOOL bLevel)
{
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_BOOL bRet = FALSE;

    if (u8Pin==13)
    {
        u16Address=0x09CE;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x10);//u8Data|=0x10;
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //GPIO output enable
        if (bLevel==FALSE)
        {
            u16Address=0x09CC;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u8Data|=0x10;
            bRet&=MSB124X_WriteReg(u16Address, u8Data); //Set level to high
        }
        else
        {
            u16Address=0x09CC;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u8Data&=(~0x10);
            bRet&=MSB124X_WriteReg(u16Address, u8Data); //Set level to high
        }
    }
    else if (u8Pin==11)
    {
        u16Address=0x095C;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //set TS_ERR to GPIO
        u16Address=0x09C3;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x08);//u8Data|=0x08;
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //GPIO output enable
        if (bLevel==FALSE)
        {
            u16Address=0x09C3;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u8Data|=0x08;
            bRet&=MSB124X_WriteReg(u16Address, u8Data); //Set level to high
        }
        else
        {
            u16Address=0x09C3;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u8Data&=(~0x08);
            bRet&=MSB124X_WriteReg(u16Address, u8Data); //Set level to high
        }
    }
    else
    {
        return FALSE;
    }
    return bRet;
}
MS_BOOL MSB124X_GPIO_GetLevel(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_BOOL bRet =FALSE;
    if (u8Pin==13)
    {
        u16Address=0x09CE;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x10);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //GPIO output disable

        u16Address=0x09D0;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        if ((u8Data&0x10)==0x10)
        {
            *bLevel=TRUE;
        }
        else
        {
            *bLevel=FALSE;
        }
    }
    else if (u8Pin==11)
    {
        u16Address=0x095C;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //set TS_ERR to GPIO

        u16Address=0x09C1;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x08);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //GPIO output disable

        u16Address=0x09C5;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        if ((u8Data&0x08)==0x08)
        {
            *bLevel=TRUE;
        }
        else
        {
            *bLevel=FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    return bRet;
}

MS_BOOL MSB124X_GPIO_Disable(MS_U8 u8Pin)
{
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_BOOL bRet =FALSE;
    if (u8Pin==13)
    {
        u16Address=0x09CE;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x10);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //GPIO output disable

    }
    else if (u8Pin==11)
    {
        u16Address=0x095C;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //set TS_ERR to GPIO

        u16Address=0x09C1;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x08);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);    //GPIO output disable
    }
    else
    {
        return FALSE;
    }
    return bRet;
}

MS_BOOL MSB124X_IIC_Bypass_Mode(MS_BOOL enable)
{
    MS_U8 u8Retry = 10;
    MS_BOOL bResult;

    PRINTE((">>>MSB124X: IIC_bypass() set %x\n", enable));
	
    while (u8Retry--)
    {
        if (enable)
            bResult = MSB124X_WriteReg(0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            bResult = MSB124X_WriteReg(0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off

        if (bResult)
            break;
    }

    return bResult;
}


MS_BOOL  MSB124X_SetFrequency(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{
     MS_BOOL bRet=TRUE;
     MS_U16 u16LockCount;
#if(FRONTEND_TUNER_TYPE == TUNER_AV2012)
	bRet&=MDrv_Tuner_AV2012_SetFreq(0, u16CenterFreq, u32SymbolRate_Hz);
#elif (FRONTEND_TUNER_TYPE == TUNER_AV2012 || FRONTEND_TUNER_TYPE == TUNER_AV2011 || FRONTEND_TUNER_TYPE == TUNER_AV2018 || FRONTEND_TUNER_TYPE == TUNER_R848)
    bRet&=MDrv_Tuner_SetFreq(u16CenterFreq, u32SymbolRate_Hz);
#elif ( FRONTEND_TUNER_TYPE == TUNER_RDA5815M || FRONTEND_TUNER_TYPE == TUNER_RT710)
    bRet&=MDrv_Tuner_SetFreq(u16CenterFreq, u32SymbolRate_Hz);
    return bRet;
#elif (FRONTEND_TUNER_TYPE==TUNER_SHARP_BS2S7VZ7801)
    bRet&=MDrv_Tuner_SetFreq(u16CenterFreq, u32SymbolRate_Hz/1000);
#endif
    if (bRet==FALSE)
        return bRet;
	
    u16LockCount=0;
    do
    {
        bRet=TRUE;
#if(FRONTEND_TUNER_TYPE == TUNER_AV2012)
		bRet&=MDrv_Tuner_AV2012_GetLock(0);		
#else
        extern MS_BOOL MDrv_Tuner_GetLock(void);
        bRet&=MDrv_Tuner_GetLock();
#endif
        MsOS_DelayTask(1);
        u16LockCount++;
    }while((bRet==FALSE) && (u16LockCount<MSB124X_TUNER_WAIT_TIMEOUT));

    if (bRet==TRUE)
    {
        PRINTE(("Tuner PLL Lock\n"));
    }
    else
    {
        PRINTE(("Tuner PLL Unlock\n"));
    }
     return bRet;
}


MS_U16  MSB124X_DTV_GetSignalStrength(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U16 u16Data;
    MS_U8  u8Data;
    MS_U8  u8Index;

    u16Address=0x3E26;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&0xF0)|0x03;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x3E04;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data|=0x80;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x3E29;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x3E28;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    u16Address=0x3E04;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=~(0x80);
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    if (bRet==FALSE)
    {
        PRINTE(("MSB124X_DTV_GetSignalStrength fail!!! \n"));
        return 0;
    }
    for (u8Index=0; u8Index < 47; u8Index++)
    {
        if ((65535 - u16Data) <= _u16SignalLevel[u8Index][0])
        {
            return _u16SignalLevel[u8Index][1];
        }
    }
    return 0;
}
MS_U32  MSB124X_DTV_GetSignalNoiseRatio(void)
{
    MS_BOOL status=TRUE;
    MS_U16 u16Address;
    MS_U32 u32Data;
    MS_U8  u8Data;
    float  fvariance;
    float  log_snr;

    if (MSB124X_DTV_GetLock()==FALSE)
            return 0;

     u16Address=0x1B08;
     status&=MSB124X_ReadReg(u16Address, &u8Data);
     u8Data|=0x10;
     status&=MSB124X_WriteReg(u16Address, u8Data);

     u16Address=0x1BA5;
     status&=MSB124X_ReadReg(u16Address, &u8Data);
     u32Data=(u8Data&0x3F);

     u16Address=0x1BA4;
     status&=MSB124X_ReadReg(u16Address, &u8Data);
     u32Data=(u32Data<<8)|u8Data;

     u16Address=0x1BA3;
     status&=MSB124X_ReadReg(u16Address, &u8Data);
     u32Data=(u32Data<<8)|u8Data;

     u16Address=0x1BA2;
     status&=MSB124X_ReadReg(u16Address, &u8Data);
     u32Data=(u32Data<<8)|u8Data;

     u16Address=0x1B08;
     status&=MSB124X_ReadReg(u16Address, &u8Data);
     u8Data&=~(0x10);
     status&=MSB124X_WriteReg(u16Address, u8Data);
    if (status==FALSE)
    {
        return 0;
    }
    fvariance=(float)u32Data/0x20000000;
    if (fvariance==0)
        fvariance=1;
    log_snr=10*Log10Approx(1/fvariance);

    return (MS_U32)log_snr;
}
MS_U16    MSB124X_DTV_GetSignalQuality(MS_BOOL bDemodType)
{
        MS_BOOL bRet=TRUE;
        MS_U16 u16Address;
        MS_U8  u8Data;
        MS_U16 u16BitErrPeriod;
        MS_U32 u32BitErr;
        float fber;
        float log_ber;

        if (MSB124X_DTV_GetLock()==FALSE)
            return 0;

        if (_bIsDVBS2==FALSE)//S
        {
        u16Address=0x1c2C;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        if (bRet==FALSE)
        {
	        PRINTE(("dvbs2_MSB124X_DTV_GetSignalQuality fail!!! \n"));
                                return 0;
        }
        if ((u8Data&0x02)!=0x02)
        {
            return 0;
        }
        u16Address=0x1c32;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data|=0x80;
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
        u16Address=0x1c31;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1c30;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
        u16Address=0x1c3D;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x1c3C;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c3B;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c3A;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c32;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=~(0x80);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
        if (u16BitErrPeriod == 0)
            u16BitErrPeriod = 1;
        if (u32BitErr <= 0)
            fber = 0.5 / ((float)u16BitErrPeriod *128 * 188 * 8);
        else
            fber = (float)u32BitErr / ((float)u16BitErrPeriod *128 * 188 * 8);
        if (bRet==FALSE)
        {
            PRINTE(("MSB131X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }
        log_ber = ( - 1) *Log10Approx(1 / fber);
        if (log_ber <= ( - 7.0))
        {
            return 100;
        }
        else if (log_ber < ( - 3.7))
                      {
                            return (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
                      }
                       else if (log_ber < ( - 2.7))
                       {
                        return (10+((( - 2.7) - log_ber) / (( - 2.7) - ( - 3.7))*(60-10)));
                      }
                      else
                      {
                            return 10;
                      }
                      return 0;

    }
    else    //S2
    {
        u16Address=0x1d04;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data|=0x01;
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
        u16Address=0x1d25;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1d24;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
        u16Address=0x1d5b;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=u8Data;
	    u16Address=0x1d5a;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
	    u16Address=0x1d59;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
	    u16Address=0x1d58;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1d04;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=~(0x01);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
        if (u16BitErrPeriod == 0)
            u16BitErrPeriod = 1;
        fber = (float)u32BitErr/(u16BitErrPeriod*64800);
        if (bRet==FALSE)
        {
	        PRINTE(("MSB124X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }
        log_ber = ( - 1) *Log10Approx(1 / fber);
        if (log_ber <= ( - 7.0))
        {
            return 100;
         }
         else if (log_ber < ( - 3.7))
         {
            return (80+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-80)));
         }
         else if (log_ber < ( - 1.7))
         {
            return (40+((( - 1.7) - log_ber) / (( - 1.7) - ( - 3.7))*(80-40)));
         }
         else if (log_ber < ( - 0.7))
         {
            return (10+((( - 0.7) - log_ber) / (( - 0.7) - ( - 1.7))*(40-10)));
         }
         else
          {
            return 5;
          }
          return 0;
    }
}
MS_BOOL  MSB124X_DTV_GetLock(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    u16Address=0x0990;
    bRet &= MSB124X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u16Address=0x0B56;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);

        if((u8Data>=15) && (u8Data!=0xcd))
        {
            if (u8Data==15)
            {
               _bIsDVBS2=FALSE;   //S
               bRet = TRUE;
            }
            else if(u8Data==16)
            {
               _bIsDVBS2=TRUE;    //S2
               bRet = TRUE;
            }
            else
            {
               bRet = FALSE;
            }
         }
          else
         {
             bRet = FALSE;
         }
 #if MSB124X_TS_DATA_SWAP
          if (bRet==FALSE)
          {
               _bTSDataSwap=FALSE;
            }
            else
            {
               if (_bTSDataSwap==FALSE)
               {
                   _bTSDataSwap= TRUE;
                   u16Address=0x2A40;
                   bRet&=MSB124X_ReadReg(u16Address, &u8Data);	
                   u8Data^=0x20;
                   bRet&=MSB124X_WriteReg(u16Address, u8Data);	
                }
            }
#endif
            }
            else
            {
                 bRet = TRUE;
            }
            MSB124X_TS_Enable(1);	

            return bRet;
}
MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_TPSGetLock(MS_BOOL *pbLock)
{
    MS_BOOL bRet=TRUE;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pbLock=MSB124X_DTV_GetLock();
    PRINTE(("MSB124X MDrv_Demod_TPSGetLock  0x%x\n", *pbLock));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_MPEGGetLock(MS_BOOL *pbLock)
{
    MS_BOOL bRet=TRUE;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pbLock=MSB124X_DTV_GetLock();
    PRINTE(("MSB131X MDrv_Demod_MPEGGetLock 0x%x\n", *pbLock));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetRollOff(MS_U8 *pRollOff)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x1B1E;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x03)==0x00)
        *pRollOff = 0;  //Rolloff 0.35
    else if (((u8Data&0x03)==0x01) || ((u8Data&0x03)==0x03))
        *pRollOff = 1;  //Rolloff 0.25
    else
        *pRollOff = 2;  //Rolloff 0.20
    PRINTE(("MDrv_Demod_GetRollOff:%d\n", *pRollOff));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetRFOffset(MS_S16 *ps16RFOff)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U32 u32FreqOffset;
    float FreqOffset;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u32FreqOffset=0;
    u16Address=0x2C39;
    u8Data=0x08;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x3E05;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data|=0x80;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);

    u16Address=0x2C46;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u32FreqOffset=u8Data;
    u16Address=0x2C45;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u32FreqOffset=(u32FreqOffset<<8)|u8Data;
    u16Address=0x2C44;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u32FreqOffset=(u32FreqOffset<<8)|u8Data;
    u16Address=0x3E05;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=~(0x80);
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    FreqOffset=(float)u32FreqOffset;
    if (FreqOffset>=2048)
    {
        FreqOffset=FreqOffset-4096;
    }
    FreqOffset=(FreqOffset/4096)*SAMPLING_RATE;
    *ps16RFOff=FreqOffset/1000;
    PRINTE(("MSB124X FreqOffset:%f[MHz]\n", FreqOffset/1000));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        //MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_Start+\n"));
    _u32DemodStartTime=MsOS_GetSystemTime();//claire test
    _u16BlindScanStartFreq=u16StartFreq;
    _u16BlindScanEndFreq=u16EndFreq;
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x0992;
    bRet&=MSB124X_WriteReg2bytes(u16Address, _u16BlindScanStartFreq);
    PRINTE(("MDrv_Demod_BlindScan_Start- _u16BlindScanStartFreq%d u16StartFreq %d u16EndFreq %d\n", _u16BlindScanStartFreq, u16StartFreq, u16EndFreq));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U32  u32TunerCutOffFreq;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_NextFreq+\n"));
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x02)
    {
            u8Data|=0x08;
            bRet&=MSB124X_WriteReg(u16Address, u8Data);
            u16WaitCount=0;
            do
            {
                u16Address=0x0B56;
                bRet&=MSB124X_ReadReg(u16Address, &u8Data);
                u16WaitCount++;
                PRINTE(("MDrv_Demod_BlindScan_NextFreq u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
                MsOS_DelayTask(1);
            }while((u8Data!=0x01)&&(u16WaitCount<MSB124X_DEMOD_WAIT_TIMEOUT));
     }
    u16WaitCount=0;

    * bBlindScanEnd=FALSE;
    _u16TunerCenterFreq=0;
    u16Address=0x0993;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    _u16TunerCenterFreq=u8Data;
    u16Address=0x0992;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    _u16TunerCenterFreq=(_u16TunerCenterFreq<<8)|u8Data;

    if (_u16TunerCenterFreq >=_u16BlindScanEndFreq)
    {
        PRINTE(("MDrv_Demod_BlindScan_NextFreq . _u16TunerCenterFreq %d _u16BlindScanEndFreq%d\n", _u16TunerCenterFreq, _u16BlindScanEndFreq));
        PRINTE((">>>blind scan time = %d(ms)<<<\n", MsOS_GetSystemTime() - _u32DemodStartTime));//claire test
        * bBlindScanEnd=TRUE;
        MsOS_ReleaseMutex(_s32FunMutexId);
        return bRet;
    }
#if (FRONTEND_TUNER_TYPE == TUNER_AV2011)
    u32TunerCutOffFreq=44000;
#else
    u32TunerCutOffFreq=44000;//34000
#endif
    MSB124X_IIC_Bypass_Mode(TRUE);
    bRet&=MSB124X_SetFrequency(_u16TunerCenterFreq,u32TunerCutOffFreq);
    MSB124X_IIC_Bypass_Mode(FALSE);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u8Data&=~(0x08);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
        u8Data|=0x02;
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
        u8Data|=0x01;
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
    }
    else
    {
        u8Data&=~(0x08);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
    }
    PRINTE(("MDrv_Demod_BlindScan_NextFreq _u16TunerCenterFreq:%d-\n", _u16TunerCenterFreq));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U32  u32Data;
    MS_U16 u16Data;
    MS_U8   u8Data;
    MS_U16  u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished+\n"));
    u16WaitCount=0;
    *u8FindNum=0;
    *u8Progress=0;
    do
    {
        u16Address=0x0B56; // "DIG_DBG_6" means state
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16WaitCount++;
        PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished+1 u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
        MsOS_DelayTask(1);
    }while((u8Data!=17)&&(u16WaitCount<MSB124X_DEMOD_WAIT_TIMEOUT));//#define     DVBS_BLIND_SCAN      17



    if (u16WaitCount>=MSB124X_DEMOD_WAIT_TIMEOUT)
    {
        bRet=FALSE;
    }
    else
    {
        u16Address=0x0B57;// "DIG_DBG_7" means sub_state
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        if (u8Data==0)
        {
        	  /*
            u16Address=0x0B86;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u32Data=u8Data;
            u16Address=0x0B85;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            u16Address=0x0B84;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000); //Center Freq
            _u16LockedCenterFreq=((u32Data+500)/1000);*/


            //Center Freq -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE13L,&u8Data);
            u32Data=u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE12H,&u8Data);
            u32Data = (u32Data<<8)|u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE12L,&u8Data);
            u32Data = (u32Data<<8)|u8Data;
            _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000); //Center Freq
            _u16LockedCenterFreq=((u32Data+500)/1000);


            /*
            u16Address=0x0B89;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B88;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(u16Data);//Symbol Rate
            _u16LockedSymbolRate=u16Data;
            _u16ChannelInfoIndex++;
            *u8FindNum=_u16ChannelInfoIndex;*/

            //symbol rate -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE14H,&u8Data);
            u16Data = u8Data;
            bRet &= ReadMailbox(E_DMD_S2_MB_SWUSE14L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(u16Data);//Symbol Rate
            _u16LockedSymbolRate = u16Data;
            _u16ChannelInfoIndex++;
            *u8FindNum = _u16ChannelInfoIndex;
            /*u16Address=0x0B8B;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B8A;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            if (u16Data*1000 >= 0x8000)
            {
                u16Data=0x10000- u16Data*1000;
                _s16CurrentCFO=-1*u16Data/1000;
            }
            else
            {
                _s16CurrentCFO=u16Data;
            }*/
            //current lock cfo -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE15H,&u8Data);
            u16Data = u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE15L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            if(u16Data*1000 >= 0x8000)
            {
            	u16Data = 0x10000 - (u16Data*1000);
            	_s16CurrentCFO = -1*u16Data/1000;
            }
            else
            {
            	_s16CurrentCFO = u16Data;
            }

            /*u16Address=0x0B8D;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B8C;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;*/
            //current step size  -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE16H,&u8Data);
            u16Data = u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE16L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;

            /*u16Address=0x0B91;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B90;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;*/
            //prelock HB  -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE18H,&u8Data);
            u16Data=u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE18L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;

            //prelock LB  -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE19H,&u8Data);
            u16Data=u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE19L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16PreLockedLB=u16Data;

            printf("Current Locked CF:%d BW:%d BWH:%d BWL:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB, _s16CurrentCFO, _u16CurrentStepSize);
            //PRINTE(("Current Locked CF:%d BW:%d BWH:%d BWL:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB, _s16CurrentCFO, _u16CurrentStepSize));


        }
        else if (u8Data==1)
        {
            u16Address=0x0993;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0992;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16NextCenterFreq=u16Data;


            //_u16CurrentSymbolRate -- mail box
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE14H,&u8Data);
            u16Data = u8Data;
            bRet &= ReadMailbox(E_DMD_S2_MB_SWUSE14L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16CurrentSymbolRate = u16Data;


            // cfo -- mail box
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE15H,&u8Data);
            u16Data = u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE15L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            if(u16Data*1000 >= 0x8000)
            {
            	u16Data = 0x10000 - (u16Data*1000);
            	_s16CurrentCFO = -1*u16Data/1000;
            }
            else
            {
            	_s16CurrentCFO = u16Data;
            }

            // current step size -- mail box
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE16H,&u8Data);
            u16Data = u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE16L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;


            //prelock HB  -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE12H,&u8Data);
            u16Data=u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE12L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;

            //prelock LB  -- mail box version
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE13H,&u8Data);
            u16Data=u8Data;
            bRet&=ReadMailbox(E_DMD_S2_MB_SWUSE13L,&u8Data);
            u16Data = (u16Data<<8)|u8Data;
            _u16PreLockedLB=u16Data;

            printf("Pre Locked CF:%d BW:%d HBW:%d LBW:%d Current CF:%d BW:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB,  _u16NextCenterFreq-_u16CurrentStepSize, _u16CurrentSymbolRate, _s16CurrentCFO, _u16CurrentStepSize);
            //PRINTE(("Pre Locked CF:%d BW:%d HBW:%d LBW:%d Current CF:%d BW:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB,  _u16NextCenterFreq-_u16CurrentStepSize, _u16CurrentSymbolRate, _s16CurrentCFO, _u16CurrentStepSize));
        }
    }
    *u8Progress=100;
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished u8Progress%d u8FindNum %d-\n", *u8Progress, *u8FindNum));
    //claire polling result
    /*
            u16Address=0x098f;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u32Data=u8Data;
            u16Address=0x098e;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            u16Address=0x0989;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            u16Address=0x0988;
            bRet&=MSB124X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
    PRINTE(("blind scan time :%d\n", u32Data));
    */
    //end claire
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_Cancel(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_Cancel+\n"));
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x0992;
    u16Data=0x0000;
    bRet&=MSB124X_WriteReg2bytes(u16Address, u16Data);
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    PRINTE(("MDrv_Demod_BlindScan_Cancel-\n"));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_End(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_End+\n"));
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x0992;
    u16Data=0x0000;
    bRet&=MSB124X_WriteReg2bytes(u16Address, u16Data);
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    PRINTE(("MDrv_Demod_BlindScan_End-\n"));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16TableIndex;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *u16TPNum=_u16ChannelInfoIndex-u16ReadStart;
    for(u16TableIndex = 0; u16TableIndex < (*u16TPNum); u16TableIndex++)
    {
        pTable[u16TableIndex].u32Frequency = _u16ChannelInfoArray[0][_u16ChannelInfoIndex-1];
        pTable[u16TableIndex].SatParam.u32SymbolRate = _u16ChannelInfoArray[1][_u16ChannelInfoIndex-1];
       PRINTE(("MDrv_Demod_BlindScan_GetChannel Freq:%d SymbolRate:%d\n", pTable[u16TableIndex].u32Frequency, pTable[u16TableIndex].SatParam.u32SymbolRate));
    }
    PRINTE(("MS124X u16TPNum:%d\n", *u16TPNum));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *u32CurrentFeq=_u16TunerCenterFreq;
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MSB124X_DiSEqC_Init(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    u16Address=0x0DCD; //Clear Status
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&(~0x2E))|(0x2E);
    bRet&=MSB124X_WriteReg(u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x00;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);

    //u16Address=0x0DC0;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&(~0x02))|(0x02);
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    return bRet;
}

MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U8 u8ReSet22k;

   u16Address=0x0DC4;
    u8Data=0x01;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x4E;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    u16Address=0x0DCC;
    u8Data=0x80;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);

    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8ReSet22k=u8Data;

    if (bTone1==TRUE)
    {
       u8Data=0x19;
       bRet&=MSB124X_WriteReg(u16Address, u8Data);
        _u8ToneBurstFlag=1;
    }
    else
    {
       u8Data=0x11;
       bRet&=MSB124X_WriteReg(u16Address, u8Data);
       _u8ToneBurstFlag=2;
    }
    u16Address=0x0DCD;
    u8Data=u8Data&~(0x01);
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x3E;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data&~(0x3E);
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(1);
    u8Data=u8Data|0x01;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(100);
    u16Address=0x0DC2;
    u8Data=u8ReSet22k;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);

    //MsOS_DelayTask(100);
    return bRet;
}
/*
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
         MsOS_DelayTask(1);
         u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB131X_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB131X_DEMOD_WAIT_TIMEOUT)
    {
        PRINTE(("MSB131X DiSEqC Send Tone Busrt Busy!!!\n"));
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }

    u16Address=0x0BC4;
    u8Data=0x01;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0B54;
    if (bTone1==TRUE)
    {
        u8Data=(0x40|0xA0);
    }
    else
    {
        u8Data=(0x40|0x20);
    }
    bRet&=MSB131X_WriteReg(u16Address, u8Data);

    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
*/
MS_BOOL MDrv_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if (bLow==TRUE)
    {
        u8Data=(u8Data|0x40);
    }
    else
    {
        u8Data=(u8Data&(~0x40));
    }
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if( (u8Data&0x40)==0x40)
    {
        * bLNBOutLow=TRUE;
    }
    else
    {
        * bLNBOutLow=FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if (b22kOn==TRUE)
    {
        u8Data=(u8Data|0x08);
    }
    else
    {
        u8Data=(u8Data&(~0x08));
    }
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x08)==0x08)
    {
        *b22kOn=TRUE;
    }
    else
    {
        *b22kOn=FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        MsOS_DelayTask(1);
        u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB124X_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB124X_DEMOD_WAIT_TIMEOUT)
    {
        PRINTE(("MSB124X DiSEqC Send Command Busy!!!\n"));
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }

    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u16Address=0x0DC4+u8Index;
        u8Data=*(pCmd+u8Index);
        bRet&=MSB124X_WriteReg(u16Address, u8Data);
    }
    u16Address=0x0B54;
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    if (_u8ToneBurstFlag==1)
    {
        u8Data|=0x80;//0x20;
    }
    else if (_u8ToneBurstFlag==2)
    {
        u8Data|=0x20;//0x80;
    }
    _u8ToneBurstFlag=0;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB124X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_Init(void)
{
    MS_BOOL bRet = true;

    sDMD_MSB124X_InitData sMSB124x_InitData;

    if (_s32TaskId >= 0)
    {
        PRINTE(("MSB124X TaskID 0x%x\n",_s32TaskId));
        return TRUE;
    }
    if (_bInited==TRUE)
    {
        PRINTE(("MSB124X Already Init 0x%x\n",_bInited));
        return TRUE;
    }
     _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_MSB124X",MSOS_PROCESS_SHARED);
     _s32FunMutexId=MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_MSB124X_Function",MSOS_PROCESS_SHARED);
    if ((_s32MutexId < 0) || (_s32FunMutexId <0))
    {
        PRINTE(("creat  MSB124X  Mutex error!!! _s32MutexId:%d _s32FunMutexId:%d\n", _s32MutexId, _s32FunMutexId));
        if (_s32MutexId >=0)
            MsOS_DeleteMutex(_s32MutexId);
        if (_s32FunMutexId >=0)
            MsOS_DeleteMutex(_s32FunMutexId);
        return FALSE;
    }
   // extern void MApi_Demod_HWReset(void);
   // MApi_Demod_HWReset();
   	
    _msb124x_hw_reset();
    MDrv_DMD_MSB124X_Exit();
//    printf("MSB124X Init\n");

    MsOS_DelayTask(10);

    sMSB124x_InitData.u8WO_SPI_Flash = TRUE;
    sMSB124x_InitData.bPreloadDSPCodeFromMainCHIPI2C = FALSE;
    sMSB124x_InitData.bFlashWPEnable = TRUE;
    sMSB124x_InitData.fpGPIOReset = NULL;
    sMSB124x_InitData.fpMSB124X_I2C_Access = msb124x_I2C_Access;
    sMSB124x_InitData.pDVBC_DSP_REG = NULL;
    sMSB124x_InitData.pDVBT_DSP_REG = NULL;
    sMSB124x_InitData.pDVBT2_DSP_REG = NULL;

    {
        sMSB124x_InitData.bEnableSPILoadCode = FALSE;
        sMSB124x_InitData.fpMSB124x_SPIPAD_En = NULL;
        printf(("HB MSB1245 I2C mode\n"));
    }

    bRet &= MDrv_DMD_MSB124X_Init(&sMSB124x_InitData, sizeof(sDMD_MSB124X_InitData));
    if (bRet)
    {
        MDrv_DMD_MSB124X_SetCurrentDemodulatorType(E_DMD_MSB124X_DEMOD_DVBS2);
        bRet &=MDrv_DMD_MSB1245_LoadDSPCodeToSram();
    }

    //set Tuner
    MsOS_DelayTask(100);
    MSB124X_IIC_Bypass_Mode(TRUE);

#if(FRONTEND_TUNER_TYPE == TUNER_AV2012)
    bRet &= MDrv_Tuner_AV2012_Init(0);
#else
    bRet &= MDrv_Tuner_Init();
#endif

    MSB124X_IIC_Bypass_Mode(FALSE);
    if (bRet == FALSE)
    {
        MsOS_DeleteMutex(_s32MutexId);
        MsOS_DeleteMutex(_s32FunMutexId);
    }
    //set DiSEqC
    bRet &= MSB124X_DiSEqC_Init();
    _bInited = TRUE;
    return bRet;
}

MS_BOOL MDrv_Demod_Open(void)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Close(void)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Reset(void)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);  //PRINTE(("MDrv_Demod_Reset\n"));
    return bRet;
}
MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
//    mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_GetSignalQuality(MS_U16 *pu16Quality)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pu16Quality=MSB124X_DTV_GetSignalQuality(_bDemodType);
    PRINTE(("MSB131X SNR %d\n", *pu16Quality));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_GetLock(EN_LOCK_STATUS *peLockStatus)
{
    MS_BOOL bRet=TRUE;
    *peLockStatus = E_DEMOD_CHECKING;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    if (MSB124X_DTV_GetLock())
    {
        *peLockStatus = E_DEMOD_LOCK;
    }
    PRINTE(("MSB124X MDrv_Demod_GetLock 0x%x\n", *peLockStatus));
    MsOS_ReleaseMutex(_s32FunMutexId);
	
    return bRet;
}
MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pu32SNR=MSB124X_DTV_GetSignalNoiseRatio();
    PRINTE(("MSB124X SNR %d\n", *pu32SNR));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Data;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Data=MSB124X_DTV_GetSignalQuality(_bDemodType);
    *pfBER=(float)u16Data;
    PRINTE(("MSB124X BER %f\n", *pfBER));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *ps32Signal=(MS_S32)MSB124X_DTV_GetSignalStrength();
    *ps32Signal=(-1)*(*ps32Signal)/10;
    PRINTE(("MSB124X Signal Level %d\n", *ps32Signal));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16SymbolRate;
    MS_U16 u16CenterFreq;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16SymbolRate = (pParam->SatParam.u32SymbolRate/1000);
    u16CenterFreq = pParam->u32Frequency;
    PRINTE(("MSB124X MDrv_CofdmDmd_Restart+ Fc:%d SymbolRate %d\n", u16CenterFreq, u16SymbolRate));
    MSB124X_IIC_Bypass_Mode(TRUE);
    bRet &= MSB124X_SetFrequency(pParam->u32Frequency,(pParam->SatParam.u32SymbolRate/1000));
    MSB124X_IIC_Bypass_Mode(FALSE);
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB124X_WriteReg(u16Address,u8Data);
	#if 0
    //MCU reset
    //MsOS_DelayTask(50);
    u16Address=0x0B32;
    u8Data = 0x03;
    bRet&=MSB124X_WriteReg(u16Address,u8Data);
    //MsOS_DelayTask(5);
    u16Address=0x0B32;
    u8Data = 0x00;
    bRet &= MSB124X_WriteReg(u16Address,u8Data);
    MsOS_DelayTask(50);
    //end MCU reset
	#endif
    u16Address=0x0B52;//DIG_DBG_2
    u8Data=(u16SymbolRate&0xFF);
    bRet&=MSB124X_WriteReg(u16Address,u8Data);
    u16Address = 0x0B53;//DIG_DBG_3
    u8Data = ((u16SymbolRate>>8)&0xFF);
    bRet &= MSB124X_WriteReg(u16Address,u8Data);

    //ADCPLL IQ swap
  //  if(MSB124X_ADCPLL_IQ_SWAP==1)
  //  {
        u16Address= 0x0A03;
        bRet&= MSB124X_ReadReg(u16Address, &u8Data);
        u8Data |= (0x10);
        bRet&= MSB124X_WriteReg(u16Address, u8Data);
   // }

    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    u8Data|=0x01;
    bRet&=MSB124X_WriteReg(u16Address,u8Data);
    #if MSB124X_TS_DATA_SWAP
    _bTSDataSwap=FALSE;
    #endif
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_Get_Packet_Error(MS_U16 *u16PktErr)
{
    MS_BOOL    bRet = TRUE;
    MS_U8     u8Data = 0;
    MS_U16    u16Data=0,u16Address = 0;
    float      Packet_Err=0;
    MS_U16     PRD;
    MS_U16     PER=0;

    if (_bIsDVBS2==FALSE)//S
    {
        u16Address=0x1c32;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data|=0x80;
        bRet&=MSB124X_WriteReg(u16Address, u8Data); //freeze

        // bank 17 0x18 [7:0] reg_bit_err_sblprd_7_0  [15:8] reg_bit_err_sblprd_15_8
        u16Address=0x1c30;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1c31;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data;

        PRD=u16Data;
        if (PRD == 0)    // avoide division by 0
            PRD++;

        // bank 17 0x1F [7:0] reg_uncrt_pkt_num_7_0 [15:8] reg_uncrt_pkt_num_15_8
        u16Address=0x1c3E;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1c3F;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data; //Packet error
        PER=u16Data;


        u16Address=0x1c32;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x80);
        bRet&=MSB124X_WriteReg(u16Address, u8Data); //unfreeze
        Packet_Err = (float)PER/(128*PRD);
    }
    else  //s2
    {

        u16Address=0x1d04;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data|=0x01;
        bRet&=MSB124X_WriteReg(u16Address, u8Data); //freeze


        u16Address=0x1d56;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1d57;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data;    //E-flag, error
        PER=u16Data;

        u16Address=0x1d04;
        bRet&=MSB124X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB124X_WriteReg(u16Address, u8Data); //unfreeze
    }

    *u16PktErr = PER;

    return bRet;
}

MS_U16 MDrv_Demod_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    MSB124X_ReadReg(RegAddr, &RegData);
    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{

    return MSB124X_WriteReg(RegAddr, RegData);
}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
   return MSB124X_IIC_Bypass_Mode(bOn);
}

MS_BOOL MDrv_Demod_DeInit(void)
{

    if (!_bInited)
    {
        PRINTE(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB124X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }


    if (MDrv_DMD_MSB124X_Exit())
    {
         _bInited = FALSE;
    }
    else
    {
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    MsOS_DeleteMutex(_s32FunMutexId);
    _s32FunMutexId = -1;
     return TRUE;
}


//=== mailbox function ===
MS_BOOL  ReadMailbox(MS_U16 u16Addr, MS_U8* u8Data)
{
    MS_BOOL     status = true;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;
    //MS_U16    bank;
    MS_U16    addr;
    MS_U8     data;
   // MsOS_ObtainMutex(_s32_Demod_DVBS2_RW_Mutex, MSOS_WAIT_FOREVER);


   addr = REG_MB_ADDR_H;
   data = ((u16Addr >> 8)& 0xff) ;
   status &= MSB124X_WriteReg(addr, data);


   addr = REG_MB_ADDR_L;
   data = u16Addr & 0xff;
   status &= MSB124X_WriteReg(addr, data);


  addr = REG_MB_CNTL;
  status &= MSB124X_WriteReg(addr, 0x03); // 3: param read



  addr = REG_MB_DATA ;
  status &=MSB124X_ReadReg(addr, u8Data);


    do
    {
        addr = REG_MB_CNTL ;
        status &=MSB124X_ReadReg(addr, &cntl);
        if (cntr++ > 0x7ff)
        {
            //PRINTE(("MSB131X_MB_WRITE_FAILURE\n"));
            //MsOS_ReleaseMutex(_s32_Demod_DVBS2_RW_Mutex);
            return FALSE;
        }
          addr = REG_MB_DATA ;
          status &=MSB124X_ReadReg(addr, u8Data);

    }
    while(cntl != 0xff);
    //MsOS_ReleaseMutex(_s32_Demod_DVBS2_RW_Mutex);
    return status;
}
#endif

