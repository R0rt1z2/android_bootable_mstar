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
#include <exports.h>
#include <MsTypes.h>
#include <common.h>
#include <MsCommon.h>
//#include <drvBDMA.h>
#include <MsDebug.h>
#include <mdrv_voice.h>
#include <MsApiCM4.h>
#include <MsOS.h>
extern int snprintf(char *str, size_t size, const char *fmt, ...);

#define CM4_BIN_SIZE            0x80000
#define RETRY_TIMES        5
int do_run_cm4( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN \n");

    char* pBufferAddr= NULL;
    char cmd[64] ="\0";
    stMic_config stMicConfig_para;
    BOOLEAN enVoice = FALSE;
    unsigned int nRetry = 0;
    if(Read_MicParaFromFlash(&stMicConfig_para) < 0)
    {
        UBOOT_ERROR("can't get ini setting from flash, please run \"dbtable_init 1\"\n");
        return -1;
    }
    char* tmp = getenv("ENABLE_MIC");
    if(tmp == NULL)
    {
        UBOOT_DEBUG("getenv error, use ini setting!\n");
        enVoice = stMicConfig_para.voice_onoff;
    }else{
        if(strcmp(tmp,"1") == 0)
        {
            enVoice = TRUE;
        }
    }
    if(enVoice == FALSE)
    {
        UBOOT_DEBUG("Voice is disable. don't load CM4\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }

    MIC_CONFIG_S tmp_Mic;
    tmp_Mic.mic_num = stMicConfig_para.mic_number;
    tmp_Mic.mic_bitwidth = stMicConfig_para.mic_bitwidth;
    tmp_Mic.mic_audRate = E_MBX_AUD_RATE_16K;

    pBufferAddr=malloc(CM4_BIN_SIZE);
    if(pBufferAddr == NULL)
    {
        UBOOT_ERROR("malloc msphyAddr memory error !\n");
        return -1;
    }
    memset(pBufferAddr,0,CM4_BIN_SIZE);

#if defined (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    snprintf(cmd, sizeof(cmd)-1, "nand read.e 0x%08lX cm4 0x%x", (MS_U32)(pBufferAddr),CM4_BIN_SIZE);
#elif defined (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX cm4 0x%x", (MS_U32)(pBufferAddr),CM4_BIN_SIZE);
#endif

    UBOOT_DEBUG("cmd=%s \n",cmd);
    if(0 != run_command(cmd, 0))
    {
        UBOOT_ERROR("%s: '%s' fails, at %d\n", __func__, cmd, __LINE__);
        free(pBufferAddr);
        return -1;
    }
    UBOOT_DUMP(pBufferAddr, 0x100);
    MDrv_voice_Init();
    MDrv_voice_Halt();
    if(MDrv_voice_verifyHeader(pBufferAddr))
    {
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_updateFw(pBufferAddr,0)==0)
            {
                break;
            }else{
                UBOOT_ERROR("retry %d, MDrv_voice_updateFw failed\n",nRetry);
                MDrv_voice_DcOn();
            }
            mdelay(1);
        }
        if(nRetry>RETRY_TIMES)
        {
            UBOOT_DEBUG("in %s, update CM4 is failed!!\n",__FUNCTION__);
            goto err;
        }
        mdelay(100);
        VQ_CONFIG_S sConfig;
        if((stMicConfig_para.keyword_onoff == 1) ||
            (stMicConfig_para.keyword_onoff == 2))
        {
            sConfig.nMode = stMicConfig_para.keyword_onoff+1;
        }else{
            sConfig.nMode = 0;
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_ConfigVq(sConfig)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_ConfigVq\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_EnableVq(1)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_EnableVq\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_EnableHpf(stMicConfig_para.hpf_onoff)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_EnableHpf\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_ConfigMIC(tmp_Mic)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_MicNum\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_EnableSinegen(stMicConfig_para.sigen_onoff)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_EnableSinegen\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_EnableI2S(stMicConfig_para.i2s_onoff)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_EnableI2S\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_EnableDa(stMicConfig_para.da_onoff)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_EnableDa\n",nRetry);
            mdelay(1);
        }
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_Gain(stMicConfig_para.mic_gain)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_Gain\n",nRetry);
            mdelay(1);
        }
        MDrv_voice_EnableUart(stMicConfig_para.uart_onoff);

        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_EnableVp(1)==0)
            {
                break;
            }
            UBOOT_ERROR("retry %d, MDrv_voice_EnableVp\n",nRetry);
            mdelay(1);
        }

        mdelay(10);
        MDrv_voice_DcOff();
        mdelay(10);
    }else{
        nRetry = 0;
        while(nRetry++<RETRY_TIMES)
        {
            if(MDrv_voice_updateFw(pBufferAddr,CM4_BIN_SIZE)==0)
            {
                break;
            }
            mdelay(1);
        }
        if(nRetry >RETRY_TIMES)
        {
            UBOOT_ERROR(" CM4 BDMA LOAD CODE FAIL CODE !!!\n");
        }
        else
        {
            UBOOT_DEBUG(" CM4 BDMA LOAD CODE PASS !!!\n");
            MDrv_voice_Run();
            mdelay(100);
            MDrv_voice_Gain(stMicConfig_para.mic_gain);
            mdelay(10);
            //MDrv_voice_VQ_ONOFF();
            MDrv_voice_EnableVp(1);
            VQ_CONFIG_S sConfig;
            sConfig.nMode = stMicConfig_para.keyword_onoff;
            MDrv_voice_ConfigVq(sConfig);
            MDrv_voice_EnableVq(1);
            mdelay(10);
            MDrv_voice_EnableHpf(stMicConfig_para.hpf_onoff);
            MDrv_voice_ConfigMIC(tmp_Mic);
            MDrv_voice_EnableSinegen(stMicConfig_para.sigen_onoff);
            MDrv_voice_EnableI2S(stMicConfig_para.i2s_onoff);
            MDrv_voice_EnableDa(stMicConfig_para.da_onoff);
            MDrv_voice_EnableUart(stMicConfig_para.uart_onoff);
            mdelay(10);
            MDrv_voice_DcOff();
            mdelay(10);
        }
    }
err:
    free(pBufferAddr);
    UBOOT_TRACE("OK\n");
    return 0;
}



