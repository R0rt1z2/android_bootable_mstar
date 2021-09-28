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
/*! \defgroup G_PWS PWS interface
    \ingroup  G_PERIPHERAL

    \brief
    Load power settings of demo atop and video atop at a specified source.

    <b>Features</b>

    - Load power settings.

    <b> PWS Block Diagram: </b> \n

    NA

    \defgroup G_PWS_INIT Initialization Task relative
    \ingroup  G_PWS
    \defgroup G_PWS_COMMON Common Task relative
    \ingroup  G_PWS
    \defgroup G_PWS_CONTROL Control relative
    \ingroup  G_PWS
    \defgroup G_PWS_OTHER  other relative
    \ingroup  G_PWS
    \defgroup G_PWS_ToBeModified PWS api to be modified
    \ingroup  G_PWS
    \defgroup G_PWS_ToBeRemove PWS api to be removed
    \ingroup  G_PWS
*/

#ifndef _DRVPWS_H_
#define _DRVPWS_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief power saving driver
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_PWS_LIB_CODE                     {'P','W','S','_'}        //Lib code
#define MSIF_PWS_LIBVER                       {'0','2'}            //LIB version
#define MSIF_PWS_BUILDNUM                     {'0','9'}            //Build Number
#define MSIF_PWS_CHANGELIST                   {'0','0','4','4','8','1','7','6'} //P4 ChangeList Number

#define PWS_API_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_PWS_LIB_CODE,                        /* IP__                                             */  \
    MSIF_PWS_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_PWS_BUILDNUM,                        /* 00 ~ 99                                          */  \
    MSIF_PWS_CHANGELIST,                      /* CL#                                              */  \
    MSIF_OS

#define SIOCDEVON       0x89F1
#define SIOCDEVOFF      0x89F2

#define PWS_VERSION            0x00000001


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    PWS_FAIL    = 0,
    PWS_SUCCESS = 1
} E_PWS_Result;

#if defined (MSOS_TYPE_LINUX)
typedef enum
{
    PHY_OFF    = 0,
    PHY_ON   = 1
} E_PWS_PHY;
#endif

typedef enum
{
    PWS_ADD_SOURCE  = 0,
    PWS_DEL_SOURCE  = 1,
    PWS_INVAILD_OP  = 2
} E_PWS_Operation;

typedef enum
{
    PWS_FULL    = 0,
    PWS_OFF_LINE_DETECT = 1
} E_PWS_Sync;

typedef enum
{
    _NO_SOURCE_   = 0,
    _USB_         = BIT_(0),
    _SV_          = BIT_(1),
    _HDMI4_       = BIT_(2),
    _HDMI3_       = BIT_(3),
    _HDMI2_       = BIT_(4),
    _HDMI1_       = BIT_(5),
    _YPbPr_       = BIT_(6),
    _SCART_       = BIT_(7),
    _RGB_         = BIT_(8),
    _CVBS_        = BIT_(9),
    _ATV_SSIF_    = BIT_(10),
    _ATV_VIF_     = BIT_(11),
    _DTV_ATSC_    = BIT_(12),
    _DTV_DVB_     = BIT_(13),
    _CVBSOe_      = BIT_(14),
    _CVBSOi_      = BIT_(15),
    _UNKNOWN_     = BIT_(16)
} E_PWS_SouceInfo;

typedef enum
{
    E_PWS_DBGLV_NONE,          //no debug message
    E_PWS_DBGLV_ERR_ONLY,      //show error only
    E_PWS_DBGLV_REG_DUMP,      //show error & reg dump
    E_PWS_DBGLV_INFO,          //show error & informaiton
    E_PWS_DBGLV_ALL            //show error, information & funciton name
} E_PWS_DBG_LEVEL;

typedef struct
{
    E_PWS_DBG_LEVEL u8DbgLevel;
    MS_BOOL bInit;
} PWS_Status;

typedef struct
{
    E_PWS_SouceInfo SourceList;
    MS_U32 u32IOMap;
} PWS_Info;

typedef enum
{
    E_PWS_API_UNUSED = 0,
    E_PWS_USE_HANDLE_SOURCE = 1,
    E_PWS_USE_HANDLE_IP = 2
} E_PWS_Use;

typedef enum
{
    E_PWS_IP_ON  = 0,
    E_PWS_IP_OFF = 1,
    E_PWS_IP_RESET = 2,
} E_PWS_IpPowerCtrl;

typedef enum
{
    E_PWS_IP_CALLBACK_BEFORE_ON  = 0,
    E_PWS_IP_CALLBACK_BEFORE_OFF = 1,
    E_PWS_IP_CALLBACK_AFTER_ON   = 2,
    E_PWS_IP_CALLBACK_AFTER_OFF  = 3,
    E_PWS_IP_CALLBACK_UNKNOWN = 4
} E_PWS_CallbackCtrl;

#define STR_LEN     32
typedef void (*P_PWS_Callback)(void);
typedef struct
{
    E_PWS_CallbackCtrl ctrl;
    unsigned char regName[STR_LEN];
    P_PWS_Callback _PWSCallback;
} PWS_RegisterCallback;

typedef enum
{
    E_PWS_VIF_NO_SAW     = 0,
    E_PWS_VIF_SINGLE_SAW = 1,
    E_PWS_VIF_DUAL_SAW   = 2,
    E_PWS_VIF_SINGLE_SAW_DIF = 3,
    E_PWS_VIF_UNKNOWN    = 3
} E_PWS_VIF_type;

#define PWS_Register_CB_size    2


typedef enum
{
    E_PWS_IP_AUDIO = 0,
    E_PWS_IP_DEMOD,
    E_PWS_IP_AUPLL,
    E_PWS_IP_USB_PORT0,
    E_PWS_IP_USB_PORT1,
    E_PWS_IP_USB_PORT2,
    E_PWS_IP_USB_PORT3,
    E_PWS_IP_USB_3,
    E_PWS_IP_MHL,
    E_PWS_IP_VEDIO,
    E_PWS_IP_ATSC_DVBTC,
    E_PWS_IP_GME,
    E_PWS_IP_GMAC,
    E_PWS_IP_IDAC,
    E_PWS_IP_ETH,
    E_PWS_IP_TSP,
    E_PWS_IP_CLK_R2_SECURE,
    E_PWS_IP_VE,
    E_PWS_IP_AUDIO_R2,
    E_PWS_IP_SC1,
    E_PWS_IP_MAX
} E_PWS_IP_name;

typedef enum
{
    E_PWS_SCENE_MM_MAIN = 0,
    E_PWS_SCENE_MM_SUB,
    E_PWS_SCENE_MAX
} E_PWS_SCENE_name;

typedef enum
{
    E_PWS_GROUP0 = 0,
    E_PWS_GROUP1,
    E_PWS_GROUP2,
    E_PWS_GROUP_MAX
} E_PWS_GROUP_ID;

typedef enum
{
    E_PWS_CTL_GROUP,
    E_PWS_CTL_HDMI,
    E_PWS_CTL_MAX,
} E_PWS_POWER_CTL;

typedef struct _PWS_GROUP_CTL_PARAM
{
    E_PWS_GROUP_ID GroupId;
    MS_U8 ClientId;
    MS_BOOL enable;
} PWS_GROUP_CTL_PARAM;


typedef enum
{
    E_PWS_HDMI_A = 0,
    E_PWS_HDMI_B,
    E_PWS_HDMI_C,
    E_PWS_HDMI_D,
    E_PWS_HDMI_MAX,
} E_PWS_HDMI_INFO;

typedef enum
{
    E_PWS_HDMI_CTL_NORMAL = 0,
    E_PWS_HDMI_CTL_STANDBY,
    E_PWS_HDMI_CTL_POWERDOWN,
    E_PWS_HDMI_CTL_MAX,
} E_PWS_HDMI_CTL;

typedef struct _PWS_HDMI_CTL_PARAM
{
    E_PWS_HDMI_INFO source;
    E_PWS_HDMI_CTL control;
} PWS_HDMI_CTL_PARAM;

//------------------------------------------------------------------------------
// Extern Function
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_SetDbgLevel(E_PWS_DBG_LEVEL eLEVEL);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void         MDrv_PWS_GetInfo(PWS_Info *pInfo); // ToBeRemove
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void         MDrv_PWS_GetStatus(PWS_Status *pStatus);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_SouceInfo MDrv_PWS_GetSourceInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16       MDrv_PWS_Read2Byte(MS_U32 u32RegAddr );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_IpPowerControl(E_PWS_IP_name eIpName, E_PWS_IpPowerCtrl pwr_type);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_ScenePowerControl(E_PWS_SCENE_name eSceneName, E_PWS_IpPowerCtrl pwr_type);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param CtlType \b IN: Control type
/// @param len \b IN: data length
/// @param data \b IN: data
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SYMBOL_WEAK E_PWS_Result MDrv_PWS_PowerControl(E_PWS_POWER_CTL CtlType, MS_U32 len, void *data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_Init(E_PWS_VIF_type eVifType);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_HandleSource(E_PWS_Operation operation,E_PWS_SouceInfo source,E_PWS_Sync sync_type);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_HandleIP(E_PWS_IpPowerCtrl IpPowerCtrl,const unsigned char *RegName); // ToBeRemove
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_RegisterCallback(MS_U8 index,E_PWS_CallbackCtrl cb_ctrl,unsigned char *RegName,P_PWS_Callback pfCallback);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_CancelCallback(MS_U8 index);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PWS_SetPowerState(EN_POWER_MODE u16PowerState);
#if defined (MSOS_TYPE_LINUX)
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_PHYPower(E_PWS_PHY bSetFlag); // ToBeRemove
#endif

#ifdef __cplusplus
}
#endif

#endif
