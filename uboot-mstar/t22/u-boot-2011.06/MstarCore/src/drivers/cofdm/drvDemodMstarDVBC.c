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
#if (defined(CONFIG_KRATOS)&&(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))

#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBC.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"

#define COFDMDMD_MUTEX_TIMEOUT       (2000)
//------------------------------INTERNAL DVBC DMD PARAMETERS-----------------
#define RESERVED 0x00
#define INIT_TBL_VERIOSN        4
#define DMD_DVBC_TS_CLK         0x15
#define DMD_DVBC_TRI_STATE      1
#define DMD_DVBC_PAD_SEL        0           //0=Normal, 1=analog pad
#define DMD_DVBC_PGA_EN         DISABLE     // 0=disable, 1=enable
#define DMD_DVBC_PGA_GAIN       5
#define DMD_DVBC_SET_DSP_REG    DISABLE

/*typedef struct
{
    struct drv_tunertab_entry*                    pstTunertab;
    MS_U8                                         u32DmxInputPath;
} DEMOD_MS_INIT_PARAM;*/

static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;

//static DEMOD_MS_INIT_PARAM MS_DVBC_INIT_PARAM;
const static MS_U32 _u32IFrequency = 5000;

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


static unsigned char u8DSPTable[] =
{
#ifdef SET_DSP_REG
  #include "DVBC_DMD_DSP_Init_Reg.h"
#endif
};

static void _demod_hw_reset(void)
{
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(100);
    mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_DelayTask(200);
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(100);
}

MS_BOOL MDrv_MSDVBC_Demod_Init(MS_U8 u8DemodIndex)
{
    DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            printf("%s: Create mutex failed.\n", __FUNCTION__);
            return FALSE;
        }

    }

    MDrv_SYS_DMD_VD_MBX_Init();

    static MS_U8 u8DMD_DVBC_InitExt[]={
       INIT_TBL_VERIOSN,
       RESERVED,
       DMD_DVBC_TS_CLK,
       DMD_DVBC_TRI_STATE,
       RESERVED,
       RESERVED,
       RESERVED,
       RESERVED,
       RESERVED,
       RESERVED,
       RESERVED,
       RESERVED,
       FRONTEND_DEMOD_IQ_SWAP,
       FRONTEND_DEMOD_IQ_TYPE,
       DMD_DVBC_PAD_SEL,
       DMD_DVBC_PGA_EN,
       DMD_DVBC_PGA_GAIN,
       (MS_U8)(AutoSymbol_Timeout>>8), // AutoSymbol_Timeout  10000ms~
       (MS_U8)(AutoSymbol_Timeout>>0),
       (MS_U8)(FixSymbol_AutoQam_Timeout>>8), // FixSymbol_AutoQam_Timeout 2000ms~
       (MS_U8)(FixSymbol_AutoQam_Timeout>>0),
       (MS_U8)(FixSymbol_FixQam_Timeout>>8), // FixSymbol_FixQam_Timeout  2000ms~
       (MS_U8)(FixSymbol_FixQam_Timeout>>0),
       };                // tuner parameter

    // tuner parameter
    sDMD_DVBC_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBC_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBC_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    if(!DMD_DVBC_SET_DSP_REG)
    {
        sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt=NULL; // TODO use system variable type
        sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize=0;
    }
    else
    {
        sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt=u8DSPTable; // TODO use system variable type
        sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize= (MS_U8)((sizeof(u8DSPTable) - 2)/4);
    }
    sDMD_DVBC_InitData.u8DMD_DVBC_InitExt=u8DMD_DVBC_InitExt; // TODO use system variable type

    ret = MDrv_DMD_DVBC_Init(&sDMD_DVBC_InitData, sizeof(sDMD_DVBC_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }
    //MDrv_DMD_DVBC_IQ(FRONTEND_DEMOD_IQ_TYPE);

    return ret;
}

MS_BOOL MDrv_MSDVBC_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_MSDVBC_Demod_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }


    ret = MDrv_DMD_DVBC_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_SetSerialControl(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_SetSerialControl(bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_MSDVBC_Demod_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_DVBC_LOCK_STATUS LockStatus;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    if(MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus) != TRUE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    switch (LockStatus)
    {
        case DMD_DVBC_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_DVBC_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_DVBC_UNLOCK:
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case DMD_DVBC_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBC_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfSNR)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfSNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    ret = MDrv_DMD_DVBC_GetSNR(pfSNR);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_MSDVBC_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_GetPostViterbiBer(pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_MSDVBC_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBC_GetSignalStrength((MS_U16*)ps32Signal);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_MSDVBC_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16* u16quality)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *u16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBC_GetSignalQuality(u16quality);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_MSDVBC_Demod_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_MSDVBC_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBC_MODULATION_TYPE QAMMode;
    MS_U16 u16SymbolRate;
    float FreqOff;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }


    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(FALSE == MDrv_DMD_DVBC_GetStatus(&QAMMode, &u16SymbolRate, &FreqOff))
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

MS_BOOL MDrv_MSDVBC_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    DMD_DVBC_MODULATION_TYPE eModulationType;
    MS_U32 u32IF_Freq = 0;
    /*//mark for warning: set but not used
    static DMD_DVBC_MODULATION_TYPE ePreModulationType = DMD_DVBC_QAMAUTO;
    static MS_U16 u16PreSymbolRate = 0;
    */

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    extern MS_BOOL Mdrv_GetTunerIF(MS_U32* u32IF_Freq);
    if(!Mdrv_GetTunerIF(&u32IF_Freq))
    {
        u32IF_Freq = _u32IFrequency;
        printf("INTERN_DVBC Get Tuner IF FAIL, use default Tuner IF %d kHz\n",(int)u32IF_Freq);
    }
    else
    {
       printf("INTERN_DVBC Get Tuner IF %d kHz\n", (int)u32IF_Freq);
    }


    switch(pParam->CabParam.eConstellation)
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

    //printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType);

    //if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    //{
        /*//mark for warning: set but not used
        ePreModulationType = eModulationType;
        u16PreSymbolRate = pParam->CabParam.u16SymbolRate;
        */
        printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType);

        if(FALSE == MDrv_DMD_DVBC_SetConfig(pParam->CabParam.u16SymbolRate, eModulationType, u32IF_Freq, FALSE, FALSE))
        {
            printf("MDrv_DMD_DVBC_SetConfig Fail \n");
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(FALSE == MDrv_DMD_DVBC_SetActive(TRUE))
        {
            printf("MDrv_DMD_DVBC_SetActive Fail \n");
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
    //}

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}


MS_BOOL MDrv_MSDVBC_Demod_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
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
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_MSDVBC_Demod_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_MSDVBC_Demod_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_BOOL DEMOD_MSDVBC_DVBC_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_FINALIZE:
            if (_s32MutexId >= 0)
            {
                MsOS_DeleteMutex(_s32MutexId);
                _s32MutexId = -1;
            }
            bInited = FALSE;
            bret &= MDrv_DMD_DVBC_Exit();
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
    }
    return bret;
}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
     return TRUE;
}
MS_BOOL Power_On_Initialization(void)
{
    _demod_hw_reset();
    MS_BOOL bOK = FALSE;
    bOK = MDrv_MSDVBC_Demod_Init(0);
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
        case 0:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM16;
        break;
        case 1:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM32;
        break;
        case 2:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM64;
        break;
        case 3:
            stDemodData.CabParam.eConstellation = DEMOD_CAB_QAM128;
        break;
        case 4:
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
    while(MDrv_MSDVBC_Demod_Restart(0,&stDemodData,0)!= TRUE)
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
    if(TRUE != MDrv_MSDVBC_Demod_GetLock(0,&eLockStatus))
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
    MDrv_MSDVBC_Demod_GetSNR(0,&u32Snr);
    return u32Snr;
}

MS_U32 DTV_GetSignalQuality(void)
{
    MS_U32 u16Snr;
    MDrv_MSDVBC_Demod_GetSignalQuality(0,&u16Snr);
    return u16Snr;
}


/*DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSINTERN_DVBC",
     .data                         = DEMOD_MSINTERN_DVBC,
     .init                         = MDrv_MSDVBC_Demod_Init,
     .GetLock                      = MDrv_MSDVBC_Demod_GetLock,
     .GetSNR                       = MDrv_MSDVBC_Demod_GetSNR,
     .GetBER                       = MDrv_MSDVBC_Demod_GetBER,
     .GetPWR                       = MDrv_MSDVBC_Demod_GetPWR,
     .GetQuality                   = MDrv_MSDVBC_Demod_GetSignalQuality,
     .GetParam                     = MDrv_MSDVBC_Demod_GetParam,
     .Restart                      = MDrv_MSDVBC_Demod_Restart,
     .I2CByPass                    = MDrv_MSDVBC_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSDVBC_DVBC_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
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
};*/

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC_51)

