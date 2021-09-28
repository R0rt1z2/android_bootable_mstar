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
/// @brief  AVD Driver private Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_AVD_PRIV_H_
#define _DRV_AVD_PRIV_H_

#include "drvAVD_v2.h"

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
typedef enum{
    MDBCMD_Reset_DSP_CODE = 0x0,
    MDBCMD_Reset_AFEC_HW = 0x1,
    MDBCMD_Reset_COMB = 0x2,
}AVD_MDBCMD_RESET;

typedef enum{
    MDBCMD_PATTERN_CANCEL = 0x0,
    MDBCMD_PATTERN_ROW = 0x1,
    MDBCMD_PATTERN_COLUMN = 0x2,
}AVD_MDBCMD_PATTERN;

typedef enum{
    MDBCMD_FORCE_CANCEL = 0x0,
    MDBCMD_FORCE_PAL = 0x1,
    MDBCMD_FORCE_SECAM = 0x2,
    MDBCMD_FORCE_NTSC= 0x3,
    MDBCMD_FORCE_NTSC443 = 0x4,
    MDBCMD_FORCE_PAL_M = 0x5,
    MDBCMD_FORCE_PAL_60 = 0x6,
    MDBCMD_FORCE_PAL_NC = 0x7,
}AVD_MDBCMD_FORCE_STANDARD;

typedef enum{
    MDBCMD_LEVEL_CANCEL = 0x0,
    MDBCMD_LEVEL_44 = 0x1,
    MDBCMD_LEVEL_88 = 0x2,
    MDBCMD_LEVEL_CC= 0x3,
    MDBCMD_LEVEL_FF = 0x4,
}AVD_MDBCMD_SLICE_LEVEL;

typedef enum{
    MDBCMD_PLL_CANCEL = 0x0,
    MDBCMD_PLL_Level1 = 0x1, // 0x82/0x04
    MDBCMD_PLL_Level2 = 0x2, //0x90/0x20
    MDBCMD_PLL_Level3 = 0x3, //0x9A/0x35
    MDBCMD_PLL_Level4 = 0x4, //0xAA/0x6A
    MDBCMD_PLL_Level5 = 0x5, //0xBC/0x6A
}AVD_MDBCMD_HSYNC_PLL;

typedef enum{
    MDBCMD_COMB_CANCEL = 0x0,
    MDBCMD_COMB_ByPassComb_Filter = 0x1,
}AVD_MDBCMD_COMB;

typedef enum{
    MDBCMD_Window_Start = 0x1,
    MDBCMD_Window_End = 0x2,
}AVD_MDBCMD_BurstWindow;

typedef enum{
    MDBCMD_FixGain_CANCEL = 0x0,
    MDBCMD_FixGain_ENABLE = 0x1,
}AVD_MDBCMD_FixGain;

/// AVD status indicator
typedef enum
{
    MDBCMD_STATUS_OK,             //AVD status OK
    MDBCMD_STATUS_ERROR,          // AVD status ERROR
    MDBCMD_NOT_SUPPORT,           //AVD status NOT SUPPORT
} AVD_MDBCMD_STATUS;
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern AVD_Result Drv_AVD_Init(VD_INITDATA *pVD_InitData, MS_U32 u32InitDataLen, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_Exit(AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_ResetMCU(AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_FreezeMCU(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource);

extern MS_U16 Drv_AVD_Scan_HsyncCheck(MS_U8 u8HtotalTolerance, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_StartAutoStandardDetection(AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_ForceVideoStandard(AVD_VideoStandardType eVideoStandardType, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_3DCombSpeedup(AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_LoadDSP(MS_U8 *pu8VD_DSP, MS_U32 len, AVD_RESOURCE_PRIVATE *pResource);

extern MS_U8 Drv_AVD_MBX_ReadByteByVDMbox(MS_U8 u8Addr, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_BackPorchWindowPositon(MS_BOOL bEnable , MS_U8 u8Value);


extern void Drv_AVD_SetFlag(MS_U32  u32VDPatchFlag, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetRegValue(MS_U16 u16Addr,MS_U8 u8Value);

extern void Drv_AVD_SetRegFromDSP(AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_SetInput(AVD_InputSourceType eSource, MS_U8 u8ScartFB, AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_SetVideoStandard(AVD_VideoStandardType eStandard, MS_BOOL bIsInAutoTuning, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetChannelChange(AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetHsyncDetectionForTuning(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_Set3dComb(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetShiftClk(MS_BOOL bEnable,AVD_ATV_CLK_TYPE eShiftMode, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetFreerunPLL(AVD_VideoFreq eVideoFreq, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetFreerunFreq(AVD_FreeRunFreq eFreerunfreq, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetFactoryPara(AVD_Factory_Para FactoryPara,MS_U8 u8Value, AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_SetDbgLevel(AVD_DbgLv u8DbgLevel, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetPQFineTune(AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_Set3dCombSpeed(MS_U8 u8COMB57, MS_U8 u8COMB58, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetStillImageParam(AVD_Still_Image_Param param, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_Set2D3DPatchOnOff(MS_BOOL bEnable);

extern MS_U8 Drv_AVD_SetAutoFineGainToFixed(void);


extern MS_U8 Drv_AVD_GetRegValue(MS_U16 u16Addr);

extern MS_U16 Drv_AVD_GetStatus(void);

extern MS_U8 Drv_AVD_GetNoiseMag(void);

extern MS_U16 Drv_AVD_GetVTotal(void);

extern AVD_VideoStandardType Drv_AVD_GetStandardDetection(MS_U16 *u16LatchStatus, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_GetCaptureWindow(void *stCapWin, AVD_VideoStandardType eVideoStandardType , AVD_RESOURCE_PRIVATE *pResource);

extern AVD_VideoFreq Drv_AVD_GetVerticalFreq(void);

extern MS_U8 Drv_AVD_GetHsyncEdge(void);

extern MS_U8 Drv_AVD_GetDSPFineGain(AVD_RESOURCE_PRIVATE *pResource);

extern MS_U16 Drv_AVD_GetDSPVersion(void);

extern AVD_Result Drv_AVD_GetLibVer(const MSIF_Version **ppVersion);

extern void  Drv_AVD_GetInfo(AVD_Info* pAVD_Info, AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_IsSyncLocked(void);

extern MS_BOOL Drv_AVD_IsSignalInterlaced(void);

extern MS_BOOL Drv_AVD_IsColorOn(void);

extern MS_U32 Drv_AVD_SetPowerState(EN_POWER_MODE u16PowerState, AVD_RESOURCE_PRIVATE *pResource);

extern MS_BOOL Drv_AVD_GetMacroVisionDetect(void);

extern MS_BOOL Drv_AVD_GetCGMSDetect(void);

extern void Drv_AVD_SetBurstWinStart(MS_U8 u8BusrtStartPosition);

extern MS_BOOL Drv_AVD_IsAVDAlive(void);

extern MS_BOOL Drv_AVD_IsLockAudioCarrier(void);

extern MS_BOOL Drv_AVD_DBG_EchoCmd(MS_U64* pu64ReqHdl, MS_U32 u32CmdSize, char* pcmd);

extern MS_BOOL Drv_AVD_DBG_GetModuleInfo(MS_U64* pu64ReqHdl);

extern char* Drv_AVD_Str_Tok(char* pstrSrc, char* pstrDes, char delimit);

extern int Drv_AVD_StrToHex(char* pstrSrc);

extern void Drv_AVD_SetAnaDemod(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource);

extern void Drv_AVD_SetHsyncSensitivityForDebug(MS_U8 u8AFEC99, MS_U8 u8AFEC9C, AVD_RESOURCE_PRIVATE *pResource);
//-------------------------------- functions ------------------------------------------------------------

typedef AVD_Result (*IOCTL_AVD_Init)(VD_INITDATA *, MS_U32, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_Exit)(AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_ResetMCU)(AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_FreezeMCU)(MS_BOOL, AVD_RESOURCE_PRIVATE *);
typedef MS_U16 (*IOCTL_AVD_Scan_HsyncCheck)(MS_U8, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_StartAutoStandardDetection)(AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_ForceVideoStandard)(AVD_VideoStandardType, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_3DCombSpeedup)(AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_LoadDSP)(MS_U8 *, MS_U32, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_BackPorchWindowPositon)(MS_BOOL , MS_U8);
typedef MS_U8 (*IOCTL_AVD_MBX_ReadByteByVDMbox)(MS_U8, AVD_RESOURCE_PRIVATE *);

typedef void (*IOCTL_AVD_SetFlag)(MS_U32, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetRegValue)(MS_U16 ,MS_U8);
typedef void (*IOCTL_AVD_SetRegFromDSP)(AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_SetInput)(AVD_InputSourceType , MS_U8, AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_SetVideoStandard)(AVD_VideoStandardType , MS_BOOL, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetChannelChange)(AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetHsyncDetectionForTuning)(MS_BOOL, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_Set3dComb)(MS_BOOL , AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetShiftClk)(MS_BOOL ,AVD_ATV_CLK_TYPE, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetFreerunPLL)(AVD_VideoFreq, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetFreerunFreq)(AVD_FreeRunFreq, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetFactoryPara)(AVD_Factory_Para ,MS_U8,AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_SetDbgLevel)(AVD_DbgLv, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetPQFineTune)(AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_Set3dCombSpeed)(MS_U8 , MS_U8, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetStillImageParam)(AVD_Still_Image_Param, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_Set2D3DPatchOnOff)(MS_BOOL);
typedef MS_U8 (*IOCTL_AVD_SetAutoFineGainToFixed)(void);

typedef MS_U8 (*IOCTL_AVD_GetRegValue)(MS_U16);
typedef MS_U16 (*IOCTL_AVD_GetStatus)(void);
typedef MS_U8 (*IOCTL_AVD_GetNoiseMag)(void);
typedef MS_U16 (*IOCTL_AVD_GetVTotal)(void);
typedef AVD_VideoStandardType (*IOCTL_AVD_GetStandardDetection)(MS_U16 *, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_GetCaptureWindow)(void *, AVD_VideoStandardType, AVD_RESOURCE_PRIVATE *);
typedef AVD_VideoFreq (*IOCTL_AVD_GetVerticalFreq)(void);
typedef MS_U8 (*IOCTL_AVD_GetHsyncEdge)(void);
typedef MS_U8 (*IOCTL_AVD_GetDSPFineGain)(AVD_RESOURCE_PRIVATE *);
typedef MS_U16 (*IOCTL_AVD_GetDSPVersion)(void);
typedef AVD_Result (*IOCTL_AVD_GetLibVer)(const MSIF_Version **);
typedef void (*IOCTL_AVD_GetInfo)(AVD_Info*, AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_IsSyncLocked)(void);
typedef MS_BOOL (*IOCTL_AVD_IsSignalInterlaced)(void);
typedef MS_BOOL (*IOCTL_AVD_IsColorOn)(void);
typedef MS_U32 (*IOCTL_AVD_SetPowerState)(EN_POWER_MODE , AVD_RESOURCE_PRIVATE *);
typedef MS_BOOL (*IOCTL_AVD_GetMacroVisionDetect)(void);
typedef MS_BOOL (*IOCTL_AVD_GetCGMSDetect)(void);
typedef void (*IOCTL_AVD_SetBurstWinStart)(MS_U8);
typedef MS_BOOL (*IOCTL_AVD_IsAVDAlive)(void);
typedef MS_BOOL (*IOCTL_AVD_IsLockAudioCarrier)(void);
typedef void (*IOCTL_AVD_SetAnaDemod)(MS_BOOL, AVD_RESOURCE_PRIVATE *);
typedef void (*IOCTL_AVD_SetHsyncSensitivityForDebug)(MS_U8 ,MS_U8, AVD_RESOURCE_PRIVATE *);
typedef struct _AVD_INSTANT_PRIVATE
{
    IOCTL_AVD_Init                                      fpAVDInit;
    IOCTL_AVD_Exit                                     fpAVDExit;
    IOCTL_AVD_ResetMCU                            fpAVDResetMCU;
    IOCTL_AVD_FreezeMCU                           fpAVDFreezeMCU;
    IOCTL_AVD_Scan_HsyncCheck                 fpAVDScanHsyncCheck;
    IOCTL_AVD_StartAutoStandardDetection    fpAVDStartAutoStandardDetection;
    IOCTL_AVD_ForceVideoStandard               fpAVDForceVideoStandard;
    IOCTL_AVD_3DCombSpeedup                   fpAVD3DCombSpeedup;
    IOCTL_AVD_LoadDSP                               fpAVDLoadDSP;
    IOCTL_AVD_BackPorchWindowPositon       fpAVDBackPorchWindowPositon;
    IOCTL_AVD_MBX_ReadByteByVDMbox      fpAVDMBX_ReadByteByVDMbox;

    IOCTL_AVD_SetFlag                                 fpAVDSetFlag;
    IOCTL_AVD_SetRegValue                         fpAVDSetRegValue;
    IOCTL_AVD_SetRegFromDSP                    fpAVDSetRegFromDSP;
    IOCTL_AVD_SetInput                               fpAVDSetInput;
    IOCTL_AVD_SetVideoStandard                  fpAVDSetVideoStandard;
    IOCTL_AVD_SetChannelChange                fpAVDSetChannelChange;
    IOCTL_AVD_SetHsyncDetectionForTuning   fpAVDSetHsyncDetectionForTuning;
    IOCTL_AVD_Set3dComb                           fpAVDSet3dComb;
    IOCTL_AVD_SetShiftClk                            fpAVDSetShiftClk;
    IOCTL_AVD_SetFreerunPLL                        fpAVDSetFreerunPLL;
    IOCTL_AVD_SetFreerunFreq                      fpAVDSetFreerunFreq;
    IOCTL_AVD_SetFactoryPara                      fpAVDSetFactoryPara;
    IOCTL_AVD_SetDbgLevel                          fpAVDSetDbgLevel;
    IOCTL_AVD_SetPQFineTune                       fpAVDSetPQFineTune;
    IOCTL_AVD_Set3dCombSpeed                   fpAVDSet3dCombSpeed;
    IOCTL_AVD_SetStillImageParam                fpAVDSetStillImageParam;
    IOCTL_AVD_Set2D3DPatchOnOff                fpAVDSet2D3DPatchOnOff;
    IOCTL_AVD_SetAutoFineGainToFixed          fpAVDSetAutoFineGainToFixed;

    IOCTL_AVD_GetRegValue                          fpAVDGetRegValue;
    IOCTL_AVD_GetStatus                              fpAVDGetStatus;
    IOCTL_AVD_GetNoiseMag                          fpAVDGetNoiseMag;
    IOCTL_AVD_GetVTotal                              fpAVDGetVTotal;
    IOCTL_AVD_GetStandardDetection             fpAVDGetStandardDetection;
    IOCTL_AVD_GetCaptureWindow                 fpAVDGetCaptureWindow;
    IOCTL_AVD_GetVerticalFreq                      fpAVDGetVerticalFreq;
    IOCTL_AVD_GetHsyncEdge                        fpAVDGetHsyncEdge;
    IOCTL_AVD_GetDSPFineGain                     fpAVDGetDSPFineGain;
    IOCTL_AVD_GetDSPVersion                      fpAVDGetDSPVersion;
    IOCTL_AVD_GetLibVer                              fpAVDGetLibVer;
    IOCTL_AVD_GetInfo                                 fpAVDGetInfo;
    IOCTL_AVD_IsSyncLocked                       fpAVDIsSyncLocked;
    IOCTL_AVD_IsSignalInterlaced                 fpAVDIsSignalInterlaced;
    IOCTL_AVD_IsColorOn                             fpAVDIsColorOn;
    IOCTL_AVD_SetPowerState                       fpAVDSetPowerState;
    IOCTL_AVD_GetMacroVisionDetect         fpAVDGetMacroVisionDetect;
    IOCTL_AVD_GetCGMSDetect                  fpAVDGetCGMSDetect;
    IOCTL_AVD_SetBurstWinStart                    fpAVDSetBurstWinStart;
    IOCTL_AVD_IsAVDAlive                  fpAVDIsAVDAlive;
    IOCTL_AVD_IsLockAudioCarrier                    fpAVDIsLockAudioCarrier;
    IOCTL_AVD_SetAnaDemod                       fpAVDSetAnaDemod;
    IOCTL_AVD_SetHsyncSensitivityForDebug                         fpAVDSetHsyncSensitivityForDebug;
}AVD_INSTANT_PRIVATE;


void AVDRegisterToUtopia(void);
MS_U32 AVDOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 AVDClose(void* pInstantTmp);
MS_U32 AVDIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
MS_U32 AVDStr(MS_U32 u32PowerState, void* pModule);
void vd_str_stablestatus(void);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32  AVDMdbIoctl(MS_U32 u32Cmd, const void* const pArgs);
#endif

#endif // _DRV_AVD_PRIV_H_
