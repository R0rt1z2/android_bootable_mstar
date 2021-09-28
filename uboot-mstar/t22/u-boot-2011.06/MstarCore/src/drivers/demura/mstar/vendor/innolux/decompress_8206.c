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
#include <MsTypes.h>
#include "CommonDataType.h"
#include <math.h>
#include "bits_stream_reader.h"
#include "dmalloc.h"
#include "ms_array.h"
#include "decompress_8206.h"

//define DBG_MODE 1
#ifdef DBG_MODE
    #define DBG_PRINTF(msg...)    printf(msg);
#else
    #define DBG_PRINTF(msg...)
#endif

static int AdjustWth(int lutW, int Divisor)
{
    int Q = lutW / Divisor;
    int R = lutW % Divisor;
    int W = 0;

    if (R != 0)
        W = Divisor * (Q + 1);
    else
        W = Divisor * Q;

    return W;
}


static void Decode_AQC(DeCmpxInfo *pDeCmpx, int **Code, int **DeLut)
{
    int i, j, k;
    int H = pDeCmpx->Height;
    int W16 = pDeCmpx->Width16;

    for (i = 0; i < H; i++)
    {
        for (j = 0; j < (W16 / 16); j++)
        { 
            double Min   = (double) Code[i * 31 + j][21];
            double Range = (double) Code[i * 31 + j][20];
            double M, N, Q;
            double subRange1 = round((Range * 1 / 4) + Min);
            double subRange2 = round((Range * 2 / 4) + Min);
            double subRange3 = round((Range * 3 / 4) + Min);

            for (k = 0; k < 4; k++)
            {
                int Data[4];
                int Mode = Code[i * 31 + j][16 + k];
                int m;

                for (m = 0; m < 4; m++)
                {
                    Data[m] = Code[i * 31 + j][k * 4 + m];
                }

                switch (Mode)
                {
                    case 0:
                        M = Min+Range;
                        N = Min;
                        break;

                    case 1:
                        M = subRange3;
                        N = Min;
                        break;

                    case 2:
                        M = subRange2;
                        N = Min;
                        break;

                    case 3:
                        M = subRange1;
                        N = Min;
                        break;

                    case 4:
                        M = Min + Range;
                        N = subRange1;
                        break;

                    case 5:
                        M = Min + Range;
                        N = subRange2;
                        break;

                    case 6:
                        M = Min + Range;
                        N = subRange3;
                        break;

                    case 7:
                        M = subRange3;
                        N = subRange1;
                        break;

                    default:
                        M = Min + Range;
                        N = Min;
                        break;
                }
                Q = M - N;

                for (m = 0; m < 4; m++)
                {
                    if (Q != 0)
                        DeLut[i][j*16 + k*4 + m] = (int) round(((double)Data[m] * 546 / 8192) * Q + N);
                    else
                        DeLut[i][j * 16 + k * 4 + m] = (int)N;
                }
            }
        }
    }
}


MS_BOOL DeCmpx(DeCmpxInfo *pDeCmpx)
{
    int ret = 0;
    MS_BOOL bRet = TRUE;
    int W   = pDeCmpx->Width;
    int H   = pDeCmpx->Height;

    int W16  = AdjustWth(W, 16);
    int W256 = AdjustWth(W16 * 90 * 5 / 16, 256); // per row with 5 planes, 14080 bit
    int CodeBookSize  = (W16/16)*H;

    DBG_PRINTF("W = %d, H = %d, W16 = %d, W256 = %d\n", W, H, W16, W256);

    pDeCmpx->Width16      = W16;
    pDeCmpx->Width256     = W256;
    pDeCmpx->CodeBookSize = CodeBookSize;

    int NumPerRow = 22 * (W16 / 16) * 5;
    DEFINE_ARRAY_DIM2(_DataByRow, sizeof(int), 271, NumPerRow);
    int **DataByRow = (int **)PTR_ARRAY_DIM2(_DataByRow);
    //int BytePerRow = W256/8; // 1760 byte

    DEFINE_ARRAY_DIM2(_CodeLut1, sizeof(int), CodeBookSize, 22);
    DEFINE_ARRAY_DIM2(_CodeLut2, sizeof(int), CodeBookSize, 22);
    DEFINE_ARRAY_DIM2(_CodeLut3, sizeof(int), CodeBookSize, 22);
    DEFINE_ARRAY_DIM2(_CodeLut4, sizeof(int), CodeBookSize, 22);
    DEFINE_ARRAY_DIM2(_CodeLut5, sizeof(int), CodeBookSize, 22);

    int ** CodeLut1 = (int **)PTR_ARRAY_DIM2(_CodeLut1);
    int ** CodeLut2 = (int **)PTR_ARRAY_DIM2(_CodeLut2);
    int ** CodeLut3 = (int **)PTR_ARRAY_DIM2(_CodeLut3);
    int ** CodeLut4 = (int **)PTR_ARRAY_DIM2(_CodeLut4);
    int ** CodeLut5 = (int **)PTR_ARRAY_DIM2(_CodeLut5);

    DEFINE_ARRAY_DIM2(_DeLut1, sizeof(int), H, W16);
    DEFINE_ARRAY_DIM2(_DeLut2, sizeof(int), H, W16);
    DEFINE_ARRAY_DIM2(_DeLut3, sizeof(int), H, W16);
    DEFINE_ARRAY_DIM2(_DeLut4, sizeof(int), H, W16);
    DEFINE_ARRAY_DIM2(_DeLut5, sizeof(int), H, W16);

    int **DeLut1 = (int **)PTR_ARRAY_DIM2(_DeLut1);
    int **DeLut2 = (int **)PTR_ARRAY_DIM2(_DeLut2);
    int **DeLut3 = (int **)PTR_ARRAY_DIM2(_DeLut3);
    int **DeLut4 = (int **)PTR_ARRAY_DIM2(_DeLut4);
    int **DeLut5 = (int **)PTR_ARRAY_DIM2(_DeLut5);

    BitsStreamReader bsr;
    ret = Init_BSReader(&bsr, pDeCmpx->ByteCmpxIn, pDeCmpx->CmpxSize);
    if (ret != 0)
    {
        goto done;
    }

    int i, j, k, m;
    for (i = 0; i < H; i++)
    {
        for (j = 0; j < 31; j++)
        {
            for(k = 0 ; k < 5; k++)
            {
                for(m = 0; m<16; m++)
                {
                    DataByRow[i][j * 5 * 22 + k * 22 + m] = BSR_ReadData(&bsr, 4);
                }
                for (m = 16; m < 20; m++)
                {
                    DataByRow[i][j * 5 * 22 + k * 22 + m] = BSR_ReadData(&bsr, 3);
                }
                for (m = 20; m < 22; m++)
                {
                    DataByRow[i][j * 5 * 22 + k * 22 + m] = BSR_ReadData(&bsr, 7);
                }
            }
        }
        BSR_ShiftBits(&bsr,130);
    }

    for (i = 0; i < H; i++)
    {
        for (j = 0; j < (W16 / 16); j++)
        { 
            for(k = 0; k< 22; k++)
            {
                CodeLut1[i * (W16 / 16) + j][k] = DataByRow[i][j * 5 * 22 + k];
                CodeLut2[i * (W16 / 16) + j][k] = DataByRow[i][22 * (j * 5 + 1) + k];
                CodeLut3[i * (W16 / 16) + j][k] = DataByRow[i][22 * (j * 5 + 2) + k];
                CodeLut4[i * (W16 / 16) + j][k] = DataByRow[i][22 * (j * 5 + 3) + k];
                CodeLut5[i * (W16 / 16) + j][k] = DataByRow[i][22 * (j * 5 + 4) + k];
            }
        }
    }
    

    Decode_AQC(pDeCmpx, CodeLut1, DeLut1);
    Decode_AQC(pDeCmpx, CodeLut2, DeLut2);
    Decode_AQC(pDeCmpx, CodeLut3, DeLut3);
    Decode_AQC(pDeCmpx, CodeLut4, DeLut4);
    Decode_AQC(pDeCmpx, CodeLut5, DeLut5);

    for (i = 0; i < H; i++)
    {
        for (j = 0; j < W; j++)
        {
            #define LUT_OUT(index)  ((pDeCmpx->LutOut)[index - 1])
        
            LUT_OUT(1)[i][j] = DeLut1[i][j] - 64;
            LUT_OUT(2)[i][j] = DeLut2[i][j] - 64;
            LUT_OUT(3)[i][j] = DeLut3[i][j] - 64;
            LUT_OUT(4)[i][j] = DeLut4[i][j] - 64;
            LUT_OUT(5)[i][j] = DeLut5[i][j] - 64;
        }
    }

done:
    FREE_ARRAY_DIM2(_DataByRow);
    FREE_ARRAY_DIM2(_CodeLut1);
    FREE_ARRAY_DIM2(_CodeLut2);
    FREE_ARRAY_DIM2(_CodeLut3);
    FREE_ARRAY_DIM2(_CodeLut4);
    FREE_ARRAY_DIM2(_CodeLut5);
    FREE_ARRAY_DIM2(_DeLut1);
    FREE_ARRAY_DIM2(_DeLut2);
    FREE_ARRAY_DIM2(_DeLut3);
    FREE_ARRAY_DIM2(_DeLut4);
    FREE_ARRAY_DIM2(_DeLut5);
    return bRet;
}

