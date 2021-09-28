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

#include "MsTypes.h"
#include "ScriptTypes.h"


static ST_BuffInfo *_gpstBufInfoJPDRad = NULL;
static ST_BuffInfo *_gpstBufInfoJPDInter = NULL;
static ST_BuffInfo *_gpstBufInfoJPDOutput = NULL;
static ST_BuffInfo *_gpstBufInfoGOPOutput = NULL;
static ST_BuffInfo *_gpstBufInfoVEFrame = NULL;

MS_BOOL Script_MemMgr_SetBufInfo(EN_BUFF_ID eBufID, ST_BuffInfo* pstBuffInfo)
{
    ST_BuffInfo *tmp = NULL;

    switch (eBufID)
    {
        case E_BUFF_ID_JPD_READ:
            _gpstBufInfoJPDRad = pstBuffInfo;
            tmp = _gpstBufInfoJPDRad;
            LDR_PUTS("E_BUFF_ID_JPD_READ:  ");
            break;
        case E_BUFF_ID_JPD_INTER:
            _gpstBufInfoJPDInter = pstBuffInfo;
            tmp = _gpstBufInfoJPDInter;
            LDR_PUTS("E_BUFF_ID_JPD_INTER:  ");
            break;
        case E_BUFF_ID_JPD_OUTPUT:
            _gpstBufInfoJPDOutput = pstBuffInfo;
            tmp = _gpstBufInfoJPDOutput;
            LDR_PUTS("E_BUFF_ID_JPD_OUTPUT:  ");
            break;
        case E_BUFF_ID_GOP_OUTPUT:
            _gpstBufInfoGOPOutput = pstBuffInfo;
            tmp = _gpstBufInfoGOPOutput;
            LDR_PUTS("E_BUFF_ID_GOP_OUTPUT:  ");
            break;
        case E_BUFF_ID_VE_FRAME:
            _gpstBufInfoVEFrame = pstBuffInfo;
            tmp = _gpstBufInfoVEFrame;
            LDR_PUTS("E_BUFF_ID_VE_FRAME:  ");
            break;
        default :
            LDR_PUTS("Wrong Buffer ID\n");
            break;
    }

    LDR_PUTDW(tmp->u32VirtualAddr);
    LDR_PUTS("   ");
    LDR_PUTDW(tmp->u32PhysicalAddr);
    LDR_PUTS("   ");
    LDR_PUTDW(tmp->u32Size);
    LDR_PUTS("\n");

    return TRUE;
}

ST_BuffInfo* Script_MemMgr_GetBufInfo(EN_BUFF_ID eBufID)
{
    switch (eBufID)
    {
        case E_BUFF_ID_JPD_READ:
            return _gpstBufInfoJPDRad;
            break;
        case E_BUFF_ID_JPD_INTER:
            return _gpstBufInfoJPDInter;
            break;
        case E_BUFF_ID_JPD_OUTPUT:
            return _gpstBufInfoJPDOutput;
            break;
        case E_BUFF_ID_GOP_OUTPUT:
            return _gpstBufInfoGOPOutput;
            break;
        case E_BUFF_ID_VE_FRAME:
            return _gpstBufInfoVEFrame;
            break;
        default :
            LDR_PUTS("Wrong Buffer ID\n");
            return NULL;
            break;
    }
}
