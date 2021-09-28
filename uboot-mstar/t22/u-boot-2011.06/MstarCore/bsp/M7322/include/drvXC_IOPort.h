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
/// @brief  IO ports API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IOPORT_H_
#define _IOPORT_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

///This Enum is interface for Hal and Application level.
typedef enum
{
    INPUT_PORT_NONE_PORT = 0,

    INPUT_PORT_ANALOG0 = 1,        // Data port
    INPUT_PORT_ANALOG1,
    INPUT_PORT_ANALOG2,
    INPUT_PORT_ANALOG3,
    INPUT_PORT_ANALOG4,

    // Reserved area

    INPUT_PORT_ANALOG0_SYNC = 10,  // Sync port
    INPUT_PORT_ANALOG1_SYNC,
    INPUT_PORT_ANALOG2_SYNC,
    INPUT_PORT_ANALOG3_SYNC,
    INPUT_PORT_ANALOG4_SYNC,

    // Reserved area

    INPUT_PORT_YMUX_CVBS0 = 30,
    INPUT_PORT_YMUX_CVBS1,
    INPUT_PORT_YMUX_CVBS2,
    INPUT_PORT_YMUX_CVBS3,
    INPUT_PORT_YMUX_CVBS4,
    INPUT_PORT_YMUX_CVBS5,
    INPUT_PORT_YMUX_CVBS6,
    INPUT_PORT_YMUX_CVBS7,
    INPUT_PORT_YMUX_G0,
    INPUT_PORT_YMUX_G1,
    INPUT_PORT_YMUX_G2,
    INPUT_PORT_YMUX_R0,
    INPUT_PORT_YMUX_R1,
    INPUT_PORT_YMUX_R2,
    INPUT_PORT_YMUX_B0,
    INPUT_PORT_YMUX_B1,
    INPUT_PORT_YMUX_B2,

    // Reserved area

    INPUT_PORT_CMUX_CVBS0 = 50,
    INPUT_PORT_CMUX_CVBS1,
    INPUT_PORT_CMUX_CVBS2,
    INPUT_PORT_CMUX_CVBS3,
    INPUT_PORT_CMUX_CVBS4,
    INPUT_PORT_CMUX_CVBS5,
    INPUT_PORT_CMUX_CVBS6,
    INPUT_PORT_CMUX_CVBS7,
    INPUT_PORT_CMUX_R0,
    INPUT_PORT_CMUX_R1,
    INPUT_PORT_CMUX_R2,
    INPUT_PORT_CMUX_G0,
    INPUT_PORT_CMUX_G1,
    INPUT_PORT_CMUX_G2,
    INPUT_PORT_CMUX_B0,
    INPUT_PORT_CMUX_B1,
    INPUT_PORT_CMUX_B2,
    // Reserved area

    INPUT_PORT_DVI0 = 80,
    INPUT_PORT_DVI1,
    INPUT_PORT_DVI2,
    INPUT_PORT_DVI3,

    // Reserved area

    INPUT_PORT_MVOP = 100,
    INPUT_PORT_MVOP2,
    INPUT_PORT_MVOP3,

    INPUT_PORT_SCALER_OP = 110,
    INPUT_PORT_SCALER_DI,

    INPUT_PORT_YMUX_RY0 = 120,
    INPUT_PORT_YMUX_RY1,
    INPUT_PORT_YMUX_RY2,
} E_MUX_INPUTPORT;

///This Enum is interface for Hal and Application level.
typedef enum
{
    E_INPUT_NOT_SUPPORT_MHL = 0x0,
    E_INPUT_SUPPORT_MHL_PORT_DVI0 = 0x1,
    E_INPUT_SUPPORT_MHL_PORT_DVI1 = 0x2,
    E_INPUT_SUPPORT_MHL_PORT_DVI2 = 0x4,
    E_INPUT_SUPPORT_MHL_PORT_DVI3 = 0x8,
    E_INPUT_ALL_SUPPORT_MHL = 0xF,

}EN_MUX_INPUTPORT_MHL_INFO;

///This Enum is interface for Hal and Application level.
typedef enum
{
    OUTPUT_PORT_NONE_PORT = 0,

    OUTPUT_PORT_SCALER_MAIN_WINDOW = 1,
    OUTPUT_PORT_SCALER2_MAIN_WINDOW = 2,

    // Reserved area

    OUTPUT_PORT_SCALER_SUB_WINDOW1 = 10,
    OUTPUT_PORT_SCALER2_SUB_WINDOW = 11,

    // Reserved area

    OUTPUT_PORT_CVBS1 = 20,
    OUTPUT_PORT_CVBS2,

    // Reserved area

    OUTPUT_PORT_YPBPR1 = 40,
    OUTPUT_PORT_YPBPR2,

    // Reserved area

    OUTPUT_PORT_HDMI1 = 60,
    OUTPUT_PORT_HDMI2,

    // Reserved area

    OUTPUT_PORT_OFFLINE_DETECT = 80,

    // Reserved area
    OUTPUT_PORT_DWIN = 100,

} E_MUX_OUTPUTPORT;

#define IsAnalogPort(x)         ( (x) >= INPUT_PORT_ANALOG0 && (x) <= INPUT_PORT_ANALOG4 )
#define IsDVIPort(x)            ( (x) >= INPUT_PORT_DVI0 && (x) <= INPUT_PORT_DVI3 )
#define IsYCVBSPort(x)          ( (x) >= INPUT_PORT_YMUX_CVBS0 && (x) <= INPUT_PORT_YMUX_CVBS7 )
#define IsCCVBSPort(x)          ( (x) >= INPUT_PORT_CMUX_CVBS0 && (x) <= INPUT_PORT_CMUX_CVBS7 )
#define IsCVBSPort(x)           ( IsYCVBSPort(x) || IsCCVBSPort(x) )

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#endif // _IOPORT_H_
