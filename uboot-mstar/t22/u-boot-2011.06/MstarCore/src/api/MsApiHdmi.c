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
#include <MsDebug.h>
#if(CONFIG_HDMITX_MSTAR_ROCKET ==1)
#include <apiHDMITx.h>
#elif(CONFIG_HDMITX_MSTAR_ROCKET2 ==1)
#include <hdmitx/mstar/rocket2/MsDrvRocket.h>
#endif

#if (ENABLE_HDMITX_MSTAR==1)
extern int msHdmitx_Disp_Init(void);
#elif (CONFIG_HDMITX_MSTAR_ROCKET==1)
extern MS_BOOL msHdmitx_Rocket_Init(HDMITX_VIDEO_TIMING outPutType );
#elif (CONFIG_TV_HDMITX_MSTAR==1)
extern int MsDrvTvHdmiTxInit(void);
#endif

int do_hdmi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        cmd_usage(cmdtp);
    }
    else
    {
       if(strncmp(argv[1], "init", 5) == 0)
       {
        #if defined(CONFIG_STB_ECOS_BOOT)
            #define ENV_CVBS_STATUS "cvbs_status"

            char *pInfo = NULL;
            pInfo = getenv(ENV_CVBS_STATUS);
            if(pInfo == NULL || strcmp(pInfo, "0") == 0)
            {
                setenv(ENV_CVBS_STATUS, "1");
                saveenv();
            }
        #endif

        #if(CONFIG_HDMITX_MSTAR_ROCKET ==1 ||CONFIG_HDMITX_MSTAR_ROCKET2 ==1)

            char * p_str = NULL;
        #if (CONFIG_HDMITX_MSTAR_ROCKET==1)
            HDMITX_VIDEO_TIMING u8ResolutionEnv = HDMITX_RES_1920x1080p_60Hz;
        #else
            EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING u8ResolutionEnv = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
        #endif

            p_str = getenv ("resolution");
            if(NULL != p_str)
            {
                u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
            }
            UBOOT_DEBUG("ROCKET resolution Type=%d \n",u8ResolutionEnv);
            ret =msHdmitx_Rocket_Init(u8ResolutionEnv);
            if(ret == TRUE)
                ret = 0;
            else
                ret = -1;
        #elif (CONFIG_TV_HDMITX_MSTAR==1)
            ret = MsDrvTvHdmiTxInit();
        #else

            ret = msHdmitx_Disp_Init();
        #endif
       }
       else
       {
           cmd_usage(cmdtp);
       }
    }

    UBOOT_TRACE("OK\n");
    return ret;
}

