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
#include <stddef.h>
#include <stdio.h>
#include "assert.h"
#include "mhal_arch.h"
#include "datatype.h"

typedef unsigned long               MS_U32;
typedef unsigned char               MS_U8;
typedef signed long               MS_S32;
typedef unsigned char               MS_BOOL;
typedef void ( *TimerCb ) (MS_U32 u32StTimer, MS_U32 u32TimerID);  ///< Timer callback function  u32StTimer: not used; u32TimerID: Timer ID
#if defined(__arm__) || defined(__aarch64__)
static volatile TimerCb timer_isr[2] = {0, 0};
static volatile MS_U32 timer_period[2]={1, 1};
static volatile MS_BOOL gStartTimer[2]={0, 0};
static volatile MS_U32 firsttimer_interval[2]={0, 0};
static volatile MS_U32 firsttimer_interval_old[2]={0, 0};
static volatile MS_BOOL firsttimer_interval_flag[2]={0, 0};
static volatile MS_U32 gsystem_ip_time_ms[2]={0, 0};
#else
volatile static TimerCb timer_isr[2] = {0, 0};
volatile static MS_U32 timer_period[2]={1, 1};
volatile static MS_BOOL gStartTimer[2]={0, 0};
volatile static MS_U32 firsttimer_interval[2]={0, 0};
volatile static MS_U32 firsttimer_interval_old[2]={0, 0};
volatile static MS_BOOL firsttimer_interval_flag[2]={0, 0};
volatile static MS_U32 gsystem_ip_time_ms[2]={0, 0};
#endif

extern volatile MS_U32 gsystem_time_ms;
//MS_U8 gtimer_cb_number;
void Timer_firsttime_interval(MS_U32 firsttime_interval, MS_S32 s32TimerId_set)
{
    firsttimer_interval_flag[s32TimerId_set]=0;
    firsttimer_interval_old[s32TimerId_set]=0;
    gsystem_ip_time_ms[s32TimerId_set]=0;
    firsttimer_interval[s32TimerId_set]=firsttime_interval;

}
void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set)
{
    gStartTimer[s32TimerId_set]=bStartTimer_set;
}
void Timer_Period_Register(MS_U32 periodCb, MS_S32 s32TimerId_set)
{

    //printf("Timer size of = %lx\n",sizeof(timer_period));
    if(s32TimerId_set>=2)
    {
        printf("Not enough buffer to set period!!!!!\n");
    }
    if(periodCb==0)
    {
        periodCb=1;
    }
    timer_period[s32TimerId_set]=periodCb;

}
MS_S32 Timer_IRQ_Register(void *ptCb)
{
    unsigned int i;
    //printf("Timer size of = %lx\n",sizeof(timer_isr));

    for(i=0;i<=(sizeof(timer_isr)>>2);i++)
    {
        if(i==((sizeof(timer_isr))>>2))
        {
            printf("Not enough buffer to attach timerCB!!!!!\n");
            ASSERT(0);
        }
        if(timer_isr[i]==0)
        {
            timer_isr[i]=(TimerCb)ptCb;
            break;
        }
    }
    return i;
}
void Timer_IRQ_Delete(MS_S32 timer_ld)
{
    if(timer_isr[timer_ld]!=0)
    {
        timer_isr[timer_ld]=0;
        gsystem_ip_time_ms[timer_ld]=0;
    }
}

#if defined (__arm__) || defined(__aarch64__)
void  ProcessTimer0Isr(void)
{
    int i;

    gsystem_time_ms++;

    for(i=0;i<2;i++)
        {
        if (timer_isr[i])
            {
             if((firsttimer_interval_flag[i]==0)&&(gStartTimer[i]==1))
                {
                if(firsttimer_interval_old[i]==0)
                    {
                        firsttimer_interval_old[i]=gsystem_time_ms;
                    }
                if(((gsystem_time_ms<firsttimer_interval_old[i])&&((0xFFFFFFFF-firsttimer_interval_old[i]+gsystem_time_ms)>=firsttimer_interval[i]))||((gsystem_time_ms>firsttimer_interval_old[i])&&((gsystem_time_ms-firsttimer_interval_old[i])>=firsttimer_interval[i])))
                    {
                        firsttimer_interval_flag[i]=1;
                    }
                if(firsttimer_interval[i]==0)
                    {
                        firsttimer_interval_flag[i]=1;
                    }
                }
                if(((gsystem_ip_time_ms[i]%timer_period[i])==0)&&(gStartTimer[i]==1)&&(firsttimer_interval_flag[i]==1))
                    {
                        timer_isr[i](0,i);
                    }
                if((gStartTimer[i]==1)&&(firsttimer_interval_flag[i]==1))
                    {
                        gsystem_ip_time_ms[i]++;
                    }
            }
        }
}

#else

void  ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, U32 vector )
{
    int i;
    UNUSED(pHalReg);
    UNUSED(vector);

    gsystem_time_ms++;
    for(i=0;i<2;i++)
        {
        if (timer_isr[i])
            {
             if((firsttimer_interval_flag[i]==0)&&(gStartTimer[i]==1))
                {
                if(firsttimer_interval_old[i]==0)
                    {
                        firsttimer_interval_old[i]=gsystem_time_ms;
                    }
                if(((gsystem_time_ms<firsttimer_interval_old[i])&&((0xFFFFFFFF-firsttimer_interval_old[i]+gsystem_time_ms)>=firsttimer_interval[i]))||((gsystem_time_ms>firsttimer_interval_old[i])&&((gsystem_time_ms-firsttimer_interval_old[i])>=firsttimer_interval[i])))
                    {
                        firsttimer_interval_flag[i]=1;
                    }
                if(firsttimer_interval[i]==0)
                    {
                        firsttimer_interval_flag[i]=1;
                    }
                }
                if(((gsystem_ip_time_ms[i]%timer_period[i])==0)&&(gStartTimer[i]==1)&&(firsttimer_interval_flag[i]==1))
                    {
                        timer_isr[i](0,i);
                    }
                if((gStartTimer[i]==1)&&(firsttimer_interval_flag[i]==1))
                    {
                        gsystem_ip_time_ms[i]++;
                    }
            }
        }
}
#endif
