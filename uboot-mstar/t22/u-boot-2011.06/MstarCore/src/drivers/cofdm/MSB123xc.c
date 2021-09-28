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
#include <cofdm/MSB123xc.h>

#include "MsCommon.h"
#include <MsUboot.h>
#include <MsOS.h>
#include "drvGPIO.h"
U8 MSB123xC_LIB[]=
{
#include "msb123xc_dvbt.dat"
};
#include "drvMMIO.h"
#include "drvHWI2C.h"
#include "drvMSPI.h"
#include "drvSYS.h"

#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"

/*@ </Include> @*/
/*@ <Definitions> @*/
#define ERR_DEMOD_MSB(x)    x
#define DBG_DEMOD_MSB(x)     //x
#define DBG_DEMOD_FLOW(x)     //x
#define DBG_GET_SIGNAL(x)      //x
#define DBG_DEMOD_LOAD_I2C(x)      //x
#define DBG_DEMOD_CHECKSUM(x)         //x
#define DBG_FLASH_WP(x)         //x
#define DBG_DUMP_LOAD_DSP_TIME 0
#define MDrv_Timer_Delayms OS_DELAY_TASK
#define MDrv_Timer_GetTime0 OS_SYSTEM_TIME

#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C_ENABLE==1)
#define PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C  1
#else
#define PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C  0
#endif
#define LOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 1
#define USE_SPI_LOAD_TO_SDRAM       0// 1
#define SDRAM_DATA_CHECK                 0//1
#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define TIMING_VERIFICATION              0
#define SDRAM_BASE                       0x5000
#define SPI_DEVICE_BUFFER_SIZE		256

//skip dvbc code loading procedure to save boot time
#define SKIP_DVBC_LOAD 1
// msb123xc flash size: 1Mbits
#define MAX_MSB123xC_LIB_LEN              131072
#define FLASH_WP_ENABLE                  1

#define MSB123xC_BOOT  0x01
#define MSB123xC_DVBT2 0x02
#define MSB123xC_DVBT  0x04
#define MSB123xC_DVBC  0x08
#define MSB123xC_ALL   0x0F

#define EDINBURGH_BOOT_START_ADDR     0x00000
#define EDINBURGH_BOOT_END_ADDR       0x00FFF
#define EDINBURGH_DVBT2_P1_START_ADDR 0x01000
#define EDINBURGH_DVBT2_P1_END_ADDR   0x08FFF
#define EDINBURGH_DVBT2_P2_START_ADDR 0x09000
#define EDINBURGH_DVBT2_P2_END_ADDR   0x0FFFF
#define EDINBURGH_DVBT_START_ADDR     0x10000
#define EDINBURGH_DVBT_END_ADDR       0x17FFF
#define EDINBURGH_DVBC_START_ADDR     0x18000
#define EDINBURGH_DVBC_END_ADDR       0x1FFFF

#define EDINBURGH_WINDOWS_BASE                0x100
#define EDINBURGH_BOOT_WINDOWS_OFFSET         EDINBURGH_WINDOWS_BASE
#define EDINBURGH_DVBT2_P2_WINDOWS_OFFSET    (EDINBURGH_WINDOWS_BASE + 0x08)
#define EDINBURGH_DVBT2_P1_WINDOWS_OFFSET    (EDINBURGH_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBT_WINDOWS_OFFSET        (EDINBURGH_DVBT2_P1_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBC_WINDOWS_OFFSET        (EDINBURGH_DVBT_WINDOWS_OFFSET + 0x08)

#define    MSB123xC_MAX_FLASH_ON_RETRY_NUM 3

#define usleep(x)                   MsOS_DelayTaskUs(x)

typedef enum
{
    DEV_MSB123xC_FFT_2K      = 0x0,
    DEV_MSB123xC_FFT_8K      = 0x1,
    DEV_MSB123xC_FFT_4K      = 0x2,
    DEV_MSB123xC_FFT_1K      = 0x3,
    DEV_MSB123xC_FFT_16K      = 0x4,
    DEV_MSB123xC_FFT_32K      = 0x5,
    DEV_MSB123xC_FFT_8K_     = 0x6,
    DEV_MSB123xC_FFT_32K_    = 0x7,
} DEV_MSB123xC_FFT_VAL;

#if 0
MAPI_BOOL device_demodulator::MSB123xC_demod_info(t2_mod_info* t2_mod_info)
{
    MAPI_U8 reg = 0;
    MAPI_U8 s1_siso_miso = 0;
    MAPI_U8 plp_number = 0;
    MAPI_U8 plp_id = 0;
    MAPI_U8 constellation_l1 = 0;
    MAPI_U8 coderate_l1 = 0; //5
    MAPI_U8 plp_constellation = 0;
    MAPI_U8 plp_rotation_constellation = 0;
    MAPI_U8 plp_coderate = 0;
    MAPI_U8 plp_fec = 0;
    MAPI_U8 plp_frame_il_length = 0; //10
    MAPI_U8 plp_type_time_il = 0;
    MAPI_U8 plp_length_time_il = 0;
    MAPI_U8 pilot_pattern = 0;
    MAPI_U8 mode_carrier = 0;
    MAPI_U8 fef_type = 0; //15
    MAPI_U32 fef_length = 0;
    MAPI_U32 fef_interval = 0;
    MAPI_U8 papr = 0;
    MAPI_U8 guard_interval = 0;

    ReadReg(0x2400+0x19*2,&reg); //TDP'S1
    s1_siso_miso = (reg>>4)&0x07;
//BIT 6:4
    t2_mod_info->s1_siso_miso = s1_siso_miso; //  (1)

    ReadReg(0x2700+0x42*2,&reg); //PARSER
    plp_number = reg; //BIT 7:0
    t2_mod_info->plp_number = plp_number; // (2)

    //ReadDspReg(MAPI_U16 u16Addr,MAPI_U8 * pData); //DSP

    ReadDspReg((MAPI_U16)E_T2_PLP_ID, &reg);
    ReadReg(0x2700+0x45*2,&reg); //PARSER
    plp_id = reg; //BIT 7:0
    t2_mod_info->plp_id = plp_id; // (3)

    ReadReg(0x2700+0x31*2 + 1,&reg); //PARSER
    constellation_l1 = reg&0x0f; //BIT 8:11
    t2_mod_info->constellation_l1 = constellation_l1; // (4)
    coderate_l1 = (reg>>4)&0x03; //BIT 13:12
    t2_mod_info->coderate_l1 = coderate_l1; //(5)

    ReadReg(0x2700+0x47*2,&reg); //PARSER
    plp_coderate = reg&0x07; //BIT 2:0
    t2_mod_info->plp_coderate = plp_coderate; //(8)
    plp_constellation = (reg>>3)&0x07; //BIT 5:3
    t2_mod_info->plp_constellation = plp_constellation; //(6)
    plp_rotation_constellation = (reg>>6)&0x01; //BIT 6:6
    t2_mod_info->plp_rotation_constellation = plp_rotation_constellation; //(7)

    plp_fec = (reg>>7)&0x01; // BIT 8:7
    ReadReg(0x2700+0x47*2+1,&reg); //PARSER
    plp_fec = plp_fec|((reg&0x01)<<1);
    t2_mod_info->plp_fec = plp_fec; //(9)

    ReadReg(0x2700+0x49*2,&reg); //PARSER
    plp_frame_il_length = reg; // BIT 7:0
    t2_mod_info->plp_frame_il_length = plp_frame_il_length; //(10)

    ReadReg(0x2700+0x49*2 + 1,&reg); //BIT 8:15
    plp_length_time_il = reg;
    t2_mod_info->plp_length_time_il = plp_length_time_il; //(11-2)
    ReadReg(0x2700+0x48*2 + 1,&reg);
    plp_type_time_il = (reg>>5)&0x01; //BIT 12:12
    t2_mod_info->plp_type_time_il = plp_type_time_il; //(11-1)

    ReadReg(0x2700+0x36*2,&reg); //PARSER
    pilot_pattern = reg&0x0f; //BIT 3:0
    t2_mod_info->pilot_pattern = pilot_pattern; //(12)

    ReadReg(0x2400+0x19*2,&reg); //TDP' S2
    mode_carrier = reg&0x0f;
//BIT 3:0
    t2_mod_info->mode_carrier = mode_carrier; //  (13)

    ReadReg(0x2700+0x42*2 + 1,&reg); //PARSER
    fef_type = (reg>>4)&0x0f; //BIT 15:12
    t2_mod_info->fef_type = fef_type; // (14-1)

    ReadReg(0x2700+0x44*2 ,&reg); //PARSER
    fef_length = (reg&0x3F);
    ReadReg(0x2700+0x43*2 + 1,&reg);
    fef_length = (fef_length<<8)|reg;
    ReadReg(0x2700+0x43*2,&reg);
    fef_length = (fef_length<<8)|reg;
    t2_mod_info->fef_length = fef_length; // (14-2)

    ReadReg(0x2700+0x44*2 + 1 ,&reg); //PARSER
    fef_interval = (reg>>0)&0xff; //BIT 15:8
    t2_mod_info->fef_interval = fef_interval; // (14-3)

    ReadReg(0x2700+0x31*2,&reg); //PARSER
    guard_interval = (reg>>1)&0x07; //BIT 3:1
    t2_mod_info->guard_interval = guard_interval; // (16)
    papr = (reg>>4)&0x0f;  //BIT 7:4
    t2_mod_info->papr = papr; // (15)
    /*
        printf("-[msb123xc][mod][1]s1_siso_miso=%d, plp_number=%d, plp_id=%d\n",s1_siso_miso, plp_number, plp_id);
        printf("-[msb123xc][mod][2]constellation_l1=0x%x, coderate_l1=0x%x\n",constellation_l1, coderate_l1);
        printf("-[msb123xc][mod][3]plp_constellation=0x%x, plp_rotation_constellation=%d, plp_coderate=%d\n", plp_constellation, plp_rotation_constellation, plp_coderate);
        printf("-[msb123xc][mod][4]plp_fec=0x%x, plp_frame_il_length=0x%x, plp_type_time_il=0x%x, plp_length_time_il==0x%x\n", plp_fec, plp_frame_il_length, plp_type_time_il, plp_length_time_il);
        printf("-[msb123xc][mod][5]pilot_pattern=%d, mode_carrier=0x%x\n", pilot_pattern, mode_carrier);
        printf("-[msb123xc][mod][6]fef_type=%d, fef_length=%lx, fef_interval=%lx\n", fef_type, fef_length, fef_interval);
        printf("-[msb123xc][mod][7]papr=0x%x, guard_interval=0x%x\n", papr, guard_interval);
    */
    return true;
}
#endif
/*@ </Definitions> @*/
MAPI_U8 u8DeviceBusy = 0;
//MAPI_U8 device_demodulator::u8PowerOnInit = 1;

MAPI_BOOL m_bSerialOut = MAPI_FALSE;
MAPI_U8 gVifTop = VIF_TOP;
MAPI_U8 gVifSoundSystem = (MAPI_U8)VIF_SOUND_DK2;
MAPI_U8 gVifIfFreq = (MAPI_U8)IF_FREQ_3890;
MAPI_U8 u8MsbData[6];
MAPI_U8 gu8ChipRevId = 0;
MAPI_U8 gCalIdacCh0 = 0;
MAPI_U8 gCalIdacCh1 = 0;
#if(EXTER_DEMOD_MSB123XC_ENABLE ==1)
t2_mod_info st2_mod_info;    //@@++-- 20120405 Arki
#endif

MAPI_BOOL FECLock = MAPI_FALSE;
MAPI_BOOL gbTVAutoScanEn = MAPI_FALSE; //init value=MAPI_FALSE, follow with auto/manual scan
EN_DEVICE_DEMOD_TYPE m_enCurrentDemodulator_Type = E_DEVICE_DEMOD_NULL;


static MAPI_U32  u32ChkScanTimeStart = 0;
static MAPI_BOOL bPower_init_en = MAPI_FALSE;
static MAPI_U32  g_u32Frequency = 0;
static MAPI_BOOL p1_ever_lock_flag = 0;
static MAPI_U8   g_u8_bw = 0;

#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
static MAPI_BOOL bPreloadFail = MAPI_FALSE;
#endif

#if(TIMING_VERIFICATION == 1)
static MAPI_U32 tmm_1 = 0x00;
static MAPI_U32 tmm_2 = 0x00;
static MAPI_U32 tmm_3 = 0x00;
static MAPI_U32 tmm_4 = 0x00;
static MAPI_U32 tmm_5 = 0x00;
static MAPI_U32 tmm_6 = 0x00;
static MAPI_U32 tmm_7 = 0x00;
static MAPI_U32 tmm_8 = 0x00;
static MAPI_U32 tmm_9 = 0x00;
static MAPI_U32 tmm_10 = 0x00;
static MAPI_U32 tmm_11 = 0x00;
static MAPI_U32 tmm_12 = 0x00;
static MAPI_U32 tmm_13 = 0x00;
static MAPI_U32 tmm_14 = 0x00;
static MAPI_U32 tmm_15 = 0x00;
static MAPI_U32 tmm_16 = 0x00;
static MAPI_U32 tmm_17 = 0x00;
static MAPI_U32 tmm_18 = 0x00;
static MAPI_U32 tmm_19 = 0x00;
static MAPI_U32 tmm_20 = 0x00;

#define GIVE_ME_TIME MsOS_GetSystemTime();
#endif

static MAPI_BOOL bDoReset = FALSE;
MAPI_U32 u32StartTime = 0;
MAPI_U8 gQamVal;

#define resetDemodTime  50
#define waitFlashTime   50

static MS_S32 _s32_Demod_DVBT2_Mutex=-1;
static MS_S32 _s32_Demod_DVBT2_RW_Mutex=-1;
static MS_S32 _s32_Demod_DVBT2_RW2_Mutex=-1;

#if (LOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
static MS_U8 g_WO_SPI_FLASH = 1;
#else
static MS_U8 g_WO_SPI_FLASH = 0;
#endif

// ONLY wo flash procedure used.
static MS_U8 g_sdram_code = 0x0;
static MS_U8 g_sram_code  = 0x0;

static MS_U8 g_msb123xc_plp_id   = 0xff;
static MS_U8 g_msb123xc_group_id = 0xff;
static MS_U8 u8_g_pid0_timeout_flag = 0;

static MAPI_U8       PLPIDSize = 0;
static MAPI_U8       PLPIDList[256];

//static float g_msb123xc_fSNR = 0.0; // static float g_msb123x_fSNR = 0.0;
#if 0
static S_DVBT2_SQI_CN_NORDIGP1 g_msb123x_dvbt2_sqi_cn_nordigp1[] =
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
    {_T2_QAM_UNKNOWN, _T2_CR_UNKNOWN, 0.0}
};
#endif

MAPI_BOOL msb123xc_flash_mode_en(void);
MAPI_BOOL msb123xc_flash_boot_ready_waiting(MAPI_U8 *ptimeout);

MAPI_BOOL MSB123xC_MEM_switch(MAPI_U8 mem_type);
EN_LOCK_STATUS DTV_DVB_T_GetLockStatus(void);
MAPI_BOOL I2C_CH_Reset(MAPI_U8 ch_num);
MAPI_BOOL WriteReg(MAPI_U16 u16Addr, MAPI_U8 u8Data);
MAPI_BOOL WriteRegs(MAPI_U16 u16Addr, MAPI_U8* u8pData, MAPI_U16 data_size);
MAPI_BOOL WriteReg2bytes(MAPI_U16 u16Addr, MAPI_U16 u16Data);
MAPI_BOOL ReadReg(MAPI_U16 u16Addr, MAPI_U8 *pu8Data);
MAPI_BOOL  WriteDspReg(MAPI_U16 u16Addr, MAPI_U8 u8Data);
MAPI_BOOL ReadDspReg(MAPI_U16 u16Addr, MAPI_U8* pData);
MAPI_BOOL Cmd_Packet_Send(S_CMDPKTREG *pCmdPacket, MAPI_U8 param_cnt);
void Driving_Control(MAPI_BOOL bEnable);
MAPI_BOOL LoadDSPCode(void);
//MAPI_U16 MSB123xC_Lock(E_SYSTEM system, COFDM_LOCK_STATUS eStatus );
    
#if(TIMING_VERIFICATION == 1)
static void show_timer(void)
{
    printf("***************************\n");
    printf("[tmm1]t2-t1 = %ld (%ld - %ld)\n",tmm_2-tmm_1,tmm_2,tmm_1);
    printf("[tmm2]t4-t3 = %ld (%ld - %ld)\n",tmm_4-tmm_3,tmm_4,tmm_3);
    printf("[tmm3]t6-t5 = %ld (%ld - %ld)\n",tmm_6-tmm_5,tmm_6,tmm_5);
    printf("[tmm4]t8-t7 = %ld (%ld - %ld)\n",tmm_8-tmm_7,tmm_8,tmm_7);
    printf("[tmm5]t10-t9 = %ld (%ld - %ld)\n",tmm_10-tmm_9,tmm_10,tmm_9);
    printf("[tmm6]t12-t11 = %ld (%ld - %ld)\n",tmm_12-tmm_11,tmm_12,tmm_11);
    printf("[tmm7]t14-t13 = %ld (%ld - %ld)\n",tmm_14-tmm_13,tmm_14,tmm_13);
    printf("[tmm8]t16-t15 = %ld (%ld - %ld)\n",tmm_16-tmm_15,tmm_16,tmm_15);
    printf("[tmm9]t18-t17 = %ld (%ld - %ld)\n",tmm_18-tmm_17,tmm_18,tmm_17);
    printf("[tmm10]t20-t19 = %ld (%ld - %ld)\n",tmm_20-tmm_19,tmm_20,tmm_19);
    printf("[tmm11]lock_time# = %ld (%ld - %ld)\n",tmm_14-u32StartTime,tmm_14,u32StartTime);
    printf("[tmm12]lock-setf = %ld (%ld - %ld)\n",tmm_14-tmm_11,tmm_14,tmm_11);
    printf("[tmm13]lock-loaddsp = %ld (%ld - %ld)\n",tmm_14-tmm_9,tmm_14,tmm_9);
    printf("***************************\n");
}
#endif

//##########################################################################################################
//##########################################################################################################
//###############################  Public:Common Function Implementation ###################################
//##########################################################################################################
//##########################################################################################################

static MAPI_BOOL IspCheckVer(MAPI_U8* pLibData, MAPI_BOOL* pMatch)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8  bReadData[VERSION_CODE_SIZE];
    MAPI_U32  indx = 0;
    *pMatch = true;

    MAPI_U8 bWriteData[5] = {0x4D, 0x53, 0x54, 0x41, 0x52};
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_2);
//    iptr->WriteBytes(0, NULL, 5, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 5, bWriteData);

    MAPI_U8    bAddr[1], bError = true;
    //MAPI_U16   Count;

    memset(bReadData, 0 , sizeof(bReadData));

    bAddr[0] = 0x10;
    //dwStartAddr=0;

    bWriteData[0] = 0x03;
    bWriteData[1] = VERSION_CODE_ADDR >> 16;
    bWriteData[2] = VERSION_CODE_ADDR >> 8;
    bWriteData[3] = VERSION_CODE_ADDR & 0xFF;

//    bError &=  iptr->WriteBytes(1, bAddr, 4, bWriteData);
    bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 4, bWriteData);

    bAddr[0] = 0x11;
//    bError &= iptr->ReadBytes(1, bAddr, VERSION_CODE_SIZE, bReadData);
    bError &= MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, VERSION_CODE_SIZE, bReadData);

    bWriteData[0] = 0x12;
//    bError &= iptr->WriteBytes(0, NULL, 1,  bWriteData);
    bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1,  bWriteData);

    if(MAPI_FALSE == bError)
    {
        bWriteData[0] = 0x24 ;
//        iptr->WriteBytes(0, NULL, 1, bWriteData);
        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1,  bWriteData);

        return MAPI_FALSE;
    }

    bWriteData[0] = 0x24 ;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1,  bWriteData);

    printf("sttest version data = ");
    for(indx = 0; indx < (VERSION_CODE_SIZE); indx++)
    {
        printf(" %x ,", bReadData[indx]);
        if(pLibData[indx+VERSION_CODE_ADDR] != bReadData[indx])
        {
            *pMatch = false;
            //break;
        }
    }
    printf(" \n");

    return MAPI_TRUE;
}

static MAPI_BOOL dram_crc_check(MAPI_U16 chksum_lib, MAPI_BOOL* pMatch)
{
    MAPI_U16  chksum = 0;
    // MAPI_U16  chksum_lib = 0;
    MAPI_U16  u16Addr = 0;
    MAPI_U8   u8MsbData[5];
    MAPI_U8   reg = 0;
    MAPI_BOOL bRet;
    MAPI_U8   mcu_status = 0;

    *pMatch = false;

    // MAPI_U8 bWriteData[5]={0x4D, 0x53, 0x54, 0x41, 0x52};
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);
    // iptr->WriteBytes(0, NULL, 5, bWriteData);

/// crc H byte
    u16Addr = 0x0c00+0x0d*2+1;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 5, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//    iptr->ReadBytes(0, NULL, 1, &reg);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
//    bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    chksum = reg;

/// crc L byte
    u16Addr = 0x0c00+0x0d*2;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 5, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//    iptr->ReadBytes(0, NULL, 1, &reg);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
//    bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);


    chksum = (chksum<<8)|reg;

// get mcu status

    u16Addr = 0x0900+0x4f*2;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 5, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//    iptr->ReadBytes(0, NULL, 1, &reg);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
//    bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    mcu_status = reg;


/// check the crc in dsp lib array

    if (mcu_status == 0xaa && ((chksum_lib&0xff00) == (chksum&0xff00)) )
        *pMatch = true;
    else if(chksum_lib == chksum)
        *pMatch = true;

    DBG_DEMOD_CHECKSUM(printf("[crc]chksum_lib=0x%x, chksum=0x%x, bRet=%d, Match=%d, mcu_status=0x%x\n",chksum_lib,chksum,bRet,*pMatch,mcu_status));

    return bRet;
}

#if (FLASH_WP_ENABLE == 1)
static MAPI_BOOL msb123xc_flash_WP_reg_read(MAPI_U16 u16Addr, MAPI_U8 *pu8Data)
{
    MAPI_BOOL bRet;
    MAPI_U8   u8MsbData[5];
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 5, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//    iptr->ReadBytes(0, NULL, 1, pu8Data);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
//    bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    return bRet;
}

static MAPI_BOOL msb123xc_flash_WP_reg_write(MAPI_U16 u16Addr, MAPI_U8 u8Data)
{
    MAPI_BOOL bRet;
    MAPI_U8   u8MsbData[6];
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    //bRet &= iptr->SetSpeed(0);


    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
    
    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 6, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 6, u8MsbData);
    
    u8MsbData[0] = 0x34;
//    bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    return bRet;
}

static MAPI_BOOL msb123xc_flash_WRSR(MAPI_U8 reg)
{
    MAPI_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};
    MAPI_U8     bAddr[1];
    MAPI_BOOL   rbet = true;

//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_2);
//    iptr->WriteBytes(0, NULL, 5, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 5, bWriteData);
    
    // WREN
    bAddr[0] = 0x10;
    bWriteData[0] = 0x06;
//    iptr->WriteBytes(1, bAddr, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

    bWriteData[0] = 0x12;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    // WRSR
    bAddr[0] = 0x10;
    bWriteData[0] = 0x01;
    bWriteData[1] = reg;
//    iptr->WriteBytes(1, bAddr, 2, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 2, bWriteData);

    bWriteData[0] = 0x12;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    // WRDI
    bAddr[0] = 0x10;
    bWriteData[0] = 0x04;
//    iptr->WriteBytes(1, bAddr, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

    bWriteData[0] = 0x12;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    // end
    bWriteData[0] = 0x24;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    DBG_FLASH_WP(printf("[wb]msb123xc_flash_WRSR, reg=0x%x\n",reg);)

    return rbet;
}

static MAPI_BOOL msb123xc_flash_SRSR(MAPI_U8 *p_reg)
{

    MAPI_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};
    MAPI_U8     bAddr[1];
    MAPI_BOOL   bRet = true;

//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_2);
//    iptr->WriteBytes(0, NULL, 5, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 5, bWriteData);


    bAddr[0] = 0x10;
    bWriteData[0] = 0x05;
//    iptr->WriteBytes(1, bAddr, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

    bAddr[0] = 0x11;
//    iptr->ReadBytes(1, bAddr, 1, p_reg);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, p_reg);

    bWriteData[0] = 0x12;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    // end
    bWriteData[0] = 0x24 ;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    DBG_FLASH_WP(printf("[wb]msb123xc_flash_SRSR, reg=0x%x\n",*p_reg);)

    return bRet;
}

static MAPI_BOOL msb123xc_flash_WP(MAPI_U8 enable)
#if 0
{
    MAPI_U8 reg = 0;
    MAPI_BOOL bRet = true;
    MAPI_U8 u8_count = 0;

    DBG_FLASH_WP(printf("[wb]msb123xc_flash_WP_Enable=%d\n",enable);)

    if (enable == 1)
    {
        u8_count = 20;
        do
        {
            msb123xc_flash_SRSR(&reg);
            usleep(1*1000);
        }
        while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = false;
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            return bRet;
        }

        msb123xc_flash_WRSR(reg|0x9c);


        u8_count = 20;
        do
        {
            msb123xc_flash_SRSR(&reg);
            usleep(1*1000);
        }
        while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = false;
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            return bRet;
        }

        // active low
        // pull low
        bRet &= msb123xc_flash_WP_reg_read(0x0900+0x63*2+1, &reg);
        bRet &= msb123xc_flash_WP_reg_write(0x0900+0x63*2+1, reg&(~0x08));

        // gpio11 output enable
        bRet &= msb123xc_flash_WP_reg_read(0x0900+0x64*2+1, &reg);
        bRet &= msb123xc_flash_WP_reg_write(0x0900+0x64*2+1, reg&(~0x08));
    }
    else
    {
        // unactive high
        // pull high
        bRet &= msb123xc_flash_WP_reg_read(0x0900+0x63*2+1, &reg);
        bRet &= msb123xc_flash_WP_reg_write(0x0900+0x63*2+1, reg|0x08);

        // gpio11 output enable
        bRet &= msb123xc_flash_WP_reg_read(0x0900+0x64*2+1, &reg);
        bRet &= msb123xc_flash_WP_reg_write(0x0900+0x64*2+1, reg&(~0x08));

        u8_count = 20;
        do
        {
            msb123xc_flash_SRSR(&reg);
            usleep(1*1000);
        }
        while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = false;
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            return bRet;
        }

        msb123xc_flash_WRSR(reg&(~0x9c));

        u8_count = 20;
        do
        {
            msb123xc_flash_SRSR(&reg);
            usleep(1*1000);
        }
        while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = false;
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            return bRet;
        }
    }

    return bRet;
}
#else
{

    MAPI_U8 reg = 0;

    MAPI_BOOL bRet = true;

    MAPI_U8 u8_count = 0;



    DBG_FLASH_WP(printf("[wb]msb123xc_flash_WP_Enable=%d\n",enable);)



    if (enable == 1)

    {

        u8_count = 20;

        do

        {

            msb123xc_flash_SRSR(&reg);

            usleep(1*1000);

        }
        while(reg&0x01 && u8_count--);



        if (u8_count == 0)

        {

            bRet = false;

            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)

            return bRet;

        }



        msb123xc_flash_WRSR(reg|0x9c);





        u8_count = 20;

        do

        {

            msb123xc_flash_SRSR(&reg);

            usleep(1*1000);

        }
        while(reg&0x01 && u8_count--);



        if (u8_count == 0)

        {

            bRet = false;

            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)

            return bRet;

        }



        // active low

        // pull low

        bRet &= msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);

        bRet &= msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x01));



        // gpio11 output enable

        bRet &= msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);

        bRet &= msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x02));

    }

    else

    {

        // unactive high

        // pull high

        bRet &= msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);

        bRet &= msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg|0x01);



        // gpio11 output enable

        bRet &= msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);

        bRet &= msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x02));



        u8_count = 20;

        do

        {

            msb123xc_flash_SRSR(&reg);

            usleep(1*1000);

        }
        while(reg&0x01 && u8_count--);



        if (u8_count == 0)

        {

            bRet = false;

            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)

            return bRet;

        }



        msb123xc_flash_WRSR(reg&(~0x9c));



        u8_count = 20;

        do

        {

            msb123xc_flash_SRSR(&reg);

            usleep(1*1000);

        }
        while(reg&0x01 && u8_count--);



        if (u8_count == 0)

        {

            bRet = false;

            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)

            return bRet;

        }

    }



    return bRet;

}


#endif
#endif

EN_DEVICE_DEMOD_TYPE DTV_Get_Demod_TYPE(void)
{
    return m_enCurrentDemodulator_Type;
}

// 144 ms roughly
MAPI_BOOL MSB123xC_HW_init(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_BOOL bRet = true;
    MAPI_U8 u8_tmp = 0;
//    MAPI_U8 u8_timeout = 0;

    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]MSB123xC_HW_init\n"));

    // ASIC INIT for EDB DVB-T2
    {
        // Reset MCU
        // wreg 11 0x19 0x0003  // don't do this in MCU firmware!
        // -------------------------------------------------------------------
        // Initialize DMD_ANA_MISC
        // -------------------------------------------------------------------
        // [1:0]    reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
        // [15:8]   reg_mpll_loop_div_second      feedback divider, div by binary data number
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h33, 2'b11, 16'h2400);  // Loop divider ; VCO = 24*(2^2)*9 = 864
        bRet &= WriteReg2bytes(0x0A00+(0x33)*2, 0x2400);

        // [2:0]    reg_mpll_ictrl          charge pump current control
        // [3]  reg_mpll_in_sel         1.8V or 3.3V reference clock domain select (1'b0=0==>3.3 V reference clock domain)
        // [4]  reg_mpll_xtal2adc_sel       select the XTAL clock bypass to MPLL_ADC_CLK
        // [5]  reg_mpll_xtal2next_pll_sel  crystal clock bypass to next PLL select
        // [6]  reg_mpll_vco_offset     set VCO initial offset frequency
        // [7]  reg_mpll_pd         gated reference clock and power down PLL analog_3v: 1=power down
        // [8]  reg_xtal_en         XTAL enable register; 1: enable
        // [10:9]   reg_xtal_sel            XTAL driven strength select.
        // [11]     reg_mpll_porst          MPLL input  power on reset, connect to reg as MPLL_RESET
        // [12]     reg_mpll_reset          PLL software reset; 1:reset
        // [13]     reg_pd_dmpll_clk        XTAL to MPLL clock reference power down
        // [14]     reg_pd_3p3_1            XTAL to CLK_24M_3P3_1 power down
        // [15]     reg_pd_3p3_2            XTAL to CLK_24M_3P3_2 power down
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h35, 2'b11, 16'h1804); // MPLL reset
        bRet &= WriteReg2bytes(0x0A00+(0x35)*2, 0x1804);
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h35, 2'b11, 16'h0004); // release MPLl reset
        bRet &= WriteReg2bytes(0x0A00+(0x35)*2, 0x0004);

        // [15:0]   reg_mpll_test
        // [14] mpll reset
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h34, 2'b11, 16'h4000);
        bRet &= WriteReg2bytes(0x0A00+(0x34)*2, 0x4000);
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h34, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0A00+(0x34)*2, 0x0000);

        // [0]  reg_mpll_clk_dp_pd  dummy
        // [1]  reg_adc_clk_pd      ADC output clock power down
        // [2]  reg_mpll_div2_pd    MPLL_DIV2 power down
        // [3]  reg_mpll_div3_pd    MPLL_DIV3 power down
        // [4]  reg_mpll_div4_pd    MPLL_DIV4 power down
        // [5]  reg_mpll_div8_pd    MPLL_DIV8 power down
        // [6]  reg_mpll_div10_pd   MPLL_DIV10 power down
        // [13:8]  reg_mpll_adc_div_sel select the ADC clock divide ratio,ADC clk=XTAL_IN * (LOOP_DIV_FIRST*LOOP_DIV_SECOND)/div_ratio

        // `ifdef SIM_ADC_48MHZ
        //
        //       // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1200);  // divide ADC clock to 144Mhz = 24*36/48
        //       wreg 10 0x30 0x1200
        //
        // `elsif SIM_ADC_45P47MHZ
        //
        //       // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1300);  // divide ADC clock to 45.47Mhz = 24*36/19
        //       wreg 10 0x30 0x1300
        //
        // `elsif SIM_ADC_28P8MHZ

        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1e00);  // divide ADC clock to 28.8Mhz = 24*36/30
        bRet &= WriteReg2bytes(0x0A00+(0x30)*2, 0x1e00);

        // `elsif SIM_ADC_20P48MHZ
        //
        //       // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h2a00);  // divide ADC clock to 20.48Mhz = 24*36/42
        //       wreg 10 0x30 0x2a00
        //
        // `else
        //
        //     // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1600);  // divide ADC clock to 39.27Mhz = 24*36/22
        //     wreg 10 0x30 0x1600
        //
        // `endif

        // `ifdef SIM_SAR_ADC
        //
        //       // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h3C, 2'b11, 16'h0010);  // enable SAR
        //       wreg 10 0x3c 0x0010
        //
        // `endif

        // `ifndef SKIP_ADC_INIT
        //     $display("--------------------------------------");
        //     $display("Initialize ADC I/Q");
        //     $display("--------------------------------------");

        // [0]  Q channel ADC power down
        // [1]  I channel ADC power down
        // [2]  Q channel clamp enable. 0:enable, 1:disable
        // [3]  I channel clamp enable. 0:enable, 1:disable
        // [6:4]    I channel input mux control;
        //      3'b000=I channel ADC calibration mode input
        //          3'b001=VIF signal from VIFPGA
        //          3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
        //          all the other combination are only for test mode, don't use without understanding.
        // [10:8]   Q channel input mux control;
        //      3'b000=Q channel ADC calibration mode input
        //          3'b001=VIF signal from VIFPGA 3'b010 = SSIF signal from PAD_SIFP(M)
        //          3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
        //          all the other combination are only for test mode, don't use without understanding.
        // [12] ADC I,Q swap enable; 1: swap
        // [13] ADC clock out select; 1: ADC_CLKQ
        // [14] ADC linear calibration bypass enable; 1:enable
        // [15] ADC internal 1.2v regulator control always 0 in T3
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h01, 2'b11, 16'h0440); // Set IMUXS QMUXS
        bRet &= WriteReg2bytes(0x0A00+(0x01)*2, 0x0440);

        // [0]  Channel I ADC power down: 1=power dwon
        // [1]  Channel Q ADC power down: 1=power dwon
        // [2]  power down clamp buffer for test mode
        // [3]  change ADC reference voltage for SSIF
        // [6:4]    ADC source bias current control
        // [9:8]    XTAL receiver amp gain
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0C, 2'b11, 16'h0000); // Set enable ADC clock
        bRet &= WriteReg2bytes(0x0A00+(0x0c)*2, 0x0000);

        // [3:0]    clamp voltage control
        //          3'b000 = 0.7v
        //          3'b001 = 0.75v
        //          3'b010 = 0.5v
        //          3'b011 = 0.4v
        //          3'b100 = 0.8v
        //          3'b101 = 0.9v
        //          3'b110 = 0.65v
        //          3'b111 = 0.60v
        // [4]  REFERENCE power down
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h20, 2'b11, 16'h0000); // Disable PWDN_REF
        bRet &= WriteReg2bytes(0x0A00+(0x20)*2, 0x0000);

        // Set ADC gain is 1
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0b, 2'b11, 16'h0005);
        bRet &= WriteReg2bytes(0x0A00+(0x0b)*2, 0x0009);

        // Disable ADC Sign bit
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2e, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0A00+(0x2e)*2, 0x0000);

        // ADC I channel offset
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2a, 2'b11, 16'h0c00);
        bRet &= WriteReg2bytes(0x0A00+(0x2a)*2, 0x0c00);

        // ADC Q channel offset
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2b, 2'b11, 16'h0c00);
        bRet &= WriteReg2bytes(0x0A00+(0x2b)*2, 0x0c00);

        // `ifndef ZIF_ADC

        // [2:0]reg_acl_ref
        // [5:4]reg_acl_isel
        // [8]  reg_xtal_pm_isel
        // [9]  reg_bond_mode
        // [10] reg_clk_bond_mode
        // [11] reg_clk_usb_3p3_en
        // [13:12]  reg_iq_ctrl = 2'd1
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h19, 2'b11, 16'h1e00);
        bRet &= WriteReg2bytes(0x0A00+(0x19)*2, 0x1e00);

        // [ 4:0]reg_ckg_bist[4:0]
        // [11:8]reg_ckg_adcd_d2[3:0]
        // `RIU_W((`RIUBASE_TOP>>1)+7'h1c, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x1c)*2, 0x0000);

        // [ 4:0]reg_ckg_dvbtm_sram_t11x_t22x[4:0]
        // [12:8]reg_ckg_dvbtm_sram_t11x_t24x[4:0]
        // `RIU_W((`RIUBASE_TOP>>1)+7'h1e, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x1e)*2, 0x0000);

        // `else
        //
        //     // [5:0]reg_ckg_mcu
        //     // [6]   reg_power_good_mask
        //     // [11:8]reg_ckg_inner
        //  // `RIU_W((`RIUBASE_TOP>>1)+7'h0b, 2'b11, 16'h4430);
        //  wreg  9 0x0b 0x4430
        //
        //     // [3:0]reg_ckg_isdbt_adc
        //     // [6:4]reg_ckg_isdbt_inner1x
        //     // [10:8]reg_ckg_isdbt_inner2x
        //     // [14:12]reg_ckg_isdbt_inner4x
        //  // `RIU_W((`RIUBASE_TOP>>1)+7'h10, 2'b11, 16'h0004);
        //  wreg  9 0x10 0x0004
        //
        //     // [1:0]reg_chanout_sel
        //     // [2]   reg_iq_filter_enable    = 1
        //     // [3]   reg_iq_filter_sel
        //     // [5:4]reg_adc_debug_clk_sel
        //  // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h17, 2'b11, 16'h0004);
        //  wreg 10 0x17 0x0004
        //
        // `endif

        // `endif //SKIP_ADC_INIT
        //
        // `ifndef SKIP_RELEASE_CLOCK_GATING

        // [15:0]   reg_mpll_test
        // [4]  mpll lock detector enable
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h34, 2'b01, 16'h0010);
        bRet &= WriteReg2bytes(0x0A00+(0x34)*2, 0x0010);

        // -------------------------------------------------------------------
        // Release clock gating
        // -------------------------------------------------------------------
        // $display("--------------------------------------");
        // $display("Release clock gating");
        // $display("--------------------------------------");

        // [0]  reg_xtal_en
        // [9:8]    reg_clk_pd_iic
        // [10] reg_clk_pd_all
        // `RIU_W((`RIUBASE_TOP>>1)+7'h09, 2'b11, 16'h0101);
        bRet &= WriteReg2bytes(0x0900+(0x09)*2, 0x0101);

        // [3:0]    reg_ckg_adcd
        // [7:4]    reg_ckg_sadc
        // [11:8]   reg_ckg_iicm
        // [13:12]  reg_ckg_sbus
        // `RIU_W((`RIUBASE_TOP>>1)+7'h0a, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x0a)*2, 0x0000);

        // [5:0]    reg_ckg_mcu
        // [6]  reg_ckg_live
        // [11:8]   reg_ckg_inner
        // `RIU_W((`RIUBASE_TOP>>1)+7'h0b, 2'b11, 16'h0030);
        bRet &= WriteReg2bytes(0x0900+(0x0b)*2, 0x0030);

        // @0x0910
        // [3:0]    reg_ckg_dvbtm_adc
        // [6:4]    reg_ckg_dvbt_inner1x
        // [10:8]   reg_ckg_dvbt_inner2x
        // [14:12]  reg_ckg_dvbt_inner4x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h10, 2'b11, 16'h4440);
        //     wreg  9 0x10 0x4440
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h10, 2'b11, 16'h1110);
        //     wreg  9 0x10 0x1110
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h10, 2'b11, 16'h1110);
        bRet &= WriteReg2bytes(0x0900+(0x10)*2, 0x1110);
        // `endif

        // @0x0911
        // [2:0]    reg_ckg_dvbt_outer1x
        // [6:4]    reg_ckg_dvbt_outer2x
        // [11:8]   reg_ckg_dvbtc_outer2x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h11, 2'b11, 16'h0844);
        //     wreg  9 0x11 0x0844
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h11, 2'b11, 16'h0844);
        //     wreg  9 0x11 0x0844
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h11, 2'b11, 16'h0111);
        bRet &= WriteReg2bytes(0x0900+(0x11)*2, 0x0111);
        // `endif

        // @0x0912
        // [3:0]    reg_ckg_dvbtm_ts
        // [4]  reg_dvbtm_ts_out_mode
        // [5]  reg_dvbtm_ts_clk_pol
        // [15:8]   reg_dvbtm_ts_clk_divnum
        // `RIU_W((`RIUBASE_TOP>>1)+7'h12, 2'b11, 16'h0618);
        bRet &= WriteReg2bytes(0x0900+(0x12)*2, 0x0618);

        // @0x0913
        // [5:0]    reg_ckg_spi
        // `RIU_W((`RIUBASE_TOP>>1)+7'h13, 2'b11, 16'h0020);
        bRet &= WriteReg2bytes(0x0900+(0x13)*2, 0x0020);

        // @0x0914
        // [12:8]   reg_ckg_dvbtm_sram_t1o2x_t22x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h14, 2'b11, 16'h0400);
        //     wreg  9 0x14 0x0400
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h14, 2'b11, 16'h0400);
        //     wreg  9 0x14 0x0400
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h14, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x14)*2, 0x0000);
        // `endif

        // @0x0915
        // [3:0]    reg_ckg_dvbc_inner
        // [6:4]    reg_ckg_dvbc_eq
        // [10:8]   reg_ckg_dvbc_eq8x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h15, 2'b11, 16'h0111);
        //     wreg  9 0x15 0x0111
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h15, 2'b11, 16'h0000);
        //     wreg  9 0x15 0x0000
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h15, 2'b11, 16'h0111);
        bRet &= WriteReg2bytes(0x0900+(0x15)*2, 0x0111);
        // `endif

        // @0x0916
        // [3:0]    reg_ckg_dvbtm_adc_2x_4x
        // [8:4]    reg_ckg_dvbtm_sram_adc_t22x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h16, 2'b11, 16'h0041);
        //     wreg  9 0x16 0x0041
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h16, 2'b11, 16'h0011);
        //     wreg  9 0x16 0x0011
        // `elsif SIM_ADC_DUMP
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h16, 2'b11, 16'h0101);
        //     wreg  9 0x16 0x0101
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h16, 2'b11, 16'h0001);
        bRet &= WriteReg2bytes(0x0900+(0x16)*2, 0x0001);
        // `endif

        // @0x0917
        // [4:0]    reg_ckg_dvbtm_sram_t12x_t22x
        // [12:8]   reg_ckg_dvbtm_sram_t12x_t24x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h17, 2'b11, 16'h0c00);
        //     wreg  9 0x17 0x0c00
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h17, 2'b11, 16'h0011);
        //     wreg  9 0x17 0x0011
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h17, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x17)*2, 0x0000);
        // `endif

        // @0x0918
        // [4:0]    reg_ckg_dvbtm_sram_t14x_t24x
        // [12:8]   reg_ckg_dvbtm_ts_in
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h18, 2'b11, 16'h0404);
        //     wreg  9 0x18 0x0404
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h18, 2'b11, 16'h0401);
        //     wreg  9 0x18 0x0401
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h18, 2'b11, 16'h1000);
        bRet &= WriteReg2bytes(0x0900+(0x18)*2, 0x1000);
        // `endif

        // @0x0919
        // [2:0]    reg_ckg_tdp_jl_inner1x
        // [6:4]    reg_ckg_tdp_jl_inner4x
        // [15:8]   reg_ckg_miu
        // `RIU_W((`RIUBASE_TOP>>1)+7'h19, 2'b11, 16'h3c00);
        bRet &= WriteReg2bytes(0x0900+(0x19)*2, 0x3c00);

        // @0x091a
        // [6:4]    reg_ckg_dvbt2_inner1x
        // [10:8]   reg_ckg_dvbt2_inner2x
        // [14:12]  reg_ckg_dvbt2_inner4x
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h1a, 2'b11, 16'h1110);
        //     wreg  9 0x1a 0x1110
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h1a, 2'b11, 16'h1110);
        //     wreg  9 0x1a 0x1110
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h1a, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x1a)*2, 0x0000);
        // `endif

        // @0x091b
        // [2:0]    reg_ckg_dvbt2_outer1x
        // [6:4]    reg_ckg_dvbt2_outer2x
        // [10:8]   reg_ckg_syn_miu
        // [14:12]  reg_ckg_syn_ts
        // `ifdef SIM_DVBT
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h1b, 2'b11, 16'h0011);
        //     wreg  9 0x1b 0x0011
        // `elsif SIM_DVBC
        //     // `RIU_W((`RIUBASE_TOP>>1)+7'h1b, 2'b11, 16'h0011);
        //     wreg  9 0x1b 0x0011
        // `else //DVBT2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h1b, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x1b)*2, 0x0000);
        // `endif


        // @0x091c
        // [4:0]    reg_ckg_bist
        // [11:8]   reg_ckg_adcd_d2
        // `RIU_W((`RIUBASE_TOP>>1)+7'h1c, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x1c)*2, 0x0000);


        // [1:0]    reg_iicm_pad_sel
        // [4]  reg_i2c_sbpm_en
        // [12:8]   reg_i2c_sbpm_idle_num
        // `RIU_W((`RIUBASE_TOP>>1)+7'h08, 2'b01, 16'h0a01);
        bRet &= WriteReg2bytes(0x0900+(0x08)*2, 0x0a01);

        // [8]  reg_turn_off_pad
        // `RIU_W((`RIUBASE_TOP>>1)+7'h28, 2'b10, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x28)*2, 0x0000);
        // `endif
        //
        // `ifndef SKIP_TS_SYNTHESIZER_APLL
        //     $display("--------------------------------------");
        //     $display("Initialize Transport Stream synthesizer and APLL");
        //     $display("--------------------------------------");

        // ////////////////////////////////////////////////////
        //
        //  According to analog APLL designer's suggest:
        //  Fix apll_loop_div_first = 2'b11 (2^3 = 8)
        //  Fix apll_loop_div_second = 2'b00 ( = 1)
        //  APLL default output divider is 4
        //
        //  so apll_1x_out = (synthesizer_out * 8 / 4)
        //
        //  VCO = synthesizer_out * 8 , should 320Mhz ~ 640Mhz
        //  => synthesizer_out = 216/reg_synth_set , should 40Mhz ~ 80Mhz
        //  => reg_synth_set should 2.7 ~ 5.4
        //  => apll_1x_out should 80Mhz ~ 160Mhz
        //
        //  Current setting:
        //  apll_1x_out = (216/4.0) * 8 / 4 = 108MHz
        //  choose reg_ckg_ts_apll_div[2:0] = 3'd4
        //  ts_clk_apll_div = 108/(2^4) = 6.75MHz
        //
        // ////////////////////////////////////////////////////


        // [15:0]   reg_synth_set[15: 0]
        // [ 7:0]   reg_synth_set[23:16]
        // `RIU_W((`RIUBASE_TOP>>1)+7'h51, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x0900+(0x51)*2, 0x0000);
        // `RIU_W((`RIUBASE_TOP>>1)+7'h52, 2'b11, 16'h0020);
        bRet &= WriteReg2bytes(0x0900+(0x52)*2, 0x0020);


        // [0]  reg_synth_reset
        // [1]  reg_synth_ssc_en
        // [2]  reg_synth_ssc_mode
        // [4]  reg_synth_sld
        // `RIU_W((`RIUBASE_TOP>>1)+7'h50, 2'b01, 16'h0010);
        bRet &= WriteReg2bytes(0x0900+(0x50)*2, 0x0010);

        // #10_000;
        //delay 0


        // [1:0]    reg_apll_loop_div_first
        // [15:8]   reg_apll_loop_div_second
        // `RIU_W((`RIUBASE_TOP>>1)+7'h57, 2'b11, 16'h0003);
        bRet &= WriteReg2bytes(0x0900+(0x57)*2, 0x0003);


        // [0]  reg_apll_pd
        // [1]  reg_apll_reset
        // [2]  reg_apll_porst
        // [3]  reg_apll_vco_offset
        // [4]  reg_apll_en_ts
        // [5]  reg_apll_endcc
        // [6]  reg_apll_clkin_sel
        // `RIU_W((`RIUBASE_TOP>>1)+7'h55, 2'b11, 16'h0006);
        bRet &= WriteReg2bytes(0x0900+(0x55)*2, 0x0006);
        // `RIU_W((`RIUBASE_TOP>>1)+7'h55, 2'b01, 16'h0010);
        bRet &= WriteReg2bytes(0x0900+(0x55)*2, 0x0010);

        // [16:0]   reg_apll_test
        // [4]  lock detector enable
        // `RIU_W((`RIUBASE_TOP>>1)+7'h59, 2'b01, 16'h0010);
        bRet &= WriteReg2bytes(0x0900+(0x59)*2, 0x0010);

        // 0x0920
        // [3:0]    reg_ckg_ts_apll_div[2:0]
        // `RIU_W((`RIUBASE_TOP>>1)+7'h20, 2'b01, 16'h0004);
        bRet &= WriteReg2bytes(0x0900+(0x20)*2, 0x0004);


        // `endif //SKIP_TS_SYNTHESIZER_APLL
        //
        //
        // `ifndef SKIP_INIMIU

        // -------------------------------------------------------------------
        // initialize MIU
        // -------------------------------------------------------------------
        // $display("--------------------------------------");
        // $display("Initialize MIU ");
        // $display("--------------------------------------");

        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0000);   //dummy register setting
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0000);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0000);   //dummy register setting
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0000);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0000);   //dummy register setting
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0000);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0c01);   //miu software reset
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0c01);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0c00);
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0c00);
        // set frequence 130MHz
        // -- charles_comment 1001 --    `ifndef USE_SBUS
        // -- charles_comment 1001 --      `RIU_W((`RIUBASE_MIU>>1)+7'h10, 2'b11, 16'h000d);
        // -- charles_comment 1001 --    `else
        // -- charles_comment 1001 --      `DBG.SWrite32(1,0,`RIUBASE_MIU+('h10<<1)+1 , 8'h00);
        // -- charles_comment 1001 --      `DBG.SWrite32(1,0,`RIUBASE_MIU+('h10<<1)+0 , 8'h0d);
        // -- charles_comment 1001 --    `endif
        // -- charles_comment 1001 --    `ifndef USE_SBUS
        // -- charles_comment 1001 --      `RIU_W((`RIUBASE_MIU>>1)+7'h30, 2'b11, 16'h0300);
        // -- charles_comment 1001 --    `else
        // -- charles_comment 1001 --      `DBG.SWrite32(1,0,`RIUBASE_MIU+('h30<<1)+1 , 8'h03);
        // -- charles_comment 1001 --      `DBG.SWrite32(1,0,`RIUBASE_MIU+('h30<<1)+0 , 8'h00);
        // -- charles_comment 1001 --    `endif
        // -- charles_comment 1001 --    `ifndef USE_SBUS
        // -- charles_comment 1001 --      `RIU_W((`RIUBASE_MIU>>1)+7'h31, 2'b11, 16'h6000);
        // -- charles_comment 1001 --    `else
        // -- charles_comment 1001 --      `DBG.SWrite32(1,0,`RIUBASE_MIU+('h31<<1)+1 , 8'h60);
        // -- charles_comment 1001 --      `DBG.SWrite32(1,0,`RIUBASE_MIU+('h31<<1)+0 , 8'h00);
        // -- charles_comment 1001 --    `endif
        // `RIU_W((`RIUBASE_MIU>>1)+7'h11, 2'b11, 16'h0078);
        bRet &= WriteReg2bytes(0x1200+(0x11)*2, 0x0052);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h10, 2'b11, 16'h29a3);
        bRet &= WriteReg2bytes(0x1200+(0x10)*2, 0x352b); // 0x29a378 = 166MHz, 0x352b52 = 130MHz
        // `RIU_W((`RIUBASE_MIU>>1)+7'h11, 2'b11, 16'h6078);   //execute reset and porst
        bRet &= WriteReg2bytes(0x1200+(0x11)*2, 0x6052);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h11, 2'b11, 16'h0078);   //exit reset and porst
        bRet &= WriteReg2bytes(0x1200+(0x11)*2, 0x0052);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h12, 2'b11, 16'hc000);
        bRet &= WriteReg2bytes(0x1200+(0x12)*2, 0xc000);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h13, 2'b11, 16'h0000);
        bRet &= WriteReg2bytes(0x1200+(0x13)*2, 0x0000);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h16, 2'b11, 16'h0030);
        bRet &= WriteReg2bytes(0x1200+(0x16)*2, 0x0030);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h1c, 2'b11, 16'h0080); //reg_fb_clk_sel                 : [7]
        bRet &= WriteReg2bytes(0x1200+(0x1c)*2, 0x0080);

        // `RIU_W((`RIUBASE_MIU>>1)+7'h01, 2'b11, 16'h8100);
        bRet &= WriteReg2bytes(0x1200+(0x01)*2, 0x8100);
        // cke               : [0]
        // reg_self_refresh     : [1]
        // reg_dynamic_cke      : [2]
        // reg_dynamic_ck_odt       : [3]
        // reg_dram_bus         : [5:4] 00: 16b, 01: 32b, 10: 64b
        // reg_dram_type            : [7:6] 00: sdr, 01: ddr, 10: ddr2
        // reg_4ba          : [8]    0: 2bk,  1: 4bk
        // reg_col_size         : [10:9]
        // reg_cke_oenz         : [12]
        // reg_dq_oenz          : [13]
        // reg_adr_oenz         : [14]
        // reg_cs_z         : [15]
        // `RIU_W((`RIUBASE_MIU>>1)+7'h01, 2'b11, 16'he100);
        bRet &= WriteReg2bytes(0x1200+(0x01)*2, 0xe100);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h01, 2'b11, 16'h8100);
        bRet &= WriteReg2bytes(0x1200+(0x01)*2, 0x8100);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h02, 2'b11, 16'h0360);  //in_phase
        bRet &= WriteReg2bytes(0x1200+(0x02)*2, 0x0360);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h03, 2'b11, 16'h0030);  //tREFRESH                      : [7:0] * 16   30
        bRet &= WriteReg2bytes(0x1200+(0x03)*2, 0x0030);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h04, 2'b11, 16'h33c9);
        bRet &= WriteReg2bytes(0x1200+(0x04)*2, 0x33c9);
        // reg_tRAS                      : [3:0]        9
        // reg_tRC                       : [7:4]        c
        // reg_tRCD                      : [11:8]       3
        // reg_tRP                       : [15:12]      3
        // `RIU_W((`RIUBASE_MIU>>1)+7'h05, 2'b11, 16'h4232);
        bRet &= WriteReg2bytes(0x1200+(0x05)*2, 0x4232);
        // reg_tRRD                      : [3:0]         2
        // tWR                           : [7:4]         3
        // reg_tMRD                      : [11:8]        2
        // reg_tRTP         : [15:12]   4
        // `RIU_W((`RIUBASE_MIU>>1)+7'h06, 2'b11, 16'h5532);
        bRet &= WriteReg2bytes(0x1200+(0x06)*2, 0x5532);
        // reg_w2r_dly(tWTR)             : [3:0]         2
        // reg_w2r_oen_dly               : [7:4]         3
        // reg_r2w_dly(tRTW)             : [11:8]        5
        // reg_r2w_oen_dly               : [15:12]       5
        // `RIU_W((`RIUBASE_MIU>>1)+7'h07, 2'b11, 16'h400c);
        bRet &= WriteReg2bytes(0x1200+(0x07)*2, 0x400c);
        // tRFC                          : [5:0]         c
        // reg_tRAS[4]                   : [6]           0
        // reg_tRC[4]                    : [7]           0
        // reg_write_latency             : [10:8]        0
        // reg_tCCD                      : [15:14]       1
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0e, 2'b11, 16'h1800);
        bRet &= WriteReg2bytes(0x1200+(0x0e)*2, 0x1800);

        // `RIU_W((`RIUBASE_MIU>>1)+7'h23, 2'b11, 16'h7ffe);  //mask other request
        bRet &= WriteReg2bytes(0x1200+(0x23)*2, 0x7ffe);

        // `RIU_W((`RIUBASE_MIU>>1)+7'h20, 2'b11, 16'hc001);  //reg_rq0_round_robin       : [0]
        bRet &= WriteReg2bytes(0x1200+(0x20)*2, 0xc001);

        // #40000
        //delay 1
        usleep(1000);

        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0c01);  //miu software reset
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0c01);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0c00);  //
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0c00);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0c01);  //miu software reset
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0c01);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h0f, 2'b11, 16'h0c00);  //
        bRet &= WriteReg2bytes(0x1200+(0x0f)*2, 0x0c00);

        // `RIU_W((`RIUBASE_MIU>>1)+7'h01, 2'b11, 16'h010d);
        bRet &= WriteReg2bytes(0x1200+(0x01)*2, 0x010d);
        // #400
        //delay 0
        // $display("--------Initial DRAM start here!!!");
        // `RIU_W((`RIUBASE_MIU>>1)+7'h00, 2'b11, 16'h0001);
        bRet &= WriteReg2bytes(0x1200+(0x00)*2, 0x0001);


        // wait(test_chip_top.dut.i_dig_top.miu_inst.miu_reg_0.init_done);
        // while( (getValue(0x1200) & 0x8000)!= 0x8000);
        bRet &= ReadReg(0x1201, &u8_tmp);
        while(  ((u8_tmp) & 0x80)!= 0x80);

        // $display("--------Initial Done");
        // `RIU_W((`RIUBASE_MIU>>1)+7'h08, 2'b11, 16'h0001);  //turn on report counter    : [0]
        bRet &= WriteReg2bytes(0x1200+(0x08)*2, 0x0001);


        // $display("-------------------------");
        // $display("-- miu self test start --");
        // $display("-------------------------");
        // `RIU_W((`RIUBASE_MIU>>1)+7'h71, 2'b11, 16'h0000);   //base
        bRet &= WriteReg2bytes(0x1200+(0x71)*2, 0x0000);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h72, 2'b11, 16'h0010);   //length
        bRet &= WriteReg2bytes(0x1200+(0x72)*2, 0x0010);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h74, 2'b11, 16'h5aa5);   //test data
        bRet &= WriteReg2bytes(0x1200+(0x74)*2, 0x5aa5);
        // `RIU_W((`RIUBASE_MIU>>1)+7'h70, 2'b11, 16'h0001);   //reg_test_en                   : [0]
        bRet &= WriteReg2bytes(0x1200+(0x70)*2, 0x0001);
        // reg_test_mode                 : [2:1] 00 : address,
        //                                      01 : test data
        //                                      10 : shift data
        // reg_inv_data                  : [3]
        // reg_test_loop                 : [4]
        // reg_force_out                 : [5]
        // reg_force_in                  : [6]
        // reg_read_only                 : [8]
        // reg_write_only                : [9]

        // #10000;
        //delay 1
        usleep(1000);
        // `ifndef USE_SBUS
        //  `RIU_RP((`RIUBASE_MIU>>1)+7'h70, 2'b11, 16'b1xxx_xxxx_xxxx_xxxx);   //wait test_finish
        // `else
        // wait(test_chip_top.dut.i_dig_top.miu_inst.miu_test_0.test_finish);
        //while( (getValue(0x1200+(0x70)*2) & 0x8000) != 0x8000);

        bRet &= ReadReg(0x12E1, &u8_tmp);
        while(  ((u8_tmp) & 0x80)!= 0x80);


        //     if  (test_chip_top.dut.i_dig_top.miu_inst.miu_test_0.test_fail == 1'b1)
        //                   begin
        //            $display("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
        //                   $display("XXXxxx MIU self test FAIL! xxxXXX",$time);
        //        $display("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
        //                   $finish;
        //                   end
        //                 else
        //            begin
        //        $display("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        //                   $display("OOOooo MIU self test PASS! oooOOO",$time);
        //            $display("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        //        end

        // #10000;
        //delay 1
        usleep(1000);

        // MIU self test FAIL let program stuck in this while loop
        //while( (getValue(0x1200+(0x70)*2) & 0x4000) != 0x0000);
        bRet &= ReadReg(0x12E1, &u8_tmp);
        while(  ((u8_tmp) & 0x40)!= 0x00);

        // `RIU_W((`RIUBASE_MIU>>1)+7'h23, 2'b11, 16'h0000);   //reg_rq0_mask
        bRet &= WriteReg2bytes(0x1200+(0x23)*2, 0x0000);

        // $finish;
        // -------------------------------------------------------------------
        // initialize MIU  finish
        // -------------------------------------------------------------------

        // `endif  //SKIP_INIMIU
        //
        // `ifdef SIM_SPIF
        //     `DBG.riu_release_cpuc;
        // `endif
        //
        // `endif  //SKIP_INIVEC
        //
        //
        // `endif// ASIC mode

        // -------------------------------------------------------------------
        //  Turn on pads
        // -------------------------------------------------------------------

        // ------Turn off all pad in
        // [0] reg_set_pad_low
        // [1] reg_set_pad_high
        // [2] reg_set_i2cs_pad_low
        // [3] reg_set_i2cs_pad_high
        // [8] reg_turn_off_pad
        bRet &= WriteReg2bytes(0x0900+(0x28)*2, 0x0000);

        // ------I2CM pad on
        // [1:0]    reg_iicm_pad_sel[1:0]   1:iicm enable 2:UART enable
        // [4]      reg_i2c_sbpm_en     1: enable I2CS bypass to I2CM function
        // [12:8]   reg_i2c_sbpm_idle_num[4:0]  a: default
        bRet &= WriteReg2bytes(0x0900+(0x08)*2, 0x0a01);

        // ------Transport Stream pad on (except TS ERR pad)
        // [15:0]   reg_en_ts_pad[15:0] 0x00ff:normal TS location 0xff00:reverse TS location
        bRet &= WriteReg2bytes(0x0900+(0x2d)*2, 0x00ff);

        // ------Transport Stream pad on (TS ERR pad)
        // [0]      reg_en_ts_err_pad   1: enable
        // [4]      reg_ts_err_pol  1: inverse 0:normal
        bRet &= WriteReg2bytes(0x0900+(0x2e)*2, 0x0001);

        // ------AGC pad on
        // [4]    reg_ifagc_enable  Tuner IF AGC enable. 1:enable
        // [5]    reg_ifagc_odmode  1: inverse IF AGC
        // [6]    reg_ifagc_data_sel    IFAGC data select 0: IF AGC 1: RF AGC
        bRet &= WriteReg2bytes(0x0a00+(0x18)*2, 0x0010);

    }

#if(TIMING_VERIFICATION == 1)
        tmm_2 = GIVE_ME_TIME
#endif

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]MSB123xC_HW_init, bRet=0x%x\n",bRet));

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return bRet;

}

// i2c 150Kb/s, average rate 110Kb/s, 32KB, 2436ms.
MAPI_BOOL Load2Sdram(MAPI_U8 *u8_ptr, MAPI_U16 data_length, MAPI_U16 sdram_win_offset_base)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));

#if(TIMING_VERIFICATION == 1)
    tmm_3 = GIVE_ME_TIME;
#endif

#if USE_SPI_LOAD_TO_SDRAM
    MAPI_U32 u32Addr;

    u32Addr = (MAPI_U32)sdram_win_offset_base * 0x1000; // unit: 4K
    bRet &= MDrv_SS_MIU_Writes(u32Addr, u8_ptr, data_length);
    printf("****SPI load2sdram****, sdram_addr=0x%lx, data_length=0x%x\n", u32Addr, data_length);

    #if (SDRAM_DATA_CHECK == 1)
    MAPI_U16 i, j, index;
    MAPI_U8 buf[SPI_DEVICE_BUFFER_SIZE];

    if((data_length % SPI_DEVICE_BUFFER_SIZE) == 0)
        index = data_length / SPI_DEVICE_BUFFER_SIZE;
    else
        index = data_length / SPI_DEVICE_BUFFER_SIZE + 1;

    for (i=0; i<index; i++)
    {
        memset(buf, 0x00, SPI_DEVICE_BUFFER_SIZE);

        bRet &= MDrv_SS_MIU_Reads(u32Addr+SPI_DEVICE_BUFFER_SIZE*i, buf, SPI_DEVICE_BUFFER_SIZE);

        for (j=0; j<SPI_DEVICE_BUFFER_SIZE; j++)
        {
            //printf("j=%d, buf=0x%x, data=0x%x", j, buf[j], u8_ptr[j]);
            if (buf[j] != u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j])
            {
                printf("spi miu write&read fail! idx=%x, y=0x%x, x=0x%x\n", (SPI_DEVICE_BUFFER_SIZE*i+j), buf[i], u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j]);
                //break;
            }
        }
    }
#endif

#else

    MAPI_U16  sdram_win_offset = sdram_win_offset_base;
    MAPI_U16  x_data_offset = 0;
    MAPI_U16  y_cir_addr = 0;

            if (I2C_CH_Reset(0) == MAPI_FALSE)
    {
        ERR_DEMOD_MSB(printf(">>>MSB123xC CH0 Reset:Fail\n"));
        return MAPI_FALSE;
    }
    else
    {
        // set xData map upper and low bound for 64k DRAM window
        bRet &= WriteReg2bytes(0x2B00+(0x63)*2, 0x3F24);

        // set xData map offset for 64k DRAM window, 64kbytes alignment
        bRet &= WriteReg2bytes(0x2B00+(0x64)*2, 0x0000);

        // set xData map upper and low bound for 4k DRAM window
        bRet &= WriteReg2bytes(0x2B00+(0x65)*2, 0x2014);

        // set xData map offset for 4k DRAM window, 4kbytes alignment
        bRet &= WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);

        // enable xData map for DRAM
        bRet &= WriteReg2bytes(0x2B00+(0x62)*2, 0x0007);

        for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
        {
            if (y_cir_addr == 0x6000)
            {
                //set xData map offset for 4k DRAM window, 4kbytes alignment
                // 0x1000, 4096 bytes
                bRet &= WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                y_cir_addr = SDRAM_BASE;
            }

            // max 0x200, error above.....

            if((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
            {
                bRet &= WriteRegs(y_cir_addr, (u8_ptr + x_data_offset),LOAD_CODE_I2C_BLOCK_NUM);
                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
            }
            else
            {
                bRet &= WriteRegs(y_cir_addr, (u8_ptr + x_data_offset),data_length - x_data_offset);
                y_cir_addr += (data_length - x_data_offset);
                x_data_offset += (data_length - x_data_offset);
            }
        }
        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]x_data_offset=%d,y_cir_addr=%d,z_block_num=%d\n",x_data_offset,y_cir_addr,sdram_win_offset));

#if (SDRAM_DATA_CHECK == 1)
        // beg data check.
        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]SDRAM data check...\n"));

        sdram_win_offset = sdram_win_offset_base;

        // set xData map offset for 4k DRAM window, 4kbytes alignment
        bRet &= WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);

        for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
        {
            MAPI_U8 u8_tmp;
            if (y_cir_addr == 0x6000)
            {
                //set xData map offset for 4k DRAM window, 4kbytes alignment
                // 0x1000, 4096 bytes
                bRet &= WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                y_cir_addr = SDRAM_BASE;
            }

            bRet &= ReadReg(y_cir_addr++, &u8_tmp);
            if(u8_tmp != *(u8_ptr + x_data_offset++))
            {
                ERR_DEMOD_MSB(printf("[msb123xc]error, idx=0x%x, y=0x%x, x=0x%x\n",y_cir_addr-1, u8_tmp, *(u8_ptr + x_data_offset-1)));
            }
        }

        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]SDRAM data check...Done\n"));

        // end data check
#endif
        //  Release xData map for SDRAM
        bRet &= WriteReg2bytes(0x2B00+(0x62)*2, 0x0000);

        // Channel changed from CH 0x00 to CH 0x03
        if (I2C_CH_Reset(3) == MAPI_FALSE)
        {
            ERR_DEMOD_MSB(printf(">>>MSB123xC CH3 Reset:Fail\n"));
            return MAPI_FALSE;
        }
    }
#endif

#if(TIMING_VERIFICATION == 1)
    tmm_4 = GIVE_ME_TIME
            show_timer();
#endif

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));
    return bRet;
}

// 92~95ms roughly
MAPI_BOOL LoadSdram2Sram(MAPI_U8 CodeNum)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_BOOL bRet = true;
    MAPI_U8   u8_tmp = 0;
    MAPI_U8   u8DoneFlag = 0;
    MAPI_U32  u32Timeout = 0;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadSdram2Sram, g_sram_code=0x%x, codeNum=0x%x\n",g_sram_code,CodeNum));

#if(TIMING_VERIFICATION == 1)
    tmm_17 = GIVE_ME_TIME
#endif

             if(CodeNum == g_sram_code)
    {
        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]LoadSdram2Sram, code is available.\n"));
        return bRet;
    }

    bRet &= MSB123xC_MEM_switch(0);

    if(CodeNum == MSB123xC_DVBT2)
        u8_tmp = 1|0x10;
    else if(CodeNum == MSB123xC_DVBT)
        u8_tmp = 2|0x10;
    else if(CodeNum == MSB123xC_DVBC)
        u8_tmp = 3|0x10;
    else
        u8_tmp = 0|0x10;

    // Assign f/w code type to load => 0x11: dvbt2, 0x12: dvbt, 0x13: dvbc
    bRet &= WriteReg(0x0900+(0x4f)*2, u8_tmp);
    // enable miu mask, miu, mcu, gdma
    bRet &= WriteReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= WriteReg(0x1200+(0x23)*2,0xf0);
    // enable mcu
    bRet &= WriteReg(0x0b00+(0x19)*2, 0x00);
    do
    {
        bRet &= ReadReg(0x0900+(0x4f)*2, &u8DoneFlag);

        if (u32Timeout++ > 500)
        {
            printf("@msb123xc, LoadSdram2Sram boot move code fail.!!!\n");
            return MAPI_FALSE;
        }
        usleep(1*1000);

    }
    while(u8DoneFlag != 0xaa);

    // mask miu access of mcu
    bRet &= WriteReg(0x1200+(0x23)*2,0xf2);

    // 10us delay
    usleep(10);

    // Disable MCU
    bRet &= WriteReg(0x0b00+(0x19)*2, 0x03);

    // enable miu mask, miu, mcu, gdma
    bRet &= WriteReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= WriteReg(0x1200+(0x23)*2,0xf0);

    bRet &= MSB123xC_MEM_switch(1);

    if(bRet == false)
    {
        g_sram_code = 0x00;
    }
    else
    {
        g_sram_code = CodeNum;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_18 = GIVE_ME_TIME
#endif

             DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadSdram2Sram, codeNum=0x%x, g_sram_code=0x%x, bRet=0x%x\n",CodeNum,g_sram_code,bRet));

    return bRet;
}


MAPI_BOOL LoadDspCodeToSDRAM_Boot(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U16  code_size, win_offset;
    MAPI_U8   *data_ptr;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_Boot\n"));

    if(!(g_sdram_code&MSB123xC_BOOT))
    {
        if(sizeof(MSB123xC_LIB) > EDINBURGH_BOOT_START_ADDR)
        {
            // boot code
            data_ptr = MSB123xC_LIB + EDINBURGH_BOOT_START_ADDR;
            code_size = EDINBURGH_BOOT_END_ADDR - EDINBURGH_BOOT_START_ADDR + 1;
            win_offset = EDINBURGH_BOOT_WINDOWS_OFFSET;
            bRet &= Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                g_sdram_code |= MSB123xC_BOOT;
            }
        }
        else
        {
            printf("@msb123xc, boot code is unavailable!!!\n");
        }
    }
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_Boot\n"));

    return bRet;
}

MAPI_BOOL LoadDspCodeToSDRAM_dvbt2(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U16  code_size, win_offset;
    MAPI_U8   *data_ptr;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_dvbt2\n"));

    if( !(g_sdram_code&MSB123xC_DVBT2) )
    {
        if(sizeof(MSB123xC_LIB) > EDINBURGH_DVBT2_P1_START_ADDR)
        {
            // dvbt2_p2
            data_ptr = MSB123xC_LIB + EDINBURGH_DVBT2_P2_START_ADDR;
            code_size = EDINBURGH_DVBT2_P2_END_ADDR - EDINBURGH_DVBT2_P2_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT2_P2_WINDOWS_OFFSET;
            bRet &= Load2Sdram(data_ptr,code_size,win_offset);

            // dvbt2_p1
            data_ptr = MSB123xC_LIB + EDINBURGH_DVBT2_P1_START_ADDR;
            code_size = EDINBURGH_DVBT2_P1_END_ADDR - EDINBURGH_DVBT2_P1_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT2_P1_WINDOWS_OFFSET;
            bRet &= Load2Sdram(data_ptr,code_size,win_offset);

            if(bRet == true)
            {
                g_sdram_code |= MSB123xC_DVBT2;
            }
        }
        else
        {
            ERR_DEMOD_MSB(printf("@msb123xc, dvbt2 code is unavailable!!!\n"));
        }
    }
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_dvbt2\n"));

    return bRet;
}

MAPI_BOOL LoadDspCodeToSDRAM_dvbt(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U16  code_size, win_offset;
    MAPI_U8   *data_ptr;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_dvbt\n"));

    if(!(g_sdram_code&MSB123xC_DVBT))
    {
        // dvbt code
        if(sizeof(MSB123xC_LIB) > EDINBURGH_DVBT_START_ADDR)
        {
            data_ptr = MSB123xC_LIB + EDINBURGH_DVBT_START_ADDR;
            code_size = EDINBURGH_DVBT_END_ADDR - EDINBURGH_DVBT_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT_WINDOWS_OFFSET;
            bRet &= Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                g_sdram_code |= MSB123xC_DVBT;
            }
        }
        else
        {
            printf("@msb123xc, dvbt code is unavailable!!!\n");
        }
    }
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_dvbt\n"));
    return bRet;
}

MAPI_BOOL LoadDspCodeToSDRAM_dvbc(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U16  code_size, win_offset;
    MAPI_U8   *data_ptr;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_dvbc\n"));
    if(!(g_sdram_code&MSB123xC_DVBC))
    {
        // dvbc code
        if(sizeof(MSB123xC_LIB) > EDINBURGH_DVBC_START_ADDR)
        {
            data_ptr = MSB123xC_LIB + EDINBURGH_DVBC_START_ADDR;
            code_size = EDINBURGH_DVBC_END_ADDR - EDINBURGH_DVBC_START_ADDR + 1;
            win_offset = EDINBURGH_DVBC_WINDOWS_OFFSET;
            bRet &= Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                g_sdram_code |= MSB123xC_DVBC;
            }
        }
        else
        {
            printf("@msb123xc, dvbc code is unavailable!!!\n");
        }
    }
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_dvbc\n"));
    return bRet;
}

// [0] bootloader [1] dvbt2, [2] dvbt, [3] dvbc [4] all
MAPI_BOOL LoadDspCodeToSDRAM(MAPI_U8 code_n)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM, code_n=0x%x\n",code_n));

#if(TIMING_VERIFICATION == 1)
    tmm_7 = GIVE_ME_TIME
#endif

            switch(code_n)
    {
        case MSB123xC_BOOT:
        {
            // boot code
            bRet &= LoadDspCodeToSDRAM_Boot();
        }
        break;
        case MSB123xC_DVBT2:
        {
            // dvbt2 code
            bRet &= LoadDspCodeToSDRAM_dvbt2();
        }
        break;
        case MSB123xC_DVBT:
        {
            // dvbt
            bRet &= LoadDspCodeToSDRAM_dvbt();
        }
        break;
        case MSB123xC_DVBC:
        {
            // dvbtc
            bRet &= LoadDspCodeToSDRAM_dvbc();
        }
        break;
        case MSB123xC_ALL:
        default:
        {
            // boot+dvbt2+dvbt+dvbc

            // boot code
            bRet &= LoadDspCodeToSDRAM_Boot();
            // dvbt2
            bRet &= LoadDspCodeToSDRAM_dvbt2();
            // dvbt
            bRet &= LoadDspCodeToSDRAM_dvbt();
            // dvbtc
            #if(SKIP_DVBC_LOAD==0)
            bRet &= LoadDspCodeToSDRAM_dvbc();
            #endif
        }
        break;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_8 = GIVE_ME_TIME
            show_timer();
#endif

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM, code_n=0x%x, bRet=0x%x\n",code_n,bRet));
    return bRet;
}

// mem_type 0: dram, 1:dram+sram
// 28 ms roughly
MAPI_BOOL MSB123xC_MEM_switch(MAPI_U8 mem_type)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_BOOL bRet = true;
    MAPI_U8 u8_tmp = 0;
    MAPI_U16 timeout = 0;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]MSB123xC_MEM_switch, mem_type=0x%x\n",mem_type));

#if(TIMING_VERIFICATION == 1)
    tmm_15 = GIVE_ME_TIME
#endif

             if(mem_type == 1)
    {

        bRet &= WriteReg(0x2B80,0x10);

        // SRAM_START_ADDR 0x0000
        bRet &= WriteReg2bytes(0x1000,0x0000);
        bRet &= WriteReg2bytes(0x1004,0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= WriteReg2bytes(0x1002,0x0000);
        bRet &= WriteReg2bytes(0x1006,0x7FFF);

        // DRAM_START_ADDR 0x8000
        bRet &= WriteReg2bytes(0x1008,0x0000);
        bRet &= WriteReg2bytes(0x100C,0x8000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= WriteReg2bytes(0x100A,0x0000);
        bRet &= WriteReg2bytes(0x100E,0xFFFF);

        // Enable SRAM&SDRAM memory map
        bRet &= WriteReg(0x1018,0x05);

        // Wait memory map to be enabled
        do
        {
            bRet &= ReadReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printf("@msb123xc, D+S memory mapping failure.!!!\n");
                return MAPI_FALSE;
            }
        }
        while(u8_tmp != 0x05);
    }
    else if(mem_type == 0)
    {
        // Enable SRAM&SDRAM memory map

        bRet &= WriteReg(0x2B80,0x10);

        // DRAM_START_ADDR 0x8000
        bRet &= WriteReg2bytes(0x1008,0x0000);
        bRet &= WriteReg2bytes(0x100C,0x0000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= WriteReg2bytes(0x100A,0x0000);
        bRet &= WriteReg2bytes(0x100E,0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= WriteReg(0x1018,0x04);

        // Wait memory map to be enabled
        do
        {
            bRet &= ReadReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printf("@msb123xc, D memory mapping failure.!!!\n");
                return MAPI_FALSE;
            }
        }
        while(u8_tmp != 0x04);
    }
    else
    {
        printf("@msb123xc, invalid mem type mapping.\n");
        return MAPI_FALSE;
    }
#if(TIMING_VERIFICATION == 1)
    tmm_16 = GIVE_ME_TIME
#endif

             DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]MSB123xC_MEM_switch, , mem_type=0x%x, bRet=0x%x\n",mem_type,bRet));

    return bRet;
}
//m
static MAPI_BOOL IspProcFlash(MAPI_U8* pLibArry, MAPI_U32 dwLibSize)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U32    dwLoop, dwTimeOut;
    MAPI_U32    dwStartAddr, dwEndAddr;
    //MAPI_U16    wLoop;
    MAPI_U8     bError = false;//, bWriteData[PAGE_WRITE_SIZE];

    MAPI_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_2);
//    iptr->WriteBytes(0, NULL, 5, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 5, bWriteData);

    dwStartAddr = (MAPI_U32) 0;
    dwEndAddr = dwLibSize;


    MAPI_U8     bAddr[1];
    MAPI_U8     bReadData=0;
    for(dwLoop=dwStartAddr; (dwLoop < dwEndAddr); dwLoop+=PAGE_WRITE_SIZE)
    {

        dwTimeOut = 10000;

        while(dwTimeOut--)
        {
            bAddr[0] = 0x10;
            bWriteData[0] = 0x05;
//            iptr->WriteBytes(1, bAddr, 1, bWriteData);
            MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

            bAddr[0] = 0x11;
//            iptr->ReadBytes(1, bAddr, 1, &bReadData);
            MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, &bReadData);

            bWriteData[0] = 0x12;
//            iptr->WriteBytes(0, NULL, 1, bWriteData);
            MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

            if (!(bReadData & 0x01))
                break;

            if(dwTimeOut==1)
            {
                bError = 1;
                break;
            }
            usleep(0);
        }

        if(!bError)
        {

            MAPI_U8    bAddr[5], bWriteData[1];
            MAPI_BOOL bError = TRUE;

//            mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_2);

            bAddr[0] = 0x10;
            bWriteData[0] = 0x06;
//            bError &= iptr->WriteBytes(1, bAddr, 1, bWriteData);
            bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

            bWriteData[0] = 0x12;
//            bError &= iptr->WriteBytes(0, NULL, 1, bWriteData);
            bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

            // Page Program
            bAddr[0] = 0x10;
            bAddr[1] = 0x02;
            bAddr[2] = dwLoop >> 16;
            bAddr[3] = dwLoop >> 8;
            bAddr[4] = dwLoop;

//            bError &= iptr->WriteBytes(5, bAddr, PAGE_WRITE_SIZE, (pLibArry+dwLoop));
            bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 5, bAddr, PAGE_WRITE_SIZE, (pLibArry+dwLoop));

            bWriteData[0] = 0x12;
//            bError &=  iptr->WriteBytes(0, NULL, 1, bWriteData);
            bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);
            
            bAddr[0] = 0x10;
            bWriteData[0] = 0x04;
//            bError &=  iptr->WriteBytes(1, bAddr, 1, bWriteData);
            bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

            bWriteData[0] = 0x12;
//            bError &=  iptr->WriteBytes(0, NULL, 1, bWriteData);
            bError &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

            if(bError == FALSE)
            {
                break;
            }
        }
    }

    bWriteData[0]=0x24 ;
//    iptr->WriteBytes(0, NULL, 1, bWriteData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

    if(bError==false)
        return MAPI_TRUE;
    else
        return MAPI_FALSE;

}

/*
MAPI_BOOL device_demodulator::DeviceDemodCreate()
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    mapi_gpio *gptr = mapi_gpio::GetGPIO_Dev(Demodulator_Reset);
    if(gptr != NULL)
    {
        gptr->SetOff();
    }

    usleep(resetDemodTime*1000);

    if(gptr != NULL)
    {
        gptr->SetOn();
    }
    usleep(resetDemodTime*1000);

    mapi_demodulator *pMapiDemod;
    pMapiDemod = mapi_interface::Get_mapi_demod();
    //Public:Common
    pMapiDemod->stMapiDemodulatorFun.Connect = Connect;
    pMapiDemod->stMapiDemodulatorFun.Disconnect = Disconnect;
    pMapiDemod->stMapiDemodulatorFun.Reset = Reset;
    pMapiDemod->stMapiDemodulatorFun.IIC_Bypass_Mode = IIC_Bypass_Mode;
    pMapiDemod->stMapiDemodulatorFun.Power_On_Initialization = Power_On_Initialization;
    pMapiDemod->stMapiDemodulatorFun.Set_PowerOn = Set_PowerOn;
    pMapiDemod->stMapiDemodulatorFun.Set_PowerOff = Set_PowerOff;
    pMapiDemod->stMapiDemodulatorFun.Active = Active;
    pMapiDemod->stMapiDemodulatorFun.GetCurrentDemodulatorType = GetCurrentDemodulatorType;
    pMapiDemod->stMapiDemodulatorFun.ExtendCmd = ExtendCmd;

    //Public:DTV
    pMapiDemod->stMapiDemodulatorFun.DTV_SetFrequency = DTV_SetFrequency;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetSNR = DTV_GetSNR;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetBER = DTV_GetBER;
//    pMapiDemod->stMapiDemodulatorFun.DTV_GetPacketErr = DTV_GetPacketErr;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetPostBER = DTV_GetPostBER;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetSignalQuality = DTV_GetSignalQuality;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetSignalStrength = DTV_GetSignalStrength;
    pMapiDemod->stMapiDemodulatorFun.DTV_Serial_Control = DTV_Serial_Control;
    pMapiDemod->stMapiDemodulatorFun.DTV_GetCellID = DTV_GetCellID;
    pMapiDemod->stMapiDemodulatorFun.DTV_IsHierarchyOn = DTV_IsHierarchyOn;

    //Public:DTV-DVB-T
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T_GetLockStatus = DTV_DVB_T_GetLockStatus;

    //Public:DTV-DVB-C
    //mick
#if(SKIP_DVBC_LOAD==0)
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_C_GetLockStatus = DTV_DVB_C_GetLockStatus;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_C_SetFrequency = DTV_DVB_C_SetFrequency;
#endif
    //DTV_DVB_C_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth,MAPI_U32 uSymRate,mapi_demodulator::EN_CAB_CONSTEL_TYPE eQAM);
    //DTV_DVB_C_GetCurrentSymbolRate(void);

    //mick
    //CCI_Check( COFDM_CHECK_FLAG eFlag );
    //SetDvbcParam(MAPI_U8 constel);
    //GetDvbcInfo( MAPI_U32 * DVBC_parameter);
    //MSB123xC_Lock(E_SYSTEM system, COFDM_LOCK_STATUS eStatus );
    //Set_Config_dvbc_atv_detector(MAPI_U8 bEnable);
    //MSB123xC_Config_DVBC(MAPI_U16 SymbolRate, MAPI_U32 u32IFFreq, MAPI_U16 bSpecInv);

    //Public:DTV-DVB-T2
    pMapiDemod->stMapiDemodulatorFun.SetCurrentDemodulatorType = SetCurrentDemodulatorType;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T2_GetPlpBitMap = DTV_GetPlpBitMap;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T2_GetPlpGroupID = DTV_GetPlpGroupID;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T2_SetPlpGroupID = DTV_SetPlpGroupID;

    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T_GetSignalModulation = DTV_DVB_T_GetSignalModulation;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T_GetSignalGuardInterval = DTV_DVB_T_GetSignalGuardInterval;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T_GetSignalFFTValue = DTV_DVB_T_GetSignalFFTValue;
    pMapiDemod->stMapiDemodulatorFun.DTV_DVB_T_GetSignalCodeRate = DTV_DVB_T_GetSignalCodeRate;
#if 0
    pMapiDemod->stMapiDemodulatorFun.GetDemodInfo                               = GetDemodInfo;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_s1_siso_miso                = GetDVBT2_Demod_s1_siso_miso;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_number                  = GetDVBT2_Demod_plp_number;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_id                      = GetDVBT2_Demod_plp_id;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_constellation_l1            = GetDVBT2_Demod_constellation_l1;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_coderate_l1                 = GetDVBT2_Demod_coderate_l1;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_constellation           = GetDVBT2_Demod_plp_constellation;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_rotation_constellation  = GetDVBT2_Demod_plp_rotation_constellation;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_coderate                = GetDVBT2_Demod_plp_coderate;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_fec                     = GetDVBT2_Demod_plp_fec;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_frame_il_length         = GetDVBT2_Demod_plp_frame_il_length;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_type_time_il            = GetDVBT2_Demod_plp_type_time_il;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_plp_length_time_il          = GetDVBT2_Demod_plp_length_time_il;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_pilot_pattern               = GetDVBT2_Demod_pilot_pattern;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_mode_carrier                = GetDVBT2_Demod_mode_carrier;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_fef_type                    = GetDVBT2_Demod_fef_type;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_fef_length                  = GetDVBT2_Demod_fef_length;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_fef_interval                = GetDVBT2_Demod_fef_interval;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_papr                        = GetDVBT2_Demod_papr;
    pMapiDemod->stMapiDemodulatorFun.GetDVBT2_Demod_guard_interval              = GetDVBT2_Demod_guard_interval;
#endif

#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)

    int intPTHChk;

    pthread_t pthread_id;
    pthread_attr_t attr1;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);

    intPTHChk = PTH_RET_CHK(pthread_create(&pthread_id, &attr1, PreLoadDSPcode, NULL));

    if(intPTHChk != 0)
    {
        ASSERT(0);
    }
#endif

    _s32_Demod_DVBT2_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2", MSOS_PROCESS_SHARED);
    _s32_Demod_DVBT2_RW_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2RW", MSOS_PROCESS_SHARED);
    _s32_Demod_DVBT2_RW2_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2RW2", MSOS_PROCESS_SHARED);


    return MAPI_TRUE;
}
*/
//m
void* PreLoadDSPcode(void *arg)
{
#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    ERR_DEMOD_MSB(printf("[msb123xc][beg] PreLoadDSPcode beg...\n"));

    MAPI_U8     status = TRUE;

    if(MSB123xC_Set_bonding_option(MDrv_SYS_GetChipID()) == FALSE)
    {
        ERR_DEMOD_MSB(printf(">>>>MSB123xC_Set_bonding_option:Fail\n"));
        pthread_exit(NULL);
        return NULL;
    }

    Connect(E_DEVICE_DEMOD_DVB_T);

    status &= I2C_CH_Reset(3);
    if(status == MAPI_FALSE)
    {
        ERR_DEMOD_MSB(printf("[msb123xc][err] I2C_CH_Reset failure...\n"));
        bPreloadFail = MAPI_TRUE;
        pthread_exit(NULL);
        return NULL;
    }

    status &= MSB123xC_HW_init();
    if(status == MAPI_FALSE)
    {
        ERR_DEMOD_MSB(printf("[msb123xc][err] MSB123xC_HW_init failure...\n"));
        bPreloadFail = MAPI_TRUE;
        pthread_exit(NULL);
        return NULL;
    }

    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);

    status &= LoadDspCodeToSDRAM(MSB123xC_ALL);
    if(status == MAPI_FALSE)
    {
        ERR_DEMOD_MSB(printf("[msb123xc][err] LoadDspCodeToSDRAM failure...\n"));
        MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    }
    else
    {
        MAPI_U8 u8Data = 0;
        switch(m_enCurrentDemodulator_Type)
        {
            case E_DEVICE_DEMOD_DVB_T2:
                u8Data=MSB123xC_DVBT2;
                break;
            case E_DEVICE_DEMOD_DVB_T:
                u8Data=MSB123xC_DVBT;
                break;
            case E_DEVICE_DEMOD_DVB_C:
                u8Data=MSB123xC_DVBC;
                break;
            default:
                u8Data=MSB123xC_DVBT;
                bPreloadFail = MAPI_TRUE;
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                pthread_exit(NULL);
                return NULL;
                break;
        }

        // mask miu access for all and mcu
        status &= WriteReg(0x1200+(0x23)*2 + 1,0x7f);
        status &= WriteReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        usleep(10);

        status &= WriteReg(0x0b00 + (0x19) * 2, 0x03);

        status &= LoadSdram2Sram(u8Data);

        // enable miu access of mcu gdma
        status &= WriteReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        status &= WriteReg(0x0b00+(0x19)*2, 0x00);

        switch(m_enCurrentDemodulator_Type)
        {
            case E_DEVICE_DEMOD_DVB_T2:
                DTV_DVBT2_DSPReg_Init();
                break;
            case E_DEVICE_DEMOD_DVB_T:
                DTV_DVBT_DSPReg_Init();
                break;
        #if(SKIP_DVBC_LOAD==0)
            case E_DEVICE_DEMOD_DVB_C://mick
                DTV_DVBC_DSPReg_Init();
                break;
        #endif
            default:
                bPreloadFail = MAPI_TRUE;
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                pthread_exit(NULL);
                return NULL;
                break;
        }

        MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    }

    if(status == MAPI_FALSE)
    {
        ERR_DEMOD_MSB(printf(">>>>MSB123xC:Fail\n"));
        bPreloadFail = MAPI_TRUE;
    }
    else
    {
        bPower_init_en = TRUE;
    }
#endif
    pthread_exit(NULL);
}

/*@ <Operation ID=I2b28dd03m121c8cf959bmm722c> @*/

MAPI_BOOL Connect(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_MSB(printf("device_demodulator_msb123xc: Connect type=%d\n",(int)enDemodType));

    if (u8DeviceBusy == 1)
        return MAPI_FALSE;
    m_enCurrentDemodulator_Type = enDemodType;
    u8DeviceBusy = 1;
    return MAPI_TRUE;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm722c> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm7207> @*/

MAPI_BOOL Disconnect(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_MSB(printf("device_demodulator_msb123xc: Disconnect\n"));
    u8DeviceBusy = 0;
    return MAPI_TRUE;
}

#include "drvXC_IOPort.h"
#include "apiXC.h"

/*@ </Operation ID=I2b28dd03m121c8cf959bmm7207> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
void Reset()
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8     u8Data = 0x00;
    MAPI_U32    u32Retry = 0x00;

    DBG_DEMOD_MSB(printf(">>>MSB123xC: Reset()\n"));
    u32StartTime = MsOS_GetSystemTime();
    DBG_DEMOD_MSB(printf("\t\t\tRESET TIME   %ld\n", u32StartTime));//to measure time
    bDoReset = TRUE;

    if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
        // mask miu access for all and mcu
        WriteReg(0x1200+(0x23)*2 + 1,0x7f);
        WriteReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        usleep(10);

        WriteReg(0x0B00 + (0x10) * 2, 0x01);
        WriteReg(0x0B00 + (0x19) * 2, 0x03);
        WriteReg(0x0C00, 0x00);             //clear MB_CNTL
        usleep(5 * 100);

        // enable miu access of mcu gdma
        WriteReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        usleep(10);

        WriteReg(0x0B00 + (0x19) * 2, 0x00);
        WriteReg(0x0B00 + (0x10) * 2, 0x00);
        usleep(5 * 1000);
        ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready
        while(u8Data != 0xff)
        {
            usleep(50*1000);
            ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                ERR_DEMOD_MSB(printf(">>>MSB123xC: Reset Fail!\n"));
                break;
            }
        }
    }
    else if ((m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T) ||
             (m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_C))
    {
        // mask miu access for all and mcu
        WriteReg(0x1200+(0x23)*2 + 1,0x7f);
        WriteReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        usleep(10);

        WriteReg(0x0B00 + (0x19) * 2, 0x01);
        WriteReg(0x0B00 + (0x10) * 2, 0x01);
        WriteReg(0x0C00, 0x00);             //clear MB_CNTL

        usleep(5 * 1000);

        // enable miu access of mcu gdma
        WriteReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        usleep(10);

        WriteReg(0x0B00 + (0x10) * 2, 0x00);
        WriteReg(0x0B00 + (0x19) * 2, 0x00);

        ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
        while(u8Data != 0xff)
        {
            usleep(50*1000);
            ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                ERR_DEMOD_MSB(printf(">>>MSB123xC: Reset Fail!\n"));
                break;
            }
        }

    }
    //printf(">>>MSB123xC: Reset ok!\n");

    FECLock = MAPI_FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();
    p1_ever_lock_flag = 0;

}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
MAPI_BOOL IIC_Bypass_Mode(MAPI_BOOL enable)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8 u8Retry=10;

    DBG_DEMOD_MSB(printf(">>>MSB123xC: IIC_bypass() set %x\n", enable));

//FRA_T2_BRINGUP
    while(u8Retry--)
    {
        if (enable)
            WriteReg(0x0900+(0x08)*2, 0x10);// IIC by-pass mode on
        else
            WriteReg(0x0900+(0x08)*2, 0x00);// IIC by-pass mode off
    }
    return MAPI_TRUE;
}

MS_U16 MSB123xc_CHIP_MATCH_TABLE[] =
{
    //Kaiser, Kaiserin, Keltic, Kronus
    0x56,       0x41,     0x72,  0x2F,
};

MAPI_BOOL MSB123xC_Set_bonding_option(MS_U16 u16ChipID)
{
    MS_BOOL  bRet = TRUE;
    MS_U8    u8Idx;
    MS_U8    u8MatchFlag  = 0;
    MS_U8    u8Data  = 0;
    MS_U32    u32_timeout = 0;

    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    //printf("MSB123xc_set_bonding_option u16ChipID %x\n", u16ChipID);

    for (u8Idx = 0 ; u8Idx < (sizeof(MSB123xc_CHIP_MATCH_TABLE) / sizeof(MSB123xc_CHIP_MATCH_TABLE[0])) ; u8Idx++)
    {
        if(u16ChipID == MSB123xc_CHIP_MATCH_TABLE[u8Idx])
        {
            u8MatchFlag = 0x01;
            break;
        }
        else
        {
            u8MatchFlag = 0x00;
        }
    }

    if (I2C_CH_Reset(3) == FALSE)
    {
        ERR_DEMOD_MSB(printf(">>>MSB123xc CH3 Reset:Fail\n"));
        //printf(">>>MSB123xc CH3 Reset:Fail\n");
        return FALSE;
    }

    // MSB123xc : 0x0902[8]=0 , 0x0902[0]=0;
    // MSB1235 : 0x0902[8]=1 , 0x0902[0]=1; (before overwrite, SDRAM not enable)
    //                  0x0902[8]=1 , 0x0902[0]=0; (after overwrite, SDRAM enable)
    // check bonding value, 0x0902[8]
    bRet &= ReadReg(0x0900+(0x02)*2+1, &u8Data);
    if((u8Data & 0x01) == 0x01) //for MSB1236C
    {
        if(u8MatchFlag == 0x01)
        {
            //check overwrite or not
            //0x0902[0] : reg_bonding[0]
            bRet &= ReadReg(0x0900+(0x02)*2, &u8Data);
            if((u8Data & 0x01) != 0x00)
            {
                //0x0905[0] : reg_bond_ov_en[0] = 1
                //0x0905[8] : reg_bond_ov[0] = 0
                // set overwrite enable
                bRet &= WriteReg(0x0900+(0x05)*2, 0x01);
                // set overwrite value
                bRet &= WriteReg(0x0900+(0x05)*2+1, 0x00);
            }

            do
            {
                bRet &= ReadReg(0x0900+(0x02)*2, &u8Data);
                if(u32_timeout++ > 500)
                {
                    ERR_DEMOD_MSB(printf("@msb1236c, Set bonding option failure.!!!\n"));
                    return FALSE;
                }
            }while((u8Data & 0x01) == 0x01);

            printf("@ Set bonding option for MSB1236 \n");
        }
        else
        {
            return FALSE;
        }
    }
    else  // for MSB123xc
    {
        //check overwrite or not
        //0x0902[0] : reg_bonding[0]
        bRet &= ReadReg(0x0900+(0x02)*2, &u8Data);
        if((u8Data & 0x01) != 0x00)
        {
            //0x0905[0] : reg_bond_ov_en[0] = 1
            //0x0905[8] : reg_bond_ov[0] = 0
            // set overwrite enable
            bRet &= WriteReg(0x0900+(0x05)*2, 0x01);
            // set overwrite value
            bRet &= WriteReg(0x0900+(0x05)*2+1, 0x00);
        }

        do
        {
            bRet &= ReadReg(0x0900+(0x02)*2, &u8Data);
            if(u32_timeout++ > 500)
            {
                ERR_DEMOD_MSB(printf("@msbMSB123xc, Set bonding option failure.!!!\n"));
                return FALSE;
            }
        }while((u8Data & 0x01) == 0x01);

        printf("@ Set bonding option for MSB123xc \n");
    }

    if (!bRet) printf("%s %d Error\n", __func__, __LINE__);
        return bRet;
}

/*@ <Operation ID=Im17018142m1221763cc7cmm46c6> @*/
extern MS_BOOL MDrv_Tuner_Init(void);
MAPI_BOOL Power_On_Initialization(void)
{

    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));


#if(TIMING_VERIFICATION == 1)
    tmm_1 = GIVE_ME_TIME
#endif

#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
    IIC_Bypass_Mode(1);
    MDrv_Tuner_Init();
    IIC_Bypass_Mode(0);
    return MAPI_TRUE;
#endif

    MAPI_U8     status = TRUE;
    MAPI_BOOL   bMatch = false;
    MAPI_U8     u8RetryCnt = 6;

    DBG_DEMOD_MSB(printf(">>>MSB123xC: Enter Power_On_Initialization()\n"));

    if (bPower_init_en == TRUE)
    {
        return  MAPI_TRUE;
    }

    if(g_WO_SPI_FLASH == 1)
    {
#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
#else
        // HW reset
/*        mapi_gpio *gptr = mapi_gpio::GetGPIO_Dev(Demodulator_Reset);
        if(gptr != NULL)
        {
            gptr->SetOff();
        }
*/
        mdrv_gpio_set_low(DEMODULATOR_RESET_GPIO);

        usleep(resetDemodTime*1000);

/*
        if(gptr != NULL)
        {
            gptr->SetOn();
        }
*/
    mdrv_gpio_set_high(DEMODULATOR_RESET_GPIO);

        // check bonding option
        if (MSB123xC_Set_bonding_option(MDrv_SYS_GetChipID()) == FALSE)
        {
            return MAPI_TRUE;
        }

        status &= I2C_CH_Reset(3);

        status &= MSB123xC_HW_init();

        //Initialize SPI0 for MSPI
#if USE_SPI_LOAD_TO_SDRAM
        if(MDrv_MSPI_Init_Ext(0) != E_MSPI_OK)
        {
            ERR_DEMOD_MSB(printf(">>>>MDrv_MSPI_Init_Ext:Fail\n"));
        status= MAPI_FALSE;
        }
#endif

        status &= LoadDspCodeToSDRAM(MSB123xC_ALL);

        if (LoadDSPCode() == MAPI_FALSE)
        {
            ERR_DEMOD_MSB(printf(">>>>MSB123xC:Fail\n"));
            status= MAPI_FALSE;
        }

#endif
    }
    else
    {

            MAPI_U8     u8DoneFlag = 0;
            MAPI_U16    u16_counter = 0;

            MAPI_U16    crc16 = 0;

            crc16 = MSB123xC_LIB[sizeof(MSB123xC_LIB)-2];
            crc16 = (crc16<<8)|MSB123xC_LIB[sizeof(MSB123xC_LIB)-1];

            do
            {
                DBG_DEMOD_MSB(printf(">>>MSB123xC: u8RetryCnt = %d\n",u8RetryCnt));

                MAPI_U8 flash_waiting_ready_timeout = 0;
                u8RetryCnt--;

#if(TIMING_VERIFICATION)
                tmm_3 = GIVE_ME_TIME
#endif


                        status = msb123xc_flash_boot_ready_waiting(&flash_waiting_ready_timeout);
                if ( (flash_waiting_ready_timeout == 1) || (status == FALSE) ) ERR_DEMOD_MSB(printf("[msb123xc][error]msb123xc_flash_boot_ready_waiting fail....\n");)


#if(TIMING_VERIFICATION)
                    tmm_4 = GIVE_ME_TIME
                            printf("[tmm1]t4-t3 = %ld (%ld - %ld)\n",tmm_4-tmm_3,tmm_4,tmm_3);
#endif


                if(status == FALSE)
                {
#ifndef T3_Winbledon
                    DBG_DEMOD_MSB(printf(">>>MSB123xC: Reset Demodulator\n"));

/*
                    mapi_gpio *gptr = mapi_gpio::GetGPIO_Dev(Demodulator_Reset);
                    if(gptr != NULL)
                    {
                        gptr->SetOff();
                    }
*/
                    mdrv_gpio_set_low(DEMODULATOR_RESET_GPIO);

                    usleep(resetDemodTime*1000);

/*
                    if(gptr != NULL)
                    {
                        gptr->SetOn();
                    }
*/
                    mdrv_gpio_set_high(DEMODULATOR_RESET_GPIO);

                    usleep(waitFlashTime * 1000);
                    if (I2C_CH_Reset(3) == MAPI_FALSE)
                    {
                        DBG_DEMOD_MSB(printf(">>>MSB123xC CH Reset:Fail\n"));
                        status= MAPI_FALSE;
                        continue;
                    }

                    u16_counter = 1000;
                    do
                    {
                        // 10 ms
                        usleep(10*1000);
                        u16_counter--;
                        ReadReg(0x0900+(0x4f)*2, &u8DoneFlag);
                    }
                    while(u8DoneFlag != 0x99 && u16_counter != 0);

                    if(u16_counter == 0 && u8DoneFlag != 0x99)
                    {
                        DBG_DEMOD_MSB(printf("[wb]Err, MSB123xC didn't ready yet\n"));
                        status = false;
                    }
                    else
                        status = TRUE;

#endif
                }
                // No need to switch to CH0 before SPI Flash access.
#if 0
                if (I2C_CH_Reset(0) == MAPI_FALSE)
                {
                    ERR_DEMOD_MSB(printf(">>>MSB123xC CH0 Reset:Fail\n"));
                    status= MAPI_FALSE;
                    continue;
                }
                else
#endif
                {

//                DBG_DEMOD_MSB(printf(">>>MSB123xC CH0 Reset:OK\n"));

                    DBG_DEMOD_MSB(printf(">>>MSB123xC: Check Version..."));

                    if (IspCheckVer(MSB123xC_LIB, &bMatch) == MAPI_FALSE)
                    {
                        ERR_DEMOD_MSB(printf(">>> ISP read FAIL!\n"));
                        status= MAPI_FALSE;
                        continue;
                    }


                    if(bMatch == false)
                    {
                        printf(">>> IspCheckVer FAIL!\n");
                    }
                    else
                    {



                        if (I2C_CH_Reset(3) == MAPI_FALSE)
                        {
                            (printf(">>>MSB123xC CH Reset:Fail\n"));
                            status= MAPI_FALSE;
                            continue;
                        }
                        else
                        {
                            (printf(">>>MSB123xC CH Reset:OK\n"));
                        }

                        if (dram_crc_check(crc16, &bMatch) == MAPI_FALSE)
                        {
                            (printf(">>> reg read fail!\n"));
                            status= MAPI_FALSE;
                            continue;
                        }

                        if(bMatch == false)
                            printf(">>> dram crc check FAIL!\n");
                        else
                            printf(">>> dram crc check OK!\n");


                    }

#if (FLASH_WP_ENABLE == 1)
                    if (bMatch == false)
                    {
                        // disable flash WP, pull high.
                        if(msb123xc_flash_WP(0) == false)
                        {
                            DBG_FLASH_WP(printf("[wb]Err, FLASH WP Disable Fail!!!\n");)
                        }
                        usleep(100*1000);
                    }
#endif
                    ////bMatch = true; //FIXME : Remove this to enable auto FW reload.
                    if (bMatch == false)// Version not match
                    {
                        MAPI_U8     bAddr[1];
                        MAPI_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};

                        printf(">>> Not match! Reload Flash...");
                        if ( (sizeof(MSB123xC_LIB)%256) != 0)
                        {
                            printf(" MSB123xC_LIB 256byte alignment error!%u \n",sizeof(MSB123xC_LIB));
                        }

//                        mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_2);
//                        iptr->WriteBytes(0, NULL, 5, bWriteData);
                        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 5, bWriteData);

                        bAddr[0] = 0x10;
                        bWriteData[0] = 0x06;
//                        iptr->WriteBytes(1, bAddr, 1, bWriteData);
                        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

                        bWriteData[0] = 0x12;
//                        iptr->WriteBytes(0, NULL, 1, bWriteData);
                        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

                        bAddr[0] = 0x10;

                        bWriteData[0] = 0xC7;
//                        iptr->WriteBytes(1, bAddr, 1, bWriteData);
                        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 1, bAddr, 1, bWriteData);

                        bWriteData[0] = 0x12;
//                        iptr->WriteBytes(0, NULL, 1, bWriteData);
                        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);
                        
                        bWriteData[0]=0x24 ;
//                        iptr->WriteBytes(0, NULL, 1, bWriteData);
                        MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID_2, 0, NULL, 1, bWriteData);

                        DBG_DEMOD_MSB(printf("\t\t\tStart   %ld\n", MsOS_GetSystemTime()));//to measure time
                        if ( (sizeof(MSB123xC_LIB) - 2) > MAX_MSB123xC_LIB_LEN)
                        {
                            printf("Err, msb123xc_lib size(%d) is larger than flash size(%d)\n",sizeof(MSB123xC_LIB),MAX_MSB123xC_LIB_LEN);
                        }

                        // if (IspProcFlash(MSB123xC_LIB, sizeof(MSB123xC_LIB)) == MAPI_FALSE)
                        if (IspProcFlash(MSB123xC_LIB, sizeof(MSB123xC_LIB)-2) == MAPI_FALSE)
                        {
                            ERR_DEMOD_MSB(printf(" ISP write FAIL\n"));
                            status= MAPI_FALSE;
                            continue;
                        }
                        else
                        {
                            DBG_DEMOD_MSB(printf("\t\t\tEnd   %ld\n", MsOS_GetSystemTime()));//to measure time
                            //check again
                            if ((IspCheckVer(MSB123xC_LIB, &bMatch) == MAPI_FALSE)||(bMatch==false))
                            {
                                ERR_DEMOD_MSB(printf(">>> ISP read FAIL! bMatch %d \n",bMatch));
                                status= MAPI_FALSE;
                                continue;
                            }
                            else // reset again
                            {
#ifndef T3_Winbledon
                                DBG_DEMOD_MSB(printf(">>>MSB123xC: Reset Demodulator\n"));

                                printf(">>>MSB123xC[2]: Reset Demodulator\n");
/*
                                mapi_gpio *gptr = mapi_gpio::GetGPIO_Dev(Demodulator_Reset);

                                if(gptr != NULL)
                                {
                                    gptr->SetOff();
                                }
*/
                                mdrv_gpio_set_low(DEMODULATOR_RESET_GPIO);

                                usleep(resetDemodTime*1000);

/*
                                if(gptr != NULL)
                                {
                                    gptr->SetOn();
                                }
*/
                                mdrv_gpio_set_high(DEMODULATOR_RESET_GPIO);

                                usleep(waitFlashTime * 1000);

                                if (I2C_CH_Reset(3) == MAPI_FALSE)
                                {
                                    ERR_DEMOD_MSB(printf(">>>MSB123xC CH Reset:Fail\n"));
                                    status= MAPI_FALSE;
                                    continue;
                                }

                                u16_counter = 1000;
                                do
                                {
                                    // 10 ms
                                    usleep(10*1000);
                                    u16_counter--;
                                    ReadReg(0x0900+(0x4f)*2, &u8DoneFlag);
                                }
                                while(u8DoneFlag != 0x99 && u16_counter != 0);

                                if(u16_counter == 0 && u8DoneFlag != 0x99)
                                {
                                    ERR_DEMOD_MSB(printf("[wb]Err, MSB123xC didn't ready yet\n"));
                                    status = false;
                                }
                                else
                                    status = TRUE;
#endif

                                if (I2C_CH_Reset(3) == MAPI_FALSE)
                                {
                                    (printf(">>>MSB123xC CH Reset:Fail\n"));
                                    status= MAPI_FALSE;
                                    continue;
                                }
                                else
                                {
                                    (printf(">>>MSB123xC CH Reset:OK\n"));
                                }

                                if (dram_crc_check(crc16, &bMatch) == MAPI_FALSE)
                                {
                                    (printf(">>> reg read fail!\n"));
                                    status= MAPI_FALSE;
                                    continue;
                                }

                                if(bMatch == false)
                                    printf(">>> dram crc check FAIL!\n");
                                else
                                    printf(">>> dram crc check OK!\n");
                            }
                            DBG_DEMOD_MSB(printf(" OK\n"));
                        }
                    }
                    else
                    {
                        // Version match, do nothing
                        DBG_DEMOD_MSB(printf(">>> Match\n"));
                    }

#if (FLASH_WP_ENABLE == 1)
                    if (bMatch == true)
                    {
                        // Enable flash WP, pull high.
                        if(msb123xc_flash_WP(1) == false)
                        {
                            DBG_FLASH_WP(printf("[wb]Err, FLASH WP Enable Fail!!!\n");)
                        }
                        usleep(100*1000);
                    }
#endif
                }


                if (I2C_CH_Reset(3) == MAPI_FALSE)
                {
                    ERR_DEMOD_MSB(printf(">>>MSB123xC CH Reset:Fail\n"));
                    status= MAPI_FALSE;
                    continue;
                }
                else
                {
                    DBG_DEMOD_MSB(printf(">>>MSB123xC CH Reset:OK\n"));
                }

#if(TIMING_VERIFICATION)
                tmm_5 = GIVE_ME_TIME
#endif
                        if (LoadDSPCode() == MAPI_FALSE)
                {
                    ERR_DEMOD_MSB(printf(">>>>MSB123xC:Fail\n"));
                    status= MAPI_FALSE;
                    continue;
                }
                else
                {
                    DBG_DEMOD_MSB(printf(">>>MSB123xC:OK\n"));
                }
#if(TIMING_VERIFICATION)
                tmm_6 = GIVE_ME_TIME
                        printf("[tmm1]t6-t5 = %ld (%ld - %ld)\n",tmm_6-tmm_5,tmm_6,tmm_5);
#endif


                ReadReg(0x0900+(0x01)*2, &gu8ChipRevId);
                DBG_DEMOD_MSB(printf(">>>MSB123xC:Edinburgh RevID:%x\n", gu8ChipRevId));


                ReadReg(0x0900+(0x49)*2, &gu8ChipRevId);
                DBG_DEMOD_MSB(printf(">>>MSB123xC:Edinburgh 0x49_L:%x\n", gu8ChipRevId));

                ReadReg(0x0900+(0x49)*2+1, &gu8ChipRevId);
                DBG_DEMOD_MSB(printf(">>>MSB123xC:Edinburgh 0x49_H:%x\n", gu8ChipRevId));

                ReadReg(0x0900+(0x4A)*2, &gu8ChipRevId);
                DBG_DEMOD_MSB(printf(">>>MSB123xC:Edinburgh 0x4A_L:%x\n", gu8ChipRevId));

            }
            while((u8RetryCnt>0)&&(status==FALSE));
        }

    if(status==FALSE)
    {
        ERR_DEMOD_MSB(printf("msb123xc power_on_init FAIL !!!!!! \n\n"));
    }
    else
    {
        bPower_init_en = MAPI_TRUE;
        printf("msb123xc power_on_init OK !!!!!! \n\n");


#if(TIMING_VERIFICATION == 1)
        tmm_2 = GIVE_ME_TIME
                printf("[tmm]t2-t1 = %ld (%ld - %ld)\n",tmm_2-tmm_1,tmm_2,tmm_1);
#endif


    }
    IIC_Bypass_Mode(1);
    MDrv_Tuner_Init();
    IIC_Bypass_Mode(0);
    return status;
}

/*@ </Operation ID=Im17018142m1221763cc7cmm46c6> @*/
//m
MAPI_BOOL Set_PowerOn(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    return TRUE;
}
//m
MAPI_BOOL Set_PowerOff(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    return TRUE;
}
//--------------------------------------------------------------------- for DVB-T
//operation
#define RFAGC_EN                0x00
#define HUMDET_EN               0x00
#define DCR_EN                  0x01
#define IIS_EN                  0x01
#define CCI_EN                  0x01
#define ACI_EN                  0x01
#define IQB_EN                  0x00
#define AUTO_IQ_SWAP            0x01
#define AUTO_RFMAX              0x00
#define AUTO_ACI                0x00
#define MODE_CP_FORCED          0x00
#define TPS_FORCED              0x00
#define AUTO_SCAN               0x00
#define RSV_0D                  0x00
#define RSV_0E                  0x00
#define RSV_0F                  0x00

//configure
#define RSSI                    0x00
#define ZIF                     0x00
#define FREQ                    0x00
#define FC_L                    0xc0
#define FC_H                    0x12
#define FS_L                    0x80
#define FS_H                    0x70
#define BW                      0x03
#define MODE                    0x01
#define CP                      0x03
#define LP_SEL                  0x00
#define CSTL                    0x00
#define HIER                    0x00
#define HPCR                    0x00
#define LPCR                    0x00
#define IQ_SWAP                 0x00
#define RFMAX                   0x01
#define ATV_SYSTEM              0x01
#define ICFO_RANGE              0x01
#define RFAGC_REF               0x64
#define IFAGC_REF_2K            0x4B
#define IFAGC_REF_8K            0x4B
#define IFAGC_REF_ACI           0x4B
#define IFAGC_REF_IIS_2K        0xA0
#define IFAGC_REF_IIS_8K        0xA0
#define IFAGC_ACI_DET_TH_L      0x9A
#define IFAGC_ACI_DET_TH_H      0x01
#define SERIAL_TS               0x00
#define TS_CLK_SEL              0x06
#define TS_OUT_INV              0x01
#define TS_DATA_SWAP            0x00
#define SFO_2K_H                0x00
#define SFO_2K_L                0xC8
#define SFO_8K_H                0x00
#define SFO_8K_L                0xC8
#define CHECK_CHANNEL           0x00
#define SNR_POS                 0x00
#define CCI_KP                  0x00
#define CCI_FSWEEP              0x00
#define TS_CLK_RATE_AUTO        0x00
#define DVBT_CRC                0x00


MAPI_U8 MSB123xC_DVBT_DSPREG_TABLE[] = // andy 2009-12-15  3:55:03 TW model
{
#if 1
    RFAGC_EN,     HUMDET_EN,    DCR_EN,     IIS_EN,         CCI_EN,      ACI_EN,
    IQB_EN,       AUTO_IQ_SWAP, AUTO_RFMAX, AUTO_ACI,       MODE_CP_FORCED, TPS_FORCED,
    AUTO_SCAN,    RSV_0D,       RSV_0E,     RSV_0F,
    RSSI,         ZIF,          FREQ,       FC_L,           FC_H,        FS_L,      FS_H,
    BW,           MODE,         CP,         LP_SEL,         CSTL,        HIER,      HPCR,
    LPCR,         IQ_SWAP,      RFMAX,      ATV_SYSTEM,     ICFO_RANGE,  RFAGC_REF,
    IFAGC_REF_2K, IFAGC_REF_8K, IFAGC_REF_ACI,  IFAGC_REF_IIS_2K,  IFAGC_REF_IIS_8K, IFAGC_ACI_DET_TH_L,
    IFAGC_ACI_DET_TH_H,         SERIAL_TS,  TS_CLK_SEL,  TS_OUT_INV,  TS_DATA_SWAP,
    SFO_2K_H,     SFO_2K_L,     SFO_8K_H,   SFO_8K_L,       CHECK_CHANNEL,          SNR_POS,
    CCI_KP,       CCI_FSWEEP    ,TS_CLK_RATE_AUTO, DVBT_CRC
#else
   RFAGC_EN,
   ZIF,
   FC_L,
   FC_H,
   BW,
   IQ_SWAP,
   SERIAL_TS,
   TS_SERIAL_CLK,
   TS_OUT_INV,
   TS_DATA_SWAP,
   TS_CLK_RATE_AUTO,
  DVBT_CRC,

#endif
};

MAPI_U8  DTV_DVBT_DSPReg_CRC(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_U8 crc = 0;
    MAPI_U8 idx = 0;

    for (idx = 0; idx<((MAPI_U8)DVBT_PARAM_LEN - (MAPI_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        crc ^= MSB123xC_DVBT_DSPREG_TABLE[idx];
    }

    crc = ~crc;

    return crc;
}

void  DTV_DVBT_DSPReg_ReadBack(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_U8 ret = 0;
    MAPI_U8 idx = 0;

    for (idx = T_OPMODE_RFAGC_EN; idx< (MAPI_U8)DVBT_PARAM_LEN; idx++)
    {
        ReadDspReg(idx,&ret);
        printf("@msb123xc, idx=0x%x, dsp=0x%x, sw=0x%x\n",idx,ret,MSB123xC_DVBT_DSPREG_TABLE[idx-(MAPI_U8)T_OPMODE_RFAGC_EN]);
    }

    return;
}

MAPI_BOOL  DTV_DVBT_DSPReg_Init(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8    idx = 0;

#if(TIMING_VERIFICATION == 1)
    tmm_7 = GIVE_ME_TIME
#endif

            for (idx = 0; idx<((MAPI_U8)DVBT_PARAM_LEN - (MAPI_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        if( WriteDspReg(idx + (MAPI_U8)T_OPMODE_RFAGC_EN, MSB123xC_DVBT_DSPREG_TABLE[idx])!=TRUE)
        {
            ERR_DEMOD_MSB(printf("dsp reg init NG\n"));
            return FALSE;
        }
    }

#if(TIMING_VERIFICATION == 1)
    tmm_8 = GIVE_ME_TIME
            printf("[1111]t8 - t7 = %ld (%ld - %ld) \n",tmm_8-tmm_7,tmm_8,tmm_7);
#endif


#if(TIMING_VERIFICATION == 1)
    tmm_7 = GIVE_ME_TIME
#endif



            MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_PARAM_CHECK_SUM-(MAPI_U8)T_OPMODE_RFAGC_EN] = DTV_DVBT_DSPReg_CRC();


#if(TIMING_VERIFICATION == 1)
    tmm_8 = GIVE_ME_TIME
            printf("[2222]t8 - t7 = %ld (%ld - %ld) \n",tmm_8-tmm_7,tmm_8,tmm_7);
#endif


#if(TIMING_VERIFICATION == 1)
    tmm_7 = GIVE_ME_TIME
#endif


            if( WriteDspReg((MAPI_U8)T_PARAM_CHECK_SUM, MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_PARAM_CHECK_SUM-(MAPI_U8)T_OPMODE_RFAGC_EN])!=TRUE)
    {
        ERR_DEMOD_MSB(printf("dsp reg write crc NG\n"));
        return FALSE;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_8 = GIVE_ME_TIME
            printf("[33333]t8 - t7 = %ld (%ld - %ld) \n",tmm_8-tmm_7,tmm_8,tmm_7);
#endif


    printf("dvbt dsp reg init ok\n");

    return MAPI_TRUE;
}


//configure
//--------------------------------------------------------------------- for DVB-T2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL               0x04

// FC: FC = IF = 5000 = 0x1388     (5.00MHz IF)
#define T2_FC_L_VAL            0x88    // 5.00M
#define T2_FC_H_VAL            0x13
#define T2_TS_SERIAL_VAL        0x00
#define T2_TS_CLK_RATE_VAL      0x06
#define T2_TS_OUT_INV_VAL       0x00
#define T2_TS_DATA_SWAP_VAL     0x00
#define T2_TS_ERR_POL_VAL       0x00
#define T2_IF_AGC_INV_PWM_EN_VAL    0x00

#if 0
MAPI_U8 MSB123xC_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL,     T2_FC_L_VAL,     T2_FC_H_VAL
};
#endif


MAPI_BOOL DTV_DVBT2_DSPReg_Init(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
#if 0
    MAPI_U8    idx = 0;
    for (idx = 0; idx<sizeof( MSB123xC_DVBT2_DSPREG_TABLE); idx++)
    {
        if( WriteDspReg(idx + 0x40, MSB123xC_DVBT2_DSPREG_TABLE[idx])!=TRUE)
        {
            printf("T2 dsp reg init NG\n");
            return FALSE;
        }
    }
#endif
    if( WriteDspReg((MAPI_U8)E_T2_BW, T2_BW_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_FC_L, T2_FC_L_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_FC_H, T2_FC_H_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_TS_SERIAL, T2_TS_SERIAL_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_TS_CLK_RATE, T2_TS_CLK_RATE_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_TS_OUT_INV, T2_TS_OUT_INV_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_TS_DATA_SWAP, T2_TS_DATA_SWAP_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( WriteDspReg((MAPI_U8)E_T2_TS_ERR_POL, T2_TS_ERR_POL_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if(WriteDspReg((MAPI_U8)E_T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }

    printf("T2 dsp reg init ok\n");

    return MAPI_TRUE;
}
//--------------------------------------------------------------------- for DVB-C
#define SR0_H_AUTO              0x0F
#define SR0_L_AUTO              0xA0
#define SR0_H                   0x1A
#define SR0_L                   0xDB
/*
#define REAGC_EN                0x00 //0x20
#define HUMDET_EN_C             0x00
#define DCR_EN                  0x01
#define IQB_EN                  0x00
#define AUTO_IQ                 0x01
#define AUTO_RFMAX              0x00
#define AUTO_ACI                0x01
*/
#define AUTO_SCAN               0x00
#define AUTO_SCAN_SYM_RATE      0x00 //0x28
#define AUTO_SCAN_QAM           0x00
#define ATV_DET_EN              0x01 //0x2A
/*
#define RSV_0B                  0x00
#define RSV_0C                  0x00
#define RSV_0D                  0x00
#define RSV_0E                  0x00
#define RSV_0F                  0x00
*/
#define RSSI                    0x00 //0x30
//#define ZIF                     0x00
#define FREQ                    0x00
//#define FC_L                    0xE0
//#define FC_H                    0x2E
//#define FS_L                    0x80
//#define FS_H                    0xBB
#define BW_L                    0xDB
#define BW_H                    0x1A
#define BW1_L                   0xF4
#define BW1_H                   0x1A
#define BW2_L                   0xDB
#define BW2_H                   0x1A
#define BW3_L                   0xDB
#define BW3_H                   0x1A
//#define RSV_0x1F              0x00
//#define RFMAX                 0x00 //0x40
#define QAM                     0x02
#define IQ_SWAP_C               0x01
#define CCI                     0x00
#define SERIAL_TS               0x00 //0: parallel 1:serial
//#define TS_SERIAL_CLK           0x05
#define TS_OUT_INV              0x01 //
#define TS_DATA_SWAP            0x00

MAPI_U8 MSB123xC_DVBC_DSPREG_TABLE[] =
{
    00,     00,     01,     00,     01,     00,     01,       AUTO_SCAN,
    AUTO_SCAN_SYM_RATE, AUTO_SCAN_QAM,  ATV_DET_EN,     00,     00,     00,     00,     00,
    00,     00,     00,     FC_L,           FC_H,           FS_L,           FS_H,
    BW_L,       BW_H,       BW1_L,      BW1_H,      BW2_L,      BW2_H,      BW3_L,      BW3_H,      00,
    00,     QAM,        IQ_SWAP_C,      CCI,        SERIAL_TS,      05,
    TS_OUT_INV,     TS_DATA_SWAP
};

#if(EXTER_DEMOD_MSB123XC_ENABLE == 1)
MAPI_BOOL  DTV_DVBC_DSPReg_Init(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8    idx = 0;

    for (idx = 0; idx<((MAPI_U8)DVBC_PARAM_LEN - (MAPI_U8)C_opmode_rfagc_en - 1); idx++)
    {
        if( WriteDspReg(idx + (MAPI_U8)C_opmode_rfagc_en, MSB123xC_DVBC_DSPREG_TABLE[idx])!=TRUE)
        {
            ERR_DEMOD_MSB(printf("dsp reg init NG\n"));
            return FALSE;
        }
    }
    ERR_DEMOD_MSB(printf("DVBC dsp reg init ok\n"));

    return MAPI_TRUE;
}
#endif

/*@ <Operation ID=Im17018142m1221763cc7cmm464e> @*/

MAPI_BOOL Active(MAPI_BOOL bEnable)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    bEnable = bEnable;
    if (m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
        //FRA_T2_BRINGUP
#if 0
        return MAPI_TRUE;
#else
        printf(" @MSB123xC_active T2\n");

        MAPI_BOOL status = WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN

        if(status)
        {
            printf(" @MSB123xC_Active OK\n");
        }
        else
        {
            printf(" @MSB123xC_Active NG\n");
        }
        return status;
#endif
    }
    else if ((m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T)
             ||(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_C))
    {
        printf(" @MSB123xC_active T C !!\n");

        MAPI_BOOL status = WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN

        if(status)
        {
            printf(" @MSB123xC_Active DVBT OK\n");
        }
        else
        {
            printf(" @MSB123xC_Active DVBT NG\n");
        }
        return status;
    }
    return MAPI_FALSE;
}

/*@ </Operation ID=Im17018142m1221763cc7cmm464e> @*/
EN_DEVICE_DEMOD_TYPE GetCurrentDemodulatorType(void)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d,type is %d\n",__func__,__LINE__,(int)m_enCurrentDemodulator_Type));
    return m_enCurrentDemodulator_Type;
}
#if 0
//@@++ 20120405 Arki
MAPI_BOOL device_demodulator::GetDemodInfo(void)
{
    ExtendCmd(0xEE, 0, 0, &st2_mod_info);
    return TRUE;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_s1_siso_miso(void)
{
    return st2_mod_info.s1_siso_miso;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_number(void)
{
    return st2_mod_info.plp_number;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_id(void)
{
    return st2_mod_info.plp_id;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_constellation_l1(void)
{
    return st2_mod_info.constellation_l1;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_coderate_l1(void)
{
    return st2_mod_info.coderate_l1;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_constellation(void)
{
    return st2_mod_info.plp_constellation;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_rotation_constellation(void)
{
    return st2_mod_info.plp_rotation_constellation;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_coderate(void)
{
    return st2_mod_info.plp_coderate;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_fec(void)
{
    return st2_mod_info.plp_fec;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_frame_il_length(void)
{
    return st2_mod_info.plp_frame_il_length;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_type_time_il(void)
{
    return st2_mod_info.plp_type_time_il;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_plp_length_time_il(void)
{
    return st2_mod_info.plp_length_time_il;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_pilot_pattern(void)
{
    return st2_mod_info.pilot_pattern;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_mode_carrier(void)
{
    return st2_mod_info.mode_carrier;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_fef_type(void)
{
    return st2_mod_info.fef_type;
}

MAPI_U32 device_demodulator::GetDVBT2_Demod_fef_length(void)
{
    return st2_mod_info.fef_length;
}

MAPI_U32 device_demodulator::GetDVBT2_Demod_fef_interval(void)
{
    return st2_mod_info.fef_interval;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_papr(void)
{
    return st2_mod_info.papr;
}

MAPI_U8 device_demodulator::GetDVBT2_Demod_guard_interval(void)
{
    return st2_mod_info.guard_interval;
}
//@@-- 20120405 Arki
#endif
MAPI_BOOL ExtendCmd(MAPI_U8 SubCmd, MAPI_U32 u32Param1, MAPI_U32 u32Param2, void *pvParam3)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8 u8Data = 0;
    MAPI_U8 u8Status = MAPI_TRUE;


    SubCmd=SubCmd;
    u32Param1=u32Param1;
    u32Param2=u32Param2;
    pvParam3=pvParam3;

    if(SubCmd == 1)
    {
        //1 => write
        // printf("[dvbt2]write register\n");
        return WriteReg((MS_U16) u32Param1, (MS_U8) u32Param2);
    }

    if(SubCmd == 2)
    {
        //2 =>  Read
        // printf("[dvbt2]read register\n");
        return ReadReg((MS_U16)u32Param1, (MS_U8*)pvParam3);
    }


    if (SubCmd == 0x77)
    {
#if 0
        if (FECLock == TRUE)
            u8_g_pid0_timeout_flag = 1;
        else
            u8_g_pid0_timeout_flag = 0;
#endif
    }

    if (SubCmd == 0x80) // temp solution, to turn on/off IF AGC
    {
        if (u32Param1 == MAPI_TRUE)         // turn on
        {
            u8Status &= ReadReg(0x0a00+(0x18)*2, &u8Data);
            u8Data = u8Data|0x10;
            u8Status &= WriteReg(0x0a00+(0x18)*2, u8Data);
            printf("MSB123xC: Turn on IF AGC: %02x\n", u8Data);
        }
        else if (u32Param1 == MAPI_FALSE)    // turn off
        {
            u8Status &= ReadReg(0x0a00+(0x18)*2, &u8Data);
            u8Data = u8Data&(0xff-0x10);
            u8Status &= WriteReg(0x0a00+(0x18)*2, u8Data);
            printf("MSB123xC: Turn off IF AGC: %02x\n", u8Data);
        }
    }

    // DVB-T2 Tri-State setting
    // MAPI_FALSE(0) : Disable Tri-State, enable DVB-T2 output
    // MAPI_TRUE(1) : Enable Tri-State, disable DVB-T2 output
    if(SubCmd == 0x81)
    {
        if(u32Param1 == MAPI_TRUE)
        {
            WriteReg2bytes(0x0900+(0x2d)*2, 0x0000);

        }
        else
        {
            WriteReg2bytes(0x0900+(0x2d)*2, 0x00ff);
        }
    }

    if(SubCmd == 0x82)  // to turn on/off null packet insert
    {
        if(u32Param1 == 1)  // null packet on
        {
            //DVBT2
            u8Status &= ReadReg(0x2a00 + (0x20) * 2 , &u8Data);
            u8Status &= WriteReg(0x2a00 + (0x20) * 2 , (u8Data |0x80));
            //DVBT/C
            u8Status &= ReadReg(0x1100 + (0x20) * 2 , &u8Data);
            u8Status &= WriteReg(0x1100 + (0x20) * 2 , (u8Data |0x80));
        }
        else if(u32Param1 == 0)     // null packet off
        {
            //DVBT2
            u8Status &= ReadReg(0x2a00 + (0x20) * 2 , &u8Data);
            u8Status &= WriteReg(0x2a00 + (0x20) * 2 , (u8Data & (~0x80)));
            //DVBT/C
            u8Status &= ReadReg(0x1100 + (0x20) * 2 , &u8Data);
            u8Status &= WriteReg(0x1100 + (0x20) * 2 , (u8Data & (~0x80)));
        }
    }

    // get fef indiator
    if (SubCmd == 0x83)
    {
        MAPI_U8 u8Data = 0;
        u8Status &= ReadDspReg(E_T2_FEF_DET_IND, &u8Data);
        *((MAPI_U8*)pvParam3) = u8Data;

//        printf("[dvbt2]get fef indicator = %d\n",u8Data);
    }

    // get CFO(frequency offset) value
    // Unit: kHz
    if (SubCmd == 0x84)
    {
        float    fCFO = (float)0.0;
        // MAPI_U8  u8Data = 0;
        MAPI_U32 RegTotalCfo = 0;
        float    SigTotalCfo = (float)0.0;
        float    FB[6] =
        {
            1845.0704,  // 1.7M 131/71
            5714.2857,  // 5.0M 40/7
            6857.1429,  // 6.0M 48/7
            8000.0,     // 7.0M 8/1
            9142.8571, // 8.0M 64/7
            11428.5714  //10.0M 80/7
        };
#if 0
        u8Status &= ReadDspReg(E_T2_TOTAL_CFO_3, &u8Data);
        RegTotalCfo = (u8Data&0x07);
        u8Status &= ReadDspReg(E_T2_TOTAL_CFO_2, &u8Data);
        RegTotalCfo = (RegTotalCfo<<8)|u8Data;
        u8Status &= ReadDspReg(E_T2_TOTAL_CFO_1, &u8Data);
        RegTotalCfo = (RegTotalCfo<<8)|u8Data;
        u8Status &= ReadDspReg(E_T2_TOTAL_CFO_0, &u8Data);
        RegTotalCfo = (RegTotalCfo<<8)|u8Data;
#endif
        RegTotalCfo = 0;
        if (RegTotalCfo & 0x4000000)
            SigTotalCfo = (float)1.0*(0x8000000 - RegTotalCfo);
        else
            SigTotalCfo = (float)(-1.0)*RegTotalCfo;

        fCFO = (SigTotalCfo*FB[g_u8_bw])/(float)134217728.0;
//        printf("@@@@@@@@@@@@@@@@@@@@@@@@@>[msb123xc_extend.cpp]RegTotalCfo = 0x%lx\n", RegTotalCfo);
//        printf("@@@@@@@@@@@@@@@@@@@@@@@@@>[msb123xc_extend.cpp]SigTotalCfo = %f\n", SigTotalCfo);
//        printf("@@@@@@@@@@@@@@@@@@@@@@@@@>[msb123xc_extend.cpp]CFO = %f\n", fCFO);

        *((float*)pvParam3) = fCFO;
    }

    if (SubCmd == 0xEE)
    {
        MAPI_BOOL bRet = TRUE;
        // bRet &= MSB123xC_demod_info((t2_mod_info*)pvParam3);
        if (bRet == FALSE)
            printf("ERROR!!!!!!!!!!!!!!!\n");
    }

    if (SubCmd == 0xFF) // temp solution, only for internal use, can't be called by upper layer
    {
        MAPI_U32    u32Timeout;
        MAPI_U32    u32LockTimeout=2000;
        MAPI_U8     u8Data = 0;
        MAPI_U16    u16RegAddress;
        MAPI_U8     u8LockState;
        MAPI_U8     u8BitMask;
        MAPI_BOOL   bCheckPass=FALSE;
        static MAPI_U32 u32LockTimeStartDVBT = 0;
        static MAPI_U32 u32LockTimeStartDVBC = 0;//mick
        static MAPI_U32 u32LockTimeStartDVBT2 = 0;
        MAPI_U32 u32NowTime=MsOS_GetSystemTime();
        MAPI_U8     u8_fef_ind = 0;
        MAPI_U8 tmp1, tmp2, tmp3;
        //printf("EXTCMD:0xFF. \n");
        switch(m_enCurrentDemodulator_Type)
        {
            case E_DEVICE_DEMOD_DVB_T2:
            {
                DBG_DEMOD_MSB(printf("EXTCMD:0xFF.                   T2 T2 T2\n"));
                u32Timeout=8000;

                ReadDspReg(E_T2_DVBT2_LOCK_HIS, &u8Data);

                ReadDspReg(E_T2_FEF_DET_IND, &u8_fef_ind);
                if (u8_fef_ind == 1)
                    u32Timeout = 12000;

                DBG_DEMOD_MSB(printf("=>[Debug] E_T2_DVBT2_LOCK_HIS = %d \n", u8Data));
                ReadDspReg(E_T2_FW_VER_0, &tmp1);
                ReadDspReg(E_T2_FW_VER_1, &tmp2);
                ReadDspReg(E_T2_FW_VER_2, &tmp3);
                DBG_DEMOD_MSB(printf("=========================>[Debug] FW Ver = %x.%x.%x\n", tmp1, tmp2, tmp3));
                ReadReg(0x0990, &tmp1);
                DBG_DEMOD_MSB(printf("=========================>[Debug] State = %d \n", tmp1));

                if((u8Data&BIT7) != 0x00)
                    bCheckPass=MAPI_TRUE;
                else
                {
                    if (p1_ever_lock_flag == 0)
                    {
                        u16RegAddress = E_T2_DVBT2_LOCK_HIS; //P1 Lock History
                        u8BitMask = BIT5;
                        ReadDspReg(u16RegAddress, &u8Data);
                        if((u8Data&u8BitMask) == 0)
                        {
                            // Fix me please!
                            // Tmp solution: this timeout value needs to correct. (move to UpperLayer)
                            u32Timeout=600; //1500;//2000;
                        }
                        else
                        {
                            p1_ever_lock_flag = 1;
                        }
                    }
                    else
                    {
                        ;
                    }
                }

                DBG_DEMOD_MSB(printf(">>>MSB123xC: [%s][%d] Lock Status = %d\n", __FUNCTION__, __LINE__, u8Data));
                if (bCheckPass)
                {

                    u32LockTimeStartDVBT2=MsOS_GetSystemTime();

                    FECLock = MAPI_TRUE;
                    if(bDoReset == TRUE)
                    {
                        printf("[msb123xc]++++DVBT2 lock, freq=%ld\n",g_u32Frequency);
                        DBG_DEMOD_MSB(printf("\t\t\tDTV_DVB_T_GetLockStatus(LOCK/T2) TIME   %ld(=%ld-%ld)\n", u32LockTimeStartDVBT2-u32StartTime, u32LockTimeStartDVBT2, u32StartTime));//to measure time
#if (TIMING_VERIFICATION==1)
                        tmm_14 = GIVE_ME_TIME
                                 show_timer();
#endif
                    }
                    bDoReset = FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_LOCK;
                }
                else if ((u32NowTime - u32ChkScanTimeStart < u32Timeout)
                         ||(u32NowTime - u32LockTimeStartDVBT2 < u32LockTimeout))
                {
                    FECLock = MAPI_FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_CHECKING;
                }
                else
                {
                    FECLock = MAPI_FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_UNLOCK;
                }

                if ( (FECLock == MAPI_TRUE) && (u8_g_pid0_timeout_flag == 1))
                {
                    MS_U16 bch_eflag2_sum = 0;
                    MS_U8  reg = 0;
             ///       static MS_U32 t1  = 0;

                    ReadReg(0x2600+0x26*2 + 1,&reg);
                    bch_eflag2_sum = reg;
                    ReadReg(0x2600+0x26*2,&reg);
                    bch_eflag2_sum = (bch_eflag2_sum<<8)|reg;

         ///           if ( (bch_eflag2_sum == 0x00) && ((MsOS_GetSystemTime() - t1) > 1000))
                    if (bch_eflag2_sum == 0x00)
                    {
                        //djb rst
                        ReadReg(0x2900+0x00*2,&reg);
                        reg |= 0x01;
                        WriteReg(0x2900+0x00*2,reg);
                        usleep(1000);
                        ReadReg(0x2900+0x00*2,&reg);
                        reg &= (0xff-0x01);
                        WriteReg(0x2900+0x00*2,reg);
        ///                t1 = MsOS_GetSystemTime();
                        // DBG_DEMOD_MSB(printf("\n......[mj]djb rst......\n");)
                    }
                    u8_g_pid0_timeout_flag = 0;
                }
            }
            break;
            case E_DEVICE_DEMOD_DVB_T:
            {
                DBG_DEMOD_MSB(printf("EXTCMD:0xFF.                   T T T\n"));
                    u32Timeout=1500;
                u16RegAddress=0x11E0;
                u8LockState=0x0B;
                ReadReg(u16RegAddress, &u8Data);
                DBG_DEMOD_MSB(printf(">>>MSB123xC: [%s][%d] Lock Status = %d\n", __FUNCTION__, __LINE__, u8Data));
                if(u8Data==u8LockState)
                    bCheckPass=MAPI_TRUE;
                else if(u8Data == 0xBB)
                {
                    // dsp_table_crc_error
                    ERR_DEMOD_MSB(printf("@msb123xc Dsp table init param crc error !!!\n"));
                    DTV_DVBT_DSPReg_ReadBack();
                }
                else
                {
                    u16RegAddress =  0x0990; //TPS his Lock,
                    u8BitMask = BIT3;
                    ReadReg(u16RegAddress, &u8Data);
                    if((u8Data&u8BitMask) > 0)
                           u32Timeout=6000;
                }
                /*
                            if(FECLock) //check fec
                            {
                                u32Timeout=6000;
                                u16RegAddress=0x11E0;
                                u8LockState=0x0B;
                                ReadReg(u16RegAddress, &u8Data);
                                if(u8Data==u8LockState)
                                    bCheckPass=MAPI_TRUE;
                            }
                           else
                            {
                                u32Timeout=2000;
                                //u16RegAddress =  0x0f22; //TPS Lock,
                                //u8BitMask = BIT1;
                                u16RegAddress =  0x0990; //TPS his Lock,
                                u8BitMask = BIT3;
                                ReadReg(u16RegAddress, &u8Data);
                                if((u8Data&u8BitMask) > 0)
                                    bCheckPass=MAPI_TRUE;
                            }
                */
                //DBG_DEMOD_MSB(printf(">>>MSB123xC: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
                if (bCheckPass)
                {
                    u32LockTimeStartDVBT=MsOS_GetSystemTime();
                    FECLock = MAPI_TRUE;
                    if(bDoReset==TRUE)
                    {
                        printf("[msb123xc]++++DVBT lock, freq=%ld\n",g_u32Frequency);
                        DBG_DEMOD_MSB(printf("\t\t\tDTV_DVB_T_GetLockStatus(LOCK/T) TIME   %ld(=%ld-%ld)\n", u32LockTimeStartDVBT-u32StartTime, u32LockTimeStartDVBT, u32StartTime));//to measure time
#if (TIMING_VERIFICATION==1)
                        tmm_14 = GIVE_ME_TIME
                                 show_timer();
#endif
                    }
                    bDoReset = FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_LOCK;
                }
                else if ((u32NowTime - u32ChkScanTimeStart < u32Timeout)
                         ||(u32NowTime - u32LockTimeStartDVBT < u32LockTimeout))
                {
                    FECLock = MAPI_FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_CHECKING;
                }
                else
                {
                    FECLock = MAPI_FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_UNLOCK;
                }
            }
            break;
            case E_DEVICE_DEMOD_DVB_C://mick
            {
                u32Timeout=1500;
                u16RegAddress=0x11E0;
                u8LockState=0x0C;
                ReadReg(u16RegAddress, &u8Data);
                //printf("[msb123xc]+++++++++DVBC State---->%d\n",u8Data);//mick
                if(u8Data==u8LockState)
                    bCheckPass=MAPI_TRUE;
                else
                {
                    u16RegAddress =  0x0990; //TPS his Lock,
                    u8BitMask = BIT3;
                    ReadReg(u16RegAddress, &u8Data);
                    if((u8Data&u8BitMask) > 0)
                        u32Timeout=6000;
                }
                //DBG_DEMOD_MSB(printf(">>>MSB123xC: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
                if (bCheckPass)
                {
                    u32LockTimeStartDVBC=MsOS_GetSystemTime();
                    FECLock = MAPI_TRUE;
                    if(bDoReset==TRUE)
                    {
                        printf("[msb123xc]++++DVBC lock, freq=%ld\n",g_u32Frequency);
                        DBG_DEMOD_MSB(printf("\t\t\tDTV_DVB_C_GetLockStatus(LOCK/C) TIME   %ld(=%ld-%ld)\n", u32LockTimeStartDVBC-u32StartTime, u32LockTimeStartDVBC, u32StartTime));//to measure time
#if (TIMING_VERIFICATION==1)
                        tmm_14 = GIVE_ME_TIME
                                 show_timer();
#endif
                    }
                    bDoReset = FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_LOCK;
                }
                else if ((u32NowTime - u32ChkScanTimeStart < u32Timeout)
                         ||(u32NowTime - u32LockTimeStartDVBC < u32LockTimeout))
                {
                    FECLock = MAPI_FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_CHECKING;
                }
                else
                {
                    FECLock = MAPI_FALSE;
                    *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_UNLOCK;
                }
            }
            break;
            default:
                *((EN_LOCK_STATUS *)pvParam3) = E_DEMOD_UNLOCK;
        }
    }
    return MAPI_TRUE;
}


//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ######################################
//##########################################################################################################
//##########################################################################################################
// DVBT2 95~101ms, DVBT 38~39ms
MAPI_BOOL DTV_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth)//, MAPI_BOOL bPalBG, MAPI_BOOL bLPsel)
{
//    mapi_tuner *pTuner = NULL;
    MS_U32 u32DMD_IfFreq = 5000;
    MS_U8  u8_if_agc_mode = 0;

    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    //MAPI_BOOL b_pal_bg;
    //MAPI_BOOL bStatus = MAPI_FALSE;
    g_u32Frequency=u32Frequency;
    DBG_DEMOD_MSB(printf("DTV_SetFrequency %d ,%d \n", (int)u32Frequency, (int)eBandWidth));
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    FECLock = MAPI_FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();

#if(TIMING_VERIFICATION == 1)
    tmm_11 = GIVE_ME_TIME
#endif

    if (bPower_init_en != MAPI_TRUE)
    {
        ERR_DEMOD_MSB(printf("[msb123x][%s] Init not ready!\n", __FUNCTION__));
#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
        if (bPreloadFail == MAPI_TRUE)
        {
            ERR_DEMOD_MSB(printf("[msb123x][%s] Preload Failed!\n", __FUNCTION__));
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
        }
#endif
    }

    if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
        MAPI_U8 bw = E_DEMOD_BW_8M;
        switch (eBandWidth)
        {
            case E_RF_CH_BAND_6MHz:
                bw = E_DEMOD_BW_6M;
                break;
            case E_RF_CH_BAND_7MHz:
                bw = E_DEMOD_BW_7M;
                break;
            case E_RF_CH_BAND_8MHz:
                bw = E_DEMOD_BW_8M;
                break;
            default:
                bw = E_DEMOD_BW_8M;
                break;
        }
/*
        pTuner = mapi_interface::Get_mapi_pcb()->GetDvbtTuner(0);
        if (pTuner!=NULL)
        {
            if ( MAPI_FALSE == (pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_DTV_IF_FREQ, E_TUNER_DTV_DVB_T2_MODE, eBandWidth, &u32DMD_IfFreq)))
            {
                u32DMD_IfFreq = 5000;
            }
            if ( MAPI_FALSE == (pTuner->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_DEMOD_CONFIG, E_TUNER_DTV_DVB_T2_MODE, eBandWidth, (void*)&u8_if_agc_mode)))
            {
                u8_if_agc_mode = 0;
            }
        }
*/

        Reset();
        usleep(500);
        WriteDspReg((MAPI_U16)E_T2_BW, bw);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M

        WriteDspReg((MAPI_U16)E_T2_PLP_ID, g_msb123xc_plp_id);
        WriteDspReg((MAPI_U16)E_T2_FC_L, (MS_U8)u32DMD_IfFreq);
        WriteDspReg((MAPI_U16)E_T2_FC_H, (MS_U8)(u32DMD_IfFreq>>8));
        WriteDspReg((MAPI_U16)E_T2_IF_AGC_INV_PWM_EN, u8_if_agc_mode);

        WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
        g_u8_bw = bw;
        // DBG_DEMOD_MSB(printf("\n[msb123xc][dvbt2]DTV_SetFrequency, plp=0x%x\n",g_msb123xc_plp_id);)
        // printf("\n[msb123xc][dvbt2]DTV_SetFrequency, plp=0x%x,bw=%d\n",g_msb123xc_plp_id,bw);
    }
    else if (m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T)
    {
        Reset();
        usleep(500);

        WriteDspReg((MAPI_U8)T_CONFIG_BW, eBandWidth);      // BW: 1->6M, 2->7M, 3->8M

        MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_CONFIG_BW-(MAPI_U8)T_OPMODE_RFAGC_EN] = (MAPI_U8)eBandWidth;
        MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_PARAM_CHECK_SUM-(MAPI_U8)T_OPMODE_RFAGC_EN] = DTV_DVBT_DSPReg_CRC();

        WriteDspReg((MAPI_U8)T_PARAM_CHECK_SUM, MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_PARAM_CHECK_SUM-(MAPI_U8)T_OPMODE_RFAGC_EN]);      // BW: 1->6M, 2->7M, 3->8M

        WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN//Active(1);
    }

#if(TIMING_VERIFICATION == 1)
    tmm_12 = GIVE_ME_TIME
             show_timer();
#endif

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return MAPI_TRUE;
}

#if(EXTER_DEMOD_MSB123XC_ENABLE == 1)
MAPI_BOOL DTV_DVB_C_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth,MAPI_U32 u32SymRate, EN_CAB_CONSTEL_TYPE eQAM)
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);

    MAPI_U8             reg_qam = 0;
    MAPI_U8             reg_symrate_l = 0;
    MAPI_U8             reg_symrate_h = 0;

    u32Frequency = u32Frequency;
    eBandWidth = eBandWidth;

    DBG_DEMOD_MSB(printf("DTV_SetFrequency %d ,%d \n", (int)u32Frequency, (int)eBandWidth));
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);

    SetDvbcParam(eQAM);//QAM

    reg_symrate_l = (MAPI_U8) (u32SymRate & 0xff);
    reg_symrate_h = (MAPI_U8) (u32SymRate >> 8);
    reg_qam = gQamVal;

    //// Reset Demod ///////////////////
    Reset();
    usleep(500);
    //// DSP Register Overwrite ///////////////////
    if (u32SymRate == 0)//Auto SR QAM Mode
    {
        DTV_DVB_C_Set_Config_dvbc_auto(MAPI_TRUE);
    }
    else //Manual Mode
    {
        // Symbol Rate 4000~7000
        if( WriteDspReg(0x17 + 0x20, reg_symrate_l)!= TRUE)//Driver update 2009/11/20
        {
            DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config symbol L ng\n"));
            return MAPI_FALSE;
        }
        if( WriteDspReg(0x18 + 0x20, reg_symrate_h)!= TRUE)//Driver update 2009/11/20
        {
            DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config symbol H ng\n"));
            return MAPI_FALSE;
        }
        // QAM mode
        if( WriteDspReg(0x21 + 0x20, reg_qam)!= TRUE)//Driver update 2009/11/20
        {
            DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config QAM ng\n"));
            return MAPI_FALSE;
        }
    }
    DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config SymRate=[%d]\n",(int)u32SymRate));
    DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config QAM Mode=[%d]\n",reg_qam));
    DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config OK\n"));

    Active(1);

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return MAPI_TRUE;
}

MAPI_U16 DTV_DVB_C_GetCurrentSymbolRate(void)
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    return 0;
}
#endif
/****************************************************************************
Subject:    Function providing approx. result of Log10(X)
Function:   Log10Approx
Parmeter:   Operand X in float
Return:     Approx. value of Log10(X) in float
Remark:      Ouput range from 0.0, 0.3 to 9.6 (input 1 to 2^32)
*******:**********************************************************************/
static float Log10Approx(float flt_x)
{
    MAPI_U32 u32_temp = 1;
    MAPI_U8 indx = 0;
    do
    {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }
    while (++indx < 32);
    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 *indx;
}

EN_FRONTEND_SIGNAL_CONDITION DTV_GetSNR(void)
{
/*
    MAPI_U8   status = MAPI_TRUE;
    MAPI_U32  snr_out = 0;
    MAPI_U16  snr_ave_num = 0;
    MAPI_U8   reg = 0;
    float     fSNRDivider[]= {1024*2, 2048*2, 4096*2, 8192*2, 16384*2, 32768*2, 32768*2, 32768*2};
    float     snr_poly = 0.0; // for SNR polynomial eq.

    mapi_demodulator_datatype::EN_FRONTEND_SIGNAL_CONDITION eSignalCondition;
    // Coverity issue: 113565: UNINIT
    eSignalCondition = mapi_demodulator_datatype::E_FE_SIGNAL_NO;

    switch (device_demodulator::m_enCurrentDemodulator_Type)
    {
        case mapi_demodulator_datatype::E_DEVICE_DEMOD_DVB_T2:
        {
            if(FECLock == FALSE) g_msb123xc_fSNR = (float)0.0;
            else
            {
                float fsnr1 = 0.0;
                float fsnr2 = 0.0;
                fsnr2 = fsnr2;
                float snr_calib_slope = 0.0;
                float snr_calib_bias  = 0.0;

                status &= ReadReg(0x2800+(0x7f<<1), &reg);
                if ( (reg&0x03)==0x03 )
                {
                    status &= ReadReg(0x2800+(0x7d<<1)+1, &reg);
                    snr_out = (reg&0x1f);
                    status &= ReadReg(0x2800+(0x7d<<1)+0, &reg);
                    snr_out = (snr_out<<8)|reg;
                    status &= ReadReg(0x2800+(0x7d<<1)+3, &reg);
                    snr_out = (snr_out<<8)|reg;
                    status &= ReadReg(0x2800+(0x7d<<1)+2, &reg);
                    snr_out = (snr_out<<8)|reg;

                    status &= ReadReg(0x2800+(0x7f<<1), &reg);
                    status &= WriteReg(0x2800+(0x7f<<1), reg&(0xff-0x03));
                    // printf("\nreg1=0x%lx\n",snr_out);
                }
                else
                {
                    status &= ReadReg(0x2800+(0x0e<<1)+3, &reg);
                    snr_out = (reg&0x1f);
                    status &= ReadReg(0x2800+(0x0e<<1)+2, &reg);
                    snr_out = (snr_out<<8)|reg;
                    status &= ReadReg(0x2800+(0x0e<<1)+1, &reg);
                    snr_out = (snr_out<<8)|reg;
                    status &= ReadReg(0x2800+(0x0e<<1)+0, &reg);
                    snr_out = (snr_out<<8)|reg;
                    // printf("\nreg2=0x%lx\n",snr_out);
                }

                status &= ReadReg(0x2800+(0x0a<<1), &reg);
                snr_ave_num = reg&0x07;
                // printf("\nsnr_ave_num=%d\n",snr_ave_num);

                fsnr1 = 10.0*log10f((float)snr_out/fSNRDivider[snr_ave_num]);

                status &= ReadReg(0x2800+(0x07<<1), &reg);

                // HW cali en?
                if ((reg&0x10) == 0x10)
                {
                    // HW calibration.
                    // slope
                    ReadReg(0x2800+(0x08<<1), &reg);
                    snr_calib_slope = (float)reg/32.0;
                    // bias
                    ReadReg(0x2800+(0x08<<1)+1, &reg);
                    snr_calib_bias = (float)((MAPI_S8)((reg&0x1f)<<3))/32.0;
                    snr_calib_bias = pow(2.0,snr_calib_bias);
                    fsnr2 = snr_calib_slope*fsnr1+10.0*log10f(snr_calib_bias);
                }
                else
                    fsnr2 = fsnr1;

                g_msb123xc_fSNR = fsnr1;

                // printf("\n[dvbt2]slope=%.3f,bias=%.3f,fsnr1=%.2f,fsnr2=%.2f\n",snr_calib_slope,snr_calib_bias,fsnr1,fsnr2);
            }


            //use Polynomial curve fitting to fix snr
            //snr_poly = 0.0027945*pow(*fSNR,3) - 0.2266*pow(*fSNR,2) + 6.0101*(*fSNR) - 53.3621;
            g_msb123xc_fSNR = g_msb123xc_fSNR + snr_poly;

            if (g_msb123xc_fSNR>25)
                eSignalCondition = mapi_demodulator_datatype::E_FE_SIGNAL_STRONG;
            else if (g_msb123xc_fSNR>20)
                eSignalCondition = mapi_demodulator_datatype::E_FE_SIGNAL_MODERATE;
            else if (g_msb123xc_fSNR>15)
                eSignalCondition = mapi_demodulator_datatype::E_FE_SIGNAL_WEAK;
            else
                eSignalCondition = mapi_demodulator_datatype::E_FE_SIGNAL_NO;
        }
        break;
        default:
            g_msb123xc_fSNR = 0.0;
            eSignalCondition = mapi_demodulator_datatype::E_FE_SIGNAL_NO;
            break;
    }
    return eSignalCondition;
*/
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    return E_FE_SIGNAL_NO;
}

MAPI_BOOL DTV_GetPreBER(float *p_preBer)
{
    MAPI_U16 BitErrPeriod = 0;
    MAPI_U32 BitErr = 0;
    MAPI_U8  reg = 0;
    MAPI_U16 FecType = 0;
    MAPI_U8  status = MAPI_TRUE;
    float    fber = (float)0.0;

    if (FECLock== TRUE)
    {
        switch(m_enCurrentDemodulator_Type)
        {
            case E_DEVICE_DEMOD_DVB_T2:
            {
                /////////// Data BER /////////////
                // bank 38 0x02 [0] freeze
                // status &= WriteReg(0x2604, 0x01);  // avoid confliction

                // bank 38 0x12 Data BER Window[15:0]
                status &= ReadReg(0x2625, &reg);
                BitErrPeriod = reg;
                status &= ReadReg(0x2624, &reg);
                BitErrPeriod = (BitErrPeriod << 8) | reg;

                // bank 38 0x32 Data BER count[15:0]
                // bank 38 0x33 Data BER count[31:16]
                status &= ReadReg(0x2667, &reg);
                BitErr = reg;
                status &= ReadReg(0x2666, &reg);
                BitErr = (BitErr << 8) | reg;
                status &= ReadReg(0x2665, &reg);
                BitErr = (BitErr << 8) | reg;
                status &= ReadReg(0x2664, &reg);
                BitErr = (BitErr << 8) | reg;

                // bank 38 0x02 [0] freeze
                // status &= WriteReg(0x2604, 0x00);   // avoid confliction
                if (BitErrPeriod == 0)
                    //protect 0
                    BitErrPeriod = 1;

                status &= ReadReg(0x278f, &reg); //FEC Type[8:7]
                FecType = reg;
                status &= ReadReg(0x278e, &reg); //FEC Type[8:7]
                FecType = (FecType << 8) | reg;
                if (FecType&0x0180)
                    fber = (float)BitErr/(float)(BitErrPeriod*64800);
                else
                    fber = (float)BitErr/(float)(BitErrPeriod*16200);

                *p_preBer = fber;
                DBG_DEMOD_MSB(printf("MSB123xC Extend Data Pre BER = %8.3e \n ", fber));
            }
            break;
            default:
                *p_preBer = (float)1.0;
                status = MAPI_FALSE;
        }
    }
    else
    {
        *p_preBer = (float)1.0;
        status = MAPI_FALSE;
    }
    return status;
}


MAPI_BOOL DTV_GetPostBER(float *p_postBer)
{
    MAPI_U16 BitErrPeriod = 0;
    MAPI_U16 BitErr = 0;
    MAPI_U8  reg = 0;
    float    fber = 0;
    MAPI_U8  status = MAPI_TRUE;
    MAPI_U8  FecType = 0;

    if (FECLock == TRUE)
    {
        switch(m_enCurrentDemodulator_Type)
        {
            case E_DEVICE_DEMOD_DVB_T2:
            {
                /////////// Data BER /////////////
                // bank 38 0x02 [0] freeze
                status &= WriteReg(0x2604, 0x01);  // avoid confliction

                // bank 38 0x12 Data BER Window[15:0]
                status &= ReadReg(0x2625, &reg);
                BitErrPeriod = reg;
                status &= ReadReg(0x2624, &reg);
                BitErrPeriod = (BitErrPeriod << 8) | reg;

                // bank 38 0x32 Data BER count[15:0]
                // bank 38 0x33 Data BER count[31:16]
                status &= ReadReg(0x2600+(0x34<<1)+3, &reg);
                BitErr = reg;
                status &= ReadReg(0x2600+(0x34<<1)+2, &reg);
                BitErr = (BitErr << 8) | reg;
                status &= ReadReg(0x2600+(0x34<<1)+1, &reg);
                BitErr = (BitErr << 8) | reg;
                status &= ReadReg(0x2600+(0x34<<1)+0, &reg);
                BitErr = (BitErr << 8) | reg;

                // bank 38 0x02 [0] freeze
                status &= WriteReg(0x2604, 0x00);   // avoid confliction
                if (BitErrPeriod == 0)
                    //protect 0
                    BitErrPeriod = 1;

                status &= ReadReg(0x278f, &reg); //FEC Type[8:7]
                FecType = reg;
                status &= ReadReg(0x278e, &reg); //FEC Type[8:7]
                FecType = (FecType << 8) | reg;

                if (FecType&0x0180)
                {
                    if (BitErr == 0)
                        fber = (float)0.5/(float)(BitErrPeriod*64800);
                    else
                        fber = (float)BitErr/(float)(BitErrPeriod*64800);
                }
                else
                {
                    if (BitErr == 0)
                        fber = (float)0.5/(float)(BitErrPeriod*16200);
                    else
                        fber = (float)BitErr/(float)(BitErrPeriod*16200);
                }
                *p_postBer = fber;
                DBG_DEMOD_MSB(printf("MSB123xC Extend Data Post BER = %8.3e \n ", fber));
            }
            break;
            default:
                fber   = (float)1.0;
                status = MAPI_FALSE;

        }
    }
    else
    {
        fber = (float)1.0;
        status = MAPI_FALSE;
    }
    return status;
}

MAPI_U32 DTV_GetBER(void)
{

    MAPI_U8         status = MAPI_TRUE;
    MAPI_U8             reg = 0, reg_frz = 0;
    MAPI_U16            BitErrPeriod = 0;
    MAPI_U32            BitErr = 0;
    MAPI_U16            PktErr = 0;

    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    if (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T2)
    {
        MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
        return 0;
    }
    else if ((m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T)
             || (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_C))
    {
        /////////// Post-Viterbi BER /////////////

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= ReadReg(0x1132, &reg_frz);
        status &= WriteReg(0x1132, reg_frz|0x80);

        // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= ReadReg(0x1131, &reg);
        BitErrPeriod = reg;

        status &= ReadReg(0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8)|reg;

        // bank 7 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 7 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24

        status &= ReadReg(0x113D, &reg);
        BitErr = reg;

        status &= ReadReg(0x113C, &reg);
        BitErr = (BitErr << 8)|reg;

        status &= ReadReg(0x113B, &reg);
        BitErr = (BitErr << 8)|reg;

        status &= ReadReg(0x113A, &reg);
        BitErr = (BitErr << 8)|reg;

#if 1
        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //             [15:8] reg_uncrt_pkt_num_15_8
        status &= ReadReg(0x113F, &reg);
        PktErr = reg;

        status &= ReadReg(0x113E, &reg);
        PktErr = (PktErr << 8)|reg;
#endif

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= WriteReg(0x1132, reg_frz);

        if (BitErrPeriod == 0 ) //protect 0
        {
            BitErrPeriod = 1;
        }
        /*
            if (BitErr <=0 )
                *ber = 0.5 / ((float)BitErrPeriod*128*188*8);
            else
                *ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);


            printf("MSB PostVitBER = %8.3e \n ", *ber);
            printf("MSB PktErr = %d \n ", (int)PktErr);
        */

        if(status)
        {
            printf(" @MSB1231_GetPostViterbiBer OK\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return  MAPI_TRUE;
        }
        else
        {
            printf(" @MSB1231_GetPostViterbiBer NG\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
        }
    }
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return MAPI_FALSE;
}
#if(EXTER_DEMOD_MSB123XC_ENABLE == 1)
MAPI_BOOL DTV_GetPacketErr(MAPI_U16 *pu16BitErr)
{
    MAPI_BOOL status = MAPI_TRUE;
    MAPI_U8   reg = 0;
    MAPI_U16  PktErr=0;

    if (FECLock == TRUE)
    {
       //read packet error
       #if 0
        //for T2
        status &= ReadReg(0x2600+(0x2d<<1)+1, &reg);
        PktErr = reg;
        status &= ReadReg(0x2600+(0x2d<<1), &reg);
        PktErr = (PktErr << 8)|reg;
        *pu16BitErr = PktErr;
        printf("[dvbt2]GetPktErr = %d\n",PktErr);
       #else
        //for T
        status &= ReadReg(0x1100+(0x1f<<1)+1, &reg);
        PktErr = reg;
        status &= ReadReg(0x1100+(0x1f<<1), &reg);
        PktErr = (PktErr << 8)|reg;
        *pu16BitErr = PktErr;
        printf("[dvbt]GetPktErr = %d\n",PktErr);
       #endif
    }
    else
        *pu16BitErr = 65535;
    return status;
}
#endif
#if 1 //FIXME - 2009.11.08 by chunlun.lai
/****************************************************************************
Subject:    Function providing approx. result of Log10(X)
Function:   Log10Approx
Parmeter:   Operand X in float
Return:     Approx. value of Log10(X) in float
Remark:      Ouput range from 0.0, 0.3 to 9.6 (input 1 to 2^32)
 *******:**********************************************************************/
//      static float Log10Approx(float flt_x)
//      {
//          MAPI_U32 u32_temp = 1;
//          MAPI_U8 indx = 0;
//          do
//          {
//              u32_temp = u32_temp << 1;
//              if (flt_x < (float)u32_temp)
//                  break;
//          }
//          while (++indx < 32);
//          // 10*log10(X) ~= 0.3*N, when X ~= 2^N
//          return (float)0.3 *indx;
//      }
#endif
#if 0
/****************************************************************************
Subject:    To get the Post viterbi BER
Function:   MSB123xC_GetPostViterbiBer
Parmeter:   Quility
Return:     E_RESULT_SUCCESS
E_RESULT_FAILURE =>Read I2C fail, MSB123xC_VIT_STATUS_NG
Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
We will not read the Period, and have the "/256/8"
 *****************************************************************************/
static MAPI_BOOL GetPostViterbiBer(float *ber)
{
    MAPI_U8 status = true;
    MAPI_U8 reg, reg_frz;
    MAPI_U16 BitErrPeriod;
    MAPI_U32 BitErr;
    MAPI_U16 PktErr;
    /////////// Post-Viterbi BER /////////////
    // bank 7 0x32 [7] reg_bit_err_num_freeze
    status &= ReadReg(0x8732, &reg_frz);
    status &= WriteReg(0x8732, reg_frz | 0x80);
    // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
    //             [15:8] reg_bit_err_sblprd_15_8
    status &= ReadReg(0x8731, &reg);
    BitErrPeriod = reg;
    status &= ReadReg(0x8730, &reg);
    BitErrPeriod = (BitErrPeriod << 8) | reg;
    // bank 7 0x3a [7:0] reg_bit_err_num_7_0
    //             [15:8] reg_bit_err_num_15_8
    // bank 7 0x3c [7:0] reg_bit_err_num_23_16
    //             [15:8] reg_bit_err_num_31_24
    status &= ReadReg(0x873D, &reg);
    BitErr = reg;
    status &= ReadReg(0x873C, &reg);
    BitErr = (BitErr << 8) | reg;
    status &= ReadReg(0x873B, &reg);
    BitErr = (BitErr << 8) | reg;
    status &= ReadReg(0x873A, &reg);
    BitErr = (BitErr << 8) | reg;
    // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
    //             [15:8] reg_uncrt_pkt_num_15_8
    status &= ReadReg(0x873F, &reg);
    PktErr = reg;
    status &= ReadReg(0x873E, &reg);
    PktErr = (PktErr << 8) | reg;
    // bank 7 0x32 [7] reg_bit_err_num_freeze
    status &= WriteReg(0x8732, reg_frz);
    if (BitErrPeriod == 0)
        //protect 0
        BitErrPeriod = 1;
    if (BitErr <= 0)
        *ber = 0.5 / ((float)BitErrPeriod *128 * 188 * 8);
    else
        *ber = (float)BitErr / ((float)BitErrPeriod *128 * 188 * 8);
    DBG_DEMOD_MSB(printf("MSB123xC PostVitBER = %8.3e \n ",  *ber));
    DBG_DEMOD_MSB(printf("MSB123xC PktErr = %d \n ", (int)PktErr));
    return status;
}
#endif

MAPI_BOOL DTV_DVB_T_Get_TPS_Parameter(MAPI_U16 * pu16TPS_parameter, E_SIGNAL_TYPE eSignalType)
{
    MAPI_U8         u8Data = 0;

    if(DTV_DVB_T_GetLockStatus() == E_DEMOD_LOCK)
    {

        if(eSignalType == TS_MODUL_MODE)
        {

            if(ReadReg(0x2700 + (0x47 * 2), &u8Data) == FALSE)
                return FALSE;

            *pu16TPS_parameter  = (((MAPI_U16) u8Data) & (BIT5 | BIT4 | BIT3)) >> 3;

        }

        else  if(eSignalType == TS_CODE_RATE)
        {
            if(ReadReg(0x2700 + (0x47 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MAPI_U16) u8Data) & (BIT2 | BIT1 | BIT0));

        }

        else if(eSignalType == TS_GUARD_INTERVAL)
        {
            if(ReadReg(0x2400 + 0x40 * 2 + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter |= (((MAPI_U16) u8Data) & (BIT6 | BIT5 | BIT4)) >> 4 ;

        }

        else if(eSignalType == TS_FFT_VALUE)
        {
            if(ReadReg(0x2400 + 0x40 * 2 + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter |= ((MAPI_U16) u8Data) & (BIT2 | BIT1 | BIT0);
            /*DenizG|10.03.2011:CL_27888 Sometimes not valid FFT value is read*/
            if((*pu16TPS_parameter) == DEV_MSB123xC_FFT_32K_)
                *pu16TPS_parameter = DEV_MSB123xC_FFT_32K;

            if((*pu16TPS_parameter) == DEV_MSB123xC_FFT_8K_)
                *pu16TPS_parameter = DEV_MSB123xC_FFT_8K;
            /*DenizG|10.03.2011: ends*/
            /*DenizG|01.03.2011: CL_27888 is fixed*/
            if(ReadReg(0x2510, &u8Data)  == FALSE)
            {
                return FALSE;
            }

            *pu16TPS_parameter |= (((MAPI_U16) u8Data) & BIT3) ;
            return FALSE;
            /*DenizG|01.03.2011: ends*/
        }
        else
        {
            return FALSE;
        }

        return TRUE;

    }
    return FALSE;
}

EN_DVBT_CONSTEL_TYPE DTV_DVB_T_GetSignalModulation(void)
{
    MAPI_U16    u16Modulation = 0;

    if(DTV_DVB_T_Get_TPS_Parameter(&u16Modulation, TS_MODUL_MODE) == FALSE)
    {
        printf("TPS parameter can not be read.\n");
    }
    printf("MSB123xC Extend Data Constellation Type = %d \n ", u16Modulation);

    return (EN_DVBT_CONSTEL_TYPE)u16Modulation;
}


EN_DVBT_GUARD_INTERVAL DTV_DVB_T_GetSignalGuardInterval(void)
{
    MAPI_U16    u16GuardInterval = 0;

    if(DTV_DVB_T_Get_TPS_Parameter(&u16GuardInterval, TS_GUARD_INTERVAL) == FALSE)
    {
        printf("TPS parameter can not be read.\n");
    }
    printf("MSB123xC Extend Data Guard Interval = %d \n ", u16GuardInterval);
    return (EN_DVBT_GUARD_INTERVAL)u16GuardInterval;

}

EN_DVBT_FFT_VAL DTV_DVB_T_GetSignalFFTValue(void)
{
    MAPI_U16    u16FFTValue = 0;

    if(DTV_DVB_T_Get_TPS_Parameter(&u16FFTValue, TS_FFT_VALUE) == FALSE)
    {
        printf("TPS parameter can not be read.\n");
    }
    printf("MSB123xC Extend Data FFT Value = %d \n ", u16FFTValue);
    return (EN_DVBT_FFT_VAL)u16FFTValue;

}

EN_DVBT_CODE_RATE DTV_DVB_T_GetSignalCodeRate(void)
{
    MAPI_U16    u16CodeRate = 0;
    printf("~~ [DTV_DVB_T_GetSignalCodeRate]\n" );

    if(DTV_DVB_T_Get_TPS_Parameter(&u16CodeRate, TS_CODE_RATE) == FALSE)
    {
        printf("TPS parameter can not be read.\n");
    }
    printf("MSB123xC Extend Data Code Rate = %d \n ", u16CodeRate);
    return (EN_DVBT_CODE_RATE)u16CodeRate;
}


MAPI_U16 DTV_GetSignalQuality(void)
{
#if 0
    MAPI_U8     status = true;
    MAPI_U8     reg = 0, reg_frz = 0;
    MAPI_U16    BitErrPeriod = 0;
    MAPI_U32    BitErr = 0;
    MAPI_U16    FecType = 0;
    MAPI_U16    PktErr = 0;
    MAPI_U8     u8Data = 0;
    float       fber = 0;
    float       log_ber = 0;
    MAPI_U16 SQI = 0;

    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    switch(device_demodulator::m_enCurrentDemodulator_Type)
    {
        case mapi_demodulator_datatype::E_DEVICE_DEMOD_DVB_T2:
        {
            /////////// Check lock status /////////////
            ReadDspReg(E_T2_DVBT2_LOCK_HIS, &reg);
            if((reg & MBIT7) == 0x00)
            {
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                return 0;
            }

            /////////// Data BER /////////////
            // bank 38 0x02 [0] freeze
            status &= WriteReg(0x2604, 0x01);    // avoid confliction

            // bank 38 0x12 Data BER Window[15:0]
            status &= ReadReg(0x2625, &reg);
            BitErrPeriod = reg;
            status &= ReadReg(0x2624, &reg);
            BitErrPeriod = (BitErrPeriod << 8) | reg;

            // bank 38 0x32 Data BER count[15:0]
            // bank 38 0x33 Data BER count[31:16]
            status &= ReadReg(0x266b, &reg);
            BitErr = reg;
            status &= ReadReg(0x266a, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= ReadReg(0x2669, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= ReadReg(0x2668, &reg);
            BitErr = (BitErr << 8) | reg;

            // bank 38 0x02 [0] freeze
            status &= WriteReg(0x2604, 0x00);     // avoid confliction

            if(BitErrPeriod == 0)
                //protect 0
                BitErrPeriod = 1;

            //   &= ReadReg(0x268e, &reg);    //FEC Type[8:7]
            status &= ReadReg(0x278f, &reg);    //FEC Type[8:7]
            FecType = reg;
            status &= ReadReg(0x278e, &reg);    //FEC Type[8:7]
            FecType = (FecType << 8) | reg;
            // if (reg&0x080)
            if (FecType&0x0180)
            {
                fber = (float)BitErr / (BitErrPeriod * 64800);
            }
            else
            {
                fber = (float)BitErr / (BitErrPeriod * 16200);
            }
            //DBG_GET_SIGNAL(printf("MSB123xC Data BER = %8.3e \n ", fber));

            if(status == FALSE)
            {
                ERR_DEMOD_MSB(printf("DTV_GetSignalQuality_DVBT2_GetPostViterbiBer Fail!\n"));
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                return 0;
            }

            float fSNR=0.0, fBER_SQI=0.0, fCN_Quality=0.0;
            MAPI_U8 u8Modulation=0, u8CodeRate=0;

            DTV_GetSNR();

            fSNR = g_msb123xc_fSNR;
            // Avoid SQI value to become infinity
            if (fber <= 0.0f)
                fber = 1.0E-07f;
            else // decrese C/N by 0.5 dB to compute SQI
                fSNR = fSNR - 0.5f;

            if (fSNR<0.0f) fSNR=0.0f;

            ReadReg(0x2700 + (0x47 * 2), &u8Modulation);
            u8Modulation=(((MAPI_U16) u8Modulation) & (MBIT5 | MBIT4 | MBIT3)) >> 3;
            if (u8Modulation>=3) u8Modulation=3;
            ReadReg(0x2700 + (0x47 * 2), &u8CodeRate);
            u8CodeRate=(((MAPI_U16) u8CodeRate) & (MBIT2 | MBIT1 | MBIT0));
            if (u8CodeRate>=5) u8CodeRate=5;

            float fDVBT2_SQI_CNref[][6]=
            {
                {1.0, 2.3, 3.1, 4.1, 4.7, 5.2},
                {6.0, 7.6, 8.9, 10.0, 10.8, 11.4},
                {9.9, 12.0, 13.5, 15.1, 16.1, 16.8},
                {13.2, 16.1, 17.8, 20.0, 21.3, 22.0},
            };
            fBER_SQI=(log10f(1/fber))*20.0 - 40.0;
            if (fBER_SQI<=0.0) fBER_SQI=0.0;
            if (fBER_SQI>=100.0) fBER_SQI=100.0;
            fCN_Quality=log10f(fSNR/fDVBT2_SQI_CNref[u8Modulation][u8CodeRate])*10+0.3;
            if (fCN_Quality<0.0) fCN_Quality=0.0;
            if (fCN_Quality>1.0) fCN_Quality=1.0;
            SQI=fBER_SQI*fCN_Quality;

            DBG_GET_SIGNAL(printf(">>> MSB123xC SNR=%f\n",fSNR);)
            DBG_GET_SIGNAL(printf(">>> MSB123xC SQI=%d\n",SQI);)
            // printf("[msb123x]signalquality, coderate=%d, constel=%d,cn_rec=%f, cn_ref=%f, BER_SQI=%f, SQI=%f,log_ber=%f\n",coderate,constel,cn_rec,cn_ref,BER_SQI,SQI,log_ber);

            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return SQI;
            break;
        }

        case mapi_demodulator_datatype::E_DEVICE_DEMOD_DVB_T:
        case mapi_demodulator_datatype::E_DEVICE_DEMOD_DVB_C:
        default:
        {
            if(ReadReg(0x112C, &u8Data) == FALSE)
            {
                ERR_DEMOD_MSB(printf("DTV_DVBT_GetSignalQuality ReadReg fail!!! \n"));
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                return 0;
            }
            if(u8Data & 0x02 /*BIT1*/)
            {
                /////////// Post-Viterbi BER /////////////
                // bank 7 0x32 [7] reg_bit_err_num_freeze
                status &= ReadReg(0x1132, &reg_frz);
                status &= WriteReg(0x1132, reg_frz | 0x80);
                // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
                //             [15:8] reg_bit_err_sblprd_15_8
                status &= ReadReg(0x1131, &reg);
                BitErrPeriod = reg;
                status &= ReadReg(0x1130, &reg);
                BitErrPeriod = (BitErrPeriod << 8) | reg;
                // bank 7 0x3a [7:0] reg_bit_err_num_7_0
                //             [15:8] reg_bit_err_num_15_8
                // bank 7 0x3c [7:0] reg_bit_err_num_23_16
                //             [15:8] reg_bit_err_num_31_24
                status &= ReadReg(0x113D, &reg);
                BitErr = reg;
                status &= ReadReg(0x113C, &reg);
                BitErr = (BitErr << 8) | reg;
                status &= ReadReg(0x113B, &reg);
                BitErr = (BitErr << 8) | reg;
                status &= ReadReg(0x113A, &reg);
                BitErr = (BitErr << 8) | reg;
                // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
                //             [15:8] reg_uncrt_pkt_num_15_8
                status &= ReadReg(0x113F, &reg);
                PktErr = reg;
                status &= ReadReg(0x113E, &reg);
                PktErr = (PktErr << 8) | reg;
                // bank 7 0x32 [7] reg_bit_err_num_freeze
                status &= WriteReg(0x1132, reg_frz);
                if(BitErrPeriod == 0)
                    //protect 0
                    BitErrPeriod = 1;
                if(BitErr <= 0)
                    fber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
                else
                    fber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);
                DBG_GET_SIGNAL(printf("MSB123xC PostVitBER = %8.3e \n ", fber);)
                DBG_GET_SIGNAL(printf("MSB123xC PktErr = %d \n ", (int)PktErr);)

                if(status == FALSE)
                {
                    ERR_DEMOD_MSB(printf("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                    return 0;
                }

                log_ber = (- 1) * Log10Approx(1 / fber); // Log10Approx() provide 1~2^32 input range only
                //DBG_GET_SIGNAL(printf("Log(BER) = %f\n",log_ber));
                if(log_ber <= (- 7.0))
                    // PostVit BER < 1e-7
                {
                    //*quality = 100;
                    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                    return 100;
                }
                else if(log_ber < (- 3.7))
                    // PostVit BER < 2e-4
                {
                    //*quality = 60 + (((-3.7) - log_ber) / ((-3.7)-(-7.0)) * (100-60));
                    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                    return (60 + (((- 3.7) - log_ber) / ((- 3.7) - (- 7.0)) * (100 - 60)));
                }
                else if(log_ber < (- 2.7))
                    // PostVit BER < 2e-3
                {
                    //*quality = 10 + (((-2.7) - log_ber) / ((-2.7)-(-3.7)) * (60-10));
                    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                    return (10 + (((- 2.7) - log_ber) / ((- 2.7) - (- 3.7)) * (60 - 10)));
                }
                else
                {
                    //*quality = 10;
                    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                    return 10;
                }
            }
            else
            {
                //*quality = 0;
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                return 0;
            }
            // MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            // return 0;

            break;
        }
    }  // end of switch
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return 0;
#endif

    if(FECLock==FALSE)
    {
        return 0;
    }
    //FRA_T2_BRINGUP
    return 80;

}


S_RFAGC_SSI FT2125_RFAGC_SSI[] =
{
    {-19.40,    0x1B},
    {-20.40,    0x1C},
    {-21.40,    0x1D},
    {-22.40,    0x1E},
    {-23.40,    0x1F},
    {-24.40,    0x20},
    {-25.20,    0x21},
    {-26.60,    0x22},
    {-28.00,    0x23},
    {-29.20,    0x24},
    {-31.50,    0x25},
    {-32.75,    0x26},
    {-34.00,    0x27},
    {-35.25,    0x28},
    {-36.75,    0x29},
    {-38.50,    0x2a},
    {-40.00,    0x2b},
    {-42.00,    0x2c},
    {-43.50,    0x2d},
    {-44.80,    0x2e},
    {-46.20,    0x2f},
    {-47.40,    0x30},
    {-48.00,    0x31},
    {-48.40,    0x32},
    {-49.00,    0x33},
    {-99.00,    0x3f},
};

S_IFAGC_SSI FT2125_IFAGC_SSI[] =
{
    {-48,   0x3A},
    {-49,   0x3B},
    {-50,   0x3C},
    {-51,   0x3E},
    {-52,   0x40},
    {-53,   0x43},
    {-54,   0x45},
    {-55,   0x47},
    {-56,   0x49},
    {-57,   0x4C},
    {-58,   0x4f},
    {-59,   0x51},
    {-60,   0x54},
    {-61,   0x56},
    {-62,   0x59},
    {-63,   0x5C},
    {-64,   0x5F},
    {-65,   0x62},
    {-66,   0x65},
    {-67,   0x69},
    {-68,   0x6E},
    {-69,   0x73},
    {-70,   0x78},
    {-71,   0x7F},
    {-72,   0x85},
    {-73,   0x8b},
    {-74,   0x9c},
    {-85,   0xc0},
    {-99,   0xFF},
};

S_SSI_PREF SsiPref[] =
{
    {_QPSK_1,  _CR1Y2_1, -93.0},
    {_QPSK_1,  _CR2Y3_1, -91.0},
    {_QPSK_1,  _CR3Y4_1, -90.0},
    {_QPSK_1,  _CR5Y6_1, -89.0},
    {_QPSK_1,  _CR7Y8_1, -88.0},

    {_16QAM_1, _CR1Y2_1, -87.0},
    {_16QAM_1, _CR2Y3_1, -85.0},
    {_16QAM_1, _CR3Y4_1, -84.0},
    {_16QAM_1, _CR5Y6_1, -83.0},
    {_16QAM_1, _CR7Y8_1, -82.0},

    {_64QAM_1, _CR1Y2_1, -82.0},
    {_64QAM_1, _CR2Y3_1, -80.0},
    {_64QAM_1, _CR3Y4_1, -78.0},
    {_64QAM_1, _CR5Y6_1, -77.0},
    {_64QAM_1, _CR7Y8_1, -76.0},
};

MAPI_U16 DTV_GetSignalStrength(void)
{
    MAPI_U8    status = true;
    MAPI_U8    reg_tmp = 0;
    MAPI_U8    rf_agc_val = 0;
    MAPI_U8    if_agc_val = 0;
//    MAPI_U8    if_agc_err = 0;
    MAPI_U8    i = 0;
    MAPI_U8    ssi_tbl_len = 0;
//    MAPI_U8    err_tbl_len = 0;
    MAPI_U8    tps_cnstl = 0;
    MAPI_U8    tps_cr = 0;
    MAPI_U8    ssi = 0;
    MAPI_U8    u8RFAGC = 0;
    MAPI_U16   ypIFAGC1 = 0, ypIFAGC2 = 0;
    ypIFAGC2 = ypIFAGC2;
    float    ch_power_db = 0;
    float    ch_power_rf = 0;
    float    ch_power_if = 0;
    float    ch_power_ref = 0;
    float    ch_power_rel = 0;
    double fPrel=0.0, fPinput=0.0;
    MAPI_U8  demodState = 0;
    MAPI_U8  reg = 0;
    //MAPI_U8 tmpValue = 0;
    status &= ReadReg(0x0990, &reg);
    demodState = reg;

    EN_LOCK_STATUS eLockStatus = E_DEMOD_CHECKING;
    S_IFAGC_SSI     *ifagc_ssi;
//   TUNDRV_GetRSSI(&fPinput);
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    switch(m_enCurrentDemodulator_Type)
    {
        case E_DEVICE_DEMOD_DVB_T2:
        {
            u8RFAGC=150;//MDrv_SAR_Adc_GetValue(2);
            DBG_GET_SIGNAL(printf(">>> RFAGC = %d <<<\n", (int)u8RFAGC));
            if ( u8RFAGC < 89)
            {
                ch_power_rf = -45;
            }
            else if ( u8RFAGC < 111)
            {
                ch_power_rf = (-45) - (float)((u8RFAGC - 89)*5) /(111-89);
            }
            else if ( u8RFAGC < 149)
            {
                ch_power_rf = (-50) - (float)((u8RFAGC - 111)*5) /(148-111);
            }
            else
            {
                ch_power_rf = -55;
            }
            // IFAGC
            // debug select
            WriteReg(0x3e00+0x13*2, 0x03);
            WriteReg(0x3e00+0x02*2, 0x00);   //set freeze & dump [Write]
            WriteReg(0x3e00+0x02*2+1, 0x80);

            // agc gain2
            ReadReg(0x3e00+0x14*2+2, &reg_tmp);
            ypIFAGC2 = reg_tmp;
            ReadReg(0x3e00+0x14*2+1, &reg_tmp);
            ypIFAGC1 = reg_tmp;
            ReadReg(0x3e00+0x14*2+0, &reg_tmp);
            ypIFAGC1 = (ypIFAGC1 << 8) | reg_tmp;
            ReadReg(0x3e00+0x14*2+1, &reg_tmp);

            WriteReg(0x3e00+0x02*2, 0x00);   //set freeze & dump [Write]
            WriteReg(0x3e00+0x02*2+1, 0x00);
            DBG_GET_SIGNAL(printf(">>> reg_tmp = %d <<<\n", (int)reg_tmp));
            if_agc_val = reg_tmp;
            DBG_GET_SIGNAL(printf(">>> if_agc_val = %d <<<\n", (int)if_agc_val));
            if ( if_agc_val < 67)
            {
                ch_power_if = -55;
            }
            else if ( if_agc_val < 76)
            {
                ch_power_if = (-55) - (float)((if_agc_val - 67)*5) /(76-67);
            }
            else if ( if_agc_val < 88)
            {
                ch_power_if = (-60) - (float)((if_agc_val - 76)*5) /(88-76);
            }
            else if ( if_agc_val < 99)
            {
                ch_power_if = (-65) - (float)((if_agc_val - 88)*5) /(99-88);
            }
            else if ( if_agc_val < 113)
            {
                ch_power_if = (-70) - (float)((if_agc_val - 99)*5) /(113-99);
            }
            else if ( if_agc_val < 128)
            {
                ch_power_if = (-75) - (float)((if_agc_val - 113)*5) /(128-113);
            }
            else if ( if_agc_val < 145)
            {
                ch_power_if = (-80) - (float)((if_agc_val - 128)*5) /(145-128);
            }
            else if ( if_agc_val < 165)
            {
                ch_power_if = (-85) - (float)((if_agc_val - 145)*5) /(165-145);
            }
            else if ( if_agc_val < 183)
            {
                ch_power_if = (-90) - (float)((if_agc_val - 165)*5) /(183-165);
            }
            else
            {
                ch_power_if = -96;
            }

            if(ch_power_rf > (-55.0 + 0.5))
            {
                ch_power_db = ch_power_rf;
            }
            else if(ch_power_if < (-55.0 - 0.5))
            {
                ch_power_db = ch_power_if;
            }
            else
            {
                ch_power_db = (ch_power_if + ch_power_rf)/2;
            }

            DBG_GET_SIGNAL(printf("ch_power_rf = %f\n", ch_power_rf));
            DBG_GET_SIGNAL(printf("ch_power_if = %f\n", ch_power_if));
            DBG_GET_SIGNAL(printf("ch_power_db = %f\n", ch_power_db));
            // -50 dB ~ -95 dB
            // 100 ~ 0

            if(ch_power_db > -50)
            {
                ssi = 100;
            }
            else if(ch_power_db < -95)
            {
                ssi = 0;;
            }
            else
            {
                ssi = 100 - ((-50 - ch_power_db) * 2.22);
            }

            {
                float fDVBT2_SSI_Pref[][6]=
                {
                    {-95.7, -94.4, -93.6, -92.6, -92.0, -91.5},
                    {-90.8, -89.1, -87.9, -86.7, -85.8, -85.2},
                    {-86.9, -84.6, -83.2, -81.4, -80.3, -79.7},
                    {-83.5, -80.4, -78.6, -76.0, -74.4, -73.3},
                };
                MS_U8 u8Modulation=0, u8CodeRate=0;

                ReadReg(0x2700 + (0x47 * 2), &u8Modulation);
                u8Modulation=(((MAPI_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
                if (u8Modulation>=3) u8Modulation=3;
                ReadReg(0x2700 + (0x47 * 2), &u8CodeRate);
                u8CodeRate=(((MAPI_U16) u8CodeRate) & (BIT2 | BIT1 | BIT0));
                if (u8CodeRate>=5) u8CodeRate=5;

                //Fix me, temp solution; demod doesn't lock, then use fref = -98 to compute SSI
                if (demodState != 0x09)
                {
                    fPrel=fPinput-(-50.0f);
                }
                else
                {
                    fPrel=fPinput-fDVBT2_SSI_Pref[u8Modulation][u8CodeRate];
                }

                if (fPrel<-15.0)
                {
                    ssi = 0;
                }
                else if (fPrel<0.0)
                {
                    ssi = (MAPI_U8)(((double)2.0/3)*(fPrel+15.0));
                }
                else if (fPrel<20.0)
                {
                    ssi = 4*fPrel+10;
                }
                else if (fPrel<35.0)
                {
                    ssi = (MAPI_U8)(((double)2.0/3)*(fPrel-20.0)+90);
                }
                else
                {
                    ssi = 100;
                }
//                printf("=============================>SSI: u8Modulation= %d, u8CodeRate=%d\n, fPinput=%f, fDVBT2_SSI_Pref=%f\n, fPrel=%f, ssi=%d\n",u8Modulation, u8CodeRate, fPinput, fDVBT2_SSI_Pref[u8Modulation][u8CodeRate], fPrel, ssi);
            }

            DBG_GET_SIGNAL(printf(">>> SSI = %d <<<\n", (int)ssi));
        }
        break;

        case E_DEVICE_DEMOD_DVB_T:
        case E_DEVICE_DEMOD_DVB_C:
        {
            ///////// RF/IF-AGC Gain Out /////////////
            rf_agc_val = 0x3f;

            // ==== IFAGC_GAIN_OUT (GAIN2) ====

            // reg_agc_debug_sel[3:0]
            ReadReg(0x3e26, &reg_tmp);
            reg_tmp = (reg_tmp&0xf0)|0x03;    // select ifagc_gain
            WriteReg(0x3e26, reg_tmp);

            // reg_agc_gain2_out
            // use only high byte value
            ReadReg(0x3e29, &reg_tmp);
            if_agc_val = reg_tmp;

            DBG_GET_SIGNAL(printf("SSI_RFAGC (SAR) = 0x%x\n", (MAPI_U16)rf_agc_val));
            DBG_GET_SIGNAL(printf("SSI_IFAGC_H = 0x%x\n", (MAPI_U16) if_agc_val));

            for(i = 0; i < sizeof(FT2125_RFAGC_SSI)/sizeof(S_RFAGC_SSI); i++)
            {
                if (rf_agc_val <= FT2125_RFAGC_SSI[i].sar3_val)
                {
                    ch_power_rf = FT2125_RFAGC_SSI[i].power_db;
                    break;
                }
            }

            ifagc_ssi = FT2125_IFAGC_SSI;
            ifagc_ssi = ifagc_ssi;
            ssi_tbl_len = sizeof(FT2125_IFAGC_SSI)/sizeof(S_IFAGC_SSI);

            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (if_agc_val <= FT2125_IFAGC_SSI[i].agc_val)
                {
                    ch_power_if = FT2125_IFAGC_SSI[i].power_db;
                    break;
                }
            }

            DBG_GET_SIGNAL(printf("ch_power_rf = %f\n", ch_power_rf));
            DBG_GET_SIGNAL(printf("ch_power_if = %f\n", ch_power_if));

            ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

            ExtendCmd(0xFF, 0, 0, (void *)&eLockStatus);

            if( E_DEMOD_LOCK == eLockStatus )
            {
                ///////// Get Constellation and Code Rate to determine Ref. Power //////////
                ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

                status &= ReadReg(0x0f24, &reg_tmp);
                tps_cnstl = reg_tmp & 0x03;

                status &= ReadReg(0x0f24+1, &reg_tmp);
                tps_cr = (reg_tmp & 0x70) >> 4;

                for(i = 0; i < sizeof(SsiPref)/sizeof(S_SSI_PREF); i++)
                {
                    if ( (tps_cnstl == SsiPref[i].constel)
                         && (tps_cr == SsiPref[i].code_rate) )
                    {
                        ch_power_ref = SsiPref[i].p_ref;
                        break;
                    }
                }
            }
            else
            {
                ch_power_ref = SsiPref[8].p_ref;
            }
            //printf("tps_cnstl=%d, tps_cr = %d\n", tps_cnstl, tps_cr);

            ch_power_rel = ch_power_db - ch_power_ref;

            //printf("ch_power_db = %f, ch_power_ref=%f, ch_power_rel=%f\n", ch_power_db, ch_power_ref, ch_power_rel);

            if (ch_power_rel < -15.0)
                ssi = 0.0;
            else if (ch_power_rel < 0.0)
                ssi = (ch_power_rel + 15.0)*(10.0/15.0);
            else if (ch_power_rel < 20.0)
                ssi = ch_power_rel*4.0 + 10.0;
            else if (ch_power_rel < 35.0)
                ssi = (ch_power_rel - 20.0)*(10.0/15.0) + 90.0;
            else
                ssi = 100.0;

            DBG_GET_SIGNAL(printf(">>> SSI = %d <<<\n", (int)ssi));

        }
        break;

        default:
            ssi = 0;
            DBG_GET_SIGNAL(printf("Undefined!!!\n"));
            break;
    }
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return ssi;
}

MAPI_BOOL DTV_Serial_Control(MAPI_BOOL bEnable)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    if (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T2)
    {
        //FRA_T2_BRINGUP
    }
    else if ((m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T)
             || (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_C))
    {
        // set serial/parallel mode
        if( WriteDspReg((MAPI_U8)T_CONFIG_TS_SERIAL, (MAPI_U8)bEnable)!= TRUE)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Serial_Control seting NG\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
        }
        MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_CONFIG_TS_SERIAL-(MAPI_U8)T_OPMODE_RFAGC_EN] = (MAPI_U8)bEnable;
        // set clok inverse
        if( WriteDspReg((MAPI_U8)T_CONFIG_TS_OUT_INV, (MAPI_U8)bEnable)!= TRUE)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Serial_Control seting NG\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
        }
        MSB123xC_DVBT_DSPREG_TABLE[(MAPI_U8)T_CONFIG_TS_OUT_INV-(MAPI_U8)T_OPMODE_RFAGC_EN] = (MAPI_U8)bEnable;
    }
    Driving_Control(1);
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return MAPI_TRUE;
}

//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV-DVB-T Implementation ################################
//##########################################################################################################
//##########################################################################################################
EN_LOCK_STATUS DTV_DVB_T_GetLockStatus(void)
{
    EN_LOCK_STATUS eLockStatus = E_DEMOD_CHECKING;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    ExtendCmd(0xFF, 0, 0, (void *)&eLockStatus);
#if 0
    MAPI_U8 u8_reg = 0;
    float   fber = 0.0;
    MAPI_U16 err = 0;
    t2_mod_info t2_mod_info_a;
    if(eLockStatus == mapi_demodulator::E_DEMOD_LOCK)
    {
        MAPI_U16 err = 0;
        DTV_GetPacketErr(&err);
    }
    ReadReg(0x0900+(0x48)*2, &u8_reg);
    printf("[dvbt2]fsm=%d,",u8_reg);
    DTV_GetPreBER(&fber);
    printf("preber=%.3e, ",fber);
    DTV_GetPostBER(&fber);
    printf("postber=%.3e\n",fber);
    DTV_GetSNR(&fber);
    DTV_GetPacketErr(&err);
    MSB123xC_demod_info(&t2_mod_info_a);
#else
    //MAPI_U8 u8_reg = 0;
    //float   fber = 0.0;
    //MAPI_U16 err = 0;
    // t2_mod_info t2_mod_info_a;
    // ReadReg(0x1100+(0x70)*2, &u8_reg);
    // printf("[dvbt]fsm=%d \n,",u8_reg);
    // the following only porting T2
    //DTV_GetPreBER(&fber);
    //printf("preber=%.3e, ",fber);
    //DTV_GetPostBER(&fber);
    //printf("postber=%.3e\n",fber);
    //DTV_GetSNR(&fber);
    // DTV_GetPacketErr(&err);
    //MSB123xC_demod_info(&t2_mod_info_a);
#endif
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return eLockStatus;
}

#if(EXTER_DEMOD_MSB123XC_ENABLE == 1)
//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV-DVB-C Implementation ################################
//##########################################################################################################
//##########################################################################################################
EN_LOCK_STATUS DTV_DVB_C_GetLockStatus(void)
{
    //printf("%s(),%d\n",__FUNCTION__,__LINE__);
    EN_LOCK_STATUS eLockStatus = E_DEMOD_CHECKING;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    ExtendCmd(0xFF, 0, 0, (void *)&eLockStatus);
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return eLockStatus;
}

/************************************************************************************************
Subject:    channel change config
Function:   MSB123xC_Config
Parmeter:   BW: bandwidth
Return:     BOOLEAN :
Remark:
*************************************************************************************************/
//mick,For Parameter overwrite.
MAPI_U8 MSB123xC_Config_DVBC(MAPI_U16 SymbolRate, MAPI_U32 u32IFFreq, MAPI_U16 bSpecInv)//0723 update
{
    MAPI_U8             reg_qam;
    MAPI_U8             reg_symrate_l;
    MAPI_U8             reg_symrate_h;

    //DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config=[%d][%d][%d]\n",SymbolRate,u32IFFreq,bSpecInv));

    reg_symrate_l = (MAPI_U8) (SymbolRate & 0xff);
    reg_symrate_h = (MAPI_U8) (SymbolRate >> 8);
    reg_qam = gQamVal;

    //// Reset Demod ///////////////////
    Reset();

    //// DSP Register Overwrite ///////////////////

    // Symbol Rate 4000~7000
    if( WriteDspReg(0x17+ 0x20, reg_symrate_l)!= TRUE)//Driver update 2009/11/20
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config symbol L ng\n"));
        return MAPI_FALSE;
    }
    if( WriteDspReg(0x18+ 0x20, reg_symrate_h)!= TRUE)//Driver update 2009/11/20
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config symbol H ng\n"));
        return MAPI_FALSE;
    }

    DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config QAM Mode=[%d]\n",reg_qam));
    // QAM mode
    if( WriteDspReg(0x21+ 0x20, reg_qam)!= TRUE)//Driver update 2009/11/20
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config QAM ng\n"));
        return MAPI_FALSE;
    }

    DBG_DEMOD_MSB(printf(" @MSB123xC_dvbc_config OK\n"));
    return  MAPI_TRUE;
}
/************************************************************************************************
//Subject   :
//Function  :   MSB123xC_Set_Config_dvbc_auto (Auto SR+QAM or Manual SR+QAM)
//Parmeter  :
//Return    :
//Remark    :
*************************************************************************************************/
MAPI_U16 DTV_DVB_C_Set_Config_dvbc_auto (MAPI_U8 bAutoDetect)
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    MAPI_U8 status = MAPI_TRUE;

    if( WriteDspReg(0x08 + 0x20, bAutoDetect)!=MAPI_TRUE)
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_auto NG 1\n"));
    }
    if( WriteDspReg(0x09 + 0x20, bAutoDetect)!=MAPI_TRUE)
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_auto NG 2\n"));
    }

    if(bAutoDetect)
    {
        status &= WriteDspReg(0x37, SR0_L_AUTO);
        status &= WriteDspReg(0x38, SR0_H_AUTO);
    }
    else
    {
        status &= WriteDspReg(0x37, SR0_L);
        status &= WriteDspReg(0x38, SR0_H);
    }

    if(status)
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_auto OK\n"));
        return  MAPI_TRUE;
    }
    else
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_auto NG 3\n"));
        return MAPI_FALSE;
    }

}

/************************************************************************************************
//Subject   :
//Function  : MSB123xC_Set_Config_dvbc_atv_detector
//Parmeter  :
//Return    :
//Remark    : CCI detection enable and disable function.
*************************************************************************************************/
MAPI_U16 Set_Config_dvbc_atv_detector (MAPI_U8 bEnable)
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    if( WriteDspReg(0x0A+0x20, bEnable)!=0) //atv detector enable //Driver update 2010/09/28
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_atv_detector NG 1\n"));
        return MAPI_FALSE;
    }

    if(bEnable)
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_atv_detector ENABLE\n"));
    }
    else
    {
        DBG_DEMOD_MSB(printf(" @MSB123xC_Set_Config_dvbc_atv_detector DISABLE\n"));
    }
    return  MAPI_TRUE;
}

/************************************************************************************************
//Subject   : To get the DVBC parameter
//Function  : MSB123xC_GetDvbcInfo
//Parmeter  :
//Return    :
//Remark    :
*************************************************************************************************/

MAPI_U16 GetDvbcInfo( MAPI_U32 * DVBC_parameter)
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    MAPI_U8     u8Temp=0;
    MAPI_U16    dvb_c_info_temp=0;

    if (MSB123xC_Lock(E_DEVICE_DEMOD_DVB_C,COFDM_FEC_LOCK_DVBC) == FALSE)// FEC unlock
        return MAPI_FALSE;

    if (ReadReg(0x0C05, &u8Temp) == FALSE)
        return MAPI_FALSE;
    *DVBC_parameter = ((MAPI_U32)u8Temp) << 16;      //QAM (b23 ~ b16)

    DBG_DEMOD_MSB(printf("QAM Mode= %d\n",u8Temp));

    //symbole rate
    if (ReadReg(0x0C07, &u8Temp) == FALSE)
        return MAPI_FALSE;

    *DVBC_parameter |= ((MAPI_U32)u8Temp) << 8;         //Symbol Rate (b15 ~ b0) HIGH Byte
    DBG_DEMOD_MSB(printf("Symbol Rate H byte= %d\n",u8Temp));

    if (ReadReg(0x0C06, &u8Temp) == FALSE)
        return MAPI_FALSE;
    *DVBC_parameter |= (MAPI_U32)u8Temp;                    //Symbol Rate (b15 ~ b0) LOW Byte
    DBG_DEMOD_MSB(printf("Symbol Rate L byte= %d\n",u8Temp));

    dvb_c_info_temp = *DVBC_parameter&0xFFFF;
    DBG_DEMOD_MSB(printf("S7 DVBC symbol rate= %d\n",dvb_c_info_temp));
    dvb_c_info_temp = *DVBC_parameter/0x10000;

    switch(dvb_c_info_temp)
    {
        case 0:
            DBG_DEMOD_MSB(printf("S7 DVBC qam mode= 16 QAM\n"));
            break;

        case 1:
            DBG_DEMOD_MSB(printf("S7 DVBC qam mode= 32 QAM\n"));
            break;

        case 2:
            DBG_DEMOD_MSB(printf("S7 DVBC qam mode= 64 QAM\n"));
            break;

        case 3:
            DBG_DEMOD_MSB(printf("S7 DVBC qam mode= 128 QAM\n"));
            break;

        case 4:
            DBG_DEMOD_MSB(printf("S7 DVBC qam mode= 256 QAM\n"));
            break;

        default:
            DBG_DEMOD_MSB(printf("S7 DVBC qam mode= No information\n"));
            break;
    }

    return MAPI_TRUE;
}

/************************************************************************************************
//Subject   :
//Function  :   MSB123xC_SetDvbcParam
//Parmeter  :
//Return        :
//Remark    :
*************************************************************************************************/
MAPI_U16 SetDvbcParam (MAPI_U8 constel)
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    gQamVal = constel;
    DBG_DEMOD_MSB(printf(" @@ MSB123xC_SetDvbcParam : %d\n",gQamVal));
    return MAPI_TRUE;
}

/************************************************************************************************
//Subject   :
//Function  :   MSB123xC_CCI_Check
//Parmeter  :
//Return        :
//Remark    :
*************************************************************************************************/
MAPI_U16 CCI_Check( COFDM_CHECK_FLAG eFlag )
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    MAPI_U16    u16Address = 0;
    MAPI_U8     cData=0;
    MAPI_U8     cBitMask = 0;
    MAPI_U8     cBitVal = 0;

    switch( eFlag )
    {
        case COFDM_CCI_FLAG:
            u16Address =  0x0C09;//0x20C4; //0917 CCI detect
            cBitMask = BIT1;
            cBitVal = BIT1;
            break;

        case CHECK_FLAG_MAX_NUM:
            // to remove compile warning.

        default:
            break;
    }

    if (ReadReg(u16Address, &cData) == FALSE)
        return FALSE;

    if ((cData & cBitMask) == cBitVal)
    {
        MAPI_U16    time_check; //fisher
        MAPI_U8     data1=0, data2=0;

        //MSB123xC_ReadReg(0x20DB, &data1);
        //MSB123xC_ReadReg(0x20DA, &data2);
        time_check = data1 << 8;
        time_check |= data2;
        DBG_DEMOD_MSB(printf("DVB-C CCI detect time :[%d]ms\n",time_check));
        return MAPI_TRUE;
    }
    else
    {
        DBG_DEMOD_MSB(printf("DVB-C NO CCI \n"));
        return MAPI_FALSE;
    }
}
#endif

//##########################################################################################################
//##########################################################################################################
//########################################  Private Function Implementation ################################
//##########################################################################################################
//##########################################################################################################

MAPI_BOOL I2C_CH_Reset(MAPI_U8 ch_num)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    //MAPI_U8         addr[4] = {0x00, 0x00, 0x00, 0x00};
    MAPI_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]I2C_CH_Reset, CH=0x%x\n",ch_num));
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
    data[0] = 0x53;
//    iptr->WriteBytes(0, NULL, 5, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
//    iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);
    
    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
//    iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
//    iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
//    iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
//    iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
//    iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

    /*
        // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
        data[0] = 0x35;
        iptr->WriteBytes(0, NULL, 1, data);

        // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
        data[0] = 0x71;
        iptr->WriteBytes(0, NULL, 1, data);
    */
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]I2C_CH_Reset, CH=0x%x\n",ch_num));
    return MAPI_TRUE;
}

/*@ <Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
MAPI_BOOL WriteReg(MAPI_U16 u16Addr, MAPI_U8 u8Data)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    //bRet &= iptr->SetSpeed(0);


    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 6, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
//  bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    if(bRet == FALSE)
        printf("demod write i2c error \n");
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    return bRet;

}

MAPI_BOOL WriteRegs(MAPI_U16 u16Addr, MAPI_U8* u8pData, MAPI_U16 data_size)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U8   u8MsbDataValue[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MAPI_U16   idx = 0;

    bRet &= MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    // bRet &= iptr->SetSpeed(0);

    u8MsbDataValue[0] = 0x10;
    u8MsbDataValue[1] = 0x00;
    u8MsbDataValue[2] = 0x00;
    u8MsbDataValue[3] = (u16Addr >> 8) &0xff;
    u8MsbDataValue[4] = u16Addr &0xff;
    // u8MsbDataValue[5] = 0x00;

    for(idx = 0; idx < data_size ; idx++)
    {
        u8MsbDataValue[5+idx] = u8pData[idx];
    }

    u8MsbDataValue[0] = 0x35;
//    bRet &= iptr->WriteBytes(0, NULL, 1, u8MsbDataValue);
    bRet &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbDataValue);

    u8MsbDataValue[0] = 0x10;
//    bRet &= iptr->WriteBytes(0, NULL, 5 + data_size, u8MsbDataValue);
    bRet &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5 + data_size, u8MsbDataValue);

    u8MsbDataValue[0] = 0x34;
//    bRet &= iptr->WriteBytes(0, NULL, 1, u8MsbDataValue);
    bRet &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbDataValue);

    bRet &= MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    if(!bRet)
        printf("write ReadReg error \n");
    return bRet;
}

MAPI_BOOL WriteReg2bytes(MAPI_U16 u16Addr, MAPI_U16 u16Data)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_BOOL ret = true;
    ret &= WriteReg(u16Addr, (MAPI_U8)u16Data&0x00ff);
    ret &= WriteReg(u16Addr + 0x0001, (MAPI_U8)(u16Data>>8)&0x00ff);
    return ret;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6fcf> @*/
MAPI_BOOL ReadReg(MAPI_U16 u16Addr, MAPI_U8 *pu8Data)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
//    iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
//    iptr->WriteBytes(0, NULL, 5, u8MsbData);
    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//    iptr->ReadBytes(0, NULL, 1, pu8Data);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
//    bRet=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    bRet = MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
    if(!bRet)
        printf("demod ReadReg error \n");
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    return bRet;

}

MAPI_BOOL MDrv_SS_RIU_Write8(MAPI_U16 u16Addr, MAPI_U8 data)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MS_U8 Wdata[4];

    Wdata[0] = RIU_W1_CMD;
    Wdata[1] = u16Addr & 0xFF;
    Wdata[2] = (u16Addr >> 8) & 0xFF;
    Wdata[3] = data;
    printf("MDrv_SS_RIU_Write\n");

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);

    // send write address & data
    MDrv_MSPI_Write(Wdata,4);

    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}
MAPI_BOOL MDrv_SS_RIU_Read8(MAPI_U16 u16Addr, MAPI_U8 *pdata)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U8 Rdata[5];

    Rdata[0] = RIU_R1T_CMD;
    Rdata[1] = u16Addr & 0xFF;
    Rdata[2] = (u16Addr >> 8) & 0xFF;
    Rdata[3] = 0x00;
    printf("MDrv_SS_RIU_Read8\n");

    MDrv_MSPI_SlaveEnable(TRUE);
    // send read command to read data
    MDrv_MSPI_Write(Rdata,4);
    // read operation
    printf("Read Operation\n");
    MDrv_MSPI_Read(pdata, 1);
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;

}

MAPI_BOOL MDrv_SS_MIU_Writes(MAPI_U32 u32Addr, MAPI_U8 *pdata, MAPI_U16 u16Size)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MS_U8 Wdata[5];

    Wdata[0] = MIU_W_CMD;
    Wdata[1] = u32Addr & 0xFF;
    Wdata[2] = (u32Addr >> 8) & 0xFF;
    Wdata[3] = (u32Addr >> 16)& 0xFF;
    Wdata[4] = (u32Addr >> 24);
    //printf("MDrv_SS_MIU_Writes\n");

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    // send data
    MDrv_MSPI_Write(pdata,u16Size);
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;

}

MAPI_BOOL MDrv_SS_MIU_Reads(MAPI_U32 u32Addr, MAPI_U8 *pdata, MAPI_U16 u16Size)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL bRet = true;
    MAPI_U8 Rdata[SPI_DEVICE_BUFFER_SIZE];
    MAPI_U16 dataLen, i, j=0;

    do
    {
        dataLen = (u16Size>16?16:u16Size);//(len>24?24:len);

        Rdata[0] = MIU_R_CMD;
        Rdata[1] = u32Addr & 0xFF;
        Rdata[2] = (u32Addr >> 8) & 0xFF;
        Rdata[3] = (u32Addr >> 16)& 0xFF;
        Rdata[4] = (u32Addr >> 24);
        Rdata[5] = dataLen+1;
        //printf("MDrv_SS_MIU_Reads, addr=0x%lx, dataLen=%d\n", u32Addr, dataLen);

        // send read command to read data
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,6);
        MDrv_MSPI_SlaveEnable(FALSE);

        // read operation
        Rdata[0] = MIU_ST_CMD;
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,1);
        //printf("Read Operation\n");
        MDrv_MSPI_Read(Rdata, dataLen+1);
        MDrv_MSPI_SlaveEnable(FALSE);

        if(Rdata[0] != 0x0A)
        {
            ERR_DEMOD_MSB(printf("MDrv_SS_MIU_Reads fail, status=0x%x\n", Rdata[0] ));
            return false;
        }

        for (i=1; i<dataLen+1; i++, j++)
        {
            pdata[j] = Rdata[i];
            //printf("%x, ", pdata[j]);
        }

        u16Size -= dataLen;
        u32Addr += dataLen;
        //printf("u16Size=%d,  u32Addr=0x%lx\n", u16Size, u32Addr);
    }while(u16Size);

    return bRet;
}

#define REG_MB_CNTL     0x0C00
#define REG_MB_ADDR_L   0x0C02
#define REG_MB_ADDR_H   0x0C03
#define REG_MB_DATA     0x0C04

MAPI_BOOL  WriteDspReg(MAPI_U16 u16Addr, MAPI_U8 u8Data)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8     status = true;
    MAPI_U8     cntl = 0x00;
    MAPI_U16    cntr = 0x00;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW2_Mutex, MSOS_WAIT_FOREVER);
    status &= WriteReg(REG_MB_DATA, u8Data);
    status &= WriteReg(REG_MB_ADDR_H, (MAPI_U8)(u16Addr >> 8));
    status &= WriteReg(REG_MB_ADDR_L, (MAPI_U8)(u16Addr));
    status &= WriteReg(REG_MB_CNTL, 0x04);
    do
    {
        status &= ReadReg(REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            ERR_DEMOD_MSB(printf("MSB123xC_MB_WRITE_FAILURE\n"));
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW2_Mutex);
            return false;
        }
    }
    while(cntl != 0xff);
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW2_Mutex);
    return status;
}

MAPI_BOOL ReadDspReg(MAPI_U16 u16Addr, MAPI_U8* pData)
{
    //DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8     status = true;
    MAPI_U8     cntl = 0x00;
    MAPI_U16    cntr = 0x00;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW2_Mutex, MSOS_WAIT_FOREVER);

    status &= WriteReg(REG_MB_ADDR_H, (MAPI_U8)(u16Addr >> 8));
    status &= WriteReg(REG_MB_ADDR_L, (MAPI_U8)(u16Addr));
    status &= WriteReg(REG_MB_CNTL, 0x03);

    do
    {
        status &= ReadReg(REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            ERR_DEMOD_MSB(printf("MSB123xC_MB_READ_FAILURE\n"));
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW2_Mutex);
            return false;
        }
    }
    while(cntl != 0xff);

    status &= ReadReg(REG_MB_DATA, pData);
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW2_Mutex);
    return status;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm6fcf> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6faa> @*/
MAPI_BOOL Cmd_Packet_Send(S_CMDPKTREG *pCmdPacket, MAPI_U8 param_cnt)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    MAPI_U8 status = true, indx;
    //MAPI_U8             reg_val, timeout = 0;
    MAPI_U16 timeout = 0;
    MAPI_U8 reg_val = 0;
    // ==== Command Phase ===================
    DBG_DEMOD_MSB(printf(">>>MSB123xC: (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n", pCmdPacket->cmd_code, pCmdPacket->param[0], pCmdPacket->param[1], pCmdPacket->param[2], pCmdPacket->param[3], pCmdPacket->param[4], pCmdPacket->param[5]));
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    // wait _BIT_END clear
    do
    {
        status &= ReadReg(REG_CMD_CTRL, &reg_val);
        if((reg_val & _BIT_END) != _BIT_END)
            break;
        //MDrv_Timer_Delayms(5);
        usleep(5 *1000);
        if (timeout++ > 200)
        {
            DBG_DEMOD_MSB(printf(">>>MSB123xC: MSB123xC_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n"));
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return false;
        }
    }
    while(1);

    // set cmd_3:0 and _BIT_START
    status &= ReadReg(REG_CMD_CTRL, &reg_val);
    reg_val = (reg_val &0x0f) | (pCmdPacket->cmd_code << 4) | _BIT_START;
    status &= WriteReg(REG_CMD_CTRL, reg_val);

    // wait _BIT_START clear
    do
    {
        status &= ReadReg(REG_CMD_CTRL, &reg_val);
        if((reg_val & _BIT_START) != _BIT_START)
            break;
        usleep(5 *1000);
        if (timeout++ > 200)
        {
            DBG_DEMOD_MSB(printf(">>>MSB123xC: MSB123xC_Cmd_Packet_Send fail on 'wait _BIT_START clear' \n"));
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return false;
        }
    }
    while(1);

    // ==== Data Phase ======================
    status &= WriteReg(REG_CMD_ADDR, 0x00);
    for (indx = 0; indx < param_cnt; indx++)
    {
        status &= ReadReg(REG_CMD_ADDR, &reg_val);
        DBG_DEMOD_MSB(printf("demod_config: param[%d] = %x\n", reg_val, pCmdPacket->param[indx]));
        // set param[indx] and _BIT_DRQ
        status &= WriteReg(REG_CMD_DATA, pCmdPacket->param[indx]);
        status &= ReadReg(REG_DTA_CTRL, &reg_val);
        status &= WriteReg(REG_DTA_CTRL, reg_val | _BIT_DRQ);

        // wait _BIT_DRQ clear
        do
        {
            status &= ReadReg(REG_DTA_CTRL, &reg_val);
            if ((reg_val & _BIT_DRQ) != _BIT_DRQ)
                break;
            usleep(5 *1000);
            if (timeout++ > 200)
            {
                DBG_DEMOD_MSB(printf(">>>MSB123xC: MSB123xC_Cmd_Packet_Send fail on 'wait _BIT_DRQ clear' \n"));
                MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
                return false;
            }
        }
        while(1);
    }

    // ==== End Phase =======================
    // set _BIT_END to finish command
    status &= ReadReg(REG_CMD_CTRL, &reg_val);
    status &= WriteReg(REG_CMD_CTRL, reg_val | _BIT_END);
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return status;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm6faa> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6f85> @*/
#if 0
MAPI_BOOL device_demodulator::Cmd_Packet_Exe_Check(MAPI_BOOL *cmd_done)
{
    MAPI_U8 status = true;
    MAPI_U8 reg_val;
    MAPI_U16 timeout = 0;
    // wait _BIT_END clear
    do
    {
        status &= ReadReg(REG_CMD_CTRL, &reg_val);
        if ((reg_val &_BIT_END) != _BIT_END)
            break;
        if (timeout++ > 200)
        {
            *cmd_done = false;
            return status;
        }
        //MDrv_Timer_Delayms(10);
        usleep(10 *1000);
    }
    while (1);
    DBG_DEMOD_MSB(printf(">>>MSB123xC:Exe duration: %d\n", (int)timeout));
    *cmd_done = true;
    return status;
}
/*@ </Operation ID=I2b28dd03m121c8cf959bmm6f85> @*/
#endif

/*@ <Operation ID=I2b28dd03m121c8cf959bmm6f60> @*/
MAPI_BOOL LoadDSPCode(void)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    ////MAPI_U32 u32Now = MsOS_GetSystemTime();
    //printf("\t\t\tLoadDSPCode TIME   %ld (=%ld-%ld)\n", u32Now-u32StartTime, u32Now, u32StartTime);
    ////DBG_DEMOD_MSB(printf("\t\t\tLoadDSPCode TIME   %ld (=%ld-%ld)\n", u32Now-u32StartTime, u32Now, u32StartTime));//to measure time
    //u32StartTime = u32Now;
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCode\n"));
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);

    MAPI_U32        u32Timeout = 0;
    MAPI_U8         u8DoneFlag = 0;
    MAPI_U8         u8Data = 0;
    MAPI_BOOL       bRet = true;


    DBG_DEMOD_MSB(printf(">>>MSB123xC: Load DSP...\n"));

    switch(m_enCurrentDemodulator_Type)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            u8Data=1;
            break;
        case E_DEVICE_DEMOD_DVB_T:
            u8Data=2;
            break;
        case E_DEVICE_DEMOD_DVB_C:
            u8Data=3;
            break;
        default:
            u8Data=2;
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
    }
    printf("this is load LoadDSPCode \n");
    if(g_WO_SPI_FLASH == 1)
    {
        MAPI_U8 u8FirmwareType = MSB123xC_DVBT;

        // mask miu access for all and mcu
        bRet &= WriteReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= WriteReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        usleep(10);

        // Disable MCU
        bRet &= WriteReg(0x0b00+(0x19)*2, 0x03);

        switch (u8Data)
        {
            case 1:
                u8FirmwareType = MSB123xC_DVBT2;
                break;
            case 2:
            default:
                u8FirmwareType = MSB123xC_DVBT;
                break;
            case 3:
                u8FirmwareType = MSB123xC_DVBC;
                break;
        }

        bRet &= LoadDspCodeToSDRAM(u8FirmwareType);

        bRet &= LoadSdram2Sram(u8FirmwareType);

        // enable miu access of mcu gdma
        bRet &= WriteReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        bRet &= WriteReg(0x0b00+(0x19)*2, 0x00);

    }
    else
    {
#if(TIMING_VERIFICATION == 1)
        tmm_9 = GIVE_ME_TIME
#endif
                // mask miu access for all and mcu
                printf("this is 2 \n");
        bRet &= WriteReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= WriteReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        usleep(10);

        // Disable MCU
        WriteReg(0x0b00+(0x19)*2, 0x03);

        // Run code on bootloader
        WriteReg(0x1000+(0x0c)*2, 0x02);

        // Assign f/w code type to load => 0: boot-loader 1: dvbt2, 2: dvbt, 3: dvbc
        WriteReg(0x0900 + (0x4f) * 2, u8Data);


        // enable miu access of mcu gdma
        bRet &= WriteReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        WriteReg(0x0b00+(0x19)*2, 0x00);

#if(TIMING_VERIFICATION == 1)
        tmm_10 = GIVE_ME_TIME
                 printf("[tmm8]t10 - t9 = %ld (%ld - %ld)\n",tmm_10-tmm_9,tmm_10,tmm_9);
#endif


        do
        {
            ReadReg(0x0900+(0x4f)*2, &u8DoneFlag);

            if (u32Timeout++ > 500)
                return MAPI_FALSE;

            usleep(1*1000);

        }
        while(u8DoneFlag != 0xaa);

        printf("this si 3 \n");
#if(TIMING_VERIFICATION == 1)
        tmm_11 = GIVE_ME_TIME
                 printf("[tmm8]t11 - t10 = %ld (%ld - %ld)\n",tmm_11-tmm_10,tmm_11,tmm_10);
#endif


        // mask miu access for all and mcu
        bRet &= WriteReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= WriteReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        usleep(10);

        // Disable MCU
        WriteReg(0x0b00+(0x19)*2, 0x03);

        // Run code on loaded firmware
        WriteReg(0x1000+(0x0c)*2, 0x05);

        do
        {
            ReadReg(0x1000+(0x0c)*2, &u8DoneFlag);

            if (u32Timeout++ > 500)
                return MAPI_FALSE;

            usleep(1*1000);

        }
        while(u8DoneFlag != 0x05);

        printf("this is 4 \n");


        // enable miu access of mcu gdma
        bRet &= WriteReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        WriteReg(0x0b00+(0x19)*2, 0x00);
    }

#if(TIMING_VERIFICATION == 1)
    tmm_12 = GIVE_ME_TIME
             printf("[tmm8]t12 - t11 = %ld (%ld - %ld), TYPE is %d \n",tmm_12-tmm_11,tmm_12,tmm_11,device_demodulator::m_enCurrentDemodulator_Type);
#endif


    printf("1: %d, 2:%d, 3:%d \n",E_DEVICE_DEMOD_DVB_T2,E_DEVICE_DEMOD_DVB_T,E_DEVICE_DEMOD_DVB_C);
    switch(m_enCurrentDemodulator_Type)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            printf("11111\n");
            DTV_DVBT2_DSPReg_Init();
            break;
        case E_DEVICE_DEMOD_DVB_T:
            printf("2222\n");
            DTV_DVBT_DSPReg_Init();
            break;
        #if(SKIP_DVBC_LOAD==0)
        case E_DEVICE_DEMOD_DVB_C://mick
            printf("3333\n");
            DTV_DVBC_DSPReg_Init();
            break;
        #endif
        default:
            printf("44444\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_13 = GIVE_ME_TIME
             printf("[tmm8]t13 - t12 = %ld (%ld - %ld)\n",tmm_13-tmm_12,tmm_13,tmm_12);
#endif

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCode\n"));
    return bRet;
}




/************************************************************************************************
Subject:    Set demodulator type
Function:   SetCurrentDemodulatorType
Parmeter:   enDemodType
Return:     BOOLEAN :
*************************************************************************************************/

MAPI_BOOL SetCurrentDemodulatorType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));

    printf("device_demodulator_msb123xc: SetCurrentDemodulatorType %d\n", (int)enDemodType);
#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
    if(enDemodType == m_enCurrentDemodulator_Type)
    {
        return MAPI_TRUE;
    }
#endif
    u32ChkScanTimeStart = MsOS_GetSystemTime();
    switch(enDemodType)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            m_enCurrentDemodulator_Type = enDemodType;
            break;
        case E_DEVICE_DEMOD_DVB_T:
            m_enCurrentDemodulator_Type = enDemodType;
            break;
        case E_DEVICE_DEMOD_DVB_C://mick
            m_enCurrentDemodulator_Type = enDemodType;
            break;
        default:
            return MAPI_FALSE;
    }
#if (PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
    if (bPower_init_en != TRUE)
    {
        return MAPI_TRUE;
    }
#endif
    if (LoadDSPCode() == MAPI_FALSE)
    {

        ERR_DEMOD_MSB(printf(">>>>MSB123xC:Fail\n"));
        return MAPI_FALSE;
    }
    else
    {
        DBG_DEMOD_MSB(printf(">>>MSB123xC:OK\n"));
    }


    return MAPI_TRUE;
}

/************************************************************************************************
Subject:    Get PLP-ID Bit Map
Function:   DTV_GetPlpBitMap
Parmeter:   u32PlpBitMap
Return:     BOOLEAN :
Remark:     32 byte of data to show PLP-ID existense
            |00000000|00000000|...|00000000|00011111|11111111|
             byte31   byte30   ... byte2    byte1    byte0   => 256 bit in total
*************************************************************************************************/

MAPI_BOOL DTV_GetPlpBitMap(MAPI_U8* u8PlpBitMap)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL   status = MAPI_TRUE;
    MAPI_U8     u8Data = 0;
    MAPI_U8     indx = 0;

    status &= ReadDspReg(E_T2_L1_FLAG, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
        return MAPI_FALSE;

    while(indx < 32)
    {
        status &= ReadDspReg(E_T2_PLP_ID_ARR+indx, &u8Data);
        u8PlpBitMap[indx] = u8Data;
        // printf("=========================>[GetPlpBitMap] u8PlpBitMap[%d] = 0x%x\n", indx, u8Data);
        indx++;
    }

    return status;
}

/************************************************************************************************
Subject:    Get GROUP_ID upon PLP_ID for MPLP mode
Function:   DTV_GetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
*************************************************************************************************/

MAPI_BOOL DTV_GetPlpGroupID(MAPI_U8 u8PlpID, MAPI_U8* u8GroupID)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_BOOL   status = MAPI_TRUE;
    MAPI_U8 u8Data = 0;
    status &= ReadDspReg(E_T2_L1_FLAG, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        ERR_DEMOD_MSB(printf("[msb123xc]dvbt2 L1 not ready yet\n"));
        return MAPI_FALSE;
    }
    ReadDspReg(E_T2_DVBT2_LOCK_HIS, &u8Data);

    if ((u8Data&BIT7) == 0x00)
    {
        ERR_DEMOD_MSB(printf("[msb123xc]dvbt2 is un-lock\n"));
        return MAPI_FALSE;
    }
    // assign PLP-ID value
    status &= WriteReg(0x2700+(0x78)*2, u8PlpID);
    status &= WriteReg(0x2700+(0x01)*2 + 1, 0x01);  // MEM_EN
    usleep(1000);
    status &= ReadReg(0x2700+(0x79)*2, u8GroupID);
    status &= WriteReg(0x2700+(0x01)*2 + 1, 0x00);  // ~MEM_EN
//  printf("=========================>[GetPlpGroupID] PlpID = %d, GroupID = %d\n", u8PlpID, *u8GroupID);

    return status;
}

/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
*************************************************************************************************/

MAPI_BOOL DTV_SetPlpGroupID(MAPI_U8 u8PlpID, MAPI_U8 u8GroupID)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n", __func__, __LINE__));

    MAPI_BOOL   status = MAPI_TRUE;
    FECLock = MAPI_FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();

    g_msb123xc_plp_id = u8PlpID;
    g_msb123xc_group_id = u8GroupID;


    // assign Group-ID and PLP-ID value (must be written in order)
     status &= WriteDspReg(0x0122, u8GroupID);
     status &= WriteDspReg(0x0121, u8PlpID);

    return status;
}

MAPI_U8 MDrv_Demod_GetPlpIDList(void)
{
    MAPI_U8 i, j, u8PlpBitMap[32];

    PLPIDSize = 0;
    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));
    if(FALSE == DTV_GetPlpBitMap(u8PlpBitMap))
    {
        ERR_DEMOD_MSB(printf("MDrv_Demod_GetPlpIDList fail\n"));
        return 0;
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
    return PLPIDSize;
}

MAPI_U8 MDrv_Demod_GetNextPlpID(MAPI_U8 u8Index)
{
    return PLPIDList[u8Index];
}

MAPI_BOOL MDrv_Demod_SetPlpID(MAPI_U8 u8PlpID)
{
    MAPI_U8   u8GroupID = 0xff;
    MAPI_BOOL status = MAPI_TRUE;

    status &= DTV_GetPlpGroupID(u8PlpID, &u8GroupID);

    status &= DTV_SetPlpGroupID(u8PlpID,u8GroupID);

    return status;
}

// MAPI_BOOL device_demodulator::DTV_SetPlpGroupID(MAPI_U8 u8PlpID, MAPI_U8 u8GroupID)
// {
//     MAPI_BOOL   status = MAPI_TRUE;
//     // assign PLP-ID and Group-ID value
//     status &= WriteReg(0x0900+(0x4f)*2, u8PlpID);
//     status &= WriteReg(0x0900+(0x4f)*2 + 1, u8GroupID);
//
//     // cmd to change PLP
//     status &= WriteReg(0x0900+(0x4e)*2, 0xaa);
//     status &= WriteReg(0x0900+(0x4e)*2 + 1, 0x55);
//     u32ChkScanTimeStart = MsOS_GetSystemTime();
//     return status;
// }

/*@ </Operation ID=Im17018142m1221763cc7cmm45a6> @*/
/************************************************************************************************
Subject:    channel change config
Function:   MSB123xC_Config
Parmeter:   BW: bandwidth
Return:     BOOLEAN :
Remark:
*************************************************************************************************/
/*
MAPI_BOOL device_demodulator::DTV_DVB_T_Config(RF_CHANNEL_BANDWIDTH BW, MAPI_BOOL bSerialTS, MAPI_BOOL bPalBG)
{
    U8              bandwidth;
//  U8              status = MAPI_TRUE;
    static B16      reg_force = 0;
    static B16      reg_set_lp = 0;
    static B16      reg_auto_scan = 0;

    //printf(" @MSB123xC_config_dvbt start = [%d][%d][%d][%d][%d][%d]\n",u16ChBw,bSetLp,bForce,bAutoScan,u16FFTMode,u16CP);

    switch(u16ChBw)
    {
        case 6000:
            bandwidth = 1;
            break;
        case 7000:
            bandwidth = 2;
            break;
        case 8000:
        default:
            bandwidth = 3;
            break;
    }

    Reset();

    //// DSP Register Overwrite ///////////////////

    // Bandwidth: 0:5MHz, 1:6MHz, 2:7MHz, 3:8MHz
    if( WriteDspReg(0x17, bandwidth)!= 0)//Driver update 2009/11/20
    {
        printf(" @MSB123xC_Config BW seting NG\n");
        return MAPI_FALSE;
    }
    if (reg_set_lp != bSetLp)
    {
        reg_set_lp = bSetLp;// andy 2009-12-15  8:03:16
        // LP select: 0: select HP, 1: setlect LP
        if( WriteDspReg(0x1A, bSetLp)!= 0)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Config LP seting NG\n");
            return MAPI_FALSE;
            }
        }

        if (bForce != reg_force)
        {
        reg_force = bForce;

        // mode-cp force: 0: auto, 1: force
        if( WriteDspReg(0x0A, (U8)bForce)!= 0)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Config MODE-CP seting NG\n");
            return MAPI_FALSE;
        }
        // FFT mode - 0:2K, 1:8K
        if( WriteDspReg(0x18, (U8)u16FFTMode)!= 0)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Config FFT seting NG\n");
            return MAPI_FALSE;
        }
        // CP - 0:1/32, 1/16, 1/8, 1/4
        if( WriteDspReg(0x19, (U8)u16CP)!= 0)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Config CP seting NG\n");
            return MAPI_FALSE;
        }
    }

    if (bAutoScan != reg_auto_scan)
    {
        reg_auto_scan = bAutoScan;

        // Auto Scan - 0:channel change, 1:auto-scan
        if( WriteDspReg(0x0C, (U8)bAutoScan)!= 0)//Driver update 2009/11/20
        {
            printf(" @MSB123xC_Config auto mode seting NG\n");
            return MAPI_FALSE;
        }
    }

    printf(" @MSB123xC_Config OK\n");
    return  MAPI_TRUE;
}
*/

void Driving_Control(MAPI_BOOL bEnable)
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8 u8Temp;
    ReadReg(0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    WriteReg(0x0958, u8Temp);

    ReadReg(0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp &(~0x0F);
    }
    WriteReg(0x0959, u8Temp);
}

MAPI_U16 DTV_GetCellID()
{
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    MAPI_U8 id=0;
    MAPI_U8 status = MAPI_TRUE;
    MAPI_U16  cell_id  = 0;
    MsOS_ObtainMutex(_s32_Demod_DVBT2_Mutex, MSOS_WAIT_FOREVER);
    if (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T2)
    {
        MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
        return 0;
    }
    else if ((m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T)
             || (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_C))
    {
        status &= WriteReg(0x0ffe, 0x01);

        status &= ReadReg(0x0f2b, &id);
        cell_id = (MAPI_U16)id;

        status &= ReadReg(0x0f2a, &id);
        cell_id |= (MAPI_U16)id << 8;

        status &= WriteReg(0x0ffe, 0x00);

        if(status)
        {
            printf(" @MSB123xC_GetCellId OK\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return  MAPI_TRUE;
        }
        else
        {
            printf(" @MSB123xC_GetCellId NG\n");
            MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
            return MAPI_FALSE;
        }
    }
    MsOS_ReleaseMutex(_s32_Demod_DVBT2_Mutex);
    return cell_id;

}

/*
MAPI_U16 MSB123xC_Lock(E_SYSTEM system, COFDM_LOCK_STATUS eStatus )
{
    printf("%s(),%d\n",__FUNCTION__,__LINE__);
    MAPI_U16        u16Address = 0;
    MAPI_U8         cData = 0;
    MAPI_U8         cBitMask = 0;
    MAPI_U16        Ret = 0;
    MAPI_U8         use_dsp_reg = 0;//addy update 0805

    if(system == E_DEVICE_DEMOD_DVB_C)
    {
        //MSB1228_Check_StepLock(E_SYS_DVBC);
    }
    else if(system == E_DEVICE_DEMOD_DVB_T)
    {
        //MSB1228_Check_StepLock(E_SYS_DVBT);
    }
    else if(system == E_DEVICE_DEMOD_DVB_T2)
    {
        //MSB1228_Check_StepLock(E_SYS_DVBT);
    }

    switch( eStatus )
    {
            //############################ DVB-C ###############################
        case COFDM_FEC_LOCK_DVBC:
            //Update reg information
            ReadReg(0x11E0, &cData);//addy update 0805
            if (cData == 0x0C)
            {
                Ret = TRUE;
            }
            else
            {
                Ret = FALSE;
            }

            return Ret;

            break;

        case COFDM_TR_LOCK_DVBC://addy update 0805
            u16Address =  0x2250; //DVB-C : TR Lock,
            cBitMask = BIT0;

            break;
            //############################ DVB-T ###############################
        case COFDM_FEC_LOCK_DVBT:
            //Update reg information
            ReadReg(0x11E0, &cData);//addy update 0805
            if (cData == 0x0B)
            {
                return TRUE;
            }
            else
                return FALSE;

            break;

        case COFDM_PSYNC_LOCK:
            u16Address =  0x112C; //FEC: P-sync Lock,ok
            cBitMask = BIT1;

            break;

        case COFDM_TPS_LOCK:
            u16Address =  0x0f22; //TPS Lock,ok
            cBitMask = BIT1;

            break;
        case COFDM_TPS_LOCK_HISTORY:
            // change to use dsp reg ok
            use_dsp_reg = 1;
            u16Address =  0x00F0; //TPS lock history,// andy 2009-9-28  7:20:03
            cBitMask = BIT3;

            break;

        case COFDM_DCR_LOCK:
            u16Address =  0x3E45; //DCR Lock,ok
            cBitMask = BIT0;
            break;

        case COFDM_AGC_LOCK:
            u16Address =  0x3E2F; //AGC Lock,ok
            cBitMask = BIT0;

            break;

        case COFDM_MODE_DET:
            u16Address =  0x0ECF; //Mode CP Detect,ok
            cBitMask = BIT4;

            break;

        case COFDM_LOCK_STABLE_DVBT: //0923 update
            // change to use dsp reg
            use_dsp_reg = 1;
            u16Address =  0x00F0; //Lock Stable,
            cBitMask = BIT6;

            break;

        case COFDM_SYNC_LOCK_DVBT://addy 0629
            // change to use dsp reg
            use_dsp_reg = 1;
            u16Address =  0x00F0; //Valid sync,
            cBitMask = BIT2;

            break;

        case COFDM_FAST_LOCK_DVBT://addy 0920
            // change to use dsp reg
            use_dsp_reg = 1;
            u16Address =  0x00F0; //Valid sync,
            cBitMask = BIT5;

            break;

            //############################ DVB-T2 #############################
        case COFDM_P1_LOCK:
            u16Address =  0x2482; //Pl lock,
            cBitMask = BIT3;
            break;

        case COFDM_P1_LOCK_HISTORY:
            use_dsp_reg = 1;
            u16Address =  E_T2_DVBT2_LOCK_HIS; //Pl ever lock,
            cBitMask = BIT3;
            break;

        case COFDM_L1_CRC_LOCK:
            u16Address =  0x2741; //L1 CRC check,
            cBitMask = BIT5|BIT6|BIT7;
            break;

        case COFDM_FEC_LOCK_T2:
#if 0
        {
            U16 u16RegAddress;
            U8 u8Data;
            u16RegAddress=0x0900+(0x48)*2;
            ReadReg(u16RegAddress, &u8Data);

            DBG_DOMOD_MSB123xC(printf(">>>MSB123xC:DVB-T2 [%s] Lock Status = 0x%x\n", __FUNCTION__, u8Data));

            if(u8Data == 0x09)
                Ret = TRUE;
            else
                Ret = FALSE;

            return Ret;
        }
#else
        use_dsp_reg = 1;
        u16Address =  E_T2_DVBT2_LOCK_HIS; //FEC lock,
        cBitMask = BIT7;
#endif
        break;


        default:
            return FALSE;
    }

    //addy update 0805
    if (!use_dsp_reg)
    {
        if (ReadReg(u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (ReadDspReg(u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }


    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }

    return FALSE;
}
*/
MAPI_BOOL DTV_IsHierarchyOn(void)
{

    return FALSE;

}

MAPI_BOOL msb123xc_flash_mode_en(void)
{
    MAPI_BOOL  retb = TRUE;
    MAPI_U8    data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MAPI_U8    u8MsbData[6] = {0};
    MAPI_U8    ch_num  = 3;
    MAPI_U8    u8Data  = 0;
    MAPI_U16   u16Addr = 0;
    MAPI_U8    retry_num = MSB123xC_MAX_FLASH_ON_RETRY_NUM;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]msb123xc_flash_mode_en\n"));

    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    do
    {

        if (retry_num != MSB123xC_MAX_FLASH_ON_RETRY_NUM)
        {
            ERR_DEMOD_MSB(printf("[msb123xc][error]flash mode en fail.....retry=%d\n",retry_num);)
        }
        // retb = TRUE;
        // password
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
//        retb &= iptr->WriteBytes(0, NULL, 5, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, data);

        // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
        data[0] = 0x71;
//        retb &= iptr->WriteBytes(0, NULL, 1, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
        data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
//        retb &= iptr->WriteBytes(0, NULL, 1, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
        data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
//        retb &= iptr->WriteBytes(0, NULL, 1, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
        data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
//        retb &= iptr->WriteBytes(0, NULL, 1, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
        data[0] = 0x53;
//        retb &= iptr->WriteBytes(0, NULL, 1, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
        data[0] = 0x7f;
//        retb &= iptr->WriteBytes(0, NULL, 1, data);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, data);

        /*
            // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
            data[0] = 0x35;
            retb &= iptr->WriteBytes(0, NULL, 1, data);

            // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
            data[0] = 0x71;
            retb &= iptr->WriteBytes(0, NULL, 1, data);
        */
        retb = TRUE;

        // confirm first, 0x99 and 0xaa.
        // beg read register
        u16Addr = 0x0900+(0x4f<<1);
        u8Data = 0x0;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;

        u8MsbData[0] = 0x35;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
//        retb &= iptr->WriteBytes(0, NULL, 5, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//        retb &= iptr->ReadBytes(0, NULL, 1, &u8Data);
        retb &= MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
        // end read register

        if ((u8Data == 0x99) || (u8Data == 0xaa))
        {
            ERR_DEMOD_MSB(printf("[msb123xc][warning]flash is already on....\n");)
            break;
        }
        // flash mode enable.
        // beg read register
        u16Addr = 0x0900+(0x28<<1)+1;
        u8Data = 0x0;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;

        u8MsbData[0] = 0x35;

//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);


        u8MsbData[0] = 0x10;
//        retb &= iptr->WriteBytes(0, NULL, 5, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//        retb &= iptr->ReadBytes(0, NULL, 1, &u8Data);
        retb &= MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, &u8Data);
        
        u8MsbData[0] = 0x34;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

        // end read register

        // beg write register
        u16Addr = 0x0900+(0x28<<1) + 1;
        u8Data &= (0xff-0x01);

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;
        u8MsbData[5] = u8Data;

        u8MsbData[0] = 0x35;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
//        retb &= iptr->WriteBytes(0, NULL, 6, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 6, u8MsbData);
        
        u8MsbData[0] = 0x34;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
        // end write register

        // beg write register
        u16Addr = 0x0900+(0x06<<1);
        u8Data = 0x10;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;
        u8MsbData[5] = u8Data;

        u8MsbData[0] = 0x35;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
//        retb &= iptr->WriteBytes(0, NULL, 6, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 6, u8MsbData);
        
        u8MsbData[0] = 0x34;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
        // end write register

        // beg write register

        u16Addr = 0x0900+(0x07<<1);
        u8Data = 0x10;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;
        u8MsbData[5] = u8Data;

        u8MsbData[0] = 0x35;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
//        retb &= iptr->WriteBytes(0, NULL, 6, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
    }
    while( (retb == FALSE) && (retry_num-- != 0));
    // end write register

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]msb123xc_flash_mode_en,retb=%d\n",retb));

    return retb;
}

MAPI_BOOL msb123xc_flash_boot_ready_waiting(MAPI_U8 *ptimeout)
{

    MAPI_BOOL  retb = TRUE;
//    MAPI_U8    data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MAPI_U8    u8MsbData[6] = {0};
    MAPI_U8    u8Data  = 0;
    MAPI_U16   u16Addr = 0;
    MAPI_U8    u8_timeout = 0;

    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]msb123xc_flash_boot_ready_waiting\n"));

    MsOS_ObtainMutex(_s32_Demod_DVBT2_RW_Mutex, MSOS_WAIT_FOREVER);
//    mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    // wait for flash->dram ready.
    // read register

    u16Addr = 0x0900+(0x4f<<1);
    u8Data = 0x0;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8_timeout = 0xff;

    while( (u8Data != 0x99) && (u8Data != 0xaa) && (u8_timeout-->0))
    {
        u8MsbData[0] = 0x35;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
//        retb &= iptr->WriteBytes(0, NULL, 5, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 5, u8MsbData);
//        retb &= iptr->ReadBytes(0, NULL, 1, &u8Data);
        retb &= MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
//        retb &= iptr->WriteBytes(0, NULL, 1, u8MsbData);
        retb &= MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, DEMOD_SLAVE_ID, 0, NULL, 1, u8MsbData);
        // 10ms
        usleep(1000*10);
    }
    // end read register
    *ptimeout = 0;
    if (u8_timeout == 0x00)
    {
        *ptimeout = 1;
        ERR_DEMOD_MSB(printf("[msb123xc][error]msb123xc_flash_boot_ready_waiting, timeout....\n");)
    }

    MsOS_ReleaseMutex(_s32_Demod_DVBT2_RW_Mutex);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]msb123xc_flash_boot_ready_waiting, t=%d\n",u8_timeout));
    return retb;
}

/*
MAPI_BOOL device_demodulator::DTV_GetVersion(MS_U32 *ver)
{

    MS_BOOL bRet = true;
    MS_U8 tmp = 0;
    MS_U32 u32_Version;

    bRet &= ReadDspReg(0x02, &tmp);
    u32_Version = tmp;
    bRet &= ReadDspReg(0x03, &tmp);
    u32_Version = (u32_Version <<8) | tmp;
    bRet &= ReadDspReg(0x04, &tmp);
    u32_Version = (u32_Version <<8) | tmp;
    *ver = u32_Version;

    return bRet;
}
*/

