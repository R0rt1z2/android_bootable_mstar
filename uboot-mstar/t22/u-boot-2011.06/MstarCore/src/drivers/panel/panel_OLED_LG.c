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

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsTypes.h>

#include "panel_OLED_LG.h"

#include <drvGPIO.h>
#include "panel.h"
#if defined(CONFIG_URSA20_VB1)
#include <ursa/ursa_20.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
static MS_S32 g_s32OLEDErrorTimerID = -1;
static MS_BOOL g_bPanelStatus = FALSE;
static MS_U16 g_u16OLEDErrorHighCount = 0;
static MS_U16 g_u16OLEDTimerPeriod = 50;
static MS_BOOL g_bOLEDCheckOnRF = FALSE;
static MS_U16 g_u16OLEDOnRFLowCount = 0;
static MS_U16 g_u16ACOffCount = 0;
static MS_U16 g_u16EVDDOffCount = 0;
static MS_U16 g_u16VDDOffCount = 0;

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_OLED_LG_SetOLED_PanelStatus(MS_BOOL bPanelStatus)
{
    g_bPanelStatus = bPanelStatus;
}

MS_BOOL MDrv_Panel_OLED_LG_GetOLED_PanelStatus(void)
{
    return g_bPanelStatus;
}

MS_BOOL MDrv_Panel_OLED_LG_ErrorCountCheck(void)
{
    char* pchOledError = NULL;
    pchOledError =  getenv("oledErrorCount");
    if(pchOledError != NULL)
    {
        if((int)strtol(pchOledError, NULL, 10) >= 3)
        {
            MS_U8 u8ErrorPrintCounter = 0;

            while(1)
            {
                printf("[OLED][Error] System cannot be opened since 3 consecutive errors were observed FILE : %s   LINE : %u \n",__FILE__,__LINE__);
                MsOS_DelayTask(1000);

                u8ErrorPrintCounter++;

                if(u8ErrorPrintCounter > 5)
                {
                    printf("Since error was observed 3 times, system will go to standby");
                    run_command("Goto_STB",0);
                    return false;
                }
            }
        }
    }
    else
    {
        printf("oledErrorCount is set to 0  FILE : %s  LINE : %d \n",__FILE__,__LINE__);
        setenv("oledErrorCount", "0");
        saveenv();
    }

    printf("oledErrorDetected is set to 0  FILE : %s  LINE : %d \n",__FILE__,__LINE__);
    setenv("oledErrorDetected", "0");
    saveenv();

    if(g_s32OLEDErrorTimerID == -1)
    {
        g_s32OLEDErrorTimerID = MsOS_CreateTimer( (TimerCb)MDrv_Panel_OLED_LG_TimerOLEDError, 0, g_u16OLEDTimerPeriod, TRUE, "OLED ERROR");

        if(g_s32OLEDErrorTimerID >= 0)
            printf("Create Timer Success,TimerId = %ld  FILE : %s  LINE : %d \n",g_s32OLEDErrorTimerID, __FILE__, __LINE__);
        else
            printf("[OLED][ERROR] Create Timer Failed... FILE : %s  LINE : %d \n", __FILE__, __LINE__);
    }
    return true;
}

void MDrv_Panel_OLED_LG_TurnOledPanelOnOff(MS_BOOL bOn)
{
    UBOOT_TRACE("IN\n");
    if(bOn)
    {
        int i=0;
        int endTimeMS=0;
        int startTime=0;
        if(i==0)
        {
            UBOOT_DEBUG("\n[OLED] OLED_VDD_ON\n");
            MDrv_Panel_OLED_LG_VDD_ON();

            startTime= MsOS_GetSystemTime();
            MsOS_DelayTask(GetOnTiming2());
            endTimeMS= MsOS_GetSystemTime()-startTime;
            UBOOT_DEBUG("\n[OLED] VDD_ON Delay: %d\n",endTimeMS);
            i++;
        }
        if(i==1)
        {
            UBOOT_DEBUG("\n[OLED] OLED_AC_DET_ON\n");
            MDrv_Panel_OLED_LG_AC_DET_ON();

            startTime= MsOS_GetSystemTime();
            MsOS_DelayTask(GetOnTiming3());
            endTimeMS= MsOS_GetSystemTime()-startTime;
            UBOOT_DEBUG("\n[OLED] VDD_ON Delay: %d\n",endTimeMS);
            i++;
        }
        if(i==2)
        {
            UBOOT_DEBUG("\n[OLED] OLED_EVDD_ON\n");
            MDrv_Panel_OLED_LG_EVDD_ON();
            i++;
        }
    }
    else
    {
        // Turn off Panel
        MDrv_Panel_OLED_LG_AC_DET_OFF();
        MsOS_DelayTask(GetOffTiming1());

        MDrv_Panel_OLED_LG_EVDD_OFF();
        MsOS_DelayTask(GetOffTiming2());

        MDrv_Panel_OLED_LG_VDD_OFF();
        // Turn off panel, End
    }
    UBOOT_TRACE("OK\n");
}

void MDrv_Panel_OLED_LG_IncreaseOledErrorCount(void)
{
    char *pcOledErrorDetected = getenv("oledErrorDetected");
    if(pcOledErrorDetected != NULL)
    {
        if(strncmp(pcOledErrorDetected,"1",1) == 0)
        {
            printf("Error has already been detected.");
            return;
        }
    }

    char *pcOledErrorCount = NULL;
    int nOledErrorCount = 0;

    pcOledErrorCount =	getenv("oledErrorCount");
    if(pcOledErrorCount == NULL)
    {
        setenv("oledErrorCount", "1");
        saveenv();
        printf("%s : oledErrorCount = 1 \r\n", __func__);
    }
    else
    {
        nOledErrorCount = (int)strtol(pcOledErrorCount, NULL, 10);
        nOledErrorCount++;
        printf("%s : oledErrorCount = %d \r\n", __func__, nOledErrorCount);

        char buffer[50] = {0};
        snprintf(buffer, 50, "setenv oledErrorCount %d", nOledErrorCount);
        run_command(buffer, 0);
        saveenv();
    }

    setenv("oledErrorDetected", "1");
    saveenv();
}

void MDrv_Panel_OLED_LG_TimerOLEDError(MS_U32 u32Tick,MS_U32 u32TimerID)
{
    u32Tick    = u32Tick;
    u32TimerID = u32TimerID;

    const MS_U16 u16OLEDMaxErrorDuration = 500;
    int intOLEDErrorDetLevel = 0;

    if(g_u16OLEDOnRFLowCount > 0)
    {

        if(g_u16OLEDOnRFLowCount == 0)
        {
            printf("[OLED][ERROR] Turning AC Det off.\n");
            MDrv_Panel_OLED_LG_AC_DET_OFF();
        }

        if(g_u16OLEDOnRFLowCount == g_u16EVDDOffCount)
        {
            printf("[OLED][ERROR] Turning EVDD off.\n");
            MDrv_Panel_OLED_LG_EVDD_OFF();
        }

        if(g_u16OLEDOnRFLowCount == g_u16VDDOffCount)
        {
            printf("[OLED][ERROR] Turning VDD off.\n");
            MDrv_Panel_OLED_LG_VDD_OFF();
        }

        if(g_u16OLEDOnRFLowCount > g_u16VDDOffCount)
        {
            MDrv_Panel_OLED_LG_IncreaseOledErrorCount();

            printf("[ERROR] OLED TCON ERROR : oledErrorDetected is set FILE : %s  LINE : %d \n",__FILE__,__LINE__);

            if(g_s32OLEDErrorTimerID != -1)
            {
                MsOS_DeleteTimer(g_s32OLEDErrorTimerID);
                g_s32OLEDErrorTimerID = -1;
            }
        }

        g_u16OLEDOnRFLowCount++;
    }
    else if(g_bOLEDCheckOnRF == true)
    {
        if((MDrv_Panel_OLED_LG_Get_EVDD_DET_Status() == true) && (MDrv_Panel_OLED_LG_Get_ON_RF_DONE_Level() == false))
        {
            printf("[OLED][ERROR] On RF signal low was detected.\n");

            // ReadBytesfromPanelEEPROM(ADDR_PANEL_PARAM_OFF_TIMING1 ,(MS_U16)sizeof(MS_U16), (MS_U8*)(&g_u16EVDDOffCount), USE_AS_BIG_ENDIEN);
            g_u16EVDDOffCount = GetOffTiming1();
            g_u16EVDDOffCount = ((g_u16EVDDOffCount/g_u16OLEDTimerPeriod) == 0 ? 1 : (g_u16EVDDOffCount/g_u16OLEDTimerPeriod));

            // ReadBytesfromPanelEEPROM(ADDR_PANEL_PARAM_OFF_TIMING2 ,(MS_U16)sizeof(MS_U16), (MS_U8*)(&g_u16VDDOffCount), USE_AS_BIG_ENDIEN);
            g_u16VDDOffCount = GetOffTiming2();
            g_u16VDDOffCount = ((g_u16VDDOffCount/g_u16OLEDTimerPeriod) == 0 ? 1 + g_u16EVDDOffCount : (g_u16VDDOffCount/g_u16OLEDTimerPeriod)) + g_u16EVDDOffCount;

            g_u16OLEDOnRFLowCount++;
            return;
        }
    }

    intOLEDErrorDetLevel = MDrv_Panel_OLED_LG_Get_ERROR_DET_Level();

    if(intOLEDErrorDetLevel || (g_u16OLEDErrorHighCount >= g_u16ACOffCount))
    {
        if(g_u16OLEDErrorHighCount == 0)
        {
            g_u16ACOffCount = u16OLEDMaxErrorDuration / g_u16OLEDTimerPeriod;

            // ReadBytesfromPanelEEPROM(ADDR_PANEL_PARAM_OFF_TIMING1 ,(MS_U16)sizeof(MS_U16), (MS_U8*)(&g_u16EVDDOffCount), USE_AS_BIG_ENDIEN);
            printf("\nGetOffTiming 1: %d 2: %d\n",GetOffTiming1(),GetOffTiming2());
            g_u16EVDDOffCount = GetOffTiming1();
            g_u16EVDDOffCount = ((g_u16EVDDOffCount/g_u16OLEDTimerPeriod) == 0 ? 1 : (g_u16EVDDOffCount/g_u16OLEDTimerPeriod)) + g_u16ACOffCount;

            // ReadBytesfromPanelEEPROM(ADDR_PANEL_PARAM_OFF_TIMING2 ,(MS_U16)sizeof(MS_U16), (MS_U8*)(&g_u16VDDOffCount), USE_AS_BIG_ENDIEN);
            g_u16VDDOffCount = GetOffTiming2();
            g_u16VDDOffCount = ((g_u16VDDOffCount/g_u16OLEDTimerPeriod) == 0 ? 1 + g_u16ACOffCount + g_u16EVDDOffCount : (g_u16VDDOffCount/g_u16OLEDTimerPeriod)) + g_u16ACOffCount + g_u16EVDDOffCount;

            printf("u16ACOffCount = %d \n", g_u16ACOffCount);
            printf("u16EVDDOffCount = %d \n", g_u16EVDDOffCount);
            printf("u16VDDOffCount = %d \n", g_u16VDDOffCount);
        }

        g_u16OLEDErrorHighCount++;
        printf("g_u16OLEDErrorHighCount = %d \n", g_u16OLEDErrorHighCount);

        if(g_u16OLEDErrorHighCount == g_u16ACOffCount)
        {
            printf("[OLED][ERROR] Turning AC Det off.\n");
            MDrv_Panel_OLED_LG_AC_DET_OFF();
        }
        if(g_u16OLEDErrorHighCount == g_u16EVDDOffCount)
        {
            printf("[OLED][ERROR] Turning EVDD off.\n");
            MDrv_Panel_OLED_LG_EVDD_OFF();
        }
        if(g_u16OLEDErrorHighCount == g_u16VDDOffCount)
        {
            printf("[OLED][ERROR] Turning VDD off.\n");
            MDrv_Panel_OLED_LG_VDD_OFF();
        }
        if(g_u16OLEDErrorHighCount > g_u16VDDOffCount)
        {
            MDrv_Panel_OLED_LG_IncreaseOledErrorCount();

            printf("[ERROR] OLED TCON ERROR : oledErrorDetected is set FILE : %s  LINE : %d \n",__FILE__,__LINE__);

            if(g_s32OLEDErrorTimerID != -1)
            {
                MsOS_DeleteTimer(g_s32OLEDErrorTimerID);
                g_s32OLEDErrorTimerID = -1;
            }
        }
    }
    else
    {
        g_u16OLEDErrorHighCount = 0;
    }
}

MS_BOOL MDrv_Panel_OLED_LG_EVDD_DET_Check(void)
{
    char *pUrsaVersionEnv = NULL;
    if(true != MDrv_Panel_OLED_LG_Get_EVDD_DET_Status())
    {// Check EVDD Detect
        printf("EVDD Detect is still low. System will go to standby FILE : %s  LINE : %u \n",__FILE__, __LINE__);
        MDrv_Panel_OLED_LG_TurnOledPanelOnOff(false);
        run_command("Goto_STB",0);
        return false;
    }
    else
    {
        printf("EVDD Detected successfully \n");
        return true;
    }
}

MS_BOOL MDrv_Panel_OLED_LG_OnRFCountCheck(void)
{
    char *pUrsaVersionEnv = NULL;
    MS_U8 u8OnRFCounter = 0;
    while(1)
    {
        if(MDrv_Panel_OLED_LG_Get_ON_RF_DONE_Level())
        {
            // g_bOLEDCheckOnRF = true;
            printf("On RF is ready!\n");
            break;
        }
        else
        {
            printf("On RF is not ready\n");
            MsOS_DelayTask(1000);
        }

        u8OnRFCounter++;

        if(u8OnRFCounter > 7)
        {
            printf("ON RF couldn't be detected.\n");
            MDrv_Panel_OLED_LG_TurnOledPanelOnOff(false);
            MDrv_Panel_OLED_LG_IncreaseOledErrorCount();

            pUrsaVersionEnv = getenv("ursa_version");
            if (pUrsaVersionEnv == NULL)
            {
                UBOOT_ERROR("URSA_VERSION_ENV not found, need to update ursa.\n");
            }

            return false;
        }
    }
    return true;
}

void MDrv_Panel_OLED_LG_ErrorTimerID_Reset(void)
{
    if(g_s32OLEDErrorTimerID != -1)
    {
        MsOS_DeleteTimer(g_s32OLEDErrorTimerID);
        g_s32OLEDErrorTimerID = -1;
    }
}

void MDrv_Panel_OLED_LG_VDD_ON(void)
{
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_URSA20_VB1) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
       Panel_OLED_LG_VDD_ON();
#endif
    UBOOT_TRACE("OK\n");

}
void MDrv_Panel_OLED_LG_VDD_OFF(void)
{
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    Panel_OLED_LG_VDD_OFF();
#endif
    UBOOT_TRACE("OK\n");

}

void MDrv_Panel_OLED_LG_AC_DET_ON(void)
{
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    Panel_OLED_LG_AC_DET_ON();
#endif
    UBOOT_TRACE("OK\n");

}

void MDrv_Panel_OLED_LG_AC_DET_OFF(void)
{
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    Panel_OLED_LG_AC_DET_OFF();
#endif
    UBOOT_TRACE("OK\n");

}

void MDrv_Panel_OLED_LG_EVDD_ON(void)
{
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    Panel_OLED_LG_EVDD_ON();
#endif
    UBOOT_TRACE("OK\n");

}

void MDrv_Panel_OLED_LG_EVDD_OFF(void)
{
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    Panel_OLED_LG_EVDD_OFF();
#endif
    UBOOT_TRACE("OK\n");

}

MS_BOOL MDrv_Panel_OLED_LG_Get_EVDD_DET_Status(void)
{
    UBOOT_TRACE("IN\n");
    MS_BOOL bRet = true;
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    bRet = Panel_OLED_LG_EVDD_DET_STATUS();
#endif
    UBOOT_TRACE("OK\n");
    return bRet;
}

MS_BOOL MDrv_Panel_OLED_LG_Get_ON_RF_DONE_Level(void)
{
    UBOOT_TRACE("IN\n");
    MS_BOOL bRet = false;
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    bRet = Panel_OLED_LG_ONRF_DONE_LEVEL();
#endif
    UBOOT_TRACE("OK\n");
    return bRet;
}

MS_BOOL MDrv_Panel_OLED_LG_Get_ERROR_DET_Level(void)
{
    UBOOT_TRACE("IN\n");
    MS_BOOL bRet = true;
#if defined(CONFIG_URSA20_VB1)&& defined(CONFIG_PNL_OLED_LG_SUPPORT)
    bRet = Panel_OLED_LG_Get_ERROR_DET_Level();
#endif
    UBOOT_TRACE("OK\n");
    return bRet;
}
