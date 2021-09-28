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

#ifndef _API_XC_PANEL_V2_H_
#define _API_XC_PANEL_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "UFO.h"
//******** ENUM DEFINITIONS ********//
typedef enum
{
    E_PNL_POOL_ID_INTERNAL = 0,
    E_PNL_POOL_ID_INTERNAL1 = 1,

    E_PNL_POOL_ID_NUM,
    E_PNL_POOL_ID_MAX = E_PNL_POOL_ID_NUM,
} E_PNL_POOL_ID;

/// Define PNL device ID
typedef enum
{
    E_PNL_DEVICE_ID_0 = 0,
    E_PNL_DEVICE_ID_1 = 1,
    E_PNL_DEVICE_ID_NUM,
    E_PNL_DEVICE_ID_MAX = E_PNL_DEVICE_ID_NUM,
} E_PNL_DEVICE_ID;

// For multi-device
#define PNL_GET_INTERNAL_POOL_ID(u32Id)\
            ((u32Id == E_PNL_DEVICE_ID_1) ? E_PNL_POOL_ID_INTERNAL1 : E_PNL_POOL_ID_INTERNAL)

typedef enum
{
    E_PNL_CMD_GETLIBVER,
    E_PNL_CMD_GETINFO,
    E_PNL_CMD_GETSTATUS,
    E_PNL_CMD_GETSTATUSEX,
    E_PNL_CMD_SETDBGLEVEL,
    E_PNL_CMD_IOMAPBASEINIT,
    E_PNL_CMD_PREINIT,
    E_PNL_CMD_INIT,
    E_PNL_CMD_INIT_EX,
    E_PNL_CMD_GET_CONFIG,
    E_PNL_CMD_SETOUTPUT,
    E_PNL_CMD_CHANGEPANELTYPE,
    E_PNL_CMD_TCONMAP_DUMPTABLE,
    E_PNL_CMD_TCONMAP_POWER_SEQUENCE,
    E_PNL_CMD_TCON_COUNT_RESET,
    E_PNL_CMD_TCON_INIT,
    E_PNL_CMD_GETDSTINFO,
    E_PNL_CMD_CONTROL_OUT_SWING,
    E_PNL_CMD_SET_OUTPUT_PATTERN,
    E_PNL_CMD_MOD_CALIBRATION_SETTING,
    E_PNL_CMD_MOD_DO_CALIBRATION,
    E_PNL_CMD_BD_LVDS_OUTPUT_TYPE,
    E_PNL_CMD_SETLPLLTYPEEXT,
    E_PNL_CMD_INIT_MISC,
    E_PNL_CMD_GET_MISC_STATUS,
    E_PNL_CMD_MOD_OUTPUTCONFIG_USER,
    E_PNL_CMD_MOD_OUTPUT_CHANNEL_ORDER,
    E_PNL_CMD_HWLVDSRESERVEDTOLRFLAG,
    E_PNL_CMD_MOD_PVDD_POWER_SETTING,
    E_PNL_CMD_SETSSC_EN,
    E_PNL_CMD_SETSSC_FMODULATION,
    E_PNL_CMD_SETSSC_RDEVIATION,
    E_PNL_CMD_SETOSDSSC_EN,
    E_PNL_CMD_SETOSDSSC_FMODULATION,
    E_PNL_CMD_SETOSDSSC_RDEVIATION,
    E_PNL_CMD_SKIPTIMINGCHANGE,
    E_PNL_CMD_PRE_SET_MODE_ON,
    E_PNL_CMD_OVERDRIVER_INIT,
    E_PNL_CMD_OVERDRIVER_SETTING,
    E_PNL_CMD_SETFILEPATH,
    E_PNL_CMD_OVERDRIVER_ENABLE,
    E_PNL_CMD_GET_LPLL_TYPE,
    E_PNL_CMD_GET_TCON_CAPABILITY,
    E_PNL_CMD_SETPAIRSWAP,
    E_PNL_CMD_SET_EXT_LPLL_TYPE,
    E_PNL_CMD_CALEXTLPLLSETBYDCLK,
    E_PNL_CMD_SETDIFFSWINGLEVEL,
    E_PNL_CMD_SETPOWERSTATE,
    E_PNL_CMD_FORCESETPANELDCLK,
    E_PNL_CMD_ENABLEINTERNALTERMINATION,
    E_PNL_CMD_OUTPUTDEVICEHANDSHAKE,
    E_PNL_CMD_OUTPUTDEVICEOCHANDSHAKE,
    E_PNL_CMD_SETOUTPUTINTERLACETIMING,
    E_PNL_CMD_GETOUTPUTINTERLACETIMING,
    E_PNL_CMD_GETPANELDATA,
    E_PNL_CMD_DUMPPANELDATA,
    E_PNL_CMD_SETSSC,
    E_PNL_CMD_GETPANELONTIMING,
    E_PNL_CMD_GETPANELOFFTIMING,
    E_PNL_CMD_GETPANELDIMCTRL,
    E_PNL_CMD_GETALLGAMMATBL,
    E_PNL_CMD_ENABLEPANEL,
    E_PNL_CMD_SETGAMMATBL,
    E_PNL_CMD_GETGAMMATBL,
    E_PNL_CMD_SETPNLGAMMATBL,
    E_PNL_CMD_SETGAMMAVALUE,
    E_PNL_CMD_GETLPLLMODE,
    E_PNL_CMD_GETDEFVFREQ,
    E_PNL_CMD_FORCESETPANELHSTART,
    E_PNL_CMD_GETPANELNAME,
    E_PNL_CMD_GETHSTART,
    E_PNL_CMD_GETVSTART,
    E_PNL_CMD_GETWIDTH,
    E_PNL_CMD_GETHEIGHT,
    E_PNL_CMD_GETHTOTAL,
    E_PNL_CMD_GETVTOTAL,
    E_PNL_CMD_GETHSYNCWIDTH,
    E_PNL_CMD_GETHSYNCBACKPORCH,
    E_PNL_CMD_GETVSYNCBACKPORCH,
    E_PNL_CMD_GETLPLLTYPE,
    E_PNL_CMD_GETARC,
    E_PNL_CMD_GETMINSET,
    E_PNL_CMD_GETMAXSET,
    E_PNL_CMD_GETOUTTIMINGMODE,
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
    E_PNL_CMD_GETSUPPORTMAXDCLK,
    E_PNL_CMD_INIT_LOCALDIMMING,
#else
    E_PNL_CMD_CHECKVBY1HANDSHAKESTATUS,
    E_PNL_CMD_SET_VIDEOHWTRAINING_MODE,
    E_PNL_CMD_SET_OSDHWTRAINING_MODE,
    E_PNL_CMD_GET_VIDEOHWTRAINING_MODE,
    E_PNL_CMD_GET_OSDHWTRAINING_MODE,
    E_PNL_CMD_Setting,
#endif
    E_PNL_CMD_SET_POLINVERT_THRESHOLD,
    E_PNL_CMD_POLINVERT,
    E_PNL_CMD_SETPNLGAMMABINTBL,
    E_PNL_CMD_SETMIRRORMODE,
    E_PNL_CMD_GETMIRRORSTATUS,
    E_PNL_CMD_GETPNLGAMMATBL,
    E_PNL_CMD_GETGAMMATBLEX,
    E_PNL_CMD_NUM,
    E_PNL_CMD_MAX = E_PNL_CMD_NUM,
} E_PNL_IOCTL_CMDS;

typedef struct DLL_PACKED  _stPNL_GetLibVer
{
    const MSIF_Version **ppVersion;
    APIPNL_Result eResult;
} stPNL_GetLibVer, *pstPNL_GetLibVer;

typedef struct DLL_PACKED  _stPNL_GetInfo
{
    const PNL_ApiInfo *pApiInfo;
} stPNL_GetInfo, *pstPNL_GetInfo;

typedef struct DLL_PACKED  _stPNL_GetStatus
{
    PNL_ApiStatus *pPnlStatus;
    MS_BOOL bReturn;
} stPNL_GetStatus, *pstPNL_GetStatus;

typedef struct DLL_PACKED  _stPNL_GetStatusEx
{
    PNL_ApiExtStatus *pPnlExtStatus;
    MS_BOOL bReturn;
} stPNL_GetStatusEx, *pstPNL_GetStatusEx;

typedef struct _stPNL_SetDbgLevel
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturn;
} stPNL_SetDbgLevel, *pstPNL_SetDbgLevel;

typedef struct _stPNL_IOMapBaseInit
{
    MS_BOOL bReturn;
} stPNL_IOMapBaseInit, *pstPNL_IOMapBaseInit;

typedef struct _stPNL_PreInit
{
    E_PNL_PREINIT_OPTIONS eInitParam;
    MS_BOOL bReturn;
} stPNL_PreInit, *pstPNL_PreInit;

typedef struct DLL_PACKED  _stPNL_Init
{
    PanelType *pSelPanelType;
    MS_BOOL bReturn;
} stPNL_Init, *pstPNL_Init;

typedef struct DLL_PACKED  _stPNL_Init_Ex
{
    PanelType *pSelPanelType;
    MSIF_Version LIBVER;
    MS_BOOL bReturn;
} stPNL_Init_Ex, *pstPNL_Init_Ex;

typedef struct DLL_PACKED  _stPNL_GetConfig
{
    PanelType *pSelPanelType;
    APIPNL_Result eReturn;
} stPNL_GetConfig, *pstPNL_GetConfig;

typedef struct _stPNL_SetOutput
{
    APIPNL_OUTPUT_MODE eOutputMode;
} stPNL_SetOutput, *pstPNL_SetOutput;

typedef struct DLL_PACKED  _stPNL_ChangePanelType
{
    PanelType *pSelPanelType;
    MS_BOOL bReturn;
} stPNL_ChangePanelType, *pstPNL_ChangePanelType;

typedef struct DLL_PACKED  _stPNL_TCONMAP_DumpTable
{
    MS_U8 *pTCONTable;
    MS_U8 u8Tcontype;
    MS_BOOL bReturn;
} stPNL_TCONMAP_DumpTable, *pstPNL_TCONMAP_DumpTable;

typedef struct DLL_PACKED  _stPNL_TCONMAP_Power_Sequence
{
    MS_U8 *pTCONTable;
    MS_BOOL bEnable;
    MS_BOOL bReturn;
} stPNL_TCONMAP_Power_Sequence, *pstPNL_TCONMAP_Power_Sequence;

typedef struct _stPNL_TCON_Count_Reset
{
    MS_BOOL bEnable;
} stPNL_TCON_Count_Reset, *pstPNL_TCON_Count_Reset;

typedef struct _stPNL_TCON_Init
{
} stPNL_TCON_Init, *pstPNL_TCON_Init;

typedef struct DLL_PACKED  _stPNL_GetDstInfo
{
    MS_PNL_DST_DispInfo *pDstInfo;
    MS_U32 u32SizeofDstInfo;
    MS_BOOL bReturn;
} stPNL_GetDstInfo, *pstPNL_GetDstInfo;

typedef struct _stPNL_Control_Out_Swing
{
    MS_U16 u16Swing_Level;
    MS_BOOL bReturn;
} stPNL_Control_Out_Swing, *pstPNL_Control_Out_Swing;

typedef struct _stPNL_SetOutputPattern
{
    MS_BOOL bEnable;
    MS_U16 u16Red;
    MS_U16 u16Green;
    MS_U16 u16Blue;
} stPNL_SetOutputPattern, *pstPNL_SetOutputPattern;

typedef struct DLL_PACKED  _stPNL_Mod_Calibration_Setting
{
	MS_PNL_ModCaliInfo *pstModCaliInfo;
	MS_BOOL bReturn;
} stPNL_Mod_Calibration_Setting, *pstPNL_Mod_Calibration_Setting;

typedef struct _stPNL_Mod_Do_Calibration
{
    MS_BOOL bReturn;
} stPNL_Mod_Do_Calibration, *pstPNL_Mod_Do_Calibration;

typedef struct _stPNL_BD_LVDS_Output_Type
{
    MS_U16 Type;
} stPNL_BD_LVDS_Output_Type, *pstPNL_BD_LVDS_Output_Type;

typedef struct _stPNL_SetLPLLTypeExt
{
    APIPNL_LINK_EXT_TYPE eLPLL_TypeExt;
} stPNL_SetLPLLTypeExt, *pstPNL_SetLPLLTypeExt;

typedef struct _stPNL_Init_MISC
{
    APIPNL_MISC ePNL_MISC;
} stPNL_Init_MISC, *pstPNL_Init_MISC;

typedef struct _stPNL_Get_MISC_Status
{
    MS_U32 u32Status;
} stPNL_Get_MISC_Status, *pstPNL_Get_MISC_Status;

typedef struct _stPNL_MOD_OutputConfig_User
{
    MS_U32 u32OutputCFG0_7;
    MS_U32 u32OutputCFG8_15;
    MS_U32 u32OutputCFG16_21;
} stPNL_MOD_OutputConfig_User, *pstPNL_MOD_OutputConfig_User;

typedef struct _stPNL_MOD_OutputChannelOrder
{
    MS_U8  u8OutputOrderType;
    MS_U16 u16OutputOrder0_3;
    MS_U16 u16OutputOrder4_7;
    MS_U16 u16OutputOrder8_11;
    MS_U16 u16OutputOrder12_13;
} stPNL_MOD_OutputChannelOrder, *pstPNL_MOD_OutputChannelOrder;

typedef struct _stPNL_SETMIRRORMODE
{
    ST_PNL_MIRRORINFO *pstPnlMirrorInfo;
    APIPNL_Result eReturn;
} stPNL_SETMIRRORMODE, *pstPNL_SETMIRRORMODE;

typedef struct _stPNL_GETMIRRORSTATUS
{
    ST_PNL_MIRRORINFO *pstPnlMirrorInfo;
    APIPNL_Result eReturn;
} stPNL_GETMIRRORSTATUS, *pstPNL_GETMIRRORSTATUS;

typedef struct _stPNL_HWLVDSReservedtoLRFlag
{
    MS_PNL_HW_LVDSResInfo lvdsresinfo;
} stPNL_HWLVDSReservedtoLRFlag, *pstPNL_HWLVDSReservedtoLRFlag;

typedef struct _stPNL_MOD_PVDD_Power_Setting
{
    MS_BOOL bIs2p5;
} stPNL_MOD_PVDD_Power_Setting, *pstPNL_MOD_PVDD_Power_Setting;

typedef struct _stPNL_SetSSC_En
{
    MS_BOOL bEnable;
    APIPNL_Result eReturn;
} stPNL_SetSSC_En, *pstPNL_SetSSC_En;

typedef struct _stPNL_SetSSC_Fmodulation
{
    MS_U16 u16Fmodulation;
    APIPNL_Result eReturn;
} stPNL_SetSSC_Fmodulation, *pstPNL_SetSSC_Fmodulation;

typedef struct _stPNL_SetSSC_Rdeviation
{
    MS_U16 u16Rdeviation;
    APIPNL_Result eReturn;
} stPNL_SetSSC_Rdeviation, *pstPNL_SetSSC_Rdeviation;

typedef struct _stPNL_SetOSDSSC_En
{
    MS_BOOL bEnable;
    APIPNL_Result eReturn;
} stPNL_SetOSDSSC_En, *pstPNL_SetOSDSSC_En;

typedef struct _stPNL_SetOSDSSC_Fmodulation
{
    MS_U16 u16Fmodulation;
    APIPNL_Result eReturn;
} stPNL_SetOSDSSC_Fmodulation, *pstPNL_SetOSDSSC_Fmodulation;

typedef struct _stPNL_SetOSDSSC_Rdeviation
{
    MS_U16 u16Rdeviation;
    APIPNL_Result eReturn;
} stPNL_SetOSDSSC_Rdeviation, *pstPNL_SetOSDSSC_Rdeviation;

typedef struct _stPNL_SkipTimingChange
{
    MS_BOOL bFlag;
    APIPNL_Result eResult;
} stPNL_SkipTimingChange, *pstPNL_SkipTimingChange;

typedef struct _stPNL_PreSetModeOn
{
    MS_BOOL bSetMode;
    APIPNL_Result eResult;
} stPNL_PreSetModeOn, *pstPNL_PreSetModeOn;

typedef struct DLL_PACKED  _stPNL_OverDriver_Init
{
    MS_PNL_OD_INITDATA *pPNL_ODInitData;
    MS_U32 u32ODInitDataLen;
    APIPNL_Result eResult;
} stPNL_OverDriver_Init, *pstPNL_OverDriver_Init;

typedef struct DLL_PACKED  _stPNL_OverDriver_Setting
{
    MS_PNL_OD_SETTING *pPNL_ODSetting;
    MS_U32 u32ODInitDataLen;
    APIPNL_Result eResult;
} stPNL_OverDriver_Setting, *pstPNL_OverDriver_Setting;

typedef struct DLL_PACKED  _stPNL_Set_File_Path
{
    E_PNL_FILE_PATH enPNLFilePath;
    char* pPNLFilePath;
    APIPNL_Result eResult;
} stPNL_Set_File_Path, *pstPNL_Set_File_Path;

typedef struct DLL_PACKED  _stPNL_SetPNLGammaBINTbl
{
    MS_U8* pu8GammaTab;
    MS_U32 u32Size;
    MS_U8 u8Index;
    MS_BOOL bReturn;
} stPNL_SetPNLGammaBINTbl, *pstPNL_SetPNLGammaBINTbl;

typedef struct _stPNL_OverDriver_Enable
{
    MS_BOOL bEnable;
    APIPNL_Result eResult;
} stPNL_OverDriver_Enable, *pstPNL_OverDriver_Enable;

typedef struct _stPNL_Get_LPLL_Type
{
    MS_U8 u8Return;
} stPNL_Get_LPLL_Type, *pstPNL_Get_LPLL_Type;

typedef struct _stPNL_Get_TCON_Capability
{
    MS_BOOL bReturn;
} stPNL_Get_TCON_Capability, *pstPNL_Get_TCON_Capability;

typedef struct _stPNL_SetPairSwap
{
    MS_U32 u32Polarity;
} stPNL_SetPairSwap, *pstPNL_SetPairSwap;

typedef struct _stPNL_SetExt_LPLL_Type
{
    MS_U16 u16Ext_lpll_type;
} stPNL_SetExt_LPLL_Type, *pstPNL_SetExt_LPLL_Type;

typedef struct _stPNL_CalExtLPLLSETbyDClk
{
    MS_U32 ldHz;
} stPNL_CalExtLPLLSETbyDClk, *pstPNL_CalExtLPLLSETbyDClk;

typedef struct _stPNL_SetDiffSwingLevel
{
    MS_U8 u8Swing_Level;
    MS_BOOL bReturn;
} stPNL_SetDiffSwingLevel, *pstPNL_SetDiffSwingLevel;

typedef struct _stPNL_SetPowerState
{
    EN_POWER_MODE ePowerState;
    MS_U32 u32Return;
} stPNL_SetPowerState, *pstPNL_SetPowerState;

typedef struct _stPNL_ForceSetPanelDCLK
{
    MS_U16 u16PanelDCLK;
    MS_BOOL bSetDCLKEnable;
    MS_BOOL bReturn;
} stPNL_ForceSetPanelDCLK, *pstPNL_ForceSetPanelDCLK;

typedef struct _stPNL_ForceSetPanelHStart
{
    MS_U16 u16PanelHStart;
    MS_BOOL bSetHStartEnable;
    MS_BOOL bReturn;
} stPNL_ForceSetPanelHStart, *pstPNL_ForceSetPanelHStart;

typedef struct _stPNL_EnableInternalTermination
{
    MS_BOOL bEnable;
    MS_BOOL bReturn;
} stPNL_EnableInternalTermination, *pstPNL_EnableInternalTermination;

typedef struct _stPNL_OutputDeviceHandshake
{
    MS_BOOL bReturn;
} stPNL_OutputDeviceHandshake, *pstPNL_OutputDeviceHandshake;

typedef struct _stPNL_OutputDeviceOCHandshake
{
    MS_BOOL bReturn;
} stPNL_OutputDeviceOCHandshake, *pstPNL_OutputDeviceOCHandshake;

typedef struct _stPNL_SetOutputInterlaceTiming
{
    MS_BOOL bEnable;
    APIPNL_Result eReturn;
} stPNL_SetOutputInterlaceTiming, *pstPNL_SetOutputInterlaceTiming;

typedef struct _stPNL_GetOutputInterlaceTiming
{
    MS_BOOL bReturn;
} stPNL_GetOutputInterlaceTiming, *pstPNL_GetOutputInterlaceTiming;

typedef struct DLL_PACKED  _stPNL_GetPanelData
{
    PanelType* pstPNLData;
} stPNL_GetPanelData, *pstPNL_GetPanelData;

typedef struct _stPNL_SetSSC
{
    MS_U16 u16Fmodulation;
    MS_U16 u16Rdeviation;
    MS_BOOL bEnable;
} stPNL_SetSSC, *pstPNL_SetSSC;

typedef struct _stPNL_GetPanelOnTiming
{
    APIPNL_POWER_TIMING_SEQ seq;
    MS_U16 u16Return;
} stPNL_GetPanelOnTiming, *pstPNL_GetPanelOnTiming;

typedef struct _stPNL_GetPanelOffTiming
{
    APIPNL_POWER_TIMING_SEQ seq;
    MS_U16 u16Return;
} stPNL_GetPanelOffTiming, *pstPNL_GetPanelOffTiming;

typedef struct _stPNL_GetPanelDimCtrl
{
    APIPNL_DIMMING_CTRL dim_type;
    MS_U8 u8Return;
} stPNL_GetPanelDimCtrl, *pstPNL_GetPanelDimCtrl;

typedef struct DLL_PACKED  _stPNL_GetAllGammaTbl
{
    MS_U8** ppu8Return;
} stPNL_GetAllGammaTbl, *pstPNL_GetAllGammaTbl;

typedef struct _stPNL_EnablePanel
{
    MS_BOOL bPanelOn;
    MS_BOOL bReturn;
} stPNL_EnablePanel, *pstPNL_EnablePanel;

typedef struct  DLL_PACKED  _stPNL_SetGammaTbl
{
    APIPNL_GAMMA_TYPE eGammaType;
    MS_U8* pu8GammaTab[3];
    APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode;
    MS_BOOL bReturn;
} stPNL_SetGammaTbl, *pstPNL_SetGammaTbl;

typedef struct DLL_PACKED  _stPNL_GetGammaTbl
{
    APIPNL_GAMMA_TYPE eGammaType;
    MS_U8* pu8GammaTab[3];
    APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode;
    MS_BOOL bReturn;
} stPNL_GetGammaTbl, *pstPNL_GetGammaTbl;

typedef struct DLL_PACKED  _stPNL_GetGammaTblEx
{
    ST_PNL_GAMMATBL *pstGammaTbl;
    MS_BOOL bReturn;
} ST_PNL_GETGAMMATBLEX, *pstPNL_GETGAMMATBLEX;

typedef struct DLL_PACKED  _stPNL_SetPNLGammaTbl
{
    MS_U8* pu8GammaTab;
    MS_U8 u8Index;
    MS_BOOL bReturn;
} stPNL_SetPNLGammaTbl, *pstPNL_SetPNLGammaTbl;

typedef struct _stPNL_SetGammaValue
{
    MS_U8 u8Channel;
    MS_U16 u16Offset;
    MS_U16 u16GammaValue;
    MS_BOOL bReturn;
} stPNL_SetGammaValue, *pstPNL_SetGammaValue;

typedef struct _stPNL_GetLPLLMode
{
    MS_U8 u8Return;
} stPNL_GetLPLLMode, *pstPNL_GetLPLLMode;

typedef struct _stPNL_GetDefVFreqe
{
    MS_U16 u16Return;
} stPNL_GetDefVFreq, *pstPNL_GetDefVFreq;

typedef struct DLL_PACKED  _stPNL_GetName
{
    const char* pPNLName;
}stPNL_GetName, *pstPNL_GetName;

typedef struct _stPNL_GetHstart
{
    MS_U16 u16Return;
} stPNL_GetHstart, *pstPNL_GetHstart;

typedef struct _stPNL_GetVstart
{
    MS_U16 u16Return;
} stPNL_GetVstart, *pstPNL_GetVstart;

typedef struct _stPNL_GetWidth
{
    MS_U16 u16Return;
} stPNL_GetWidth, *pstPNL_GetWidth;

typedef struct _stPNL_GetHeight
{
    MS_U16 u16Return;
} stPNL_GetHeight, *pstPNL_GetHeight;

typedef struct _stPNL_GetHtotal
{
    MS_U16 u16Return;
} stPNL_GetHtotal, *pstPNL_GetHtotal;

typedef struct _stPNL_GetVtotal
{
    MS_U16 u16Return;
} stPNL_GetVtotal, *pstPNL_GetVtotal;

typedef struct stPNL_GetHsyncWidth
{
    MS_U8 u8Return;
} stPNL_GetHsyncWidth,*pstPNL_GetHsyncWidth;

typedef struct _stPNL_GetHsyncBackPorch
{
    MS_U8 u8Return;
} stPNL_GetHsyncBackPorch,*pstPNL_GetHsyncBackPorch;

typedef struct _stPNL_GetVsyncBackPorch
{
    MS_U8 u8Return;
} stPNL_GetVsyncBackPorch,*pstPNL_GetVsyncBackPorch;

typedef struct _stPNL_GetLPLLType
{
    MS_U8 u8Return;
} stPNL_GetLPLLType,*pstPNL_GetLPLLType;

typedef struct _stPNL_GetARC
{
    E_PNL_ASPECT_RATIO ePanelAspectRatio;
} stPNL_GetARC,*pstPNL_GetARC;

typedef struct _stPNL_GetMinSET
{
    MS_U32 u32Return;
} stPNL_GetMinSET,*pstPNL_GetMinSET;

typedef struct _stPNL_GetMaxSET
{
    MS_U32 u32Return;
} stPNL_GetMaxSET,*pstPNL_GetMaxSET;

typedef struct _stPNL_GetOutTimingMode
{
    APIPNL_OUT_TIMING_MODE ePanelOutTimingMode;
} stPNL_GetOutTimingMode,*pstPNL_GetOutTimingMode;

typedef struct _stPNL_GetSupportMaxDclk
{
    MS_U16 u16Return;
} stPNL_GetSupportMaxDclk,*pstPNL_GetSupportMaxDclk;

typedef struct _stPNL_Init_LocalDimming
{
    pstPNL_Init_LocalDimming_Parameters pLD_InitData;
    MS_U16 u16DataLen;
    MS_U16 u16Return;
} stPNL_Init_LocalDimming, *pstPNL_Init_LocalDimming;

typedef struct _stPNL_CheckVBY1HandshakeStatus
{
    MS_BOOL bResult;
} stPNL_CheckVBY1HandshakeStatus,*pstPNL_CheckVBY1HandshakeStatus;

typedef struct _stPNL_MOD_Video_HWTrainingMode
{
    MS_BOOL bEnable;
}stPNL_MOD_Video_HWTrainingMode,*pstPNL_MOD_Video_HWTrainingMode;

typedef struct _stPNL_MOD_OSD_HWTrainingMode
{
    MS_BOOL bEnable;
}stPNL_MOD_OSD_HWTrainingMode,*pstPNL_MOD_OSD_HWTrainingMode;

typedef struct _stPNL_GetVideoHWTraining_status
{
    MS_BOOL bStatus;
}stPNL_GetVideoHWTraining_status,*pstPNL_GetVideoHWTraining_status;

typedef struct _stPNL_GetOSDHWTraining_status
{
    MS_BOOL bStatus;
}stPNL_GetOSDHWTraining_status,*pstPNL_GetOSDHWTraining_status;

typedef struct DLL_PACKED _stPNL_Setting
{
    MS_U32 u32Cmd;
    void *pCmdArgs;
    MS_U32 u32CmdArgsSize;
    APIPNL_Result eReturnValue;
}stPNL_Setting,*pstPNL_Setting;

typedef struct DLL_PACKED  _stPNL_Set_PolInvert_Threshold
{
    MS_BOOL bResult;
    MS_U8 u8OsdThreshold;
    MS_U8 u8VideoThreshold;
    MS_U8 u8MotionThreshold;
} stPNL_Set_PolInvert_Threshold, *pstPNL_Set_PolInvert_Threshold;

typedef struct DLL_PACKED  _stPNL_PolInvert
{
    MS_BOOL bResult;
} stPNL_PolInvert, *pstPNL_PolInvert;

typedef struct DLL_PACKED _stPNL_SET_PNLGammaTbl
{
    ST_PNL_PNLGAMMATBL *pstPNLGammaTbl;
    MS_BOOL bReturn;
} ST_PNL_SET_PNLGAMMATBL, *pstPNL_SET_PNLGAMMATBL;

typedef struct DLL_PACKED _stPNL_GET_PNLGammaTbl
{
    ST_PNL_PNLGAMMATBL *pstPNLGammaTbl;
    MS_BOOL bReturn;
} ST_PNL_GET_PNLGAMMATBL, *pstPNL_GET_PNLGAMMATBL;
#ifdef __cplusplus
}
#endif

#endif
