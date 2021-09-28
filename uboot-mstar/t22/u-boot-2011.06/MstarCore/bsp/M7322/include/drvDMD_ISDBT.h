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
/// @brief  ISDBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ISDBT_H_
#define _DRV_ISDBT_H_

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#if !defined UTPA2
#define DMD_ISDBT_UTOPIA_EN                  1
#define DMD_ISDBT_UTOPIA2_EN                 0
#else
#define DMD_ISDBT_UTOPIA_EN                  0
#define DMD_ISDBT_UTOPIA2_EN                 1
#endif

#define DMD_ISDBT_STR_EN                     1
#define DMD_ISDBT_MULTI_THREAD_SAFE          0
#define DMD_ISDBT_MULTI_DMD_EN               1
#define DMD_ISDBT_EXTDMD_EN                  1
#define DMD_ISDBT_3PARTY_EN                  0
#define DMD_ISDBT_3PARTY_IN_KERNEL_EN        0
#define DMD_ISDBT_3PARTY_MSPI_EN             0

#if DMD_ISDBT_3PARTY_EN
#undef DMD_ISDBT_UTOPIA_EN
#undef DMD_ISDBT_UTOPIA2_EN

#define DMD_ISDBT_UTOPIA_EN                  0
#define DMD_ISDBT_UTOPIA2_EN                 0

#if DMD_ISDBT_3PARTY_IN_KERNEL_EN
 #ifndef UTPA2
  #define UTPA2
 #endif
 #ifndef MSOS_TYPE_LINUX_KERNEL
  #define MSOS_TYPE_LINUX_KERNEL
 #endif
#endif // #if DMD_ISDBT_3PARTY_IN_KERNEL_EN
#endif // #if DMD_ISDBT_3PARTY_EN

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsTypes.h"
#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#ifndef MSIF_TAG
#include "MsVersion.h"
#endif
#include "MsCommon.h"
#endif
#if DMD_ISDBT_UTOPIA2_EN || (DMD_ISDBT_STR_EN && DMD_ISDBT_UTOPIA_EN)
#include "utopia.h"
#endif

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#include "drvMSPI.h"
#include "drvMIU.h"
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
#if DMD_ISDBT_3PARTY_EN
// Please define PRINT according to 3 party platform
#define PRINT    printf
#else // #if DMD_ISDBT_3PARTY_EN
#define PRINT    printf
#endif // #if DMD_ISDBT_3PARTY_EN
#endif // #ifdef MSOS_TYPE_LINUX_KERNEL

#if DMD_ISDBT_MULTI_DMD_EN
#define DMD_ISDBT_MAX_DEMOD_NUM          2
#else
#define DMD_ISDBT_MAX_DEMOD_NUM          1
#endif

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#define MSIF_DMD_ISDBT_LIB_CODE          {'D','M','D','_','I','S','D','B','T','_'} //Lib code
#define MSIF_DMD_ISDBT_LIBVER            {'0','0'}                                 //LIB version
#define MSIF_DMD_ISDBT_BUILDNUM          {'0','0'}                                 //Build Number
#define MSIF_DMD_ISDBT_CHANGELIST        {'0','0','0','0','0','0','0','0'}         //P4 ChangeList Number

#define DMD_ISDBT_VER                    /* Character String for DRV/API version             */  \
    MSIF_TAG,                            /* 'MSIF'                                           */  \
    MSIF_CLASS,                          /* '00'                                             */  \
    MSIF_CUS,                            /* 0x0000                                           */  \
    MSIF_MOD,                            /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                   \
    MSIF_CPU,                                                                                    \
    MSIF_DMD_ISDBT_LIB_CODE,             /* IP__                                             */  \
    MSIF_DMD_ISDBT_LIBVER,               /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_ISDBT_BUILDNUM,             /* 00 ~ 99                                          */  \
    MSIF_DMD_ISDBT_CHANGELIST,           /* CL#                                              */  \
    MSIF_OS
#endif // #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN

#ifndef BIT_
#define BIT_(n)                             (1 << (n))
#endif

#define DMD_ISDBT_LOCK_FSA_TRACK_LOCK       BIT_(0)
#define DMD_ISDBT_LOCK_PSYNC_LOCK           BIT_(1)
#define DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK   BIT_(2)
#define DMD_ISDBT_LOCK_FEC_LOCK             BIT_(3)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    DMD_ISDBT_HAL_CMD_Exit = 0,
    DMD_ISDBT_HAL_CMD_InitClk,
    DMD_ISDBT_HAL_CMD_Download,
    DMD_ISDBT_HAL_CMD_FWVERSION,
    DMD_ISDBT_HAL_CMD_SoftReset,
    DMD_ISDBT_HAL_CMD_SetACICoef,
    DMD_ISDBT_HAL_CMD_SetISDBTMode,
    DMD_ISDBT_HAL_CMD_SetModeClean,
    DMD_ISDBT_HAL_CMD_Active,
    DMD_ISDBT_HAL_CMD_Check_FEC_Lock,
    DMD_ISDBT_HAL_CMD_Check_FSA_TRACK_Lock,
    DMD_ISDBT_HAL_CMD_Check_PSYNC_Lock,
    DMD_ISDBT_HAL_CMD_Check_ICFO_CH_EXIST_Lock,
    DMD_ISDBT_HAL_CMD_GetSignalCodeRate,
    DMD_ISDBT_HAL_CMD_GetSignalGuardInterval,
    DMD_ISDBT_HAL_CMD_GetSignalTimeInterleaving,
    DMD_ISDBT_HAL_CMD_GetSignalFFTValue,
    DMD_ISDBT_HAL_CMD_GetSignalModulation,
    DMD_ISDBT_HAL_CMD_ReadIFAGC,
    DMD_ISDBT_HAL_CMD_GetFreqOffset,
    DMD_ISDBT_HAL_CMD_GetSignalQuality,
    DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerA,
    DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerB,
    DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerC,
    DMD_ISDBT_HAL_CMD_GetSignalQualityCombine,
    DMD_ISDBT_HAL_CMD_GetSNR,
    DMD_ISDBT_HAL_CMD_GetPreViterbiBer,
    DMD_ISDBT_HAL_CMD_GetPostViterbiBer,
    DMD_ISDBT_HAL_CMD_Read_PKT_ERR,
    DMD_ISDBT_HAL_CMD_TS_INTERFACE_CONFIG,
    DMD_ISDBT_HAL_CMD_IIC_Bypass_Mode,
    DMD_ISDBT_HAL_CMD_SSPI_TO_GPIO,
    DMD_ISDBT_HAL_CMD_GPIO_GET_LEVEL,
    DMD_ISDBT_HAL_CMD_GPIO_SET_LEVEL,
    DMD_ISDBT_HAL_CMD_GPIO_OUT_ENABLE,
    DMD_ISDBT_HAL_CMD_DoIQSwap,
    DMD_ISDBT_HAL_CMD_GET_REG,
    DMD_ISDBT_HAL_CMD_SET_REG
} DMD_ISDBT_HAL_COMMAND;

typedef enum
{
    E_ISDBT_Layer_A = 0x00,
    E_ISDBT_Layer_B = 0x01,
    E_ISDBT_Layer_C = 0x02,
    E_ISDBT_Layer_INVALID,
} EN_ISDBT_Layer;

typedef enum
{
    E_ISDBT_FFT_2K = 0x00,  /// 2K
    E_ISDBT_FFT_4K = 0x01,  /// 4k
    E_ISDBT_FFT_8K = 0x02,  /// 8k
    E_ISDBT_FFT_INVALID,    /// invalid indicator
} EN_ISDBT_FFT_VAL;

typedef enum
{
    E_ISDBT_DQPSK   = 0,   /// DQPSK
    E_ISDBT_QPSK    = 1,   /// QPSK
    E_ISDBT_16QAM   = 2,   /// 16QAM
    E_ISDBT_64QAM   = 3,   /// 64QAM
    E_ISDBT_QAM_INVALID,   /// invalid indicator
} EN_ISDBT_CONSTEL_TYPE;

typedef enum
{
    E_ISDBT_CODERATE_1_2 = 0,   /// 1/2
    E_ISDBT_CODERATE_2_3 = 1,   /// 2/3
    E_ISDBT_CODERATE_3_4 = 2,   /// 3/4
    E_ISDBT_CODERATE_5_6 = 3,   /// 5/6
    E_ISDBT_CODERATE_7_8 = 4,   /// 7/8
    E_ISDBT_CODERATE_INVALID,   /// invalid indicator
} EN_ISDBT_CODE_RATE;

typedef enum
{
    E_ISDBT_GUARD_INTERVAL_1_4  = 0,   /// 1/4
    E_ISDBT_GUARD_INTERVAL_1_8  = 1,   /// 1/8
    E_ISDBT_GUARD_INTERVAL_1_16 = 2,   /// 1/16
    E_ISDBT_GUARD_INTERVAL_1_32 = 3,   /// 1/32
    E_ISDBT_GUARD_INTERVAL_INVALID,    /// invalid indicator
} EN_ISDBT_GUARD_INTERVAL;

typedef enum
{
    // 2K mode
    E_ISDBT_2K_TDI_0 = 0,   /// Tdi = 0
    E_ISDBT_2K_TDI_4 = 1,   /// Tdi = 4
    E_ISDBT_2K_TDI_8 = 2,   /// Tdi = 8
    E_ISDBT_2K_TDI_16 = 3,  /// Tdi = 16
    // 4K mode
    E_ISDBT_4K_TDI_0 = 4,   /// Tdi = 0
    E_ISDBT_4K_TDI_2 = 5,   /// Tdi = 2
    E_ISDBT_4K_TDI_4 = 6,   /// Tdi = 4
    E_ISDBT_4K_TDI_8 = 7,   /// Tdi = 8
    // 8K mode
    E_ISDBT_8K_TDI_0 = 8,   /// Tdi = 0
    E_ISDBT_8K_TDI_1 = 9,   /// Tdi = 1
    E_ISDBT_8K_TDI_2 = 10,  /// Tdi = 2
    E_ISDBT_8K_TDI_4 = 11,  /// Tdi = 4
    E_ISDBT_TDI_INVALID,    /// invalid indicator
} EN_ISDBT_TIME_INTERLEAVING;

typedef struct DLL_PACKED _ISDBT_MODULATION_MODE
{
    EN_ISDBT_CODE_RATE         eIsdbtCodeRate;
    EN_ISDBT_GUARD_INTERVAL    eIsdbtGI;
    EN_ISDBT_TIME_INTERLEAVING eIsdbtTDI;
    EN_ISDBT_FFT_VAL           eIsdbtFFT;
    EN_ISDBT_CONSTEL_TYPE      eIsdbtConstellation;
} sISDBT_MODULATION_MODE;

typedef struct DLL_PACKED _DMD_ISDBT_GET_MODULATION
{
    EN_ISDBT_Layer        eIsdbtLayer;
    EN_ISDBT_CONSTEL_TYPE eConstellation;
} DMD_ISDBT_GET_MODULATION;

typedef struct DLL_PACKED _DMD_ISDBT_GET_CodeRate
{
    EN_ISDBT_Layer     eIsdbtLayer;
    EN_ISDBT_CODE_RATE eCodeRate;
} DMD_ISDBT_GET_CodeRate;

typedef struct DLL_PACKED _DMD_ISDBT_GET_TimeInterleaving
{
    EN_ISDBT_Layer             eIsdbtLayer;
    EN_ISDBT_TIME_INTERLEAVING eTimeInterleaving;
} DMD_ISDBT_GET_TimeInterleaving;

typedef struct DLL_PACKED _DMD_ISDBT_GET_BER_VALUE
{
    EN_ISDBT_Layer eIsdbtLayer;
    #ifdef UTPA2
    MS_U32 BerValue;
    MS_U16 BerPeriod;
    #else
    float  fBerValue;
    #endif
} DMD_ISDBT_GET_BER_VALUE;

typedef struct DLL_PACKED _DMD_ISDBT_GET_PKT_ERR
{
    EN_ISDBT_Layer eIsdbtLayer;
    MS_U16         u16PacketErr;
} DMD_ISDBT_GET_PKT_ERR;

typedef struct DLL_PACKED
{
    MS_U8  FFT_Mode;
    MS_S32 TdCfoRegValue;
    MS_S32 FdCfoRegValue;
    MS_S16 IcfoRegValue;
} DMD_ISDBT_CFO_DATA;

typedef struct DLL_PACKED
{
    MS_U32 RegSNR;
    MS_U16 RegSnrObsNum;
} DMD_ISDBT_SNR_DATA;

typedef struct DLL_PACKED
{
    MS_U8 u8Pin;
    union
    {
        MS_BOOL bLevel;
        MS_BOOL bIsOut;
    };
} DMD_ISDBT_GPIO_PIN_DATA;

typedef struct DLL_PACKED
{
    MS_U16 u16Addr;
    MS_U8  u8Data;
} DMD_ISDBT_REG_DATA;

typedef enum
{
    DMD_ISDBT_DBGLV_NONE,    // disable all the debug message
    DMD_ISDBT_DBGLV_INFO,    // information
    DMD_ISDBT_DBGLV_NOTICE,  // normal but significant condition
    DMD_ISDBT_DBGLV_WARNING, // warning conditions
    DMD_ISDBT_DBGLV_ERR,     // error conditions
    DMD_ISDBT_DBGLV_CRIT,    // critical conditions
    DMD_ISDBT_DBGLV_ALERT,   // action must be taken immediately
    DMD_ISDBT_DBGLV_EMERG,   // system is unusable
    DMD_ISDBT_DBGLV_DEBUG,   // debug-level messages
} DMD_ISDBT_DbgLv;

typedef enum
{   DMD_ISDBT_DEMOD,
    DMD_ISDBT_DEMOD_6M = DMD_ISDBT_DEMOD,
    DMD_ISDBT_DEMOD_7M,
    DMD_ISDBT_DEMOD_8M,
    DMD_ISDBT_DEMOD_MAX,
    DMD_ISDBT_DEMOD_NULL = DMD_ISDBT_DEMOD_MAX,
} DMD_ISDBT_DEMOD_TYPE;

typedef enum
{
    DMD_ISDBT_GETLOCK,
    DMD_ISDBT_GETLOCK_FSA_TRACK_LOCK,
    DMD_ISDBT_GETLOCK_PSYNC_LOCK,
    DMD_ISDBT_GETLOCK_ICFO_CH_EXIST_LOCK,
    DMD_ISDBT_GETLOCK_FEC_LOCK
} DMD_ISDBT_GETLOCK_TYPE;

typedef enum
{
    DMD_ISDBT_LOCK,
    DMD_ISDBT_CHECKING,
    DMD_ISDBT_CHECKEND,
    DMD_ISDBT_UNLOCK,
    DMD_ISDBT_NULL,
} DMD_ISDBT_LOCK_STATUS;

#if (DMD_ISDBT_STR_EN && !DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN)
typedef enum
{
    E_POWER_SUSPEND     = 1,    // State is backup in DRAM, components power off.
    E_POWER_RESUME      = 2,    // Resume from Suspend or Hibernate mode
    E_POWER_MECHANICAL  = 3,    // Full power off mode. System return to working state only after full reboot 
    E_POWER_SOFT_OFF    = 4,    // The system appears to be off, but some components remain powered for trigging boot-up.
} EN_POWER_MODE;
#endif

/// For demod init
typedef struct DLL_PACKED
{
    // init
    MS_U16 u16ISDBTIcfoChExistCheckTime;
    MS_U16 u16ISDBTFECLockCheckTime;

    // register init
    MS_U8 *u8DMD_ISDBT_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_ISDBT_DSPRegInitSize;
    MS_U8 *u8DMD_ISDBT_InitExt; // TODO use system variable type

    //By Tuners:
    MS_U16  u16IF_KHZ;//By Tuners
    MS_BOOL bIQSwap;//0
    MS_U16  u16AgcReferenceValue;//0
    MS_BOOL bTunerGainInvert;//0

    //By IC:
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

    //By SYS memory mapping (Only for int demod):
    MS_PHY u32TdiStartAddr;
} DMD_ISDBT_InitData;

typedef struct ISDBT_IOCTL_DATA DMD_ISDBT_IOCTL_DATA;

typedef struct DLL_PACKED
{
    MS_VIRT virtDMDBaseAddr;

    MS_BOOL bInit;
    MS_BOOL bDownloaded;
    
    #if DMD_ISDBT_STR_EN
    MS_BOOL              bIsDTV;
    EN_POWER_MODE        eLastState;
    #endif
    DMD_ISDBT_DEMOD_TYPE eLastType;
    
    MS_BOOL bIsQPad;
    
    MS_U32 tdi_dram_size;

    MS_BOOL (*HAL_DMD_ISDBT_IOCTL_CMD)(DMD_ISDBT_IOCTL_DATA *pData, DMD_ISDBT_HAL_COMMAND eCmd, void *pPara);
    MS_BOOL (*HAL_DMD_ISDBT_IOCTL_CMD1)(DMD_ISDBT_HAL_COMMAND eCmd, void *pPara);

    #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
    void (*DelayMS)(MS_U32 u32Ms);
    
    MSPI_ErrorNo (*MSPI_Init_Ext)(MS_U8 u8HWNum);
    void (*MSPI_SlaveEnable)(MS_BOOL Enable);
    MSPI_ErrorNo (*MSPI_Write)(MS_U8 *pData, MS_U16 u16Size);
    MSPI_ErrorNo (*MSPI_Read)(MS_U8 *pData, MS_U16 u16Size);

    MS_BOOL (*MIU_IsSupportMIU1)(void);
    MS_BOOL (*MIU_SelMIU)(eMIUClientID eClientID, eMIU_SelType eType);
    void (*MIU_MaskReq)(MS_U8 u8Miu, eMIUClientID eClientID);
    void (*MIU_UnMaskReq)(MS_U8 u8Miu, eMIUClientID eClientID);
    #endif	
} DMD_ISDBT_PriData;

typedef struct DLL_PACKED
{
    MS_U8  u8Version;
    MS_U32 u32ISDBTScanTimeStart;
    MS_U32 u32ISDBTFECLockTime;
    MS_U32 u32ISDBTLockStatus;
} DMD_ISDBT_Info;

typedef struct DLL_PACKED
{
    DMD_ISDBT_InitData  sDMD_ISDBT_InitData;
    DMD_ISDBT_PriData   sDMD_ISDBT_PriData;
    DMD_ISDBT_Info      sDMD_ISDBT_Info;
} DMD_ISDBT_ResData;

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
typedef struct DLL_PACKED
{
    void (*DelayMS)(MS_U32 u32Ms);
    MSPI_ErrorNo (*MSPI_Init_Ext)(MS_U8 u8HWNum);
    void (*MSPI_SlaveEnable)(MS_BOOL Enable);
    MSPI_ErrorNo (*MSPI_Write)(MS_U8 *pData, MS_U16 u16Size);
    MSPI_ErrorNo (*MSPI_Read)(MS_U8 *pData, MS_U16 u16Size);
    MS_BOOL (*MIU_IsSupportMIU1)(void);
    MS_BOOL (*MIU_SelMIU)(eMIUClientID eClientID, eMIU_SelType eType);
    void (*MIU_MaskReq)(MS_U8 u8Miu, eMIUClientID eClientID);
    void (*MIU_UnMaskReq)(MS_U8 u8Miu, eMIUClientID eClientID);
} DMD_ISDBT_SYS_PTR;
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
} DMD_ISDBT_SYS_PTR;
#endif
    
struct DLL_PACKED ISDBT_IOCTL_DATA
{
    MS_U8              id;
    DMD_ISDBT_ResData *pRes;
    DMD_ISDBT_SYS_PTR  sys;
};

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// Set detailed level of ISDBT driver debug message
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
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of ISDBT driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern DMD_ISDBT_Info* MDrv_DMD_ISDBT_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// Get ISDBT driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion);

////////////////////////////////////////////////////////////////////////////////
/// Should be called once when power on init
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Initial_Hal_Interface(void);  

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN

////////////////////////////////////////////////////////////////////////////////
///                            SINGLE DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when enter DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Init(DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when exit DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Exit(void);
////////////////////////////////////////////////////////////////////////////////
/// Get Initial Data
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U32 MDrv_DMD_ISDBT_GetConfig(DMD_ISDBT_InitData *psDMD_ISDBT_InitData);
////////////////////////////////////////////////////////////////////////////////
/// Get demod FW version (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetFwVer(MS_U16 *pu16FwVer);
////////////////////////////////////////////////////////////////////////////////
/// Reset demod (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern void MDrv_DMD_ISDBT_SetReset(void);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_AdvSetConfig(DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetConfig(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Active demod (not used)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetActive(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Set demod power state for STR
////////////////////////////////////////////////////////////////////////////////
#if DMD_ISDBT_STR_EN
DLL_PUBLIC extern MS_U32 MDrv_DMD_ISDBT_SetPowerState(EN_POWER_MODE u16PowerState);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod lock status
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK_TYPE eType);
////////////////////////////////////////////////////////////////////////////////
/// Get demod modulation mode
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetModulationMode(EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode);
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal strength (IF AGC gain)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetSignalStrength(MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// Get demod frequency offset
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetFreqOffset(float *pFreqOff);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetFreqOffset(DMD_ISDBT_CFO_DATA *pFreqOff);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal quality (post Viterbi BER)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQuality(void);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerA(void);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerB(void);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerC(void);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityCombine(void);
////////////////////////////////////////////////////////////////////////////////
/// Get demod SNR
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetSNR(float *f_snr);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetSNR(DMD_ISDBT_SNR_DATA *f_snr);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod pre Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer eLayerIndex, float *fber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer eLayerIndex, DMD_ISDBT_GET_BER_VALUE *fber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod post Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer eLayerIndex, float *fber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer eLayerIndex, DMD_ISDBT_GET_BER_VALUE *fber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod packet error
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Read_PKT_ERR(EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr);
////////////////////////////////////////////////////////////////////////////////
/// Set TS output mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetSerialControl(MS_U8 u8TsConfigData);
////////////////////////////////////////////////////////////////////////////////
/// Enable I2C bypass mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_IIC_BYPASS_MODE(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Switch pin to SSPI or GPIO (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SWITCH_SSPI_GPIO(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Get GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin output or input (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut);
////////////////////////////////////////////////////////////////////////////////
/// Swap ADC input (usually for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_DoIQSwap(MS_BOOL bIsQPad);

////////////////////////////////////////////////////////////////////////////////
/// To get ISDBT's register  value, only for special purpose.\n
/// u16Addr       : the address of ISDBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set ISDBT's register value, only for special purpose.\n
/// u16Addr       : the address of ISDBT's register\n
/// u8Value       : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data);

#endif // #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN

////////////////////////////////////////////////////////////////////////////////
///                            MULTI DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when enter DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_Init(MS_U8 id, DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called every time when exit DTV input source
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_Exit(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Get Initial Data
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U32 MDrv_DMD_ISDBT_MD_GetConfig(MS_U8 id, DMD_ISDBT_InitData *psDMD_ISDBT_InitData);  
////////////////////////////////////////////////////////////////////////////////
/// Get demod FW version (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetFwVer(MS_U8 id, MS_U16 *pu16FwVer);
////////////////////////////////////////////////////////////////////////////////
/// Reset demod (no use)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern void MDrv_DMD_ISDBT_MD_SetReset(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_AdvSetConfig(MS_U8 id, DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Set demod mode and enable demod
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetConfig(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Active demod (not used)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetActive(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Set demod power state for STR
////////////////////////////////////////////////////////////////////////////////
#if DMD_ISDBT_STR_EN
DLL_PUBLIC extern MS_U32 MDrv_DMD_ISDBT_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod lock status
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_MD_GetLock(MS_U8 id, DMD_ISDBT_GETLOCK_TYPE eType);
////////////////////////////////////////////////////////////////////////////////
/// Get demod modulation mode
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetModulationMode(MS_U8 id, EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode);
////////////////////////////////////////////////////////////////////////////////
/// Get demod signal strength (IF AGC gain)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// Get demod frequency offset
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, float *pFreqOff);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, DMD_ISDBT_CFO_DATA *pFreqOff);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get signal quality (post Viterbi BER)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQuality(MS_U8 id);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(MS_U8 id);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(MS_U8 id);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(MS_U8 id);
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(MS_U8 id);
////////////////////////////////////////////////////////////////////////////////
/// Get demod SNR
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, float *f_snr);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, DMD_ISDBT_SNR_DATA *f_snr);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod pre Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, DMD_ISDBT_GET_BER_VALUE *fber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod post Viterbi BER
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber);
#else
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, DMD_ISDBT_GET_BER_VALUE *fber);
#endif
////////////////////////////////////////////////////////////////////////////////
/// Get demod packet error
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_Read_PKT_ERR(MS_U8 id, EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr);
////////////////////////////////////////////////////////////////////////////////
/// Set TS output mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData);
////////////////////////////////////////////////////////////////////////////////
/// Enable I2C bypass mode (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Switch pin to SSPI or GPIO (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// Get GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin high or low (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel);
////////////////////////////////////////////////////////////////////////////////
/// Set GPIO pin output or input (only for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut);
////////////////////////////////////////////////////////////////////////////////
/// Swap ADC input (usually for external demod)
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad);

////////////////////////////////////////////////////////////////////////////////
/// To get ISDBT's register  value, only for special purpose.\n
/// u16Addr       : the address of ISDBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set ISDBT's register value, only for special purpose.\n
/// u16Addr       : the address of ISDBT's register\n
/// u8Value       : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data);

#ifdef UTPA2
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel);
DLL_PUBLIC extern DMD_ISDBT_Info* _MDrv_DMD_ISDBT_GetInfo(void);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_Init(MS_U8 id, DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_Exit(MS_U8 id);
DLL_PUBLIC extern MS_U32 _MDrv_DMD_ISDBT_MD_GetConfig(MS_U8 id, DMD_ISDBT_InitData *psDMD_ISDBT_InitData);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_AdvSetConfig(MS_U8 id, DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable);
#if DMD_ISDBT_STR_EN
DLL_PUBLIC extern MS_U32 _MDrv_DMD_ISDBT_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState);
#endif
DLL_PUBLIC extern DMD_ISDBT_LOCK_STATUS _MDrv_DMD_ISDBT_MD_GetLock(MS_U8 id, DMD_ISDBT_GETLOCK_TYPE eType);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetModulationMode(MS_U8 id, EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, DMD_ISDBT_CFO_DATA *cfo);
DLL_PUBLIC extern MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQuality(MS_U8 id);
DLL_PUBLIC extern MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(MS_U8 id);
DLL_PUBLIC extern MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(MS_U8 id);
DLL_PUBLIC extern MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(MS_U8 id);
DLL_PUBLIC extern MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(MS_U8 id);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, DMD_ISDBT_SNR_DATA *f_snr);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, DMD_ISDBT_GET_BER_VALUE *ber);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, DMD_ISDBT_GET_BER_VALUE *ber);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_Read_PKT_ERR(MS_U8 id, EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data);
DLL_PUBLIC extern MS_BOOL _MDrv_DMD_ISDBT_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data);
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_ISDBT_H_
