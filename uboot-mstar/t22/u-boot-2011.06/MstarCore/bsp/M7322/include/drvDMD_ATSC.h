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
/// @brief  ATSC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ATSC_H_
#define _DRV_ATSC_H_

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#ifndef UTPA2
#define DMD_ATSC_UTOPIA_EN                  1
#define DMD_ATSC_UTOPIA2_EN                 0
#else
#define DMD_ATSC_UTOPIA_EN                  0
#define DMD_ATSC_UTOPIA2_EN                 1
#endif

#define DMD_ATSC_STR_EN                     1
#define DMD_ATSC_MULTI_THREAD_SAFE          0
#define DMD_ATSC_MULTI_DMD_EN               1
#define DMD_ATSC_3PARTY_EN                  0
#define DMD_ATSC_3PARTY_IN_KERNEL_EN        0
#define DMD_ATSC_3PARTY_MSPI_EN             0

#if DMD_ATSC_3PARTY_EN
#undef DMD_ATSC_UTOPIA_EN
#undef DMD_ATSC_UTOPIA2_EN

#define DMD_ATSC_UTOPIA_EN                  0
#define DMD_ATSC_UTOPIA2_EN                 0

#if DMD_ATSC_3PARTY_IN_KERNEL_EN
 #ifndef UTPA2
  #define UTPA2
 #endif
 #ifndef MSOS_TYPE_LINUX_KERNEL
  #define MSOS_TYPE_LINUX_KERNEL
 #endif
#endif // #if DMD_ATSC_3PARTY_IN_KERNEL_EN
#endif // #if DMD_ATSC_3PARTY_EN

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsTypes.h"
#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#ifndef MSIF_TAG
#include "MsVersion.h"
#endif
#include "MsCommon.h"
#endif
#if  DMD_ATSC_UTOPIA2_EN || (DMD_ATSC_STR_EN && DMD_ATSC_UTOPIA_EN)
#include "utopia.h"
#endif
#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#include "drvMSPI.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef DLL_PACKED 
#define DLL_PACKED 
#endif

#ifndef DLL_PUBLIC 
#define DLL_PUBLIC 
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#define PRINT    printk
#else // #ifdef MSOS_TYPE_LINUX_KERNEL
#if DMD_ATSC_3PARTY_EN
// Please define PRINT according to 3 perty platform
#define PRINT    printf
#else // #if DMD_ATSC_3PARTY_EN
#define PRINT    printf
#endif // #if DMD_ATSC_3PARTY_EN
#endif // #ifdef MSOS_TYPE_LINUX_KERNEL

#if DMD_ATSC_MULTI_DMD_EN
#define DMD_ATSC_MAX_DEMOD_NUM          8
#define DMD_ATSC_MAX_EXT_CHIP_NUM       2
#else
#define DMD_ATSC_MAX_DEMOD_NUM          1
#define DMD_ATSC_MAX_EXT_CHIP_NUM       1
#endif

#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#define MSIF_DMD_ATSC_LIB_CODE          {'D','M','D','_','A','T', 'S','C','_'} //Lib code
#define MSIF_DMD_ATSC_LIBVER            {'0','0'}                              //LIB version
#define MSIF_DMD_ATSC_BUILDNUM          {'0','0' }                             //Build Number
#define MSIF_DMD_ATSC_CHANGELIST        {'0','0','0','0','0','0','0','0'}      //P4 ChangeList Number

#define DMD_ATSC_VER                    /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_ATSC_LIB_CODE,             /* IP__                                             */  \
    MSIF_DMD_ATSC_LIBVER,               /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_ATSC_BUILDNUM,             /* 00 ~ 99                                          */  \
    MSIF_DMD_ATSC_CHANGELIST,           /* CL#                                              */  \
    MSIF_OS
#endif // #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN

#ifndef BIT_
#define BIT_(n)                                 (1 << (n))
#endif

#define DMD_ATSC_LOCK_VSB_PRE_LOCK              BIT_(0)
#define DMD_ATSC_LOCK_VSB_FSYNC_LOCK            BIT_(1)
#define DMD_ATSC_LOCK_VSB_CE_LOCK               BIT_(2)
#define DMD_ATSC_LOCK_VSB_FEC_LOCK              BIT_(3)

#define DMD_ATSC_LOCK_QAM_AGC_LOCK              BIT_(8)
#define DMD_ATSC_LOCK_QAM_PRE_LOCK              BIT_(9)
#define DMD_ATSC_LOCK_QAM_MAIN_LOCK             BIT_(10)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    DMD_ATSC_HAL_CMD_Exit = 0,
    DMD_ATSC_HAL_CMD_InitClk,
    DMD_ATSC_HAL_CMD_Download,
    DMD_ATSC_HAL_CMD_FWVERSION,
    DMD_ATSC_HAL_CMD_SoftReset,
    DMD_ATSC_HAL_CMD_SetVsbMode,
    DMD_ATSC_HAL_CMD_Set64QamMode,
    DMD_ATSC_HAL_CMD_Set256QamMode,
    DMD_ATSC_HAL_CMD_SetModeClean,
    DMD_ATSC_HAL_CMD_Set_QAM_SR,
    DMD_ATSC_HAL_CMD_Active,
    DMD_ATSC_HAL_CMD_Check8VSB64_256QAM,
    DMD_ATSC_HAL_CMD_AGCLock,
    DMD_ATSC_HAL_CMD_Vsb_PreLock,
    DMD_ATSC_HAL_CMD_Vsb_FSync_Lock,
    DMD_ATSC_HAL_CMD_Vsb_CE_Lock,
    DMD_ATSC_HAL_CMD_Vsb_FEC_Lock,
    DMD_ATSC_HAL_CMD_QAM_PreLock,
    DMD_ATSC_HAL_CMD_QAM_Main_Lock,
    DMD_ATSC_HAL_CMD_ReadIFAGC,
    DMD_ATSC_HAL_CMD_CheckSignalCondition,
    DMD_ATSC_HAL_CMD_ReadSNRPercentage,
    DMD_ATSC_HAL_CMD_GET_QAM_SNR,
    DMD_ATSC_HAL_CMD_ReadPKTERR,
    DMD_ATSC_HAL_CMD_GetPreViterbiBer,
    DMD_ATSC_HAL_CMD_GetPostViterbiBer,
    DMD_ATSC_HAL_CMD_ReadFrequencyOffset,
    DMD_ATSC_HAL_CMD_TS_INTERFACE_CONFIG,
    DMD_ATSC_HAL_CMD_IIC_Bypass_Mode,
    DMD_ATSC_HAL_CMD_SSPI_TO_GPIO,
    DMD_ATSC_HAL_CMD_GPIO_GET_LEVEL,
    DMD_ATSC_HAL_CMD_GPIO_SET_LEVEL,
    DMD_ATSC_HAL_CMD_GPIO_OUT_ENABLE,
    DMD_ATSC_HAL_CMD_DoIQSwap,
    DMD_ATSC_HAL_CMD_GET_REG,
    DMD_ATSC_HAL_CMD_SET_REG,
    DMD_ATSC_HAL_CMD_SET_DIGRF_FREQ
} DMD_ATSC_HAL_COMMAND;

typedef struct DLL_PACKED
{
    MS_U8  Mode;
    MS_S16 FF;
    MS_S16 Rate;
} DMD_ATSC_CFO_DATA;

typedef struct DLL_PACKED
{
    MS_U32 BitErr;
    MS_U16 Error_window;
    MS_U32 Win_unit;
} DMD_ATSC_BER_DATA;

typedef struct DLL_PACKED
{
    MS_U16 noisepower;
    MS_U32 sym_num;
} DMD_ATSC_SNR_DATA;

typedef struct DLL_PACKED
{
    MS_U8 u8Pin;
    union
    {
        MS_BOOL bLevel;
        MS_BOOL bIsOut;
    };
} DMD_ATSC_GPIO_PIN_DATA;

typedef struct DLL_PACKED
{
    MS_U16 u16Addr;
    MS_U8  u8Data;
} DMD_ATSC_REG_DATA;

typedef enum
{
    DMD_ATSC_DBGLV_NONE,    // disable all the debug message
    DMD_ATSC_DBGLV_INFO,    // information
    DMD_ATSC_DBGLV_NOTICE,  // normal but significant condition
    DMD_ATSC_DBGLV_WARNING, // warning conditions
    DMD_ATSC_DBGLV_ERR,     // error conditions
    DMD_ATSC_DBGLV_CRIT,    // critical conditions
    DMD_ATSC_DBGLV_ALERT,   // action must be taken immediately
    DMD_ATSC_DBGLV_EMERG,   // system is unusable
    DMD_ATSC_DBGLV_DEBUG    // debug-level messages
} DMD_ATSC_DbgLv;

typedef enum
{
    DMD_ATSC_DEMOD_ATSC_VSB,
    DMD_ATSC_DEMOD_ATSC_64QAM,
    DMD_ATSC_DEMOD_ATSC_256QAM,
    DMD_ATSC_DEMOD_ATSC_16QAM,
    DMD_ATSC_DEMOD_ATSC_32QAM,
    DMD_ATSC_DEMOD_ATSC_128QAM,
    DMD_ATSC_DEMOD_MAX,
    DMD_ATSC_DEMOD_NULL = DMD_ATSC_DEMOD_MAX
} DMD_ATSC_DEMOD_TYPE;

typedef enum
{
    DMD_ATSC_SIGNAL_NO                   = 0,    /* little or no input power detected    */
    DMD_ATSC_SIGNAL_WEAK                 = 1,    /* some power detected.                 */
    DMD_ATSC_SIGNAL_MODERATE             = 2,
    DMD_ATSC_SIGNAL_STRONG               = 4,
    DMD_ATSC_SIGNAL_VERY_STRONG          = 8
} DMD_ATSC_SIGNAL_CONDITION;

typedef enum
{
    DMD_ATSC_GETLOCK,
    DMD_ATSC_GETLOCK_VSB_AGCLOCK,
    DMD_ATSC_GETLOCK_VSB_PRELOCK, // pilot lock
    DMD_ATSC_GETLOCK_VSB_FSYNCLOCK,
    DMD_ATSC_GETLOCK_VSB_CELOCK,
    DMD_ATSC_GETLOCK_VSB_FECLOCK,
    DMD_ATSC_GETLOCK_QAM_AGCLOCK,
    DMD_ATSC_GETLOCK_QAM_PRELOCK, // TR lock
    DMD_ATSC_GETLOCK_QAM_MAINLOCK
} DMD_ATSC_GETLOCK_TYPE;

typedef enum
{
    DMD_ATSC_LOCK,
    DMD_ATSC_CHECKING,
    DMD_ATSC_CHECKEND,
    DMD_ATSC_UNLOCK,
    DMD_ATSC_NULL
} DMD_ATSC_LOCK_STATUS;

#if (DMD_ATSC_STR_EN && !DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN)
typedef enum
{
    E_POWER_SUSPEND     = 1,    // State is backup in DRAM, components power off.
    E_POWER_RESUME      = 2,    // Resume from Suspend or Hibernate mode
    E_POWER_MECHANICAL  = 3,    // Full power off mode. System return to working state only after full reboot 
    E_POWER_SOFT_OFF    = 4,    // The system appears to be off, but some components remain powered for trigging boot-up.
} EN_POWER_MODE;
#endif

typedef enum
{
    I_PAD,
    Q_PAD,
} DMD_ATSC_SEL_IQPAD;

typedef enum
{
    QAM_64,
    QAM_256,
} DMD_ATSC_QAM_TYPE;

typedef struct DLL_PACKED
{
    MS_U8   QAM_type;
    MS_U16  symbol_rate;
}DMD_J83B_Info;

typedef enum _eDMD_SEL
{
    DMD0 = 0,
    DMD1,
    HK,
}eDMD_SEL;

/// For demod init
typedef struct DLL_PACKED
{
    // Timeout time
    MS_U16 u16VSBAGCLockCheckTime;//100
    MS_U16 u16VSBPreLockCheckTime;//300
    MS_U16 u16VSBFSyncLockCheckTime;//1200
    MS_U16 u16VSBFECLockCheckTime;//5000

    MS_U16 u16QAMAGCLockCheckTime;//100
    MS_U16 u16QAMPreLockCheckTime;//1000
    MS_U16 u16QAMMainLockCheckTime;//3000

    // register init
    MS_U8 *u8DMD_ATSC_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_ATSC_DSPRegInitSize;
    MS_U8 *u8DMD_ATSC_InitExt; // TODO use system variable type

    //By Tuners:
    MS_U16  u16IF_KHZ;//By Tuners
    MS_BOOL bIQSwap;//0
    MS_U16  u16AGC_REFERENCE;//0
    MS_BOOL bTunerGainInvert;//0
    MS_BOOL bIsQPad;//0 //Don't beed used anymore
    
    //By IC:
    MS_U8   u8IS_DUAL;//0
    MS_BOOL bIsExtDemod;//0
    
    //By TS (Only for MCP or ext demod):
    MS_U8 u1TsConfigByte_SerialMode : 1;
    MS_U8 u1TsConfigByte_DataSwap   : 1;
    MS_U8 u1TsConfigByte_ClockInv   : 1;
    MS_U8 u5TsConfigByte_DivNum     : 5;

    //By SYS I2C (Only for MCP or ext demod):
    MS_U8 u8I2CSlaveAddr;
    MS_U8 u8I2CSlaveBus;
    MS_BOOL (*I2C_WriteBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
    MS_BOOL (*I2C_ReadBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
    
    //By SYS MSPI (Only for MCP or ext demod):
    MS_BOOL bIsUseSspiLoadCode;
    MS_BOOL bIsSspiUseTsPin;
} DMD_ATSC_InitData;

typedef struct ATSC_IOCTL_DATA DMD_ATSC_IOCTL_DATA;

typedef struct DLL_PACKED
{
    MS_VIRT virtDMDBaseAddr;
    
    MS_BOOL bInit;
    MS_BOOL bDownloaded;

    #if DMD_ATSC_STR_EN
    MS_BOOL             bIsDTV;
    EN_POWER_MODE       eLastState;
    #endif
    DMD_ATSC_DEMOD_TYPE eLastType;
    MS_U16              u16SymRate;
    
    MS_BOOL bIsQPad;
    
    MS_BOOL (*HAL_DMD_ATSC_IOCTL_CMD)(DMD_ATSC_IOCTL_DATA *pData, DMD_ATSC_HAL_COMMAND eCmd, void *pPara);
    MS_BOOL (*HAL_DMD_ATSC_IOCTL_CMD1)(DMD_ATSC_HAL_COMMAND eCmd, void *pPara);
} DMD_ATSC_PriData;

typedef struct DLL_PACKED
{
    MS_U8  u8Version;
    MS_U32 u32ATSCScanTimeStart;
    MS_U32 u32ATSCFECLockTime;
    MS_U32 u32ATSCLockStatus; 
} DMD_ATSC_Info;

typedef struct DLL_PACKED
{
    DMD_ATSC_InitData  sDMD_ATSC_InitData;
    DMD_ATSC_PriData   sDMD_ATSC_PriData;
    DMD_ATSC_Info      sDMD_ATSC_Info;
} DMD_ATSC_ResData;

#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
typedef struct DLL_PACKED
{
    void (*DelayMS)(MS_U32 u32Ms);
    MSPI_ErrorNo (*MSPI_Init_Ext)(MS_U8 u8HWNum);
    void (*MSPI_SlaveEnable)(MS_BOOL Enable);
    MSPI_ErrorNo (*MSPI_Write)(MS_U8 *pData, MS_U16 u16Size);
    MSPI_ErrorNo (*MSPI_Read)(MS_U8 *pData, MS_U16 u16Size);
} DMD_ATSC_SYS_PTR;
#else
typedef struct DLL_PACKED
{
    MS_U32  (*GetSystemTimeMS)(void);       // Get sys time (unit: ms)
    void    (*DelayMS)(MS_U32 ms);          // Delay time (unit: ms)
    MS_BOOL (*CreateMutex)(MS_BOOL enable); // Create&Delete mutex
    void    (*LockDMD)(MS_BOOL enable);     // Enter&Leave mutex
    
    MS_U32 (*MSPI_Init_Ext)(MS_U8 u8HWNum);
    void (*MSPI_SlaveEnable)(MS_BOOL Enable);
    MS_U32 (*MSPI_Write)(MS_U8 *pData, MS_U16 u16Size);
    MS_U32 (*MSPI_Read)(MS_U8 *pData, MS_U16 u16Size);
} DMD_ATSC_SYS_PTR;
#endif
    
struct DLL_PACKED ATSC_IOCTL_DATA
{
    MS_U8             id;
    DMD_ATSC_ResData *pRes;
    MS_U8            *pExtChipChNum;
    MS_U8            *pExtChipChId;
    MS_U8            *pExtChipI2cCh;
    DMD_ATSC_SYS_PTR  sys;
};

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
/// Set detailed level of ATSC driver debug message
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
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetDbgLevel(DMD_ATSC_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of ATSC driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern DMD_ATSC_Info* MDrv_DMD_ATSC_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// Get ATSC driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetLibVer(const MSIF_Version **ppVersion);

////////////////////////////////////////////////////////////////////////////////
/// Should be called once when power on init (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Initial_Hal_Interface(void);

#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN

////////////////////////////////////////////////////////////////////////////////
///                            SINGLE DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when enter DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Init(DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when exit DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Exit(void);
////////////////////////////////////////////////////////////////////////////////
/// Get Initial Data
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U32 MDrv_DMD_ATSC_GetConfig(DMD_ATSC_InitData *psDMD_ATSC_InitData);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Reset FW state machine (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetReset(void);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod (only for J83B)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Set_QAM_SR(DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate);
////////////////////////////////////////////////////////////////////////////////
/// Active demod (not use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetActive(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Set demod power state for STR
////////////////////////////////////////////////////////////////////////////////
#if DMD_ATSC_STR_EN
DLL_PUBLIC extern MS_U32 MDrv_DMD_ATSC_SetPowerState(EN_POWER_MODE u16PowerState);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod lock status
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ATSC_LOCK_STATUS MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK_TYPE eType);
////////////////////////////////////////////////////////////////////////////////
/// Get demod modulation mode
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ATSC_DEMOD_TYPE MDrv_DMD_ATSC_GetModulationMode(void);
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal strength (IF AGC gain)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetSignalStrength(MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal quality (NO SIGNAL, WEAK, MODERATE, STRONG and VERY_STRONG)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ATSC_SIGNAL_CONDITION MDrv_DMD_ATSC_GetSignalQuality(void);
////////////////////////////////////////////////////////////////////////////////
/// Get demod SNR percentage (MAX SNR 40dB)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U8 MDrv_DMD_ATSC_GetSNRPercentage(void);
////////////////////////////////////////////////////////////////////////////////
/// Get demod SNR (only for J.83ABC)
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GET_QAM_SNR(float *f_snr);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GET_QAM_SNR(DMD_ATSC_SNR_DATA *f_snr);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod packet error number
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Read_uCPKT_ERR(MS_U16 *u16PacketErr);
////////////////////////////////////////////////////////////////////////////////
/// Get demod pre Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetPreViterbiBer(float *ber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetPreViterbiBer(DMD_ATSC_BER_DATA *ber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod post Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetPostViterbiBer(float *ber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetPostViterbiBer(DMD_ATSC_BER_DATA *ber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod frequency offset
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_ReadFrequencyOffset(MS_S16 *cfo);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_ReadFrequencyOffset(DMD_ATSC_CFO_DATA *cfo);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Set TS output mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetSerialControl(MS_U8 u8TsConfigData);
////////////////////////////////////////////////////////////////////////////////
/// Enable I2C bypass mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_IIC_BYPASS_MODE(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Switch pin to SSPI or GPIO (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SWITCH_SSPI_GPIO(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Get GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin output or input (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut);
////////////////////////////////////////////////////////////////////////////////
/// Swap ADC input (usually for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_DoIQSwap(MS_BOOL bIsQPad);

////////////////////////////////////////////////////////////////////////////////
/// To get ATSC's register  value, only for special purpose.\n
/// u16Addr       : the address of ATSC's register\n
/// pu8Data       : the value to be gotten\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set ATSC's register value, only for special purpose.\n
/// u16Addr       : the address of ATSC's register\n
/// u8Value       : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetReg(MS_U16 u16Addr, MS_U8 u8Data);

////////////////////////////////////////////////////////////////////////////////
/// Set Digital RF frequency KHz (only for Digital RF)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SET_DIGRF_FREQ(MS_U32 u32RfFreq);

////////////////////////////////////////////////////////////////////////////////
///                      BACKWARD COMPATIBLE API                             ///
////////////////////////////////////////////////////////////////////////////////

#ifndef UTPA2
////////////////////////////////////////////////////////////////////////////////
/// SEL DEMOD (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SEL_DMD(eDMD_SEL eDMD_NUM);
////////////////////////////////////////////////////////////////////////////////
/// Load FW (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_LoadFW(eDMD_SEL DMD_NUM);
////////////////////////////////////////////////////////////////////////////////
/// Set VSB mode (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_SetVsbMode(void);
////////////////////////////////////////////////////////////////////////////////
/// Set 256QAM mode (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Set256QamMode(void);
////////////////////////////////////////////////////////////////////////////////
/// Set 64QAM mode (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_Set64QamMode(void);
#endif

#endif // #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN

////////////////////////////////////////////////////////////////////////////////
///                            MULTI DEMOD API                               ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when enter DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_Init(MS_U8 id, DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when exit DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_Exit(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Get Initial Data
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U32 MDrv_DMD_ATSC_MD_GetConfig(MS_U8 id, DMD_ATSC_InitData *psDMD_ATSC_InitData);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SetConfig(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Reset FW state machine (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SetReset(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod (only for J83B)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_Set_QAM_SR(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate);
////////////////////////////////////////////////////////////////////////////////
/// Active demod (not use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SetActive(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Set demod power state for STR
////////////////////////////////////////////////////////////////////////////////
#if DMD_ATSC_STR_EN
DLL_PUBLIC extern MS_U32 MDrv_DMD_ATSC_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod lock status
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ATSC_LOCK_STATUS MDrv_DMD_ATSC_MD_GetLock(MS_U8 id, DMD_ATSC_GETLOCK_TYPE eType);
////////////////////////////////////////////////////////////////////////////////
/// Get demod modulation mode
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ATSC_DEMOD_TYPE MDrv_DMD_ATSC_MD_GetModulationMode(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal strength (IF AGC gain)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal quality (NO SIGNAL, WEAK, MODERATE, STRONG and VERY_STRONG)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ATSC_SIGNAL_CONDITION MDrv_DMD_ATSC_MD_GetSignalQuality(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Get demod SNR percentage (MAX SNR 40dB)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U8 MDrv_DMD_ATSC_MD_GetSNRPercentage(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Get demod SNR (only for J.83ABC)
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GET_QAM_SNR(MS_U8 id, float *f_snr);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GET_QAM_SNR(MS_U8 id, DMD_ATSC_SNR_DATA *f_snr);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod packet error number
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_Read_uCPKT_ERR(MS_U8 id, MS_U16 *u16PacketErr);
////////////////////////////////////////////////////////////////////////////////
/// Get demod pre Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GetPreViterbiBer(MS_U8 id, float *ber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GetPreViterbiBer(MS_U8 id, DMD_ATSC_BER_DATA *ber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod post Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GetPostViterbiBer(MS_U8 id, float *ber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GetPostViterbiBer(MS_U8 id, DMD_ATSC_BER_DATA *ber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod frequency offset
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_ReadFrequencyOffset(MS_U8 id, MS_S16 *cfo);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_ReadFrequencyOffset(MS_U8 id, DMD_ATSC_CFO_DATA *cfo);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Set TS output mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData);
////////////////////////////////////////////////////////////////////////////////
/// Enable I2C bypass mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Switch pin to SSPI or GPIO (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Get GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin output or input (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut);
////////////////////////////////////////////////////////////////////////////////
/// Swap ADC input (usually for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad);

////////////////////////////////////////////////////////////////////////////////
/// To get ATSC's register  value, only for special purpose.\n
/// u16Addr       : the address of ATSC's register\n
/// pu8Data       : the value to be gotten\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set ATSC's register value, only for special purpose.\n
/// u16Addr       : the address of ATSC's register\n
/// u8Value       : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data);

////////////////////////////////////////////////////////////////////////////////
/// Set Digital RF frequency KHz (only for Digital RF)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ATSC_MD_SET_DIGRF_FREQ(MS_U8 id, MS_U32 u32RfFreq);

#ifdef UTPA2
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_SetDbgLevel(DMD_ATSC_DbgLv u8DbgLevel);
DLL_PUBLIC extern DMD_ATSC_Info* _MDrv_DMD_ATSC_GetInfo(void);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_GetLibVer(const MSIF_Version **ppVersion);

DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_Init(MS_U8 id, DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_Exit(MS_U8 id);
DLL_PUBLIC extern MS_U32 _MDrv_DMD_ATSC_MD_GetConfig(MS_U8 id, DMD_ATSC_InitData *psDMD_ATSC_InitData);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SetConfig(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SetReset(MS_U8 id);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_Set_QAM_SR(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SetActive(MS_U8 id, MS_BOOL bEnable);
#if DMD_ATSC_STR_EN
DLL_PUBLIC extern MS_U32 _MDrv_DMD_ATSC_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState);
#endif
DLL_PUBLIC extern DMD_ATSC_LOCK_STATUS _MDrv_DMD_ATSC_MD_GetLock(MS_U8 id, DMD_ATSC_GETLOCK_TYPE eType);
DLL_PUBLIC extern DMD_ATSC_DEMOD_TYPE _MDrv_DMD_ATSC_MD_GetModulationMode(MS_U8 id);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength);
DLL_PUBLIC extern DMD_ATSC_SIGNAL_CONDITION _MDrv_DMD_ATSC_MD_GetSignalQuality(MS_U8 id);
DLL_PUBLIC extern MS_U8 _MDrv_DMD_ATSC_MD_GetSNRPercentage(MS_U8 id);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GET_QAM_SNR(MS_U8 id, DMD_ATSC_SNR_DATA *f_snr);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_Read_uCPKT_ERR(MS_U8 id, MS_U16 *u16PacketErr);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GetPreViterbiBer(MS_U8 id, DMD_ATSC_BER_DATA *ber);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GetPostViterbiBer(MS_U8 id, DMD_ATSC_BER_DATA *ber);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_ReadFrequencyOffset(MS_U8 id, DMD_ATSC_CFO_DATA *cfo);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data);

DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ATSC_MD_SET_DIGRF_FREQ(MS_U8 id, MS_U32 u32RfFreq);
#endif // #ifdef UTPA2

#ifdef __cplusplus
}
#endif

#endif // _DRV_ATSC_H_
