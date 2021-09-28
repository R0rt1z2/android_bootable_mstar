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
#include <common.h>
#include <malloc.h>

#define PAGE_SIZE	 4096

typedef unsigned int volatile ulv;
typedef unsigned long int volatile ullv;

#if CONFIG_MINIUBOOT
#else
int Test_MemBandWidth_int(ulv * pSrc, ulv * pDest, unsigned int nCount)
{
	ulv *p1 = NULL;
	ulv *p2 = NULL;
	unsigned int	i;
	unsigned int	nTest;

	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (ulv *) pSrc;
		p2 = (ulv *) pDest;

		for (i = 0; i < nCount; i++)
			*p2++ = *p1++;
	}

	return nTest;
}

int Test_MemBandWidth_long(ulv * pSrc, ulv * pDest, unsigned int nCount)
{
	ullv *p1 = NULL;
	ullv *p2 = NULL;
	unsigned int	i;
	unsigned int	nTest;

	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (ullv *) pSrc;
		p2 = (ullv *) pDest;

		for (i = 0; i < nCount; i++)
			*p2++ = *p1++;
	}

	return nTest;
}

int Test_Memcpy(ulv * pSrc, ulv * pDest, unsigned int nCount)
{
	int nTest;
	for (nTest = 0; nTest < 10; nTest++)
		memcpy((void*)pDest, (void*)pSrc, nCount*sizeof(unsigned int));
	return nTest;
}

int Test_Memread(ulv * pSrc, unsigned int nCount)
{
	register unsigned int val;
	ulv *p1 = NULL;
	unsigned int    nTest, i;
	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (ulv *) pSrc;
		for (i = 0; i < nCount; i++)
			val = *p1++;
	}
	return nTest;
}

int Test_Memwrite(ulv * pDest, unsigned int nCount)
{
	register unsigned int val = 0xA5A4B5B4;
	ulv *p2 = NULL;
	unsigned int    nTest, i;
	for (nTest = 0; nTest < 10; nTest++)
	{
		p2 = (ulv *) pDest;
		for (i = 0; i < nCount; i++)
			*p2++ = val;
	}
	return nTest;
}

int Test_MemBandWidthRW(ulv * pSrc, ulv * pDest, unsigned int nCount, unsigned int step_size)
{
	ulv *p1 = NULL;
	ulv *p2 = NULL;
	//unsigned int	i;
	unsigned int	nTest;
	int Count;

	for (nTest = 0; nTest < 10 * step_size; nTest++)
	{
        p1 = (ulv *) pSrc;
		p2 = (ulv *) pDest;
		Count = nCount / step_size;
        //memcpy((void*)p2, (void*)p1, nCount*4);
        while(Count--)
        {
            *p2 = *p1;
            p2 += step_size;
            p1 += step_size;
        }
	}

	return nTest;
}

int Test_MemBandWidthR(ulv * pSrc, ulv * pDest, unsigned int nCount, unsigned int step_size)
{
	ulv *p1 = NULL;
	ulv *p2 = NULL;
	//unsigned int	i;
	unsigned int	nTest;
	int Count;

	for (nTest = 0; nTest < 10 * step_size; nTest++)
	{
        p1 = (ulv *) pSrc;
		p2 = (ulv *) pDest;
		Count = nCount / step_size;
        //memcpy((void*)p2, (void*)p1, nCount*4);
        while(Count--)
        {
            *p2 = *p1;
            p1 += step_size;
        }
	}

	return nTest;
}

int Test_MemBandWidthW(ulv * pSrc, ulv * pDest, unsigned int nCount, unsigned int step_size)
{
	ulv *p1 = NULL;
	ulv *p2 = NULL;
	//unsigned int	i;
	unsigned int	nTest;
	int Count;

	for (nTest = 0; nTest < 10 * step_size; nTest++)
	{
        p1 = (ulv *) pSrc;
		p2 = (ulv *) pDest;
		Count = nCount / step_size;
        //memcpy((void*)p2, (void*)p1, nCount*4);
        while(Count--)
        {
            *p2 = *p1;
            p2 += step_size;
        }
	}

	return nTest;
}

/*Unit is 1MB*/
void MemPerformance(unsigned int uMemSize)
{
	unsigned int	nLoop = 0;
	unsigned int	nAllocBytes;
	unsigned int	nBufSize;
	unsigned int	nCount;
	unsigned int	PAGE_MASK;
	void volatile	* pBuf = NULL;
	void volatile	* pAlignedBuf = NULL;
	ulv				* pSrc;
	ulv				* pDest;
	int				nDelay;
	int				nTestCount = 0;
	int				nSize;
    ulong           pasttime;
    ulong           currtime;

	PAGE_MASK = (unsigned int) ~(PAGE_SIZE - 1);
	nBufSize	= (unsigned int) (uMemSize << 20);
	nAllocBytes = nBufSize + PAGE_SIZE;


	pBuf = (void volatile *) malloc(nAllocBytes);
	if (!pBuf)
	{
		printf(" Error :: malloc failed\n");
		return;
	}

	printf(" Allocated %d bytes at 0x%08x\n", nAllocBytes, (unsigned int) pBuf);
	if ((unsigned int) pBuf % PAGE_SIZE)
	{
		pAlignedBuf = (void volatile *) (((unsigned int) pBuf + PAGE_SIZE) & PAGE_MASK);
		printf(" Aligned at 0x%08x\n", (unsigned int) pAlignedBuf);
	}
	else
	{
		pAlignedBuf = pBuf;
	}

	nCount = (nBufSize / 2) / sizeof(unsigned int);

	pSrc = (ulv *) pAlignedBuf;
	pDest = (ulv *) ((unsigned int) pAlignedBuf + (nBufSize / 2));

	printf(" Read from : 0x%p\n", pSrc);
	printf(" Write to  : 0x%p\n", pDest);

	nSize = nCount * sizeof(unsigned int);

	printf(" Size : 0x%x\n", nSize);

	printf("\nMemory read/write test\n");
	nLoop  = 0;
	int i = 0;
    
    /*Memory performance test item 1 */
	printf("\n(1) Memory read/write test through 16-bit pointer access\n");
    pasttime = get_timer(0);
    nTestCount = Test_MemBandWidth_int(pSrc, pDest, nCount);
    currtime = get_timer(0);
    nDelay = ((currtime - pasttime)/12) / 1000;
	printf("Read/Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
        nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);

    /*Memory performance test item 2 */
	printf("\n(2) Memory read/write test through 32-bit pointer access\n");
    pasttime = get_timer(0);
    nTestCount = Test_MemBandWidth_long(pSrc, pDest, nCount);
    currtime = get_timer(0);
    nDelay = ((currtime - pasttime)/12) / 1000;
    printf("Read/Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
        nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);

    /*Memory performance test item 3 */
	printf("\n(3) Memory read/write test through memcpy()\n");
    pasttime = get_timer(0);
    nTestCount = Test_Memcpy(pSrc, pDest, nCount);
    currtime = get_timer(0);
    nDelay = ((currtime - pasttime)/12) / 1000;
    printf("Read/Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
        nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);

    /*Memory performance test item 4 */
	printf("\n(4) Memory read test\n");
    pasttime = get_timer(0);
    nTestCount = Test_Memread(pSrc, nCount);
    currtime = get_timer(0);
    nDelay = ((currtime - pasttime)/12) / 1000;
    printf("Read/Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
        nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);

    /*Memory performance test item 5 */
	printf("\n(5) Memory write test\n");
    pasttime = get_timer(0);
    nTestCount = Test_Memwrite(pDest, nCount);
    currtime = get_timer(0);
    nDelay = ((currtime - pasttime)/12) / 1000;
    printf("Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
        nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);

    /*Memory performance test item 6 */
	printf("\n(6) Memory read/write test\n");
    for(i = 1 ; i < 513; i = i<<1)
    {
        pasttime = get_timer(0);
        nTestCount = Test_MemBandWidthRW(pSrc, pDest, nCount, i);
        currtime = get_timer(0);
        nDelay = ((currtime - pasttime)/12) / 1000;
        printf("Read/Write  %8d bytes, skip %4d bytes %4d msec => %6d KB/sec\n",
            nSize, i*4, nDelay, ((((nSize / i) * nTestCount) / 1024) * 1000) / nDelay);
    }

    /*Memory performance test item 7 */
	printf("\n(7) Memory read test\n");
    for(i = 1 ; i < 513; i = i<<1)
    {
        pasttime = get_timer(0);
        nTestCount = Test_MemBandWidthR(pSrc, pDest, nCount, i);
        currtime = get_timer(0);
        nDelay = ((currtime - pasttime)/12) / 1000;
        printf("Read  %8d bytes, skip %4d bytes %4d msec => %6d KB/sec\n",
            nSize, i*4, nDelay, ((((nSize / i) * nTestCount) / 1024) * 1000) / nDelay);
    }

    /*Memory performance test item 8 */
	printf("\n(8) Memory write test\n");
    for(i = 1 ; i < 513; i = i<<1)
    {
        pasttime = get_timer(0);
        nTestCount = Test_MemBandWidthW(pSrc, pDest, nCount, i);
        currtime = get_timer(0);
        nDelay = ((currtime - pasttime)/12) / 1000;
        printf("Write  %8d bytes, skip %4d bytes %4d msec => %6d KB/sec\n",
            nSize, i*4, nDelay, ((((nSize / i) * nTestCount) / 1024) * 1000) / nDelay);
    }

	free((void *) pBuf);
}
#endif

