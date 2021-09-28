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
#include <stdio.h>
#include <linux/string.h>
#include <dmalloc.h>
#include <MsTypes.h>
#include "ms_array.h"

MS_BOOL  malloc_array_dim2(struct array_dim2 *p, int item_size, int dim1_num, int dim2_num)
{
    memset(p, 0, sizeof(*p));
    p->item_size = item_size;
    p->dim1_num  = dim1_num;
    p->dim2_num  = dim2_num;

    p->dim1_buf = (void **)dmalloc(sizeof(char **) * dim1_num);
    CHECK_DMALLOC_SPACE(p->dim1_buf, (unsigned int)(sizeof(char **) * dim1_num));

    p->dim2_buf = (void *)dmalloc(item_size * dim1_num * dim2_num);
    CHECK_DMALLOC_SPACE(p->dim2_buf, (unsigned int)(item_size * dim1_num * dim2_num));

    int i;
    for (i = 0; i < dim1_num; i++)
    {
        p->dim1_buf[i] = (void *)((size_t)(p->dim2_buf) + dim2_num * item_size * i);
    }
    return TRUE;
}


void free_array_dim2(struct array_dim2 *p)
{
    dfree(p->dim2_buf);
    dfree(p->dim1_buf);
    p->dim1_buf = NULL;
    p->dim2_buf = NULL;
}


MS_BOOL  malloc_array_dim4(struct array_dim4 *p, int item_size, int dim1, int dim2, int dim3, int dim4)
{
    memset(p, 0, sizeof(*p));
    p->item_size = item_size;
    p->dim1_num  = dim1;
    p->dim2_num  = dim2;
    p->dim3_num  = dim3;
    p->dim4_num  = dim4;

    p->dim1_buf = (void ****)dmalloc(sizeof(char *) * dim1);
    CHECK_DMALLOC_SPACE(p->dim1_buf, (unsigned int)(sizeof(char *) * dim1));

    p->dim2_buf = (void ***)dmalloc(sizeof(char *) * dim1 * dim2);
    CHECK_DMALLOC_SPACE(p->dim2_buf, (unsigned int)(sizeof(char *) * dim1 * dim2));

    p->dim3_buf = (void **)dmalloc(sizeof(char *) * dim1 * dim2 * dim3);
    CHECK_DMALLOC_SPACE(p->dim3_buf, (unsigned int)(sizeof(char *) * dim1 * dim2 * dim3));

    p->dim4_buf = (void *)dmalloc(item_size * dim1 * dim2 * dim3 * dim4);
    CHECK_DMALLOC_SPACE(p->dim4_buf, (unsigned int)(item_size * dim1 * dim2 * dim3 * dim4));

    int i, j, k;
    for (i = 0; i < dim1; i++)
    {
        p->dim1_buf[i] = (void ***)((size_t)(p->dim2_buf) +  sizeof(char *) * i * dim2);
        for (j = 0; j < dim2; j++)
        {
            p->dim1_buf[i][j] = (void **)((size_t)(p->dim3_buf) + sizeof(char *) * ((i * dim2 + j) *dim3));
            for (k = 0; k < dim3; k++)
            {
                p->dim1_buf[i][j][k] = (void *)((size_t)(p->dim4_buf) + item_size * (((i * dim2 + j)* dim3 + k) * dim4));
            }
        }
    }

    return TRUE;
}


void free_array_dim4(struct array_dim4 *p)
{
    dfree(p->dim4_buf);
    dfree(p->dim3_buf);
    dfree(p->dim2_buf);
    dfree(p->dim1_buf);
    p->dim1_buf = NULL;
    p->dim2_buf = NULL;
    p->dim3_buf = NULL;
    p->dim4_buf = NULL;
}
