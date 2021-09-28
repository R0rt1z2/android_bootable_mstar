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
void FTLSd_Sleep(U32 u32_us)
{
    nand_hw_timer_delay(u32_us);
}

U8 FtlSdReservedBuf[FTLSD_RESERVED_RAM_SIZE] __attribute__((aligned(UNFD_CACHE_LINE)));

void *FSdMALLOC(unsigned int ByteCnt)
{
    void *p;
    
    #if 1
    static volatile U32 UsedByteCnt=0;

    if(ByteCnt + UsedByteCnt > FTLSD_RESERVED_RAM_SIZE)
    {
        LDR_PUTS("FtlSd has no memory\n");
        return NULL;
    }

    p = (U32)(FtlSdReservedBuf + UsedByteCnt);
    
    UsedByteCnt += ByteCnt;

    #else

    p = malloc(ByteCnt+UNFD_CACHE_LINE-1);
    if(NULL==p)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"size: %Xh \n", ByteCnt);
        FTLSd_Die();
        return NULL;
    }
    p = (void*)(((U32)p+(UNFD_CACHE_LINE-1)) & ~(UNFD_CACHE_LINE-1));
    #endif
    return p;
}

void FSdFREE(void *p)
{
}


//===========================================


