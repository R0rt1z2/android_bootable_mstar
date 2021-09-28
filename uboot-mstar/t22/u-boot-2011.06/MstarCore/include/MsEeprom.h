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

#ifndef __CMD_MS_EEPROM__
#define __CMD_MS_EEPROM__

#include <MsTypes.h>

#define EEPROM_PAGESIZE           0x10
#define BUFFER_SIZE               128


void eeprom_switch(int i);   // base 0

int do_eeprom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);

int eepromWrite( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);

int eepromRead( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);

MS_U16 getaddress();
//-------------------------------------------------------------------------------------------------    
/// Read bytes from the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @param  pBuf                                      \b IN: the read data dram buffer
/// @param  u16BufLen                                  \b IN: the read data length
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int eeprom_PageRead(MS_U16 offset, MS_U8 *pBuf, MS_U16 u16BufLen);

//-------------------------------------------------------------------------------------------------    
/// write bytes to the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @param  pBuf                                      \b IN: the write data dram buffer
/// @param  u16BufLen                                  \b IN: the write data length
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int eeprom_PageWrite(MS_U16 offset, MS_U8 *pBuf, MS_U16 u16BufLen);

//-------------------------------------------------------------------------------------------------    
/// Read 1 byte from the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @return  MS_U8                                    \b OUT: the read byte value
//-------------------------------------------------------------------------------------------------
MS_U8 eeprom_ReadByte(MS_U16 offset);
//-------------------------------------------------------------------------------------------------    
/// Write 1 byte to the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @return  u8Value                                  \b OUT: the write byte value
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int eeprom_WriteByte(MS_U16 offset,MS_U8 u8Value);

#endif

