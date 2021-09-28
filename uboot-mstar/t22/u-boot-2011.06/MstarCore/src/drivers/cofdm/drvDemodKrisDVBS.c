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
#if defined(CONFIG_KRIS)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDemod.h"
#include "drvDMD_INTERN_DVBS.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include "../tuner/drvTuner.h"
//#if FRONTEND_TUNER_TYPE == TUNER_AV2028
//#include "drvTuner_AV2028.h"
//#endif
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;
typedef struct
{
    struct drv_tunertab_entry*                    pstTunertab;
    MS_U8                                         u32DmxInputPath;
} DEMOD_MS_INIT_PARAM;
static DEMOD_MS_INIT_PARAM   InitParam[1];
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS) DDI_DRV_TABLE_ENTRY(demodtab);


//#if ((FRONTEND_TUNER_TYPE == TUNER_AV2028 || FRONTEND_TUNER_TYPE == TUNER_MXL603 || FRONTEND_TUNER_TYPE == TUNER_TDA18250A|| FRONTEND_TUNER_TYPE == TUNER_TDA18260))
//const static MS_U32 _u32IFrequency = 5000;//KHz for demod restart&init
//#else
//const static MS_U32 _u32IFrequency = 36130;//KHz for demod restart&init
//#endif


static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
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
    {-55.00,    0xFF},
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
    {-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
    {-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
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

MS_BOOL MDrv_Kris_DVBS_Demod_Init(MS_U8 u8DemodIndex) 
{
    DMD_DVBS_InitData sDMD_DVBS_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            GEN_EXCEP;
            return FALSE;
        }
    }

    //if(pParam->pstTunertab == NULL)
    //    return FALSE;
    //else
    //    InitParam[u8DemodIndex].pstTunertab = pParam->pstTunertab;

    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();

    static MS_U8 u8DMD_DVBS_InitExt[]={
       4, // version
       0, // reserved
       0x15, // TS_CLK
       1, // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
       (MS_U8)(36167>>24), // IF Frequency
       (MS_U8)(36167>>16),
       (MS_U8)(36167>>8),
       (MS_U8)(36167>>0),
       (MS_U8)(24000>>24), // FS Frequency
       (MS_U8)(24000>>16),
       (MS_U8)(24000>>8),
       (MS_U8)(24000>>0),
       FRONTEND_DEMOD_IQ_SWAP, // IQ Swap
       FRONTEND_DEMOD_IQ_TYPE,//0, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
       0, // u8PadSel : 0=Normal, 1=analog pad
       0, // bPGAEnable : 0=disable, 1=enable
       5, // u8PGAGain : default 5
       (MS_U8)(AutoSymbol_Timeout>>8), // AutoSymbol_Timeout  10000ms~
       (MS_U8)(AutoSymbol_Timeout>>0),
       (MS_U8)(FixSymbol_AutoQam_Timeout>>8), // FixSymbol_AutoQam_Timeout 2000ms~
       (MS_U8)(FixSymbol_AutoQam_Timeout>>0),
       (MS_U8)(FixSymbol_FixQam_Timeout>>8), // FixSymbol_FixQam_Timeout  2000ms~
       (MS_U8)(FixSymbol_FixQam_Timeout>>0),
       };                // tuner parameter

    // tuner parameter
    sDMD_DVBS_InitData.u8SarChannel=1; // 0xFF means un-connected
    sDMD_DVBS_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBS_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBS_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitSize=0;
    sDMD_DVBS_InitData.u8DMD_DVBS_InitExt=u8DMD_DVBS_InitExt; // TODO use system variable type
    sDMD_DVBS_InitData.u32DjbStartAddr = 0x6000000;//INTERNAL_DVBS2_DJB_ADR;

    ret = MDrv_DMD_DVBS_Init(&sDMD_DVBS_InitData, sizeof(sDMD_DVBS_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }
    //MDrv_DMD_DVBC_IQ(FRONTEND_DEMOD_IQ_TYPE);
    
    int gpio_fe_rst_num = 117;
    mdrv_gpio_set_high(gpio_fe_rst_num);
    MsOS_DelayTask(100);
    mdrv_gpio_set_low(gpio_fe_rst_num);
    MsOS_DelayTask(200);
    mdrv_gpio_set_high(gpio_fe_rst_num);
    MsOS_DelayTask(100);

    return ret;
}

MS_BOOL MDrv_Demod_Init(void)
{
    return MDrv_Kris_DVBS_Demod_Init(0);
}

MS_BOOL MDrv_Kris_DVBS_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Kris_DVBS_Demod_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    ret = MDrv_DMD_DVBS_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Kris_DVBS_Demod_SetSerialControl(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBS_SetSerialControl(bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Kris_DVBS_Demod_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Kris_DVBS_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_DVBS_LOCK_STATUS LockStatus;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    if(MDrv_DMD_DVBS_GetLock(DMD_DVBS_GETLOCK, &LockStatus) != TRUE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    switch (LockStatus)
    {
        case DMD_DVBS_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_DVBS_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_DVBS_UNLOCK:         
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case DMD_DVBS_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetLock(EN_LOCK_STATUS *peLockStatus)
{
    MDrv_Kris_DVBS_Demod_GetLock(0, peLockStatus);
}

MS_BOOL MDrv_Kris_DVBS_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    ret = MDrv_DMD_DVBS_GetSNR(pfBER);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Kris_DVBS_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBS_GetPostViterbiBer(pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Kris_DVBS_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBS_GetSignalStrength((MS_U16*)ps32Signal);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Kris_DVBS_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBS_GetSignalQuality(pu16quality);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Kris_DVBS_Demod_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Kris_DVBS_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBS_MODULATION_TYPE QAMMode;
    MS_U32 u32SymbolRate;
    float FreqOff;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(FALSE == MDrv_DMD_DVBS_GetStatus(&QAMMode, &u32SymbolRate, &FreqOff))
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (QAMMode)
    {
        case DMD_DVBS_QPSK:
            pParam->SatParam.eConstellation = DEMOD_SAT_QPSK;
            break;
        case DMD_DVBS_8PSK:
            pParam->SatParam.eConstellation = DEMOD_SAT_8PSK;
            break;
        default:
            pParam->SatParam.eConstellation = DEMOD_SAT_QPSK;
            break;
    }

    pParam->SatParam.u32SymbolRate = u32SymbolRate;
    //pParam->CalParam.u32FreqOffset = FreqOff;

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_Kris_DVBS_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBS_MODULATION_TYPE eModulationType;
    static DMD_DVBS_MODULATION_TYPE ePreModulationType = DEMOD_SAT_QPSK;
    static MS_U16 u16PreSymbolRate = 0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch(pParam->SatParam.eConstellation)
    {
        case DEMOD_SAT_QPSK:
            eModulationType = DMD_DVBS_QPSK;
            break;
        case DEMOD_SAT_8PSK:
            eModulationType = DMD_DVBS_8PSK;
            break;
        default:
            eModulationType = DMD_DVBS_QPSK;
            break;
    }

    //printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType);

    //if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    //{
        ePreModulationType = eModulationType;
        u16PreSymbolRate = pParam->SatParam.u32SymbolRate;
 #if 0
        if(FALSE == MDrv_DMD_DVBS_Demod_Restart(pParam->SatParam.u32SymbolRate, pParam->u32Frequency))
        {
            printf("MDrv_DMD_DVBS_Demod_Restart Fail \n");
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
#endif

        if(FALSE == MDrv_DMD_DVBS_SetConfig(pParam->SatParam.u32SymbolRate, eModulationType, 0, FRONTEND_DEMOD_IQ_SWAP, FALSE))
        {
            DMD_ERR(("MDrv_DMD_DVBS_SetConfig Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(FALSE == MDrv_DMD_DVBS_SetActive(TRUE))
        {
            DMD_ERR(("MDrv_DMD_DVBS_SetActive Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MDrv_Kris_DVBS_Demod_Restart(0, pParam);
}

MS_BOOL MDrv_Kris_DVBS_Demod_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_Kris_DVBS_Demod_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Kris_DVBS_Demod_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_BOOL MDrv_Demod_Kris_BlindScan_Start(MS_U8 u8DemodIndex, MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    return MDrv_DMD_DVBS_BlindScan_Start(u16StartFreq,u16EndFreq);
}

MS_BOOL MDrv_Demod_Kris_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd)
{/*
    MS_BOOL bRet;
	MS_U16 u16TunerCenterFreq;

	MDrv_DMD_DVBS_BlindScan_GetTunerFreq(&u16TunerCenterFreq);

	//MDrv_Tuner_SetFreq(u16TunerCenterFreq, 34000);
    bRet = InitParam[u8DemodIndex].pstTunertab->SetFreqS2(u8DemodIndex,u16TunerCenterFreq, 34000);
    bRet &= MDrv_DMD_DVBS_BlindScan_NextFreq(bBlindScanEnd);
*/
    return TRUE;
}

MS_BOOL MDrv_Demod_Kris_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    return MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished(u8Progress,u8FindNum);
}

MS_BOOL MDrv_Demod_Kris_BlindScan_GetChannel(MS_U8 u8DemodIndex,MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    return MDrv_DMD_DVBS_BlindScan_GetChannel(u16ReadStart,u16TPNum,(HAL_DEMOD_MS_FE_CARRIER_PARAM*)pTable );
}

MS_BOOL MDrv_Demod_Kris_BlindScan_Cancel(MS_U8 u8DemodIndex)
{
    return MDrv_DMD_DVBS_BlindScan_Cancel();
}

MS_BOOL MDrv_Demod_Kris_BlindScan_End(MS_U8 u8DemodIndex)
{
    return MDrv_DMD_DVBS_BlindScan_End();
}

MS_BOOL MDrv_Demod_Kris_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex,MS_U32 *u32CurrentFeq)
{
    return MDrv_DMD_DVBS_BlindScan_GetCurrentFreq(u32CurrentFeq);
}

MS_BOOL MDrv_Demod_Kris_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1)
{
    return MDrv_DMD_DVBS_DiSEqC_SetTone(bTone1);
}

MS_BOOL MDrv_Demod_Kris_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow)
{
    return MDrv_DMD_DVBS_DiSEqC_SetLNBOut(bLow);
}

MS_BOOL MDrv_Demod_Kris_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow)
{
    return MDrv_DMD_DVBS_DiSEqC_GetLNBOut(bLNBOutLow);
}

MS_BOOL MDrv_Demod_Kris_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn)
{
    return MDrv_DMD_DVBS_DiSEqC_Set22kOnOff(b22kOn); 
}

MS_BOOL MDrv_Demod_Kris_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn)
{
    return MDrv_DMD_DVBS_DiSEqC_Get22kOnOff(b22kOn);
}

MS_BOOL MDrv_Demod_Kris_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    return MDrv_DMD_DVBS_DiSEqC_SendCmd(pCmd,u8CmdSize);
}

MS_BOOL MDrv_Demod_Kris_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            DMD_ERR(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }
}

MS_BOOL MDrv_Kris_DVBS_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
/*
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
 */
    return TRUE;
}
/*
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSKRIS_DVBS",
     .data                         = DEMOD_MSKRIS_DVBS,
     .init                         = MDrv_Kris_DVBS_Demod_Init,
     .GetLock                      = MDrv_Kris_DVBS_Demod_GetLock,
     .GetSNR                       = MDrv_Kris_DVBS_Demod_GetSNR,
     .GetBER                       = MDrv_Kris_DVBS_Demod_GetBER,
     .GetPWR                       = MDrv_Kris_DVBS_Demod_GetPWR,
     .GetQuality                   = MDrv_Kris_DVBS_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Kris_DVBS_Demod_GetParam,
     .Restart                      = MDrv_Kris_DVBS_Demod_Restart,
     .I2CByPass                    = MDrv_Kris_DVBS_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = MDrv_Demod_Kris_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_Kris_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_Kris_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_Kris_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_Kris_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_Kris_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_Kris_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_Kris_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_Kris_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_Kris_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_Kris_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_Kris_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_Kris_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_Kris_DiSEqC_SendCmd
#endif
};
*/
#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBS_51)

