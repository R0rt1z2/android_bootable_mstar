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

#ifndef  _API_SBOOT_H_
#define _API_SBOOT_H_
//-------------------------------------------------------------------------------------------------
// Version Control
//-------------------------------------------------------------------------------------------------
#define MSIF_TAG                    {'M','S','I','F'}                   // MSVC
#define MSIF_CLASS                  {'0','0'}                           // DRV/API (DDI)
#define MSIF_CUS                    {'0','0','S','3'}
#define MSIF_QUALITY                0
#define MSIF_MOD                    {'M','B','O','T'}
#define MSIF_VER                   {'0','2','0','6'}
#define MSIF_SBT_CHANGELIST         {'0','0','0','0','0','0','0','0'}    //P4 ChangeList Number
#define MSIF_MAJORVER               {'0','2'}
#define MSIF_MINORVER               {'0','6'}
#define MSIF_OS                     '2' 

#define SBT_VER                  /* Character String for SBOOT version               */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* '00S0'                                           */  \
    MSIF_QUALITY,                   /* 0                                                */  \
    MSIF_MOD,                       /* 'SBT_'                                           */  \
    MSIF_VER,                      /* '0000'                                         */  \
    MSIF_SBT_CHANGELIST,            /* CL#                                              */  \
    {'0','0','0'}
    
#define SBT_VER_MSIF                /* Character String for SBOOT version               */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    {0,0},                          /* '0x0000'                                         */  \
    {0,0},                          /* '0x0000'                                         */  \
    {0,0},                          /* '0x0000'                                         */  \
    '0',                                                                                    \
    MSIF_MOD,                       /* 'MBOT'                                           */  \
    MSIF_MAJORVER,                  /* '02'                                             */  \
    MSIF_MINORVER,                  /* '06'                                             */  \
    MSIF_SBT_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS

typedef union _MSIF_Version
{
    struct _DDI
    {
        U8                       tag[4];
        U8                       type[2];
        U16                      customer;
        U16                      model;
        U16                      chip;
        U8                       cpu;
        U8                       name[4];
        U8                       version[2];
        U8                       build[2];
        U8                       change[8];
        U8                       os;
    } DDI;
    struct _MW
    {
        U8                                     tag[4];
        U8                                     type[2];
        U16                                    customer;
        U16                                    mod;
        U16                                    chip;
        U8                                     cpu;
        U8                                     name[4];
        U8                                     version[2];
        U8                                     build[2];
        U8                                     changelist[8];
        U8                                     os;
    } MW;
    struct _APP
    {
        U8                                     tag[4];
        U8                                     type[2];
        U8                                     id[4];
        U8                                     quality;
        U8                                     version[4];
        U8                                     ver[4];
        U8                                     changelist[8];
        U8                                     reserve[3];
    } APP;
    struct _BOOT
    {
        U8                                     tag[4];
        U8                                     type[2];
        U8                                     customer[2];
        U8                                     mod[2];
        U8                                     chip[2];
        U8                                     cpu;
        U8                                     name[4];
        U8                                     version[2];
        U8                                     build[2];
        U8                                     changelist[8];
        U8                                     os;
    } BOOT;
} MSIF_Version;

const MSIF_Version _sbt_version = {
    .APP = { SBT_VER }
};

MSIF_Version _sbt_version_msif = {
    .BOOT = { SBT_VER_MSIF }
};

#endif //_API_SBOOT_H_
