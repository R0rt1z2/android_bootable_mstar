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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  PVR IframeLUT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PVR_IFRAMELUT_V2_H_
#define _DRV_PVR_IFRAMELUT_V2_H_

#include "drvPVR_IframeLUT.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum
{
    E_MDrv_CMD_PVR_IframeLUT_Init,
    E_MDrv_CMD_PVR_IframeLUT_Open,
    E_MDrv_CMD_PVR_IframeLUT_SetVdecType,
    E_MDrv_CMD_PVR_IframeLUT_GetWritePtr,
    E_MDrv_CMD_PVR_IframeLUT_Close,
    E_MDrv_CMD_PVR_IframeLUT_Exit,
} ePVRIframeLUTIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

// param of MDrv_PVR_IframeLUT_Init is void

// param of MDrv_PVR_IframeLUT_Open
typedef PVR_IFRAMELUT_Cfg PVR_IFRAMELUT_OPEN_PARAM;

// param of MDrv_PVR_IframeLUT_SetVdecType
typedef struct DLL_PACKED
{
    MS_U32 u32PvrEng;
    EN_PVR_IFRAMELUT_VDEC_TYPE eVdecType;
} PVR_IFRAMELUT_SETVDECTYPE_PARAM;

// param of MDrv_PVR_IframeLUT_GetWritePtr
typedef struct DLL_PACKED
{
    MS_U32 u32PvrEng;
    MS_U32 *pu32WritePtr;
} PVR_IFRAMELUT_GETWRITEPTR_PARAM;

// param of MDrv_PVR_IframeLUT_Close
typedef MS_U32 PVR_IFRAMELUT_CLOSE_PARAM;

// param of MDrv_PVR_IframeLUT_Exit is void

#ifdef __cplusplus
}
#endif

#endif // _DRV_PVR_IFRAMELUT_V2_H_
