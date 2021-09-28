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

#ifndef __DRV_URSA_6M60_H__
#define __DRV_URSA_6M60_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>
#include <apiPNL.h>
#include <drvUART.h>
#include <MsDebug.h>
#include <MsOS.h>

#define TERRA2_UPGRADE_BIN_PATH   getenv("Terra2UpgradePath")==NULL?"/Terra2Upgrade.bin":getenv("Terra2UpgradePath")

#define CRC32_TYPE_FLASH               (0x00)
#define CRC32_TYPE_DRAM                (0x01)
#define URSA_COMPRESS_LENGTH_OFFSET    (0x9C)

// ursa
#define SPI_HEADER_DDC_TYPE_SUPPORT 0x0
#define SPI_HEADER_DDC_TYPE_DUAL_BIN_ENABLE 0x1
#define SPI_HEADER_DDC_TYPE_SU_AP1_ADDR 0x2
#define SPI_HEADER_DDC_TYPE_SU_AP2_ADDR 0x3
#define SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG_ADDR 0x4
#define SPI_HEADER_DDC_TYPE_SU_BOOT_FLAG 0x5
#define SPI_HEADER_DDC_TYPE_FLASH_SIZE 0x6
#define SPI_HEADER_DDC_TYPE_FAST_UPGRADE_ENABLE 0x7

#define TERRA2_BANK_SIZE  (0x8000)

typedef struct {
    uint8_t  flash_size; // 1-4
    uint32_t first_bin_start_addr; // 5-8
    uint32_t second_bin_start_addr; // 9-12
    uint32_t boot_flag_addr; // 13-16
    uint8_t is_double_bin_enable; // 17
} SFlashHeader;

enum
{
    FastUpgradeStatusOk,
    FastUpgradeStatusNotSupport,
    FastUpgradeStatusInitFail,
    FastUpgradeStatusBurstWriteFail,
    FastUpgradeStatusGetDumpAddrFail,
    FastUpgradeStatusProgramRegFail,
    FastUpgradeStatusChkDRAMFail,
    FastUpgradeStatusChkFLASHFail,
    FastUpgradeStatusWriteDRAMFail,
    FastUpgradeStatusWriteFlashFail,
    FastUpgradeStatusUpdateBootFlagFail,
};

//-------------------------------------------------------------------------------------------------
/// set m_UrsaCmdData
/// @return          \b OUT: MS_FALSE: failure
//-------------------------------------------------------------------------------------------------
MS_BOOL InitUrsaCmdData(void);

void combineDDC2UART(MS_U8 *u8UartBuf, MS_U8 *u8DDCBuf);

void calUartCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen );
void CalCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen);
void RxHandler(int c);

//-------------------------------------------------------------------------------------------------
/// Send command by uart
/// @param cValue        \b  : Which command you want to send
/// @param u32Length       \b  : Command length
/// @return Sent buffer size
//-------------------------------------------------------------------------------------------------
MS_U16 Send(MS_U8* cValue, const MS_U16 u16Length);
//-------------------------------------------------------------------------------------------------
/// Send command from uart
/// @param cValue        \b  : Read command buffer
/// @param u32Length       \b  : Command length
/// @return road buffer size
//-------------------------------------------------------------------------------------------------
MS_U32 Read(MS_U8* cValue, const MS_U32 u32Length);

//-------------------------------------------------------------------------------------------------
/// Write bytes, be able to write 7 byte
/// @param u8Buf   \b IN:  pointer to 7 bytes write data
/// @return           \b OUT: MS_TRUE:  success
/// @return           \b OUT: MS_FALSE: failure
//-------------------------------------------------------------------------------------------------
MS_BOOL Cmd_DataWrite(MS_U8 *u8Buf);

  //-------------------------------------------------------------------------------------------------
 /// Read bytes, be able to read n byte
 /// @param u8Buf   \b IN:  pointer to 1 bytes read data
 /// @param u16Size   \b IN:  size of pointer u8Buf
 /// @return           \b OUT: MS_TRUE:  success
 /// @return           \b OUT: MS_FALSE: failure
 //-------------------------------------------------------------------------------------------------
MS_BOOL Cmd_DataRead(MS_U8 *u8Buf, MS_U16 u16Size);
 //-------------------------------------------------------------------------------------------------
/// Get ursa version info
/// @return ursa version info
//-------------------------------------------------------------------------------------------------
MS_U32 GetUrsaVersionInfo(void);

int do_GetUrsaVersionInfo (cmd_tbl_t *cmdtp, int flag, int argc, char * const  argv[]);

int do_Change_Uart_connect_port2 (cmd_tbl_t *cmdtp, int flag, int argc, char * const  argv[]);
void Terra2_Upgrade_Init(MS_U32 u32Addr, MS_U32 u32Size);
MS_BOOL get_sw_upgrade_spi_header(void);
int checkTerra2Bin(MS_U32 u32Addr, MS_U32 u32FileSize);
int getUpgradeBinFromUsb(MS_U32* pAddr, MS_U32* pFileSize);
MS_BOOL is_support_get_spi_header_by_ddc(void);
MS_BOOL set_sw_upgrade_init();
MS_BOOL terra2_uart_burst_write(void);
MS_BOOL get_sw_upgrade_crc32(int type);
MS_BOOL set_sw_upgrade_write_flash();
#endif //__DRV_URSA_6M60_H__
