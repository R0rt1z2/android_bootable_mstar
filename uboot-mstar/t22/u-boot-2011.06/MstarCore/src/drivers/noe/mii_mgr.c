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

#include <common.h>
#include <command.h>
#include "mhal_noe.h"

#if defined (CONFIG_MIPS16)
#define outw(address, value)    *((volatile uint32_t *)(address)) = (value)
#define inw(address)            (*(volatile u32 *)(address))
#else
#define outw(address, value)    *((volatile uint32_t *)(address)) = cpu_to_le32(value)
#define inw(address)            le32_to_cpu(*(volatile u32 *)(address))
#endif

#define PHY_CONTROL_0       0x0004
#define MDIO_PHY_CONTROL_0  (NOE_ETH_MAC_BASE + PHY_CONTROL_0)

#define enable_mdio(x)

#define CFG_HZ              1000

u32 __mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data)
{
    u32 volatile            status  = 0;
    u32 volatile            data    = 0;
    u32             rc      = 0;
    unsigned long volatile  t_start = get_timer(0);

    //printf("\n MDIO Read operation!!\n");
    // make sure previous read operation is complete
    t_start = get_timer(0);
    while(1)
    {
        // 0 : Read/write operation complet
        if(!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31)))
        {
            break;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Read operation is ongoing !!\n");
            return rc;
        }
        udelay(10);
    }

    data  = (0x01 << 16) | (0x02 << 18) | (phy_addr << 20) | (phy_register << 25);
    outw(MDIO_PHY_CONTROL_0, data);
    data |= (1<<31);
    outw(MDIO_PHY_CONTROL_0, data);
    //printf("\n Set Command [0x%08X] to PHY 0x%8x!!\n", data, MDIO_PHY_CONTROL_0);


    // make sure read operation is complete
    t_start = get_timer(0);
    while(1)
    {
        if(!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31)))
        {
            status = inw(MDIO_PHY_CONTROL_0);
            *read_data = (u32)(status & 0x0000FFFF);
            //printf("\n MDIO_PHY_CONTROL_0: 0x%8x!!\n", status);

            return 1;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Read operation is ongoing and Time Out!!\n");
            return 0;
        }
        udelay(10);
    }
}

u32 __mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data)
{
    unsigned long volatile  t_start=get_timer(0);
    u32 volatile  data;

    // make sure previous write operation is complete
    t_start = get_timer(0);
    while(1)
    {
        if (!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31)))
        {
            break;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Write operation is ongoing !!\n");
            return 0;
        }
        udelay(10);
    }
    udelay(1);//make sequencial write more robust

    data = (0x01 << 16) | (1<<18) | (phy_addr << 20) | (phy_register << 25) | write_data;
    outw(MDIO_PHY_CONTROL_0, data);
    data |= (1<<31);
    outw(MDIO_PHY_CONTROL_0, data); //start operation
    //printf("\n Set Command [0x%08X] to PHY 0x%8x!!\n", data, MDIO_PHY_CONTROL_0);

    t_start = get_timer(0);

    // make sure write operation is complete
    while(1)
    {
        if(!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31))) // 0 : Read/write operation complete
        {
            return 1;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Write operation is ongoing and Time Out!!\n");
            return 0;
        }
        udelay(10);
    }
}

u32 mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data)
{

    {
        if(__mii_mgr_read(phy_addr, phy_register, read_data)) {
//          printf("%s: phy_addr=%x phy_register=%x *read_data=%x\n",__FUNCTION__, phy_addr, phy_register, *read_data);
            return 1;
        }
    }

    return 0;
}

u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data)
{
//  printf("%s: phy_addr=%x phy_register=%x write_data=%x\n", __FUNCTION__, phy_addr, phy_register, write_data);
    {
        if(__mii_mgr_write(phy_addr, phy_register, write_data)) {
            return 1;
        }
    }

    return 0;
}


int noe_mii_read(const char *devname, unsigned char phy_addr, unsigned char phy_register, unsigned short *read_data)
{
    u32 regValue;
    if (mii_mgr_read(phy_addr, phy_register, &regValue)) {
        /* mdio read operation is successfully executed */
        *read_data = (unsigned short)regValue;
        return 0;
    }
    else {
        return 1;
    }
}


int noe_mii_write(const char *devname, unsigned char phy_addr, unsigned char phy_register, unsigned short write_data)
{
    if (mii_mgr_write(phy_addr, phy_register, write_data)) {
        /* mdio write operation is successfully executed */
        return 0;
    }
    else {
        return 1;
    }
}


void dump_phy_reg(int port_no, int from, int to, int is_local, int page_no)
{
    u32 i=0;
    u32 temp=0;
    u32 r31=0;

    if(is_local==0) {
        printf("\n\nGlobal Register Page %d\n",page_no);
        printf("===============");
        r31 = 0 << 15; //global
        r31 = page_no&0x7 << 12; //page no
        mii_mgr_write(0, 31, r31); //select global page x
        for(i=16;i<32;i++) {
        if(i%8==0) {
            printf("\n");
        }
        mii_mgr_read(port_no,i, &temp);
        printf("%02d: %04X ",i, temp);
        }
    }else {
        printf("\n\nLocal Register Port %d Page %d\n",port_no, page_no);
        printf("===============");
        r31 = 1 << 15; //local
        r31 = page_no&0x7 << 12; //page no
        mii_mgr_write(0, 31, r31); //select local page x
        for(i=16;i<32;i++) {
        if(i%8==0) {
            printf("\n");
        }
        mii_mgr_read(port_no,i, &temp);
        printf("%02d: %04X ",i, temp);
        }
    }
    printf("\n");
}

#ifndef ON_BOARD_NAND_FLASH_COMPONENT
#define MDIO_DBG_CMD
#endif
int noe_mdio_access(cmd_tbl_t *cmdtp, int flag, int argc,  char * const argv[])
{
    u32 addr;
    u32 phy_addr;
    u32 value = 0,bit_offset,temp;
    u32 i=0;

    if(!memcmp(argv[0],"mdio.anoff",sizeof("mdio.anoff")))
    {
        value = inw(MDIO_PHY_CONTROL_0);
        value |= (1<<15);
        outw(MDIO_PHY_CONTROL_0,value);
        puts("\n GMAC1 Force link status enable !! \n");
    }
    else if(!memcmp(argv[0],"mdio.anon",sizeof("mdio.anon")))
    {
        value = inw(MDIO_PHY_CONTROL_0);
        value &= ~(1 << 15);
        outw(MDIO_PHY_CONTROL_0,value);
        puts("\n GMAC1 Force link status disable !! \n");
    }
    else if(!memcmp(argv[0],"mdio.r",sizeof("mdio.r")))
    {
        if (argc != 3) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
            }
        phy_addr = simple_strtoul(argv[1], NULL, 10);

        if(phy_addr == 31) {
            addr = simple_strtoul(argv[2], NULL, 16);
        } else {
            addr = simple_strtoul(argv[2], NULL, 10);
        }

        phy_addr &=0x1f;

        if(mii_mgr_read(phy_addr, addr, &value))
            printf("\n mdio.r addr[0x%08X]=0x%08X\n",addr,value);
        else {
            printf("\n Read addr[0x%08X] is Fail!!\n",addr);
        }
    }
    else if(!memcmp(argv[0],"mdio.w",sizeof("mdio.w")))
    {
        if (argc != 4) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
            }
        phy_addr = simple_strtoul(argv[1], NULL, 10);

        if(phy_addr == 31) {
            addr = simple_strtoul(argv[2], NULL, 16);
        } else {
            addr = simple_strtoul(argv[2], NULL, 10);
        }

        value = simple_strtoul(argv[3], NULL, 16);
        phy_addr &=0x1f;

        if(mii_mgr_write(phy_addr, addr,value)) {
            printf("\n mdio.w addr[0x%08X]  value[0x%08X]\n",addr,value);
        }
        else {
            printf("\n Write[0x%08X] is Fail!!\n",addr);
        }
    }
    else if(!memcmp(argv[0],"mdio.wb",sizeof("mdio.wb")))
    {
        if (argc != 4) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
        addr = simple_strtoul(argv[1], NULL, 10);
        bit_offset = simple_strtoul(argv[2], NULL, 10);
        value = simple_strtoul(argv[3], NULL, 10);

        if(mii_mgr_read(31, addr,&temp)) {

        }
        else {
            printf("\n Rasd PHY fail while mdio.wb was called\n");
            return 1;
        }

        if(value) {
            printf("\n Set bit[%d] to '1' \n",bit_offset);
            temp |= (1<<bit_offset);
        }
        else {
            printf("\n Set bit[%d] to '0' \n",bit_offset);
            temp &= ~(1<<bit_offset);
        }

        if(mii_mgr_write(31, addr,temp)) {
            printf("\n mdio.wb addr[0x%08X]  value[0x%08X]\n",addr,temp);
        }
        else {
            printf("\n Write[0x%08X] is Fail!!\n",addr);
        }
    }
    else if(!memcmp(argv[0],"mdio.d",sizeof("mdio.d")))
    {
        //RT6855A, RT6855, MT7620, MT7621
        /* SPEC defined Register 0~15
         * Global Register 16~31 for each page
         * Local Register 16~31 for each page
         */
        printf("SPEC defined Register\n");
        printf("===============");
        for(i=0;i<=16;i++) {
            if(i%8==0) {
            printf("\n");
            }
            mii_mgr_read(0,i, &temp);
            printf("%02d: %04X ",i, temp);
        }

        if (argc == 2) {
            addr = simple_strtoul(argv[1], NULL, 10);
            dump_phy_reg(addr, 16, 31, 0, i);
            dump_phy_reg(addr, 16, 31, 1, 0); //dump local page 0
            dump_phy_reg(addr, 16, 31, 1, 1); //dump local page 1
            dump_phy_reg(addr, 16, 31, 1, 2); //dump local page 2
            dump_phy_reg(addr, 16, 31, 1, 3); //dump local page 3
        }else {

            for(i=0;i<4;i++) { //global register  page 0~4
            dump_phy_reg(0, 16, 31, 0, i);
            }

            for(i=0;i<5;i++) { //local register port 0-port4
            dump_phy_reg(i, 16, 31, 1, 0); //dump local page 0
            dump_phy_reg(i, 16, 31, 1, 1); //dump local page 1
            dump_phy_reg(i, 16, 31, 1, 2); //dump local page 2
            dump_phy_reg(i, 16, 31, 1, 3); //dump local page 3
            }
        }
    }
    else if(!memcmp(argv[0],"mdio.s",sizeof("mdio.s")))
    {
#define MSTAR_ADDR(reg)  ((reg)<<1)
#define MSTAR_BANK(bank) (RIU_REG_BASE + (bank << 9))
#define MSTART_REG(bank,reg)  *((volatile unsigned int*)(MSTAR_BANK(bank)+MSTAR_ADDR(reg)))

        if (argc != 2) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
        value = simple_strtoul(argv[1], NULL, 3);

        i = MSTART_REG( 0x121f,0x3c);
        if ( value ==0 )
        {
            printf ("MDIO switch to ChipTop Pad!\n\n");
            i&=~(1<14);
        }
        else
        {
            printf ("MDIO switch to PM Pad!\n\n");
            i|=(1<14);
        }
        MSTART_REG( 0x121f,0x3c) = i;
    }
    return 0;
}

#if 0
U_BOOT_CMD(
    mdio,   4,  1,  noe_mdio_access,
    "mdio   - Ralink PHY register R/W command !!\n",
    "mdio.r [phy_addr(dec)] [reg_addr(dec)] \n"
    "mdio.w [phy_addr(dec)] [reg_addr(dec)] [data(HEX)] \n"
    "mdio.anoff GMAC1 Force link status enable !!  \n"
    "mdio.anon GMAC1 Force link status disable !!  \n"
    "mdio.wb [phy register(dec)] [bit offset(Dec)] [Value(0/1)]  \n"
    "mdio.d - dump all Phy registers \n"
    "mdio.d [phy register(dec)] - dump Phy registers \n"
    "mdio.s [0- ChipTop , 1- PM ] - switch MDIO Pad \n");
#endif
