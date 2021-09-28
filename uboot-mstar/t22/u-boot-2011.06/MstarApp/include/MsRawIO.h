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

#ifndef __MSRAWIO_H__
#define __MSRAWIO_H__
#include <CusRawIO.h>
typedef enum{
    E_RAW_DATA_IN_SPI=0,
    E_RAW_DATA_IN_NAND,
    E_RAW_DATA_IN_MMC,
    E_RAW_DATA_IN_UFS,
    E_RAW_DATA_INVALED,
    E_RAW_DATA_MAX=E_RAW_DATA_INVALED
}EN_RAW_DATA_TARGET;

int do_raw_io_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_config(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_status(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_config_push(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_config_pop(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int check_ubi_partition(char * volName,char *partitionName);


//-------------------------------------------------------------------------------------------------    
/// Config the taret for raw data read/write.
/// @param  target                                  \b IN: where the raw data is.
/// @param  partition                                  \b IN: which partition, if the taget device doesn't have partition, please input 'UNNECESSARY_PARTITION'
/// @param  volume                                  \b IN: which volume, if the taget device doesn't have volume, please input 'UNNECESSARY_PARTITION'
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int raw_io_config(EN_RAW_DATA_TARGET target, char *partition, char *volume);

//-------------------------------------------------------------------------------------------------    
/// Read raw data from the taget device
/// @param  addr                                     \b IN: the address of input buffer
/// @param  offset                                   \b IN: the offset in the target device
/// @param  len                                       \b IN: read len
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int raw_read(unsigned int addr, unsigned int offset, unsigned int len);

//-------------------------------------------------------------------------------------------------    
/// write raw data from the taget device
/// @param  addr                                     \b IN: the address of input buffer
/// @param  offset                                   \b IN: the offset in the target device
/// @param  len                                       \b IN: write len
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int raw_write(unsigned int addr, unsigned int offset, unsigned int len);
//-------------------------------------------------------------------------------------------------    
/// get raw config status
/// @return  EN_RAW_DATA_TARGET                        \b OUT: raw config status
//-------------------------------------------------------------------------------------------------
EN_RAW_DATA_TARGET get_raw_status(void);

//-------------------------------------------------------------------------------------------------    
/// raw_io push config setting to internal stack
//-------------------------------------------------------------------------------------------------
void raw_io_config_push(void);
//-------------------------------------------------------------------------------------------------    
/// raw_io pop config setting to internal stack
//-------------------------------------------------------------------------------------------------
void raw_io_config_pop(void);

//-------------------------------------------------------------------------------------------------    
/// Config the taret for raw data read/write for mboot.bin location
//-------------------------------------------------------------------------------------------------
int mboot_raw_io_Config(void);

#if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
//-------------------------------------------------------------------------------------------------    
/// Config the taret for raw data read/write for mboot.bin location
//-------------------------------------------------------------------------------------------------
int mbootbak_raw_io_Config(void);
#endif

#endif //__MSRAWIO_H__

