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
#ifndef DRV_GOP_SC_H
#define DRV_GOP_SC_H

#ifdef DRV_GOP_SC_C
#define INTERFACE
#else
#define INTERFACE extern
#endif


typedef enum
{
    EN_DST_FB=0,
    EN_DST_FBL=1,
    EN_DST_NUM,
}DRV_GOP_SC_Dst;

//External Info
typedef struct
{
    //op to ve timing
    MS_U16 u16VttIn;
    MS_U16 u16HttIn;
    MS_U16 u16HdeIn;
    MS_U16 u16VdeIn;

    MS_U16 u16Vde_St;
    MS_U16 u16Hde_St;

}DRV_GOP_SC_InCfg;

typedef struct
{
    //ve out timing
    MS_U16 u16VttOut;   //VttOut
    MS_U16 u16HttOut;   //HttOut
    MS_U16 u16HdeOut;    //HdeOut
    MS_U16 u16VdeOut;    //Vdeout

    MS_BOOL bInterlace;

}DRV_GOP_SC_OutCfg;

typedef struct
{
    MS_U8  u8MuxSel;
    MS_U8 u8TVSystem;
    DRV_GOP_SC_Dst stDst;

    DRV_GOP_SC_InCfg InCfg;
    DRV_GOP_SC_OutCfg OutCfg;

}DRV_GOP_SC_INFO;

//Internal Parameter
typedef struct
{
    MS_U16 u16ValidV;
    MS_U16 u16ValidH;
    MS_U8 u8SkipV;
    MS_U8 u8SkipH;

    MS_U16 u16VFacInt;
    MS_U16 u16VFacFrac;
    MS_U16 u16VE_RefY;
    MS_U16 u16VE_RefX;

    MS_U32 u32IDclk;
    MS_U32 u32ODclk;

}GOP_SC_REF;

MS_BOOL MDrv_GOP_SC_Init(void);
VOID MDrv_GOP_SC_Init_riu_base( MS_VIRT vriu_base );
MS_BOOL MDrv_GOP_SC_MuxSel(MS_U8 u8Source);
VOID MDrv_GOP_SC_SetParams(DRV_GOP_SC_InCfg *pInCfg,DRV_GOP_SC_OutCfg *pOutCfg,MS_U8 u8TvSys);
MS_BOOL MDrv_GOP_SC_SetCfg(VOID);
MS_BOOL MDrv_GOP_SC_SetDst(DRV_GOP_SC_Dst stDst);
MS_BOOL MDrv_GOP_SC_SetLock(VOID);
MS_BOOL MDrv_GOP_SC_SetFPLL_Enable(MS_BOOL bEnable);

#undef INTERFACE

#endif
