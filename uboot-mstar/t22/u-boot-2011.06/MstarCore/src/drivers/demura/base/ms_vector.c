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
/*
 * For the coversion of vendor_to_mstar binary needs lots of memory,
 * but the malloc buffer of mboot is about 10MB .
 * No need to expand the malloc buffer, for we had large unused memory
 * during the mboot stage. So just use it.
*/
#include <MsTypes.h>
#include <stdio.h>
#include <linux/string.h>
#include <dmalloc.h>
#include <ms_vector.h>


MS_BOOL alloc_vector(int item_size, int max_num, m_vector *pvec)
{
    unsigned char *pbuf;
    unsigned int size = item_size * max_num;

    pbuf = dmalloc(size);
    if (pbuf == NULL)
    {
        printf("in %s, dmalloc space failed !\n", __FUNCTION__);
        return FALSE;
    }
    pvec->item_size = item_size;
    pvec->max_num   = max_num;
    pvec->pbuf      = pbuf;
    pvec->dat_num   = 0;

    return TRUE;
}


void  free_vector(m_vector *ptr)
{
    dfree(ptr->pbuf);
}


void clear_vector(m_vector *ptr)
{
    ptr->dat_num = 0;
}


MS_BOOL merge_vector(m_vector *dest_vec, m_vector *src_vec)
{
    char *pdest, *psrc;
    MS_U32 cont_size;
    
    if (dest_vec->item_size != src_vec->item_size)
    {
        printf("merge_vector confilct!\n");
        printf("dest_vec->item_size = 0x%x\n", dest_vec->item_size);
        printf("src_vec->item_size  = 0x%x\n", src_vec->item_size);
        return FALSE;
    }
    if ((dest_vec->dat_num + src_vec->dat_num) > dest_vec->max_num)
    {
        printf("dest vector do not have enough space!\n");
        printf("dest_vec->dat_num + src_vec->dat_num = 0x%x\n", (dest_vec->dat_num + src_vec->dat_num));
        printf("dest_vec->max_num  = 0x%x\n", dest_vec->max_num);
        return FALSE;
    }
    
    psrc      = (char *)(src_vec->pbuf);
    cont_size = src_vec->dat_num * src_vec->item_size;
    pdest     = (char *)(dest_vec->pbuf) + dest_vec->dat_num * dest_vec->item_size;
    memcpy(pdest, psrc, cont_size);
    
    dest_vec->dat_num  +=  src_vec->dat_num;
    return TRUE;
}

