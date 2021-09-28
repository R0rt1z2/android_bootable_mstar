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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <command.h>
#include <common.h>
#include <linux/ctype.h>
#include <MsTypes.h>
#include <MsUartComm.h>
#include <malloc.h>
#include <stdlib.h>
#include "drvUART.h"
#include <MsIRQ.h>

#ifdef CONFIG_UARTCOMM

#define BUFFER_MAX 50
#define DEFAULT_BAUDRATE 115200

static MS_U32 uart_comm=0;

//use cmd
/*static void uartcomm_cmduse(void)
{
#ifdef CONFIG_UARTCOMM
    run_command("uartcomm init 1 1 115200",0);
    run_command("uartcomm write -s hello uartcomm",0);
    char buffer[60]={0};
    memset(buffer,51,sizeof(buffer));
    char cmd[50]="uartcomm write -x";
    sprintf(cmd,"%s %x %d",cmd,buffer,sizeof(buffer));
    printf("cmd :%s\n",cmd);
    run_command(cmd,0);
#endif
}*/

//add customer flow to manage buffer data.
static void uartcomm_cus(MS_U8 *buffer)
{
   /* if(buffer!= NULL)
    {
        printf("%s",buffer);
    }*/
    return ;
}

static void uartcomm_showusage(void)
{
    printf("uartcomm usage:\n");
    printf("uartcomm init <port> <pad> <baudrate>: init uart port,<port>--1,2,3,4 <pad> 0,1,2,3\n");
    printf("     eg:uartcomm init 1 1 115200 ==init uart_port UART1 with GPIO PAD=PAD_GPIO7,Baudrate=115200\n");
    printf("     for monaco:UART1_PAD:0=not enabled,1=PAD_GPIO7/PAD_GPIO8,2=PAD_GPIO20/PAD_GPIO19,3=PAD_GPIO4/PAD_GPIO3\n");
    printf("uartcomm write <-s> <cmd data> : uart write data <step 1--uartcomm init success,step2--uart write data>\n");
    printf("     -s: write string to uart,default cmd_size <= 30\n");
    printf("uartcomm read  <size>     : uart read data <tep 1--uartcomm init success,step2--uart read data>\n");
}

static char * nstrncpy(char * dest,const char *src,size_t index,size_t count)
{
	char *tmp = dest;
    while(index -- && *src++ != '\0')
        /* nothing */;
	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */;

	return tmp;
}
int do_uartcomm (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
        MS_U8 UartPortNum = 0;
        MS_U8 UartPadNum = 0;
        MS_U32 Baudrate = 0;
        UART_DEVICE_TYPE UartDevice = E_UART_INVALID;
        UART_PORT_TYPE UartPort = E_UART_PORT_INVALID;
        UART_PAD_TYPE UartPad = E_UART_PAD_1_0;
        MS_U8 buffer[BUFFER_MAX]={0};
        MS_U32 addr = 0;
        MS_U8* pdata =NULL;
        MS_U32 size = 0;
        MS_U8 i = 0;

        if(!strcmp(argv[1], "init"))
        {
            if(argc != 5)
            {
                uartcomm_showusage();
                return -1;
            }
            UartPortNum = simple_strtoul(argv[2], NULL, 10);
            UartPadNum = simple_strtoul(argv[3], NULL, 10);
            Baudrate = simple_strtoul(argv[4], NULL, 10);

            switch(UartPortNum)
            {
                case 1: UartPort = E_UART_PORT1;
                        UartPad = UartPadNum + E_UART_PAD_1_0;
                        UartDevice = E_UART_PIU_UART1;
                        break;
                case 2: UartPort = E_UART_PORT2;
                        UartPad = UartPadNum + E_UART_PAD_2_0;
                        UartDevice = E_UART_PIU_UART2;
                        break;
                case 3: UartPort = E_UART_PORT3;
                        UartPad = UartPadNum + E_UART_PAD_3_0;
                        UartDevice = E_UART_PIU_UART1;//uart source choose PIU_UART1 while PIU_UART1 not used
                        break;
                case 4: UartPort = E_UART_PORT4;
                        UartPad = UartPadNum + E_UART_PAD_4_0;
                        UartDevice = E_UART_PIU_UART2;//uart source choose PIU_UART2 while PIU_UART2 not used
                        break;
                default: UartPort = E_UART_PORT_INVALID;
                        printf("UART PORT select error\n");
                        break;
            }

            MDrv_UART_Init(E_UART_PIU_UART1,DEFAULT_BAUDRATE);
            if(UartDevice == E_UART_INVALID || UartPort ==E_UART_PORT_INVALID)
            {
                printf("uartcomm init failed ! uartsource or pad not avaliable.\n");
                return -1;
            }
            mdrv_uart_connect_mux(UartDevice,UartPort,UartPad);
            uart_comm = mdrv_uart_open(UartDevice);

            if (uart_comm)
            {
                mdrv_uart_set_baudrate(uart_comm,Baudrate);
                printf("uartcomm init port %d ok\n",UartPort);
            }
            else
            {
                printf("uartcomm init failed !\n");
                return -1;
            }
            return 0;
        }
        else if(!strcmp(argv[1], "read"))
        {
            if(argc != 3)
            {
                uartcomm_showusage();
                return -1;
            }
            size = simple_strtoul(argv[2], NULL, 10);
            if(uart_comm)
            {
                while(size)
                {
                    memset(buffer,0,BUFFER_MAX);

                    if(size > BUFFER_MAX)
                    {
                        mdrv_uart_read(uart_comm,buffer,BUFFER_MAX);
                        size = size-BUFFER_MAX;
                    }
                    else
                    {
                        mdrv_uart_read(uart_comm,buffer,size);
                        size =0;
                    }

                    uartcomm_cus(buffer);
                }
            }
            else
            {
                printf("please uartcomm init seccessed first\n");
                return -1;
            }
            return 0;
        }
        else if(!strcmp(argv[1],"write"))
        {
            if(argc > CONFIG_SYS_MAXARGS)//uboot cmd args limited
            {
                printf("args is too long!\n");
                return -1;
            }
            if(uart_comm)
            {
                MS_U8 n = 0;
                memset(buffer,0,BUFFER_MAX);
                if(!strcmp(argv[2], "-s"))//string
                {
                    i = 3;
                    while(argv[i])
                    {
                        size = strlen(argv[i]);

                        while(size > BUFFER_MAX)
                        {
                            nstrncpy(buffer,argv[i],n*BUFFER_MAX,BUFFER_MAX);
                            mdrv_uart_write(uart_comm,buffer,BUFFER_MAX);
                            memset(buffer,0,BUFFER_MAX);
                            size = size - BUFFER_MAX;
                            n++;
                        }

                        nstrncpy(buffer,argv[i],n*BUFFER_MAX,size);
                        mdrv_uart_write(uart_comm,buffer,size+1);
                        memset(buffer,0,BUFFER_MAX);
                        i++;
                    }
                }
                else if(!strcmp(argv[2], "-a"))//address
                {
                    if(argc != 5)
                    {
                        uartcomm_showusage();
                        return -1;
                    }

                    addr = simple_strtoul(argv[3], NULL, 16);
                    size = simple_strtoul(argv[4], NULL, 10);
                    pdata = (MS_U8*)addr;

                    while(size>BUFFER_MAX)
                    {
                        memcpy(buffer,pdata+(n*BUFFER_MAX),BUFFER_MAX);
                        mdrv_uart_write(uart_comm,buffer,size);
                        size = size - BUFFER_MAX;
                        n++;
                    }

                    memcpy(buffer,pdata+(n*BUFFER_MAX),size);
                    mdrv_uart_write(uart_comm,buffer,size);
                }
                else
                {
                    while(argv[i])
                    {
                        buffer[i]= simple_strtoul(argv[i], NULL, 10);
                        i++;
                        size++;
                    }

                    mdrv_uart_write(uart_comm,buffer,size);
                }
            }
            else
            {
                printf("please uartcomm init seccessed first\n");
                return -1;
            }
            return 0;
        }
        else if(!strcmp(argv[1],"test"))
        {
            MDrv_UART_Init(E_UART_PIU_UART1,DEFAULT_BAUDRATE);

            uart_comm = mdrv_uart_open(E_UART_PIU_UART1);

            if (uart_comm)
            {
                mdrv_uart_set_baudrate(uart_comm,DEFAULT_BAUDRATE);
            }
            else
            {
                printf("uartcomm open failed !\n");
                return -1;
            }
            mdrv_uart_write(uart_comm,"hello",5);
            mdrv_uart_read(uart_comm,buffer,10);
            printf("uart receive data :%s",buffer);
        }
        else
        {
            uartcomm_showusage();
        }
        return 0;
}
#endif
