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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  System Control Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef _DRVSYS_PRIV_H_
#define _DRVSYS_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "tee_client_api.h"
// driver version
#define SYS_VERSION            0x00000001

typedef enum {
    // Do not need to check resource
    MDrv_CMD_SYS_GetChipRev,
    MDrv_CMD_SYS_GetChipID,
    MDrv_CMD_SYS_GetInfo,
    MDrv_CMD_SYS_GetLibVer,
    MDrv_CMD_SYS_GetStatus,
    MDrv_CMD_SYS_GetSoftwareVersion,
    MDrv_CMD_SYS_DisableDebugPort,
    MDrv_CMD_SYS_EnableDebugPort,
    MDrv_CMD_SYS_SetDbgLevel,

    // check if resource left
    MDrv_CMD_SYS_WDTEnable,
    MDrv_CMD_SYS_WDTClear,
    MDrv_CMD_SYS_WDTLastStatus,
    MDrv_CMD_SYS_WDTSetTime,
    MDrv_CMD_SYS_ResetChip,
    MDrv_CMD_SYS_ResetCPU,
    MDrv_CMD_SYS_SetAGCPadMux,
    MDrv_CMD_SYS_SetPCMCardDetectMode,
    MDrv_CMD_SYS_SetPadMux,
    MDrv_CMD_SYS_SetTSOutClockPhase,
    MDrv_CMD_SYS_Query,
#ifdef _SYS_DAC_GAIN_CTRL
    MDrv_CMD_SYS_EnhanceDACGain,
#endif
    MDrv_CMD_SYS_Init,
    MDrv_CMD_SYS_GlobalInit,
    MDrv_CMD_SYS_TeecInitContext,
    MDrv_CMD_SYS_TeecOpen,
    MDrv_CMD_SYS_TeecClose,
    MDrv_CMD_SYS_TeecInvokeCmd,
    MDrv_CMD_SYS_TeecFinalContext,
} eSYSIoctlOpt;

typedef enum
{
        E_SYS_SOURCE_DTV     = 0x00,
        E_SYS_SOURCE_HDMI,
        E_SYS_SOURCE_MM,
        E_SYS_SOURCE_GOOGLE_CAST,
        E_SYS_SOURCE_DOLBY_HDMI,
        E_SYS_SOURCE_MAX,
} E_SYS_SOURCE_TYPE;

typedef enum
{
        E_SYS_INPUT_FHD24_25_30    = 0x00,
        E_SYS_INPUT_FHD48,
        E_SYS_INPUT_FHD50_60,
        E_SYS_INPUT_4K2K24_25_30,
        E_SYS_INPUT_4K2K48,
        E_SYS_INPUT_4K2K50_60,
        E_SYS_INPUT_ALWAYS_ON,
        E_SYS_INPUT_ALWAYS_OFF,
        E_SYS_INPUT_FHD_UNKNOWN,
        E_SYS_INPUT_4K2K120,
        E_SYS_INPUT_4K2K_UNKNOWN,
        E_SYS_INPUT_MAX,
} E_SYS_INPUT_TIMING;


//
// Input Source Mapping,
// Ref: mapi_types.h
//
typedef enum
{
    E_SYS_INPUT_SOURCE_VGA = 0,
    E_SYS_INPUT_SOURCE_ATV = 1,

    E_SYS_INPUT_SOURCE_CVBS = 2,        ///<AV 1           2
    E_SYS_INPUT_SOURCE_CVBS2= 3,        ///<AV 2           3
    E_SYS_INPUT_SOURCE_CVBS3= 4,        ///<AV 3           4
    E_SYS_INPUT_SOURCE_CVBS4= 5,        ///<AV 4           5
    E_SYS_INPUT_SOURCE_CVBS5= 6,        ///<AV 5           6
    E_SYS_INPUT_SOURCE_CVBS6= 7,        ///<AV 6           7
    E_SYS_INPUT_SOURCE_CVBS7= 8,        ///<AV 7           8
    E_SYS_INPUT_SOURCE_CVBS8= 9,

    E_SYS_INPUT_SOURCE_SVIDEO = 11,      ///<S-video 1      11
    E_SYS_INPUT_SOURCE_SVIDEO2= 12,      ///<S-video 2      12
    E_SYS_INPUT_SOURCE_SVIDEO3= 13,      ///<S-video 3      13
    E_SYS_INPUT_SOURCE_SVIDEO4= 14,      ///<S-video 4      14

    E_SYS_INPUT_SOURCE_YPBPR = 16,       ///<Component 1    16
    E_SYS_INPUT_SOURCE_YPBPR2= 17,       ///<Component 2    17
    E_SYS_INPUT_SOURCE_YPBPR3= 18,       ///<Component 3    18

    E_SYS_INPUT_SOURCE_SCART = 20,       ///<Scart 1         20
    E_SYS_INPUT_SOURCE_SCART2= 21,       ///<Scart 2         21

    E_SYS_INPUT_SOURCE_HDMI = 23,         ///<HDMI 1          23
    E_SYS_INPUT_SOURCE_HDMI2= 24,         ///<HDMI 2          24
    E_SYS_INPUT_SOURCE_HDMI3= 25,         ///<HDMI 3          25
    E_SYS_INPUT_SOURCE_HDMI4= 26,         ///<HDMI 4          26

    E_SYS_INPUT_SOURCE_DTV  = 28,         ///<DTV             28

    E_SYS_INPUT_SOURCE_DVI = 29,         ///<DVI 1            29
    E_SYS_INPUT_SOURCE_DVI2= 30,         ///<DVI 2            30
    E_SYS_INPUT_SOURCE_DVI3= 31,         ///<DVI 2            31
    E_SYS_INPUT_SOURCE_DVI4= 32,         ///<DVI 4            32

    E_SYS_INPUT_SOURCE_STORAGE = 34,      ///<Storage         34
    E_SYS_INPUT_SOURCE_DTV2 = 37,         ///<DTV2            37
    E_SYS_INPUT_SOURCE_STORAGE2 = 38,     ///<Storage2        38
    E_SYS_INPUT_SOURCE_DTV3 = 39,         ///<DTV3            39
    E_SYS_INPUT_SOURCE_VGA2 = 42,         ///<VGA2 input      42
    E_SYS_INPUT_SOURCE_VGA3 = 43,         ///<VGA3 input      43
    E_SYS_INPUT_SOURCE_GOOGLE_CAST = 44,  ///<Google Cast     44
} E_SYS_INPUT_SOURCE_TYPE;

//ioctl function pointer type
typedef MS_U8 (*IOCTL_SYS_GETCHIPREV)(void);
typedef MS_U16 (*IOCTL_SYS_GETCHIPID)(void);
typedef const SYS_Info* (*IOCTL_SYS_GETINFO)(void);
typedef const MSIF_Version* (*IOCTL_SYS_GETLIBVER)(void);
typedef MS_BOOL (*IOCTL_SYS_GETSTATUS)(SYS_Status *);
typedef void (*IOCTL_SYS_GETSOFTWAREVERSION)(MS_SW_VERSION_INFO *);
typedef MS_BOOL (*IOCTL_SYS_DISABLEDEBUGPORT)(void);
typedef MS_BOOL (*IOCTL_SYS_ENABLEDEBUGPORT)(void);
typedef void (*IOCTL_SYS_SETDBGLEVEL)(SYS_DbgLv);
typedef void (*IOCTL_SYS_WDTENABLE)(MS_BOOL);
typedef void (*IOCTL_SYS_WDTCLEAR)(void);
typedef MS_BOOL (*IOCTL_SYS_WDTLASTSTATUS)(void);
typedef void (*IOCTL_SYS_WDTSETTIME)(MS_U32);
typedef void (*IOCTL_SYS_RESETCHIP)(void);
typedef void (*IOCTL_SYS_RESETCPU)(void);
typedef void (*IOCTL_SYS_SETAGCPADMUX)(SYS_AGC_PAD_SET);
typedef MS_BOOL (*IOCTL_SYS_SETPCMCARDDETECTMODE)(SYS_PCM_CD_MODE);
typedef MS_BOOL (*IOCTL_SYS_SETPADMUX)(SYS_PAD_MUX_SET, SYS_PAD_SEL);
typedef MS_BOOL (*IOCTL_SYS_SETTSOUTCLOCKPHASE)(MS_U16);
typedef MS_BOOL (*IOCTL_SYS_QUERY)(E_SYS_QUERY);
#ifdef _SYS_DAC_GAIN_CTRL
typedef void (*IOCTL_SYS_ENHANCEDACGAIN)(MS_U8, MS_BOOL);
#endif
typedef MS_BOOL (*IOCTL_SYS_INIT)(void);
typedef void (*IOCTL_SYS_GLOBALINIT)(void);
typedef MS_U32 (*IOCTL_SYS_TEEC_INIT_CONTEXT)(const char *, TEEC_Context *);
typedef MS_U32 (*IOCTL_SYS_TEEC_OPEN)(TEEC_Context *, TEEC_Session *, const TEEC_UUID *, MS_U32, const void *, TEEC_Operation *, MS_U32 *);
typedef void (*IOCTL_SYS_TEEC_CLOSE)(TEEC_Session *);
typedef void (*IOCTL_SYS_TEEC_FINAL_CONTEXT)(TEEC_Context *);
typedef MS_U32 (*IOCTL_SYS_TEEC_INVOKE_CMD)(TEEC_Session *, MS_U32, TEEC_Operation *, MS_U32 *);

// INSTANCE Private(ioctl function pointer)
typedef struct _SYS_INSTANT_PRIVATE
{
    IOCTL_SYS_GETCHIPREV                    fpSYS_GetChipRev;
    IOCTL_SYS_GETCHIPID                     fpSYS_GetChipID;
    IOCTL_SYS_GETINFO                       fpSYS_GetInfo;
    IOCTL_SYS_GETLIBVER                     fpSYS_GetLibVer;
    IOCTL_SYS_GETSTATUS                     fpSYS_GetStatus;
    IOCTL_SYS_GETSOFTWAREVERSION            fpSYS_GetSoftwareVersion;
    IOCTL_SYS_DISABLEDEBUGPORT              fpSYS_DisableDebugPort;
    IOCTL_SYS_ENABLEDEBUGPORT               fpSYS_EnableDebugPort;
    IOCTL_SYS_SETDBGLEVEL                   fpSYS_SetDbgLevel;
    IOCTL_SYS_WDTENABLE                     fpSYS_WDTEnable;
    IOCTL_SYS_WDTCLEAR                      fpSYS_WDTClear;
    IOCTL_SYS_WDTLASTSTATUS                 fpSYS_WDTLastStatus;
    IOCTL_SYS_WDTSETTIME                    fpSYS_WDTSetTime;
    IOCTL_SYS_RESETCHIP                     fpSYS_ResetChip;
    IOCTL_SYS_RESETCPU                      fpSYS_ResetCPU;
    IOCTL_SYS_SETAGCPADMUX                  fpSYS_SetAGCPadMux;
    IOCTL_SYS_SETPCMCARDDETECTMODE          fpSYS_SetPCMCardDetectMode;
    IOCTL_SYS_SETPADMUX                     fpSYS_SetPadMux;
    IOCTL_SYS_SETTSOUTCLOCKPHASE            fpSYS_SetTSOutClockPhase;
    IOCTL_SYS_QUERY                         fpSYS_Query;
#ifdef _SYS_DAC_GAIN_CTRL
    IOCTL_SYS_ENHANCEDACGAIN                fpSYS_EnhanceDACGain;
#endif
    IOCTL_SYS_INIT                          fpSYS_Init;
    IOCTL_SYS_GLOBALINIT                    fpSYS_GlobalInit;
    IOCTL_SYS_TEEC_INIT_CONTEXT             fpSYS_TeecInitContext;
    IOCTL_SYS_TEEC_OPEN                     fpSYS_TeecOpen;
    IOCTL_SYS_TEEC_CLOSE                    fpSYS_TeecClose;
    IOCTL_SYS_TEEC_INVOKE_CMD               fpSYS_TeecInvokeCmd;
    IOCTL_SYS_TEEC_FINAL_CONTEXT            fpSYS_TeecFinalizeContext;
}SYS_INSTANT_PRIVATE;

//////////////////////////////////////
// function parameter structure //////
//////////////////////////////////////
// for cmd MDrv_SYS_TEEC_InitializeContext_
typedef struct _SYS_TEECINITCONTEXT_PARAM
{
    const char *name;
    TEEC_Context *context;
}SYS_SYS_TEECINITCONTEXT_PARAM, *PSYS_SYS_TEECINITCONTEXT_PARAM;

// for cmd MDrv_SYS_TEEC_Open
typedef struct _SYS_TEECOPEN_PARAM
{
    TEEC_Context *context;
    TEEC_Session *session;
    const TEEC_UUID *destination;
    MS_U32 connection_method;
    const void *connection_data;
    TEEC_Operation *operation;
    MS_U32 *error_origin;
}SYS_TEECOPEN_PARAM, *PSYS_SYS_TEECOPEN_PARAM;

// for cmd MDrv_SYS_TEEC_Close
typedef struct _SYS_TEECCLOSE_PARAM
{
    TEEC_Session *session;
}SYS_TEECCLOSE_PARAM, *PSYS_SYS_TEECCLOSE_PARAM;

// for cmd MDrv_SYS_TEEC_InvokeCmd
typedef struct _SYS_TEECINVOKECMD_PARAM
{
    TEEC_Session *session;
    MS_U32 cmd_id;
    TEEC_Operation *operation;
    MS_U32 *error_origin;
}SYS_TEECINVOKECMD_PARAM, *PSYS_SYS_TEECINVOKECMD_PARAM;

// for cmd MDrv_SYS_GetStatus
typedef struct _SYS_GETSTATUS_PARAM
{
    SYS_Status *pStatus;
}SYS_GETSTATUS_PARAM, *PSYS_GETSTATUS_PARAM;

// for cmd MDrv_SYS_GetSoftwareVersion
typedef struct _SYS_GETSOFTWAREVERSION_PARAM
{
    MS_SW_VERSION_INFO *pSoftwareVersionInfo;
}SYS_GETSOFTWAREVERSION_PARAM, *PSYS_GETSOFTWAREVERSION_PARAM;

// for cmd MDrv_SYS_SetDbgLevel
typedef struct _SYS_SETDBGLEVEL_PARAM
{
    SYS_DbgLv eLevel;
}SYS_SETDBGLEVEL_PARAM, *PSYS_SETDBGLEVEL_PARAM;

// for cmd MDrv_SYS_WDTEnable
typedef struct _SYS_WDTENABLE_PARAM
{
    MS_BOOL bEnable;
}SYS_WDTENABLE_PARAM, *PSYS_WDTENABLE_PARAM;

// for cmd MDrv_SYS_WDTSetTime
typedef struct _SYS_WDTSETTIME_PARAM
{
    MS_U32 u32Ms;
}SYS_WDTSETTIME_PARAM, *PSYS_WDTSETTIME_PARAM;

// for cmd MDrv_SYS_SetAGCPadMux
typedef struct _SYS_SETAGCPADMUX_PARAM
{
    SYS_AGC_PAD_SET eAgcPadMux;
}SYS_SETAGCPADMUX_PARAM, *PSYS_SETAGCPADMUX_PARAM;

// for cmd MDrv_SYS_SetPCMCardDetectMode
typedef struct _SYS_SETPCMCARDDETECT_PARAM
{
    SYS_PCM_CD_MODE ePCMCDMode;
}SYS_SETPCMCARDDETECT_PARAM, *PSYS_SETPCMCARDDETECT_PARAM;

// for cmd MDrv_SYS_SetPadMux
typedef struct _SYS_SETPADMUX_PARAM
{
    SYS_PAD_MUX_SET ePadMuxType;
    SYS_PAD_SEL ePadSel;
}SYS_SETPADMUX_PARAM, *PSYS_SETPADMUX_PARAM;

// for cmd MDrv_SYS_SetTSOutClockPhase
typedef struct _SYS_SETTSOUTCLOCKPHASE_PARAM
{
    MS_U16 u16Val;
}SYS_SETTSOUTCLOCKPHASE_PARAM, *PSYS_SETTSOUTCLOCKPHASE_PARAM;

// for cmd MDrv_SYS_Query
typedef struct _SYS_QUERY_PARAM
{
    E_SYS_QUERY id;
}SYS_QUERY_PARAM, *PSYS_QUERY_PARAM;

#ifdef _SYS_DAC_GAIN_CTRL
// for cmd MDrv_SYS_EnhanceDACGain
typedef struct _SYS_ENHANCEDACGAIN_PARAM
{
    MS_U8 u8DAC_ID;
    MS_BOOL bEnable;
}SYS_ENHANCEDACGAIN_PARAM, *PSYS_ENHANCEDACGAIN_PARAM;
#endif

typedef struct _SYS_IO_PROC
{
    MS_BOOL (*SysSwI2CReadBytes)(MS_U16, MS_U8, MS_U8*, MS_U16, MS_U8*);
    MS_BOOL (*SysSwI2CWriteBytes)(MS_U16, MS_U8, MS_U8*, MS_U16, MS_U8*);
} SYS_IO_PROC;

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT

#ifdef MSOS_TYPE_LINUX_KERNEL
#define mst_atoi(str) simple_strtoul(((str != NULL) ? str : ""), NULL, 0);
#else
#define mst_atoi(str) strtoul(((str != NULL) ? str : ""), NULL, 0);
#endif

#define RIU_INFO_CMD_LEN 16
#define RIU8BIT(BASE,BANK,OFFSET)   (BASE + ((BANK<<9)&0xFFFFF) + ((OFFSET&(~BIT(0)))<<1))
#define RIU16BIT(BASE,BANK,OFFSET)  (BASE + ((BANK<<9)&0xFFFFF) + (OFFSET<<2))
#define RIU8BIT_BOUND   0x100
#define RIU16BIT_BOUND  0x80
#define OUT_OF_RANGE(value,bound)   if(value>=bound) \
                                    { \
                                        break; \
                                    }

typedef enum{
    RIUINFO_PARA_CMD = 0x0,
    RIUINFO_PARA_BANK,
    RIUINFO_PARA_OFFSET,
    RIUINFO_PARA_VALUE,
    RIUINFO_PARA_MASK
} E_RIUINFO_PARA_LIST;

typedef enum{
    RIUINFO_DUMP = 0x0,
    RIUINFO_BYTE_OFFSET_R,
    RIUINFO_WORD_OFFSET_R,
    RIUINFO_BYTE_OFFSET_W,
    RIUINFO_BYTE_OFFSET_WBIT,
    RIUINFO_WORD_OFFSET_W,
    RIUINFO_WORD_OFFSET_WBIT,
    RIUINFO_HELP,
    RIUINFO_BYTE_OFFSET_POLL,
    RIUINFO_WORD_OFFSET_POLL,
    RIUINFO_END_OF_CMD
} E_RIUINFO_CMD;

typedef struct
{
    E_RIUINFO_CMD eCmd;
    char sRiuInfoCmdList[RIU_INFO_CMD_LEN];
} RIUINFO_CMD;

#endif

//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
void SYSRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 SYSOpen(void** pInstance, void* pAttribute);
MS_U32 SYSClose(void* pInstance);
MS_U32 SYSIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

// Resource_Private
typedef struct _SYS_RESOURCE_PRIVATE
{
    MS_U32  bDvfsInitFlag;
//  MS_U32  Dummy;
}SYS_RESOURCE_PRIVATE;

#ifdef __cplusplus
}
#endif

#endif // _DRVSYS_PRIV_H_
