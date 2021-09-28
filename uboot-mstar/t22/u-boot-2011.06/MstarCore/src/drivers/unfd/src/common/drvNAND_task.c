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
#include "drvNAND.h"
#include "drvNAND_utl.h"

#if defined(__VER_UNFD_FTL__)&&__VER_UNFD_FTL__


struct unfd_task_queue {
	unsigned int		flags;
	void			*data;
} unfd_tq;

void drvNAND_BGThread_Ex(void)
{
	static int all_flag=0;
    int cur_flag;

    cur_flag = unfd_tq.flags & ~all_flag;
    all_flag |= unfd_tq.flags;
    
	#if UNFD_FTL_WL
	if(cur_flag & NAND_MSG_WEAR_LEVELING)
	{
		nand_Wear_Leveling();
		all_flag &= ~NAND_MSG_WEAR_LEVELING;
	}

	if(cur_flag & NAND_MSG_WEAR_LEVELING1)
	{
		nand_Wear_Leveling1();
        all_flag &= ~NAND_MSG_WEAR_LEVELING1;
	}
	#endif
	if(cur_flag & NAND_MSG_FORCE_WRITE_BACK)
	{
		NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
		U8  u8_i;

		#if UNFD_FTL_WL
        for (u8_i=0 ; u8_i<(pNandDrv->u8_Zone1SubZoneCnt+1); u8_i++)
		{
			if(pNandDrv->au32_ZoneTotalECnt[u8_i] > WL_SAVE_EC_TIMES)
				nand_SaveEraseCounter(u8_i);
		}
		#endif

        for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
		{
			nand_FlushWBQ(u8_i);
		}

        all_flag &= ~NAND_MSG_FORCE_WRITE_BACK;
	}
	
}


void drvNAND_BGThread(int flag)
{
    //=========================
    // [if kernel]: down(&PfModeSem);
    //   need a semaphore to lock here
    // [or MBoot]:
    #if defined(NAND_DRV_TV_MBOOT) && NAND_DRV_TV_MBOOT
    if(unfd_tq.flags)
        return;
    #endif
    //=========================
    
	unfd_tq.flags = flag;
    drvNAND_BGThread_Ex();

/*
	if(flag & NAND_MSG_WEAR_LEVELING)
	{
		nand_Wear_Leveling();
	}

	if(flag & NAND_MSG_WEAR_LEVELING1)
	{
		nand_Wear_Leveling1();
	}

	if(flag & NAND_MSG_FORCE_WRITE_BACK)
	{
		NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
		U8  u8_i;

        for (u8_i=0 ; u8_i<(pNandDrv->u8_Zone1SubZoneCnt+1); u8_i++)
		{
			if(pNandDrv->au32_ZoneTotalECnt[u8_i] > WL_SAVE_EC_TIMES)
				nand_SaveEraseCounter(u8_i);
		}

        for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
		{
			nand_FlushWBQ(u8_i);
		}

	}
*/

    //=========================
    #if defined(NAND_DRV_TV_MBOOT) && NAND_DRV_TV_MBOOT
    unfd_tq.flags = 0;
    #endif
    //up(&PfModeSem);
    //=========================
}


int drvNAND_BGThread_Init(void)
{
	return 0;
}
#endif
