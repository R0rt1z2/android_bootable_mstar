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

#ifndef __DRV_URSA_vb1_C__
#define __DRV_URSA_vb1_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_vb1.h"
#include <MsSystem.h>
#include <MsTypes.h>
//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_CMD_DATA_SUM              (7)
#define URSA_I2C_BUS_NUM               (1)
#define URSA_DDC2BI_MODE_ADDR            (0x40)
#define URSA_I2C_SUB_ADDR              (0)

#ifndef CMI_UNLOCK_CK
#define CMI_UNLOCK_CK 0
#endif 

#ifndef CMI_UNLOCK_DA
#define CMI_UNLOCK_DA 0
#endif 

static SWI2C_BusCfg Ursa_I2cBusCfg[URSA_I2C_BUS_NUM]=
{
    // Bus-0
    {CMI_UNLOCK_CK, CMI_UNLOCK_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
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

#if 0
static MS_BOOL MDrv_Ursa_SWI2C_ReadBytes( MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}
#endif

//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------


void MDrv_Cmi_Transfer_Unlock(void)
{
    MS_U8    pCmdData_unlock1[URSA_CMD_DATA_SUM] = { 0x09, 0x30,0x00,0x00,0x00,0x00,0x00 };
    MS_U8    pCmdData_unlock2[URSA_CMD_DATA_SUM] = { 0x19, 0x31,0x00,0x00,0x00,0x00,0x00 };
    MS_BOOL  bRet = FALSE;
    UBOOT_TRACE("IN\n");

    printf("CMI Transfer Board Unlock\n");
    if(CMI_UNLOCK_CK==0 ||CMI_UNLOCK_DA==0 )
    {
        return;
        printf("Unsupport Cmi_Transfer_Unlock !!\n");
    }
    MApi_SWI2C_Init(Ursa_I2cBusCfg, URSA_I2C_BUS_NUM);

    UBOOT_DEBUG("CMI Write cmd:%x,%x\n", pCmdData_unlock1[0], pCmdData_unlock1[1]);
    bRet = MDrv_Ursa_SWI2C_WriteBytes(pCmdData_unlock1);
    UBOOT_DEBUG("CMI  send unlock cmd1 return %d\n", bRet);

    UBOOT_DEBUG("CMI Write cmd:%x,%x\n", pCmdData_unlock2[0], pCmdData_unlock2[1]);
    bRet = MDrv_Ursa_SWI2C_WriteBytes(pCmdData_unlock2);
    UBOOT_DEBUG("CMI  send unlock cmd2 return %d\n", bRet);
    UBOOT_TRACE("OK\n");
}




#endif //__DRV_URSA_vb1_C__
