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
#include <mstar_mbx_head_general.h>
mbx_smc_args *Sec_ARM_MBX_Base = NULL;

TEE_Result register_mbx_base(mbx_smc_args *base)
{
	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] default ENTER\033[m\n");

	if(Sec_ARM_MBX_Base != NULL)
	{
		MSTAR_MBX_PRINTF_DEBUG("\033[0;32;31m [Sec_ARM MBX] Sec_ARM_MBX_Base already init %p\033[m\n",(void *)Sec_ARM_MBX_Base);
		MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] default EXIT\033[m\n");
		return 0;
	}

	Sec_ARM_MBX_Base = (mbx_smc_args *)base;
	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] default EXIT\033[m\n");

	return 0;
}

void __attribute__((weak)) fire_mbx_R2T(void) //mboot and linux please impelement
{
	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] default ENTER\033[m\n");
	TRIGGER_MBX_INTERRUPT;
	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] default EXIT\033[m\n");
}

TEE_Result send_mbx(thread_smc_args *args,OPTEE_MBX_CMD_ID id, uint32_t mbx_slot)
{
	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] ENTER\033[m\n");

	register_mbx_base(MBX_BASE);

	if(mbx_slot != 0)
	{
		MSTAR_MBX_PRINTF_ERROR("\033[0;32;31m [Sec_ARM MBX] Error NonOS Only can use mbx_slot 0 now mbx_slot %d\033[m\n",mbx_slot);
		return 1;
	}

	if(mbx_slot >= MAX_SEC_ARM_NUM)
	{
		MSTAR_MBX_PRINTF_ERROR("\033[0;32;31m [Sec_ARM MBX] Error mbx_slot %d over %d\033[m\n",mbx_slot,MAX_SEC_ARM_NUM);
		return 1;
	}

	if(Sec_ARM_MBX_Base == NULL)
	{
		MSTAR_MBX_PRINTF_ERROR("\033[0;32;31m [Sec_ARM MBX] Error Sec_ARM_MBX_Base not Init\033[m\n");
		return 1;
	}

	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] %s %d id %x mbx_slot %d\033[m\n",__func__,__LINE__,id,mbx_slot);
	Sec_ARM_MBX_Base[mbx_slot].a0 = (uint64_t) args->a0;
	Sec_ARM_MBX_Base[mbx_slot].a1 = (uint64_t) args->a1;
	Sec_ARM_MBX_Base[mbx_slot].a2 = (uint64_t) args->a2;
	Sec_ARM_MBX_Base[mbx_slot].a3 = (uint64_t) args->a3;
	Sec_ARM_MBX_Base[mbx_slot].a4 = (uint64_t) args->a4;
	Sec_ARM_MBX_Base[mbx_slot].a5 = (uint64_t) args->a5;
	Sec_ARM_MBX_Base[mbx_slot].a6 = (uint64_t) args->a6;
	Sec_ARM_MBX_Base[mbx_slot].a7 = (uint64_t) args->a7;
	Sec_ARM_MBX_Base[mbx_slot].cmd_id = (uint64_t) id;
	Sec_ARM_MBX_Base[mbx_slot].version = (uint64_t) 0x1;

	FLUSH_CACHE(Sec_ARM_MBX_Base, sizeof(mbx_smc_args) * MAX_SEC_ARM_NUM);

	if(id == CMD_R2T_GENERAL || id == CMD_R2T_RPC_DONE) {
		fire_mbx_R2T();
		{
			int i=0;
			while(Sec_ARM_MBX_Base[mbx_slot].cmd_id != CMD_T2R_GENERAL_DONE)
			{
				FLUSH_CACHE(Sec_ARM_MBX_Base, sizeof(mbx_smc_args) * MAX_SEC_ARM_NUM);
				if(i++ == 100){
					MSTAR_MBX_PRINTF_DEBUG("\033[0;32;31m [Sec_ARM MBX] Mboot Busy wait TEE done\033[m\n",__func__,__LINE__);
				}
				udelay(10);
				i%=400;
			}

			args->a0 = Sec_ARM_MBX_Base[mbx_slot].a0;
			args->a1 = Sec_ARM_MBX_Base[mbx_slot].a1;
			args->a2 = Sec_ARM_MBX_Base[mbx_slot].a2;
			args->a3 = Sec_ARM_MBX_Base[mbx_slot].a3;
			args->a4 = Sec_ARM_MBX_Base[mbx_slot].a4;
			args->a5 = Sec_ARM_MBX_Base[mbx_slot].a5;
			args->a6 = Sec_ARM_MBX_Base[mbx_slot].a6;
			args->a7 = Sec_ARM_MBX_Base[mbx_slot].a7;
			Sec_ARM_MBX_Base[mbx_slot].cmd_id = CMD_NULL;
		}
	} else if(id == CMD_T2R_GENERAL_DONE || id == CMD_T2R_RPC) {
		Sec_ARM_MBX_Base[mbx_slot].cmd_id = CMD_NULL;
		MSTAR_MBX_PRINTF_ERROR("\033[0;32;31m [Sec_ARM MBX] Not Support Mode 0x%x\033[m\n",id);
	} else {
		Sec_ARM_MBX_Base[mbx_slot].cmd_id = CMD_NULL;
		MSTAR_MBX_PRINTF_ERROR("\033[0;32;31m [Sec_ARM MBX] Not Support Mode 0x%x\033[m\n",id);
		return 1;
	}

	MSTAR_MBX_PRINTF_FLOW("\033[0;32;31m [Sec_ARM MBX] EXIT\033[m\n");

	return 0;
}

#define NUM_C 10000
#define ROUND 5
#define EXPEC(a,b,c) do{ if(a != b){ printf("\033[0;32;31m [Ian] %s %d Expect %x Result %x Failed\033[m\n",__func__,__LINE__,a,b); while(1);} }while(0)
void do_sec_test(void)
{
	unsigned int reg = 0;
	int i = 0;
	int j = 0;
	struct thread_smc_args args;
	unsigned int time[5];

	printf("\033[0;32;31m [Ian] %s %d \033[m\n",__func__,__LINE__);

	register_mbx_base(MBX_BASE);

	for(j = 0 ; j < ROUND ; j++){
		time[j] = MDrv_TIMER_GetMs(0);
		memset(&args,0,sizeof(struct thread_smc_args));
		for(i = 0 ; i < NUM_C ; i++){
			args.a0 = 0x88002000;
			send_mbx(&args, CMD_R2T_GENERAL, 0);
			EXPEC(args.a0,0xffffffff,0);
			EXPEC(args.a1,0x1234,0);
			EXPEC(args.a2,0x5678,0);
			EXPEC(args.a3,0x9abc,0);
			args.a0 = 0x88582684;
			send_mbx(&args, CMD_R2T_GENERAL, 0);
			EXPEC(args.a0,0xFFFF000A,0);
			EXPEC(args.a1,0xFFFF0009,0);
			EXPEC(args.a2,0xFFFF0000,0);
			args.a0 = 0x88582685;
			send_mbx(&args, CMD_R2T_GENERAL, 0);
			EXPEC(args.a0,0x0,0);
			EXPEC(args.a1,0xFFFF3072,0);
			EXPEC(args.a2,0xFFFF0001,0);
		}
		time[j] = MDrv_TIMER_GetMs(0) - time[j];
	}

	for(j = 0 ; j < ROUND ; j++){
		printf("[%d] SUM : %d ms (%d)\n",j,time[j],NUM_C);
	}

}
