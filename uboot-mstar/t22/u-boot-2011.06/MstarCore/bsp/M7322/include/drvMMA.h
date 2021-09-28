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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  System Control Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRVMMA_H_
#define _DRVMMA_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// Definition
//
#define MSOS_HAL_IOVA_START_ADDR 0x200000000
#define MSOS_MMA_PIPE_ID_MAX_LEN 16
#define MSOS_MMA_BUFTAG_MAX_LEN  32

//
// Macros
//
#define MSOS_isIOVA(x) ((MS_U64)x & MSOS_HAL_IOVA_START_ADDR) > 0

//
// Structures
//
enum eMSOS_MMA_FEATURE {
    MSOS_MMA_NOT_SUPPORT,
    MSOS_MMA_SUPPORT,
};

typedef struct {
    MS_U32 u32support;  // enum eMSOS_MMA_FEATURE
    MS_U32 version;
    char   reserved[256];
} MSOS_MMA_FEATURE;

struct msos_mma_meminfo_t {
      MS_U64 addr;      // buffer start addr
      MS_U32 size;      // buffer size
      MS_S32 dmabuf_fd; // buffer fd
      char iova;        // is iova or not
      char secure;      // is secure buffer or not
      char miu_select;  // which miu the buffer located
      char reserved[256];
};

struct msos_mma_heapinfo_t {
    MS_U64 base_addr;                   // heap base addr
    MS_U64 size;                        // heap buffer size.
    char name[MSOS_MMA_BUFTAG_MAX_LEN]; // heap name
    void* buf_handle;                   // pointer to buf_handle
    char reserved[256];
};

struct msos_mma_buftag_info_t {
    char name[MSOS_MMA_BUFTAG_MAX_LEN];
    MS_U32 max_buf_size;
    MS_U32 miu;
    MS_U32 type; // 0:MMA-IOMMU, 1:MMA-CMA
    char reserved[256];
};
//
// Public APIs
//

//-------------------------------------------------------------------------------------------------
/// Feature query function for MMA driver
//  @param  feature      \b OUT: Information for whether or not MMA is supported
/// @return TRUE : support
/// @return FALSE : not support
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MSOS_MMA_query_feature(MSOS_MMA_FEATURE* feature);

//-------------------------------------------------------------------------------------------------
/// Buffer tag query function
//  @param  u8bufTag      \b IN: buffer tag
//  @param  buf_info      \b OUT: information of this buffer
/// @return TRUE : support
/// @return FALSE : not support
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MSOS_MMA_buf_tag_check(const MS_U8* u8bufTag, struct msos_mma_buftag_info_t* buf_info);

//-------------------------------------------------------------------------------------------------
/// Initialize MMA driver
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Init(void);

//-------------------------------------------------------------------------------------------------
/// Exit MMA driver
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Allocates buffer
/// @param  u8bufTag      \b IN:  buffer tag
/// @param  u32size       \b IN:  buffer size
/// @param  pPhy          \b OUT: ms_phy address of the allocated buffer
/// @param  ppBufHandle   \b OUT: not used
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Alloc(const MS_U8* u8bufTag, MS_U32 u32size, MS_PHY* pPhy, void** ppBufHandle);

//-------------------------------------------------------------------------------------------------
/// Allocates secure buffer
/// @param  u8bufTag      \b IN:  buffer tag
/// @param  u32size       \b IN:  buffer size
/// @param  pPhy          \b OUT: ms_phy address of the allocated buffer
/// @param  ppBufHandle   \b OUT: not used
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Sec_Alloc(const MS_U8* u8bufTag, MS_U32 u32size, MS_PHY* pPhy, void** ppBufHandle);

//-------------------------------------------------------------------------------------------------
/// Allocates pipe ID
/// @param  u32pipeID     \b OUT: pipe ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Pipeid_Alloc(MS_U32* u32pipeID);

//-------------------------------------------------------------------------------------------------
/// Release allocated pipe ID
/// @param  u32pipeID     \b IN: pipe ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Pipeid_Release(MS_U32* u32pipeID);

//-------------------------------------------------------------------------------------------------
/// Authorize allocated secure buffer from pipe config
/// @param  pPhy          \b IN: ms_phy address of the allocated buffer
/// @param  u32pipeID     \b IN: pipe ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Sec_AuthorizeFromPipeConfig(MS_PHY pPhy, MS_U32 u32pipeID);

//-------------------------------------------------------------------------------------------------
/// Unauthorize allocated secure buffer
/// @param  pPhy          \b IN: ms_phy address of the allocated buffer
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Sec_Unauthorize(MS_PHY pPhy);

//-------------------------------------------------------------------------------------------------
/// Reserve a specific range of ms_phy address for the specified buffer tags
/// @param  pu8space_tag  \b IN: space tag
/// @param  u64size       \b IN: address range
/// @param  pphyBaseAddr  \b IN: base address of space tag (ms_phy)
/// @param  u32tagNum     \b IN: number of buffer tag
/// @param  ...           \b IN: buffer tags
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_Reserve_Iova(const MS_U8 *pu8space_tag, MS_U64 u64size, MS_PHY* pphyBaseAddr, MS_U32 u32tagNum, ...);

//-------------------------------------------------------------------------------------------------
/// Free buffer
/// @param  pPhy          \b IN: ms_phy address of the allocated buffer
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_free(MS_PHY pPhy);

//-------------------------------------------------------------------------------------------------
/// Get Buffer info
/// @param  pPhy          \b IN: buffer start address
/// @param  pMem_info     \b OUT: buffer infomation
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_U32 __attribute__((weak)) MsOS_MMA_getinfo(MS_PHY pPhy, struct msos_mma_meminfo_t* pMem_info);

//-------------------------------------------------------------------------------------------------
/// BDMA memory copy
/// @param  phySrc        \b IN: source address
/// @param  phyDst        \b IN: destination address
/// @param  u32Len        \b IN: size of memory block to copy
//-------------------------------------------------------------------------------------------------
void __attribute__((weak)) MsOS_MMA_debugBdmacpy(MS_PHY phySrc, MS_PHY phyDst, MS_U32 u32Len);

#ifdef __cplusplus
};  /* end of extern "C" */
#endif

#endif /* _DRVMMA_H_ */
