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
#if defined(CONFIG_K5TN) || defined(CONFIG_K5AP)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_INTERN_DVBT.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#if (MS_DVBT2_INUSE == 1)
#include "drvDMD_INTERN_DVBT2.h"
#endif
#define ALIGN(align, address)            (((address+align-1)/(align))*(align))

#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define DVBT2_TDI_LEN 0x00320000
#define DVBT2_DJB_LEN 0x00096000
#define DVBT2_EQ_LEN  0x00096000

#if defined(CONFIG_K5AP)
/*Because DVB-T and DVB-T2 F/W merge together ,so F/W size is bigger*/
#define DVBT2_FW_LEN  0x00020000
#else
#define DVBT2_FW_LEN  0x00010000
#endif

#define DVBT2_TDI_ALIGN 0x10000
#define DVBT2_DJB_ALIGN 0x01000
#define DVBT2_EQ_ALIGN  0x01000
#if defined(CONFIG_K5AP)
#define DVBT2_FW_ALIGN  0x010000
#else
#define DVBT2_FW_ALIGN  0x01000
#endif

static MS_BOOL bMstarDVBT2_Init = FALSE;
static MS_S32 _s32MutexId = -1;
//static EN_DEMOD_TYPE eCurDemodType = E_DEMOD_TYPE_T;
static MS_S8 eCurDemodType = -1;
static MS_U32 u32INTERNAL_DVBT2_EQ_ADR = NULL;
static MS_U32 u32INTERNAL_DVBT2_TDI_ADR = NULL;
static MS_U32 u32INTERNAL_DVBT2_DJB_ADR = NULL;
static MS_U32 u32INTERNAL_DVBT2_FW_ADR = NULL;


#if MS_DVBT2_INUSE
static MS_U8       PLPIDList[256];
static MS_U8       PLPIDSize = 0;
static MS_U8       stu8ScanStatus = 0;
#endif

#define IF_FREQUENCY  5000//KHz for demod restart&init
#define FS_FREQUENCY 24000
#define usleep(x)                   MsOS_DelayTask(x/1000)

#if defined(CHIP_KRITI)
#define MSKRITI_NO_CHANNEL_CHECK 1
#else
#define MSKRITI_NO_CHANNEL_CHECK 0
#endif

MS_S32 _u32LockTimeMax;
typedef struct
{
    struct drv_tunertab_entry*                    pstTunertab;
} DEMOD_MS_INIT_PARAM;
DEMOD_MS_INIT_PARAM MstarDVBT2_INIT_PARAM;
static DMD_RFAGC_SSI TUNER_RfagcSsi[] =
{
    {0,    0xff},
    {0,    0xff},

};

static DMD_IFAGC_SSI TUNER_IfagcSsi_LoRef[] =
{
//current agc ref setting:150
    {10.0,     0x00},  //default setting
    {10.0,     0x00},  //default setting
    {-00.0,    0x31},
    {-05.0,    0x42},
    {-10.0,    0x57},
    {-15.0,    0x6e},
    {-20.0,    0x80},
    {-25.0,    0x87},
    {-30.0,    0x8d},
    {-35.0,    0x93},
    {-40.0,    0x98},
    {-45.0,    0x9d},
    {-50.0,    0xa3},
    {-55.0,    0xa8},
    {-60.0,    0xad},
    {-65.0,    0xb2},
    {-70.0,    0xb7},
    {-75.0,    0xbc},
    {-80.0,    0xc1},
    {-85.0,    0xc6},
    {-90.0,    0xff}, //end setting
};

static DMD_IFAGC_SSI TUNER_IfagcSsi_HiRef[] =
{
    { -48,   0x3A},
    { -49,   0x3B},
    { -50,   0x3C},
    { -51,   0x3E},
    { -52,   0x40},
    { -53,   0x43},
    { -54,   0x45},
    { -55,   0x47},
    { -56,   0x49},
    { -57,   0x4C},
    { -58,   0x4f},
    { -59,   0x51},
    { -60,   0x54},
    { -61,   0x56},
    { -62,   0x59},
    { -63,   0x5C},
    { -64,   0x5F},
    { -65,   0x62},
    { -66,   0x65},
    { -67,   0x69},
    { -68,   0x6E},
    { -69,   0x73},
    { -70,   0x78},
    { -71,   0x7F},
    { -72,   0x85},
    { -73,   0x8b},
    { -74,   0x9c},
    { -85,   0xc0},
    { -99,   0xFF},
};

static DMD_IFAGC_ERR TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 DvbtSqiCnNordigP1[] =
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

#if (MS_DVBT2_INUSE == 1)
static DMD_T2_SQI_CN_NORDIGP1 Dvbt2SqiCnNordigP1[] =
{
    {_T2_QPSK, _T2_CR1Y2, 3.5},
    {_T2_QPSK, _T2_CR3Y5, 4.7},
    {_T2_QPSK, _T2_CR2Y3, 5.6},
    {_T2_QPSK, _T2_CR3Y4, 6.6},
    {_T2_QPSK, _T2_CR4Y5, 7.2},
    {_T2_QPSK, _T2_CR5Y6, 7.7},

    {_T2_16QAM, _T2_CR1Y2, 8.7},
    {_T2_16QAM, _T2_CR3Y5, 10.1},
    {_T2_16QAM, _T2_CR2Y3, 11.4},
    {_T2_16QAM, _T2_CR3Y4, 12.5},
    {_T2_16QAM, _T2_CR4Y5, 13.3},
    {_T2_16QAM, _T2_CR5Y6, 13.8},

    {_T2_64QAM, _T2_CR1Y2, 13.0},
    {_T2_64QAM, _T2_CR3Y5, 14.8},
    {_T2_64QAM, _T2_CR2Y3, 16.2},
    {_T2_64QAM, _T2_CR3Y4, 17.7},
    {_T2_64QAM, _T2_CR4Y5, 18.7},
    {_T2_64QAM, _T2_CR5Y6, 19.4},

    {_T2_256QAM, _T2_CR1Y2, 17.0},
    {_T2_256QAM, _T2_CR3Y5, 19.4},
    {_T2_256QAM, _T2_CR2Y3, 20.8},
    {_T2_256QAM, _T2_CR3Y4, 22.9},
    {_T2_256QAM, _T2_CR4Y5, 24.3},
    {_T2_256QAM, _T2_CR5Y6, 25.1},
};
#endif
static MS_U8 u8DMD_DVBT_InitExt[]= { 4, // version
                                     0, // reserved
                                     0x15, // TS_CLK
                                     1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
                                     (MS_U8)(IF_FREQUENCY>>24), // IF Frequency
                                     (MS_U8)(IF_FREQUENCY>>16),
                                     (MS_U8)(IF_FREQUENCY>>8),
                                     (MS_U8)(IF_FREQUENCY>>0),
                                     (MS_U8)(24000>>24), // FS Frequency
                                     (MS_U8)(24000>>16),
                                     (MS_U8)(24000>>8),
                                     (MS_U8)(24000>>0),
                                     FRONTEND_DEMOD_IQ_SWAP, // IQ Swap
                                     FRONTEND_DEMOD_IQ_TYPE, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
                                     0, // u8PadSel : 0=Normal, 1=analog pad
                                     0, // bPGAEnable : 0=disable, 1=enable
                                     5, // u8PGAGain : default 5
                                     (MS_U8)(DVBT_TPS_timeout>>8), // TPS timeout 700ms~
                                     (MS_U8)(DVBT_TPS_timeout>>0),
                                     (MS_U8)(DVBT_FEC_timeout>>8), // FEC timeout 6000ms~
                                     (MS_U8)(DVBT_FEC_timeout>>0),
                                   };

static MS_U8 u8DMD_DVBT_DSPRegInitExt[]= { 1, // version, should be matched with library
                                           0, // reserved

                                           E_DMD_DVBT_CFG_FC_L,    // Addr_L
                                           E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
                                           0xFF, // Mask
                                           0x88,
                                           E_DMD_DVBT_CFG_FC_H,    // Addr_L
                                           E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
                                           0xFF, // Mask
                                           0x13,
                                           E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
                                           E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H
                                           0xFF, // Mask
                                           0x01, // 0x00=I path, 0x01=Q path
                                         }; // Value

#if (MS_DVBT2_INUSE == 1)
static MS_U8 u8DMD_DVBT2_InitExt[]= { 3, // version
                                      0, // reserved
                                      0x15, // TS_CLK
                                      1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
                                      (MS_U8)(IF_FREQUENCY>>24), // IF Frequency
                                      (MS_U8)(IF_FREQUENCY>>16),
                                      (MS_U8)(IF_FREQUENCY>>8),
                                      (MS_U8)(IF_FREQUENCY>>0),
                                      (MS_U8)(24000>>24), // FS Frequency
                                      (MS_U8)(24000>>16),
                                      (MS_U8)(24000>>8),
                                      (MS_U8)(24000>>0),
                                      FRONTEND_DEMOD_IQ_TYPE, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
                                      0, // u8PadSel : 0=Normal, 1=analog pad
                                      0, // bPGAEnable : 0=disable, 1=enable
                                      5, // u8PGAGain : default 5
                                      (MS_U8)(DVBT_TPS_timeout>>8), // P1 timeout 700ms~
                                      (MS_U8)(DVBT_TPS_timeout>>0),
                                      (MS_U8)(DVBT_FEC_timeout>>8), // FEC timeout 6000ms~
                                      (MS_U8)(DVBT_FEC_timeout>>0),
                                    };

static MS_U8 u8DMD_DVBT2_DSPRegInitExt[]= { 1, // version, should be matched with library
                                            0, // reserved

                                            E_DMD_T2_FC_L,    // Addr_L
                                            E_DMD_T2_FC_L>>8, // Addr_H
                                            0xFF, // Mask
                                            0x88,
                                            E_DMD_T2_FC_H,    // Addr_L
                                            E_DMD_T2_FC_H>>8, // Addr_H
                                            0xFF, // Mask
                                            0x13,
                                          }; // Value
#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBT2) DDI_DRV_TABLE_ENTRY(demodtab);

static MS_BOOL _InitInternalDVBT(void)
{
    MS_BOOL bret = TRUE;
    DMD_DVBT_InitData sDMD_DVBT_InitData;
    // tuner parameter
    sDMD_DVBT_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBT_InitData.pTuner_RfagcSsi=TUNER_RfagcSsi;
    sDMD_DVBT_InitData.u16Tuner_RfagcSsi_Size=sizeof(TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_LoRef=TUNER_IfagcSsi_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_HiRef=TUNER_IfagcSsi_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_LoRef=TUNER_IfagcErr_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_HiRef=TUNER_IfagcErr_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pSqiCnNordigP1=DvbtSqiCnNordigP1;
    sDMD_DVBT_InitData.u16SqiCnNordigP1_Size=sizeof(DvbtSqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt=u8DMD_DVBT_DSPRegInitExt;
    // TODO use system variable type
    sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize=(sizeof(u8DMD_DVBT_DSPRegInitExt)-2)/4;
    sDMD_DVBT_InitData.u8DMD_DVBT_InitExt=u8DMD_DVBT_InitExt; // TODO use system variable type

    // Add for DVB-T/T2 merge FW. DVB-T needs DRAM addr for init.
#if defined(CONFIG_K5AP)
#if defined(INTERNAL_DVBT2_EQ_ADR) && defined(INTERNAL_DVBT2_TDI_ADR) &&\
    defined(INTERNAL_DVBT2_DJB_ADR) && defined(INTERNAL_DVBT2_FW_ADR)
    sDMD_DVBT_InitData.u32EqStartAddr = (MS_U32)INTERNAL_DVBT2_EQ_ADR;
    sDMD_DVBT_InitData.u32TdiStartAddr= (MS_U32)INTERNAL_DVBT2_TDI_ADR;
    sDMD_DVBT_InitData.u32DjbStartAddr= (MS_U32)INTERNAL_DVBT2_DJB_ADR;
    sDMD_DVBT_InitData.u32FwStartAddr= (MS_U32)INTERNAL_DVBT2_FW_ADR;
#else
    if((NULL == u32INTERNAL_DVBT2_EQ_ADR) || (NULL == u32INTERNAL_DVBT2_TDI_ADR) ||\
            (NULL == u32INTERNAL_DVBT2_DJB_ADR) || (NULL == u32INTERNAL_DVBT2_FW_ADR))
    {
        return FALSE;
    }
    else
    {
        sDMD_DVBT_InitData.u32EqStartAddr = (MS_U32)u32INTERNAL_DVBT2_EQ_ADR;
        sDMD_DVBT_InitData.u32TdiStartAddr= (MS_U32)u32INTERNAL_DVBT2_TDI_ADR;
        sDMD_DVBT_InitData.u32DjbStartAddr= (MS_U32)u32INTERNAL_DVBT2_DJB_ADR;
        sDMD_DVBT_InitData.u32FwStartAddr= (MS_U32)u32INTERNAL_DVBT2_FW_ADR;

        printf("[%s][%d] u32INTERNAL_DVBT_TDI_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_TDI_ADR);
        printf("[%s][%d] u32INTERNAL_DVBT_DJB_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_DJB_ADR);
        printf("[%s][%d] u32INTERNAL_DVBT_EQ_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_EQ_ADR);
        printf("[%s][%d] u32INTERNAL_DVBT_FW_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_FW_ADR);
    }
#endif
#endif

    bret &= MDrv_DMD_DVBT_Init(&sDMD_DVBT_InitData, sizeof(sDMD_DVBT_InitData));
    if(bret)
    {
        printf("[%s][%d] Internal DVBT DMD init OK\n",__FUNCTION__,__LINE__);
        bMstarDVBT2_Init = TRUE;
    }
    else
    {
        printf("[%s][%d] Internal DVBT DMD init FAIL\n",__FUNCTION__,__LINE__);
    }

    return bret;
}

#if MS_DVBT2_INUSE
static MS_BOOL _InitInternalDVBT2(void)
{
    MS_BOOL bret = TRUE;
    DMD_DVBT2_InitData sDMD_DVBT2_InitData;
    // tuner parameter
    sDMD_DVBT2_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBT2_InitData.pTuner_RfagcSsi=TUNER_RfagcSsi;
    sDMD_DVBT2_InitData.u16Tuner_RfagcSsi_Size=sizeof(TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBT2_InitData.pTuner_IfagcSsi_LoRef=TUNER_IfagcSsi_LoRef;
    sDMD_DVBT2_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT2_InitData.pTuner_IfagcSsi_HiRef=TUNER_IfagcSsi_HiRef;
    sDMD_DVBT2_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT2_InitData.pTuner_IfagcErr_LoRef=TUNER_IfagcErr_LoRef;
    sDMD_DVBT2_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT2_InitData.pTuner_IfagcErr_HiRef=TUNER_IfagcErr_HiRef;
    sDMD_DVBT2_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT2_InitData.pSqiCnNordigP1=Dvbt2SqiCnNordigP1;
    sDMD_DVBT2_InitData.u16SqiCnNordigP1_Size=sizeof(Dvbt2SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitExt=u8DMD_DVBT2_DSPRegInitExt; // TODO use system variable type
    sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitSize=(sizeof(u8DMD_DVBT2_DSPRegInitExt)-2)/4;
    sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt=u8DMD_DVBT2_InitExt; // TODO use system variable type


#if defined(INTERNAL_DVBT2_EQ_ADR) && defined(INTERNAL_DVBT2_TDI_ADR) &&\
    defined(INTERNAL_DVBT2_DJB_ADR) && defined(INTERNAL_DVBT2_FW_ADR)
    sDMD_DVBT2_InitData.u32EqStartAddr = (MS_U32)INTERNAL_DVBT2_EQ_ADR;
    sDMD_DVBT2_InitData.u32TdiStartAddr= (MS_U32)INTERNAL_DVBT2_TDI_ADR;
    sDMD_DVBT2_InitData.u32DjbStartAddr= (MS_U32)INTERNAL_DVBT2_DJB_ADR;
    sDMD_DVBT2_InitData.u32FwStartAddr= (MS_U32)INTERNAL_DVBT2_FW_ADR;
#else
    if((NULL == u32INTERNAL_DVBT2_EQ_ADR) || (NULL == u32INTERNAL_DVBT2_TDI_ADR) ||\
            (NULL == u32INTERNAL_DVBT2_DJB_ADR) || (NULL == u32INTERNAL_DVBT2_FW_ADR))
    {
        return FALSE;
    }
    else
    {
        sDMD_DVBT2_InitData.u32EqStartAddr = (MS_U32)u32INTERNAL_DVBT2_EQ_ADR;
        sDMD_DVBT2_InitData.u32TdiStartAddr= (MS_U32)u32INTERNAL_DVBT2_TDI_ADR;
        sDMD_DVBT2_InitData.u32DjbStartAddr= (MS_U32)u32INTERNAL_DVBT2_DJB_ADR;
        sDMD_DVBT2_InitData.u32FwStartAddr= (MS_U32)u32INTERNAL_DVBT2_FW_ADR;

        printf("[%s][%d] u32INTERNAL_DVBT2_TDI_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_TDI_ADR);
        printf("[%s][%d] u32INTERNAL_DVBT2_DJB_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_DJB_ADR);
        printf("[%s][%d] u32INTERNAL_DVBT2_EQ_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_EQ_ADR);
        printf("[%s][%d] u32INTERNAL_DVBT2_FW_ADR = 0x%x\n",__FUNCTION__,__LINE__,u32INTERNAL_DVBT2_FW_ADR);
    }
#endif

    bret = MDrv_DMD_DVBT2_Init(&sDMD_DVBT2_InitData, sizeof(sDMD_DVBT2_InitData));
    if(bret)
    {
        printf("[%s][%d] Internal DVBT2 DMD init OK\n",__FUNCTION__,__LINE__);
        bMstarDVBT2_Init = TRUE;
    }
    else
    {
        printf("[%s][%d] Internal DVBT2 DMD init FAIL\n",__FUNCTION__,__LINE__);
    }

    return bret;
}
#endif
MS_BOOL MDrv_MSDVBT2_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL ret = FALSE;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            DMD_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
            return FALSE;
        }

    }

    //MstarDVBT2_INIT_PARAM.pstTunertab = pParam->pstTunertab;
    MDrv_SYS_DMD_VD_MBX_Init();

#if MS_DVBT2_INUSE
    if(E_DEMOD_TYPE_T2 == eCurDemodType)
    {
        ret = _InitInternalDVBT2();
        if(ret)
        {
            printf("Internal DVBT2 DMD init OK\n");
        }
        else
        {
            printf("Internal DVBT2 DMD init FAIL\n");
        }
    }
    else
#endif
    {
        ret = _InitInternalDVBT();
        if(ret)
        {
            eCurDemodType = E_DEMOD_TYPE_T;
            printf("Internal DVBT DMD init OK\n");
        }
        else
        {
            printf("Internal DVBT DMD init FAIL\n");
        }
    }

    if(ret == TRUE)
    {
        bMstarDVBT2_Init = TRUE;
    }

    return ret;
}

#ifndef GPIO_FE_RST
#define GPIO_FE_RST PAD_I2CM1_SDA
#endif
void MApi_Demod_HWReset(MS_U8 u8Port_Idx)
{
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(5);
}

MS_BOOL MDrv_Demod_Init(void)
{
    MApi_Demod_HWReset(0);
    return MDrv_MSDVBT2_Demod_Init(0, NULL);
}

MS_BOOL MDrv_MSDVBT2_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBT2_Demod_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret = true;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret &= MDrv_DMD_DVBT_Exit();
#if MS_DVBT2_INUSE
    ret &= MDrv_DMD_DVBT2_Exit();
#endif
    if(ret == TRUE)
    {
        bMstarDVBT2_Init = FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return ret;
}

MS_BOOL MDrv_MSDVBT2_Demod_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

#if MS_DVBT2_INUSE
#if MSKRITI_NO_CHANNEL_CHECK
MS_BOOL MDrv_Demod_GetNoChannelFlag(EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
{
    MS_U16        u16Address = 0;
    MS_U8         cData = 0;
    MS_U8         cBitMask = 0;
    MS_U8         use_dsp_reg = 0;
    switch (eStatus)
    {
        case COFDM_DVBT2_NOCH_FLAG:
            use_dsp_reg = 1;
            u16Address = T_DVBT2_NOCHAN_Flag; //Pl ever lock,
            cBitMask = 0x0001;
            break;

        case COFDM_DVBT_NOCH_FLAG:
            use_dsp_reg = 1;
            u16Address = T_DVBT_NOCHAN_Flag; // No DVBT CH Flag,
            cBitMask = 0x0001;
            break;
        case COFDM_DETECT_DONE_FLAG:
            use_dsp_reg = 1;
            u16Address = T_DETECT_DONE_FLAG; // No DVBT CH Flag,
            cBitMask = 0x0001;
            break;

        default:
            return FALSE;
    }

    if (use_dsp_reg == 1)
    {
        if (MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16Address, &cData) == FALSE)
        {
            printf(">INTERN_DVBT2_GetLock MBX_ReadDspReg fail \n");
            return FALSE;
        }
    }
    else
    {
        if (MDrv_SYS_DMD_VD_MBX_ReadReg(u16Address, &cData) == FALSE)
        {
            printf(">INTERN_DVBT2_GetLock MBX_ReadReg fail \n");
            return FALSE;
        }
    }
    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }
    return FALSE;
}
#endif
#endif

MS_BOOL MDrv_MSDVBT2_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    DMD_LOCK_STATUS LockStatus;
#if MS_DVBT2_INUSE
    DMD_T2_LOCK_STATUS T2LockStatus;
#endif

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(E_DEMOD_TYPE_T == eCurDemodType)
    {
#if (MS_DVBT2_INUSE == 1)
#if MSKRITI_NO_CHANNEL_CHECK
        MS_BOOL doneFlag;

        MS_BOOL tChannelFlag=FALSE;
        MS_BOOL t2ChannelFlag=FALSE;

        doneFlag = MDrv_Demod_GetNoChannelFlag(E_DEVICE_DEMOD_DVB_T,COFDM_DETECT_DONE_FLAG);
        if (doneFlag)
        {
            tChannelFlag = MDrv_Demod_GetNoChannelFlag(E_DEVICE_DEMOD_DVB_T,COFDM_DVBT_NOCH_FLAG);
            t2ChannelFlag = MDrv_Demod_GetNoChannelFlag(E_DEVICE_DEMOD_DVB_T,COFDM_DVBT2_NOCH_FLAG);

            printf("NoChannel stable .... T[%d] T2[%d] \n",tChannelFlag,t2ChannelFlag);
        }
#endif
#endif
        MDrv_DMD_DVBT_GetLock(E_DMD_DMD_DVBT_GETLOCK, &LockStatus);
        switch (LockStatus)
        {
            case E_DMD_LOCK:
                *peLockStatus = E_DEMOD_LOCK;
                //_u32LockTimeMax = DVBT_FEC_timeout;
                break;
            case E_DMD_CHECKEND:
                *peLockStatus = E_DEMOD_CHECKEND;
                break;
            case E_DMD_UNLOCK:
                *peLockStatus = E_DEMOD_UNLOCK;
#if (MS_DVBT2_INUSE == 1)
#if MSKRITI_NO_CHANNEL_CHECK
                if ( doneFlag )
                {
                    if ( t2ChannelFlag )
                    {
                        *peLockStatus = E_DEMOD_T_T2_UNLOCK;
                    }
                }
#endif
#endif
                break;
            case E_DMD_CHECKING:
#if (MS_DVBT2_INUSE == 1)
#if MSKRITI_NO_CHANNEL_CHECK
                if ( doneFlag && tChannelFlag )
                {
                    if ( t2ChannelFlag )
                    {
                        *peLockStatus = E_DEMOD_T_T2_UNLOCK;
                    }
                    else
                    {
                        *peLockStatus = E_DEMOD_UNLOCK;
                    }
                    break;
                }
#endif
#endif
            default:
                *peLockStatus = E_DEMOD_CHECKING;
                break;
        }
    }
#if MS_DVBT2_INUSE
    else
    {
        MDrv_DMD_DVBT2_GetLock(E_DMD_DVBT2_GETLOCK, &T2LockStatus);
        switch (T2LockStatus)
        {
            case E_DMD_T2_LOCK:
                *peLockStatus = E_DEMOD_LOCK;
                //_u32LockTimeMax = DVBT_FEC_timeout;
                break;
            case E_DMD_T2_CHECKEND:
                *peLockStatus = E_DEMOD_CHECKEND;
                break;
            case E_DMD_T2_UNLOCK:
                *peLockStatus = E_DEMOD_UNLOCK;
                //_u32LockTimeMax = DVBT_TPS_timeout;
                break;
            case E_DMD_T2_CHECKING:
            default:
                *peLockStatus = E_DEMOD_CHECKING;
                break;
        }
    }
#endif
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_GetLock(EN_LOCK_STATUS *peLockStatus)
{
    return MDrv_MSDVBT2_Demod_GetLock(0, peLockStatus);
}

MS_BOOL MDrv_MSDVBT2_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
    MS_BOOL ret = TRUE;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        *pfSNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(E_DEMOD_TYPE_T == eCurDemodType)
        ret = MDrv_DMD_DVBT_GetSNR(pfSNR);
#if MS_DVBT2_INUSE
    else
        ret = MDrv_DMD_DVBT2_GetSNR(pfSNR);
#endif
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_MSDVBT2_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL ret= TRUE;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(E_DEMOD_TYPE_T == eCurDemodType)
        ret = MDrv_DMD_DVBT_GetPostViterbiBer(pfBER);
#if MS_DVBT2_INUSE
    else
        ret = MDrv_DMD_DVBT2_GetPostLdpcBer(pfBER);
#endif
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_MSDVBT2_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
#if 0
    MS_BOOL ret= TRUE;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MstarDVBT2_INIT_PARAM.pstTunertab->Extension_Function(u8DemodIndex, TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal);

    HB_ReleaseMutex(_s32MutexId);

    return ret;
#endif
    return TRUE;
}

MS_BOOL MDrv_MSDVBT2_Demod_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MS_BOOL ret= TRUE;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        *pu16SSI = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if(E_DEMOD_TYPE_T == eCurDemodType)
        ret = MDrv_DMD_DVBT_GetSignalStrength(pu16SSI);
#if MS_DVBT2_INUSE
    else
        ret = MDrv_DMD_DVBT2_GetSignalStrength(pu16SSI);
#endif
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}



MS_BOOL MDrv_MSDVBT2_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
{
    MS_BOOL ret = FALSE;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(E_DEMOD_TYPE_T == eCurDemodType)
        ret = MDrv_DMD_DVBT_GetSignalQuality(pu16quality);
#if MS_DVBT2_INUSE
    else
        ret = MDrv_DMD_DVBT2_GetSignalQuality(pu16quality);
#endif
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_MSDVBT2_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL bPalBG = FALSE; //unknown variable
    DMD_RF_CHANNEL_BANDWIDTH BW = E_DMD_RF_CH_BAND_8MHz;
#if MS_DVBT2_INUSE
    DMD_DVBT2_RF_CHANNEL_BANDWIDTH BWT2 = E_DMD_T2_RF_BAND_8MHz;
    //DMD_T2_LOCK_STATUS LockStatus = E_DMD_T2_NULL;
#endif
    MS_BOOL bret = TRUE;
    //static DEMOD_EN_TER_BW_MODE cur_BW = 0xff;


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bMstarDVBT2_Init == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

#if 0
#ifdef DVBT2_STYLE
    if (DEMOD_BW_MODE_1_7MHZ == pParam->TerParam.eBandWidth)
    {
        _SetCurrentDemodType(E_DEMOD_TYPE_T2);
    }
    else if ((E_DEMOD_TYPE_T == pParam->TerParam.u8ScanType) && (eCurDemodType != pParam->TerParam.u8ScanType))
    {
        _SetCurrentDemodType(E_DEMOD_TYPE_T);
    }
    else if ((E_DEMOD_TYPE_T2 == pParam->TerParam.u8ScanType) && (eCurDemodType != pParam->TerParam.u8ScanType))
    {
        _SetCurrentDemodType(E_DEMOD_TYPE_T2);
    }
#endif
    //set freq and bw to tuner

#ifdef DVBT2_STYLE
    if(MDrv_DMD_DVBT2_GetLock(E_DMD_DVBT2_GETLOCK, &LockStatus) && (eCurDemodType == E_DEMOD_TYPE_T2))
    {
        if((E_DMD_T2_LOCK != LockStatus) || (eCurDemodType != pParam->TerParam.u8ScanType) ||\
                (tuner_config.u32CurrFreq != pParam->u32Frequency) || (tuner_config.enCurrBW != pParam->TerParam.eBandWidth))
        {
            MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
        }
    }
    else
    {
        MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
    }
#else
    MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
#endif

    tuner_config.u32CurrFreq = pParam->u32Frequency;
    tuner_config.enCurrBW = pParam->TerParam.eBandWidth;
#endif
    // ("------pParam->u32Frequency [%d] pParam->TerParam.eBandWidth[%d] ------\n",pParam->u32Frequency,pParam->TerParam.eBandWidth);
#if MS_DVBT2_INUSE
    extern MS_BOOL MDrv_MSDVBT2_Demod_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8  type);
    if ( u32BroadCastType == DVBT )
    {
        MDrv_MSDVBT2_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T);
    }
    else
    {
        MDrv_MSDVBT2_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T2);
    }
#endif

    //if(cur_BW != pParam->TerParam.eBandWidth)
    {
        switch (pParam->TerParam.eBandWidth)
        {
            case DEMOD_BW_MODE_6MHZ:
                BW = E_DMD_RF_CH_BAND_6MHz;
#if MS_DVBT2_INUSE
                BWT2 = E_DMD_T2_RF_BAND_6MHz;
#endif
                break;
            case DEMOD_BW_MODE_7MHZ:
                BW = E_DMD_RF_CH_BAND_7MHz;
#if MS_DVBT2_INUSE
                BWT2 = E_DMD_T2_RF_BAND_7MHz;
#endif
                break;
#if MS_DVBT2_INUSE
            case DEMOD_BW_MODE_1_7MHZ:
                BWT2 = E_DMD_T2_RF_BAND_1p7MHz;
                break;
#endif
            case DEMOD_BW_MODE_8MHZ:
            default:
                BW = E_DMD_RF_CH_BAND_8MHz;
#if MS_DVBT2_INUSE
                BWT2 = E_DMD_T2_RF_BAND_8MHz;
#endif
                break;
        }

        if(E_DEMOD_TYPE_T == eCurDemodType)
        {
            //bret &= MDrv_DMD_DVBT_SetConfig(BW, FALSE, bPalBG );
            bret &= MDrv_DMD_DVBT_SetConfigHPLP(BW, FALSE, bPalBG, pParam->TerParam.eLevelSel);
            bret &= MDrv_DMD_DVBT_SetActive(TRUE);
        }
#if MS_DVBT2_INUSE
        else
        {
            bret &= MDrv_DMD_DVBT2_SetConfig(BWT2, FALSE, pParam->TerParam.u8PlpID);
            bret &= MDrv_DMD_DVBT2_SetActive(TRUE);
        }
        //cur_BW = pParam->TerParam.eBandWidth;

#endif
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    return MDrv_MSDVBT2_Demod_Restart(0, pParam, (eCurDemodType == E_DEMOD_TYPE_T) ? DVBT : DVBT2);
}

#if MS_DVBT2_INUSE
MS_BOOL  MDrv_MSDVBT2_Demod_SetT2Reset(MS_U8 u8DemodIndex)
{
    if (E_DEMOD_TYPE_T2 == eCurDemodType)
        return MDrv_DMD_DVBT2_SetReset();
    else
        return FALSE;
}

MS_BOOL MDrv_MSDVBT2_Demod_SetT2Restart(MS_U8 u8DemodIndex)
{
    MS_BOOL bret = TRUE;

    if (E_DEMOD_TYPE_T == eCurDemodType)
        return bret;

    bret &= MDrv_DMD_DVBT2_SetActive(TRUE);
    return bret;
}

MS_BOOL MDrv_MSDVBT2_Demod_InitParameter(MS_U8 u8DemodIndex)
{
    stu8ScanStatus = 0;
    return TRUE;
}

MS_BOOL MDrv_MSDVBT2_Demod_GetNextPlpID(MS_U8 u8DemodIndex, MS_U8 u8Index, MS_U8* pList)
{
    *pList = PLPIDList[u8Index];
    return TRUE;
}

MS_U8 MDrv_MSDVBT2_Demod_GetScanTypeStatus(MS_U8 u8DemodIndex)
{
    return stu8ScanStatus;
}

MS_BOOL MDrv_MSDVBT2_Demod_SetScanTypeStatus(MS_U8 u8DemodIndex, MS_U8 status)
{
    switch (status)
    {
        case 0:
            stu8ScanStatus = 0;
            break;
        case 1:
            stu8ScanStatus = 1;
            break;

        case 2:
            stu8ScanStatus = 2;
            break;

        case 3:
            stu8ScanStatus = 3;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

MS_BOOL MDrv_MSDVBT2_Demod_GetPlpIDList(MS_U8 u8DemodIndex, MS_U8 *pSize)
{
    MS_U8 i, j, u8PlpBitMap[32];

    PLPIDSize = 0;
    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));
    if (FALSE == MDrv_DMD_DVBT2_GetPlpBitMap(u8PlpBitMap))
    {
        DMD_ERR(("MDrv_DMD_DVBT2_GetPlpBitMap fail\n"));
        return FALSE;
    }

    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((u8PlpBitMap[i] >> j) & 1)
            {
                PLPIDList[PLPIDSize] = i * 8 + j;
                PLPIDSize++;
            }
        }
    }

    *pSize = PLPIDSize;
    return TRUE;
}

MS_BOOL MDrv_MSDVBT2_Demod_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8 type)
{
    MS_BOOL bret = TRUE;
    if (eCurDemodType == type)
    {
        return bret;
    }
    else
    {
        if (E_DEMOD_TYPE_T == eCurDemodType)
        {
            bret &= MDrv_DMD_DVBT_Exit();
            bret = _InitInternalDVBT2();
            eCurDemodType = type;
        }
        else
        {
            bret &= MDrv_DMD_DVBT2_Exit();
            bret = _InitInternalDVBT();
            eCurDemodType = type;
        }
    }

    if (bret == FALSE)
    {
        DMD_ERR(("Set Demod Type %x ERROR\n",eCurDemodType));
    }
    return bret;
}

MS_BOOL MDrv_Demod_SetCurrentDemodType(MS_U8 type)
{
    return MDrv_MSDVBT2_Demod_SetCurrentDemodType(0, type);
}

MS_U8 MDrv_MSDVBT2_Demod_GetCurrentDemodType(MS_U8 u8DemodIndex)
{
    return eCurDemodType;
}

MS_BOOL MDrv_MSDVBT2_Demod_GetPlpBitMap(MS_U8 u8DemodIndex,MS_U8* u8PlpBitMap)
{
    return MDrv_DMD_DVBT2_GetPlpBitMap(u8PlpBitMap);
}

MS_BOOL MDrv_MSDVBT2_Demod_GetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16Retry = 0;

    if ((u8PlpID != 0xFF) && (E_DEMOD_TYPE_T2 == eCurDemodType))
    {
        do
        {
            u16Retry++;
            usleep(100 * 1000);
            bRet = MDrv_DMD_DVBT2_GetPlpGroupID(u8PlpID, u8GroupID);
        }
        while ((bRet == FALSE) && (u16Retry < 60));

        if (bRet == FALSE)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

MS_BOOL MDrv_MSDVBT2_Demod_SetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    if(E_DEMOD_TYPE_T2 == eCurDemodType)
        return MDrv_DMD_DVBT2_SetPlpID(u8PlpID, u8GroupID);
    else
        return FALSE;
}

#endif
MS_BOOL DEMOD_MSDVBT2_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;

    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_FINALIZE:
            MDrv_MSDVBT2_Demod_Close(u8DemodIndex);
            if(_s32MutexId >= 0)
            {
                bret &= MsOS_DeleteMutex(_s32MutexId);
                _s32MutexId = -1;
            }
            break;
#ifdef MS_DVBT2_INUSE
        case DEMOD_EXT_FUNC_T2_RESET:
            bret &= MDrv_MSDVBT2_Demod_SetT2Reset(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_T2_RESTART:
            bret &= MDrv_MSDVBT2_Demod_SetT2Restart(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_INIT_PARAMETER:
            bret &= MDrv_MSDVBT2_Demod_InitParameter(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_GET_PLPID_LIST:
            bret &= MDrv_MSDVBT2_Demod_GetPlpIDList(u8DemodIndex, (MS_U8 *)data);
            break;
#endif
        case DEMOD_EXT_FUNC_SET_BUFFER_ADDR:
            u32INTERNAL_DVBT2_TDI_ADR   = ALIGN(DVBT2_TDI_ALIGN, *(MS_U32*)data);
            u32INTERNAL_DVBT2_DJB_ADR   = ALIGN(DVBT2_DJB_ALIGN, u32INTERNAL_DVBT2_TDI_ADR + (MS_U32)DVBT2_TDI_LEN);
            u32INTERNAL_DVBT2_EQ_ADR    = ALIGN(DVBT2_EQ_ALIGN, u32INTERNAL_DVBT2_DJB_ADR + (MS_U32)DVBT2_DJB_LEN);
            u32INTERNAL_DVBT2_FW_ADR    = ALIGN(DVBT2_FW_ALIGN, u32INTERNAL_DVBT2_EQ_ADR +(MS_U32)DVBT2_EQ_LEN);

#if defined(CONFIG_K5AP)
            if((u32INTERNAL_DVBT2_FW_ADR & 0x10000) != 0x0)
            {
                MS_U32 u32TMP_ADR = ALIGN(DVBT2_FW_ALIGN, u32INTERNAL_DVBT2_EQ_ADR +(MS_U32)DVBT2_EQ_LEN);
                u32INTERNAL_DVBT2_FW_ADR = ((u32TMP_ADR & 0xFFFF0000) +0x10000);
            }
#else
            if((u32INTERNAL_DVBT2_FW_ADR & 0x5000) != 0x5000)
            {
                MS_U32 u32TMP_ADR = ALIGN(DVBT2_FW_ALIGN, u32INTERNAL_DVBT2_EQ_ADR +(MS_U32)DVBT2_EQ_LEN);
                MS_U32 u32GAP = ((u32TMP_ADR & 0xFFFF) < 0x5000) ? 0x0 : 0x10000;
                u32INTERNAL_DVBT2_FW_ADR = ((u32TMP_ADR & 0xFFFF0000) | 0x5000) + u32GAP;
            }
#endif
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            break;
    }
    return bret;
}

MS_BOOL MDrv_MSDVBT2_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
    return MDrv_MSDVBT2_Demod_I2C_ByPass(0, bOn);
}

MS_U8 MDrv_Demod_GetPlpIDList(void)
{
    return 0;
}

MS_U8 MDrv_Demod_GetNextPlpID(MS_U8 u8Index)
{
    return 0;
}

MS_BOOL MDrv_Demod_SetPlpID(MS_U8 u8PlpID)
{
    return TRUE;
}

MS_U8 MDrv_Demod_GetCurrentDemodType(void)
{
    return MDrv_MSDVBT2_Demod_GetCurrentDemodType(0);
}

#if 0
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBT2) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSINTERN_DVBT2",
    .data                         = DEMOD_MSINTERN_DVBT2,
    .init                         = MDrv_MSDVBT2_Demod_Init,
    .GetLock                      = MDrv_MSDVBT2_Demod_GetLock,
    .GetSNR                       = MDrv_MSDVBT2_Demod_GetSNR,
    .GetBER                       = MDrv_MSDVBT2_Demod_GetBER,
    .GetPWR                       = MDrv_MSDVBT2_Demod_GetPWR,
    .GetSSI                       = MDrv_MSDVBT2_Demod_GetSSI,
    .GetQuality                   = MDrv_MSDVBT2_Demod_GetSignalQuality,
    .GetParam                     = MDrv_Demod_null_GetParam,
    .Restart                      = MDrv_MSDVBT2_Demod_Restart,
    .I2CByPass                    = MDrv_MSDVBT2_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .Extension_Function           = DEMOD_MSDVBT2_Extension_Function,
    .Extension_FunctionPreSetting = NULL,
    .Get_Packet_Error              = MDrv_Demod_null_Get_Packet_Error,
#if MS_DVBT2_INUSE
    .SetCurrentDemodType          = MDrv_MSDVBT2_Demod_SetCurrentDemodType,
    .GetCurrentDemodType          = MDrv_MSDVBT2_Demod_GetCurrentDemodType,
    .GetPlpBitMap                 = MDrv_MSDVBT2_Demod_GetPlpBitMap,
    .GetPlpGroupID                = MDrv_MSDVBT2_Demod_GetPlpGroupID,
    .SetPlpGroupID                = MDrv_MSDVBT2_Demod_SetPlpGroupID,
    .SetScanTypeStatus            = MDrv_MSDVBT2_Demod_SetScanTypeStatus,
    .GetScanTypeStatus            = MDrv_MSDVBT2_Demod_GetScanTypeStatus,
    .GetNextPLPID                 = MDrv_MSDVBT2_Demod_GetNextPlpID,
    .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
    .GetPLPType                   = MDrv_Demod_null_GetPLPType,
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
#endif

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSKAPPA_DVBT)

