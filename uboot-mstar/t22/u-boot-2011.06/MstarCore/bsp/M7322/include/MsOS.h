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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar OS Wrapper
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup SYS SYS modules

 * \defgroup MSOS MsOS interface (MsOS.h)
 *  \ingroup SYS

    \brief

    MsOS is an OS abstraction layer, which is for utopia drivers
    not to consider different system API usage on each platform

    <b> Features </b>

    - Memory pool
    - Task
    - Lock
    - Event
    - Timer
    - Queue
    - Interrupt
    - Cache
    - Mpool
    - Ion
    - Share memory

    <b> Diagram </b>
    \image html msos_1.png

 * \defgroup MsOS_BASIC MsOS Module basic
 *  \ingroup MSOS

 * \defgroup MsOS_Task MsOS OS Platform relative
 *  \ingroup MSOS

 * \defgroup MsOS_MPool MPool relative
 *  \ingroup MSOS

 * \defgroup MsOS_Interrupt interrupt relative
 *  \ingroup MSOS

 * \defgroup MsOS_Interrupt_NOS interrupt relative for non-OS support
 *  \ingroup MSOS

 * \defgroup MsOS_ION MsOS ION driver
 *  \ingroup MSOS

 * \defgroup MsOS_ToBeModified MsOS api to be modified
 *  \ingroup MSOS

 * \defgroup MsOS_ToBeRemove MsOS api to be removed
 *  \ingroup MSOS
 */

#ifndef _MS_OS_H_
#define _MS_OS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#endif
#include "UFO.h"
#include "MsTypes.h"
#include "MsIRQ.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#define MSIF_MSOS_LIB_CODE              {'M','S','O','S'}    //Lib code
#define MSIF_MSOS_LIBVER                {'0','1'}            //LIB version
#define MSIF_MSOS_BUILDNUM              {'0','1'}            //Build Number
#define MSIF_MSOS_CHANGELIST            {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define MSOS_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_MSOS_LIB_CODE,                  /* IP__                                             */  \
    MSIF_MSOS_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_MSOS_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_MSOS_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define MSOS_TASK_MAX           (32+120)
#define MSOS_MEMPOOL_MAX        (8+64)
#define MSOS_FIXSIZE_MEMPOOL_MAX  (8)
#define MSOS_SEMAPHORE_MAX      (32+150)
#define MSOS_MUTEX_MAX          (64+240)
#ifdef CONFIG_MSOS_EVENTGROUP_MAX_SEL
#define MSOS_EVENTGROUP_MAX     (CONFIG_MSOS_EVENTGROUP_MAX_NUM)
#else
#define MSOS_EVENTGROUP_MAX     (192)
#endif
#define MSOS_TIMER_MAX          (32)
#define MSOS_QUEUE_MAX          (16+60)
#define MSOS_CONDITION_MAX      (64+240)

#define MSOS_OS_MALLOC          (0x7654FFFF)
#define MSOS_MALLOC_ID          (0x0000FFFF)

#if defined(__MIPS16E__)
#define ATTRIBUTE   __attribute__((nomips16))
#else
#define ATTRIBUTE
#endif

#ifndef SYMBOL_WEAK
#ifndef MSOS_TYPE_LINUX_KERNEL
#define SYMBOL_WEAK __attribute__((weak))
#else
#define SYMBOL_WEAK
#endif
#endif
#define MSOS_INVALID_ADDR          (0xFFFFFFFF)
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//time and clock macros
#define TICK_PER_ONE_MS     (1) //Note: confirm Kernel fisrt
#define MSOS_WAIT_FOREVER   (0xffffff00/TICK_PER_ONE_MS)

#ifdef  MSOS_PERF_DEBUG
#define MSOS_PERF_PROFILE_DECL()  MS_U32 u32time1=0, u32time2=0
#define MSOS_PERF_PROFILE_ON()    u32time1 = MsOS_GetSystemTime()
#define MSOS_PERF_PROFILE_OFF()   u32time2 = MsOS_GetSystemTime(); \
                                  printf("[MSOS_DBG]%s:%d takes %6dms\n",__FILE__, __LINE__, u32time2-u32time1)
#else
#define MSOS_PERF_PROFILE_DECL()
#define MSOS_PERF_PROFILE_ON()
#define MSOS_PERF_PROFILE_OFF()
#endif

#define     MSOS_CACHE_BOTH         0
#define     MSOS_NON_CACHE_BOTH     1
#define     MSOS_CACHE_USERSPACE    2
#define     MSOS_NON_CACHE_USERSPACE  3
#define     MSOS_CACHE_KERNEL       4
#define     MSOS_NON_CACHE_KERNEL   5

//-------------------------------------------------------------------------------------------------
// Branch prediction optimization
//-------------------------------------------------------------------------------------------------
#define MSOS_BRANCH_PREDICTION_LIKELY(x)        __builtin_expect(!!(x), 1)
#define MSOS_BRANCH_PREDICTION_UNLIKELY(x)      __builtin_expect(!!(x), 0)

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_ECOS) || defined (MSOS_TYPE_LINUX) || defined (MSOS_TYPE_NOS) || defined (MSOS_TYPE_UCOS) || defined(MSOS_TYPE_ATOMTHREADS) || defined (MSOS_TYPE_NUTTX) || defined (MSOS_TYPE_OPTEE)

//compatible with Nucleus's task_entry
typedef void ( *TaskEntry ) (MS_VIRT argc, void *argv); ///< Task entry function  argc: pass additional data to task entry; argv: not used by eCos
typedef void ( *InterruptCb ) (InterruptNum eIntNum);               ///< Interrupt callback function
typedef void ( *SignalCb ) (MS_U32 u32Signals);        ///< Signal callback function
typedef void ( *TimerCb ) (MS_U32 u32StTimer, MS_U32 u32TimerID);  ///< Timer callback function  u32StTimer: not used; u32TimerID: Timer ID

typedef struct {
    volatile MS_S32 s32Value;
} MsOS_Atomic;


#ifdef MSOS_TYPE_UCOS
/// Task priority
typedef enum
{
    E_TASK_PRI_SYS      = 0,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_TASK_PRI_HIGHEST  = 16,   ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_TASK_PRI_HIGH     = 32,   ///< High priority task     ( service task )
    E_TASK_PRI_MEDIUM   = 48,   ///< Medium priority task   ( application task )
    E_TASK_PRI_LOW      = 64,   ///< Low priority task      ( nonbusy application task )
    E_TASK_PRI_LOWEST   = 96,   ///< Lowest priority task   ( idle application task )
} TaskPriority;
#elif defined(MSOS_TYPE_ATOMTHREADS)
typedef enum
{
    E_TASK_PRI_SYS      = 0,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_TASK_PRI_HIGHEST  = 51,    ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_TASK_PRI_HIGH     = 102,    ///< High priority task     ( service task )
    E_TASK_PRI_MEDIUM   = 153,   ///< Medium priority task   ( application task )
    E_TASK_PRI_LOW      = 204,   ///< Low priority task      ( nonbusy application task )
    E_TASK_PRI_LOWEST   = 255,   ///< Lowest priority task   ( idle application task )
} TaskPriority;

#elif defined(MSOS_TYPE_NUTTX)
/// Task priority
typedef enum
{
    E_TASK_PRI_SYS      = 255,
    E_TASK_PRI_HIGHEST  = 255,
    E_TASK_PRI_HIGH     = 178,
    E_TASK_PRI_MEDIUM   = 100,
    E_TASK_PRI_LOW      = 50,
    E_TASK_PRI_LOWEST   = 1,
} TaskPriority;
#else
/// Task priority
typedef enum
{
    E_TASK_PRI_SYS      = 0,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_TASK_PRI_HIGHEST  = 4,    ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_TASK_PRI_HIGH     = 8,    ///< High priority task     ( service task )
    E_TASK_PRI_MEDIUM   = 12,   ///< Medium priority task   ( application task )
    E_TASK_PRI_LOW      = 16,   ///< Low priority task      ( nonbusy application task )
    E_TASK_PRI_LOWEST   = 24,   ///< Lowest priority task   ( idle application task )
} TaskPriority;
#endif

/// Task Satus
typedef enum
{
    E_TASK_INVALID_INFO      = 0,    ///< Invalid Task infomation
    E_TASK_RUNNING           = 1,    ///< Task is running
    E_TASK_NOT_EXIST         = 2,    ///< Task is not exist
    E_TASK_INVALID_STATE     = 3,    ///< Current OS not provide this function
} TaskStatus;

/// Suspend type
typedef enum
{
    E_MSOS_PRIORITY,            ///< Priority-order suspension
    E_MSOS_FIFO,                ///< FIFO-order suspension
} MsOSAttribute;

/// Message size type
typedef enum
{
    E_MSG_FIXED_SIZE,           ///< Fixed size message
    E_MSG_VAR_SIZE,             ///< Variable size message
} MessageType;

/// Event mode
typedef enum
{
    E_AND,                      ///< Specify all of the requested events are require.
    E_OR,                       ///< Specify any of the requested events are require.
    E_AND_CLEAR,                ///< Specify all of the requested events are require. If the request are successful, clear the event.
    E_OR_CLEAR,                 ///< Specify any of the requested events are require. If the request are successful, clear the event.
} EventWaitMode;

typedef enum
{
    E_IRQ_DISABLE           = 0, // reserve for MsOS_DisableInterrupt
    E_IRQ_ENABLE            = 1, // reserve for MsOS_EnableInterrupt
    E_IRQ_ACK               = 2,
    E_IRQ_COMPLETE          = 3,
    E_IRQ_DEBUG_STATUS_FLOW = 4,
    E_IRQ_PUSH_DISABLE      = 5,
    E_IRQ_CHECK             = 6, // reserve for MsOS_CheckInterrupt
    E_IRQ_DEBUG_DISABLE     = 1 << 31,
} IrqDebugOpt;

typedef struct
{
    MS_S32                          iId;
    MS_U32                          uPoolSize;
    MS_U32                          u32MinAllocation;
    MS_U32                          u32Addr;
    MsOSAttribute                   eAttribute;
    char                            szName[16];
} MemoryPool_Info, *PMemoryPool_Info;

typedef struct
{
    MS_S32                          iId;
    TaskPriority                    ePriority;
    void                            *pStack;
    MS_U32                          u32StackSize;
    char szName[16];
} Task_Info, *PTask_Info;

#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
typedef struct
{
        unsigned int                LX_MEM_ADDR;
        unsigned int                LX_MEM_LENGTH;
        unsigned int                LX_MEM2_ADDR;
        unsigned int                LX_MEM2_LENGTH;
        unsigned int                EMAC_ADDR;
        unsigned int                EMAC_LENGTH;
        unsigned int                DRAM_ADDR;
        unsigned int                DRAM_LENGTH;
        unsigned int                BB_ADDR;
        unsigned int                BB_LENGTH;
        unsigned int                MPOOL_MEM_ADDR;
        unsigned int                MPOOL_MEM_LENGTH;
        unsigned int                G3D_MEM0_ADDR;
        unsigned int                G3D_MEM0_LENGTH;
        unsigned int                G3D_MEM1_ADDR;
        unsigned int                G3D_MEM1_LENGTH;
        unsigned int                G3D_CMDQ_ADDR;
        unsigned int                G3D_CMDQ_LENGTH;
}IO_Sys_Info_t;
#else
typedef struct
{
        unsigned long long                LX_MEM_ADDR;
        unsigned long long                LX_MEM_LENGTH;
        unsigned long long                LX_MEM2_ADDR;
        unsigned long long                LX_MEM2_LENGTH;
        unsigned long long                EMAC_ADDR;
        unsigned long long                EMAC_LENGTH;
        unsigned long long                DRAM_ADDR;
        unsigned long long                DRAM_LENGTH;
        unsigned long long                BB_ADDR;
        unsigned long long                BB_LENGTH;
        unsigned long long                MPOOL_MEM_ADDR;
        unsigned long long                MPOOL_MEM_LENGTH;
        unsigned long long                G3D_MEM0_ADDR;
        unsigned long long                G3D_MEM0_LENGTH;
        unsigned long long                G3D_MEM1_ADDR;
        unsigned long long                G3D_MEM1_LENGTH;
        unsigned long long                G3D_CMDQ_ADDR;
        unsigned long long                G3D_CMDQ_LENGTH;
}IO_Sys_Info_t;
#endif
typedef struct
{
        unsigned int                LX_MEM_ADDR;
        unsigned int                LX_MEM_LENGTH;
        unsigned int                LX_MEM2_ADDR;
        unsigned int                LX_MEM2_LENGTH;
        unsigned int                LX_MEM3_ADDR;
        unsigned int                LX_MEM3_LENGTH;
        unsigned int                LX_MEM4_ADDR;
        unsigned int                LX_MEM4_LENGTH;
        unsigned int                LX_MEM5_ADDR;
        unsigned int                LX_MEM5_LENGTH;
        unsigned int                EMAC_ADDR;
        unsigned int                EMAC_LENGTH;
        unsigned int                DRAM_ADDR;
        unsigned int                DRAM_LENGTH;
        unsigned int                BB_ADDR;
        unsigned int                BB_LENGTH;
        unsigned int                MPOOL_MEM_ADDR;
        unsigned int                MPOOL_MEM_LENGTH;
        unsigned int                G3D_MEM0_ADDR;
        unsigned int                G3D_MEM0_LENGTH;
        unsigned int                G3D_MEM1_ADDR;
        unsigned int                G3D_MEM1_LENGTH;
        unsigned int                G3D_CMDQ_ADDR;
        unsigned int                G3D_CMDQ_LENGTH;
}IO_Sys_Info_t_EX;

typedef struct
{
    unsigned int                u32Addr;
    unsigned int                u32Size;
    unsigned int                u32Interval;
    unsigned char               u8MiuSel;
    unsigned int                bcached;
} KDrvMPool_Info_t;

#elif defined (MSOS_TYPE_LINUX_KERNEL)

//#include <linux/kernel.h>
//#include <linux/interrupt.h>

typedef void ( *TaskEntry ) (MS_VIRT argc, void *argv); ///< Task entry function  argc: pass additional data to task entry; argv: not used by eCos
typedef void ( *InterruptCb ) (InterruptNum eIntNum);               ///< Interrupt callback function
typedef void ( *SignalCb ) (MS_U32 u32Signals);        ///< Signal callback function
typedef void ( *TimerCb ) (MS_U32 u32StTimer, MS_U32 u32TimerID);  ///< Timer callback function  u32StTimer: not used; u32TimerID: Timer ID

#if 0
typedef enum
{
    E_IRQ_DISABLE = 0 << 0, // reserve for MsOS_DisableInterrupt
    E_IRQ_ENABLE = 1 << 0, // reserve for MsOS_EnableInterrupt
    E_IRQ_ACK = 1 << 1,
    E_IRQ_DEBUG_STATUS_FLOW = 1 << 2,
    E_IRQ_DEBUG_DISABLE = 1 << 31,
} IrqDebugOpt;
#endif

/// Task priority
typedef enum
{
    E_TASK_PRI_SYS      = 0,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_TASK_PRI_HIGHEST  = 4,    ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_TASK_PRI_HIGH     = 8,    ///< High priority task     ( service task )
    E_TASK_PRI_MEDIUM   = 12,   ///< Medium priority task   ( application task )
    E_TASK_PRI_LOW      = 16,   ///< Low priority task      ( nonbusy application task )
    E_TASK_PRI_LOWEST   = 24,   ///< Lowest priority task   ( idle application task )
} TaskPriority;

/// Task Satus
typedef enum
{
    E_TASK_INVALID_INFO      = 0,    ///< Invalid Task infomation
    E_TASK_RUNNING           = 1,    ///< Task is running
    E_TASK_NOT_EXIST         = 2,    ///< Task is not exist
    E_TASK_INVALID_STATE     = 3,    ///< Current OS not provide this function
} TaskStatus;

/// Suspend type
typedef enum
{
    E_MSOS_PRIORITY,            ///< Priority-order suspension
    E_MSOS_FIFO,                ///< FIFO-order suspension
} MsOSAttribute;

/// Message size type
typedef enum
{
    E_MSG_FIXED_SIZE,           ///< Fixed size message
    E_MSG_VAR_SIZE,             ///< Variable size message
} MessageType;

/// Event mode
typedef enum
{
    E_AND,                      ///< Specify all of the requested events are require.
    E_OR,                       ///< Specify any of the requested events are require.
    E_AND_CLEAR,                ///< Specify all of the requested events are require. If the request are successful, clear the event.
    E_OR_CLEAR,                 ///< Specify any of the requested events are require. If the request are successful, clear the event.
} EventWaitMode;

typedef struct
{
    MS_S32                          iId;
    MS_U32                          uPoolSize;
    MS_U32                          u32MinAllocation;
    MS_U32                          u32Addr;
    MsOSAttribute                   eAttribute;
    char                            szName[16];
} MemoryPool_Info, *PMemoryPool_Info;

typedef struct
{
    MS_S32                          iId;
    TaskPriority                    ePriority;
    void                            *pStack;
    MS_U32                          u32StackSize;
    char szName[16];
} Task_Info, *PTask_Info;

typedef struct
{
        unsigned int                LX_MEM_ADDR;
        unsigned int                LX_MEM_LENGTH;
        unsigned int                LX_MEM2_ADDR;
        unsigned int                LX_MEM2_LENGTH;
        unsigned int                EMAC_ADDR;
        unsigned int                EMAC_LENGTH;
        unsigned int                DRAM_ADDR;
        unsigned int                DRAM_LENGTH;
        unsigned int                BB_ADDR;
        unsigned int                BB_LENGTH;
        unsigned int                MPOOL_MEM_ADDR;
        unsigned int                MPOOL_MEM_LENGTH;
        unsigned int                G3D_MEM0_ADDR;
        unsigned int                G3D_MEM0_LENGTH;
        unsigned int                G3D_MEM1_ADDR;
        unsigned int                G3D_MEM1_LENGTH;
        unsigned int                G3D_CMDQ_ADDR;
        unsigned int                G3D_CMDQ_LENGTH;
}IO_Sys_Info_t;

typedef struct
{
        unsigned int                LX_MEM_ADDR;
        unsigned int                LX_MEM_LENGTH;
        unsigned int                LX_MEM2_ADDR;
        unsigned int                LX_MEM2_LENGTH;
        unsigned int                LX_MEM3_ADDR;
        unsigned int                LX_MEM3_LENGTH;
        unsigned int                LX_MEM4_ADDR;
        unsigned int                LX_MEM4_LENGTH;
        unsigned int                LX_MEM5_ADDR;
        unsigned int                LX_MEM5_LENGTH;
        unsigned int                EMAC_ADDR;
        unsigned int                EMAC_LENGTH;
        unsigned int                DRAM_ADDR;
        unsigned int                DRAM_LENGTH;
        unsigned int                BB_ADDR;
        unsigned int                BB_LENGTH;
        unsigned int                MPOOL_MEM_ADDR;
        unsigned int                MPOOL_MEM_LENGTH;
        unsigned int                G3D_MEM0_ADDR;
        unsigned int                G3D_MEM0_LENGTH;
        unsigned int                G3D_MEM1_ADDR;
        unsigned int                G3D_MEM1_LENGTH;
        unsigned int                G3D_CMDQ_ADDR;
        unsigned int                G3D_CMDQ_LENGTH;
}IO_Sys_Info_t_EX;

typedef struct
{
    unsigned int                u32Addr;
    unsigned int                u32Size;
    unsigned int                u32Interval;
    unsigned char               u8MiuSel;
    unsigned int                bcached;
} KDrvMPool_Info_t;

typedef  struct
{
   MS_U32 mpool_base;
   MS_U32 mpool_size;
   MS_U32 mmap_offset;
   MS_U32 mmap_size;
   MS_U32 mmap_interval;
   MS_U8  mmap_miusel;
   unsigned int u8MapCached;
   MS_BOOL setflag;
}MMAP_FileData;

#elif defined (MSOS_TYPE_XXX)

#error "The OS is not supported now ..."

#endif

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
//
// Init
//

//-------------------------------------------------------------------------------------------------
/// Initialize MsOS
/// @ingroup MsOS_BASIC
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Init (void);

//-------------------------------------------------------------------------------------------------
/// Initialize MsOS for STR Init
/// @ingroup MsOS_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Init_str (void);

//-------------------------------------------------------------------------------------------------
/// Initialize MsOS for Fastboot Init
/// @ingroup MsOS_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Init_Fastboot (void);


//-------------------------------------------------------------------------------------------------
/// Create a variable-size memory pool dynamically
/// @ingroup MsOS_MPool
/// @param  u32PoolSize         \b IN: pool size in bytes
/// @param  u32MinAllocation    \b IN: not used
/// @param  pPoolAddr           \b IN: starting address for the memory pool
/// @param  eAttribute          \b IN: only E_MSOS_FIFO - suspended in FIFO order
/// @param  pPoolName           \b IN: not used
/// @return >=0 : assigned memory pool ID
/// @return < 0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateMemoryPool (MS_U32 u32PoolSize,
                              MS_U32 u32MinAllocation,
                              void * pPoolAddr,
                              MsOSAttribute eAttribute,
                              char *pPoolName);

//-------------------------------------------------------------------------------------------------
/// Delete a variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  s32PoolId   \b IN: pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteMemoryPool (MS_S32 s32PoolId);

//-------------------------------------------------------------------------------------------------
/// Get the information of a variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  s32PoolId   \b IN: memory pool ID
/// @param  pPoolAddr   \b OUT: holding the starting address for the memory pool
/// @param  pu32PoolSize \b OUT: holding the total size of the memory pool
/// @param  pu32FreeSize \b OUT: holding the available free size of the memory pool
/// @param  pu32LargestFreeBlockSize  \b OUT: holding the size of the largest free block
/// @return TRUE : succeed
/// @return FALSE : the pool has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoMemoryPool (MS_S32 s32PoolId,
                             void **pPoolAddr,
                             MS_U32 *pu32PoolSize,
                             MS_U32 *pu32FreeSize,
                             MS_U32 *pu32LargestFreeBlockSize);

//-------------------------------------------------------------------------------------------------
/// Allocate a memory block with 16-Byte aligned starting address from the variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  u32Size     \b IN: request size
/// @param  s32PoolId   \b IN: memory pool ID
/// @return NULL : not enough available memory
/// @return Otherwise : pointer to the allocated memory block
//-------------------------------------------------------------------------------------------------
void * MsOS_AllocateMemory (MS_U32 u32Size, MS_S32 s32PoolId);

//void * MsOS_AllocateAlignedMemory (MS_U32 u32Size, MS_U32 u32AlignedByte, MS_S32 s32PoolId);
//-------------------------------------------------------------------------------------------------
/// Reallocate a block of memory with 4-byte aligned start address from the variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  pOrgAddress \b IN: points to the beginning of the original memory block
/// @param  u32NewSize  \b IN: size of new memory block
/// @param  s32PoolId   \b IN: memory pool ID
/// @return NULL : not enough available memory to expand the block or u32NewSize == 0 && pOrgAddress != NULL
/// @return Otherwise : pointer to the reallocated (and possibly moved) memory block
//  @note   reference realloc in malloc.cxx
//-------------------------------------------------------------------------------------------------
void * MsOS_ReallocateMemory (void *pOrgAddress, MS_U32 u32NewSize, MS_S32 s32PoolId);

//-------------------------------------------------------------------------------------------------
/// Free a memory block from the variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  pAddress    \b IN: pointer to previously allocated memory block
/// @param  s32PoolId   \b IN: memory pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_FreeMemory (void *pAddress, MS_S32 s32PoolId);

//-------------------------------------------------------------------------------------------------
/// Todo: no contant on MsOS_linux.c, please review it
/// @ingroup MsOS_ToBeRemove
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateFixSizeMemoryPool (MS_U32 u32PoolSize,
                                     MS_U32 u32BlockSize,
                                     void * pPoolAddr,
                                     MsOSAttribute eAttribute,
                                     char *pPoolName);

//-------------------------------------------------------------------------------------------------
/// Todo: no contant on MsOS_linux.c, please review it
/// @ingroup MsOS_ToBeRemove
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteFixSizeMemoryPool (MS_S32 s32PoolId);

//-------------------------------------------------------------------------------------------------
/// Todo: no contant on MsOS_linux.c, please review it
/// @ingroup MsOS_ToBeRemove
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoFixSizeMemoryPool (MS_S32 s32PoolId,
                                    void **pPoolAddr,
                                    MS_U32 *pu32PoolSize,
                                    MS_U32 *pu32FreeSize,
                                    MS_U32 *pu32LargestFreeBlockSize);

//-------------------------------------------------------------------------------------------------
/// Todo: no contant on MsOS_linux.c, please review it
/// @ingroup MsOS_ToBeRemove
//-------------------------------------------------------------------------------------------------
void * MsOS_AllocateFixSizeMemory (MS_S32 s32PoolId);

//-------------------------------------------------------------------------------------------------
/// Todo: no contant on MsOS_linux.c, please review it
/// @ingroup MsOS_ToBeRemove
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_FreeFixSizeMemory (void *pAddress, MS_S32 s32PoolId);

//-------------------------------------------------------------------------------------------------
/// Create a task
/// @ingroup MsOS_Task
/// @param  pTaskEntry       \b IN: task entry point
/// @param  u32TaskEntryData \b IN: task entry data: a pointer to some static data, or a
///          small integer, or NULL if the task does not require any additional data.
/// @param  eTaskPriority    \b IN: task priority
/// @param  bAutoStart       \b IN: start immediately or later
/// @param  pStackBase       \b IN: task stack
/// @param  u32StackSize     \b IN: stack size
/// @param  pTaskName        \b IN: task name
/// @return >=0 : assigned Task ID
/// @return < 0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateTask (TaskEntry pTaskEntry,
                        MS_VIRT u32TaskEntryData,
                        TaskPriority eTaskPriority,
                        MS_BOOL bAutoStart,
                        void * pStackEntry,
                        MS_U32 u32StackSize,
                        char *pTaskName);

//-------------------------------------------------------------------------------------------------
/// Delete a previously created task1
/// @ingroup MsOS_ToBeRemove
/// @param  s32TaskId   \b IN: task ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTask (MS_S32 s32TaskId);

//-------------------------------------------------------------------------------------------------
/// Yield the execution right to ready tasks with "the same" priority
/// @ingroup MsOS_Task
/// @return None
//-------------------------------------------------------------------------------------------------
void MsOS_YieldTask (void);

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @ingroup MsOS_Task
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms);

//-------------------------------------------------------------------------------------------------
/// Delay for u32Us microseconds
/// @ingroup MsOS_Task
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs (MS_U32 u32Us);

//-------------------------------------------------------------------------------------------------
/// Delay Poll for u32Us microseconds
/// @ingroup MsOS_Task
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs_Poll(MS_U32 u32Us);

//-------------------------------------------------------------------------------------------------
/// Resume the specified suspended task
/// @ingroup MsOS_Task
/// @param  s32TaskId   \b IN: Task ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   This API is not supported in Linux
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ResumeTask (MS_S32 s32TaskId);

//-------------------------------------------------------------------------------------------------
/// Suspend the specified task
/// @ingroup MsOS_Task
/// @param  s32TaskId   \b IN: Task ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   This API is not supported in Linux
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SuspendTask (MS_S32 s32TaskId);

#if defined(MSOS_TYPE_LINUX_KERNEL)
//-------------------------------------------------------------------------------------------------
// Get a task information
// @param  s32TaskId       \b IN: task ID
// @param  peTaskPriority  \b OUT: ptr to task priority
// @param  pu32StackSize   \b OUT: ptr to stack size
// @param  pTaskName       \b OUT: ptr to task name
// @return TRUE : succeed
// @return FALSE : the task has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoTask (MS_S32 s32TaskId,
                        TaskPriority *peTaskPriority,
                        MS_U32 *pu32StackSize,
                        char *pTaskName);
#endif

//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @ingroup MsOS_Task
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_InfoTaskID (void);

//-------------------------------------------------------------------------------------------------
/// Get task status
/// @param  pTaskInfo       \b IN: task information structure pointer
/// @param  peTaskState      \b OUT: ptr to task istate
/// @return TRUE : succeed
/// @return FALSE : invalid process result
/// @note  This API is not supported in Linux/uCos/nOS/nuttx
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_GetTaskStatus (Task_Info* pTaskInfo, TaskStatus *peTaskState);

//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// Get thread ID of current thread/process in OS
/// @ingroup MsOS_Task
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void);

//
// Mutex
//
#define MSOS_PROCESS_PRIVATE    0x00000000
#define MSOS_PROCESS_SHARED     0x00000001
#define MAX_MUTEX_NAME_LENGTH   50

//-------------------------------------------------------------------------------------------------
/// Create a mutex in the unlocked state1
/// @ingroup MsOS_Task
/// @param  eAttribute  \b IN: E_MSOS_FIFO: suspended in FIFO order
/// @param  pMutexName  \b IN: mutex name
/// @param  u32Flag  \b IN: process data shared flag
/// @return >=0 : assigned mutex Id
/// @return <0 : fail
/// @note   A mutex has the concept of an owner, whereas a semaphore does not.
///         A mutex provides priority inheritance protocol against proiorty inversion, whereas a binary semaphore does not.
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag);

//-------------------------------------------------------------------------------------------------
/// Delete the specified mutex
/// @ingroup MsOS_Task
/// @param  s32MutexId  \b IN: mutex ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It is important that the mutex be in the unlocked state when it is
///            destroyed, or else the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteMutex (MS_S32 s32MutexId);

//-------------------------------------------------------------------------------------------------
/// Attempt to lock a mutex
/// @ingroup MsOS_Task
/// @param  s32MutexId  \b IN: mutex ID
/// @param  u32WaitMs   \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the mutex is locked
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ObtainMutex (MS_S32 s32MutexId, MS_U32 u32WaitMs);

//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a mutex
/// @ingroup MsOS_Task
/// @param  s32MutexId  \b IN: mutex ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   Only the owner thread of the mutex can unlock it.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ReleaseMutex (MS_S32 s32MutexId);

//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a mutex by another thread
/// @param  s32MutexId  \b IN: mutex ID
/// @param  bEnable     \b IN: True(Enable) or False(Disable)
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   Let another thread can unlock it.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableCrossThreadReleaseMutex (MS_S32 s32MutexId, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get a mutex informaton
/// @ingroup MsOS_Task
/// @param  s32MutexId  \b IN: mutex ID
/// @param  peAttribute \b OUT: ptr to suspended mode: E_MSOS_FIFO / E_MSOS_PRIORITY
/// @param  pMutexName  \b OUT: ptr to mutex name
/// @return TRUE : succeed
/// @return FALSE : the mutex has not been created.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoMutex (MS_S32 s32MutexId, MsOSAttribute *peAttribute, char *pMutexName);

//
// read-write lock
//

//-------------------------------------------------------------------------------------------------
/// Create a read-write lcok in the unlocked state
/// @param  eAttribute  \b IN: E_MSOS_FIFO: suspended in FIFO order (not used)
/// @param  pMutexName  \b IN: read-write lock name
/// @param  u32Flag  \b IN: process data shared flag
/// @return >=0 : assigned read-write lock Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateRWLock ( MsOSAttribute eAttribute, char *pRWLockName1, MS_U32 Flag);

//-------------------------------------------------------------------------------------------------
/// Delete the specified read-write lock
/// @param  s32RWLockId  \b IN: read-write lock ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It is important that the read-write lock be in the unlocked state when it is
///            destroyed, or else the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteRWLock (MS_S32 s32RWLockId);

//-------------------------------------------------------------------------------------------------
/// Attempt to lock a read-write lock
/// @param  s32RWLockId  \b IN: read-write lock ID
/// @param  u32WaitMs   \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the read-write lock is locked
/// @param  u32Type     \b IN: READ or WRITE
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ObtainRWLock(MS_S32 s32RWLockId, MS_U32 u32WaitMs, MS_U32 u32Type);

//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a read-write lock
/// @param  s32RWLockId  \b IN: read-write lock ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ReleaseRWLock(MS_S32 s32RWLockId, MS_U32 u32Type);

//-------------------------------------------------------------------------------------------------
/// Get a read-write lock informaton
/// @param  s32RWLockId  \b IN: read-write lock ID
/// @param  peAttribute \b OUT: ptr to suspended mode: E_MSOS_FIFO / E_MSOS_PRIORITY
/// @param  pRWLockName  \b OUT: ptr to read-write lock name
/// @return TRUE : succeed
/// @return FALSE : the read-write lock has not been created.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoRWLock (MS_S32 s32RWLockId, MsOSAttribute *peAttribute, char *pRWLockName);


//-------------------------------------------------------------------------------------------------
/// Create a semaphore
/// @ingroup MsOS_Task
/// @param  u32InitCnt \b IN: initial semaphore value
/// @param  eAttribute \b IN: E_MSOS_FIFO suspended in FIFO order
/// @param  pSemaphoreName \b IN: semaphore name
/// @return >=0 : assigned Semaphore Id
/// @return <0 : fail
/// @note   A semaphore does not have the concept of an owner; it is possible for one thread to lock a
///           binary semaphore and another thread to unlock it.
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateSemaphore (MS_U32 u32InitCnt,
                             MsOSAttribute eAttribute,
                             char *pName);

//-------------------------------------------------------------------------------------------------
/// Delete the specified semaphore
/// @ingroup MsOS_Task
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It is important that there are not any threads waiting on the semaphore
///             when this function is called or the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteSemaphore (MS_S32 s32SemaphoreId);

//-------------------------------------------------------------------------------------------------
/// Attempt to decrement a semaphore count
/// @ingroup MsOS_Task
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @param  u32WaitMs       \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the semaphore count = 0
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ObtainSemaphore (MS_S32 s32SemaphoreId, MS_U32 u32WaitMs);

//-------------------------------------------------------------------------------------------------
/// Increase a semaphore count
/// @ingroup MsOS_Task
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It's possible for any thread to increase the semaphore count
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ReleaseSemaphore (MS_S32 s32SemaphoreId);

//-------------------------------------------------------------------------------------------------
/// Get a semaphore informaton
/// @ingroup MsOS_Task
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @param  pu32InitCnt     \b OUT: ptr to initial semaphore value
/// @param  peAttribute     \b OUT: ptr to suspended mode: E_MSOS_FIFO / E_MSOS_PRIORITY
/// @param  pSemaphoreName  \b OUT: ptr to semaphore name
/// @return TRUE : succeed
/// @return FALSE : the semaphore has not been created.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoSemaphore (MS_S32 s32SemaphoreId, MS_U32 *pu32Cnt, MsOSAttribute *peAttribute, char *pSemaphoreName);

//-------------------------------------------------------------------------------------------------
/// Create an event group
/// @ingroup MsOS_Task
/// @param  pEventName  \b IN: event group name
/// @return >=0 : assigned Event Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateEventGroup (char *pName);

//-------------------------------------------------------------------------------------------------
/// Create an process shared event group
/// @ingroup MsOS_Task
/// @param  pEventName  \b IN: event group name
/// @return >=0 : assigned Event Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateEventGroup_PS (char *pName);

//-------------------------------------------------------------------------------------------------
/// Delete the event group
/// @ingroup MsOS_Task
/// @param  s32EventGroupId \b IN: event group ID
/// @return TRUE : succeed
/// @return FALSE : fail, sb is waiting for the event flag
/// @note event group that are being waited on must not be deleted
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteEventGroup (MS_S32 s32EventGroupId);

//-------------------------------------------------------------------------------------------------
/// Set the event flag (bitwise OR w/ current value) in the specified event group
/// @ingroup MsOS_Task
/// @param  s32EventGroupId \b IN: event group ID
/// @param  u32EventFlag    \b IN: event flag value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SetEvent (MS_S32 s32EventGroupId, MS_U32 u32EventFlag);

//-------------------------------------------------------------------------------------------------
/// Clear the specified event flag (bitwise XOR operation) in the specified event group
/// @ingroup MsOS_Task
/// @param  s32EventGroupId \b IN: event group ID
/// @param  u32EventFlag    \b IN: event flag value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ClearEvent (MS_S32 s32EventGroupId, MS_U32 u32EventFlag);

//-------------------------------------------------------------------------------------------------
/// Wait for the specified event flag combination from the event group
/// @ingroup MsOS_Task
/// @param  s32EventGroupId     \b IN: event group ID
/// @param  u32WaitEventFlag    \b IN: wait event flag value
/// @param  pu32RetrievedEventFlag \b OUT: retrieved event flag value
/// @param  eWaitMode           \b IN: E_AND/E_OR/E_AND_CLEAR/E_OR_CLEAR
/// @param  u32WaitMs           \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the event is not ready
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_WaitEvent (MS_S32  s32EventGroupId,
                        MS_U32  u32WaitEventFlag,
                        MS_U32  *pu32RetrievedEventFlag,
                        EventWaitMode eWaitMode,
                        MS_U32  u32WaitMs);

MS_S32 MsOS_WaitEvent_Interrupt (MS_S32  s32EventGroupId,
                        MS_U32  u32WaitEventFlag,
                        MS_U32  *pu32RetrievedEventFlag,
                        EventWaitMode eWaitMode,
                        MS_U32  u32WaitMs);

/*
//
// Signal management
//
MS_BOOL MsOS_CreateSignal (SignalCb pSignalCb);

MS_BOOL MsOS_ControlSignals (MS_U32 u32SignalMask);

MS_BOOL MsOS_SendSignals (MS_S32 s32TaskId, MS_U32 u32Signal);

MS_U32 MsOS_ReceiveSignals (void);
*/

//
// Timer management
//
MS_S32 MsOS_CreateTimer (TimerCb    pTimerCb,
                         MS_U32     u32FirstTimeMs,
                         MS_U32     u32PeriodTimeMs,
                         MS_BOOL    bStartTimer,
                         char       *pName);

//-------------------------------------------------------------------------------------------------
/// Delete the Timer
/// @ingroup MsOS_Task
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTimer (MS_S32 s32TimerId);

//-------------------------------------------------------------------------------------------------
/// Start the Timer
/// @ingroup MsOS_Task
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_StartTimer (MS_S32 s32TimerId);

//-------------------------------------------------------------------------------------------------
/// Stop the Timer
/// @ingroup MsOS_Task
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   MsOS_StopTimer then MsOS_StartTimer => The timer will trigger at the same relative
///             intervals that it would have if it had not been disabled.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_StopTimer (MS_S32 s32TimerId);

//-------------------------------------------------------------------------------------------------
/// Reset a Timer & reset the expiration periods
/// @ingroup MsOS_Task
/// @param  s32TimerId      \b IN: Timer ID
/// @param  u32FirstTimeMs  \b IN: first ms for timer expiration
/// @param  u32PeriodTimeMs \b IN: periodic ms for timer expiration after first expiration
///                                0: one shot timer
/// @param  bStartTimer     \b IN: TRUE: activates the timer after it is created
///                                FALSE: leaves the timer disabled after it is created
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ResetTimer (MS_S32     s32TimerId,
                         MS_U32     u32FirstTimeMs,
                         MS_U32     u32PeriodTimeMs,
                         MS_BOOL    bStartTimer);

//-------------------------------------------------------------------------------------------------
/// Get current system time in ms
/// @ingroup MsOS_Task
/// @return system time in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSystemTime (void);

//-------------------------------------------------------------------------------------------------
///[OBSOLETE]
/// @ingroup MsOS_Task
/// Time difference between current time and task time
/// @return system time diff in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_Timer_DiffTimeFromNow(MS_U32 u32TaskTimer); ///[OBSOLETE]

//-------------------------------------------------------------------------------------------------
///[OBSOLETE]
/// @ingroup MsOS_Task
/// Time difference between setting time and task time
/// @return system time diff in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_Timer_DiffTime(MS_U32 u32Timer, MS_U32 u32TaskTimer); ///[OBSOLETE]
//MS_BOOL MsOS_SetSystemTime (MS_U32 u32SystemTime);

//-------------------------------------------------------------------------------------------------
/// Create a Queue
/// @ingroup MsOS_Task
/// @param  pStartAddr      \b IN: It is useless now, can pass NULL.
/// @param  u32QueueSize    \b IN: queue size (byte unit) : now fixed as
///                                CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE * u32MessageSize
/// @param  eMessageType    \b IN: E_MSG_FIXED_SIZE / E_MSG_VAR_SIZE
/// @param  u32MessageSize  \b IN: message size (byte unit) for E_MSG_FIXED_SIZE
///                                max message size (byte unit) for E_MSG_VAR_SIZE
/// @param  eAttribute      \b IN: E_MSOS_FIFO suspended in FIFO order
/// @param  pQueueName      \b IN: queue name
/// @return assigned message queue ID
/// @return < 0 - fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateQueue (void           *pStartAddr,
                         MS_U32         u32QueueSize,
                         MessageType    eMessageType,
                         MS_U32         u32MessageSize,
                         MsOSAttribute  eAttribute,
                         char           *pQueueName);

/// Create a share Queue (cross process)
/// @param  pStartAddr      \b IN: It is useless now, can pass NULL.
/// @param  u32QueueSize    \b IN: queue size (byte unit) : now fixed as
///                                MSOS_QUEUE_SIZE * u32MessageSize
/// @param  eMessageType    \b IN: E_MSG_FIXED_SIZE / E_MSG_VAR_SIZE
/// @param  u32MessageSize  \b IN: message size (byte unit) for E_MSG_FIXED_SIZE
///                                max message size (byte unit) for E_MSG_VAR_SIZE
/// @param  eAttribute      \b IN: E_MSOS_FIFO suspended in FIFO order
/// @param  pQueueName      \b IN: queue name
/// @return assigned message queue ID
/// @return < 0 - fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateQueue_PS(void          *pStartAddr,
                           MS_U32        u32QueueSize,
                           MessageType   eMessageType,
                           MS_U32        u32MessageSize,
                           MsOSAttribute eAttribute,
                           char          *pQueueName);

//-------------------------------------------------------------------------------------------------
/// Delete the Queue
/// @ingroup MsOS_Task
/// @param  s32QueueId  \b IN: Queue ID
/// @return TRUE : succeed
/// @return FALSE :  fail
/// @note   It is important that there are not any threads blocked on the queue
///             when this function is called or the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteQueue (MS_S32 s32QueueId);

//-------------------------------------------------------------------------------------------------
/// Send a message to the end of the specified queue
/// @ingroup MsOS_Task
/// @param  s32QueueId  \b IN: Queue ID
/// @param  pu8Message  \b IN: ptr to msg to send. NULL ptr is not allowed
/// @param  u32Size     \b IN: msg size (byte)
/// @param  u32WaitMs   \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the queue is full
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SendToQueue (MS_S32 s32QueueId, MS_U8 *pu8Message, MS_U32 u32Size, MS_U32 u32WaitMs);

//-------------------------------------------------------------------------------------------------
/// Receive a message from the specified queue
/// @ingroup MsOS_Task
/// @param  s32QueueId      \b IN: Queue ID
/// @param  pu8Message      \b OUT: msg destination
/// @param  u32IntendedSize \b IN: intended msg size (byte unit) to receive:
/// @param  pu32ActualSize  \b OUT: actual msg size (byte unit) received
/// @param  u32WaitMs       \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the queue is empty
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RecvFromQueue (MS_S32 s32QueueId, MS_U8 *pu8Message, MS_U32 u32IntendedSize, MS_U32 *pu32ActualSize, MS_U32 u32WaitMs);

//-------------------------------------------------------------------------------------------------
/// Receive a message from the specified queue
/// @ingroup MsOS_Task
/// @param  s32QueueId      \b IN: Queue ID
/// @param  pu8Message      \b OUT: msg destination
/// @param  u32IntendedSize \b IN: intended msg size (byte unit) to receive:
/// @param  pu32ActualSize  \b OUT: actual msg size (byte unit) received
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_PeekFromQueue (MS_S32 s32QueueId, MS_U8 *pu8Message, MS_U32 u32IntendedSize, MS_U32 *pu32ActualSize);

//-------------------------------------------------------------------------------------------------
/// Attach the interrupt callback function to interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_AttachInterrupt (InterruptNum eIntNum, InterruptCb pIntCb);


//-------------------------------------------------------------------------------------------------
/// Attach the interrupt callback function to interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_AttachInterrupt_Shared (InterruptNum eIntNum, InterruptCb pIntCb);

//-------------------------------------------------------------------------------------------------
/// Detach the interrupt callback function from interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DetachInterrupt (InterruptNum eIntNum);

//-------------------------------------------------------------------------------------------------
/// Detach the interrupt callback function from interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DetachInterrupt_Shared (InterruptNum eIntNum);

//-------------------------------------------------------------------------------------------------
/// Enable (unmask) the interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableInterrupt (InterruptNum eIntNum);

//-------------------------------------------------------------------------------------------------
/// Debug the interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param IrqDebugOpt \n IN: Debug option
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DebugInterrupt (InterruptNum eIntNum, IrqDebugOpt eIrqDebugOpt);

//-------------------------------------------------------------------------------------------------
/// Disable (mask) the interrupt #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DisableInterrupt (InterruptNum eIntNum);

//-------------------------------------------------------------------------------------------------
/// Check the interrupt enable or disable #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return >= 0 : 0: FALSE, 1: TRUE
/// @return < 0  : error number
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CheckInterrupt(InterruptNum eIntNum);

//-------------------------------------------------------------------------------------------------
/// Notify the interrupt complete #
/// @ingroup MsOS_Interrupt
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CompleteInterrupt (InterruptNum eIntNum);

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @ingroup MsOS_Interrupt
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void);

//-------------------------------------------------------------------------------------------------
/// Disable all interrupts (including timer interrupt), the scheduler is disabled.
/// @ingroup MsOS_Interrupt
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32  MsOS_DisableAllInterrupts(void);

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @ingroup MsOS_Interrupt
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts);

//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @ingroup MsOS_Interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void);



#if defined(__aeon__)
typedef enum {
    E_EXCEPTION_BUS_ERROR = 2,
    E_EXCEPTION_DATA_PAGE_FAULT,
    E_EXCEPTION_INSTRUCTION_PAGE_FAULT,
    E_EXCEPTION_TICK_TIMER,             ///< tick timer, do not use directly
    E_EXCEPTION_UNALIGNED_ACCESS,
    E_EXCEPTION_ILLEGAL_INSTRUCTION,
    E_EXCEPTION_EXTERNAL_INTERRUPT,     ///< external interrupt, do not use directly
    E_EXCEPTION_DTLB_MISS,
    E_EXCEPTION_ITLB_MISS,
    E_EXCEPTION_RANGE,
    E_EXCEPTION_SYSCALL,                ///< caused by l.sys
    E_EXCEPTION_RESERVED,
    E_EXCEPTION_TRAP,                   ///< caused by l.trap
    E_EXCEPTION_MAX = E_EXCEPTION_TRAP,
} MHAL_EXCEPTION_TYPE;

typedef enum {
    E_INTERRUPT_TICK_TIMER, //< risc32 builtin tick timer
#if defined(MSOS_TYPE_NUTTX)
    E_INTERRUPT_00 = 0,
#else
    E_INTERRUPT_00 = 1,     //< PIC interrupt start from 1 for handler performance
#endif
    E_INTERRUPT_01,
    E_INTERRUPT_02,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
    E_INTERRUPT_08,
    E_INTERRUPT_09,
    E_INTERRUPT_10,
    E_INTERRUPT_11,
    E_INTERRUPT_12,
    E_INTERRUPT_13,
    E_INTERRUPT_14,
    E_INTERRUPT_15,
    E_INTERRUPT_16,
    E_INTERRUPT_17,
    E_INTERRUPT_18,
    E_INTERRUPT_19,
    E_INTERRUPT_20,
    E_INTERRUPT_21,
    E_INTERRUPT_22,
    E_INTERRUPT_23,
    E_INTERRUPT_24,
    E_INTERRUPT_25,
    E_INTERRUPT_26,
    E_INTERRUPT_27,
    E_INTERRUPT_28,
    E_INTERRUPT_29,
    E_INTERRUPT_30,
    E_INTERRUPT_31,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_02
#define E_INTERRUPT_IRQ         E_INTERRUPT_03
#define E_INTERRUPT_UART        E_INTERRUPT_19
#define E_INTERRUPT_MAX         E_INTERRUPT_31

typedef struct
{
    unsigned long   r[32];          ///< GPR registers
#ifdef __AEONR2__
    unsigned long   machi2;         // Highest 32-bits of new 32x32=64 multiplier
#endif
    unsigned long   machi;          // High and low words of
    unsigned long   maclo;          //   multiply/accumulate reg

    // These are only saved for exceptions and interrupts
    int             vector;         ///< vector number
    int             sr;             ///< status register
    unsigned long   pc;             ///< program counter

    // Saved only for exceptions, and not restored when continued:
    // Effective address of instruction/data access that caused exception
    unsigned long   eear;           ///< exception effective address
} MHAL_SavedRegisters;
#else
typedef enum {
    E_EXCEPTION_DATA_TLBERROR_ACCESS = 1,    // TLB modification exception
    E_EXCEPTION_DATA_TLBMISS_ACCESS,         // TLB miss (Load or IFetch)
    E_EXCEPTION_DATA_TLBMISS_WRITE,          // TLB miss (Store)
    E_EXCEPTION_DATA_UNALIGNED_ACCESS,       // Address error (Load or Ifetch)
    E_EXCEPTION_DATA_UNALIGNED_WRITE,        // Address error (store)
    E_EXCEPTION_CODE_ACCESS,                 // Bus error (Ifetch)
    E_EXCEPTION_DATA_ACCESS,                 // Bus error (data load or store)
    E_EXCEPTION_SYSTEM_CALL,                 // System call
    E_EXCEPTION_INSTRUCTION_BP,              // Break point
    E_EXCEPTION_ILLEGAL_INSTRUCTION,         // Reserved instruction
    E_EXCEPTION_COPROCESSOR,                 // Coprocessor unusable
    E_EXCEPTION_OVERFLOW,                    // Arithmetic overflow
    E_EXCEPTION_RESERVED_13,                 // Reserved
    E_EXCEPTION_DIV_BY_ZERO,                 // Division-by-zero [reserved vector]
    E_EXCEPTION_FPU,                         // Floating point exception
    E_EXCEPTION_MAX = E_EXCEPTION_FPU,
} MHAL_EXCEPTION_TYPE;

typedef enum {
    E_INTERRUPT_02 = 0,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_03
#define E_INTERRUPT_IRQ         E_INTERRUPT_02
#define E_INTERRUPT_TICK_TIMER  E_INTERRUPT_07
#define E_INTERRUPT_MAX         E_INTERRUPT_07
#define E_EXCEPTION_TRAP        E_EXCEPTION_RESERVED_13

typedef struct
{
    // These are common to all saved states
    unsigned long    d[32];          /* Data regs                    */
    unsigned long    hi;             /* hi word of mpy/div reg       */
    unsigned long    lo;             /* lo word of mpy/div reg       */

    // The status register contains the interrupt-enable bit which needs
    // to be preserved across context switches.
    unsigned long    sr;             /* Status Reg                   */

    // These are only saved for exceptions and interrupts
    unsigned long    vector;         /* Vector number                */
    unsigned long    pc;             /* Program Counter              */

    // These are only saved for exceptions, and are not restored
    // when continued.
    unsigned long    cause;          /* Exception cause register     */
    unsigned long    badvr;          /* Bad virtual address reg      */

} MHAL_SavedRegisters;
#endif

#if defined (__arm__) || defined (__aarch64__)
typedef void (*mhal_isr_t)(void);
#else
#if defined(MSOS_TYPE_NUTTX)
typedef int (*mhal_isr_t)(int irq, void *context);
#else
typedef void (*mhal_isr_t)(MHAL_SavedRegisters *regs, MS_U32 vector);
#endif
#endif

//-------------------------------------------------------------------------------------------------
/// Disable the CPU interrupt
/// @ingroup MsOS_ToBeRemove
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 ATTRIBUTE MsOS_CPU_DisableInterrupt (void);

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @ingroup MsOS_Interrupt_NOS
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void);

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @ingroup MsOS_Interrupt_NOS
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts);

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @ingroup MsOS_Interrupt_NOS
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void);

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @ingroup MsOS_Interrupt_NOS
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num);

//-------------------------------------------------------------------------------------------------
/// UnMask the CPU interrupt
/// @ingroup MsOS_Interrupt_NOS
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnMaskInterrupt (MHAL_INTERRUPT_TYPE intr_num);

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @ingroup MsOS_Interrupt_NOS
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void);

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @ingroup MsOS_Interrupt_NOS
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void);

//-------------------------------------------------------------------------------------------------
/// Attach the CPU interrupt callback function to interrupt #
/// @ingroup MsOS_Interrupt_NOS
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_AttachInterrupt (MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, MS_U32 dat);

//-------------------------------------------------------------------------------------------------
/// Detach the CPU interrupt callback function to interrupt #
/// @ingroup MsOS_Interrupt_NOS
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_DetachInterrupt (MHAL_INTERRUPT_TYPE intr_num);

//-------------------------------------------------------------------------------------------------
/// Attach the CPU exception callback function to interrupt #
/// @ingroup MsOS_Interrupt_NOS
/// @param  eIntNum \b IN: Exception number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Exception callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_AttachException (MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, MS_U32 dat);

//-------------------------------------------------------------------------------------------------
/// Detach the CPU exception callback function to interrupt #
/// @ingroup MsOS_Interrupt_NOS
/// @param  eIntNum \b IN: Exception number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Exception callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_DetachExceptiont (MHAL_EXCEPTION_TYPE expt_num);

//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @ingroup MsOS_Interrupt_NOS
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr);

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @ingroup MsOS_MPool
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_Dcache_Flush( MS_VIRT ptrStart, MS_U32 tSize );
#else
MS_BOOL MsOS_Dcache_Flush( MS_VIRT ptrStart, MS_SIZE tSize );
#endif
//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate whole the cache
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Flush_All(void);

//-------------------------------------------------------------------------------------------------
/// Invalidate the cache lines in the given range
/// @ingroup MsOS_MPool
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_Dcache_Invalidate( MS_VIRT ptrStart , MS_U32 tSize );
#else
MS_BOOL MsOS_Dcache_Invalidate( MS_VIRT ptrStart , MS_SIZE tSize );
#endif
//-------------------------------------------------------------------------------------------------
/// Write back if dirty the cache lines in the given range
/// @ingroup MsOS_MPool
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_Dcache_Writeback( MS_VIRT ptrStart , MS_U32 tSize );
#else
MS_BOOL MsOS_Dcache_Writeback( MS_VIRT ptrStart , MS_SIZE tSize );
#endif
#if defined(CHIP_T12) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A2) || defined(CHIP_A5) || defined(CHIP_A3) || defined(CHIP_Edison)

//-------------------------------------------------------------------------------------------------
/// MsOS_L2Cache_Flush (Detail need to be done.)
/// @ingroup MsOS_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_L2Cache_Flush(void);

//-------------------------------------------------------------------------------------------------
/// MsOS_ReadMemory (Detail need to be done.)
/// @ingroup MsOS_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_L2Cache_Read(void);
#endif

//
// CPU relative Operation
//
//-------------------------------------------------------------------------------------------------
/// MsOS_Sync (Detail need to be done.)
/// @ingroup MsOS_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
void MsOS_Sync(void);

typedef enum //_MsOSMPool_DbgLevel
{
    E_MsOSMPool_DBG_Release = 0,
    E_MsOSMPool_DBG_L1, // display error msg
} MsOSMPool_DbgLevel;

// Kernel related information
//-------------------------------------------------------------------------------------------------
/// System information
/// @ingroup MsOS_BASIC
/// @param  SysInfo \b IN: SysInfo from user
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_Info(IO_Sys_Info_t* SysInfo);

MS_BOOL MDrv_SYS_Info_EX(IO_Sys_Info_t_EX *SysInfo);

// MPool Operation
//-------------------------------------------------------------------------------------------------
/// Set MPool Debug level
/// @ingroup MsOS_BASIC
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Check MPool driver is initialized or not
/// @ingroup MsOS_ToBeRemove
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_IsInitialized(void);

//-------------------------------------------------------------------------------------------------
/// Init MPool driver
///  @ingroup MsOS_ToBeModified
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Init(void);

//-------------------------------------------------------------------------------------------------
/// Check ION driver is initialized or not
/// @ingroup MsOS_ION
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ION_IsInitialized(void);

//-------------------------------------------------------------------------------------------------
/// Init ION driver
/// @ingroup MsOS_ION
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ION_Init(void);

//-------------------------------------------------------------------------------------------------
/// Setting MPool driver
/// @ingroup MsOS_MPool
/// @param  pAddrVirt     \b IN: Virtual address that get from kernel mpool driver
/// @param  pu32AddrPhys  \b IN: Physical address about to be mapped
/// @param  pu32Size      \b IN: Size that to be mapped
/// @param  bNonCache     \b IN: Is Non-cache or not (1:Non-cache, 0:Cache)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Get(void** pAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache);

//-------------------------------------------------------------------------------------------------
/// Close MPool driver
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Close(void);

//-------------------------------------------------------------------------------------------------
/// Get MPool information (Total mapping/Used VA/Free VA)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#ifdef UFO_PUBLIC_HEADER_700
void MsOS_MPool_InfoMsg(void);
#else
void MsOS_MPool_InfoMsg(MS_U8 u8MsgType);
#endif
//-------------------------------------------------------------------------------------------------
/// Close ION driver
/// @ingroup MsOS_ION
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ION_Close(void);

//-------------------------------------------------------------------------------------------------
/// Get Physical address from user by using Virtual address
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrVirt     \b IN: Virtual address
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt);

//-------------------------------------------------------------------------------------------------
/// Get Virtual address from user by using Physical address (Non-Cache)
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrPhys     \b IN: Physical address
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys);

//-------------------------------------------------------------------------------------------------
/// Get Virtual address from user by using Physical address (Cache)
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrPhys     \b IN: Physical address
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_VIRT MsOS_MPool_PA2KSEG0(MS_PHY pAddrPhys);

//-------------------------------------------------------------------------------------------------
/// Get Bus address from user by using Physical address
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrPhys     \b IN: Bus address
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_PHY MsOS_MPool_PA2BA(MS_PHY pAddrPhys);

//-------------------------------------------------------------------------------------------------
/// Get Physical address from user by using Bus address
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrPhys     \b IN: Physical address
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_PHY MsOS_MPool_BA2PA(MS_PHY pAddrPhys);

//-------------------------------------------------------------------------------------------------
/// Check if MPool already allocated Virtual address to the input Physical address (Non-Cache)
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrPhys     \b IN: Physical address
/// @return TRUE : mapped
/// @return FALSE : not mapped
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_IsPA2KSEG1Mapped(MS_PHY pAddrPhys);

//-------------------------------------------------------------------------------------------------
/// Check if MPool already allocated Virtual address to the input Physical address (Non-Cache)
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrPhys     \b IN: Physical address
/// @return TRUE : mapped
/// @return FALSE : not mapped
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_IsPA2KSEG0Mapped(MS_PHY pAddrPhys);

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @ingroup MsOS_ToBeRemove
/// @param  pAddrVirt \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_MPool_Dcache_Flush(MS_U32 pAddrVirt, MS_U32 u32Size); // the input address should be user mode cacheable address
#else
MS_BOOL MsOS_MPool_Dcache_Flush(MS_VIRT pAddrVirt, MS_SIZE tSize); // the input address should be user mode cacheable address
#endif
//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate whole the cache
/// @ingroup MsOS_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Dcache_Flush_All(void);

//-------------------------------------------------------------------------------------------------
/// Mapping MPool driver
/// @ingroup MsOS_MPool
/// @param  u8MiuSel      \b IN: Virtual address that get from kernel mpool driver
/// @param  u32Offset     \b IN: Physical address about to be mapped
/// @param  u32MapSize    \b IN: Size that to be mapped
/// @param  bNonCache     \b IN: Is Non-cache or not (1:Non-cache, 0:Cache)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_700)
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_U8 u8MapMode);
#elif defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache);
#else
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_BOOL bNonCache);
#endif
MS_BOOL __attribute__((weak)) MsOS_MPool_Mapping_v2(MS_U8 u8MiuSel, MS_PHY offset, MS_SIZE tMapSize, MS_U8 u8MapMode);

//-------------------------------------------------------------------------------------------------
/// Unmapping MPool driver
/// @ingroup MsOS_MPool
/// @param  u32VirtStart   \b IN: Virtual address that get from kernel mpool driver
/// @param  u32MapSize     \b IN: The size that to be unmapped
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_MPool_UnMapping(MS_U32 u32VirtStart, MS_U32 u32MapSize);
#else
MS_BOOL MsOS_MPool_UnMapping(MS_VIRT ptrVirtStart, MS_SIZE tMapSize);
#endif

//-------------------------------------------------------------------------------------------------
/// Mapping MPool driver (dynamc)
/// @ingroup MsOS_ToBeRemove
/// @param  u8MiuSel      \b IN: Virtual address that get from kernel mpool driver
/// @param  u32Offset     \b IN: Physical address about to be mapped
/// @param  u32MapSize    \b IN: Size that to be mapped
/// @param  bNonCache     \b IN: Is Non-cache or not (1:Non-cache, 0:Cache)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_700)
MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_U8 u8MapMode);
#elif defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache);
#else
MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_BOOL bNonCache);
#endif
MS_BOOL __attribute__((weak)) MsOS_MPool_Mapping_Dynamic_v2(MS_U8 u8MiuSel, MS_PHY pOffset, MS_SIZE u32MapSize, MS_U8 u8MapMode);

//-------------------------------------------------------------------------------------------------
/// Get the size of Linux kernel
/// @ingroup MsOS_ToBeRemove
/// @param  lx_addr      \b IN: address of lx memory
/// @param  lx_size     \b IN:  size of lx memory
/// @param  lx2_addr    \b IN: address of lx2 memory
/// @param  lx2_size     \b IN: size of lx2 memory
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_MPool_Kernel_Detect(MS_U32 *lx_addr, MS_U32 *lx_size, MS_U32 *lx2_addr, MS_U32 *lx2_size);
#else
MS_BOOL MsOS_MPool_Kernel_Detect(MS_PHY *lx_addr, MS_U64 *lx_size, MS_PHY *lx2_addr, MS_U64 *lx2_size);
#endif
//-------------------------------------------------------------------------------------------------
/// Set watch point
/// @ingroup MsOS_BASIC
/// @param  u32Addr      \b IN: address to be set watch point
/// @param  u32ASID     \b IN: the ASID of process
/// @param  u8Global    \b IN: Global value
/// @param  u8WType     \b IN: Watch point type
/// @param  u32Mask     \b IN: mask
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_SetWatchPT(MS_VIRT u32Addr, MS_U32 u32ASID, MS_U8 u8Global, MS_U8 u8WType, MS_U32 u32Mask);

//-------------------------------------------------------------------------------------------------
/// Get watch point
/// @ingroup MsOS_BASIC
/// @param  str      \b IN: Get the watch pointer address
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_GetWatchPT(char* str);

//-------------------------------------------------------------------------------------------------
/// Set watch point
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// ION Mapping (Detail need to be done.)
/// @ingroup MsOS_ION
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_ION_Mapping(MS_U32 u32ION_Alloc_Flag, MS_U8 u8ION_FixAddr_MiuSel, MS_U32 u32ION_FixAddrOffset, MS_U32 u32MapSize, MS_U32 u32Driver_Heap_ID, MS_BOOL bNonCache);

#if defined(MSOS_TYPE_LINUX)
#ifdef ENABLE_KERNEL_DLMALLOC
#include "dlmalloc.h"
//-------------------------------------------------------------------------------------------------
/// Create a variable-size memory pool dynamically
/// @ingroup MsOS_MPool
/// @param  base               \b IN: starting address for the memory pool
/// @param  capacity           \b IN: pool size in bytes
/// @param  locked             \b IN: set to 1
/// @return >=0 : assigned memory pool address
/// @return = 0 : fail
//-------------------------------------------------------------------------------------------------
mspace MsOS_MPool_CreateMemoryPool(void * base, size_t capacity, int locked);

//-------------------------------------------------------------------------------------------------
/// Delete a variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  msp                \b IN: memory pool address
//-------------------------------------------------------------------------------------------------
size_t MsOS_MPool_DeleteMemoryPool(mspace msp);

//-------------------------------------------------------------------------------------------------
/// Allocate a memory block with 16-Byte aligned starting address from the variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  msp                \b IN: memory pool address
/// @param  bytes              \b IN: request size
/// @return >=0 : allocated memory pool address
/// @return = 0 : fail
//-------------------------------------------------------------------------------------------------
void * MsOS_MPool_AllocateMemory(mspace msp, size_t bytes);

//-------------------------------------------------------------------------------------------------
/// Free a memory block from the variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  msp                \b IN: memory pool address
/// @param  pAddress           \b IN: pointer to previously allocated memory block
//-------------------------------------------------------------------------------------------------
void MsOS_MPool_FreeMemory(mspace msp, void *free_start_addr);

//-------------------------------------------------------------------------------------------------
/// Reallocate a block of memory with 4-byte aligned start address from the variable-size memory pool
/// @ingroup MsOS_MPool
/// @param  msp                  \b IN: memory pool address
/// @param  old_alloc_start_addr \b IN: points to the beginning of the original memory block
/// @param  bytes                \b IN: size of new memory block
/// @return >=0 : allocated new memory pool address
/// @return = 0 : fail
//-------------------------------------------------------------------------------------------------
void * MsOS_MPool_ReallocateMemory(mspace msp, void *old_alloc_start_addr, size_t bytes);
#endif
#endif

#define MsOS_MPool_PA2VA MsOS_MPool_PA2KSEG1

//-------------------------------------------------------------------------------------------------
/// MsOS_GetSHMSize
/// @ingroup MsOS_Task
/// @return share memory size
/// @return 0 : fail
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSHMSize(void);

//-------------------------------------------------------------------------------------------------
/// MsOS_Mapping_SharedMem (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_VIRT MsOS_Mapping_SharedMem(MS_U32 u32ShmSize,MS_BOOL *bInit);

//-------------------------------------------------------------------------------------------------
/// MsOS_CreateNamedMutex (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateNamedMutex(MS_S8 *ps8MutexName);

//-------------------------------------------------------------------------------------------------
/// MsOS_LockMutex (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_LockMutex(MS_S32 u32Index,int flag);
#else
MS_BOOL MsOS_LockMutex(MS_S32 u32Index,MS_S32 flag);
#endif
//-------------------------------------------------------------------------------------------------
/// MsOS_LockMutex (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL MsOS_UnlockMutex(MS_S32 u32Index,int flag);
#else
MS_BOOL MsOS_UnlockMutex(MS_S32 u32Index,MS_S32 flag);
#endif
//-------------------------------------------------------------------------------------------------
/// MsOS_DeleteNamedMutexbyIndex (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteNamedMutexbyIndex(MS_S32 u32Index);

//-------------------------------------------------------------------------------------------------
/// MsOS_CreateNamedRWLock
/// @param  *ps8RWLockName  \b IN: read-write lock name
/// @return >=0 : assigned read-write lock Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateNamedRWLock(MS_S8 *ps8RWLockName);

//-------------------------------------------------------------------------------------------------
/// MsOS_DeleteNamedRWLockbyIndex (Detail need to be done.)
/// @param  u32Index        \b IN: read-write lock ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteNamedRWLockbyIndex(MS_S32 u32Index);

//-------------------------------------------------------------------------------------------------
/// MsOS_LockRWLock
/// @param  u32Index        \b IN: read-write lock ID
/// @param  flag            \b IN: 0: block, -1: non-block, other: waitint time
/// @param  u32Type         \b IN: READ or WRITE
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_LockRWLock(MS_S32 u32Index,MS_S32 flag, MS_U32 u32Type);

//-------------------------------------------------------------------------------------------------
/// MsOS_UnlockRWLock
/// @param  u32Index        \b IN: read-write lock ID
/// @param  flag            \b IN: not used
/// @param  u32Type         \b IN: READ or WRITE
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_UnlockRWLock(MS_S32 u32Index,MS_S32 flag,MS_U32 u32Type);


//-------------------------------------------------------------------------------------------------
/// MsOS_CreateNamedSemaphore
/// @ingroup MsOS_Task
/// @return MS_S32 : semaphore index id
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateNamedSemaphore(char *ps8SemaName, MS_U32 u32SemaNum);

//-------------------------------------------------------------------------------------------------
/// MsOS_ObtainNamedSemaphore
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ObtainNamedSemaphore(MS_S32 u32Index, MS_S32 flag);

//-------------------------------------------------------------------------------------------------
/// MsOS_ReleaseNamedSemaphore
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ReleaseNamedSemaphore(MS_S32 u32Index, MS_S32 flag);

//-------------------------------------------------------------------------------------------------
/// MsOS_MPool_Add_PA2VARange; (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL __attribute__((weak)) MsOS_MPool_Add_PA2VARange(MS_U32 u32PhysAddr, MS_U32 u32VirtAddr, MS_U32 u32MapSize, MS_BOOL bNonCache);
#else
MS_BOOL __attribute__((weak)) MsOS_MPool_Add_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache);
#endif
//-------------------------------------------------------------------------------------------------
/// MsOS_MPool_Remove_PA2VARange; (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_BOOL __attribute__((weak)) MsOS_MPool_Remove_PA2VARange(MS_U32 u32PhysAddr, MS_U32 u32VirtAddr, MS_U32 u32MapSize, MS_BOOL bNonCache);
#else
MS_BOOL __attribute__((weak)) MsOS_MPool_Remove_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache);
#endif
// Share memory operation
#define MAX_CLIENT_NAME_LENGTH  50UL
#define MSOS_SHM_QUERY          0x00000000UL
#define MSOS_SHM_CREATE         0x00000001UL

//-------------------------------------------------------------------------------------------------
/// MsOS_SHM_Init (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SHM_Init(void);

//-------------------------------------------------------------------------------------------------
/// MsOS_SHM_GetId (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SHM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_VIRT* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag);

//-------------------------------------------------------------------------------------------------
/// MsOS_SHM_FreeId (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SHM_FreeId(MS_U8* pu8ClientName, MS_U32 u32ShmId);
#if defined (MSOS_TYPE_NOS)
//-------------------------------------------------------------------------------------------------
/// MsOS_MEM_GetId (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MEM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_VIRT* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag);
#endif

//
// OS Dependent Macro
//

// Worldwide thread safe macro
// Usage:
//     MS_S32 os_X_MutexID;
//     os_X_MutexID = OS_CREATE_MUTEX(_M_);
//     if (os_X_MutexID < 0) {  return FALSE; }
//     if (OS_OBTAIN_MUTEX(os_X_MutexID, 1000) == FALSE) { return FALSE; }
//     ...
//     OS_RELEASE_MUTEX(os_X_MutexID);
//     return X;
//

#define OS_CREATE_MUTEX(_M_)        MsOS_CreateMutex(E_MSOS_FIFO, "OS_"#_M_"_Mutex", MSOS_PROCESS_SHARED)
#define OS_OBTAIN_MUTEX(_mx, _tm)   MsOS_ObtainMutex(_mx, _tm)
#define OS_RELEASE_MUTEX(_mx)       MsOS_ReleaseMutex(_mx)
#define OS_DELETE_MUTEX(_mx)        MsOS_DeleteMutex(_mx)
#define OS_DELAY_TASK(_msec)        MsOS_DelayTask(_msec)
#define OS_SYSTEM_TIME()            MsOS_GetSystemTime()
#define OS_ENTER_CRITICAL()         MsOS_DisableAllInterrupts();
#define OS_EXIT_CRITICAL()          MsOS_EnableAllInterrupts();

//-------------------------------------------------------------------------------------------------
// Virutal/Physial address operation
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MsOS_VA2PA (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_PHY MsOS_VA2PA(MS_VIRT addr);

//-------------------------------------------------------------------------------------------------
/// MsOS_PA2KSEG0 (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_VIRT MsOS_PA2KSEG0(MS_PHY addr);

//-------------------------------------------------------------------------------------------------
/// MsOS_PA2KSEG1 (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_VIRT MsOS_PA2KSEG1(MS_PHY addr);

//-------------------------------------------------------------------------------------------------
/// MsOS_PA2BA (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_PHY MsOS_PA2BA(MS_PHY PhyAddr);

//-------------------------------------------------------------------------------------------------
/// MsOS_BA2PA (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_PHY MsOS_BA2PA(MS_PHY BusAddr);

//-------------------------------------------------------------------------------------------------
/// MsOS_FlushMemory (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
void   MsOS_FlushMemory(void);

//-------------------------------------------------------------------------------------------------
/// MsOS_ReadMemory (Detail need to be done.)
/// @ingroup MsOS_MPool
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
void   MsOS_ReadMemory(void);

//-------------------------------------------------------------------------------------------------
/// MsOS_GetKattribute (Detail need to be done.)
/// @ingroup MsOS_BASIC
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetKattribute(char *pAttr);

//-------------------------------------------------------------------------------------------------
/// MsOS_KernelMode (utopia kernel or user mode)
/// @ingroup MsOS_BASIC
/// @param u32Module     \b IN: Utopia Module ID
/// @param bSet          \b IN: TRUE=>Set  FALSE=>Get
/// @return TRUE : Kernel mode
/// @return FALSE : User mode
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_KernelMode(MS_U32 u32Module, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// MsOS_KernelMemoryRegionCheck (address is in kerenl space or not)
/// @ingroup MsOS_BASIC
/// @return TRUE : address is in kernel space
/// @return FALSE : address is in user space or NULL pointer
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_KernelMemoryRegionCheck(const void* const addr);

#define MS_VA2PA(_addr_)                (MS_PHY)MsOS_VA2PA((_addr_))
#define MS_PA2KSEG0(_addr_)             (MS_VIRT)MsOS_PA2KSEG0((_addr_))
#define MS_PA2KSEG1(_addr_)             (MS_VIRT)MsOS_PA2KSEG1((_addr_))

//-------------------------------------------------------------------------------------------------
// Debug message
//-------------------------------------------------------------------------------------------------
#define MS_CRITICAL_MSG(x)      x       // for dump critical message
#define MS_FATAL_MSG(fmt,...) printf( "[MS_FATAL]: %s: %d \n"  fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__)


#if defined (MSOS_TYPE_LINUX_KERNEL)
  #define printf(fmt, args...) printk( fmt, ## args)                 // utopia being a kernel driver should use printk
#endif

#if defined (MS_DEBUG)
  #define MS_DEBUG_MSG(x)       x
#elif defined (MS_OPTIMIZE)
  #define MS_DEBUG_MSG(x)               // retail version remove debug message
#endif

//-------------------------------------------------------------------------------------------------
// debug
//-------------------------------------------------------------------------------------------------
extern void MsOS_RegMyDbg(void);                                        ///< MsOS debug register itself debug

typedef MS_BOOL (*UartDbg_IP_CallBack) (int argc, char *argv[]);        ///< MsOS debug call back function prototype

///< define your own name, help, callback mapping here
typedef struct
{
    const char *Func_Name;
    const char *Func_Help;
    UartDbg_IP_CallBack pCallBack;
} MS_DBG_LINK;

///< for application to pass debug command into MsOS debug module
extern MS_BOOL MsOS_Dbg_ParseCmd(char *Cmd, MS_U32 u32CmdLen);

///< register your main menu here
extern MS_BOOL MsOS_Dbg_Regist(const char *Func_Name, const char *Func_Help, MS_DBG_LINK *pAryDbgLink);

///< for user to handle their own sub menu
extern MS_BOOL MsOS_Dbg_ExecuteSubCB(const char *Func_Name, int argc, char *argv[], MS_DBG_LINK *pAryDbgLink);

#ifdef __cplusplus
}
#endif

#endif // _MS_OS_H_
