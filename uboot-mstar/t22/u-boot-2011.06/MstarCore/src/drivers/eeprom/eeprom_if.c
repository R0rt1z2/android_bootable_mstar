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

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
//#include "wrap.h"
#include <eeprom_if.h>
//#include "drvcust_if.h"
//#include "x_ldr_env.h"
#include <MsSystem.h>

#include <MsDebug.h>

#if 0 //for testing
U_BOOT_CMD(
	quiteboot,1,        1,	do_QuiteHotBoot,
	"quiteboot  - do_QuiteHotBoot\n",
	"command: quiteboot\n"
);

int do_QuiteHotBoot ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    EEPRON_IsQuiteHotBootMode();
    //mdrv_uart_connect(E_UART_PORT0,E_UART_OFF);
    return 0;
}
#endif

MS_BOOL EEPRON_IsQuiteHotBootMode(void) 
{
    UBOOT_TRACE("IN");
    char cmd[CMD_BUF] ="\0";
    DTVCFG_T prDtvCfg;
    MS_BOOL bIsQuiteBoot= FALSE;
    memset(cmd,0,sizeof(cmd));
    memset(&prDtvCfg, 0, sizeof(prDtvCfg));
    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%x eeprom 0x%x", &prDtvCfg,sizeof(DTVCFG_T));
    UBOOT_DEBUG("Run Cmd:%s \n",cmd);
    if(0 != run_command(cmd, 0))
    {
        UBOOT_ERROR("%s: '%s' fails, at %d\n", __func__, cmd, __LINE__);
        return FALSE;
    }
    if(prDtvCfg.u1Flags2&DTVCFG_FLAG2_QUIET_BOOT)
    {
        bIsQuiteBoot= TRUE;
        printf("Quite Hot Boot Mode !!!! \n");
    }
    else
    {
        bIsQuiteBoot= FALSE;
    }
	UBOOT_TRACE("OK\n");
    return bIsQuiteBoot;
}


#if 0
//-------------------------------------------------------------------------
/** EEPROM_Read
 *  SYSTEM EEPROM read function.
 *  @param  u8Offset     begin address to read.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     read data length in bytes.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Read(UINT64 u8Offset, unsigned long u4MemPtr, UINT32 u4MemLen)
{
    return 0;
}

//-------------------------------------------------------------------------
/** EEPROM_Write
 *  SYSTEM EEPROM write function.
 *  @param  u8Offset     begin address to write.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     writedata length in bytes.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Write(UINT64 u8Offset, unsigned long u4MemPtr, UINT32 u4MemLen)
{
    return 0;
}

INT32 EEPDTV_Read(UINT64 u8Offset, UPTR u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    if ((u8Offset + u4MemLen) > EEPROM_DTV_DRIVER_SIZE)
    {
        return 1;
    }
    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DTV_DRIVER_OFFSET;

    i4Ret = EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}

INT32 EEPDTV_Write(UINT64 u8Offset, UPTR u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    if ((u8Offset + u4MemLen) > EEPROM_DTV_DRIVER_SIZE)
    {
        return 1;
    }
    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DTV_DRIVER_OFFSET;

    i4Ret = EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _EepDtvCorrection()
 *  Make DtvCfg triple redudancy sync.
 *  @param prDtvCfg Output - the final decision DtvCfg data structure pointer.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
static INT32 EEPDTV_Correction(DTVCFG_T* prDtvCfg)
{
    return 0;
}

//-------------------------------------------------------------------------
/** _EepDtvCalChecksum()
 *  Calculate DTVCFG_T checksum value.
 *  @param prDtvCfg the DtvCfg data structure pointer.
 *  @return the UINT8 checksum value.
 */
//-------------------------------------------------------------------------
static UINT8 _EepDtvCalChecksum(const DTVCFG_T* prDtvCfg)
{
    UINT8 u1Ret;
    
    ASSERT(prDtvCfg != NULL);
    u1Ret = 0;
    u1Ret += (prDtvCfg->u1Magic1 + prDtvCfg->u1Magic2);
    u1Ret += (prDtvCfg->u1Flags + prDtvCfg->u1Upgrade);
    u1Ret += (prDtvCfg->u1Upgrade_Ext + prDtvCfg->u1Flags2);

    return u1Ret;
}

//-------------------------------------------------------------------------
/** _EepDtvSyncData()
 *  Copy final DTVCFG_T to the location.
 *  @param prDtvCfg the final DTVCFG data.
 *  @param fgSync1 copy DTVCFG to location 1.
 *  @param fgSync2 copy DTVCFG to location 2.
 *  @param fgSync3 copy DTVCFG to location 3.
 */
//-------------------------------------------------------------------------
static INT32 _EepDtvSyncData(DTVCFG_T* prDtvCfg, UINT32 fgSync1, UINT32 fgSync2, UINT32 fgSync3)
{
    DTVCFG_T rDtvCfg;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;

    x_memset((void*)&rDtvCfg, 0, sizeof(DTVCFG_T));
    ASSERT(prDtvCfg != NULL);
    prDtvCfg->u1Magic1 = DTVCFG_MAGIC_CHAR1;
    prDtvCfg->u1Magic2 = DTVCFG_MAGIC_CHAR2;
    prDtvCfg->u1Checksum = _EepDtvCalChecksum(prDtvCfg);

    if (fgSync1)
    {
        if (EEPDTV_Read((UINT64)0, (UPTR)(void*)&rDtvCfg, sizeof(DTVCFG_T)) != 0)
        {
            return 1;
        }
        if ((rDtvCfg.u1Flags != prDtvCfg->u1Flags)
            || (rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2)
            || (rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade)
            || (rDtvCfg.u1Upgrade_Ext != prDtvCfg->u1Upgrade_Ext) || (rDtvCfg.u1Flags2 != prDtvCfg->u1Flags2)
            )
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1, (UPTR)(void*)prDtvCfg, sizeof(DTVCFG_T)) != 0)
            {
                return 1;
            }
        }
    }

    if (fgSync2)
    {
        if (EEPDTV_Read((UINT64)(sizeof(DTVCFG_T)), (UPTR)(void*)&rDtvCfg, sizeof(DTVCFG_T)) != 0)
        {
            return 1;
        }
        if ((rDtvCfg.u1Flags != prDtvCfg->u1Flags)
            || (rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2)
            || (rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade)
            || (rDtvCfg.u1Upgrade_Ext != prDtvCfg->u1Upgrade_Ext) || (rDtvCfg.u1Flags2 != prDtvCfg->u1Flags2)
            )
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1 + (UINT64)(sizeof(DTVCFG_T)), (UPTR)(void*)prDtvCfg, sizeof(DTVCFG_T)) != 0)
            {
                return 1;
            }
        }
    }

    if (fgSync3)
    {
        if (EEPDTV_Read((UINT64)(sizeof(DTVCFG_T)*2), (UPTR)(void *)&rDtvCfg, sizeof(DTVCFG_T))!=0)
        {
            return 1;
        }
        if ((rDtvCfg.u1Flags != prDtvCfg->u1Flags)
            || (rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2)
            || (rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade)
            || (rDtvCfg.u1Upgrade_Ext != prDtvCfg->u1Upgrade_Ext) || (rDtvCfg.u1Flags2 != prDtvCfg->u1Flags2)
            )
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1 + (UINT64)(sizeof(DTVCFG_T)*2), (UPTR)(void*)prDtvCfg, sizeof(DTVCFG_T)) != 0)
            {
                return 1;
            }
        }

    }

    // save dtvcfg in dram cache
    x_memcpy((void*)prLdrEnv->au1DtvCfg, (void*)prDtvCfg, sizeof(DTVCFG_T));
    return 0;
}

INT32 EEPDTV_GetCfg(DTVCFG_T* prDtvCfg)
{
    INT32 i4Ret;

    static BOOL fgInit = FALSE;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;

    ASSERT(prDtvCfg != NULL);

    if (EEPDTV_Correction(prDtvCfg))
    {
        printf("EEPDTV_Correction fail.\n");
        return 1;
    }

    if (!fgInit)
    {
        // read the first DtvCfg location.
        i4Ret = EEPDTV_Read((UINT64) 0, (void*)(prLdrEnv->au1DtvCfg), sizeof(DTVCFG_T));
        if (i4Ret)
        {
            return i4Ret;
        }
        fgInit = TRUE;
    }

    // load dtvcfg from dram cache
    x_memcpy((void*)prDtvCfg, (void*)(prLdrEnv->au1DtvCfg), sizeof(DTVCFG_T));

    return 0;
}

//-------------------------------------------------------------------------
/** EEPDTV_SetCfg()
 *  @param prDtvCfg the input DtvCfg data structure pointer.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_SetCfg(DTVCFG_T* prDtvCfg)
{
    DTVCFG_T rDtvCfg;
    INT32 i4Ret = 1;

    ASSERT(prDtvCfg != NULL);

    prDtvCfg->u1Magic1 = DTVCFG_MAGIC_CHAR1;
    prDtvCfg->u1Magic2 = DTVCFG_MAGIC_CHAR2;
    prDtvCfg->u1Checksum = _EepDtvCalChecksum(prDtvCfg);

    i4Ret = _EepDtvSyncData(prDtvCfg, 1, 1, 1);

    return i4Ret;
}
#endif
