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

///////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar Demux Driver Interface for Tee
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup DMX TEE modules
  */
//-------------------------------------------------

#ifndef __API_DMX_TEE_H__
#define __API_DMX_TEE_H__

#include "MsCommon.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Pocily Mamager Pipie id definitation
#define DMX_PIPE_NULL                                       0xFFFFFFFF
#define DMX_PIPE_TSPFW                                      0x00000001
#define DMX_PIPE_TSPVQ                                      0x00000002
#define DMX_PIPE_PVR_0                                      0x80000000
#define DMX_PIPE_PVR_1                                      0x80000001
#define DMX_PIPE_PVR_2                                      0x80000002
#define DMX_PIPE_PVR_3                                      0x80000003
#define DMX_PIPE_FILE_0                                     0x40000000
#define DMX_PIPE_FILE_1                                     0x40000001
#define DMX_PIPE_FILE_2                                     0x40000002
#define DMX_PIPE_FILE_3                                     0x40000003
#define DMX_PIPE_MMFI_0                                     0x41000000
#define DMX_PIPE_MMFI_1                                     0x41000001

#ifdef MSOS_TYPE_OPTEE
typedef struct
{
    MS_U32 ID[4]; //id[3]: 0, id[2]=address_h, od[1]=address_l, id[0]=length
}DMX_TEE_RESOURCE;
#endif //MSOS_TYPE_OPTEE

typedef struct _DMX_TEE_BUF_Param
{
    MS_U32  u32EngId;             // Engine ID
    MS_U32  u32Opt;               // Option value or MIU select number
    MS_U32  u32BufSize;          // Phsical Buffer Size
    MS_U32  u32BufAddr;          // Phsical Buffer Address
    MS_U32  u32BufAddr_H;        // Phsical Buffer Address_H
    MS_U32  u32PvrSrc;
} DMX_TEE_BUF_Param;

typedef enum
{
    E_DMX_TEECMD_SET_TSPFW_BUF,                ///< For TSP FW buffer control
    E_DMX_TEECMD_SET_TSPVQ_BUF,                ///< For TSP VQ buffer control
    E_DMX_TEECMD_SET_PVR_BUF,                  ///< For TSP PVR buffer control
    E_DMX_TEECMD_SET_FILEIN_BUF,               ///< For TSP Finein buffer control
    E_DMX_TEECMD_SET_MMFI_BUF,                 ///< For TSP MMFI buffer control
} DMX_TEE_USER_CMD;

typedef enum
{
    E_DMX_REE_TO_TEE_CMD_NULL,
    E_DMX_REE_TO_TEE_CMD_FW_LoadCode,               //None parameters
    E_DMX_REE_TO_TEE_CMD_SET_VqBuf,                 //None parameters
    E_DMX_REE_TO_TEE_CMD_SET_PvrBuf,                //Param1: Engine id, Param2: Option 1 is reset buffer to 0
    E_DMX_REE_TO_TEE_CMD_SET_FileinBuf,             //Param1: Engine id, Param2: Buf address, Param3: Buf size
    E_DMX_REE_TO_TEE_CMD_GET_PvrWPtr,               //Param1: Engine id, Param2: Return Buf address
    E_DMX_REE_TO_TEE_CMD_SET_MMFIBuf,               //Param1: Engine id, Param2: Return Buf address
    E_DMX_REE_TO_TEE_CMD_SET_PVRSrc                  //Param1: Engine id, Param5: PVRSrc from
} DMX_REE_TO_TEE_CMD_TYPE;

//-----------------------------------------------------------------------------
//  Functions
//-----------------------------------------------------------------------------


//Used for TEE
//-------------------------------------------------------------------------------------------------
/// Create DMX Tee service task
/// @ingroup DMX_Tee
/// @return TRUE or FALSE for Pass or Fail
/// @note Please call this API when Tee system init, and must create this task before DMX init is called
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_NUTTX
MS_BOOL MApi_DMX_TEE_Create_Tee_System(void);
#endif

//------------------------------------------------------------------------------
/// SetControl for TEE
/// @ingroup DMX_Tee
/// @param cmd \b IN : TEE user cmd
/// @param param \b IN : TEE control parameters
/// @return MS_BOOL
/// @note Tee set DMX setting inform to DMX
//------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_SetControl(DMX_TEE_USER_CMD cmd, void* param);

//------------------------------------------------------------------------------
/// Process REE commend
/// @param cmd \b IN : Ree Commend
/// @param param \b IN : TEE control parameters
/// @return MS_U32 Error code
/// @note For buffer control commend, the input parameters should be DMX_TEE_BUF_Param
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_TEE_ProcReeCmd(DMX_REE_TO_TEE_CMD_TYPE cmd, void* param);

#ifdef MSOS_TYPE_OPTEE
//------------------------------------------------------------------------------
/// Get secure resource by Pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param pu32MaxNRetRes \b IN : Pointer to store max request resource number
/// @param pResource \b OUT : Pointer to store resource data
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_GetResourceByPipeID(MS_U32 u32PipeID, MS_U32* pu32MaxNRetRes, DMX_TEE_RESOURCE* pResource);

//------------------------------------------------------------------------------
/// Configure secure resource by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_ConfigPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);

//------------------------------------------------------------------------------
/// Check secure resource status by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Status: 0 -> Configure failed; 1 -> Configure OK
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_CheckPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
#endif //MSOS_TYPE_OPTEE

#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_TEE_H__
