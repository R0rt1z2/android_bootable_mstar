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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Serial Flash COommand Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    E_SPINAND_DBGLV_NONE,    //disable all the debug message
    E_SPINAND_DBGLV_INFO,    //information
    E_SPINAND_DBGLV_NOTICE,  //normal but significant condition
    E_SPINAND_DBGLV_WARNING, //warning conditions
    E_SPINAND_DBGLV_ERR,     //error conditions
    E_SPINAND_DBGLV_CRIT,    //critical conditions
    E_SPINAND_ALERT,   //action must be taken immediately
    E_SPINAND_DBGLV_EMERG,   //system is unusable
    E_SPINAND_DBGLV_DEBUG,   //debug-level messages
} SPINAND_DbgLv;

SPINAND_FLASH_INFO_t _gtSpinandInfo;

#define SPARE_SIZE  _gtSpinandInfo.u16_SpareByteCnt
#define BLOCK_SIZE  _gtSpinandInfo.u16_BlkPageCnt
#define SECTOR_SIZE _gtSpinandInfo.u16_SectorByteCnt
#define PAGE_SIZE   _gtSpinandInfo.u16_PageByteCnt
#define PLANE       _gtSpinandInfo.u8PlaneCnt
#define CLKCFG      _gtSpinandInfo.u8CLKConfig
#define READ_MODE   _gtSpinandInfo.u8ReadMode
#define WRITE_MODE  _gtSpinandInfo.u8WriteMode
#define RIU_FLAG    _gtSpinandInfo.bRIURead
// SPI NAND COMMAND
#define SPI_NAND_CMD_PGRD                        0x13
#define SPI_NAND_CMD_RDID                        0x9F
#define SPI_NAND_CMD_WREN                        0x06
#define SPI_NAND_CMD_WRDIS                       0x04
#define SPI_NAND_CMD_RFC                         0x03
#define SPI_NAND_CMD_PP                          0x02
#define SPI_NAND_CMD_RPL                         0x84
#define SPI_NAND_CMD_PE                          0x10
#define SPI_NAND_CMD_GF                          0x0F
#define SPI_NAND_CMD_RESET                       0xFF

        #define SPI_NAND_REG_PROT                0xA0
        #define SPI_NAND_REG_FEAT                0xB0
                #define QUAD_ENABLE              1
        #define SPI_NAND_REG_STAT                0xC0
                #define SPI_NAND_STAT_OIP        (0x01 << 0)
                #define E_FAIL                   (0x01 << 2)
                #define P_FAIL                   (0x01 << 3)
                #define ECC_STATUS_PASS          (0x00 << 4)
                #define ECC_STATUS_ERR           (0x02 << 4)
                #define ECC_STATUS_BITFLIP    (0x01 << 4)
        #define SPI_NAND_REG_FUT                 0xD0
#define SPI_NAND_CMD_SF                          0x1F
#define SPI_NAND_CMD_BE                          0xD8
#define SPI_NAND_CMD_CE                          0xC7
#define SPI_NAND_ADDR_LEN                        3
#define SPI_NAND_PAGE_ADDR_LEN                   2
#define SPI_NAND_PLANE_OFFSET                    6
#define SPI_PLANE_ADDR_MASK                      0x40


