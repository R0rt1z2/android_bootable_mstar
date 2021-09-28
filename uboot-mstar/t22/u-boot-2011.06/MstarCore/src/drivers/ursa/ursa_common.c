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
#ifndef __DRV_URSA_COMMON_C__
#define __DRV_URSA_COMMON_C__
#include "ursa_common.h"
#include <command.h>
#include <common.h>
#include <MsSystem.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
#include <bootlogo/MsPoolDB.h>

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif

/*
 * creating the global URSA command table here.
 */
static MS_BOOL gbGetUrsaType = FALSE;
static int  gintUrsa_type_num = URSA_COMMON_UNKNOWN;

URSA_COMMON_STATUS MDrv_URSA_COM_Write_URSA_HandshakeType(void)
{
    UBOOT_TRACE("IN\n");

    URSA_COMMON_TYPE ursa_type = MDrv_URSA_COM_Get_URSA_Type();

    switch (ursa_type)
    {
        case URSA_COMMON_U6:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U7:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA7_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U8:        // it don't need handshake
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U9:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA7_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U20:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U20.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U20;

        case URSA_COMMON_KS2:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_FRC:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_UNKNOWN:
            UBOOT_ERROR("[URSA_COMMON] Unknown URSA type !\n");
            UBOOT_TRACE("OK\n");
            return URSA_FAILED;

        default:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is a illeagal value !\n");
            UBOOT_TRACE("OK\n");
            return URSA_FAILED;
    }
}

URSA_COMMON_TYPE MDrv_URSA_COM_Get_URSA_Type(void)
{
    UBOOT_TRACE("IN\n");

    int ursa_type_num = URSA_COMMON_UNKNOWN;
    int ret = -1;
    st_sys_misc_setting misc_setting;
    char *ursa_type = NULL;
    char UrsaTypeBuf[BUFFER_SIZE] = "\0";

#ifdef UFBL_FEATURE_IDME
    if (!idme_get_var_external("memc", UrsaTypeBuf, BUFFER_SIZE-1))
    {
        if(0==strcmp(UrsaTypeBuf, "0"))
        {
            ursa_type_num = 0;
        }
        else
        {
            ursa_type_num = 20;
        }
        UBOOT_DEBUG("[URSA_COMMON]Get Ursa Type from idme: --------> %d \n",(int)ursa_type_num);
        ret = 0;
    }
#endif

    if(0 != ret)
    {
        ursa_type = getenv("ursa_type");

        if (NULL != ursa_type)
        {
            ursa_type_num = simple_strtoul(ursa_type, NULL, 10);
            UBOOT_DEBUG("[URSA_COMMON] get env ursa_type value=%s\n", ursa_type);
        }
        else
        {
            extern MS_BOOL getDBtableReadyFlag(void);
            if(getDBtableReadyFlag() == FALSE)
            {
                UBOOT_DEBUG("[URSA_COMMON] call dbtable_init to update DB table\n");
                run_command("dbtable_init", 0);
            }
            if(gbGetUrsaType == FALSE)
            {
                Read_MiscSetting_ToFlash(&misc_setting);
                ursa_type_num = misc_setting.m_u32ursa_type;
                gbGetUrsaType = TRUE;
                gintUrsa_type_num = ursa_type_num;
                UBOOT_DEBUG("[URSA_COMMON] get DB table MiscSetting ursa_type_num value = %d\n", ursa_type_num);
            }
            else
            {
                ursa_type_num = gintUrsa_type_num;
                UBOOT_DEBUG("[URSA_COMMON] ursa_type_num value = %d \n", ursa_type_num);
            }
        }
    }

    switch (ursa_type_num)
    {
        case URSA_COMMON_U6:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U6.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U6;

        case URSA_COMMON_U7:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U7.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U7;

        case URSA_COMMON_U8:        // it don't need handshake
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U8.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U8;

        case URSA_COMMON_U9:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U9.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U9;

        case URSA_COMMON_U20:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U20.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U20;

        case URSA_COMMON_KS2:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is KS2.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_KS2;

        case URSA_COMMON_FRC:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is FRC.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_FRC;

        case URSA_COMMON_U11:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is FRC.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U11;

        case URSA_COMMON_UNKNOWN:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is an illeagal value !\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_UNKNOWN;

        default:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is a illeagal value !\n");
            return URSA_FAILED;
    }

}
#endif //__DRV_URSA_COMMON_C__
