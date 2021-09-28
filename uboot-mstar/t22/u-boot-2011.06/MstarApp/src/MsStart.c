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

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <ShareType.h>
#include <MsDevice.h>
#include <common.h>
#include <command.h>
#include <MsCmdTable.h>
#include <MsAppRegister.h>
#include <MsCustomerRegister.h>
#include <MsDebug.h>
#include <MsStart.h>
#include <MsSystem.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  MstarToKernel
/// @brief         :  This function is always executed when mboot booting.
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN MstarProcess(void)
{
#if CONFIG_MINIUBOOT
    run_command("updatemiureg", 0);
    run_command("bootargs_set", 0);
#else
    ST_CMD_RECORED *pCmd=NULL;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m --------------------MstarProcess---------------------------\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");

    Customer_Register_Process();
    Customer_Register_ToKernel();


    pCmd=getFirstCmd();
    if(pCmd==NULL)
    {
        UBOOT_DEBUG("There are no any cmds in table\n");
        return TRUE;
    }
    
    while(1)
    {
        
        if(pCmd->stage == STAGE_PROCESS)
        {
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_start\n",pCmd->cmd, MsSystemGetBootTime());
            run_command(pCmd->cmd,  pCmd->flag);
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_end\n",pCmd->cmd, MsSystemGetBootTime());
        }

        pCmd=getNextCmd(pCmd);
        if(pCmd==NULL)
        {
            UBOOT_DEBUG("It's the last cmd\n");
            break;
        }
    }
    
    UBOOT_TRACE("OK\n");     
 #endif
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  MstarToKernel
/// @brief         :  This function is only executed when mboot jumps to kernel.
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN MstarToKernel(void)
{
#if CONFIG_MINIUBOOT
    run_command("bootcheck", 0);
    #if (CONFIG_PANEL_INIT)
    run_command("panel_pre_init", 0);
    #endif

    #if (ENABLE_HDMI_TX == 1)
    run_command("hdmi init", 0);
    #endif

    #ifdef CONFIG_DISPLAY_LOGO
    run_command("bootlogo", 0);
    #endif
#else
    ST_CMD_RECORED *pCmd=NULL;
    UBOOT_TRACE("IN\n");     
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ------------------ MstarToKernel---------------------------\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");


    pCmd=getFirstCmd();
    if(pCmd==NULL)
    {
        UBOOT_DEBUG("There are no any cmds in table\n");
        return TRUE;
    }
    
    while(1)
    {
        if(pCmd->stage == STAGE_TOKERNEL)
        {
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_start\n",pCmd->cmd, MsSystemGetBootTime());
            run_command(pCmd->cmd,  pCmd->flag);
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_end\n",pCmd->cmd, MsSystemGetBootTime());
        }
        pCmd=getNextCmd(pCmd);
        if(pCmd==NULL)
        {
            UBOOT_DEBUG("It's the last cmd\n");
            break;
        }
    }
    
    UBOOT_TRACE("OK\n");
 #endif
    return TRUE;
}

