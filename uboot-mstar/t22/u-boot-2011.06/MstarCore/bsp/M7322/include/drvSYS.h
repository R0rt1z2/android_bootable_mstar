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
/// @brief  System Control Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SYS SYS interface
    \ingroup  G_PERIPHERAL

    \brief

    This drvSYS.h provide functions like WDT, FlushMemory, and Chip-Related utility.

    <b>Features</b>

    - WDT control function
    - System Memory control function
    - System Chip-Related function

    <b> Operation Code Flow: </b> \n
    -# Prepare SYS setting for each operation
    -# Set and start SYS in command handle
    -# Trigger SYS

     \defgroup G_SYS_INIT Initialization Task relative
     \ingroup  G_SYS
     \defgroup G_SYS_COMMON Common Task relative
     \ingroup  G_SYS
     \defgroup G_SYS_CONTROL Control relative
     \ingroup  G_SYS
     \defgroup G_SYS_OTHER  other relative
     \ingroup  G_SYS
*/

#ifndef _DRV_SYS_H_
#define _DRV_SYS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#include "UFO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define MDRV_SYS_XC_TIMING_VERSION        0x00000001UL
#define MDRV_SYS_XC_BYPART_TIMING_VERSION 0x00000001UL

#define SYS_ENCODER_MFE		0x00000001UL
#define SYS_ENCODER_VE		0x00000002UL

#define SYS_FRONTEND_AVD	0x00000001UL
#define SYS_FRONTEND_DEMOD	0x00000002UL
#define SYS_FRONTEND_VBI	0x00000004UL
#define SYS_FRONTEND_VIF	0x00000008UL

#define SYS_SYSTEM_CEC		0x00000001UL
#define SYS_SYSTEM_MBX		0x00000002UL
#define SYS_SYSTEM_SWI2C	0x00000004UL
#define SYS_SYSTEM_BDMA		0x00000008UL
#define SYS_SYSTEM_CPU		0x00000010UL
#define SYS_SYSTEM_GPIO		0x00000020UL
#define SYS_SYSTEM_HWI2C	0x00000040UL
#define SYS_SYSTEM_IR		0x00000080UL
#define SYS_SYSTEM_MIU		0x00000100UL
#define SYS_SYSTEM_MPIF		0x00000200UL
#define SYS_SYSTEM_MSPI		0x00000400UL
#define SYS_SYSTEM_PM		0x00000800UL
#define SYS_SYSTEM_PWM		0x00001000UL
#define SYS_SYSTEM_PWS		0x00002000UL
#define SYS_SYSTEM_RTC		0x00004000UL
#define SYS_SYSTEM_SAR		0x00008000UL
#define SYS_SYSTEM_URDMA	0x00010000UL
#define SYS_SYSTEM_WDT		0x00020000UL

#define SYS_CA_AESDMA		0x00000001UL
#define SYS_CA_CA			0x00000002UL
#define SYS_CA_PCMCIA		0x00000004UL
#define SYS_CA_SC			0x00000008UL

#define SYS_CODEC_GPD		0x00000001UL
#define SYS_CODEC_JPEG		0x00000002UL
#define SYS_CODEC_VDEC		0x00000004UL

#define SYS_DISPLAY_ACE		0x00000001UL
#define SYS_DISPLAY_DAC		0x00000002UL
#define SYS_DISPLAY_DDC2BI	0x00000004UL
#define SYS_DISPLAY_DIP		0x00000008UL
#define SYS_DISPLAY_DLC		0x00000010UL
#define SYS_DISPLAY_GOP		0x00000020UL
#define SYS_DISPLAY_HDMITX	0x00000040UL
#define SYS_DISPLAY_MHL		0x00000080UL
#define SYS_DISPLAY_MVOP	0x00000100UL
#define SYS_DISPLAY_PNL		0x00000200UL
#define SYS_DISPLAY_XC		0x00000400UL

#define SYS_GFX_GFX			0x00000001UL
#define SYS_AUDIO_AUDIO		0x00000001UL
#define SYS_STORAGE_SERFLASH	0x00000001UL
#define SYS_DEMUX_DMX		0x00000001UL

#define SYS_FW_NODE             "/Mstar-sys/firmware"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Version string.
    #define MSIF_SYS_LIB_CODE               {'S','Y','S','_'} //Lib code
    #define MSIF_SYS_LIBVER                 {'0','1'}      //LIB version
    #define MSIF_SYS_BUILDNUM               {'0','9' }    //Build Number
    #define MSIF_SYS_CHANGELIST             {'0','0','6','3','0','6','5','2'} //P4 ChangeList Number

#define SYS_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_SYS_LIB_CODE,                  /* IP__                                             */  \
    MSIF_SYS_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_SYS_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_SYS_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define SYS_LIB_NAME_MAX            16UL      ///< Maximum length of library name
#define SYS_LIB_TIME_MAX            16UL     ///< Maximum length of time
#define SYS_LIB_DATE_MAX            16UL      ///< Maximum length of date
#define SYS_BOARD_MODEL_MAX         32UL      ///< Maximum length of board name
#define SYS_PLATFORM_CHIP_MAX       32UL      ///< Maximum length of playform name
#define SYS_EFUSE_DID_LENGTH         4UL      ///< efuse device ID length
#define SYS_TEEWRP_NAME_LENGTH      32UL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// System information
typedef struct _SYS_Info
{
    struct
    {
        MS_U16    DeviceId;                           ///< Device ID
        MS_U8     Version;                            ///< Version
        MS_U8     Revision;                           ///< Revision
        MS_PHY  MIU1Base;                           ///< MIU1Base
        MS_PHY  MIU1BusBase;                        ///< MIU1Base
        MS_PHY  MIU2Base;                           ///< MIU2Base
        MS_PHY  MIU2BusBase;                        ///< MIU3Base
        MS_PHY  MIU3Base;                           ///< MIU2Base
        MS_PHY  MIU3BusBase;                        ///< MIU3Base
    } Chip;                                         ///< Chip information

    struct
    {
        MS_U8   Time[SYS_LIB_TIME_MAX];             ///< Library Built Time
        MS_U8   Date[SYS_LIB_DATE_MAX];             ///< Library Built Date
    } SWLib;                                        ///< Software Library Information

} SYS_Info;


typedef struct _SYS_Status
{
    MS_BOOL     WDT_Active;                         ///< Watchdog Timer

} SYS_Status;

typedef enum _SYS_DbgLv
{
    E_SYS_DBGLV_NONE           //no debug message
    ,E_SYS_DBGLV_ERR_ONLY      //show error only
    ,E_SYS_DBGLV_REG_DUMP      //show error & reg dump
    ,E_SYS_DBGLV_INFO          //show error & informaiton
    ,E_SYS_DBGLV_ALL           //show error, information & funciton name
}SYS_DbgLv;

typedef enum
{
    E_SYS_QUERY_H264_SUPPORTED = 0x00,
    E_SYS_QUERY_DVBC_SUPPORTED = 0x10,
    E_SYS_QUERY_MM_SUPPORTED = 0x11,
    E_SYS_QUERY_TTXNICAM_SUPPORTED = 0x12,
    E_SYS_QUERY_TCON_SUPPORTED = 0x13,
    E_SYS_QUERY_DTV_SUPPORTED = 0x14,
    E_SYS_QUERY_DTVCC_SUPPORTED = 0x15,
    E_SYS_QUERY_GINGA_NCL_SUPPORTED = 0x16,
    E_SYS_QUERY_GINGA_JAVA_SUPPORTED = 0x17,
    E_SYS_QUERT_HD_SUPPORTED = 0x18,
    E_SYS_QUERY_MHL_SUPPORTED = 0x19,
    E_SYS_QUERY_DVBT_SUPPORTED = 0x1A,
    E_SYS_QUERY_ISDBT_SUPPORTED = 0x1B,
    E_SYS_QUERY_HBBTV_SUPPORTED = 0x1C,
    E_SYS_QUERY_TTS_SUPPORTED = 0x1D,

    E_SYS_QUERY_DOLBY_HDR_SUPPORTED = 0x1E,
    E_SYS_QUERY_EVDHD_SUPPORTED = 0x1F,
    E_SYS_QUERY_VIDAC1_SUPPORTED = 0x20,
    E_SYS_QUERY_ETHTX_SUPPORTED = 0x21,
    E_SYS_QUERY_ETHRX_SUPPORTED = 0x22,
    E_SYS_QUERY_ETHTXOUTSWING_SUPPORTED = 0x23,
    E_SYS_QUERY_DVBT2_SUPPORTED = 0x24,
    E_SYS_QUERY_4K_SUPPORTED = 0x25,
    E_SYS_QUERY_TSIN_SUPPORTED = 0x26,
    E_SYS_QUERY_VP8_SUPPORTED = 0x27,
    E_SYS_QUERY_VP9_SUPPORTED = 0x28,
    E_SYS_QUERY_DVBS2_SUPPORTED = 0x29,
    E_SYS_QUERY_DD_SUPPORTED = 0x2A,
    E_SYS_QUERY_DDP_SUPPORTED = 0x2B,
    E_SYS_QUERY_BML_SUPPORTED = 0x2C,
    E_SYS_QUERY_SC_HD_SUPPORTED = 0x2D,
    E_SYS_QUERY_SC_FHD_SUPPORTED = 0x2E,
    E_SYS_QUERY_COMPONENT_SUPPORTED = 0x2F,
    E_SYS_QUERY_GINGA_SOLUTION_CONTROL1_SUPPORTED = 0x100,
    E_SYS_QUERY_GINGA_SOLUTION_CONTROL2_SUPPORTED = 0x101,
    E_SYS_QUERY_GINGA_SOLUTION_CONTROL3_SUPPORTED = 0x102,
    E_SYS_QUERY_SECURED_IC_SUPPORTED = 0x200,
} E_SYS_QUERY;

typedef enum
{
    E_SYS_ATV_AGC_PAD_SET,
    E_SYS_DTV_AGC_PAD_SET,
    E_SYS_DTV_AGC_PAD_SET_ALL_OFF,
}SYS_AGC_PAD_SET;

typedef enum
{
    E_TS0_PAD_SET,
    E_TS1_PAD_SET,
    E_TS2_PAD_SET,
    E_TS3_PAD_SET,
    E_CA_CI_PAD_SET,
    E_SC_PAD_SET,
    E_MSPI_PAD_SET,
    E_TS4_PAD_SET,
    E_TS5_PAD_SET,
    E_TSO_PAD_SET,
    E_PAD_SET_MAX
}SYS_PAD_MUX_SET;

typedef enum
{
    E_PARALLEL_IN,
    E_SERIAL_IN,
    E_PARALLEL_IN_2nd_PORT,
    E_PARALLEL_OUT,
    E_CA_CI_PAD_CA,
    E_CA_CI_PAD_CI,
    E_CA_CI_PAD_NONE,
    E_TRI_STATE_ON,
    E_TRI_STATE_OFF,
    E_SC_PAD_PCM,
    E_SC_PAD_TS1,
    E_MSPI_PAD_ON,
    E_MSPI_PAD_GPIO,
    E_SERIAL_IN_3_WIRE,
    E_PARALLEL_OUT_SER2PAR,
    E_PARALLEL_OUT_INTDMD,
    E_PARALLEL_OUT_TSO,
    E_PARALLEL_OUT_SER2PAR1,
    E_MSPI_PAD_ON_2nd_PORT,
    E_PAD_DRIVING_4MA = 0x00001000,
    E_PAD_DRIVING_8MA = 0x00001001,
    E_PAD_SEL_MAX
}SYS_PAD_SEL;

typedef struct _SYS_XC_TIMING
{
    MS_U32 u32TimingVersion;      //init it with MDRV_SYS_XC_TIMING_VERSION
    MS_U32 u32StructLength;       //init it with the size of this stuct
    MS_U32 u32Width;
    MS_U32 u32Height;
    MS_U16 u16Framerate;
} SYS_XC_TIMING_t;

typedef struct _SYS_XC_BYPART_TIMING
{
    MS_U32 u32TimingVersion;      //init it with MDRV_SYS_XC_BYPART_TIMING_VERSION
    MS_U32 u32StructLength;       //init it with the size of this stuct
    SYS_XC_TIMING_t stInput;
    SYS_XC_TIMING_t stOutput;
} SYS_XC_BYPART_TIMING_t;

typedef enum
{
    E_PCM_CD_SINGLE,
    E_PCM_CD_OR,
    E_PCM_CD_MAX
}SYS_PCM_CD_MODE;

typedef enum
{
    E_SYS_DOLBY_VERSION = 0x00,
    E_SYS_DOLBY_CONTROL_BIT = 0x01,
    E_SYS_DOLBY_REVERSE_BIT = 0x02,

} E_SYS_DOLBY_HASH_INFO;

typedef enum
{
    E_SYS_CHIP_TV,
    E_SYS_CHIP_STB,
    E_SYS_CHIP_MAX,
} E_SYS_CHIP_TYPE;

typedef enum
{
	E_SYS_IP_ENCODER,
	E_SYS_IP_FRONTEND,
	E_SYS_IP_DEMUX,
	E_SYS_IP_SYSTEM,
	E_SYS_IP_CA,
	E_SYS_IP_CODEC,
	E_SYS_IP_DISPLAY,
	E_SYS_IP_AUDIO,
	E_SYS_IP_GFX,
	E_SYS_IP_STORAGE,
} E_SYS_IP_TYPE;

typedef enum
{
	E_SYS_IO_SWI2C_READ     = 0x00,
	E_SYS_IO_SWI2C_WRITE    = 0x01,
	E_SYS_IO_HWI2C_READ     = 0x02,
	E_SYS_IO_HWI2C_WRITE    = 0x03,
	E_SYS_IO_MAX            = 0xFF,
} E_SYS_IO_PROC;

typedef enum
{
    E_SYS_OK = 0x0,
    E_SYS_FAIL,
    E_SYS_ERROR_PARAMETER,
    E_SYS_NOT_SUPPORT,
} E_SYS_ReturnValue;

typedef enum
{
    E_SYS_QOS_QUERY_XC_BYPART_EN = 0x00,  //Info: SYS_XC_BYPART_TIMING_t
    E_SYS_QOS_QUERY_MEMC_EN,              //Info: SYS_XC_TIMING_t
    E_SYS_QOS_MAX,
} E_SYS_QOS_QUERY_TYPE;

typedef enum
{
    E_SYS_MEMC_4X = 0x0,
    E_SYS_MEMC_2X,
    E_SYS_MEMC_BYPASS,
    E_SYS_MEMC_OFF,
} E_SYS_MEMC_LEVEL;

typedef enum
{
    E_SYS_INPUT_TYPE_DOLBY_HDMI = 0,
    E_SYS_INPUT_TYPE_HDMI,
    E_SYS_INPUT_TYPE_HDR10_PLUS_HDMI,
    E_SYS_INPUT_TYPE_MAX,
} E_SYS_INPUT_TYPE;

typedef enum
{
    E_SYS_BGA17 = 0x0,
    /* append new BGA package above this line */

    E_SYS_QFP156 = 0x40,
    /* append new QFP package above this line */

    E_SYS_PACKAGE_UNKNOWN = 0xFF,
} E_SYS_PACKAGE_TYPE;

#define SYS_TEEINFO_OSTYPE_NOTEE                 0
#define SYS_TEEINFO_OSTYPE_NUTTX                 1
#define SYS_TEEINFO_OSTYPE_OPTEE                 2
#define SYS_TEEINFO_OSTYPE_SECARM                3

typedef struct _SYS_TEEInfo
{
    MS_U32 OsType;
}SYS_TEEINFO;

typedef enum
{
    /* TSP && VDEC && XC need to sync. */
    E_SYS_TEEWRP_FUNC_RET_NULL = 0,
    E_SYS_TEEWRP_FUNC_RET_INVALID,
    E_SYS_TEEWRP_FUNC_RET_NON_TEE,
    E_SYS_TEEWRP_FUNC_RET_SUCCESS,
    E_SYS_TEEWRP_FUNC_RET_FAIL,
    /* CA Wrapper. */
    E_SYS_TEEWRP_FUNC_RET_INIT = 10,
    E_SYS_TEEWRP_FUNC_RET_ID,
    E_SYS_TEEWRP_FUNC_RET_RETRY_TIMEOUT,
    E_SYS_TEEWRP_FUNC_RET_DATA_TIMEOUT,
    /* TSP need to sync. */
    E_SYS_TEEWRP_FUNC_RET_NOT_SUPPORT = 20,
    E_SYS_TEEWRP_FUNC_RET_NOT_FOUND,
    E_SYS_TEEWRP_FUNC_RET_BAD_PARAMTERS,
    E_SYS_TEEWRP_FUNC_RET_DENIED,
} SYS_TEEWRP_FUNC_RET;

/* Must equal to TEEC_UUID. */
typedef struct
{
    MS_U32  u32timeLow;
    MS_U16  u16timeMid;
    MS_U16  u16timeHiAndVersion;
    MS_U8   u8clockSeqAndNode[8];
} _TEEC_UUID;

typedef struct DLL_PACKED _SYS_TEEWRP_DATA
{
    /* Common. */
    MS_S32          s32Id;
    MS_U8           u8Cmd;
    MS_VIRT         virtAddr;
    MS_PHY          phySize;
    MS_U32          u32FuncRet;
    /* Nuttx. */
    char            pR2teeName[SYS_TEEWRP_NAME_LENGTH];
    MS_U32          u32CmdTimeout;
    /* Optee. */
    char            pOpteeName[SYS_TEEWRP_NAME_LENGTH];
    _TEEC_UUID      stUuid;
} SYS_TEEWRP_DATA;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_GlobalInit(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
const SYS_Info* MDrv_SYS_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8           MDrv_SYS_GetChipRev(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16          MDrv_SYS_GetChipID(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WDTEnable(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WDTClear(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_WDTLastStatus(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WDTSetTime(MS_U32 u32Ms);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_ResetChip(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_ResetCPU(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_SetDbgLevel(SYS_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_FlushMemory(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_ReadMemory(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_Query(E_SYS_QUERY id);

#ifdef _SYS_DAC_GAIN_CTRL_
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_EnhanceDACGain(MS_U8 u8DAC_ID, MS_BOOL bEnable);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_GetSoftwareVersion(MS_SW_VERSION_INFO *pSoftwareVersionInfo);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel);
//-------------------------------------------------------------------------------------------------
/// Get Utopia Pack Mode
/// @ingroup G_SYS_CONTROL
/// @return TRUE : Pack Mode
/// @return FALSE : Non-Pack Mode
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_PackMode(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_DisableDebugPort(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_EnableDebugPort(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetTSOutClockPhase(MS_U16 u16Val);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetTSClockPhase(SYS_PAD_MUX_SET ePadMuxType, MS_U16 u16Val);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_PadMuxTableSuspend(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_PadMuxTableResume(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SYS_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_SYS_CHIP_TYPE MDrv_SYS_GetChipType(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_SetChipType(E_SYS_CHIP_TYPE Type);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_GetDolbyKeyCustomer(MS_U8 * u8pkey);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16          MDrv_SYS_ReadBrickTerminatorStatus(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WriteBrickTerminatorStatus(MS_U16 u16Status);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_GetEfuseDid(MS_U16 *u16efuse_did);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey);
//-------------------------------------------------------------------------------------------------
/// Query SoC package type
/// @ingroup G_SYS_OTHER
//  @return @ref E_SYS_PACKAGE_TYPE
//-------------------------------------------------------------------------------------------------
E_SYS_PACKAGE_TYPE MDrv_SYS_GetPackageType(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SYS_GetIpList(E_SYS_IP_TYPE TYPE);
//-------------------------------------------------------------------------------------------------
/// Query MEMC Config
/// @ingroup G_SYS_COMMON
/// @param u32SourceType \b IN: Source Type, Ref: INPUT_SOURCE_TYPE_t
/// @param stXCTiming \b IN: Input Timing, Ref: SYS_XC_TIMING_t
/// @param retEnMemc \b OUT: Query result of enabling EMMC
/// @return @ref E_SYS_ReturnValue
//-------------------------------------------------------------------------------------------------
E_SYS_ReturnValue __attribute__((weak)) MDrv_SYS_GetMemcConfg(MS_U32 u32SourceType, SYS_XC_TIMING_t *stXCTiming, MS_BOOL *retEnMemc);
//-------------------------------------------------------------------------------------------------
/// Query QoS Config
/// @ingroup G_SYS_COMMON
/// @param eQueryType \b IN: Query Type, Ref: E_SYS_QOS_QUERY_TYPE
/// @param u32SourceType \b IN: Source Type, Ref: INPUT_SOURCE_TYPE_t
/// @param stXCTiming \b IN: Input Info Structure, each E_SYS_QOS_QUERY_TYPE maps to a specified struct
/// @param retEn \b OUT: Query result of enabling status
/// @return @ref E_SYS_ReturnValue
//-------------------------------------------------------------------------------------------------
E_SYS_ReturnValue __attribute__((weak)) MDrv_SYS_GetQoSConfig(E_SYS_QOS_QUERY_TYPE eQueryType, MS_U32 u32SourceType, void *stInfo, MS_BOOL *retEn);
//-------------------------------------------------------------------------------------------------
/// Query QoS Config
/// @ingroup G_SYS_COMMON
/// @param eQueryType \b IN: Query Type, Ref: E_SYS_QOS_QUERY_TYPE
/// @param u32SourceType \b IN: Source Type, Ref: INPUT_SOURCE_TYPE_t
/// @param stXCTiming \b IN: Input Info Structure, each E_SYS_QOS_QUERY_TYPE maps to a specified struct
/// @param retSt \b OUT: Query result of Qos status
/// @return @ref E_SYS_ReturnValue
//-------------------------------------------------------------------------------------------------
E_SYS_ReturnValue __attribute__((weak)) MDrv_SYS_GetQoSStatus(E_SYS_QOS_QUERY_TYPE eQueryType, MS_U32 u32SourceType, void *stInfo, MS_U32 *retSt);

void __attribute__((weak)) MDrv_SYS_SetInputType(E_SYS_INPUT_TYPE TYPE);

//FOR INTERNAL USE ONLY
MS_U32          MDrv_SYS_RegisterIoProc(E_SYS_IO_PROC dwIoType, void *pdwIoFunction);
void MDrv_SYS_ReadKernelCmdLine(void);
MS_U32 MDrv_SYS_SetTEEInfo(MS_U32 osType);
MS_U32 MDrv_SYS_GetTEEInfo(SYS_TEEINFO* TeeInfo);

//-------------------------------------------------------------------------------------------------
/// Query device key-value via DTS.
/// @ingroup G_SYS_COMMON
/// @param pu8Node      \b IN: Deviece node
/// @param pu8Key       \b IN: Key word
/// @param pu8Value     \b OUT: Result
/// @param u32BufferSize \b IN: Buffer length
/// @return >= 0 : Success
/// @return <  0 : Fail
//-------------------------------------------------------------------------------------------------
MS_S32 MDrv_Sys_GetDTSValue(MS_U8* pu8Node, MS_U8* pu8Key, MS_U8* pu8Value, MS_U32 u32BufferSize);

//-------------------------------------------------------------------------------------------------
/// Integration Nuttx and Optee initialize function.
/// @ingroup G_SYS_COMMON
/// @param  pstData \b IN: Data setting.
/// @return MS_BOOL : TRUE = Success, FALSE = Fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_TEEWRP_Init(SYS_TEEWRP_DATA *pstData);

//-------------------------------------------------------------------------------------------------
/// Integration Nuttx and Optee communicate function.
/// @ingroup G_SYS_COMMON
/// @param  pstData \b IN: Data setting.
/// @return MS_BOOL : TRUE = Success, FALSE = Fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_TEEWRP_Comm(SYS_TEEWRP_DATA *pstData);

//-------------------------------------------------------------------------------------------------
/// Integration Nuttx and Optee destroy function.
/// @ingroup G_SYS_COMMON
/// @param  pstData \b IN: Data setting.
/// @return MS_BOOL : TRUE = Success, FALSE = Fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_TEEWRP_Destroy(SYS_TEEWRP_DATA *pstData);

#ifdef UFO_SYS_PADMUX
//-------------------------------------------------------------------------------------------------
/// SYS Pad Mux Control
/// @param u32RegAddr \b IN: Register Address
/// @param u8RegVal \b IN: Register Value
/// @param u8Mask \b IN: Mask Value
/// @return MS_BOOL :
/// - TRUE: Success
/// - FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_PadMuxCtrl(MS_U32 u32RegAddr, MS_U8  u8RegVal, MS_U8  u8Mask);
#endif

char* MDrv_SYS_GetUtopiaReleaseLabel(void);
MS_BOOL __attribute__((weak)) MDrv_SYS_ReadHDCPKey(MS_U8 *u8HDCPKey);

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
MS_S16 MDrv_SYS_GetCurrentTemperature(void);
MS_U16 MDrv_SYS_GetLowerLevelTemperature(void);
MS_U16 MDrv_SYS_GetUpperLevelTemperature(void);
MS_U16 MDrv_SYS_GetMaxLevelTemperature(void);
#endif

//note:
//Fix Me. [C2P] Mdrv_SYS_VR360_Restore_BW_Setting, Mdrv_SYS_VR360_Set_BW_Setting are temporary solutions for VR360.
//Request by PM - Treasure
//It will be remove when sn/mi adds flow for recognizating vr360 mode.
void            Mdrv_SYS_VR360_Restore_BW_Setting(void);
void            Mdrv_SYS_VR360_Set_BW_Setting(void);

#ifdef __cplusplus
}
#endif


#endif // _DRV_SYS_H_
