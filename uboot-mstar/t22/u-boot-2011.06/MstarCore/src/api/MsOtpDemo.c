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
#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <MsTypes.h>
#include <linux/string.h>
#if defined (CONFIG_OTP_READ_WRITE_DEMO)
#include "drvCA.h"
#endif

#define OTP_RESERVED_ONE_PART_LEN       (32)

#define CHINA_NET_SERIAL_NO_DATA_LEN    (32)
#define CHINA_NET_SN_DATA_LEN           (24)
#define CHINA_NET_MAC_DATA_LEN          (12)

#define CHINA_NET_SN_START_ADDR         (OTP_RESERVED_ONE_PART_LEN)
#define CHINA_NET_MAC_START_ADDR        (OTP_RESERVED_ONE_PART_LEN*2)

#define CHINA_NET_SERIAL_NO_ENV_NAME    "serialno"
#define CHINA_NET_SN_ENV_NAME           "sn"
#define CHINA_NET_MAC_ENV_NAME          "macaddr"

/*
 * MSO9385 has 128 reserved bytes for user use, we divide it into 4 parts.
 * |---------------------------128 bytes---------------------------|
 * |---32 bytes----|---32 bytes----|---32 bytes----|---32 bytes----|
 * |-------0-------|-------1-------|-------2-------|-------3-------|
 * |---serialno----|------sn-------|----macaddr----|---Reserved----|
 */

int do_writeserialno(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL ret = FALSE;
#if defined (CONFIG_OTP_READ_WRITE_DEMO)
    int i = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        cmd_usage(cmdtp);
    }
    else
    {
        MS_U8 u8ReadOtpSnData[CHINA_NET_SERIAL_NO_DATA_LEN] = {0};
        MS_U8 u8SnData[CHINA_NET_SERIAL_NO_DATA_LEN+1] = {0};

        MDrv_CA_Init();

        strncpy((char*)u8SnData, argv[1], CHINA_NET_SERIAL_NO_DATA_LEN);
        if(strlen(u8SnData) == CHINA_NET_SERIAL_NO_DATA_LEN)
        {
            // check OTP data first
            MDrv_CA_OTP_ReadReserved(u8ReadOtpSnData, CHINA_NET_SERIAL_NO_DATA_LEN);
            for(i=0; i<CHINA_NET_SERIAL_NO_DATA_LEN; i++)
            {
                // judge whether OTP is empty
                if(u8ReadOtpSnData[i] != 0)
                {
                    UBOOT_ERROR("OTP data is not empty, write serialno failed!\n");
                    return ret;
                }
            }

            // convert to int
            for(i=0; i<CHINA_NET_SERIAL_NO_DATA_LEN; i++)
            {
                if(u8SnData[i] >= '0' && u8SnData[i] <= '9')
                {
                    u8SnData[i] = u8SnData[i] - '0';
                }
                else if(u8SnData[i] >= 'A' && u8SnData[i] <= 'F')
                {
                    u8SnData[i] = u8SnData[i] - 'A' + 0x0A;
                }
                else if(u8SnData[i] >= 'a' && u8SnData[i] <= 'f')
                {
                    u8SnData[i] = u8SnData[i] - 'a' + 0x0A;
                }
                else
                {
                    printf("\r\n");
                    UBOOT_ERROR("OTP data format not match!\n");
                    return ret;
                }
                printf("0x%02X ", u8SnData[i]);
            }
            printf("\r\n");
            ret = MDrv_CA_OTP_WriteReserved(u8SnData, CHINA_NET_SERIAL_NO_DATA_LEN);
        }
        else
        {
            UBOOT_ERROR("OTP data length not match %d bytes!\n", CHINA_NET_SERIAL_NO_DATA_LEN);
        }

        if(!ret)
        {
            UBOOT_ERROR("write OTP data failed!\n");
        }
    }

    UBOOT_TRACE("OK\n");
#endif
    return ret;
}

int do_writesn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL ret = FALSE;
#if defined (CONFIG_OTP_READ_WRITE_DEMO)
    int i = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        cmd_usage(cmdtp);
    }
    else
    {
        MS_U8 u8ReadOtpData[OTP_RESERVED_ONE_PART_LEN*2] = {0};
        MS_U8 u8SnData[CHINA_NET_SN_DATA_LEN+1] = {0};

        MDrv_CA_Init();

        strncpy((char*)u8SnData, argv[1], CHINA_NET_SN_DATA_LEN);
        if(strlen(u8SnData) == CHINA_NET_SN_DATA_LEN)
        {
            // check OTP data first
            MDrv_CA_OTP_ReadReserved(u8ReadOtpData, sizeof(u8ReadOtpData));
            for(i=CHINA_NET_SN_START_ADDR; i<CHINA_NET_SN_START_ADDR+CHINA_NET_SN_DATA_LEN; i++)
            {
                // judge whether OTP is empty
                if(u8ReadOtpData[i] != 0)
                {
                    UBOOT_ERROR("OTP data is not empty, write sn failed!\n");
                    return ret;
                }
            }

            // convert to int
            for(i=0; i<CHINA_NET_SN_DATA_LEN; i++)
            {
                if(u8SnData[i] >= '0' && u8SnData[i] <= '9')
                {
                    u8SnData[i] = u8SnData[i] - '0';
                }
                else if(u8SnData[i] >= 'A' && u8SnData[i] <= 'F')
                {
                    u8SnData[i] = u8SnData[i] - 'A' + 0x0A;
                }
                else if(u8SnData[i] >= 'a' && u8SnData[i] <= 'f')
                {
                    u8SnData[i] = u8SnData[i] - 'a' + 0x0A;
                }
                else
                {
                    printf("\r\n");
                    UBOOT_ERROR("OTP data format not match!\n");
                    return ret;
                }
                printf("0x%02X ", u8SnData[i]);
            }
            printf("\r\n");
            memset(u8ReadOtpData, 0, sizeof(u8ReadOtpData));
            memcpy(u8ReadOtpData+CHINA_NET_SN_START_ADDR, u8SnData, CHINA_NET_SN_DATA_LEN);
            ret = MDrv_CA_OTP_WriteReserved(u8ReadOtpData, CHINA_NET_SN_START_ADDR+CHINA_NET_SN_DATA_LEN);
        }
        else
        {
            UBOOT_ERROR("OTP data length not match %d bytes!\n", CHINA_NET_SN_DATA_LEN);
        }

        if(!ret)
        {
            UBOOT_ERROR("write OTP data failed!\n");
        }
    }

    UBOOT_TRACE("OK\n");
#endif
    return ret;
}

int do_writemac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL ret = FALSE;
#if defined (CONFIG_OTP_READ_WRITE_DEMO)
    int i = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        cmd_usage(cmdtp);
    }
    else
    {
        MS_U8 u8ReadOtpData[OTP_RESERVED_ONE_PART_LEN*3] = {0};
        MS_U8 u8SnData[CHINA_NET_MAC_DATA_LEN+1] = {0};

        MDrv_CA_Init();

        strncpy((char*)u8SnData, argv[1], CHINA_NET_MAC_DATA_LEN);
        if(strlen(u8SnData) == CHINA_NET_MAC_DATA_LEN)
        {
            // check OTP data first
            MDrv_CA_OTP_ReadReserved(u8ReadOtpData, sizeof(u8ReadOtpData));
            for(i=CHINA_NET_MAC_START_ADDR; i<CHINA_NET_MAC_START_ADDR+CHINA_NET_MAC_DATA_LEN; i++)
            {
                // judge whether OTP is empty
                if(u8ReadOtpData[i] != 0)
                {
                    UBOOT_ERROR("OTP data is not empty, write macaddr failed!\n");
                    return ret;
                }
            }

            // convert to int
            for(i=0; i<CHINA_NET_MAC_DATA_LEN; i++)
            {
                if(u8SnData[i] >= '0' && u8SnData[i] <= '9')
                {
                    u8SnData[i] = u8SnData[i] - '0';
                }
                else if(u8SnData[i] >= 'A' && u8SnData[i] <= 'F')
                {
                    u8SnData[i] = u8SnData[i] - 'A' + 0x0A;
                }
                else if(u8SnData[i] >= 'a' && u8SnData[i] <= 'f')
                {
                    u8SnData[i] = u8SnData[i] - 'a' + 0x0A;
                }
                else
                {
                    printf("\r\n");
                    UBOOT_ERROR("OTP data format not match!\n");
                    return ret;
                }
                printf("0x%02X ", u8SnData[i]);
            }
            printf("\r\n");
            memset(u8ReadOtpData, 0, sizeof(u8ReadOtpData));
            memcpy(u8ReadOtpData+CHINA_NET_MAC_START_ADDR, u8SnData, CHINA_NET_MAC_DATA_LEN);
            ret = MDrv_CA_OTP_WriteReserved(u8ReadOtpData, CHINA_NET_MAC_START_ADDR+CHINA_NET_MAC_DATA_LEN);
        }
        else
        {
            UBOOT_ERROR("OTP data length not match %d bytes!\n", CHINA_NET_MAC_DATA_LEN);
        }

        if(!ret)
        {
            UBOOT_ERROR("write OTP data failed!\n");
        }
    }

    UBOOT_TRACE("OK\n");
#endif
    return ret;
}

int do_checksn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = FALSE;
#if defined (CONFIG_OTP_READ_WRITE_DEMO)
    int i = 0, j = 0;
    UBOOT_TRACE("IN\n");
    if (argc != 1)
    {
        cmd_usage(cmdtp);
    }
    else
    {
        // 11:22:33:44:55:66 = 17bytes
        MS_U8 MacEnv[CHINA_NET_MAC_DATA_LEN+6] = {0};
        MS_U8 MacRaw[CHINA_NET_MAC_DATA_LEN] = {0};
        MS_U8 SerialnoEnv[CHINA_NET_SERIAL_NO_DATA_LEN+1] = {0};
        MS_U8 SnEnv[CHINA_NET_SN_DATA_LEN+1] = {0};
        MS_U8 u8ReadOtpData[OTP_RESERVED_ONE_PART_LEN*3] = {0};
        MS_U32 u32Sum = 0;
        char *getSerialnoEnv = NULL;
        char *getSnEnv = NULL;
        char *getMacEnv = NULL;

        // 1. read OTP data
        MDrv_CA_Init();
        MDrv_CA_OTP_ReadReserved(u8ReadOtpData, sizeof(u8ReadOtpData));

        // 2. serialno data
        memcpy(SerialnoEnv, u8ReadOtpData, CHINA_NET_SERIAL_NO_DATA_LEN);
        // convert to char
        for(i=0; i<CHINA_NET_SERIAL_NO_DATA_LEN; i++)
        {
            // judge whether the SerialnoEnv is empty
            u32Sum += SerialnoEnv[i];

            printf("0x%02X ", SerialnoEnv[i]);
            if(SerialnoEnv[i] >= 0 && SerialnoEnv[i] <= 9)
            {
                SerialnoEnv[i] = SerialnoEnv[i] + '0';
            }
            else
            {
                SerialnoEnv[i] = SerialnoEnv[i] - 0x0A + 'A';
            }
        }
        SerialnoEnv[i] = 0;
        printf("\r\n");

        if(u32Sum == 0)
        {
            UBOOT_ERROR("OTP serialno data is empty, please use \"writeserialno\" to write serialno first!\n");
            //return ret;
        }
        else
        {
            UBOOT_INFO("read from OTP: serialno = %s\n", SerialnoEnv);
        }

        // 3. sn data
        u32Sum = 0;
        memcpy(SnEnv, u8ReadOtpData+CHINA_NET_SN_START_ADDR, CHINA_NET_SN_DATA_LEN);
        // convert to char
        for(i=0; i<CHINA_NET_SN_DATA_LEN; i++)
        {
            // judge whether the SnEnv is empty
            u32Sum += SnEnv[i];

            printf("0x%02X ", SnEnv[i]);
            if(SnEnv[i] >= 0 && SnEnv[i] <= 9)
            {
                SnEnv[i] = SnEnv[i] + '0';
            }
            else
            {
                SnEnv[i] = SnEnv[i] - 0x0A + 'A';
            }
        }
        SnEnv[i] = 0;
        printf("\r\n");

        if(u32Sum == 0)
        {
            UBOOT_ERROR("OTP sn data is empty, please use \"writesn\" to write sn first!\n");
            //return ret;
        }
        else
        {
            UBOOT_INFO("read from OTP: sn = %s\n", SnEnv);
        }

        // 4. mac data
        u32Sum = 0;
        memcpy(MacRaw, u8ReadOtpData+CHINA_NET_MAC_START_ADDR, CHINA_NET_MAC_DATA_LEN);
        // convert to char
        for(i=0; i<CHINA_NET_MAC_DATA_LEN; i++)
        {
            // judge whether the MacRaw is empty
            u32Sum += MacRaw[i];

            printf("0x%02X ", MacRaw[i]);
            if(MacRaw[i] >= 0 && MacRaw[i] <= 9)
            {
                MacRaw[i] = MacRaw[i] + '0';
            }
            else
            {
                MacRaw[i] = MacRaw[i] - 0x0A + 'A';
            }
        }
        printf("\r\n");

        if(u32Sum == 0)
        {
            UBOOT_ERROR("OTP mac data is empty, please use \"writemac\" to write mac first!\n");
            return ret;
        }
        else
        {
            // parse mac
            for(i=0,j=0; i<CHINA_NET_MAC_DATA_LEN; i++,j++)
            {
                if((j % 3) == 2)
                {
                    // mac format: 11:22:33:44:55:66
                    MacEnv[j] = ':';
                    j++;
                }
                MacEnv[j] = MacRaw[i];
            }
            UBOOT_INFO("read from OTP: macaddr = %s\n", MacEnv);
        }

        // get exists env
        getSerialnoEnv = getenv(CHINA_NET_SERIAL_NO_ENV_NAME);
        getSnEnv = getenv(CHINA_NET_SN_ENV_NAME);
        getMacEnv = getenv(CHINA_NET_MAC_ENV_NAME);
        UBOOT_DEBUG("get serialno env = %s\n", getSerialnoEnv);
        UBOOT_DEBUG("get sn       env = %s\n", getSnEnv);
        UBOOT_DEBUG("get macaddr  env = %s\n", getMacEnv);

        if(strncmp(MacEnv, getMacEnv, strlen(MacEnv)) != 0)
        {
            // 5. set macaddr env
            setenv(CHINA_NET_MAC_ENV_NAME, MacEnv);
            saveenv();
            UBOOT_INFO("use OTP data to set macaddr env instead!\n");
        }
        if(strncmp(SnEnv, getSnEnv, strlen(SnEnv)) != 0)
        {
            // 6. set sn env
            setenv(CHINA_NET_SN_ENV_NAME, SnEnv);
            saveenv();
            UBOOT_INFO("use OTP data to set sn env instead!\n");
        }
        if(strncmp(SerialnoEnv, getSerialnoEnv, strlen(SerialnoEnv)) != 0)
        {
            // 7. set serialno env
            setenv(CHINA_NET_SERIAL_NO_ENV_NAME, SerialnoEnv);
            saveenv();
            UBOOT_INFO("use OTP data to set serialno env instead!\n");
        }
    }

    UBOOT_TRACE("OK\n");
#endif
    return ret;
}

int do_readrsakey(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
#if defined (CONFIG_OTP_READ_WRITE_DEMO)
    int i = 0;
    MS_U8 RSA_N[256] = {0};
    UBOOT_TRACE("IN\n");

    MDrv_CA_OTP_Write(0x3CB4, 0x40000);   // unlock read rsa data

    MDrv_CA_OTP_GetRSAKey(E_CA_RSA_KEY1, RSA_N, 256);
    for(i=0; i<256; i++)
    {
        printf("0x%02X, ", RSA_N[i]);
        if((i%16) == 15)
            printf("\r\n");
    }

    UBOOT_TRACE("OK\n");
#endif
    return ret;
}
