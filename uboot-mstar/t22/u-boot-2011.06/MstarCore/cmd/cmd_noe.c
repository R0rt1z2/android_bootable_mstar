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
 * NOE Read/Write Utilities
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <net.h>

/*-----------------------------------------------------------------------
 * Definitions
 */

typedef enum {
    E_NOE_CMD_READ = 0,
    E_NOE_CMD_WRITE,
} EN_NOE_CMD_TYPE;

typedef enum {
    E_NOE_CMD_BIST_MODE_NORMAL = 0,
    E_NOE_CMD_BIST_MODE_PHY_LOOPBACK,
    E_NOE_CMD_BIST_MODE_MAC_LOOPBACK,
} EN_NOE_CMD_BIST_MODE;

#define NOE_INPUT_LEN          2
#define NOE_ADDR_INPUT_LEN     7

#define MACADDR_ENV    "macaddr"
#define MACADDR_FORMAT "XX:XX:XX:XX:XX:XX"

extern u8 MY_MAC[6];
extern int noe_init;
extern void MDrv_NOE_Dump_Register(void);
extern int MDrv_NOE_Initialize(const bd_t * bis);
extern u8 MDrv_NOE_Bist(u8 bEnable);
extern int MDrv_NOE_eWave(int mode, int subitem);
extern int MDrv_NOE_Loopback_Is_Failed(void);

int do_noe(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    bd_t * bis=NULL;
    char * macaddr;

    if (   NULL != (macaddr = getenv(MACADDR_ENV))
        && strlen(macaddr) == strlen(MACADDR_FORMAT)
        && ':' == macaddr[2]
        && ':' == macaddr[5]
        && ':' == macaddr[8]
        && ':' == macaddr[11]
        && ':' == macaddr[14]
        )
    {
        macaddr[2]  = '\0';
        macaddr[5]  = '\0';
        macaddr[8]  = '\0';
        macaddr[11] = '\0';
        macaddr[14] = '\0';

        MY_MAC[0]   = (u8)simple_strtoul(&(macaddr[0]),  NULL, 16);
        MY_MAC[1]   = (u8)simple_strtoul(&(macaddr[3]),  NULL, 16);
        MY_MAC[2]   = (u8)simple_strtoul(&(macaddr[6]),  NULL, 16);
        MY_MAC[3]   = (u8)simple_strtoul(&(macaddr[9]),  NULL, 16);
        MY_MAC[4]   = (u8)simple_strtoul(&(macaddr[12]), NULL, 16);
        MY_MAC[5]   = (u8)simple_strtoul(&(macaddr[15]), NULL, 16);

        /* set back to ':' or the environment variable would be destoried */ // REVIEW: this coding style is dangerous
        macaddr[2]  = ':';
        macaddr[5]  = ':';
        macaddr[8]  = ':';
        macaddr[11] = ':';
        macaddr[14] = ':';
    }

    MDrv_NOE_Initialize(bis);
    printf("(Re)start NOE...\n");

    return 0;
}

int do_setMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *cmdline_buf = NULL;
    if ((argc < NOE_ADDR_INPUT_LEN) || (argc > NOE_ADDR_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MY_MAC[0]    = simple_strtoul(argv[1], NULL, 16);
    MY_MAC[1]    = simple_strtoul(argv[2], NULL, 16);
    MY_MAC[2]    = simple_strtoul(argv[3], NULL, 16);
    MY_MAC[3]    = simple_strtoul(argv[4], NULL, 16);
    MY_MAC[4]    = simple_strtoul(argv[5], NULL, 16);
    MY_MAC[5]    = simple_strtoul(argv[6], NULL, 16);

    cmdline_buf = malloc(64);
    if(cmdline_buf == NULL)
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__); //cyber
        return -1;
    }
    sprintf(cmdline_buf, "setenv "MACADDR_ENV" %02X:%02X:%02X:%02X:%02X:%02X", MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);

    run_command(cmdline_buf, 0);
    run_command("saveenv", 0);
    free(cmdline_buf);

    printf("New MAC Address is %02X:%02X:%02X:%02X:%02X:%02X\n", MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);

    return 0;
}


int noe_cmd_dump_reg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }
    printf("Dump NOE Register\n");
    MDrv_NOE_Dump_Register();
    return 0;
}

int do_bistMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u8 count = simple_strtoul(argv[1], NULL, 16);

    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }

    if (0 == count)
        printf("Usage:\n ebist count (count:0~255 times)\n Ex: ebist 10\n");
    else
    {
        u8 idx= 0;
        char buffer[256], ip_addr[][15] =
            {
                "10.0.0.253",
                "127.0.0.1",
                "172.16.90.254",
                "192.168.0.1",
                "224.30.0.253",
                "0"
            };

        if (MDrv_NOE_Bist(E_NOE_CMD_BIST_MODE_PHY_LOOPBACK))
        {
            for (; idx < count; idx++)
            {
                memset(buffer, 0 , sizeof(buffer));
                sprintf(buffer, "ping %s", ip_addr[idx%5]);
                printf("\n=========>  %s for PHY Loopback test.....\n", buffer);
                run_command(buffer, 0);
            }
            if (MDrv_NOE_Loopback_Is_Failed()) {
                printf("\n### NOE PHY loopback test FAILED ###\n");
            }
            else {
                printf("\n### NOE PHY loopback test SUCCEEDED ###\n");
            }
        }
        MDrv_NOE_Bist(E_NOE_CMD_BIST_MODE_NORMAL);
    }
    return 0;
}

int do_loadspi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ulong ram_start;
    ulong spi_start;
    ulong len,i;
    if (argc != 4) {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    spi_start = simple_strtoul(argv[1], NULL, 10);
    ram_start = simple_strtoul(argv[2], NULL, 10);
    len       = simple_strtoul(argv[3], NULL, 10);
    printf("copy from spi(%lux) to ram(%lux) with len=%lux\n",spi_start,ram_start,len);

    for(i=0;i<len;i+=4)
    {
        if(0==i%1024)printf(".");
        *((ulong*)(ram_start+i))=*((ulong*)(spi_start+i));
        //printf("[%x]=%x,[%x]=%x\n",ram_start+i,*((ulong*)(ram_start+i)),spi_start+i,*((ulong*)(spi_start+i)));
    }

    return 0;
}

int noe_cmd_ewave(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u8 mode     = simple_strtoul(argv[1], NULL, 10);
    u8 subitem  = simple_strtoul(argv[2], NULL, 10);

    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }

    if (argc < 3) {
        return cmd_usage(cmdtp);
    }

    printf("select NOE ewavetest (mode, subitem): (%d, %d)...\n", mode, subitem);
    if (MDrv_NOE_eWave(mode, subitem)) {
        return cmd_usage(cmdtp);
    }
    else {
        return 0;
    }
}

U_BOOT_CMD(
    estart, NOE_INPUT_LEN, 1,  do_noe,
    "NOE start",
    "reset  - reset NOE controller\n"
    "start  - start NOE controller\n"
);

U_BOOT_CMD(
    macaddr, NOE_ADDR_INPUT_LEN,   1,  do_setMac,
    "setup NOE MAC addr",
    "XX XX XX XX XX XX"
);

U_BOOT_CMD(
    noe_dump_reg,   4,  1,  noe_cmd_dump_reg,
    "noe_dump_reg   - Dump NOE relevant register !!\n",
    "noe_dump_reg   - Dump NOE relevant register !!\n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    ebist, NOE_INPUT_LEN,  1,  do_bistMac,
    "PHY loopback test",
    "ebist - PHY loopback test\n"
);

U_BOOT_CMD(
    loadspi, 4, 4,  do_loadspi,
    "load data from SPI",
    "XX XX XX XX XX XX"
);

U_BOOT_CMD(
    noe_ewave,   3,  1,  noe_cmd_ewave,
    "NOE ewave commands",
    "noe_ewave <mode> <subitem> - select ewave test <mode> <subitem> !!\n"
    "mode[0]-10M:\n"
    "         subitem[0] - Pseudo-random pattern\n"
    "         subitem[1] - FF pattern\n"
    "         subitem[2] - 00 pattern\n"
    "         subitem[3] - Finish 10M measurement\n"
    "mode[1]-100M:\n"
    "         subitem[0] - Output MLT-3 from channel A\n"
    "         subitem[1] - Output MLT-3 from channel B\n"
    "         subitem[2] - Finish 100M measurement\n"
    "mode[2]-1000M:\n"
    "         subitem[0] - Enable test Mode 1\n"
    "         subitem[1] - Enable test Mode 4\n"
    "         subitem[2] - Finish 1000M measurement\n"
);
#endif

