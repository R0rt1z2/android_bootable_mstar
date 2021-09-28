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

#define _MSPOWER_C_

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <command.h>
#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <ShareType.h>
#include <MsSystem.h>
#include <drvPM.h>
#include <drvMBX.h>
#include <drvBDMA.h>
#include <malloc.h>
#include <MsUboot.h>
#ifndef  CUS_IR_HEAD_FILE//Please define it in board file for customization
#include "IR_MSTAR_DTV.h"
#else
#include CUS_IR_HEAD_FILE
#endif
#include <msAPI_Power.h>
#include <idme.h>

#if (CONFIG_DYNAMIC_IR)
#include <drvIR.h>
#include <MsBoot.h>
#define IR_PARA_VERSION 0x20
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PM_MBX_QUEUESIZE 6
#define PM_CMDIDX_GLOBAL_CHIP_RESET 0x21

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

extern BOOLEAN msAPI_IR_Initialize(void);
extern BOOLEAN msAPI_KeyPad_Initialize(void);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if (CONFIG_DYNAMIC_IR)
#define IR_INFO_START 5
#define WAKE_POWERKEY_LAST 25
static void msAPI_PM_Set_IRParaVsersion(U8 u8IRParaVersion)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_MSGQ_Status MbxMsgQStatus;
    MBX_Msg MB_Command;
    MS_U32 MBX_Resendcnt=0;
    // send version  to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_LIBVER;
    MB_Command.u8ParameterCount = 1;
    MB_Command.u8Parameters[0] = (MS_S8)u8IRParaVersion;
    if(((MbxResult = MDrv_MBX_SendMsg(&MB_Command)) != E_MBX_SUCCESS))
    {
        printf("[E_IR_CPUTo51_CMD_PARAVER] >>>Send Mail fail: 0x%x.\n", MbxResult);
    }
    return;
}
#if defined(CONFIG_DATA_SEPARATION)
static void msAPI_PM_SetPowerKey_Keypad(U8 *buffer)
{
    Read_KeypadWakeup_FromFlash((st_KeypadWakeup_t*)buffer);
}
#endif
static void msAPI_PM_SetPowerKey(U8 *buffer)
{
    U8 j = IR_INFO_START;
    U32 i = 0;
    st_IRWakeup_t Wakeupinfo;
    memset(&Wakeupinfo,0,sizeof(st_IRWakeup_t));
    Read_IRWakeup_FromFlash(&Wakeupinfo);
    for(i = 0;i <IR_MAX; i++)
    {

        if((Wakeupinfo[i].u32Headcode > 0xFFFF) || j>= (PM_MAX_BUF_WAKE_IR-7))
        {
            buffer[WAKE_POWERKEY_LAST] = (Wakeupinfo[i].u8Protocol)&0xFF;
            buffer[WAKE_POWERKEY_LAST+1] = ((Wakeupinfo[i].u32Headcode)>>24)&0xFF;
            buffer[WAKE_POWERKEY_LAST+2] = ((Wakeupinfo[i].u32Headcode)>>16)&0xFF;
            buffer[WAKE_POWERKEY_LAST+3] = ((Wakeupinfo[i].u32Headcode)>>8)&0xFF;
            buffer[WAKE_POWERKEY_LAST+4] = (Wakeupinfo[i].u32Headcode)&0xFF;
            buffer[WAKE_POWERKEY_LAST+5] = ((Wakeupinfo[i].u16Key)>>8)&0xFF;
            buffer[WAKE_POWERKEY_LAST+6] = (Wakeupinfo[i].u16Key)&0xFF;
        }
        else
        {
            buffer[j] = (Wakeupinfo[i].u8Protocol)&0xFF;
            buffer[j+1] = ((Wakeupinfo[i].u32Headcode)>>8)&0xFF;
            buffer[j+2] = (Wakeupinfo[i].u32Headcode)&0xFF;
            buffer[j+3] = ((Wakeupinfo[i].u16Key)>>8)&0xFF;
            buffer[j+4] = (Wakeupinfo[i].u16Key)&0xFF;
            j += 5;
        }
    }
}
#endif
static int str_cfg = 0;
MS_BOOL msAPI_PM_MBX_Init(void)
{
    MsApi_MBX_Init();
    if( E_MBX_SUCCESS != MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_NOWAIT, PM_MBX_QUEUESIZE))
    {
        printf("E_MBX_CLASS_PM_NOWAIT - MBX register msg error\n");
    }
   return TRUE;
    }


MS_BOOL msAPI_PM_Reset(void)
{
    MBX_Msg MB_Command;
    printf("\033[0;33m Do mailbox init\033[0m\n");

    msAPI_PM_MBX_Init();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
    MB_Command.u8Index = PM_CMDIDX_GLOBAL_CHIP_RESET;
    MB_Command.u8ParameterCount = 0;
    printf("\033[0;33m Send reset message tp PM51.\033[0m\n\n");
    MDrv_MBX_SendMsg(&MB_Command);
        return TRUE;

}

EN_POWER_ON_MODE msAPI_Power_QueryPowerOnMode(void)
{
    U8 u8PowerDownMode =  (U8)MDrv_PM_PowerOnMode();

    switch ( u8PowerDownMode )
    {
        // DC ON
        case PM_MODE_STANBY:    // 0x01
        case PM_MODE_SLEEP:     // 0x02
        case PM_MODE_DEEPSLEEP: // 0x03
        {
            printf("DC on\n");
            return EN_POWER_DC_BOOT;
        }
        // AC ON
        case PM_MODE_DEFAULT:   // 0xFF
        {
            printf("AC on\n");
            return EN_POWER_AC_BOOT;
        }
        // EXCEPTION
        default:
        {
            if(u8PowerDownMode == 0xF1)
                printf("DC on !! \n");
            else
                printf("read PM_SLEEP_AC_DC_ON error\n");

            return EN_POWER_DC_BOOT;
        }
    }
}
void msAPI_SetSPIOffsetForMCU(void)
{
    MDrv_PM_SetSPIOffsetForMCU(0x01);
}

u8 u8SophiaIR2Cfg[PM_MAX_BUF_WAKE_IR2] = {
            0x0C, 0x05, 0x25, 0x29,
            0x2B, 0x3F, 0x39, 0x15,
            0x74, 0x27, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF };

u8 u8GretaIR2Cfg[PM_MAX_BUF_WAKE_IR2] = {
            0x0C, 0x05, 0x25, 0x15,
            0x29, 0x2B, 0x3F, 0x39,
            0x74, 0x8E, 0xFC, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF };

extern const char *amzn_target_device_name(void);

void msAPI_Power_PowerDown_EXEC(void)
{
    PM_Result pmResult;
    static PM_PowerDownCfg PmPowerDownCfg;
    u8 u8IR2Cfg[PM_MAX_BUF_WAKE_IR2]={0};
#if 0//eric.peng
    MDrv_BDMA_SetSPIOffsetForMCU();
#endif
    if(msAPI_PM_MBX_Init()==FALSE)
    {
        return;
    }

    if (!strcmp(amzn_target_device_name(), "sophia"))
    {
		MDrv_WriteByte(0x1040, 0x01);//Sophia, abc123 IR Header
            memcpy((void *)&(u8IR2Cfg[0]), (void *)&(u8SophiaIR2Cfg[0]), (sizeof(u8SophiaIR2Cfg)));
    }
    else if (!strcmp(amzn_target_device_name(), "greta") || !strcmp(amzn_target_device_name(), "abc123") || !strcmp(amzn_target_device_name(), "abc123"))
    {
		MDrv_WriteByte(0x1040, 0x00);//Greta, abc123 IR Header
            memcpy((void *)&(u8IR2Cfg[0]), (void *)&(u8GretaIR2Cfg[0]), (sizeof(u8GretaIR2Cfg)));
    }
    else
    {
        printf("Unknown Product\n");
    }

    //MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_ALL);
    //MDrv_PM_SetDbgLevel(E_PM_DBGLV_INFO);

    //To Do: MDrv_PM_Check_Version is used on checking PM version(zeo 2.0 or not?). However, the utopia driver for A5 doesn't update to the newest.
    // So the current head file doen't include this protoype.
    if(MDrv_PM_Check_Version(TRUE))
    {
        // Initialize IR before power down
        msAPI_IR_Initialize();
    }

#if (CONFIG_DYNAMIC_IR)
    msAPI_PM_Set_IRParaVsersion(IR_PARA_VERSION);
#endif
    #if (CONFIG_KEYPAD)
    // Initialize SAR before power down
    msAPI_KeyPad_Initialize();
    #endif

    static PM_WakeCfg PmWakeCfg =
    {
        .bPmWakeEnableIR = TRUE,
        .bPmWakeEnableSAR = TRUE,
#if defined(PM_WAKEUP_SOURCE_BT_GPIO0)
        .bPmWakeEnableGPIO0 = PM_WAKEUP_SOURCE_BT_GPIO0,
#else
        .bPmWakeEnableGPIO0 = FALSE,
#endif
#if defined(PM_WAKEUP_SOURCE_BT_GPIO1)
        .bPmWakeEnableGPIO1 = PM_WAKEUP_SOURCE_BT_GPIO1,
#else
        .bPmWakeEnableGPIO1 = FALSE,
#endif
        .bPmWakeEnableUART1 = FALSE,
        .bPmWakeEnableSYNC = FALSE,
        .bPmWakeEnableESYNC = FALSE,

        .bPmWakeEnableRTC0 = TRUE,
        .bPmWakeEnableRTC1 = TRUE,
        .bPmWakeEnableDVI0 = FALSE,
        .bPmWakeEnableDVI2 = FALSE,
        .bPmWakeEnableCEC = TRUE,
        .bPmWakeEnableAVLINK = FALSE,
        .bPmWakeEnableWOL = FALSE,
#if(CONFIG_VOICE_CM4==1)
        .bPmWakeEnableCM4 = TRUE,
#endif
        .u8PmStrMode = TRUE,

        .u8PmWakeIR =
        {   //IR wake-up key define
            IRKEY_POWER, 0x03, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF
        },


        .u8PmWakeIR2 =
        {   //IR wake-up key define
            IRKEY_POWER, 0x03, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF
        }
    };

    char* s = getenv(POWER_KEY_NAME);
    if (s)
    {
        PmWakeCfg.u8PmWakeIR[0] = simple_strtoul(s, NULL, 16);
    }
    memcpy((void *)&(PmWakeCfg.u8PmWakeIR2[0]), (void *)&(u8IR2Cfg[0]), (sizeof(u8IR2Cfg)));
    PmWakeCfg.u8PmStrMode = str_cfg;
#if defined(CONFIG_DATA_SEPARATION)
    msAPI_PM_SetPowerKey_Keypad(&PmWakeCfg.u8PmWakeIR[0]);
#endif
#if (CONFIG_DYNAMIC_IR)
    msAPI_PM_SetPowerKey(&PmWakeCfg.u8PmWakeIR[0]);
#endif
    //wol wakeup
    char *ethaddr;
    char *pmac;

    if ( NULL != (ethaddr = getenv("ethaddr"))
                    && strlen(ethaddr) == strlen("XX:XX:XX:XX:XX:XX")
                    && ':' == ethaddr[2]
                    && ':' == ethaddr[5]
                    && ':' == ethaddr[8]
                    && ':' == ethaddr[11]
                    && ':' == ethaddr[14]
        )
    {
        pmac = (char*) malloc(strlen("XX:XX:XX:XX:XX:XX")+1);

        if(pmac != NULL)
        {
            memcpy(pmac, ethaddr, strlen("XX:XX:XX:XX:XX:XX"));
            pmac[2]  = '\0';
            pmac[5]  = '\0';
            pmac[8]  = '\0';
            pmac[11] = '\0';
            pmac[14] = '\0';
            pmac[17] = '\0';

            PmWakeCfg.u8PmWakeMACAddress[0]   = (u8)simple_strtoul(&(pmac[0]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[1]   = (u8)simple_strtoul(&(pmac[3]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[2]   = (u8)simple_strtoul(&(pmac[6]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[3]   = (u8)simple_strtoul(&(pmac[9]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[4]   = (u8)simple_strtoul(&(pmac[12]), NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[5]   = (u8)simple_strtoul(&(pmac[15]), NULL, 16);

            free(pmac);
         }
    }

    //printf("PM_Init...\n");
    flush_cache((MS_U32)&PmWakeCfg, sizeof(PM_WakeCfg));
    pmResult= MDrv_PM_Init((PM_WakeCfg*)VA2PA((MS_U32) &PmWakeCfg));


    PmPowerDownCfg.u8WakeAddress = E_PM_WAKE_ZERO ;//E_PM_LAST_TWOSTAGE_POWERDOWN;
    PmPowerDownCfg.u8PowerDownMode = E_PM_SLEEP;//E_PM_STANDBY;//E_PM_SLEEP;

    flush_cache((MS_U32)&PmPowerDownCfg, sizeof(PmPowerDownCfg));
    //printf("\n\r==== Enter Standby Mode !! ==== %d \n",(int)pmResult);
    MDrv_PM_PowerDown((PM_PowerDownCfg*)VA2PA((MS_U32) &PmPowerDownCfg));

    while(1);
}
void msAPI_Power_SetStrConfig(int para)
{
    str_cfg = para;
}
