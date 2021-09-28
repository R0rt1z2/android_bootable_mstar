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
#include "Board.h"

//#if (FRONTEND_DEMOD_TYPE == DEMOD_MSKERES_DVBC)
#if ((defined(CONFIG_KERES) || defined(CONFIG_KIRIN) || defined(CONFIG_K1C))&&(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_INTERN_DVBC.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#if(FRONTEND_TUNER_TYPE == TUNER_MXL603)
#include "MxL603_TunerApi.h"
#define MXL603_I2C_ADDR         0xC0
#endif
#if(FRONTEND_TUNER_TYPE == TUNER_MXL608)
#include "../tuner/MxL608/MxL608_TunerApi.h"
#define MXL608_I2C_ADDR         0xC0
#endif


#define COFDMDMD_MUTEX_TIMEOUT       (2000)

#define QFP_PK_ENABLE 0

//////////////////////////////////////////////////////////
//
// USE_6M_ACIFILTER for dvbc 6M bandwidth, if bw is 6M,set it as "1",
//else set it as "0"
//
//////////////////////////////////////////////////////////
#define USE_6M_ACIFILTER  0
#define _BIT4           (0x00000010UL)


#define DEMOD_ADJUST_SSI   0           //to get if agc for ssi
#define MS_DEBUG_ENABLE     0          //for debug
#if MS_DEBUG_ENABLE
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif


static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;

#if ((FRONTEND_TUNER_TYPE == TUNER_PHILIPS_TDA18250HN) || (FRONTEND_TUNER_TYPE == TUNER_MXL603)|| (FRONTEND_TUNER_TYPE == TUNER_MXL608) || (FRONTEND_TUNER_TYPE == TUNER_R850) || (FRONTEND_TUNER_TYPE == TUNER_TDA18250AB) || (FRONTEND_TUNER_TYPE == TUNER_R836) || (FRONTEND_TUNER_TYPE == TUNER_SI2147))
const static MS_U32 u32IFrequency = 5000;      //KHz for demod restart&init
#elif(FRONTEND_TUNER_TYPE==TUNER_R820C)
const static MS_U32 u32IFrequency = 5070;
#else
const static MS_U32 u32IFrequency = 36130;    //KHz for demod restart&init
#endif

#if 0
static double _demod_log10(double flt_x)
{
    MS_U32      u32_temp = 1;
    MS_U8       indx = 0;

    do
    {
        u32_temp = u32_temp << 1;
        if (flt_x < (double)u32_temp)
            break;
    }
    while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (double)0.3 * indx;
}
#endif

static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
    { -100.00,    0x0}, //default setting
    { -100.00,    0xff}, //default setting
    /*
        {-15.00,    0x19},
        {-25.00,    0x31},
        {-26.00,    0x33},
        {-27.00,    0x35},
        {-28.00,    0x35},
        {-29.00,    0x36},
        {-30.00,    0x37},
        {-31.00,    0x38},
        {-32.00,    0x3A},
        {-33.00,    0x3E},
        {-34.00,    0x40},
        {-35.00,    0x41},
        {-36.00,    0x43},
        {-37.00,    0x44},
        {-38.00,    0x46},
        {-39.00,    0x47},
        {-40.00,    0x49},
        {-41.00,    0x4B},
        {-42.00,    0x4E},
        {-43.00,    0x50},
        {-44.00,    0x53},
        {-45.00,    0x56},
        {-46.00,    0x59},
        {-46.50,    0x5B},
        {-47.00,    0x5D},
        {-48.00,    0x62},
        {-49.00,    0x67},
        {-50.00,    0x6B},
        {-51.00,    0x73},
        {-52.00,    0x7A},
        {-53.00,    0x85},
        {-53.50,    0x8E},
        {-54.00,    0x98},
        {-54.10,    0x9F},
        {-54.20,    0xA4},
        {-54.30,    0xA7},
        {-54.40,    0xAC},
        {-55.00,    0xAC},
        {-55.00,    0xFF},*/
};

#if ((FRONTEND_TUNER_TYPE == TUNER_PHILIPS_TDA18250HN)|| (FRONTEND_TUNER_TYPE == TUNER_TDA18250AB))
static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    { -50.00,    0x00},
    { -51.00,    0x00},
    { -52.00,    0x00},
    { -53.00,    0x3F},
    { -53.50,    0x48},
    { -54.40,    0x4E},
    { -55.00,    0x51},
    { -56.00,    0x57},
    { -57.50,    0x5E},
    { -58.00,    0x60},
    { -58.50,    0x64},
    { -59.00,    0x6A},
    { -59.50,    0x6F},
    { -60.00,    0x73},
    { -61.00,    0x78},
    { -62.00,    0x7D},
    { -63.00,    0x80},
    { -64.00,    0x82},
    { -65.00,    0x85},
    { -66.00,    0x88},
    { -67.00,    0x89},
    { -68.00,    0x8b},
    { -69.00,    0x8C},
    { -70.00,    0x8E},
    { -71.00,    0x91},
    { -72.00,    0x94},
    { -73.00,    0x96},
    { -74.00,    0x97},
    { -75.00,    0x99},
    { -76.00,    0x9b},
    { -77.00,    0x9D},
    { -78.00,    0xA0},
    { -79.00,    0xa2},
    { -80.00,    0xa4},
    { -81.00,    0xa6},
    { -82.00,    0xa7},
    { -82.30,    0xa8},
    { -82.50,    0xA9},
    { -82.60,    0xAa},
    { -82.70,    0xAc},
    { -83.00,    0xFF},
};
#elif((FRONTEND_TUNER_TYPE == TUNER_MXL603) ||(FRONTEND_TUNER_TYPE == TUNER_MXL608))
static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    { 10.00,     0x00},
    { 10.00,     0x00},
    { -10.00,    0x88},
    { -20.00,    0x8f},
    { -30.00,    0x95},
    { -40.00,    0x9b},
    { -50.00,    0xa1},
    { -70.00,    0xac},
    { -100.00,   0xff},
};
#else
static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    { -50.00,     0x5d},
    { -51.00,    0x5e},
    { -52.00,    0x5F},
    { -53.00,    0x5F},
    { -54.00,    0x60},
    { -55.00,    0x63},
    { -56.00,    0x68},
    { -57.50,    0x6C},
    { -58.00,    0x70},
    { -59.00,    0x75},
    { -60.00,    0x79},
    { -61.40,    0x7D},
    { -62.00,    0x80},
    { -63.00,    0x81},
    { -64.00,    0x83},
    { -65.00,    0x84},
    { -66.00,    0x85},
    { -67.00,    0x87},
    { -68.00,    0x88},
    { -69.00,    0x89},
    { -70.00,    0x8A},
    { -71.00,    0x8C},
    { -72.00,    0x8D},
    { -73.00,    0x8F},
    { -74.00,    0x90},
    { -75.00,    0x92},
    { -76.00,    0x93},
    { -77.00,    0x95},
    { -78.00,    0x96},
    { -79.00,    0x98},
    { -80.00,    0x99},
    { -81.00,    0x9B},
    { -82.00,    0x9D},
    { -83.00,    0x9E},
    { -84.00,    0xA0},
    { -85.00,    0xA2},
    { -86.00,    0xA4},
    { -87.00,    0xA6},
    { -88.00,    0xA8},
    { -89.00,    0xAA},
    { -90.00,    0xAC},
    { -91.00,    0xAD},
    { -92.00,    0xAF},
    { -93.00,    0xB0},
    { -93.00,    0xFF},
};
#endif

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    { -54.30,    0x5F},
    { -54.40,    0x60},
    { -55.00,    0x63},
    { -56.00,    0x68},
    { -57.50,    0x6C},
    { -58.00,    0x70},
    { -59.00,    0x75},
    { -60.00,    0x79},
    { -61.40,    0x7D},
    { -62.00,    0x80},
    { -63.00,    0x81},
    { -64.00,    0x83},
    { -65.00,    0x84},
    { -66.00,    0x85},
    { -67.00,    0x87},
    { -68.00,    0x88},
    { -69.00,    0x89},
    { -70.00,    0x8A},
    { -71.00,    0x8C},
    { -72.00,    0x8D},
    { -73.00,    0x8F},
    { -74.00,    0x90},
    { -75.00,    0x92},
    { -76.00,    0x93},
    { -77.00,    0x95},
    { -78.00,    0x96},
    { -79.00,    0x98},
    { -80.00,    0x99},
    { -81.00,    0x9B},
    { -82.00,    0x9D},
    { -83.00,    0x9E},
    { -84.00,    0xA0},
    { -85.00,    0xA2},
    { -86.00,    0xA4},
    { -87.00,    0xA6},
    { -88.00,    0xA8},
    { -89.00,    0xAA},
    { -90.00,    0xAC},
    { -91.00,    0xAD},
    { -92.00,    0xAF},
    { -93.00,    0xB0},
    { -93.00,    0xFF},
};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 SqiCnNordigP1[] =
{
    {_QPSK,  _CR1Y2, 5.1 },
    {_QPSK,  _CR2Y3, 6.9 },
    {_QPSK,  _CR3Y4, 7.9 },
    {_QPSK,  _CR5Y6, 8.9 },
    {_QPSK,  _CR7Y8, 9.7 },
    {_16QAM, _CR1Y2, 10.8},
    {_16QAM, _CR2Y3, 13.1},
    {_16QAM, _CR3Y4, 12.2},
    {_16QAM, _CR5Y6, 15.6},
    {_16QAM, _CR7Y8, 16.0},
    {_64QAM, _CR1Y2, 16.5},
    {_64QAM, _CR2Y3, 16.3},
    {_64QAM, _CR3Y4, 17.8},
    {_64QAM, _CR3Y4, 21.2},
    {_64QAM, _CR5Y6, 21.6},
    {_64QAM, _CR7Y8, 22.5},
};

extern MS_BOOL MDrv_Demod_WriteDSPReg(MS_U16 RegAddr, MS_U16 RegData);
MS_BOOL MDrv_MSDVBC_51_Demod_Init(MS_U8 u8DemodIndex)
{
    DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
//            HB_GEN_EXCEP;
            return FALSE;
        }

    }

    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();

    static MS_U8 u8DMD_DVBC_InitExt[] =
    {
        4, // version
        0, // reserved
        0x15, // TS_CLK
        1, // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning


        (MS_U8)(u32IFrequency >> 24), // IF Frequency
        (MS_U8)(u32IFrequency >> 16),
        (MS_U8)(u32IFrequency >> 8),
        (MS_U8)(u32IFrequency >> 0),

#if QFP_PK_ENABLE
        (MS_U8)(48000 >> 24), // FS Frequency
        (MS_U8)(48000 >> 16),
        (MS_U8)(48000 >> 8),
        (MS_U8)(48000 >> 0),
#else
        (MS_U8)(45474 >> 24), // FS Frequency
        (MS_U8)(45474 >> 16),
        (MS_U8)(45474 >> 8),
        (MS_U8)(45474 >> 0),
#endif

        0, // IQ Swap
        1,//FRONTEND_DEMOD_IQ_TYPE,//0, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
        0, // u8PadSel : 0=Normal, 1=analog pad
        0, // bPGAEnable : 0=disable, 1=enable
        5, // u8PGAGain : default 5
        (MS_U8)(AutoSymbol_Timeout >> 8), // AutoSymbol_Timeout  10000ms~
        (MS_U8)(AutoSymbol_Timeout >> 0),
        (MS_U8)(FixSymbol_AutoQam_Timeout >> 8), // FixSymbol_AutoQam_Timeout 2000ms~
        (MS_U8)(FixSymbol_AutoQam_Timeout >> 0),
        (MS_U8)(FixSymbol_FixQam_Timeout >> 8), // FixSymbol_FixQam_Timeout  2000ms~
        (MS_U8)(FixSymbol_FixQam_Timeout >> 0),
    };                // tuner parameter

#if QFP_PK_ENABLE
    static MS_U8 u8DSPTable[] =
    {
        //addr_L add_H mask value
        0x01, 00,
        0x16, 0x00, 0xff, 0xBB, //FS_H  45.474M :0xB1  48M:0xBB
        0x15, 0x00, 0xff, 0x80, //FS_L  45.474M :0xA2  48M:0x80
    };                // tuner parameter
#else
    static MS_U8 u8DSPTable[] =
    {
        //addr_L add_H mask value
        0x01, 00,
        0x16, 0x00, 0xff, 0xB1, //FS_H  45.474M :0xB1  48M:0xBB
        0x15, 0x00, 0xff, 0xA2, //FS_L  45.474M :0xA2  48M:0x80
    };                // tuner parameter
#endif

    // tuner parameter
    sDMD_DVBC_InitData.u8SarChannel = 1; // 0xFF means un-connected
    sDMD_DVBC_InitData.pTuner_RfagcSsi = ALPS_TUNER_RfagcSsi;
    sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size = sizeof(ALPS_TUNER_RfagcSsi) / sizeof(DMD_RFAGC_SSI);

    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef = ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size = sizeof(ALPS_TUNER_IfagcSsi_LoRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef = ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size = sizeof(ALPS_TUNER_IfagcSsi_HiRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef = ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size = sizeof(ALPS_TUNER_IfagcErr_LoRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef = ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size = sizeof(ALPS_TUNER_IfagcErr_HiRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pSqiCnNordigP1 = SqiCnNordigP1;
    sDMD_DVBC_InitData.u16SqiCnNordigP1_Size = sizeof(SqiCnNordigP1) / sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt = u8DSPTable; // TODO use system variable type
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize = 2;
    sDMD_DVBC_InitData.u8DMD_DVBC_InitExt = u8DMD_DVBC_InitExt; // TODO use system variable type

    ret = MDrv_DMD_DVBC_Init(&sDMD_DVBC_InitData, sizeof(sDMD_DVBC_InitData)); // _UTOPIA
    if (ret == TRUE)
    {
        bInited = TRUE;
    }

#if MS_DEBUG_ENABLE
    MS_U16 demodfw;
    MDrv_DMD_DVBC_GetFWVer(&demodfw);
    printf("demod Fw=0x%x\n", demodfw);
#endif

    return ret;
}

MS_BOOL MDrv_MSDVBC_51_Demod_DeInit(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (MDrv_DMD_DVBC_Exit())
    {
        bInited = FALSE;
    }
    else
    {
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32MutexId);

    MsOS_DeleteMutex( _s32MutexId);
    _s32MutexId = -1;
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_51_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_MSDVBC_51_Demod_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_Exit();
    if (ret == TRUE)
    {
        bInited = FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_MSDVBC_51_Demod_SetSerialControl(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_SetSerialControl(bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_MSDVBC_51_Demod_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_51_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_DVBC_LOCK_STATUS LockStatus;
    *peLockStatus = E_DEMOD_CHECKING;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if (MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus) != TRUE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    switch (LockStatus)
    {
    case DMD_DVBC_LOCK:
        *peLockStatus = E_DEMOD_LOCK;
        break;
    case DMD_DVBC_CHECKING:
        *peLockStatus = E_DEMOD_CHECKING;
        break;
    case DMD_DVBC_CHECKEND:
        *peLockStatus = E_DEMOD_CHECKEND;
        break;
    case DMD_DVBC_UNLOCK:
        *peLockStatus = E_DEMOD_UNLOCK;
        break;
    default:
        *peLockStatus = E_DEMOD_CHECKING;
        break;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

static MS_U16 DTV_DVBC_GetSignalQuality(void)
{

    MS_U16 u16Signal = 0;
#if 0
    float fBER;
    float fLogBER;
#endif
    MS_BOOL bLock = FALSE;
    DMD_DVBC_LOCK_STATUS LockStatus;

    MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
    case DMD_DVBC_LOCK:
        bLock = TRUE;
        break;
    default:
        bLock = FALSE;
        break;
    }

    if (bLock)
    {
        MDrv_DMD_DVBC_GetSignalQuality(&u16Signal);//fix when dvbc 64QAM ,signal quality not stable
#if 0
        if (MDrv_DMD_DVBC_GetPostViterbiBer(&fBER) == FALSE)
        {
            DBG_MSB(HB_printf("MDrv_Dmd_GetBER = 0\n"));
            return 0;
        }

        fLogBER = (-1) * _demod_log10(1 / fBER); // Log10Approx() provide 1~2^32 input range only

        DBG_MSB(HB_printf("Log(BER) = %f\n", fLogBER));

        if (fLogBER <= (-7.0))              // PostVit BER < 1e-7
        {
            u16Signal = 100;
        }
        else  if (fLogBER < (-3.7))          // PostVit BER < 2e-4
        {
            u16Signal = 60 + (((-3.7) - fLogBER) / ((-3.7) - (-7.0)) * (100 - 60));
        }
        else  if (fLogBER < (-2.7))          // PostVit BER < 2e-3
        {
            u16Signal = 10 + (((-2.7) - fLogBER) / ((-2.7) - (-3.7)) * (60 - 10));
        }
        else
        {
            u16Signal = 10;
        }
#endif

    }
    else
    {
        u16Signal = 0;
    }

    DBG_MSB(HB_printf("Signal Quility = %d\n", (int)u16Signal));

    return u16Signal;
}

MS_BOOL MDrv_MSDVBC_51_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16Quality)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (bInited == FALSE)
    {
        *pu16Quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_GetSignalQuality(pu16Quality);
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_MSDVBC_51_Demod_GetSNR(MS_U8 u8DemodIndex,MS_U32 *pu32SNR)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (bInited == FALSE)
    {
        *pu32SNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    *pu32SNR = (MS_U32)DTV_DVBC_GetSignalQuality();

    //ret = MDrv_DMD_DVBC_GetSignalQuality((MS_U16*)pu32SNR);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_51_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (bInited == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_GetPostViterbiBer(pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

#if DEMOD_ADJUST_SSI
MS_U8 MDrv_MSDVBC_51_Demod_GetIFAGC(MS_U8 u8DemodIndex)
{
    MS_U8 u8Data;
    MDrv_DMD_DVBC_SetReg(0x2100 + 0x22, 0x03);
    MDrv_DMD_DVBC_GetReg(0x2100 + 0x25, &u8Data);
    printf("====IF AGC=0x%x\n", u8Data);
    return u8Data;
}
#endif

MS_BOOL MDrv_MSDVBC_51_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL ret = TRUE;;
    MS_BOOL bLock = FALSE;
    DMD_DVBC_LOCK_STATUS LockStatus;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
    case DMD_DVBC_LOCK:
        bLock = TRUE;
        break;
    default:
        bLock = FALSE;
        break;
    }

    if (bInited == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if (bLock)
    {
        MS_U16 u16signal = 0;
#if ((FRONTEND_TUNER_TYPE == TUNER_MXL603) ||(FRONTEND_TUNER_TYPE == TUNER_MXL608))
        short fSignal;
        *ps32Signal = 0;
#if (FRONTEND_TUNER_TYPE == TUNER_MXL603)
        if (MXL_SUCCESS != MxLWare603_API_ReqTunerRxPower(MXL603_I2C_ADDR, &fSignal))
        {
            DBG_MSB(HB_printf("MxLWare603_API_ReqTunerRxPower failed !!!\n"));
            return FALSE;
        }
#endif
#if (FRONTEND_TUNER_TYPE == TUNER_MXL608)
        if (MXL_SUCCESS != MxLWare608_API_ReqTunerRxPower(MXL608_I2C_ADDR, &fSignal))
        {
            DBG_MSB(HB_printf("MxLWare608_API_ReqTunerRxPower failed !!!\n"));
            return FALSE;
        }
#endif
        MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(&u16signal, (float)fSignal/100);
#else
        ret = MDrv_DMD_DVBC_GetSignalStrength(&u16signal);
#endif
        *ps32Signal = u16signal;
#if DEMOD_ADJUST_SSI
        MDrv_MSDVBC_51_Demod_GetIFAGC();
#endif
    }
    else
    {
        *ps32Signal = 0;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_MSDVBC_51_Demod_Config(MS_U8 u8DemodIndex,MS_U8 * pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_51_Demod_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM * pParam)
{
    DMD_DVBC_MODULATION_TYPE QAMMode;
    MS_U16 u16SymbolRate;
    float FreqOff;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    if (bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if (FALSE == MDrv_DMD_DVBC_GetStatus(&QAMMode, &u16SymbolRate, &FreqOff))
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (QAMMode)
    {
    case DMD_DVBC_QAM16:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM16;
        break;
    case DMD_DVBC_QAM32:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM32;
        break;
    case DMD_DVBC_QAM64:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM64;
        break;
    case DMD_DVBC_QAM128:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
        break;
    case DMD_DVBC_QAM256:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM256;
        break;
    case DMD_DVBC_QAMAUTO:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAMAUTO;
        break;
    default:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
        break;
    }

    pParam->CabParam.u16SymbolRate = u16SymbolRate;
    pParam->CabParam.u32FreqOffset = FreqOff;

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_MSDVBC_51_Demod_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM * pParam)
{
    DMD_DVBC_MODULATION_TYPE eModulationType;
    static DMD_DVBC_MODULATION_TYPE ePreModulationType = DMD_DVBC_QAMAUTO;
    static MS_U16 u16PreSymbolRate = 0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (pParam->CabParam.eConstellation)
    {
    case DEMOD_CAB_QAM16:
        eModulationType = DMD_DVBC_QAM16;
        break;
    case DEMOD_CAB_QAM32:
        eModulationType = DMD_DVBC_QAM32;
        break;
    case DEMOD_CAB_QAM64:
        eModulationType = DMD_DVBC_QAM64;
        break;
    case DEMOD_CAB_QAM128:
        eModulationType = DMD_DVBC_QAM128;
        break;
    case DEMOD_CAB_QAM256:
        eModulationType = DMD_DVBC_QAM256;
        break;
    default:
        eModulationType = DMD_DVBC_QAMAUTO;
        break;
    }

#if USE_6M_ACIFILTER
    MS_U8  u8Data;
    // bit0 means  SAWLESS_EN and bit4 means 6M_ACI_FILTER(1 means use 6M).
    MDrv_DMD_DVBC_GetDSPReg(0x0f, &u8Data);
    u8Data &= (~_BIT4);
    MDrv_Demod_WriteDSPReg(0x0f, u8Data | (USE_6M_ACIFILTER << 4));
#else
    MDrv_Demod_WriteDSPReg(0x0f, pParam->CabParam.u8DemodConfig);
#endif

    //if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    //{
    ePreModulationType = eModulationType;
    u16PreSymbolRate = pParam->CabParam.u16SymbolRate;
    printf("\npParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n", pParam->CabParam.u16SymbolRate, eModulationType);

    if (FALSE == MDrv_DMD_DVBC_SetConfig(pParam->CabParam.u16SymbolRate, eModulationType, u32IFrequency, FALSE, FALSE))
    {
        printf("MDrv_DMD_DVBC_SetConfig Fail \n");
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if (FALSE == MDrv_DMD_DVBC_SetActive(TRUE))
    {
        printf("MDrv_DMD_DVBC_SetActive Fail \n");
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    //}

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_MSDVBC_51_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_51_Demod_SetMode(MS_U8 u8DemodIndex,Demod_Mode * pMode)
{
#if 0
    DMD_Mode dmdMod;
    dmdMod.bX4CFE_en            = pMode->bX4CFE_en;
    dmdMod.bPPD_en              = pMode->bPPD_en;
    dmdMod.bIQAutoSwap_en       = pMode->bIQAutoSwap_en;
    dmdMod.bQAMScan_en          = pMode->bQAMScan_en;
    dmdMod.bFHO_en              = pMode->bFHO_en;
    dmdMod.fptTunerSet          = pMode->fptTunerSet;
    return MDrv_Dmd_SetMode((DMD_Mode*)pMode);
#else
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_MSDVBC_51_Demod_SetOutoutPath(MS_U8 u8DemodIndex,DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_MSDVBC_51_Demod_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_U16 MDrv_Demod_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    MDrv_DMD_DVBC_GetReg(RegAddr, &RegData); //MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{
    MS_U8 _RegData = RegData & 0x00FF;
    return MDrv_DMD_DVBC_SetReg(RegAddr, _RegData); //MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
}

MS_U16 MDrv_Demod_ReadDSPReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    MDrv_DMD_DVBC_GetDSPReg(RegAddr, &RegData); //MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_WriteDSPReg(MS_U16 RegAddr, MS_U16 RegData)
{
    MS_U8 _RegData = RegData & 0x00FF;
    return MDrv_DMD_DVBC_SetDSPReg(RegAddr, _RegData); //MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
}

MS_BOOL DEMOD_MSK1_DVBC_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            return TRUE;
    }
}

#ifndef GPIO_FE_RST
#define GPIO_FE_RST  PAD_PM_GPIO6
#endif
static void _demod_hw_reset(void)
{
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(100);
    mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_DelayTask(200);
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(100);
}

MS_BOOL Power_On_Initialization(void)
{
    _demod_hw_reset();
    MS_BOOL bOK = FALSE;
    bOK = MDrv_MSDVBC_51_Demod_Init(0);
    if(bOK == TRUE)
    {
        printf("DEMOD INIT OK!!!!!!\n");
        return TRUE;
    }
    else
    {
        printf("DEMOD INIT FAIL!!!!!!\n");
        return FALSE;
    }
}
MS_BOOL DTV_SetFrequency(MS_U32 dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    extern MS_U16 u16SymbolRate;
    extern MS_U32 u32QAM;
    DEMOD_MS_FE_CARRIER_PARAM stDemodData;
    switch(u32QAM)
    {
        case 16:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM16;
        break;
        case 32:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM32;
        break;
        case 64:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM64;
        break;
        case 128:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM128;
        break;
        case 256:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM256;
        break;
        default:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAMAUTO;
        break;
    }
    stDemodData.CabParam.u16SymbolRate = u16SymbolRate;
    stDemodData.CabParam.eIQMode = DEMOD_CAB_IQ_NORMAL;
    stDemodData.CabParam.u8TapAssign = 1;
    stDemodData.CabParam.u32FreqOffset = 0;
    stDemodData.CabParam.u8TuneFreqOffset = 0;
    stDemodData.CabParam.u8DemodConfig = 0;
    stDemodData.u32Frequency = (MS_U32)dwFrequency;
    MS_U8 u8count = 0;
    extern MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
    if(MDrv_Tuner_SetTuner(dwFrequency*1000, 2) == FALSE)
    {
        printf("%s: Set frequency failed.\n", __FUNCTION__);
        return FALSE;
    }
    while(MDrv_MSDVBC_51_Demod_Restart(0,&stDemodData)!= TRUE)
    {
        MsOS_DelayTask(100);
        u8count++;
        if(u8count == 100)
        {
            return FALSE;
        }
    }
    printf("Tune 2 RF ok!\n");
    return TRUE;
}
MS_BOOL MDrv_Demod_GetLock(void)
{
     EN_LOCK_STATUS eLockStatus = E_DEMOD_UNLOCK;
    if(TRUE != MDrv_MSDVBC_51_Demod_GetLock(0,&eLockStatus))
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
}
MS_U32 MDrv_Demod_GetSNR(void)
{
    MS_U32 u32Snr;
    MDrv_MSDVBC_51_Demod_GetSNR(0,&u32Snr);
    return u32Snr;
}

MS_U32 DTV_GetSignalQuality(void)
{
    MS_U32 u32Snr;
    MDrv_MSDVBC_51_Demod_GetSNR(0,&u32Snr);
    return u32Snr;
}
/*
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_DVBC) =//&*&*&*j1 DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSKERES_DVBC",
     .data                         = DEMOD_MSKERES_DVBC,
     .init                         = MDrv_MSDVBC_51_Demod_Init,
     .deinit                       = MDrv_MSDVBC_51_Demod_DeInit,
     .GetLock                      = MDrv_MSDVBC_51_Demod_GetLock,
     .GetSignalQuality             = MDrv_MSDVBC_51_Demod_GetSignalQuality,
     .GetSNR                       = MDrv_MSDVBC_51_Demod_GetSNR,
     .GetBER                       = MDrv_MSDVBC_51_Demod_GetBER,
     .GetPWR                       = MDrv_MSDVBC_51_Demod_GetPWR,
     .GetParam                     = MDrv_MSDVBC_51_Demod_GetParam,
     .Restart                      = MDrv_MSDVBC_51_Demod_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .Extension_Function           = DEMOD_MSK1_DVBC_Extension_Function,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};
*/
#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSKERES_DVBC)

