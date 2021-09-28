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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  DVBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBT_H_
#define _DRV_DVBT_H_

#include "MsCommon.h"
//#include "drvDMD_common.h"
#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_DMD_MSB123xc_INTERN_LIB_CODE           {'1','2', '3','x'} //Lib code
#define MSIF_DMD_MSB123xc_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_MSB123xc_INTERN_BUILDNUM           {'0','0' }    //Build Number
#define MSIF_DMD_MSB123xc_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define DMD_MSB123xc_EXTERN_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_MSB123xc_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_MSB123xc_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_MSB123xc_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_MSB123xc_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_DMD_MSB123xc_DBGLV_NONE,    // disable all the debug message
    E_DMD_MSB123xc_DBGLV_INFO,    // information
    E_DMD_MSB123xc_DBGLV_NOTICE,  // normal but significant condition
    E_DMD_MSB123xc_DBGLV_WARNING, // warning conditions
    E_DMD_MSB123xc_DBGLV_ERR,     // error conditions
    E_DMD_MSB123xc_DBGLV_CRIT,    // critical conditions
    E_DMD_MSB123xc_DBGLV_ALERT,   // action must be taken immediately
    E_DMD_MSB123xc_DBGLV_EMERG,   // system is unusable
    E_DMD_MSB123xc_DBGLV_DEBUG,   // debug-level messages
} eDMD_MSB123xc_DbgLv;

typedef enum
{
    // Operation Mode Settings,Address Start at 0x20
    C_opmode_rfagc_en = 0x20,
    C_opmode_humdet_en,
    C_opmode_dcr_en,
    C_opmode_iqb_en,
    C_opmode_auto_iq,
    C_opmode_auto_rfmax,
    C_opmode_auto_aci,
    C_opmode_auto_scan,
    C_opmode_auto_scan_sym_rate,    //0x28
    C_opmode_auto_scan_qam,
    C_opmode_atv_detector_en,
    C_opmode_rsv_0x0B,
    C_opmode_rsv_0x0C,
    C_opmode_rsv_0x0D,
    C_opmode_rsv_0x0E,
    C_opmode_rsv_0x0F,

    // Config Params
    C_config_rssi,      //0x30
    C_config_zif,
    C_config_freq,
    C_config_fc_l,
    C_config_fc_h,
    C_config_fs_l,
    C_config_fs_h,
    C_config_bw_l,
    C_config_bw_h,
    C_config_bw1_l,
    C_config_bw1_h,
    C_config_bw2_l,
    C_config_bw2_h,
    C_config_bw3_l,
    C_config_bw3_h,
    C_config_rsv_0x1F,
    C_config_rfmax,     //0x40
    C_config_qam,
    C_config_iq_swap,
    C_config_cci,
    C_config_ts_serial,
    C_config_ts_clk_rate,
    C_config_ts_out_inv,
    C_config_ts_data_swap,

    DVBC_PARAM_LEN,
} DVBC_Param;

typedef enum
{
#if 1
    // operation mode settings
    T_OPMODE_RFAGC_EN  = 0x20,   // 0x20
    T_OPMODE_HUMDET_EN,
    T_OPMODE_DCR_EN,
    T_OPMODE_IIS_EN,
    T_OPMODE_CCI_EN,
    T_OPMODE_ACI_EN,
    T_OPMODE_IQB_EN,
    T_OPMODE_AUTO_IQ,
    T_OPMODE_AUTO_RFMAX,    // 0x28
    T_OPMODE_AUTO_ACI,
    T_OPMODE_FIX_MODE_CP,
    T_OPMODE_FIX_TPS,
    T_OPMODE_AUTO_SCAN,
    T_OPMODE_RSV_0X2D,
    T_OPMODE_RSV_0X2E,
    T_OPMODE_RSV_0X2F,

    // channel config param
    T_CONFIG_RSSI,    // 0x30
    T_CONFIG_ZIF,
    T_CONFIG_FREQ,
    T_CONFIG_FC_L,
    T_CONFIG_FC_H,
    T_CONFIG_FS_L,
    T_CONFIG_FS_H,
    T_CONFIG_BW,
    T_CONFIG_MODE,    // 0x38
    T_CONFIG_CP,
    T_CONFIG_LP_SEL,
    T_CONFIG_CSTL,
    T_CONFIG_HIER,
    T_CONFIG_HPCR,
    T_CONFIG_LPCR,
    T_CONFIG_IQ_SWAP,
    T_CONFIG_RFMAX,    // 0x40
    T_CONFIG_CCI,
    T_CONFIG_ICFO_RANGE,
    T_CONFIG_RFAGC_REF,
    T_CONFIG_IFAGC_REF_2K,
    T_CONFIG_IFAGC_REF_8K,
    T_CONFIG_IFAGC_REF_ACI,
    T_CONFIG_IFAGC_REF_IIS_2K,
    T_CONFIG_IFAGC_REF_IIS_8K,    // 0x48
    T_CONFIG_ACI_DET_TH_L,
    T_CONFIG_ACI_DET_TH_H,
    T_CONFIG_TS_SERIAL,
    T_CONFIG_TS_CLK_RATE,
    T_CONFIG_TS_OUT_INV,
    T_CONFIG_TS_DATA_SWAP,
    T_CONFIG_2K_SFO_H,
    T_CONFIG_2K_SFO_L,    // 0x50
    T_CONFIG_8K_SFO_H,
    T_CONFIG_8K_SFO_L,
    T_CONFIG_CHECK_CHANNEL,
    T_CONFIG_SLICER_SNR_POS,
    T_CONFIG_TDP_CCI_KP,
    T_CONFIG_CCI_FSWEEP,
    T_CONFIG_TS_CLK_RATE_AUTO,
    T_CONFIG_IF_INV_PWM_OUT_EN,

    /**********************
     * crc =
     *    ~(T_OPMODE_RFAGC_EN^T_OPMODE_HUMDET_EN^....^T_CONFIG_TS_CLK_RATE_AUTO)
     ************************/
    T_PARAM_CHECK_SUM,

    T_DVBT_LOCK_HIS   = 0xF0,
	T_DVBT2_NOCHAN_Flag = 0xF1,
	T_DVBT_NOCHAN_Flag = 0xF2,

    DVBT_PARAM_LEN,
#else
T_OPMODE_RFAGC_EN  = 0x20,
T_OPMODE_AUTO_IQ,
T_CONFIG_ZIF,
T_CONFIG_FC_L,
T_CONFIG_FC_H,
T_CONFIG_FS_L,
T_CONFIG_FS_H,
T_CONFIG_BW,
T_CONFIG_IQ_SWAP,
T_CONFIG_TS_SERIAL,
T_CONFIG_TS_CLK_RATE,
T_CONFIG_TS_OUT_INV,
T_CONFIG_TS_DATA_SWAP,
T_CONFIG_TS_CLK_RATE_AUTO,
T_PARAM_CHECK_SUM,
 DVBT_PARAM_LEN,

#endif
} DVBT_Param;

typedef enum
{
    // fw version, check sum
    E_T2_CHECK_SUM_L      = 0x00,
    E_T2_CHECK_SUM_H,
    E_T2_FW_VER_0,
    E_T2_FW_VER_1,
    E_T2_FW_VER_2,

    // operation mode
    E_T2_ZIF_EN           = 0x20,
    E_T2_RF_AGC_EN,
    E_T2_HUM_DET_EN,
    E_T2_DCR_EN,
    E_T2_IQB_EN,
    E_T2_IIS_EN,
    E_T2_CCI_EN,
    E_T2_LOW_PWR_DET_EN,
    E_T2_ACI_DET_EN,
    E_T2_TD_MOTION_EN,
    E_T2_FD_MOTION_EN,

    // channel tuning param
    E_T2_BW               = 0x40,
    E_T2_FC_L             = 0x41,
    E_T2_FC_H             = 0x42,
    E_T2_FS_L,
    E_T2_FS_H,
    E_T2_ZIF,
    E_T2_GI,
    E_T2_ACI_DET_TYPE,
    E_T2_AGC_REF,         //0x48
    E_T2_RSSI_REF,
    E_T2_SNR_TIME_L,
    E_T2_SNR_TIME_H,
    E_T2_BER_CMP_TIME_L,
    E_T2_BER_CMP_TIME_H,
    E_T2_SFO_CFO_NUM,
    E_T2_CCI,
    E_T2_ACI_DET_TH_L,    //0x50
    E_T2_ACI_DET_TH_H,
    E_T2_TS_SERIAL     = 0x52,
    E_T2_TS_CLK_RATE   = 0x53,
    E_T2_TS_OUT_INV    = 0x54,
    E_T2_TS_DATA_SWAP  = 0x55,
    E_T2_TDP_CCI_KP,
    E_T2_CCI_FSWEEP,      //0x57
    E_T2_TS_ERR_POL,      //0x58
    E_T2_IF_AGC_INV_PWM_EN,

    // dvbt2 lock history
    E_T2_DVBT2_LOCK_HIS   = 0xF0,
    E_T2_FEF_DET_IND,
    E_T2_MPLP_NO_COMMON_IND,

    // splp, mplp releted
    E_T2_PLP_ID_ARR       = 0x100,
    E_T2_L1_FLAG          = 0x120,
    E_T2_PLP_ID,
    E_T2_GROUP_ID,
    E_T2_PARAM_NUM,
} E_DVBT2_PARAM;

typedef enum
{
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1,
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2,
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_3,
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_4
} eDMD_MSB123xc_DemodI2CSlaveID;

typedef enum
{
    E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES,
    E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES
} eDMD_MSB123xc_DemodI2CMethod;

/// For demod init
typedef struct
{
    MS_U8 u8WO_SPI_Flash;
    MS_BOOL bPreloadDSPCodeFromMainCHIPI2C;
    MS_BOOL bFlashWPEnable;
    void (*fpGPIOReset)(MS_BOOL bOnOff);
    MS_BOOL (*fpMSB123xc_I2C_Access)(eDMD_MSB123xc_DemodI2CSlaveID eSlaveID, eDMD_MSB123xc_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_U8* pDVBC_DSP_REG;
    MS_U8* pDVBT_DSP_REG;
    MS_U8* pDVBT2_DSP_REG;
    MS_BOOL bEnableSPILoadCode;
    void (*fpMSB123xc_SPIPAD_En)(MS_BOOL bOnOff);
} sDMD_MSB123xc_InitData;

typedef enum
{
    E_DMD_MSB123xc_DEMOD_NONE,
    E_DMD_MSB123xc_DEMOD_DVBT2,
    E_DMD_MSB123xc_DEMOD_DVBT,
    E_DMD_MSB123xc_DEMOD_DVBC,
} eDMD_MSB123xc_DemodulatorType;

typedef enum
{
    E_DMD_MSB123xc_FAIL=0,
    E_DMD_MSB123xc_OK=1
} DMD_MSB123xc_Result;


typedef struct
{
    MS_U16 u16Version;
} DMD_MSB123xc_Info;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Init
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_Init(sDMD_MSB123xc_InitData *pDMD_MSB123xc_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called when exit VD input source
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_Exit(void);
//------------------------------------------------------------------------------
/// Set detailed level of DVBT driver debug message
/// u8DbgLevel : debug level for Parallel Flash driver\n
/// AVD_DBGLV_NONE,    ///< disable all the debug message\n
/// AVD_DBGLV_INFO,    ///< information\n
/// AVD_DBGLV_NOTICE,  ///< normal but significant condition\n
/// AVD_DBGLV_WARNING, ///< warning conditions\n
/// AVD_DBGLV_ERR,     ///< error conditions\n
/// AVD_DBGLV_CRIT,    ///< critical conditions\n
/// AVD_DBGLV_ALERT,   ///< action must be taken immediately\n
/// AVD_DBGLV_EMERG,   ///< system is unusable\n
/// AVD_DBGLV_DEBUG,   ///< debug-level messages\n
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
extern MS_BOOL MDrv_DMD_MSB123xc_SetDbgLevel(eDMD_MSB123xc_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBT driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
//extern DMD_DVBT_Info* MDrv_DMD_DVBT_GetInfo(DMD_DVBT_INFO_TYPE eInfoType);
//-------------------------------------------------------------------------------------------------
/// Get DVBT driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_DMD_MSB123xc_GetLibVer(const MSIF_Version **ppVersion);
////////////////////////////////////////////////////////////////////////////////
/// Get DVBT FW version
/// u16Addr       : the address of DVBT's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_GetFWVer(MS_U16 *ver);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_SetReg(MS_U16 u16Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_SetRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_SetReg2Bytes(MS_U16 u16Addr, MS_U16 u16Data);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_SetDSPReg(MS_U16 u16Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_MSB123xc_SetCurrentDemodulatorType
////////////////////////////////////////////////////////////////////////////////
extern void MDrv_DMD_MSB123xc_SetCurrentDemodulatorType(eDMD_MSB123xc_DemodulatorType eCurrentDemodulatorType);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_MSB123xc_LoadDSPCode
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_LoadDSPCode(void);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_MSB123xc__DTV_DVBT_DSPReg_CRC
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_DTV_DVBT_DSPReg_CRC(void);
////////////////////////////////////////////////////////////////////////////////
/// power on init
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB123xc_Power_On_Initialization(void);
#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBT_H_

