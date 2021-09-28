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

#ifndef _MSTAR_DEBUG_H_
#define _MSTAR_DEBUG_H_

typedef enum
{
    EN_DEBUG_LEVEL_DISABLE=0,    
    EN_DEBUG_LEVEL_ERROR=0x01,
    EN_DEBUG_LEVEL_INFO=0x02,
    EN_DEBUG_LEVEL_TRACE=0x04,
    EN_DEBUG_LEVEL_DEBUG=0x08,
    EN_DEBUG_LEVEL_BOOTTIME=0x10,
    EN_DEBUG_LEVEL_INVALID=0x1000,    
    EN_DEBUG_LEVEL_MAX=EN_DEBUG_LEVEL_INVALID      
}EN_DEBUG_LEVEL;

typedef enum
{
    EN_DEBUG_MODULE_DISABLE=0,
    EN_DEBUG_MODULE_UPGRADE=(0x01<<0),    
    EN_DEBUG_MODULE_FAT=(0x01<<1),
    EN_DEBUG_MODULE_ALL=0xFFFFFFFF
}EN_DEBUG_MODULE;

extern EN_DEBUG_LEVEL dbgLevel;
extern EN_DEBUG_MODULE dbgModule;

#define DEFAULT_DEBUG_LEVEL (EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR)
#define DEFAULT_DEBUG_MODULE (EN_DEBUG_MODULE_ALL)

#if CONFIG_MINIUBOOT
#define UBOOT_ERRDUMP(msg...) 
#define UBOOT_ERROR(msg...)  \
    do{\
        printf("\033[0;31m[ERROR] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
        printf(msg);\
    }while(0)
#define UBOOT_INFO(msg...)  printf(msg)
#define UBOOT_TRACE(msg...) 
#define UBOOT_DEBUG(msg...) 
#define UBOOT_DUMP(addr, size) 
#define UBOOT_BOOTTIME(msg...)
#else
#define UBOOT_ERROR(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_ERROR) \
        { \
            printf("\033[0;31m[ERROR] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printf(msg);\
        } \
    }while(0)
    
#define UBOOT_INFO(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_INFO) \
        { \
                printf(msg);\
        } \
    }while(0)

#define UBOOT_BOOTTIME(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_BOOTTIME) \
        { \
                printf(msg);\
        } \
    }while(0)



            
#if 1
#define UBOOT_TRACE(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_TRACE) \
        { \
                printf("\033[0;32m[TRACE] %s \033[0m",__FUNCTION__);\
                printf(msg);\
        } \
    }while(0)
    
#else
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_TRACE) \
        { \
            if(dbgModule&module) \
            {               \
                printf("\033[0;32m[TRACE]%s\033[0m",__FUNCTION__);\
                printf(msg);\
            }\
        } \
    }while(0)
    
#define UBOOT_TRACE(msg...)   _UBOOT_TRACE(EN_DEBUG_MODULE_ALL,msg)  

#endif

#if 1
#define UBOOT_DEBUG(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_DEBUG) \
        { \
                printf("\033[0;34m[DEBUG] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
                printf(msg);\
        } \
    }while(0)

#else
#define _UBOOT_DEBUG(module, msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_DEBUG) \
        { \
            if(dbgModule&module) \
            {               \
                printf("\033[0;34m[DEBUG]%s:%d:\033[0m",__FUNCTION__,__LINE__);\
                printf(msg);\
            } \
        } \
    }while(0)

#define UBOOT_DEBUG(msg...)   _UBOOT_DEBUG(EN_DEBUG_MODULE_ALL,msg)      
#endif

#define UBOOT_DUMP(addr, size)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_DEBUG) \
        { \
                printf("\033[0;34m[DUMP] %s:%d:\033[0m\n",__FUNCTION__,__LINE__);\
                _dump(addr,size);\
        } \
    }while(0)
            
#define UBOOT_ERRDUMP(addr, size)\
            do{\
                if(dbgLevel&EN_DEBUG_LEVEL_ERROR) \
                { \
                        printf("\033[0;31m[DUMP] %s:%d:\033[0m\n",__FUNCTION__,__LINE__);\
                        _dump(addr,size);\
                } \
            }while(0)


int do_set_debugging_message_level(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_initDebugLevel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);        
void _dump(unsigned int addr, unsigned int size);

#endif
#endif
