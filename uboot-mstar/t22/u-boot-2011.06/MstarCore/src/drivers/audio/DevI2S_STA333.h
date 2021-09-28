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

#ifndef _DEVI2S_H_
#define _DEVI2S_H_

#ifndef _BIT0
#define _BIT0  0x0001
#endif
#ifndef _BIT1
#define _BIT1  0x0002
#endif
#ifndef _BIT2
#define _BIT2  0x0004
#endif
#ifndef _BIT3
#define _BIT3  0x0008
#endif
#ifndef _BIT4
#define _BIT4  0x0010
#endif
#ifndef _BIT5
#define _BIT5  0x0020
#endif
#ifndef _BIT6
#define _BIT6  0x0040
#endif
#ifndef _BIT7
#define _BIT7  0x0080
#endif
#ifndef _BIT8
#define _BIT8  0x0100
#endif
#ifndef _BIT9
#define _BIT9  0x0200
#endif
#ifndef _BIT10
#define _BIT10 0x0400
#endif
#ifndef _BIT11
#define _BIT11 0x0800
#endif
#ifndef _BIT12
#define _BIT12 0x1000
#endif
#ifndef _BIT13
#define _BIT13 0x2000
#endif
#ifndef _BIT14
#define _BIT14 0x4000
#endif
#ifndef _BIT15
#define _BIT15 0x8000
#endif

#define DDX_I2C_WRITE_ADD							        0x38
#define STA333_ADDR     (((U16) E_I2C_BUS_SYS << 8) | DDX_I2C_WRITE_ADD)
#define DDX_CONFIGURE_A								0x00
#define DDX_CONFIGURE_B								0x01
#define DDX_CONFIGURE_C								0x02
#define DDX_CONFIGURE_D								0x03
#define DDX_CONFIGURE_E								0x04
#define DDX_CONFIGURE_F								0x05

#define DDX_MUTE									0x06
#define DDX_MAIN_VOLUME								0x07
#define DDX_CHANNEL1_VOL							0x08
#define DDX_CHANNEL2_VOL							0x09
#define DDX_CHANNEL3_VOL 							0x0a

#define DDX_AUTO1			 						0x0b
#define DDX_AUTO2			 						0x0c
#define DDX_AUTO3			 						0x0d

#define DDX_CHANNEL1_CFG 							0x0e
#define DDX_CHANNEL2_CFG 							0x0f
#define DDX_CHANNEL3_CFG 							0x10

#define DDX_TONEBASS								0x11
#define DDX_L1AR_RATE								0x12
#define DDX_L1AR_THRESHOLD							0x13
#define DDX_L2AR_RATE								0x14
#define DDX_L2AR_THRESHOLD							0x15

#define		REG_CFADDR1			0x16	/* Cfaddr1 */
#define		REG_B1CF1			0x17	/* B1cf1 */
#define		REG_B1CF2			0x18	/* B1cf2 */
#define		REG_B1CF3			0x19	/* B1cf3 */
#define		REG_B2CF1			0x1a	/* B2cf1 */
#define		REG_B2CF2			0x1b	/* B2cf2 */
#define		REG_B2CF3			0x1c	/* B2cf3 */
#define		REG_A1CF1			0x1d	/* A1cf1 */
#define		REG_A1CF2			0x1e	/* A1cf2 */
#define		REG_A1CF3			0x1f	/* A1cf3 */
#define		REG_A2CF1			0x20	/* A2cf1 */
#define		REG_A2CF2			0x21	/* A2cf2 */
#define		REG_A2CF3			0x22	/* A2cf3 */
#define		REG_B0CF1			0x23	/* B0cf1 */
#define		REG_B0CF2			0x24	/* B0cf2 */
#define		REG_B0CF3			0x25	/* B0cf3 */
#define		REG_CFUD			0x26	/* Cfud */


#define MAX_VOLUME				50

void DevI2s_STA333_Init(void);
void DevI2S_STA333_SubwoofValue(MS_U8 Subwoof);
#if 0//(ENABLE_WALL_MODE)
void DevI2SSetCFTableByMode(SoundWallModeType mode);
#else
void DevI2S_STA333_SetCFTable(void);
#endif
void DecI2S_STA333_Patch(BYTE ucCtl);
MS_BOOL STA333_GetPowerDNStatus(void);
#endif
