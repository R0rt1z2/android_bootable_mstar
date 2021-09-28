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

#ifndef __DRV_URSA_10_C__
#define __DRV_URSA_10_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_10.h"
#include <MsTypes.h>
#include <command.h>
#include <apiSWI2C.h>
#include <MsDebug.h>
//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_CMD_DATA_SUM              (7)
#define URSA_I2C_BUS_NUM               (1)
#define URSA_DDC2BI_MODE_ADDR            (0x40)
#define URSA_I2C_SUB_ADDR              (0)


static SWI2C_BusCfg Ursa_I2cBusCfg[URSA_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    //{PAD_TGPIO2, PAD_TGPIO3, 400},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------


static MS_BOOL MDrv_Ursa_SWI2C_WriteBytes(MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}

static MS_BOOL MDrv_Ursa_SWI2C_ReadBytes( MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}

//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------


void MDrv_Ursa_10_Lvds_init(void)
{
    MS_U8 u10_cmd_init[6][7] =
    {
        {0x80,0x10,0x43,0x00,0xD8,0x28,0x15},
        {0x40,0x11,0x08,0x98,0x04,0x6F,0xA0},
        {0x40,0x12,0x07,0x80,0x04,0x38,0x2F},
        {0x40,0x13,0x50,0x05,0x3C,0x00,0x00},
        {0x01,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x5D,0x01,0x01,0x01,0x00,0x02,0x00}
    };
    MS_U8 cmd_index;


    UBOOT_TRACE("IN\n");
    MApi_SWI2C_Init(Ursa_I2cBusCfg, URSA_I2C_BUS_NUM);
    for(cmd_index = 0; cmd_index < 6; cmd_index++)
    {
        UBOOT_DEBUG("[U10] send LVDS init cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_SWI2C_WriteBytes(u10_cmd_init[cmd_index]))
        {
            UBOOT_ERROR("[U10]Send cmd: FAILED !!!\n");
        }
    }

    UBOOT_TRACE("OK\n");
}


#endif //__DRV_URSA_10_C__
