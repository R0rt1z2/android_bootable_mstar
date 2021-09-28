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
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GOP_SC_H_
#define _API_GOP_SC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_GOP_SC_LIBVER              {'0','0'}            //LIB version
#define MSIF_GOP_SC_BUILDNUM            {'1','1'}            //Build Number
#define MSIF_GOP_SC_CHANGELIST          {'0','0','3','9','1','8','8','7'} //P4 ChangeList Number

typedef enum
{
    EN_GOP_ARGB =0,
    EN_SC_RGB
}EN_GOP_SC_MUX_SEL;

typedef enum
{
    EN_GOP_SC_NTSC =0,
    EN_GOP_SC_PAL,
    EN_GOP_SC_PAL_M,
    EN_GOP_SC_TVSYS_NUM
} EN_GOP_SC_TVSYS;

typedef enum
{
    EN_GOP_SC_DST_FB=0,
    EN_GOP_SC_DST_FBL=1,
    EN_GOP_SC_DST_NUM,

}MAPI_GOP_SC_Dst;

typedef struct
{
    //op to ve timing
    MS_U16 u16VTotal;   //VttIn for GOP_SC
    MS_U16 u16HTotal;   //HttIn for GOP_SC
    MS_U16 u16Width;    //HdeIn for GOP_SC
    MS_U16 u16Hight;    //VdeIn for GOP_SC

    MS_U16 u16Vde_St;
    MS_U16 u16Hde_St;

}MAPI_GOP_SC_InCfg;

typedef struct
{
    EN_GOP_SC_MUX_SEL  enSrcSel;

    MAPI_GOP_SC_InCfg stInputCfg;
    EN_GOP_SC_TVSYS u8TVSys;
    MAPI_GOP_SC_Dst stDst;

}MAPI_GOP_SC_INFO;

void MApi_GOP_SC_SkipInit(MS_BOOL bEnable);
MS_BOOL MApi_GOP_SC_Init(void);
MS_BOOL MApi_GOP_SC_SetCfg(MAPI_GOP_SC_INFO *pGopSC_Info);
MS_BOOL MApi_GOP_SC_MuxSel(EN_GOP_SC_MUX_SEL u8Sel);
MS_BOOL MApi_GOP_SC_SetFPLL_Enable(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif
