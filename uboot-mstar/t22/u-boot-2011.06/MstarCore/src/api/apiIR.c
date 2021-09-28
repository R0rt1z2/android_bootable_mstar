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
#include <common.h>
#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <drvIR.h>
#include <drvSAR.h>

#ifndef  CUS_IR_HEAD_FILE//Please define it in board file for customization
#include "IR_MSTAR_DTV.h"
#else
#include CUS_IR_HEAD_FILE
#endif

#include <ShareType.h>
#include <MsDebug.h>

#if (CONFIG_KEYPAD)
#include <keypad/keypad_type.h>
static SAR_KpdRegCfg_t m_KpdConfig[5][MAXKEYPADCH] = {
#include <keypad.h>
};
#include <bootlogo/MsPoolDB.h>
#include <MsRawIO.h>
#endif

/// MS Return Error
#ifndef MSRET_ERROR
#define MSRET_ERROR     0
#endif
/// MS Return OK
#ifndef MSRET_OK
#define MSRET_OK        1
#endif

#define MAX_ADC_KEY_CHANNEL_NUM 4

#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))
extern ulong key_layout;
BOOLEAN msAPI_IR_Initialize(void)
{
    UBOOT_TRACE("IN\n");
    static IR_RegCfg tIRRegCfg =
    {
        .u8Ctrl0        = IR_LEADER_CODE_CHECKING_OPTION,
        .u8Ctrl1        = 0x01,
        .u8Clk_mhz      = MST_XTAL_CLOCK_MHZ,
        .u8HdrCode0     = IR_HEADER_CODE0,
        .u8HdrCode1     = IR_HEADER_CODE1,
        .u8CCodeBytes   = 2,
        .u8CodeBits     = 32,  //(32bits==4bytes)
    };
    char* s = getenv(IR_HEADER_CODE0_NAME);
    if (s)
    {
        tIRRegCfg.u8HdrCode0 = simple_strtoul(s, NULL, 16);
    }

    s = getenv(IR_HEADER_CODE1_NAME);
    if (s)
    {
        tIRRegCfg.u8HdrCode1 = simple_strtoul(s, NULL, 16);
    }

    static IR_TimeBnd tIRTimeBnd =
    {
        .tHdr={
            .s16Time = IR_HEADER_CODE_TIME,
            .s8UpBnd = IR_HEADER_CODE_TIME_UB,
            .s8LoBnd = IR_HEADER_CODE_TIME_LB},
        .tOff={
            .s16Time = IR_OFF_CODE_TIME,
            .s8UpBnd = IR_OFF_CODE_TIME_UB,
            .s8LoBnd = IR_OFF_CODE_TIME_LB},
        .tOffRpt={
            .s16Time = IR_OFF_CODE_RP_TIME,
            .s8UpBnd = IR_OFF_CODE_RP_TIME_UB,
            .s8LoBnd = IR_OFF_CODE_RP_TIME_LB},
        .tLg01Hg={
            .s16Time = IR_LOGI_01H_TIME,
            .s8UpBnd = IR_LOGI_01H_TIME_UB,
            .s8LoBnd = IR_LOGI_01H_TIME_LB},
        .tLg0={
            .s16Time = IR_LOGI_0_TIME,
            .s8UpBnd = IR_LOGI_0_TIME_UB,
            .s8LoBnd = IR_LOGI_0_TIME_LB},
        .tLg1={
            .s16Time = IR_LOGI_1_TIME,
            .s8UpBnd = IR_LOGI_1_TIME_UB,
            .s8LoBnd = IR_LOGI_1_TIME_LB}
    };

    IR_RegCfg* pirRegCfg=&tIRRegCfg;
    IR_TimeBnd* pirTimeBnd=&tIRTimeBnd;

    //(0) For debugging
    //printf("MailBox: MDrv_IR_SetDbgLevel\n");
    //MDrv_IR_SetDbgLevel(E_IR_DBGLVL_INFO);
    //(1) Initialization
    flush_cache((MS_U32)pirRegCfg, sizeof(IR_RegCfg));
//    printf("pirRegCfg = 0x%lx \n",pirRegCfg);
//    printf("_VA2PA((MS_U32)pirRegCfg) = 0x%lx \n",_VA2PA((MS_U32)pirRegCfg));
    if(MDrv_IR_Init((IR_RegCfg*)_VA2PA((MS_U32)pirRegCfg))==E_IR_FAIL)
    {
        UBOOT_ERROR("MDrv_IR_Init fails...\n");
        return MSRET_ERROR;
    }
//    printf("msAPI_IR_Initialize3333355555555555543\n");
    //(2) Configure Time Bounds
    flush_cache((MS_U32)pirTimeBnd, sizeof(IR_TimeBnd));
 //       printf("msAPI_IR_Initialize3336663444444444443\n");

    if(MDrv_IR_Config(pirTimeBnd)==E_IR_FAIL)
    {
        UBOOT_ERROR("MDrv_IR_Config fails...\n");
        return MSRET_ERROR;
    }
//    printf("$$$$$[eric] IR Init Finished!\n");
    UBOOT_TRACE("OK\n");

    return MSRET_OK;
}

#if (CONFIG_KEYPAD)
BOOLEAN msAPI_KeyPad_Initialize(void)
{
    UBOOT_TRACE("IN\n");
    static SAR_RegCfg sarRegCfgCh;
    SAR_RegCfg* psarRegCfgCh=&sarRegCfgCh;

    MS_U8 k = 0;
    MS_U8 u8Idx = 0;
    MS_U8 i = 0;
    U32 keypad_offset = 0;
    U32 keypad_size = 0;
    char *s = NULL;
    char *env = NULL;
	char *key_layout_str = NULL;

    if(MDrv_SAR_Init()==E_SAR_FAIL)
    {
        UBOOT_ERROR("MDrv_SAR_Init fails\n");
        return MSRET_ERROR;
    }
	key_layout_str = getenv("key_layout");
	if (key_layout_str!=NULL)
		key_layout = simple_strtoul(key_layout_str, NULL, 10);
	else
		key_layout = 0;

    for(i=0; i<MAXKEYPADCH; i++)
    {
        UBOOT_DEBUG("\nKeypadInfoSet[%d] = {%s, 0x%02x, 0x%02x, 0x%02x, 0x%02x}\n", \
               i, (m_KpdConfig[key_layout][i].bEnable != TRUE) ? "Disable" : "Enable", \
               m_KpdConfig[key_layout][i].u8SARChID, \
               m_KpdConfig[key_layout][i].tSARChBnd.u8UpBnd, \
               m_KpdConfig[key_layout][i].tSARChBnd.u8LoBnd, \
               m_KpdConfig[key_layout][i].u8KeyLevelNum);
        for(k=0; k<m_KpdConfig[key_layout][i].u8KeyLevelNum; k++)
        {
            UBOOT_DEBUG("u8KeyThreshold[%d]: 0x%02x\n", k, m_KpdConfig[key_layout][i].u8KeyThreshold[k]);
        }
        for(k=0; k<m_KpdConfig[key_layout][i].u8KeyLevelNum; k++)
        {
            UBOOT_DEBUG("u8KeyCode[%d]: 0x%02x\n", k, m_KpdConfig[key_layout][i].u8KeyCode[k]);
        }
    }
    s = getenv(POWER_KEY_NAME);
    MS_U8 u8PowerKey = (NULL != s) ? simple_strtoul(s, NULL, 16) : IRKEY_DUMY;
    //MS_U8 u8PowerKey = simple_strtoul(s, NULL, 16);
    for(u8Idx = 0, k = 0; k < MAXKEYPADCH; k++)
    {
        if(m_KpdConfig[key_layout][k].bEnable == ENABLE)
        {
            psarRegCfgCh->u8SARChID = k;
            psarRegCfgCh->u8UpBnd = m_KpdConfig[key_layout][k].tSARChBnd.u8UpBnd;
            psarRegCfgCh->u8LoBnd = m_KpdConfig[key_layout][k].tSARChBnd.u8LoBnd;
            psarRegCfgCh->u8KeyLevelNum = m_KpdConfig[key_layout][k].u8KeyLevelNum;
            for(i = 0; i < psarRegCfgCh->u8KeyLevelNum; i++)
            {
                psarRegCfgCh->u8KeyThreshold[i] = m_KpdConfig[key_layout][k].u8KeyThreshold[i];
                psarRegCfgCh->u8KeyCode[i] = m_KpdConfig[key_layout][k].u8KeyCode[i];
            }

            if (IRKEY_DUMY != u8PowerKey)
            {
                for (i = 0; i < psarRegCfgCh->u8KeyLevelNum; ++i)
                {
                    if (IRKEY_POWER == psarRegCfgCh->u8KeyCode[i])
                    {
                        psarRegCfgCh->u8KeyCode[i] = u8PowerKey;
                    }
                }
            }

            // configure keypad channel
            // MsOS_Dcache_Flush((MS_U32)psarRegCfgCh, sizeof(SAR_RegCfg));
            // MsOS_FlushMemory();
            flush_cache((MS_U32)psarRegCfgCh, sizeof(SAR_RegCfg));
            Chip_Flush_Memory();

            if(MDrv_SAR_Config((SAR_RegCfg*)_VA2PA((MS_U32)psarRegCfgCh))==E_SAR_FAIL)
            {
                UBOOT_ERROR("MDrv_SAR_Config: CH_%d fails\n",u8Idx);
                return MSRET_ERROR;
            }

            if((++u8Idx) >= MAX_ADC_KEY_CHANNEL_NUM)
                break;
        }
    }

    if(MDrv_SAR_Enable(TRUE)==E_SAR_FAIL)
    {
        UBOOT_ERROR("MDrv_SAR_Enable fails\n");
        return MSRET_ERROR;
    }
    UBOOT_TRACE("OK\n");
    return MSRET_OK;
}
#endif
