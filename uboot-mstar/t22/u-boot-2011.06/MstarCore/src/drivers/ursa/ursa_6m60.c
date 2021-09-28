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

#ifndef __DRV_URSA_6M60_C__
#define __DRV_URSA_6M60_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_6m60.h"
#include <MsSystem.h>

#define UART_CMD_BUF_SIZE    11
#define IS_URAT_SUCCESS_MSG(x)          (((x) & 0xF0) == 0xA0)
#define IS_URAT_SUCCESS_DATA_MSG(x)     ((((x) & 0xF0) == 0xA0) && (((x) & 0x0F) > 2))
#define IS_URAT_ERROR_MSG(x)            (((x) & 0xF0) == 0xE0)
#define IS_VALID_MSG(x)                 ((IS_URAT_SUCCESS_MSG(x) || IS_URAT_ERROR_MSG(x)) && (GET_URAT_MSG_LEN(x) >= 2))
#define GET_URAT_MSG_LEN(x)             ((x) & 0x0F)
#define URSA_CMD_INTERVAL_TIME   26
#define MFC_CMD_DATA_SUM      7

MS_U32  g_uart_fd = 0;
MS_U32 g_uart_read_timeout_count = 10;
MS_U8* m_data = NULL;
MS_U8 g_uart_init_status = 0;
MS_U8 uart_rx_buf[512];
static MS_U32 Data_count = 0;
static MS_U32 Data_Len = 0;
static MS_U8 g_ursa_boot_flag = 0;
static SFlashHeader g_flash_header;
static MS_BOOL g_ursa_fast_upgrade_enable;
static MS_U32 g_isp_src_addr;
static MS_U32 g_isp_code_size;

void RxHandler(int c)
{
    MS_BOOL isValidMsg = FALSE;
    MS_BOOL bRet = FALSE;
    MS_U32 bLen = 0;

    isValidMsg = IS_VALID_MSG(c);
    if((Data_Len != 0) && (Data_count != 0))
    {
        UBOOT_DEBUG("Date Value: 0x%x\n", c);
        //UBOOT_DEBUG("date IN1: Data_Len:%d, Data_count:%d\n", Data_Len,Data_count);
        *(m_data + (Data_Len - Data_count)) = c;
        Data_count--;
        //UBOOT_DEBUG("date IN2: Data_Len:%d, Data_count:%d\n", Data_Len,Data_count);
    }
    else if(isValidMsg == TRUE && Data_count == 0)
    {
        bLen = GET_URAT_MSG_LEN(c);
        UBOOT_DEBUG("bLen: %lx\n",bLen);
        if(m_data != NULL)
        {
            free(m_data);
        }
        m_data = (MS_U8 *)malloc(bLen * sizeof(MS_U8));
        memset(m_data, 0, bLen * sizeof(MS_U8));
        Data_count = bLen -1;
        Data_Len = bLen -1;
        //*(m_data + (Data_count - Data_count)) = c;
        //Data_count--;
    }
}

MS_BOOL InitUrsaCmdData(void)
{
    UART_DEVICE_TYPE uart_device = E_UART_PIU_UART1;
    char *p_uart_type = NULL;
    int uart_type_num = 1;  //default use PIU_UART1
    int ret = -1;

    p_uart_type = getenv("uart_type"); // piu_uart_type ={0, 1, 2}
    if(NULL != p_uart_type)
    {
        uart_type_num = simple_strtoul(p_uart_type, NULL, 10);
        switch(uart_type_num)
        {
            case 0:
                uart_device = E_UART_PIU_UART0;
                break;
            case 1:
                uart_device = E_UART_PIU_UART1;
                break;
            case 2:
                uart_device = E_UART_PIU_UART2;
                break;
            default:
                uart_device = E_UART_PIU_UART1;  //default use PIU_UART1
                break;
        }
        UBOOT_INFO("use E_UART_PIU_UART%d, uart_device=%d\n", uart_type_num, uart_device);
    }

    if(g_uart_init_status)
    {
        UBOOT_DEBUG("InitUrsaCmdData Init Once!\n");
        return FALSE;
    }

    MDrv_UART_Init(uart_device, 115200);

    MDrv_UART_SetIOMapBase();

    g_uart_fd = mdrv_uart_open(uart_device);
    if (g_uart_fd)
    {
        //mdrv_uart_set_baudrate(g_uart_fd ,115200);
        //mdrv_uart_rx_enable(g_uart_fd); //Open rx receive only when sending cmd.
        memset(uart_rx_buf, 0, sizeof(uart_rx_buf));
        if(mdrv_uart_set_rx_buffer(g_uart_fd, uart_rx_buf,sizeof(uart_rx_buf)) != 0)
        {
                UBOOT_ERROR("mdrv_uart_set_rx_buffer failed\n");
        }
        mdrv_uart_set_rx_callback(g_uart_fd, RxHandler);
        UBOOT_DEBUG("uartcomm init uart fd:%d ok\n", g_uart_fd);
    }
    else
    {
        UBOOT_ERROR("uartcomm init failed !\n");
        return FALSE;
    }
    g_uart_init_status = 1;
    return TRUE;
}

MS_U16 Send(MS_U8* cValue, const MS_U16 u16Length)
{
    MS_U16 length = 0;
    if (g_uart_fd  == 0)
    {
        UBOOT_ERROR("Please connect before send command\n");
        return -1;
    }

    length = mdrv_uart_write(g_uart_fd, cValue, u16Length);
    return length;
}

MS_U32 Read(MS_U8* cValue, const MS_U32 u32Length)
{
    int length = 0;
    if (g_uart_fd == 0)
    {
        UBOOT_ERROR("Please connect before read command\n");
        return -1;
    }

    length = mdrv_uart_read(g_uart_fd, cValue, u32Length);
    return length;
}

void combineDDC2UART(MS_U8 *u8UartBuf, MS_U8 *u8DDCBuf)
{
    u8UartBuf[0]= 0xE0;
    u8UartBuf[1]= UART_CMD_BUF_SIZE; //Length 11
    u8UartBuf[2]= 0x40; //separate-tv ,Basic DDC CMD
    u8UartBuf[3] = u8DDCBuf[0];
    u8UartBuf[4] = u8DDCBuf[1];
    u8UartBuf[5] = u8DDCBuf[2];
    u8UartBuf[6] = u8DDCBuf[3];
    u8UartBuf[7] = u8DDCBuf[4];
    u8UartBuf[8] = u8DDCBuf[5];
    u8UartBuf[9] = u8DDCBuf[6];
    u8UartBuf[10] = 0x0;

    calUartCheckSum(u8UartBuf,UART_CMD_BUF_SIZE);
}

void calUartCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen )
{
    MS_U16 u16CheckSum =0;
    MS_U8 bufSize = u8BufLen;
    u8BufLen--;
    while(u8BufLen > 0)
    {
        -- u8BufLen;
        u16CheckSum += pBuf[u8BufLen] & 0xFF;
    }
    u16CheckSum &= 0xFF;
    pBuf[bufSize-1] = 0xFF - u16CheckSum;
}

MS_BOOL checkUartCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen )
{
    MS_U16 u16CheckSum =0;
    MS_U8 tLen = u8BufLen;
    u8BufLen--;

    while(u8BufLen > 0)
    {
        --u8BufLen;
        u16CheckSum += pBuf[u8BufLen] & 0xFF;
    }
    u16CheckSum &= 0x00FF;
    u16CheckSum = (0xFF - u16CheckSum);
    if(pBuf[tLen-1] == u16CheckSum)
    {
        return TRUE;
    }
    UBOOT_ERROR("Uart CheckSum is incorrect, %x!=%x\n",u16CheckSum,pBuf[tLen-1]);
    return FALSE;
}

void CalCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen)
{
    MS_U8 u8CheckSum =0;

    while( u8BufLen > 1 )//calculate pBuf[1]~pBuf[End] do not count pBuf[0],it is checksum
    {
        -- u8BufLen;
        u8CheckSum += pBuf[u8BufLen] & 0x0F;
    }
    u8CheckSum &= 0x0F;
    pBuf[0] &= 0x0F;
    pBuf[0] |= ((u8CheckSum )<<4);
}

MS_BOOL Cmd_DataWrite(MS_U8 *u8Buf)
{
        MS_U8 u8DataBuf[UART_CMD_BUF_SIZE] = {0};
        int bLen= 0;
        MS_U32 timeout1 = 0;
        MS_U32 timeout2 = 0;
        MS_BOOL ret = FALSE;

        combineDDC2UART(u8DataBuf,u8Buf);
        UBOOT_INFO(">>CMD: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",u8DataBuf[0], u8DataBuf[1],u8DataBuf[2],u8DataBuf[3],u8DataBuf[4],u8DataBuf[5],u8DataBuf[6],
    u8DataBuf[7] ,u8DataBuf[8],u8DataBuf[9],u8DataBuf[10]);
		mdrv_uart_rx_enable(g_uart_fd);//Enable RX Call_back When Send CMD
        do
       {
            bLen = Send(u8DataBuf,UART_CMD_BUF_SIZE);

            if(bLen == UART_CMD_BUF_SIZE)
            {
                ret = TRUE;
                timeout1 = 0;
                UBOOT_DEBUG("Send success,receiving....\n");

                //don't need readMsgAndWaitCount, using callback To Read MSG
                do
                {
                     //Waiting for RX Call back Get Datalen
                     MsOS_DelayTask(10);
                     //UBOOT_DEBUG("Send success,receiving Fail retry: %d\n", timeout1);
                     //UBOOT_DEBUG("date IN: Data_Len:%d, Data_count:%d\n", Data_Len,Data_count);
                }
                while((Data_Len == 0 || Data_count != 0) && (timeout1 ++ < g_uart_read_timeout_count));

                if(Data_Len != 0)
                {
                    UBOOT_DEBUG("Send success,received Date len:%d \n", Data_Len);
                    Data_Len = 0;
                }
                else
                {
                    UBOOT_ERROR("Send success,received Failed\n");
                    ret = FALSE;
                }
            }
            else
            {
                UBOOT_ERROR("Send fail, please check!!!\n");
                ret = FALSE;
            }
            if(ret == FALSE)
            {
                UBOOT_ERROR("retry %d....\n",timeout2+1);
            }
        }
        while((ret != TRUE) && (timeout2++<3));

        g_uart_read_timeout_count = 10; // reset uart read timeout to 100ms
        mdrv_uart_rx_disable(g_uart_fd);//Disable RX Call_back When Get MSG Done
        return ret;
}

MS_BOOL Cmd_DataRead(MS_U8 *u8Buf, MS_U16 u16Size)
{
    MsOS_DelayTask(50);
    MS_BOOL bRet = FALSE;

    if (NULL != m_data)
    {
        if (u16Size == 0)
            memcpy(u8Buf, m_data, 1);
        else
            memcpy(u8Buf, m_data, u16Size);
        free(m_data);
        m_data = NULL;
        bRet = TRUE;
        UBOOT_DEBUG("Cmd_DataRead Success\n");
    }
    else
    {
         UBOOT_ERROR("Cmd_DataRead Failed\n");
    }
    return bRet;
}

MS_U32 GetUrsaVersionInfo(void)
{
    MS_U8 major = 0xFF;
    MS_U8 minor = 0xFF;
    MS_U8 minor2 = 0xFF;
    MS_U8 u8Buff[MFC_CMD_DATA_SUM] = {0x03, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
    MS_BOOL bRet =  FALSE;

    u8Buff[2] = 0;
    CalCheckSum(u8Buff, MFC_CMD_DATA_SUM);
    bRet = Cmd_DataWrite(u8Buff);
    MsOS_DelayTask(URSA_CMD_INTERVAL_TIME);
    bRet = Cmd_DataRead(&major, 0);

    u8Buff[2] = 1;
    CalCheckSum(u8Buff, MFC_CMD_DATA_SUM);
    bRet = Cmd_DataWrite(u8Buff);
    MsOS_DelayTask(URSA_CMD_INTERVAL_TIME);
    bRet = Cmd_DataRead(&minor, 0);

    u8Buff[2] = 5;
    CalCheckSum(u8Buff, MFC_CMD_DATA_SUM);
    bRet = Cmd_DataWrite(u8Buff);
    MsOS_DelayTask(URSA_CMD_INTERVAL_TIME);
    bRet = Cmd_DataRead(&minor2, 0);

    UBOOT_INFO("6M60 GetVersion major:%d minor:%d minor2:%d bRet=%d\n", major, minor, minor2, bRet);

    return ( MS_U16)(((MS_U16)major << 8 ) | minor);
}

int do_GetUrsaVersionInfo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    InitUrsaCmdData();
    GetUrsaVersionInfo();
    return 0;
}

int do_Change_Uart_connect_port2 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL ret = FALSE;
    MDrv_UART_SetIOMapBase();
    //Whaley Board use RX2/TX2 Uart.
    //MStar 095D board init at mboot/.../mhal_sys.c   use RX5/TX5
    //6M60 uart Link to Soft E_UART_PIU_UART1
    ret = mdrv_uart_connect(E_UART_PORT2, E_UART_PIU_UART1);
    if (ret < 0)
    {
        UBOOT_ERROR("device_ursa:: E_UART_PORT2, UARTInit control interface Fail\n");
        return -1;
    }
    return 0;
}

MS_BOOL set_sw_upgrade_init()
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x20};

    if (FALSE == g_ursa_fast_upgrade_enable)
    {
        return FALSE;
    }

    cmd[2] = 0x0;   // All bank
    cmd[4] = g_isp_code_size;
    cmd[5] = g_isp_code_size >> 8;
    cmd[6] = g_isp_code_size >> 16;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    return Cmd_DataWrite(cmd);
}

MS_BOOL set_sw_upgrade_write_flash()
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x21};

    if (FALSE == g_ursa_fast_upgrade_enable)
    {
        return FALSE;
    }
    g_uart_read_timeout_count = 2000;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    return Cmd_DataWrite(cmd);;
}

MS_BOOL get_sw_upgrade_crc32(int type)
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x22};
    MS_U32 retry_time  =0;
    MS_U8 status = 0;

    if (FALSE == g_ursa_fast_upgrade_enable)
    {
        return FALSE;
    }
    cmd[2] = (MS_U8)type;  //0x0 : Flash,  0x1: Dram

    g_uart_read_timeout_count = 1000;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);
    Cmd_DataWrite(cmd);
    Cmd_DataRead(&status, 1);

    UBOOT_INFO("upgrade %s crc status:0x%x\n", type ? "Dram" : "Flash", status);
    if (0x5A == status)
    {
        return TRUE;
    }

    return FALSE;
}

static MS_U32 cal_terra2_bank_checksum(MS_U8* buf, MS_U32 buf_len)
{
    MS_U32 check_sum = 0;
    MS_U32 i = 0;

    for (i = 0; i < buf_len; i++)
    {
        check_sum += (MS_U32)buf[i];
    }
    return check_sum;
}

static MS_BOOL terra2_write_onebank(MS_U8 banknum, MS_U8* data_buf, MS_U16 len)
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00};

    //bank num
    cmd[2] = banknum;

    //data size
    cmd[3] = (MS_U8)len;       //Data Size[7:0]
    cmd[4] = (MS_U8)(len>>8);  //Data Size[15:8]

    CalCheckSum(cmd, MFC_CMD_DATA_SUM);
    if (FALSE == Cmd_DataWrite(cmd))
    {
        UBOOT_INFO("terra2_write_onebank()- write cmd failed! \n");
        return FALSE;
    }

    //write 32k data by uart
    if (len != mdrv_uart_write(g_uart_fd, data_buf, len))
    {
        UBOOT_INFO("terra2_write_onebank()- write data failed! \n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL get_bank_checksum(uint32_t* bank_checksum)
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00};

    CalCheckSum(cmd, MFC_CMD_DATA_SUM);
    if (FALSE == Cmd_DataWrite(cmd))
    {
        UBOOT_INFO("get_bank_checksum()- write fail\n");
        return FALSE;
    }

    MS_U8 data[4] = {0};
    if (FALSE == Cmd_DataRead(data, sizeof(data)))
    {
        UBOOT_INFO("get_bank_checksum()- read fail\n");
        return FALSE;
    }

    *bank_checksum = ((uint32_t)data[3] << 16) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    return TRUE;
}

MS_BOOL terra2_uart_burst_write(void)
{
    MS_U16 rest_data = 0;
    MS_U8 * data_buf = NULL;
    MS_U32 checksum_cal, checksum_read;
    MS_U32 i = 0, count = 0;
    MS_BOOL ret = TRUE;

    if (g_isp_code_size % TERRA2_BANK_SIZE == 0)
    {
        count = g_isp_code_size/TERRA2_BANK_SIZE;
    }
    else
    {
        count = (g_isp_code_size/TERRA2_BANK_SIZE) + 1;
        rest_data = g_isp_code_size - ((count-1)*TERRA2_BANK_SIZE);
    }

    data_buf = malloc(TERRA2_BANK_SIZE);
    if (NULL == data_buf)
    {
        UBOOT_INFO("terra2_uart_burst_write()- malloc failed! \n");
        return FALSE;
    }

    for (i = 0; i < count; i++)
    {
        memset(data_buf, 0, TERRA2_BANK_SIZE);

        //write one bank data
        if ((rest_data > 0) && (i == (count-1)))
        {
            memcpy(data_buf, (void *)(g_isp_src_addr + (MS_U32)i*TERRA2_BANK_SIZE), rest_data);
            checksum_cal = cal_terra2_bank_checksum(data_buf, rest_data);
            if (FALSE == terra2_write_onebank(i, data_buf, rest_data))
            {
                ret = FALSE;
                goto BurstWriteEnd;
            }

        }
        else
        {
            memcpy(data_buf, (void *)(g_isp_src_addr + (MS_U32)i*TERRA2_BANK_SIZE), TERRA2_BANK_SIZE);
            checksum_cal = cal_terra2_bank_checksum(data_buf, TERRA2_BANK_SIZE);
            if (FALSE == terra2_write_onebank(i, data_buf, TERRA2_BANK_SIZE))
            {
                ret = FALSE;
                goto BurstWriteEnd;
            }
        }

        //Check checksum of a bank data
        UBOOT_INFO("Check checksum of a bank data , i=%d \n", i);
        if (FALSE == get_bank_checksum(&checksum_read))
        {
            ret = FALSE;
            goto BurstWriteEnd;
        }
        if(checksum_read != checksum_cal)
        {
            UBOOT_INFO("bank %d checksum fail, checksum_cal = 0x%08x, checksum_read = 0x%08x \n", i, checksum_cal, checksum_read);
            ret = FALSE;
            goto BurstWriteEnd;
        }
    }
    UBOOT_INFO("\n terra2_uart_burst_write()- OK!\n");

BurstWriteEnd:
    free(data_buf);
    return ret;
}

MS_BOOL get_sw_upgrade_spi_header(void)
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x1f};
    MS_U8 data[4] = {0};
    SFlashHeader * spiHeader = &g_flash_header;

    // retrieve iff double bin enable
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_DUAL_BIN_ENABLE;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        spiHeader->is_double_bin_enable = data[0];
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_DUAL_BIN_ENABLE failed\n");
        return FALSE;
    }

    // retrieve  AP1 start addr
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_SU_AP1_ADDR;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        spiHeader->first_bin_start_addr = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_SU_AP1_ADDR failed\n");
        return FALSE;
    }

    // retrieve  AP2 start addr
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_SU_AP2_ADDR;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        spiHeader->second_bin_start_addr = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_SU_AP2_ADDR failed,\n");
        return FALSE;
    }

    // retrieve  boot flag addr
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG_ADDR;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        spiHeader->boot_flag_addr = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG_ADDR failed,\n");
        return FALSE;
    }

    // retrieve  boot flag
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        g_ursa_boot_flag = data[0];
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG failed,\n");
        return FALSE;
    }

    // retrieve  flash size
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_FLASH_SIZE;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        spiHeader->flash_size = data[0];
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_FLASH_SIZE failed,\n");
        return FALSE;
    }

    // retrieve  if fast upgrade enable
    memset(data, 0, sizeof(data));
    cmd[2] = SPI_HEADER_DDC_TYPE_FAST_UPGRADE_ENABLE;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);

    if (TRUE == Cmd_DataWrite(cmd))
    {
        if (FALSE  == Cmd_DataRead(data, sizeof(data)))
        {
            return FALSE;
        }
        g_ursa_fast_upgrade_enable = data[0];
    }
    else
    {
        UBOOT_INFO("SPI_HEADER_DDC_TYPE_FAST_UPGRADE_ENABLE failed,\n");
        return FALSE;
    }

    UBOOT_INFO("SPI Header information, flashSize(0x%x), firstBinStartAddr(0x%x), secBinStartAddr(0X%x), bootFlagAddr(0x%x), is_double_bin_enable(%s)\n", \
            spiHeader->flash_size, spiHeader->first_bin_start_addr, spiHeader->second_bin_start_addr, \
            g_flash_header.boot_flag_addr, spiHeader->is_double_bin_enable?"TRUE":"FALSE");
    UBOOT_INFO("g_ursa_boot_flag = 0x%x\n", g_ursa_boot_flag);
    UBOOT_INFO("Ursa Header information, g_ursa_fast_upgrade_enable(0x%x) \n",g_ursa_fast_upgrade_enable);

    if (FALSE == g_ursa_fast_upgrade_enable)
    {
        return FALSE;
    }

    return TRUE;
}

void Terra2_Upgrade_Init(MS_U32 u32Addr, MS_U32 u32Size)
{
    g_isp_src_addr = u32Addr; //?
    g_isp_code_size = u32Size;

    InitUrsaCmdData();
}

int getUpgradeBinFromUsb(MS_U32* pAddr, MS_U32* pFileSize)
{
    char cmd[CMD_BUF] = "\0";
    MS_U32 u32Addr = 0;
    int device = 0;
    int partition = 0;
    MS_U32 u32FileSize = 0;
    char *env =NULL;
    int ret = -1;

    UBOOT_TRACE("IN\n");

    if (-1 == appInitUsbDisk())
    {
        UBOOT_ERROR("FAIL : can not init usb!! \n");
        return -1;
    }
    ret = check_file_partition(TERRA2_UPGRADE_BIN_PATH,&device,&partition);
    if(ret != 0)
    {
        UBOOT_ERROR("no %s in usb disk\n", TERRA2_UPGRADE_BIN_PATH);
        return -1;
    }
    else
    {
        UBOOT_DEBUG("has terra2 upgrade bin\n");

        UBOOT_DEBUG("Geting file size\n");
        snprintf(cmd, CMD_BUF, "fatfilesize usb %d:%d %s", device,partition, TERRA2_UPGRADE_BIN_PATH);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
           UBOOT_ERROR("get script file's size fail\n");
           return -1;
        }

        env=getenv("filesize");
        if(env==NULL)
        {
           UBOOT_ERROR("get env 'filesize' fail\n");
        }
        *pFileSize = simple_strtol(env, NULL, 16);
        UBOOT_INFO("Terra2 File Size 0x%lx \n", *pFileSize);

        snprintf(cmd, CMD_BUF, "fatload usb %d:%d  %X %s %ul",device, partition, UPGRADE_BUFFER_ADDR, TERRA2_UPGRADE_BIN_PATH, *pFileSize);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
           UBOOT_ERROR("Load Upgrade File fail!\n");
           return -1;
        }

        UBOOT_INFO("Load upgrade file OK!~\n");
        *pAddr = UPGRADE_BUFFER_ADDR;

    }

    return 0;
}

int checkTerra2Bin(MS_U32 u32Addr, MS_U32 u32FileSize)
{
    const char magic_buf[4] = {0x78, 0x56, 0xAA, 0x55};
    MS_U32 compress_len = 0;
    MS_U8 buf[4] = {0};
    //MS_U8 ver_buf[6] = {0};
    MS_U32 offset = 0;
    MS_U32 i = 0;

    for (offset = 0; offset < u32FileSize ; offset++)
    {
        memset(buf, 0, sizeof(buf));
        memcpy(buf, (void*)(u32Addr + offset), sizeof(buf));
        if (strncmp(magic_buf, (char *)buf, sizeof(buf)) == 0)
        {
            //get compress_len
            memset(buf, 0, sizeof(buf));
            memcpy(buf, (void*)(u32Addr + offset + URSA_COMPRESS_LENGTH_OFFSET), sizeof(buf));
            compress_len = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);

            UBOOT_INFO("compress_len = 0x%x, offset = 0x%x\n", compress_len, offset);
            if (offset == compress_len && compress_len != 0)
                return 0;
            else
                return -1;
        }
    }
    return -1;
}

MS_BOOL is_support_get_spi_header_by_ddc(void)
{
    MS_U8 cmd[MFC_CMD_DATA_SUM] = {0x05, 0x1f};

    cmd[2] = SPI_HEADER_DDC_TYPE_SUPPORT;
    CalCheckSum(cmd, MFC_CMD_DATA_SUM);
    if (FALSE == Cmd_DataWrite(cmd))
    {
        return FALSE;
    }

    MS_U8 data[4] = {0};
    if (FALSE == Cmd_DataRead(data, sizeof(data)))
    {
        return FALSE;
    }
    // magic number to check iff ursa support DDC cmds to retrieve spi header
    if ((0xbc != data[0]) ||
        (0x3a != data[1]) ||
        (0x12 != data[2]) ||
        (0x00 != data[3]))
    {
        UBOOT_INFO("ursa don't support retrieve spi header by DDC commands.");
        return FALSE;
    }
    return TRUE;
}


U_BOOT_CMD(getversion, CONFIG_SYS_MAXARGS, 1, do_GetUrsaVersionInfo,
    "6M60 Get Version\n",    NULL);

U_BOOT_CMD(whaley_6m60_uart, CONFIG_SYS_MAXARGS, 1, do_Change_Uart_connect_port2,
    "change 6M60 Connect to E_UART_PORT2\n",    NULL);

#endif //__DRV_URSA_6M60_C__
