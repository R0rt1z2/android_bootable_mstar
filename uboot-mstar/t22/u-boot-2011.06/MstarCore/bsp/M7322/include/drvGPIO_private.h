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
#ifndef _DRVGPIO_PRIV_H_
#define _DRVGPIO_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvGPIO.h"

typedef enum {
    MDrv_CMD_GPIO_Init,
    MDrv_CMD_GPIO_GetStauts,
    MDrv_CMD_GPIO_Set_High,
    MDrv_CMD_GPIO_Set_Low,
    MDrv_CMD_GPIO_Set_Input,
    MDrv_CMD_GPIO_Get_Inout,
    MDrv_CMD_GPIO_Get_Level,
    MDrv_CMD_GPIO_Attach_Interrupt,
    MDrv_CMD_GPIO_Detach_Interrupt,
    MDrv_CMD_GPIO_En_Interrupt,
    MDrv_CMD_GPIO_Dis_Interrupt,
    MDrv_CMD_GPIO_Dis_Interrupt_All,
    MDrv_CMD_GPIO_En_Interrupt_All,
    MDrv_CMD_GPIO_Interrupt_Action,
    MDrv_CMD_GPIO_Check_Interrupt,
    MDRV_CMD_GPIO_KernelModePoll,
} eGpioIoctlOpt;


typedef struct _GPIO_RESOURCE_PRIVATE
{
   MS_S32       _s32EventId;
}GPIO_RESOURCE_PRIVATE;

typedef struct _GPIO_GET_STS_PARAM
{
    GPIO_Status *pSts;
}GPIO_GET_STS_PARAM, *PGPIO_GET_STS_PARAM;

typedef struct _GPIO_SET_HIGH_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_HIGH_PARAM, *PGPIO_SET_HIGH_PARAM;

typedef struct _GPIO_SET_LOW_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_LOW_PARAM, *PGPIO_SET_LOW_PARAM;

typedef struct _GPIO_SET_INPUT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_INPUT_PARAM, *PGPIO_SET_INPUT_PARAM;

typedef struct _GPIO_GET_INOUT_PARAM
{
    MS_U32 u32InOut;
    MS_GPIO_NUM gpio_num;
}GPIO_GET_INOUT_PARAM, *PGPIO_GET_INOUT_PARAM;

typedef struct _GPIO_GET_LEVEL_PARAM
{
    MS_U32 u32Level;
    MS_GPIO_NUM gpio_num;
}GPIO_GET_LEVEL_PARAM, *PGPIO_GET_LEVEL_PARAM;

typedef struct _GPIO_ATTACH_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
    GPIO_Edge gpio_edge_type;
}GPIO_ATTACH_INTERRUPT_PARAM, *PGPIO_ATTACH_INTERRUPT_PARAM;

typedef struct _GPIO_DETACH_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_DETACH_INTERRUPT_PARAM, *PGPIO_DETACH_INTERRUPT_PARAM;

typedef struct _GPIO_ENABLE_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_ENABLE_INTERRUPT_PARAM, *PGPIO_ENABLE_INTERRUPT_PARAM;

typedef struct _GPIO_DISABLE_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_DISABLE_INTERRUPT_PARAM, *PGPIO_DISABLE_INTERRUPT_PARAM;

typedef struct _GPIO_CHECK_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_CHECK_INTERRUPT_PARAM, *PGPIO_CHECK_INTERRUPT_PARAM;

typedef union _GPIO_PRIVATE_PARAM
{
     GPIO_GET_STS_PARAM                 privatGpioSts;
     GPIO_SET_HIGH_PARAM                privateGpioSetHigh;
     GPIO_SET_LOW_PARAM                 privateGpioSetLow;
     GPIO_SET_INPUT_PARAM               privateGpioInput;
     GPIO_GET_INOUT_PARAM               privateGpioGetInout;
     GPIO_GET_LEVEL_PARAM               privateGpioGetLevel;
     GPIO_ATTACH_INTERRUPT_PARAM        privateGpioAttachInterrupt;
     GPIO_DETACH_INTERRUPT_PARAM        privateGpioDetachInterrupt;
     GPIO_ENABLE_INTERRUPT_PARAM        privateGpioEnableInterrupt;
     GPIO_DISABLE_INTERRUPT_PARAM       privateGpioDisableInterrupt;
     GPIO_CHECK_INTERRUPT_PARAM         privateGpioCheckInterrupt;
}GPIO_PRIVATE_PARAM;

typedef void (*IOCTL_GPIO_GET_STS)(GPIO_Status*);
typedef void (*IOCTL_GPIO_INIT)(void);
typedef void (*IOCTL_GPIO_SET_H)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_SET_L)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_SET_INPUT)(MS_GPIO_NUM);
typedef int  (*IOCTL_GPIO_GET_INOUT)(MS_GPIO_NUM);
typedef int  (*IOCTL_GPIO_GET_LEVEL)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_ATTACH_INTERRUPT)(MS_GPIO_NUM,GPIO_Edge);
typedef GPIO_Result (*IOCTL_GPIO_DETACH_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_ENALBE_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_DISABLE_INTERRUPT)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_DISABLE_INTERRUPT_ALL)(void);
typedef void (*IOCTL_GPIO_ENABLE_INTERRUPT_ALL)(void);
typedef void (*IOCTL_GPIO_INTERRUPT_ACTION)(void);
typedef MS_BOOL (*IOCTL_GPIO_CHECK_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_TASK_SLEEP)(void);

typedef struct _GPIO_INSTANT_PRIVATE
{
    IOCTL_GPIO_GET_STS                      fpGpioGetSts;
    IOCTL_GPIO_INIT                         fpGpioInit;
    IOCTL_GPIO_SET_H                        fpGpioSetH;
    IOCTL_GPIO_SET_L                        fpGpioSetL;
    IOCTL_GPIO_SET_INPUT                    fpGpioSetInput;
    IOCTL_GPIO_GET_INOUT                    fpGpioGetInout;
    IOCTL_GPIO_GET_LEVEL                    fpGpioGetLevel;
    IOCTL_GPIO_ATTACH_INTERRUPT             fpGpioAttachInterrupt;
    IOCTL_GPIO_DETACH_INTERRUPT             fpGpioDetachInterrupt;
    IOCTL_GPIO_DISABLE_INTERRUPT            fpGpioDisableInterrupt;
    IOCTL_GPIO_ENALBE_INTERRUPT             fpGpioEnableInterrupt;
    IOCTL_GPIO_DISABLE_INTERRUPT_ALL        fpGpioDisableInterruptAll;
    IOCTL_GPIO_ENABLE_INTERRUPT_ALL         fpGpioEnableInterruptAll;
    IOCTL_GPIO_INTERRUPT_ACTION             fpGpioInterruptAction;
    IOCTL_GPIO_CHECK_INTERRUPT              fpGpioCheckInterrupt;
    IOCTL_GPIO_TASK_SLEEP                   fpGpioTaskSleep;
}GPIO_INSTANT_PRIVATE;

void GPIORegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPIOOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPIOClose(void* pInstance);
MS_U32 GPIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 GPIOStr(MS_U32 u32PowerState, void* pModule);

void MDrv_GPIO_GetStatus_U2K(GPIO_Status *pStatus);
void mdrv_gpio_init_U2K(void);
void mdrv_gpio_set_high_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_set_low_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_set_input_U2K(MS_GPIO_NUM gpio);
int mdrv_gpio_get_inout_U2K(MS_GPIO_NUM gpio);
int mdrv_gpio_get_level_U2K(MS_GPIO_NUM gpio);
GPIO_Result mdrv_gpio_attach_interrupt_U2K(MS_GPIO_NUM gpio_num, GPIO_Edge gpio_edge_type, GPIO_Callback pCallback, GPIO_Callback_Extend pCallback_extend);
GPIO_Result mdrv_gpio_detach_interrupt_U2K(MS_GPIO_NUM gpio_num);
GPIO_Result mdrv_gpio_enable_interrupt_U2K(MS_GPIO_NUM gpio);
GPIO_Result mdrv_gpio_disable_interrupt_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_disable_interrupt_all_U2K(void);
void mdrv_gpio_enable_interrupt_all_U2K(void);
void mdrv_gpio_interrupt_action_U2K(void);
MS_BOOL mdrv_gpio_check_interrupt_U2K(MS_GPIO_NUM gpio);
MS_BOOL  MDrv_GPIO_TaskWait(void);
MS_BOOL  MDrv_GPIO_TaskSet(void);
#ifdef __cplusplus
}
#endif
#endif // _DRVGPIO_PRIV_H_
