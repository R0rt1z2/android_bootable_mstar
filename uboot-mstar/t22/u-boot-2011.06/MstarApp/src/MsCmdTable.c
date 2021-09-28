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
#include <command.h>
#include <MsCmdTable.h>
#include <MsDebug.h>
#include <exports.h>


static struct list_head cmd_list;


void Table_Init(void)
{
    UBOOT_TRACE("IN\n");

    INIT_LIST_HEAD(&cmd_list);

    UBOOT_TRACE("OK\n");    
}

void Add_Command_Table(char * cmd, int flag, int stage)
{

    ST_CMD_RECORED *pcmd=NULL;
    UBOOT_TRACE("IN\n");    
    pcmd=malloc(sizeof(ST_CMD_RECORED));
    if(pcmd==NULL)
    {
        UBOOT_ERROR("malloc for pcmd fail==========\n");
        return;
    }
    memset(pcmd,0,sizeof(ST_CMD_RECORED));
    INIT_LIST_HEAD(&pcmd->list);
    pcmd->cmd=malloc(strlen(cmd)+1);
    if(pcmd->cmd==NULL)
    {
        UBOOT_ERROR("malloc for cmd fail\n");
        free(pcmd);
        return;
    }
    memset(pcmd->cmd,0,strlen(cmd)+1);
    strcpy (pcmd->cmd, cmd);

    pcmd->flag = flag;
    pcmd->stage = stage;    
    list_add_tail(&pcmd->list,&cmd_list);
    
    UBOOT_TRACE("OK\n");    
    return;
}

ST_CMD_RECORED *getFirstCmd(void)
{

    ST_CMD_RECORED *pcmd=NULL;
    UBOOT_TRACE("IN\n");


    if(list_empty(&cmd_list)==1)
    {
        UBOOT_DEBUG("This is an empty list\n");
        return NULL;
    }

    pcmd=list_entry((cmd_list.next),ST_CMD_RECORED,list);
    UBOOT_TRACE("OK\n");    

    return pcmd; 
}

ST_CMD_RECORED *getNextCmd(ST_CMD_RECORED *pCmd)
{
    ST_CMD_RECORED *pcmd=NULL;
    UBOOT_TRACE("IN\n");
    if(pCmd==NULL)
    {
        UBOOT_ERROR("The input parameter 'pCmd' is a numm pointer\n");
        return NULL;
    }
    if(list_is_last(&pCmd->list,&cmd_list)==1)
    {
        UBOOT_DEBUG("This is the last cmd\n");
        return NULL;
    }

    pcmd=list_entry((pCmd->list.next),ST_CMD_RECORED,list);
    UBOOT_TRACE("OK\n");

    return pcmd;
}


int Show_Command_Table(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{    
    int index=0;
    int ShowStage=0;
    ST_CMD_RECORED *pCmd=NULL;

    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
       return cmd_usage(cmdtp);
    }
    ShowStage= (int)argv[1][0]-'0';
    
    UBOOT_INFO("No.  CmdName    Stage\n");

    pCmd=getFirstCmd();
    if(pCmd==NULL)
    {
        UBOOT_DEBUG("There are no any cmds in table\n");
        return 0;
    }
    
    while(1)
    {
        if((pCmd->stage == STAGE_MSINIT) && (ShowStage == 0 ||  ShowStage == 1))
        {
           UBOOT_INFO("%d  %s  STAGE_MSINIT\n",(index+1),pCmd->cmd);
        }
        if ((pCmd->stage == STAGE_PROCESS) && (ShowStage == 0 ||  ShowStage == 2))
        {
           UBOOT_INFO("%d  %s  STAGE_PROCESS\n",(index+1),pCmd->cmd);        
        }
        if ((pCmd->stage == STAGE_TOKERNEL) && (ShowStage == 0 ||  ShowStage == 3))
        {
           UBOOT_INFO("%d  %s  STAGE_TOKERNEL\n",(index+1),pCmd->cmd);        
        }
        pCmd=getNextCmd(pCmd);
        if(pCmd==NULL)
        {
            UBOOT_DEBUG("It's the last cmd\n");
            break;
        }
        index++;
    }
    return 0;
}

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    showtb,    3,    1,     Show_Command_Table,
    "Show register command table."
    "    -  showtb [stage]\n",
    "0:ALL , 1:MsInit , 2:MsProcess , 3:MsToKernel\n"
);
#endif

