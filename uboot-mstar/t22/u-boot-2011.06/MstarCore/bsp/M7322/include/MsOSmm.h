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
/// @brief  MsOS Memory management
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MSOSMM_H_
#define _MSOSMM_H_




#if defined (MSOS_TYPE_NOS)
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"


#define MSOSMMERROR_CREATE_POOL      -100
#define MSOSMMERROR_GET_MEM      -101
#define MSOSMMERROR_GET_MEM_TIMEOUT      -102
#define MSOSMMERROR_MAPVA      -103
#define MSOSMMERROR_LOCATE_CHUNK -104
#define MSOSMMERROR_DONOT_SUPPORT  -105

//enum for mapping virtual address type
enum MSOSMM_MAP_VA_TYPE_Param
{
    MSOSMM_MAP_VA_NO_NEEDED_Param = 0,  //we don't need to map VA
    MSOSMM_VA_CACHE_WRITE_BACK_Param = 1, //we need to map VA with cache type "write back", which is normally used
    MSOSMM_VA_CACHE_WRITE_THROUGH_Param = 2, //we need to map VA with cache type "write through", which is //mostly used by graphic system
    MSOSMM_VA_CACHE_NONE_CACHE_Param = 3,  //we don't need data cache
    MSOSMM_VA_TYPE_MAX_Param = 0xFFFFFFFF//force enum size to be 4 byte alignment
};



typedef struct _MSOS_MMPool_Chunk
{
    MS_U32 pool_handle_id;  //in:   pool handle id, when pool init, returned by kernel 
    MS_U64 offset_in_pool;  //in:   offset in pool
    MS_U64 length;          //in:   chunk length
}MSOS_MMPool_Chunk;

struct MSOS_MMPool_Unmap_Param
{
    MS_U8* virt_addr;       //in: the VA need to unmap
    MS_U64 length;          //in: unmap length
};

struct MSOS_MMPool_Map_Param
{
    MSOS_MMPool_Chunk chunk;
    
    MS_U32 cache_type;      //in:   cache type of mapping, //0:noncache,1:cache,2:bufferable
    MS_U8* virt_addr;       //out:  virtual address for user mode access
    MS_S32 error_code;      //out:  reason about why map failed
};

struct MSOS_MMPool_PutMem_Param
{
    MSOS_MMPool_Chunk chunk;
};

struct MSOS_MMPool_GetMem_Param 
{
    MSOS_MMPool_Chunk chunk;
    
    MS_S32 error_code;      //out:  reason about why MApi_MSOS_MMPool_GetMem failed
};

struct MSOS_MMPool_Init_Param
{
    char *pool_name;                //in:   global identify name for pool to shared between multiple process
	
    MS_U64 PoolStartPa;             //in:   pool location in pa
    MS_U64 len;                     //in: 

    MS_U32 pool_handle_id;          //out:  generate pool id based on heap specified by heap_id
    MS_U32 miu;                     //out:  miu id this heap belongs, index from 0.
    MS_U32 heap_type;              //out:  return heap type to application, //0: normal memory, 1:cma
    MS_S32 error_code;              //out:  error code when pool init failed
    MS_U64 heap_length; //out: heap leagth
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu

};


enum MSOS_MMPool_event_Param
{
  MSOS_MMPool_EVENT_CONFLICT_Param = 0,
  MSOS_MMPool_EVENT_NO_WAIT_Param,
  MSOS_MMPool_EVNET_NUM_Param
};

struct MSOS_MMPool_Event_Param
{
    enum MSOS_MMPool_event_Param event;
    MS_U32 pool_handle_id;
};

struct MSOS_MMPool_Notify_Param
{
    MS_U32 pool_handle_id;//pool handle
    void (*notify_callback)(MS_U32 pool_handle_id,MS_U64 start,MS_U64 length);
};

struct MSOS_MMPool_Locate_Chunk_Param
{
    MS_U32 pool_handle_id;  //in:   pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in:   offset in pool


    MS_U64 PA;//out:   PA value.
    MS_S32 error_code; // out£º reason why failed.

    MS_S32 miu;//out:  in which miu.
    MS_U32 heap_id;     //out: heap id the pa in .After get this,with the help of MApi_MSOS_MMPool_HEAP_ATTR,can get more more info.
};

MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void);
MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void);

MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param);
MS_BOOL __attribute__((weak)) MSOS_MMPool_Release(MS_U32 pool_handle_id);

MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem(struct MSOS_MMPool_GetMem_Param * get_param);
MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem_Timeout(struct MSOS_MMPool_GetMem_Param* get_param,MS_U32 timeout);
MS_BOOL __attribute__((weak)) MSOS_MMPool_PutMem(struct MSOS_MMPool_PutMem_Param * put_param);

MS_BOOL __attribute__((weak)) MSOS_MMPool_MapVA(struct MSOS_MMPool_Map_Param * map_param);//map va(same heap of msos). if want to have user mode va in kernel mode utopia, need map by it's .so.
void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param);

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param);

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param);

    #ifdef __cplusplus
}
#endif
#endif
#endif
