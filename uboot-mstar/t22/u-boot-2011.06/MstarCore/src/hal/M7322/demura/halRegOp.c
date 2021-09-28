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
#include <MsTypes.h>
#include <stdio.h>
#include <command.h>
#include <MsDebug.h>


static MS_VIRT _gMIO_MapBase = 0;

void HAL_DEMURA_SetIOMapBase(MS_VIRT nonpm_base)
{
    if (_gMIO_MapBase == 0)
    {
        _gMIO_MapBase = nonpm_base;
        UBOOT_DEBUG("Mstar Demura IOMap base:%8x, Reg offset : %s\n", _gMIO_MapBase, "None_PM_Bank");
    }
}


MS_U8 HAL_DEMURA_ReadByte(MS_PHY phy64RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(phy64RegAddr << 1) - (phy64RegAddr & 1)];
}

MS_U16 HAL_DEMURA_Read2Byte(MS_PHY phy64RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[phy64RegAddr];
}

void HAL_DEMURA_WriteByte(MS_PHY phy64RegAddr, MS_U8 u8Val)
{
    ((volatile MS_U8*)(_gMIO_MapBase))[(phy64RegAddr << 1) - (phy64RegAddr & 1)] = u8Val;
}

void HAL_DEMURA_Write2Byte(MS_PHY phy64RegAddr, MS_U16 u16Val)
{
    ((volatile MS_U16*)(_gMIO_MapBase))[phy64RegAddr] = u16Val;
}


MS_U8 HAL_DEMURA_ReadByteMask(MS_PHY phy64RegAddr, MS_U8 u8mask)
{
    return  (HAL_DEMURA_ReadByte(phy64RegAddr) & u8mask);
}

MS_U16 HAL_DEMURA_Read2ByteMask(MS_PHY phy64RegAddr, MS_U16 u16mask)
{
    return  (HAL_DEMURA_Read2Byte(phy64RegAddr) & u16mask);
}

void HAL_DEMURA_WriteByteMask(MS_PHY phy64RegAddr, MS_U8 u8Val, MS_U8 u8mask)
{
    MS_U8 regVal = HAL_DEMURA_ReadByte(phy64RegAddr);

    regVal = regVal & (~(u8mask));
    regVal = regVal | (u8Val & u8mask);
    HAL_DEMURA_WriteByte(phy64RegAddr, regVal);
}

void HAL_DEMURA_Write2ByteMask(MS_PHY phy64RegAddr, MS_U16 u16Val, MS_U16 u16mask)
{
    MS_U16 regVal = HAL_DEMURA_Read2Byte(phy64RegAddr);

    regVal = regVal & (~(u16mask));
    regVal = regVal | (u16Val & u16mask);
    HAL_DEMURA_Write2Byte(phy64RegAddr, regVal);
}
