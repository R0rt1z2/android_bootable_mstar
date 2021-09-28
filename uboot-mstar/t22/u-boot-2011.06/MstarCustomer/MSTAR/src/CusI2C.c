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
#include <common.h>
#include <apiSWI2C.h>
#if (CONFIG_TV_CHIP == 0)
#include <drvIIC.h>
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#if !defined(CONFIG_STB_ECOS_BOOT)
#if (CONFIG_TV_CHIP == 0)
#define E_I2C_BUS_NUM 8
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
 {
	//Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C    /SCL_PAD /SDA_PAD /Delay
 	// Bus-0
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100}, //IS_SW_I2C    /SCL_PAD /SDA_PAD /Delay
 	// Bus-1
	{PAD_I2CM1_SCL, PAD_I2CM1_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
 	// Bus-2
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-3
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-4
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
 	// Bus-5
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-6
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-7
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Others, add other bus here
};
#else
#if defined(CONFIG_MSTAR_M5621) || defined(CONFIG_MSTAR_M7622)
#define E_I2C_BUS_NUM 9
#else
#define E_I2C_BUS_NUM 8
#endif
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
{
	//Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-0
	{PAD_DDCR_CK,PAD_DDCR_DA,100}, //IS_SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-1
	{PAD_TGPIO0, PAD_TGPIO1,100},
        // Bus-2
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-3
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-4
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-5
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-6
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-7
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
#if defined(CONFIG_MSTAR_M5621)
	// Bus-8
	{PAD_DDCDD_CK, PAD_DDCDD_DA,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
#endif
#if defined(CONFIG_MSTAR_M7622)
	{PAD_GPIO2_PM, PAD_GPIO9_PM,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
#endif
        // Others, add other bus here
};
#endif
#endif

int msI2C_init(void)
{
#if (CONFIG_TV_CHIP==0)
#if defined(CONFIG_STB_ECOS_BOOT)
#if (CONFIG_OAD_IN_MBOOT == 1)
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(0);
    printf("IIC port == %d\n", hwi2c_port);
    MDrv_IIC_Init(hwi2c_port);
#endif
#endif
#else
	MApi_SWI2C_Init(I2CBusCfg,E_I2C_BUS_NUM);
#endif
	return 0;
}

