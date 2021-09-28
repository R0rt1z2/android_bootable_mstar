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

#ifndef __MS_STR__
#define __MS_STR__

#include <ShareType.h>
#include <MsTypes.h>
/*
MIU0_GROUP_PRIORITY=1:0:2:3
MIU1_GROUP_PRIORITY=1:0:2:3
MIU2_GROUP_PRIORITY=0:0:0:0
MIU0_GROUP_SELMIU=1140:0000:0190:0000:005D:0000:0000
MIU1_GROUP_SELMIU=4E08:70F8:0A20:0000:0002:7FFF:29FF
MIU2_GROUP_SELMIU=0000:0000:0000:0000:0000:0000:0000
*/

#define MIU_COUNT 4         // MIU0,MIU1,MIU2,MIU3.....
#define MIU_GROUP_COUNT 7

typedef struct
{
    U32 u32MiuPriority[MIU_COUNT];
    U32 u32MiuSel[MIU_COUNT][MIU_GROUP_COUNT];
}ST_MIU_INFO;


typedef struct
{
    U32 u32NuttxAddr;
    U32 u32AESAddr;
    ST_MIU_INFO stMiuInfo;
}ST_FSTR_BACKUP;



int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
void CRCCheck(void);
int check_str_resume(void);
void fast_STR_miu_Backup(ST_MIU_INFO * stMiuInfo);
int do_fast_str_backup(void);
#endif

