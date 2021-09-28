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

#include <stdlib.h>
#include <string.h>
#include "datatype.h"
#include "ms_decompress.h"
#include "ms_decompress_priv.h"

#define _ReadChar(A)\
{\
    A=(*pInStream << (8 - u32BitPos))|(*(pInStream+1)>>u32BitPos);\
    pInStream++;\
}

#define _ReadBit(A)\
{\
    u32BitPos--;\
    A = (*pInStream >> u32BitPos) & 0x01;\
    if(u32BitPos == 0)\
    {\
        u32BitPos = 8;\
        pInStream++;\
    }\
}
static U8 *pSlidingWindow;
static U32 _u32BitPos = 0;
static U32 u32NextChar = 0;
static U8 *pOutStream;
static U32 _u32Offset = 0;

BOOLEAN ms_DecompressInit(U8 *pOut)
{
    pSlidingWindow = malloc(WINDOW_SIZE);
    if(pSlidingWindow == NULL)
    {
        return FALSE;
    }
    else
    {
        memset(pSlidingWindow, 0, WINDOW_SIZE);
        _u32BitPos = 8;
        u32NextChar = 1;
        pOutStream = pOut;
        _u32Offset = 0;
        return TRUE;
    }
}

void ms_DecompressDeInit(void)
{
    free(pSlidingWindow);
}

int ms_Decompress(const U8 *pInStream, U32 u32DataLen)
{
    register U32 i ;
    register U32 offset,length;
    register U32 mask=0xFFF;
    register U32 u32BitPos = 0;
    register const U8 *pStreamEnd = NULL;
    register U32 tmp0,tmp1;
	const U8 *pOutHead = pOutStream;

    //pSlidingWindow = malloc(WINDOW_SIZE);
    if (pSlidingWindow)
    {

        u32BitPos = _u32BitPos;
        pInStream = pInStream;
        pStreamEnd = pInStream + u32DataLen;

        pInStream += _u32Offset;
        //pInStream = pInStream;
        //printf("I-pOutStream = %d \n",pOutStream);

        while (pInStream <= pStreamEnd)
        {
            _ReadBit(tmp0);
            if (tmp0 != UNCODED)
            {
                _ReadChar(tmp0);
                _ReadChar(tmp1);
                offset = (tmp0 << 8) | tmp1;
                length = (offset & 0x0F) + 2;
                offset >>= 4;
                if ((u32NextChar+length) < WINDOW_SIZE)
                {
                    for (i = 0; i < length; i++)
                    {
                        *pOutStream = pSlidingWindow[(offset+i) & mask];
                        pSlidingWindow[(u32NextChar+i)] = *pOutStream++;
                    }
                }
                else
                {
                    for (i = 0; i < length; i++)
                    {
                        *pOutStream = pSlidingWindow[(offset+i) & mask];
                        pSlidingWindow[(u32NextChar+i) & mask] = *pOutStream++;
                    }
                }
                u32NextChar+=length;
            }
            else
            {
                _ReadChar(tmp0);
                *pOutStream = tmp0;
                pSlidingWindow[u32NextChar] = *pOutStream++;
                u32NextChar++;
            }
            u32NextChar = u32NextChar & mask;
        }

    }
    _u32BitPos = u32BitPos;
    _u32Offset = pInStream - pStreamEnd;
	return (pOutStream - pOutHead);
}

//*************************************************************************************************

#include "zlib.h"


static unsigned char *pDecompressionBuf = NULL;
static unsigned char *pEndofDecompressionBuf = NULL;
static unsigned char *pCurrentBufPtr = NULL;

static void MSDecompress_MemoryAllocatorInit(unsigned char *ptr, unsigned long nSize);
void *zalloc(void *ptr, unsigned long nNum, unsigned long nSize);
void zfree(void *x, void *address, unsigned nbytes);
static U8 MsDecompress(U8 *pSrc, U8 *pDst, U32 srclen, U32 dstlen, U32 *pDecompressionLength);

static void MSDecompress_MemoryAllocatorInit(unsigned char *ptr, unsigned long nSize)
{
   pDecompressionBuf = ptr;
   pEndofDecompressionBuf = pDecompressionBuf + nSize*sizeof(unsigned char);
   pCurrentBufPtr = pDecompressionBuf;
}

void *zalloc(void *ptr, unsigned long nNum, unsigned long nSize)
{
// Because C language use call by value
// so if we modify ptr value, it will not afftect the caller value
// Considering decompression performance, we don't do memory pool
// region check here
#if 0
    nSize = ( nSize*nNum + 3 ) & ~3;

    ptr = (void *)pCurrentBufPtr;
    pCurrentBufPtr += nSize;

    if (pCurrentBufPtr > pEndofDecompressionBuf)
    {
        printf("Memory Allocate Fail\n");
        ptr = NULL;
    }
#else
    ptr = (void *)pCurrentBufPtr;
    // 4' alignment
    pCurrentBufPtr += ( nSize*nNum + 3 ) & ~3;
#endif
    return ptr;
}

void zfree(void *x, void *address, unsigned nbytes)
{
    // Considering decompression performance, we don't
    // do memory free operation here
    // This will cause a lot of memory usage, but I think we can
    // afford this, because our target files are not so big
    x = x;
    address = address;
    nbytes = nbytes;
}

static U8 MsDecompress(U8 *pSrc, U8 *pDst, U32 srclen, U32 dstlen, U32 *pDecompressionLength)
{
    z_stream z;

    z.zalloc = (alloc_func)zalloc;
    z.zfree = zfree;
    z.next_in = pSrc;
    z.avail_in = srclen;
    z.next_out = pDst;
    z.avail_out = dstlen;

    if ( inflateInit2(&z, -MAX_WBITS) != Z_OK )
    {
        *((U32 *)(0xbf206700)) = 0xDEAD;
        printf("Z Error:%s\n", z.msg);
        inflateEnd(&z);
        return 1;
    }

    if ( inflate(&z, Z_FINISH) < Z_OK )
    {
        *((U32 *)(0xbf206704)) = 0xBEEF;
        printf("Z Error:%s\n", z.msg);
        inflateEnd(&z);
        return 1;
    }

    *pDecompressionLength = z.total_out;

    inflateEnd(&z);

    return 0;
}

U8 g_MsDecomp_u8Debug = 0;
U8 g_MsDecomp_u8CheckTempSize = 0;
U32 g_MsDecomp_u32TempSize = 0;

BOOLEAN DoMsDecompression(U8 *pSrc, U8 *pDst, U8 *pTmp, U32 srclen)
{
    U32 nOutputLength = 0, OriginalFileLength = 0;

    //MSDecompress_MemoryAllocatorInit((U8*)(pSrc+BUF_OFFSET_MALLOC_BUF), MEMORY_POOL_SIZE);
    MSDecompress_MemoryAllocatorInit(pTmp, MEMORY_POOL_SIZE);  //replace (pSrc+BUF_OFFSET_MALLOC_BUF) with pTmp

    OriginalFileLength = *((U8 *)(pSrc+srclen-4+3));
    OriginalFileLength <<= 8;
    OriginalFileLength |= *((U8 *)(pSrc+srclen-4+2));
    OriginalFileLength <<= 8;
    OriginalFileLength |= *((U8 *)(pSrc+srclen-4+1));
    OriginalFileLength <<= 8;
    OriginalFileLength |= *((U8 *)(pSrc+srclen-4+0));

    if ( MsDecompress(pSrc, pDst, srclen-4, OriginalFileLength, &nOutputLength) )
    {
        return FALSE;
    }

    if( g_MsDecomp_u8Debug&1 )
    {
        printf("Decomp info\n");
        printf("srclen=0x%X\n", (unsigned int)srclen);
        printf("OriginalFileLength=0x%X\n", (unsigned int)OriginalFileLength);

        printf("pDecompressionBuf=0x%X\n", (unsigned int)pDecompressionBuf);
        printf("pEndofDecompressionBuf=0x%X\n", (unsigned int)pEndofDecompressionBuf);
        printf("=> pCurrentBufPtr=0x%X, use 0x%X\n\n", (unsigned int)pCurrentBufPtr,
                    (unsigned int)(pCurrentBufPtr-pDecompressionBuf) );
    }

    if(g_MsDecomp_u8CheckTempSize)
    {
        if( (pCurrentBufPtr-pDecompressionBuf) > g_MsDecomp_u32TempSize )
        {
            printf("\nWarning: DeComp use buf size 0x%x > 0x%ld  !!!!\n", (unsigned int)(pCurrentBufPtr-pDecompressionBuf),g_MsDecomp_u32TempSize);
        }
        g_MsDecomp_u8CheckTempSize = 0;
        g_MsDecomp_u32TempSize = 0;
    }

    if ( nOutputLength != OriginalFileLength )
    {
        //Decompression fail!
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* 2009-08-14 : Igor Pavlov : Public domain */

#include "7types.h"
#include "7alloc.h"
#include "lzmadec.h"

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };


U8 g_MsDecomp7_u8Debug = 0;

BOOLEAN DoMsDecompression7(U8 *pSrc, U8 *pDst, U8 *pTmp, U32 srclen)
{
    U32 OriginalFileLength = 0;
	int result = SZ_OK;
	U8 i;
    ELzmaStatus estatus;
    SizeT destLen, srcLen;

    #define LZMA_PROPS_SIZE 5

    //MSDecompress_MemoryAllocatorInit((U8*)(pSrc+BUF_OFFSET_MALLOC_BUF), MEMORY_POOL_SIZE);
    MSDecompress_MemoryAllocatorInit(pTmp, MEMORY_POOL_SIZE);  //replace (pSrc+BUF_OFFSET_MALLOC_BUF) with pTmp

	/* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    srclen -= (LZMA_PROPS_SIZE + 8);

	// It seems that in our system, we never have a chance to have a compressed data which needs to use U64 integer to store its file size
	// So we use just U32 here to instead of U64
	/* Read and parse header */
    for (i = 0; i < 8; i++)
        OriginalFileLength += ( (U32)pSrc[LZMA_PROPS_SIZE + i] << (i << 3) );

	destLen = OriginalFileLength;
	srcLen = srclen;

    result = LzmaDecode(pDst, &destLen, pSrc + (LZMA_PROPS_SIZE + 8), &srcLen, pSrc, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &estatus, &g_Alloc);

    if( g_MsDecomp7_u8Debug&1 )
    {
        printf("Decomp7 info\n");
        printf("srclen=0x%X\n", (unsigned int)srclen);
        printf("OriginalFileLength=0x%X\n", (unsigned int)OriginalFileLength);

        printf("pDecompressionBuf=0x%X\n", (unsigned int)pDecompressionBuf);
        printf("pEndofDecompressionBuf=0x%X\n", (unsigned int)pEndofDecompressionBuf);
        printf("=> pCurrentBufPtr=0x%X, use 0x%X\n\n", (unsigned int)pCurrentBufPtr,
                    (unsigned int)(pCurrentBufPtr-pDecompressionBuf) );
    }

    if( (pCurrentBufPtr-pDecompressionBuf) > 0x4000 )
    {
        printf("\nWarning: DeComp7 use buf size > 0x4000!!!!\n");
    }

    switch( estatus )
    {
    case LZMA_STATUS_FINISHED_WITH_MARK:
    case LZMA_STATUS_NOT_FINISHED:
    case LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK:
        break;
    default:
    //LZMA_STATUS_NOT_SPECIFIED,               /* use main error code instead */
    //LZMA_STATUS_NEEDS_MORE_INPUT,            /* you must provide more input bytes */
        printf("ELzmaStatus = %d\n", estatus);
        //Decompression fail!
        return FALSE;
    }

    if (result != SZ_OK)
    {
    //  SZ_ERROR_DATA - Data error
    //  SZ_ERROR_MEM  - Memory allocation error
    //  SZ_ERROR_UNSUPPORTED - Unsupported properties
    //  SZ_ERROR_INPUT_EOF - It needs more bytes in input buffer (src).
    //#define SZ_ERROR_DATA 1
    //#define SZ_ERROR_MEM 2
    //#define SZ_ERROR_CRC 3
    //#define SZ_ERROR_UNSUPPORTED 4
    //#define SZ_ERROR_PARAM 5
    //#define SZ_ERROR_INPUT_EOF 6
    //#define SZ_ERROR_OUTPUT_EOF 7
    //#define SZ_ERROR_READ 8
    //#define SZ_ERROR_WRITE 9
    //#define SZ_ERROR_PROGRESS 10
    //#define SZ_ERROR_FAIL 11
    //#define SZ_ERROR_THREAD 12
    //#define SZ_ERROR_ARCHIVE 16
    //#define SZ_ERROR_NO_ARCHIVE 17
        printf("error code:%d\n", result);
        //Decompression fail!
		return FALSE;
    }

	if ( destLen != OriginalFileLength )
	{
        //Decompression fail!
		return FALSE;
	}

	if ( srcLen != srclen )
	{
        //Decompression fail!
		return FALSE;
	}

	//Decompression OK!
	return TRUE;
}
