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
#include <malloc.h>
#include <MsDebug.h>
#include <drvMSPI.h>
#include <drvSYS.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <CusConfig.h>
#include <drvGPIO.h>

#if (ENABLE_CMD_SF == 1)
#include <mhal_sys.h>
#endif

#if (ENABLE_CMD_SF == 1)
int MDrv_MSPI_CFG_PIN(int port_num)
{
    return MHal_MSPI_CFG_PIN(port_num);
}

// CONFIG_MSPI_CS_GPIO : defined in "sboot/inc/$(CHIP)/board/$(BOARD_NAME).h"
#if defined(CONFIG_MSPI_CS_GPIO)
void MDrv_MSPI_CFG_CS(MS_BOOL Enable)
{
    MHal_MSPI_CFG_CS(Enable);
}
#endif

#endif

//Dolby 
#if(ENABLE_DOLBY==1)

int do_dolby_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;    
    MSPI_CLKConfig ptCLKConfig;
    MS_U8 * dolby_data=NULL;//0x20400000;
    MS_U8 * dolby_temp=NULL;//0x20400000;
    MS_U32 dolby_size=0;//0x81F2A8;
    
    UBOOT_TRACE("IN\n");
    char PathBuf[CMD_BUF] = "\0";
    mdrv_gpio_init();
    mdrv_gpio_set_high(PAD_GPIO2);
    mdelay(10);
    mdrv_gpio_set_low(PAD_GPIO2);
    mdelay(10);
    mdrv_gpio_set_high(PAD_GPIO2);
    vfs_mount(CONFIG);
    snprintf(PathBuf, sizeof(PathBuf), "%s/fpga/dolby_fpga.sof",CONFIG_PATH);
    UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
    dolby_size = vfs_getsize(PathBuf);        
    UBOOT_DEBUG("u32dolby_size=0x%x\n",(unsigned int)dolby_size);
    dolby_data=(MS_U8 *)malloc(dolby_size);
    if(dolby_data==NULL)
    {
        UBOOT_ERROR("malloc for dolby_data fail==========\n");
        return 0;
    }
    if(dolby_size==0 || (vfs_read(dolby_data,PathBuf,0,dolby_size)!=0))
    {
        UBOOT_ERROR("Error: vfs_read Fail\n");
        free(dolby_data);
        return 0;
    }
    dolby_temp=dolby_data;
    MDrv_SYS_SetPadMux(E_TS0_PAD_SET,E_MSPI_PAD_ON);
    memset(&ptCLKConfig, 0, sizeof(ptCLKConfig));
    ptCLKConfig.BClkPolarity=0;
    ptCLKConfig.BClkPhase=0;
    ptCLKConfig.U8Clock=5;
    
    ret=MDrv_MSPI_Init_Ext(0);
    if(ret!=E_MSPI_OK)
    {
        UBOOT_ERROR("MDrv_MSPI_Init_Ext error %d\n",ret);
        free(dolby_data);
        return 0;
    }
    MDrv_MasterSPI_CsPadConfig(0,0xFF);
    MDrv_MSPI_CLKConfig(&ptCLKConfig);
    MDrv_MasterSPI_MaxClkConfig(0,10);
    MDrv_MSPI_SlaveEnable(true);
    while(dolby_size>0)
        {
            if(dolby_size>0xFFFF)
            {
                UBOOT_DEBUG("dolby addr [0x%x] size[0x%x] \n",(unsigned int)dolby_temp,(unsigned int)dolby_size);
                MDrv_MSPI_SlaveEnable(false);
                MDrv_MSPI_Write(dolby_temp,0xFFFF);
                dolby_temp=dolby_temp+(0xFFFF);
                dolby_size=dolby_size-(0xFFFF);
                    
            }
            else
            {
                UBOOT_DEBUG("---dolby addr [0x%x] size[0x%x] \n",(unsigned int)dolby_temp,(unsigned int)dolby_size);
                MDrv_MSPI_SlaveEnable(false);
                MDrv_MSPI_Write(dolby_temp,dolby_size);
                dolby_size=0;
            }
        }
    MDrv_MSPI_SlaveEnable(true);

    free(dolby_data);
    UBOOT_TRACE("OK\n");
    return ret; 
}

#endif

