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
#ifndef _DRVPWS_PRIV_H_
#define _DRVPWS_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsTypes.h"

//------------------------------------------------------------------------------------
//  Utopia 2.0
//------------------------------------------------------------------------------------
typedef enum {
    MDrv_CMD_PWS_Init,
    MDrv_CMD_PWS_GetLibVer,
    MDrv_CMD_PWS_SetDbgLevel,
    MDrv_CMD_PWS_HandleSource,
    MDrv_CMD_PWS_PHYPower,
    MDrv_CMD_PWS_GetInfo,
    MDrv_CMD_PWS_GetStatus,
    MDrv_CMD_PWS_GetSourceInfo,
    MDrv_CMD_PWS_Read2Byte,
    MDrv_CMD_PWS_IpPowerControl,
    MDrv_CMD_PWS_ScenePowerControl,
    MDrv_CMD_PWS_PowerControl,
    MDrv_CMD_PWS_HandleIP,
    MDrv_CMD_PWS_RegisterCallBack,
    MDrv_CMD_PWS_CancelCallBack,
    MDrv_CMD_PWS_SetPowerState,
} ePwsIoctlOpt;

typedef E_PWS_Result (*IOCTL_PWS_INIT)(E_PWS_VIF_type eVifType);
typedef E_PWS_Result (*IOCTL_PWS_GETLIBVER)(const MSIF_Version **);
typedef E_PWS_Result (*IOCTL_PWS_SETDBGLEVEL)(E_PWS_DBG_LEVEL);
typedef E_PWS_Result (*IOCTL_PWS_HANDLESOURCE)(E_PWS_Operation, E_PWS_SouceInfo, E_PWS_Sync);
#if defined (MSOS_TYPE_LINUX)
typedef E_PWS_Result (*IOCTL_PWS_PHYPOWER)(E_PWS_PHY);
#endif
typedef void (*IOCTL_PWS_GETINFO)(PWS_Info *pInfo);
typedef void (*IOCTL_PWS_GETSTATUS)(PWS_Status *pStatus);
typedef E_PWS_SouceInfo (*IOCTL_PWS_GETSOURCEINFO)(void);
typedef MS_U16 (*IOCTL_PWS_READ2BYTE)(MS_U32 u32RegAddr);
typedef E_PWS_Result (*IOCTL_PWS_IPPOWERCONTROL)(E_PWS_IP_name eIpName, E_PWS_IpPowerCtrl pwr_type);
typedef E_PWS_Result (*IOCTL_PWS_SCENEPOWERCONTROL)(E_PWS_SCENE_name eSceneName, E_PWS_IpPowerCtrl pwr_type);
typedef E_PWS_Result (*IOCTL_PWS_POWERCONTROL)(E_PWS_POWER_CTL CtlType, MS_U32 len, void *data);
typedef E_PWS_Result (*IOCTL_PWS_HANDLEIP)(E_PWS_IpPowerCtrl IpPowerCtrl,const unsigned char *RegName);
typedef E_PWS_Result (*IOCTL_PWS_REGISTERCALLBACK)(MS_U8 index,E_PWS_CallbackCtrl cb_ctrl,unsigned char *RegName,P_PWS_Callback pfCallback);
typedef E_PWS_Result (*IOCTL_PWS_CANCELCALLBACK)(MS_U8 index);
typedef MS_U32 (*IOCTL_PWS_SETPOWERSTATE)(EN_POWER_MODE u16PowerState);

typedef struct _PWS_INSTANT_PRIVATE
{
    IOCTL_PWS_INIT                 fpPWSInit;
    IOCTL_PWS_GETLIBVER            fpPWSGetLibVer;
    IOCTL_PWS_SETDBGLEVEL          fpPWSSetDbgLevel;
    IOCTL_PWS_HANDLESOURCE         fpPWSHandleSource;
#if defined (MSOS_TYPE_LINUX)
    IOCTL_PWS_PHYPOWER             fpPWSPHYPower;
#endif
    IOCTL_PWS_GETINFO              fpPWSGetInfo;
    IOCTL_PWS_GETSTATUS            fpPWSGetStatus;
    IOCTL_PWS_GETSOURCEINFO        fpPWSGetSourceInfo;
    IOCTL_PWS_READ2BYTE            fpPWSRead2Byte;
    IOCTL_PWS_IPPOWERCONTROL       fpPWSIpPowerControl;
    IOCTL_PWS_SCENEPOWERCONTROL    fpPWSScenePowerControl;
    IOCTL_PWS_POWERCONTROL         fpPWSPowerControl;
    IOCTL_PWS_HANDLEIP             fpPWSHandleIP;
    IOCTL_PWS_REGISTERCALLBACK     fpPWSRegisterCallBack;
    IOCTL_PWS_CANCELCALLBACK       fpPWSCancelCallBack;
    IOCTL_PWS_SETPOWERSTATE        fpPWSSetPowerState;
}PWS_INSTANT_PRIVATE;

typedef struct _PWS_INIT_PARAM
{
    E_PWS_VIF_type eVifType;
}PWS_INIT_PARAM, *PPWS_INIT_PARAM;

// for pws MDrv_PWS_GetLibVer
typedef struct _PWS_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}PWS_GETLIBVER_PARAM, *PPWS_GETLIBVER_PARAM;

// for pws MDrv_PWS_SetDbgLevel
typedef struct _PWS_SETDBGLEVEL_PARAM
{
    E_PWS_DBG_LEVEL DbgLevel;
}PWS_SETDBGLEVEL_PARAM, *PPWS_SETDBGLEVEL_PARAM;

// for pws MDrv_PWS_HandleSource
typedef struct _PWS_HANDLESOURCE_PARAM
{
    E_PWS_Operation operation;
    E_PWS_SouceInfo source;
    E_PWS_Sync sync_type;
}PWS_HANDLESOURCE_PARAM, *PPWS_HANDLESOURCE_PARAM;

#if defined (MSOS_TYPE_LINUX)
// for pws MDrv_PWS_PHYPower
typedef struct _PWS_PHYPOWER_PARAM
{
    E_PWS_PHY bSetFlag;
}PWS_PHYPOWER_PARAM, *PPWS_PHYPOWER_PARAM;
#endif

typedef struct _PWS_GETINFO_PARAM
{
    PWS_Info *pInfo;
}PWS_GETINFO_PARAM, *PPWS_GETINFO_PARAM;

typedef struct _PWS_GETSTATUS_PARAM
{
    PWS_Status *pStatus;
}PWS_GETSTATUS_PARAM, *PPWS_GETSTATUS_PARAM;

typedef struct _PWS_GETSOURCEINFO_PARAM
{
    E_PWS_SouceInfo eSouceInfo;
}PWS_GETSOURCEINFO_PARAM, *PPWS_GETSOURCEINFO_PARAM;

typedef struct _PWS_READ2BYTE_PARAM
{
    MS_U32 u32RegAddr;
    MS_U16 u16RegVal;
}PWS_READ2BYTE_PARAM, *PPWS_READ2BYTE_PARAM;

typedef struct _PWS_IPPOWERCONTROL_PARAM
{
    E_PWS_IP_name eIpName;
    E_PWS_IpPowerCtrl pwr_type;
}PWS_IPPOWERCONTROL_PARAM, *PPWS_IPPOWERCONTROL_PARAM;

typedef struct _PWS_SCENEPOWERCONTROL_PARAM
{
    E_PWS_SCENE_name eSceneName;
    E_PWS_IpPowerCtrl pwr_type;
}PWS_SCENEPOWERCONTROL_PARAM, *PPWS_SCENEPOWERCONTROL_PARAM;

typedef struct _PWS_POWERCONTROL_PARAM
{
    E_PWS_POWER_CTL CtlType;
    MS_U32 len;
    void *data;
}PWS_POWERCONTROL_PARAM, *PPWS_POWERCONTROL_PARAM;

typedef struct _PWS_HANDLEIP_PARAM
{
    E_PWS_IpPowerCtrl IpPowerCtrl;
    const unsigned char *RegName;
}PWS_HANDLEIP_PARAM, *PPWS_HANDLEIP_PARAM;

typedef struct _PWS_REGISTERCALLBACK_PARAM
{
    MS_U8 index;
    E_PWS_CallbackCtrl cb_ctrl;
    unsigned char *RegName;
    P_PWS_Callback pfCallback;
}PWS_REGISTERCALLBACK_PARAM, *PPWS_REGISTERCALLBACK_PARAM;

typedef struct _PWS_CANCELCALLBACK_PARAM
{
    MS_U8 index;
}PWS_CANCELCALLBACK_PARAM, *PPWS_CANCELCALLBACK_PARAM;

typedef struct _PWS_SETPOWERSTATE_PARAM
{
    EN_POWER_MODE u16PowerState;
}PWS_SETPOWERSTATE_PARAM, *PPWS_SETPOWERSTATE_PARAM;


void PWSRegisterToUtopia(FUtopiaOpen ModuleType);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 PWSOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 PWSClose(void* pInstance);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWS_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 PWSIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

//PWS_Private
typedef struct _PWS_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}PWS_RESOURCE_PRIVATE;


#ifdef __cplusplus
}
#endif
#endif // _DRVPWS_PRIV_H_
