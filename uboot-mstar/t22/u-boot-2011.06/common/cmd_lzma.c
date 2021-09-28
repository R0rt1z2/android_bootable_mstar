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

/*
 * Tuner handling routines
 */

#include <common.h>
#include <command.h>
#include <configs/uboot_module_config.h>
#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <drvIIC.h>

#include "lzmadec.h"

//#if ((CONFIG_COMMANDS&(CFG_CMD_DEMOCODE)) == (CFG_CMD_DEMOCODE))

/* =============================================================== */

static unsigned char *g_alloc_buf;


static void *SzAlloc(void *p, size_t size)
{
    unsigned char *x = g_alloc_buf;
    p = p;
    g_alloc_buf += size;
    return x;
}

static void SzFree(void *p, void *address)
{
    p = p;
    address = address;
}


/*
 * Subroutine:  do_lzma
 *
 * Description: do lzma for compress image
 *
 * Inputs:	argv[1]: u32LzmaBufAddr
 *
 * Inputs:	argv[2]: u32AppFileLen
 *
 * Inputs:	argv[3]: u32AppDramAddr
 *
 * Inputs:	argv[4]: u32LzmaDecBufAddr
 *
 * Return:      unpack_size
 *
 */
 typedef unsigned char  U8;
 typedef unsigned short U16;
 typedef unsigned long  U32;
int do_LzmaDec(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    U32 in;
    U32 in_size;
    U32 out;
    U32 alloc_buf;
    const unsigned char *header;
    unsigned long long unpack_size;
    int i;
    ELzmaStatus status;
    SRes res;

    SizeT in_size_pure;
    SizeT out_size;
    ISzAlloc g_Alloc = { SzAlloc, SzFree };

    in     = simple_strtoul(argv[1], NULL, 16);
    in_size     = simple_strtoul(argv[2], NULL, 16);
    out     = simple_strtoul(argv[3], NULL, 16);
    alloc_buf     = simple_strtoul(argv[4], NULL, 16);

   // printf("in=0x%x, in_size=0x%x, out= 0x%x, alloc_buf = 0x%x \n", in, in_size, out,alloc_buf);

    header = (unsigned char *)in;
    in_size_pure = in_size - (LZMA_PROPS_SIZE + 8);

    g_alloc_buf = (unsigned char *)alloc_buf;

    unpack_size = 0;
    for (i = 0; i < 8; i++)
        unpack_size += (unsigned long long)header[LZMA_PROPS_SIZE + i] << (i * 8);
    out_size = unpack_size;

    //printf("in=0x%x, in_size=0x%x, out= 0x%x, alloc_buf = 0x%x \n", in, in_size, out,alloc_buf);

    //printf("in_size=%d out_size=%d unpack_size=%d \n", in_size, out_size, unpack_size);
    printf("\r\nLZMA Decompression...");
    res = LzmaDecode((Byte *)out, &out_size, ((unsigned char *)in) + (LZMA_PROPS_SIZE + 8), &in_size_pure, header, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &status, &g_Alloc);
    if(res != SZ_OK) {
        printf("fail\r\n");
        return -1;
    }
    //printf("res=%d status=%d out_size=%d\n", res, status, out_size);
    flush_cache(0x80000000,0x1000000);
    Chip_Flush_Memory();
    U32 CRC_result=0;
    U32 CRC_value=0;
    U32 addr=0;
    /* Add CRC Check for LZMA */
    addr = out+(out_size-4);
    CRC_value = CRC_result+1;
    if(addr > out)
    {
        CRC_value=*((U32*)(addr));
    }
    CRC_result=crc32( 0x00, (U8*)out, out_size-4 );
    //printf("CRC_result %.8x CRC_value %.8x\n",CRC_result,CRC_value);
    if(CRC_result!= CRC_value)
    {
        //CRC error
        printf("CRC check error !!\n");
        while(1);
    }
    else
    {
        printf("CRC check success !!\n");
    }

    printf("ok\r\n");
    return unpack_size;

}


U_BOOT_CMD(
	LzmaDec,	5,	1,	do_LzmaDec,
	"do Lzma for compress image\n",
	"arg[1]: u32LzmaBufAddr\n"
	"arg[2]: u32AppFileLen\n"
	"arg[3]: u32AppDramAddr\n"
	"arg[4]: u32LzmaDecBufAddr\n"
);




//#endif /* (CONFIG_COMMANDS) */

