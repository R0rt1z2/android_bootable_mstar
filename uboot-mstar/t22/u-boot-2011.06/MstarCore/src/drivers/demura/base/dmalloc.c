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
#include <MsTypes.h>
#include <stdio.h>
#include <linux/string.h>
#include <xlist.h>

//#define  DEBUG

#ifdef DEBUG
   #define DBG(fmt, args...)  printf("In %s :", __FUNCTION__); printf(fmt, ##args)
#else
   #define DBG(msg, ...)
#endif


/*  chunk status */
#define CHUNK_INUSE    0x00455355    // "USE"
#define CHUNK_FREE     0

typedef struct _xmalloc_chunk
{
    unsigned int      stat;          // INUSE or FREE
    unsigned int      size;          // malloc_size + sizeof(xmalloc_chunk)
    struct list_head  list;
}xmalloc_chunk;


/*  sizes, alignments */
#define SIZE_SZ                (sizeof(double))
#define MALLOC_ALIGNMENT       (SIZE_SZ + SIZE_SZ)
#define MALLOC_ALIGN_MASK      (MALLOC_ALIGNMENT - 1)
#define MINSIZE                (sizeof(xmalloc_chunk))

#define XMALLOC_MAGIC_NUM       0x20170101

typedef struct _xmalloc_info
{
    void *            malloc_start;
    void *            malloc_end;
    unsigned int      total_size;
    unsigned int      used_size;
    unsigned char     *ptr_max;       // use to dectect the max length of dmalloc
    unsigned int      init_flag;
    struct list_head  chunk_head;
}xmalloc_info;


static xmalloc_info mem_info;


void dmalloc_init(unsigned char *start, unsigned int size)
{
    struct list_head *head = &(mem_info.chunk_head);

    // init mem_info
    memset(&mem_info, 0, sizeof(mem_info));
    mem_info.malloc_start = start;
    mem_info.malloc_end   = start + size;
    mem_info.total_size   = size;
    mem_info.used_size    = 0;
    mem_info.init_flag    = XMALLOC_MAGIC_NUM;
    INIT_LIST_HEAD(head);
    printf("dmalloc addr : 0x%x, length : 0x%x\n", start, size);

    // init the first chunk
    xmalloc_chunk *pchunk  = (xmalloc_chunk *)start;
    pchunk->stat  = CHUNK_FREE;
    pchunk->size  = size;
    list_add(&(pchunk->list), head);
}


void* dmalloc(unsigned int size)
{
    struct list_head  *head;
    xmalloc_chunk     *pchunk, *pnew_chunk;
    unsigned int      alloc_size, remain_size;
    unsigned char     *pbuf, *pbase;

    if (mem_info.init_flag != XMALLOC_MAGIC_NUM)
    {
        printf("dmalloc do not inited !\n");
        return NULL;
    }

    head  = &(mem_info.chunk_head);
    alloc_size = size + sizeof(xmalloc_chunk);
    if ((alloc_size & MALLOC_ALIGN_MASK) != 0)
    {
        alloc_size += MALLOC_ALIGNMENT;
        alloc_size &= (~MALLOC_ALIGN_MASK);
    }
    DBG("alloc_size = 0x%x\n", alloc_size);

    list_for_each_entry(pchunk, head, list)
    {
        if ((pchunk->stat == CHUNK_FREE) && (pchunk->size >= alloc_size))
        {
            pchunk->stat = CHUNK_INUSE;
            pbuf = (unsigned char *)pchunk + sizeof(xmalloc_chunk);
            remain_size = pchunk->size - alloc_size;
            
            if (remain_size >= (MINSIZE*2))  
            {
                // split the space, else merge the remain size
                pchunk->size      = alloc_size;
                
                pbase             = (unsigned char *)pchunk + alloc_size;
                pnew_chunk        = (xmalloc_chunk *)pbase;
                pnew_chunk->stat  = CHUNK_FREE;
                pnew_chunk->size  = remain_size;
                list_add(&(pnew_chunk->list), &(pchunk->list));
            }
            
            // update mem_info.ptr_max
            pbase = (unsigned char *)pchunk + pchunk->size;
            if ((unsigned long)pbase > (unsigned long)mem_info.ptr_max)
            {
                mem_info.ptr_max = pbase;
            }
            
            return (void *)pbuf;
        }
    }
    DBG("malloc space failed !\n");
    return NULL;
}


void dfree(void *p)
{
    struct list_head  *head, *plist;
    xmalloc_chunk     *pchunk;
    xmalloc_chunk     *prev_chk, *next_chk;
    unsigned int      free_size;

    if (mem_info.init_flag != XMALLOC_MAGIC_NUM)
    {
        printf("dmalloc do not inited !\n");
        return;
    }
    if (p == NULL)
    {
        return;
    }

    head    = &(mem_info.chunk_head);
    pchunk  = (xmalloc_chunk *)((char *)p - sizeof(xmalloc_chunk));
    if (pchunk->stat != CHUNK_INUSE)
    {
        printf("\nxfree(%p) \033[31;1merror\033[0m, not a available chunk\n", p);
        return;
    }
    pchunk->stat  = CHUNK_FREE;

    // if "current_chunk and prev_chunk" ==> "prev_chunk"
    if (! list_is_first(&(pchunk->list), head) )
    {
        plist     = pchunk->list.prev;
        prev_chk  = container_of(plist, xmalloc_chunk, list);
        if (prev_chk->stat == CHUNK_FREE)
        {
            DBG("delete the list of current_chunk\n");
            free_size = pchunk->size;
            list_del(&(pchunk->list));
            prev_chk->size += free_size;
            pchunk = prev_chk;
        }
    }

    // if "current_chunk and next_chunk" ==> "current_chunk"
    if (! list_is_last(&(pchunk->list), head) )
    {
        plist     = pchunk->list.next;
        next_chk  = container_of(plist, xmalloc_chunk, list);
        if (next_chk->stat == CHUNK_FREE)
        {
            DBG("delete the list of next_chunk\n");
            free_size = next_chk->size;
            list_del(plist);
            pchunk->size += free_size;
        }
    }
}


//  The following function just for debug
unsigned int get_dmalloc_length(void)
{
    unsigned int len;
    len = (unsigned int)((char *)mem_info.ptr_max - (char *)mem_info.malloc_start);
    return len;
}


void dump_dmalloc_chunk(void)
{
    unsigned int  i;
    xmalloc_chunk *pchk;
    struct list_head  *head;

    if (mem_info.init_flag != XMALLOC_MAGIC_NUM)
    {
        printf("dmalloc do not inited !\n");
        return;
    }

    i  = 0;
    head  = &(mem_info.chunk_head);
    printf("\n");

    list_for_each_entry(pchk, head, list)
    {
        printf("######## chunk_%02d ########\n", i);
        printf("status  : %s\n", (pchk->stat == CHUNK_INUSE ? "in_use" : "free"));
        printf("size    : 0x%x\n", pchk->size);
        printf("ptr_dat : %p\n",  (char *)pchk + sizeof(*pchk));
        printf("\n");
        i = i + 1;
    }
}

