/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include "ftlsd_api.h"

U16 SecureNandEndBlkCnt=0;
//===========================================
int FTLSd_BlkDev_Init(void)
{
    if(FTLSd_Init(0, 0))
        return 1;

    #if CONFIG_RESERVED_END_OF_NAND != 0
    ftl_dbg(FTLSd_DBG_LV_LOG,1,"secure size: %Xh  BCnt: %Xh\n", 
        CONFIG_RESERVED_END_OF_NAND, SecureNandEndBlkCnt);
    #endif
}

void FTLSd_Sleep(U32 u32_us)
{
    while(u32_us > 20000)
    {
        udelay(20000);
        u32_us -= 20000;
    }
    udelay(u32_us);
}

void *FSdMALLOC(unsigned int ByteCnt)
{
    void *p;
    p = malloc(ByteCnt+UNFD_CACHE_LINE-1);
    if(NULL==p)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"size: %Xh \n", ByteCnt);
        FTLSd_Die();
        return NULL;
    }
    p = (void*)(((U32)p+(UNFD_CACHE_LINE-1)) & ~(UNFD_CACHE_LINE-1));
    return p;
}

void FSdFREE(void *p)
{
}



