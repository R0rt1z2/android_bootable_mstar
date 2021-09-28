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

#include <command.h>
#include <common.h>
#include <MsTypes.h>
#include <version.h>
#include <MsEnvironment.h>

#define MB_VER_STR                                  "MB_mboot_ver"
#define DO_BOOTM_STR                             "MB_do_bootm"
#define INFO_EN_CHECKPOINTS_CFG          "en_chk_p"
#define CMD_BUF                                       128
#define MAX_SAVING_FILE_NUMBERS          16
extern char aPrePoints[CMD_BUF];
extern unsigned int start_ub_time;
extern int del_bootargs_cfg(char *,MS_BOOL);
extern int set_bootargs_cfg(char *,char *,MS_BOOL);

void do_points_handle(void)
{
    char* pInfo = getenv(INFO_EN_CHECKPOINTS_CFG);

    //printf("\n...[%s, %d] pInfo=%s, val=%d...\n", __FUNCTION__, __LINE__, pInfo, val);
    if (pInfo != NULL)
    {
        int val = 0;

        val = atoi(pInfo);

        //printf("\n...[%s, %d] pInfo=%s, val=%d...\n", __FUNCTION__, __LINE__, pInfo, val);
        if (val > 0 && val < MAX_SAVING_FILE_NUMBERS)
        {
            char _buf[10] = "";

            sprintf(_buf, "%s=%d", INFO_EN_CHECKPOINTS_CFG, val++);
            set_bootargs_cfg(INFO_EN_CHECKPOINTS_CFG, _buf, 0);

            sprintf(_buf, "%d", val);
            setenv(INFO_EN_CHECKPOINTS_CFG, _buf);
            saveenv();

            //printf("\n...[%s, %d] set %s to val=%d ok...\n", __FUNCTION__, __LINE__, INFO_EN_CHECKPOINTS_CFG, val);
        }
        else
        {
            int ret =0;

            setenv(INFO_EN_CHECKPOINTS_CFG, "");
            saveenv();

            ret = del_bootargs_cfg(INFO_EN_CHECKPOINTS_CFG,0);
            if(ret == 0)
            {
                if(val == MAX_SAVING_FILE_NUMBERS)
                {
                    printf("\n\n <<<<< customer performance index is done >>>>>\n");
                    printf(" <<<<< customer performance index is done >>>>>\n");
                    printf(" <<<<< customer performance index is done >>>>>\n\n");
                }
                //printf("\n...[%s, %d] del %s ok (after catch %d times)...\n", __FUNCTION__, __LINE__, INFO_EN_CHECKPOINTS_CFG, MAX_SAVING_FILE_NUMBERS-1);
            }
            else
            {
                printf("\n...[%s, %d] %s del failed...\n", __FUNCTION__, __LINE__, INFO_EN_CHECKPOINTS_CFG);
            }
        }
    }
}

void Points_Write(MS_U32 val)
{
    extern int snprintf(char *str, size_t size, const char *fmt, ...);
    char strEnv[CMD_BUF];

    // "start_ub" to bootargs
    if(0 != del_bootargs_cfg(aPrePoints,0))
    {
        printf("\n...[%s, %d] del failed for  %s ...\n", __FUNCTION__, __LINE__, aPrePoints);
    }

    memset(strEnv , 0, CMD_BUF);
    snprintf(strEnv , CMD_BUF-1, "%s=%lu", aPrePoints, start_ub_time);
    if(0 != set_bootargs_cfg((char*)aPrePoints, strEnv, 0))
    {
        printf("\n%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
    }

    // "do_bootm" to bootargs
    if(0 != del_bootargs_cfg(DO_BOOTM_STR,0))
    {
        printf("\n...[%s, %d] del failed for  %s ...\n", __FUNCTION__, __LINE__, DO_BOOTM_STR);
    }

    memset(strEnv , 0, CMD_BUF);
    snprintf(strEnv , CMD_BUF-1, "%s=%lu", DO_BOOTM_STR, val);

    if(0 != set_bootargs_cfg((char*)DO_BOOTM_STR, strEnv, 0))
    {
        printf("\n%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
    }

    // "mboot changelist version" to bootargs
    if(0 != del_bootargs_cfg(MB_VER_STR,0))
    {
        printf("\n...[%s, %d] del failed for  %s ...\n", __FUNCTION__, __LINE__, MB_VER_STR);
    }

    memset(strEnv , 0, CMD_BUF);
    snprintf(strEnv , CMD_BUF-1, "%s=%s", MB_VER_STR,  (char *)CHANGELIST_STRING);

    if(0 != set_bootargs_cfg((char*)MB_VER_STR, strEnv, 0))
    {
        printf("\n%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
    }

    // add the count value
    do_points_handle();

    return;
}







