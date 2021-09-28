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
// Includes
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <command.h>
#include <MsDebug.h>
#include <MsTypes.h>
#include <ShareType.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define ENV_DEBUG_LEVLE "dbgLevel"

#define STR_ERROR "ERROR"
#define STR_INFO "INFO"
#define STR_TRACE "TRACE"
#define STR_DEBUG "DEBUG"
#define STR_BOOTTIME "BOOTTIME"

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
EN_DEBUG_LEVEL dbgLevel=DEFAULT_DEBUG_LEVEL;
EN_DEBUG_MODULE dbgModule=DEFAULT_DEBUG_MODULE;


//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);


//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static int _initDebugLevel(void);

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
int do_set_debugging_message_level(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[CMD_BUF] = "\0";   
    int ret=0;
    
    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    if(argv[1]==NULL)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    
    snprintf((char *)buffer,CMD_BUF,"setenv %s %s",ENV_DEBUG_LEVLE,argv[1]);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        printf("\033[0;31m[ERROR]%s:0x%x:\033[0m",__FUNCTION__,__LINE__);
        printf("set %s to env fail.\n",ENV_DEBUG_LEVLE);        
        return -1;
    }

    snprintf((char *)buffer,CMD_BUF,"saveenv");
    ret=run_command(buffer,0);    
    if(ret==-1)
    {
        printf("\033[0;31m[ERROR]%s:0x%x::\033[0m",__FUNCTION__,__LINE__);
        printf("save %s to env fail\n",ENV_DEBUG_LEVLE);        
        return -1;
    }

    _initDebugLevel();
    
    return 0;
}

int do_initDebugLevel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
     if(argc!=1)
     {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
     }
     
    _initDebugLevel();
    return 0;
}

static int _initDebugLevel(void)
{
    char *pEnv=NULL;

    pEnv=getenv(ENV_DEBUG_LEVLE);
    if(pEnv==NULL)
    {
        dbgLevel=DEFAULT_DEBUG_LEVEL;
        return 0;
    }

    if(strcmp(pEnv,STR_ERROR)==0)
    {
        dbgLevel=EN_DEBUG_LEVEL_ERROR;
        return 0;
    }
    else if(strcmp(pEnv,STR_INFO)==0)
    {
        dbgLevel=(EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR);
        return 0;
    }
    else if(strcmp(pEnv,STR_TRACE)==0)
    {
        dbgLevel=(EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR+EN_DEBUG_LEVEL_TRACE);
        return 0;
    }
    else if(strcmp(pEnv,STR_DEBUG)==0)
    {
        dbgLevel=(EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR+EN_DEBUG_LEVEL_TRACE+EN_DEBUG_LEVEL_DEBUG+EN_DEBUG_LEVEL_BOOTTIME);
        return 0;
    }
    else if(strcmp(pEnv,STR_BOOTTIME)==0)
    {
        dbgLevel=EN_DEBUG_LEVEL_BOOTTIME+DEFAULT_DEBUG_LEVEL;
        return 0;
    }   
    else
    {
        dbgLevel=DEFAULT_DEBUG_LEVEL;
    }
    
    return 0;
}

void _dump(unsigned int addr, unsigned int size)
{
    char buffer[CMD_BUF]="\0";
    snprintf(buffer,CMD_BUF,"md.b %x %x",addr,size);
    run_command(buffer,0);
}


