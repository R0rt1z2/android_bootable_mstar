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
#ifndef AMPLIFIER_RT9113_H
#define AMPLIFIER_RT9113_H

#define MAPI_BOOL MS_BOOL
#define MAPI_U8 MS_U8
#define MAPI_TRUE TRUE
#define MAPI_FALSE FALSE

enum
{
    I2C_Sub_ClockControlReg         = 0x00,        // default 0x6C        1 byte
    I2C_Sub_DeviceIDReg               = 0x01,        // default 0x28        1 byte
    I2C_Sub_ErrorStatusReg           = 0x02,        // default 0x00        1 byte
    I2C_Sub_SystemControl1Reg     = 0x03,        // default 0xA0        1 byte
    I2C_Sub_SerialDataInterReg     = 0x04,        // default 0x05        1 byte
    I2C_Sub_SystemControl2Reg     = 0x05,        // default 0x40        1 byte
    I2C_Sub_SoftMuteReg            = 0x06,        // default 0x00        1 byte
    I2C_Sub_MasterVolumnReg        = 0x07,        // default 0xFF(Mute)    1 byte
    I2C_Sub_Channel1VolReg        = 0x08,        // default 0x30(0 db)        1 byte
    I2C_Sub_Channel2VolReg        = 0x09,        // default 0x30(0 db)        1 byte
    I2C_Sub_Channel3VolReg        = 0x0A,        // default 0x30(0 db)        1 byte
    I2C_Sub_Channel4VolReg        = 0x0B,        // default 0x30(0 db)        1 byte
    I2C_Sub_HpVolReg                = 0x0C,        // default 0x30(0 db)        1 byte
    I2C_Sub_Channel6VolReg        = 0x0D,        // default 0x30(0 db)        1 byte
    I2C_Sub_VolumnConfigReg        = 0x0E,        // default 0x91        1 byte
    //I2C_Sub_ReservedReg            = 0x0F,        //  Reserved

    I2C_Sub_ModLimitReg            = 0x10,        //  default 0x02        1 byte
    I2C_Sub_IcDelayCh1Reg        = 0x11,        //  default 0x4C    1 byte
    I2C_Sub_IcDelayCh2Reg        = 0x12,        //  default 0x34        1 byte
    I2C_Sub_IcDelayCh3Reg        = 0x13,        //  default 0x1C    1 byte
    I2C_Sub_IcDelayCh4Reg        = 0x14,        //  default 0x64        1 byte
    I2C_Sub_IcDelayCh5Reg        = 0x15,        //  default 0xB0    1 byte
    I2C_Sub_IcDelayCh6Reg        = 0x16,        //  default 0x90        1 byte
    I2C_Sub_OffsetReg            = 0x17,        //  default 0x00        1 byte
    I2C_Sub_ReservedReg            = 0x18,        //  Reserved
    I2C_Sub_PwmShutReg            = 0x19,        //  default 0x30        1 byte
    I2C_Sub_StartPeriodReg        = 0x1A,        //  default 0x0A    1 byte
    I2C_Sub_OscTrimReg            = 0x1B,        //  default 0x82        1 byte
    I2C_Sub_BkErrorReg            = 0x1C,        //  default 0x02        1 byte
    //I2C_Sub_ReservedReg            = 0x1D,        //  Reserved
    //I2C_Sub_ReservedReg            = 0x1E,        //  Reserved
    //I2C_Sub_ReservedReg            = 0x1F,        //  Reserved

    I2C_Sub_InputMuxReg            = 0x20,        //  default 0x0089777A        4 byte
    I2C_Sub_DownMixInputReg        = 0x21,        //  default 0x00004203        4 byte
    I2C_Sub_AmTunFreqReg        = 0x22,        //  default 0x00000000        4 byte
    I2C_Sub_Ch6Bq2Reg            = 0x23,        //  default 0x00800000        20 byte (loudness BQ)  ....other value eq 0x000000000
    I2C_Sub_Ch6Bq3Reg            = 0x24,        //  default 0x00800000        20 byte (post vol BQ)  ....other value eq 0x000000000
    I2C_Sub_PwmMuxReg            = 0x25,        //  default 0x01021345        4  byte
    I2C_Sub_1DivGReg            = 0x26,        //  default 0x00800000        4  byte
    //I2C_Sub_ReservedReg            = 0x27,        //  Reserved
    I2C_Sub_ScaleReg                = 0x28,        //  default 0x00800000        4  byte
    I2C_Sub_Ch1Bq0Reg            = 0x29,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch1Bq1Reg            = 0x2A,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch1Bq2Reg            = 0x2B,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch1Bq3Reg            = 0x2C,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch1Bq4Reg            = 0x2D,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch1Bq5Reg            = 0x2E,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch1Bq6Reg            = 0x2F,        //  default 0x00800000        20 byte   ....other value eq 0x000000000

    I2C_Sub_Ch2Bq0Reg            = 0x30,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch2Bq1Reg            = 0x31,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch2Bq2Reg            = 0x32,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch2Bq3Reg            = 0x33,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch2Bq4Reg            = 0x34,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch2Bq5Reg            = 0x35,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch2Bq6Reg            = 0x36,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch6Bq0Reg            = 0x37,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    I2C_Sub_Ch6Bq1Reg            = 0x38,        //  default 0x00800000        20 byte   ....other value eq 0x000000000
    //I2C_Sub_ReservedReg            = 0x39,        //  Reserved
    I2C_Sub_Drc1aeReg            = 0x3A,        //  default 0x00800000        8  byte   ....other value eq 0x000000000
    I2C_Sub_Drc1aaReg            = 0x3B,        //  default 0x00800000        8  byte   ....other value eq 0x000000000
    I2C_Sub_Drc1adReg            = 0x3C,        //  default 0x00800000        8  byte   ....other value eq 0x000000000
    I2C_Sub_Drc2aeReg            = 0x3D,        //  default 0x00800000        8  byte   ....other value eq 0x000000000
    I2C_Sub_Drc2aaReg            = 0x3E,        //  default 0x00800000        8  byte   ....other value eq 0x000000000
    I2C_Sub_Drc2adReg            = 0x3F,        //  default 0x00800000        8  byte   ....other value eq 0x000000000

    I2C_Sub_Drc1TReg            = 0x40,        //  default 0xFDA21490        4 byte
    I2C_Sub_Drc1KReg            = 0x41,        //  default 0x03842109        4 byte
    I2C_Sub_Drc1OReg            = 0x42,        //  default 0x03842109        4 byte
    I2C_Sub_Drc2TReg            = 0x43,        //  default 0xFDA21490        4 byte
    I2C_Sub_Drc2KReg            = 0x44,        //  default 0x03842109        4 byte
    I2C_Sub_Drc2OReg            = 0x45,        //  default 0x03842109        4 byte
    I2C_Sub_DrcControlReg        = 0x46,        //  default 0x00000000        4 byte
    //I2C_Sub_ReservedReg            = 0x47,        //  Reserved
    //I2C_Sub_ReservedReg            = 0x48,        //  Reserved
    //I2C_Sub_ReservedReg            = 0x49,        //  Reserved

    I2C_Sub_BankUpdateComReg    = 0x50,        //  default 0x00000000        4 byte

};

    MAPI_BOOL  Amplifier_RT9113_Init(void);
    MAPI_BOOL Amplifier_RT9113_SWMute(MAPI_BOOL bMute);
    MAPI_BOOL Amplifier_RT9113_Mute(MAPI_U8 u8Para);
    MAPI_BOOL Amplifier_RT9113_StandBy(MAPI_U8 u8Para);
    MAPI_BOOL Amplifier_RT9113_Reset(MAPI_U8 u8Para);

#endif
