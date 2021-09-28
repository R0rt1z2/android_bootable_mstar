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

#ifndef __DRV_TAS5707_H__
#define __DRV_TAS5707_H__

#include <MsTypes.h>

typedef MS_U8      BOOLEAN;

// ***************************************************************************
// ************************* START OF REGISTER TYPES *************************
// ***************************************************************************
//----------------------------------------------------------------------------
// Control register bit field structure definitions
//----------------------------------------------------------------------------


//-------------------------------------------------------------------------
// Register maps enum
//-------------------------------------------------------------------------

typedef struct {

	U8		        ucAddr;
	U16		        ucNumber;
	U8	            sDatas[20];
} TAS5707_REGMAP, *TAS5707_PREGMAP;

#define BOOL        BOOLEAN

/*#ifdef DRV_TAS5707_C
#define INTERFACE
#else
#define INTERFACE extern
#endif*/
#ifdef DRV_TAS5707_C
    #define  TAS5707_EXT
#else
    #define  TAS5707_EXT  extern
#endif
//INTERFACE void drvTAS5707_Subwoof(void);
//INTERFACE void drvTAS5707_SubwoofValue(U8 Subwoof);
//INTERFACE void drvTAS5707_Shutdown(void);
TAS5707_EXT void drvTAS5707_SW_Init(void);
TAS5707_EXT BOOL drvTAS5707_Is_PowerOn(void);
//INTERFACE BOOLEAN Drv_TAS5707_READ_DATAS(U8 ucSubAddr, U8 ucCnt, U8* pBufs );
//INTERFACE BOOLEAN Drv_TAS5707_WRITE_DATAS(U8 ucSubAddr, U8 ucCnt, U8* pBufs );
//INTERFACE void drvTAS5707_SourceVolSel(INPUT_SOURCE_TYPE_t enInputSourceType);
//INTERFACE void drvTAS5707_SoftMute(BOOLEAN bMute);
//INTERFACE void drvTAS5707_AllChannelShutDown(BOOLEAN bEnable);
//INTERFACE void drvTAS5707_RegSwichChannel(BOOL bUseLeftCH);

#endif // __DRV_TAS5707_H__
