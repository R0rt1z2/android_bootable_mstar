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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  CMDQ Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVCMDQ_H_
#define _DRVCMDQ_H_

#include "MsTypes.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define AES_ENABLE                   0x01
#define TDES_ENABLE                  0x10
#define WORD_MASK                    0x0000000F

#define DIRECT_MODE                 0x01
#define INCREMENT_MODE              0x00
#define RING_BUFFER_MODE            0x04

#define MSIF_CMDQ_LIB_CODE                     {'C','M','D','Q'}    //Lib code
#define MSIF_CMDQ_LIBVER                       {'0','1'}            //LIB version
#define MSIF_CMDQ_BUILDNUM                     {'0','1'}            //Build Number
#define MSIF_CMDQ_CHANGELIST                   {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define CMDQ_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_CMDQ_LIB_CODE,                  /* IP__                                             */  \
    MSIF_CMDQ_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_CMDQ_BUILDNUM,                          /* 00 ~ 99                                          */  \
    MSIF_CMDQ_CHANGELIST,  /* CL#                                              */  \
    MSIF_OS

typedef enum
{
    E_DRVCMDQ_DBG_Release = 0,
    E_DRVCMDQ_DBG_NONE = E_DRVCMDQ_DBG_Release,
    E_DRVCMDQ_DBG_L1, // display debug msg
    E_DRVCMDQ_DBG_CRITICAL = E_DRVCMDQ_DBG_L1,
    E_DRVCMDQ_DBG_ERROR,
    E_DRVCMDQ_DBG_DEBUG,
    E_DRVCMDQ_DBG_MESSAGE,
    E_DRVCMDQ_DBG_ALL = E_DRVCMDQ_DBG_MESSAGE
} DrvCMDQ_DbgLevel;

// Trigger Bus
typedef enum	// wait command signal defination
{
    GE_GE2CMDQ_TRI = 0,
    BDMA_INT_BDMA_0,
    BDMA_INT_BDMA_1,
    GPU_GPU2CMDQ_TRIG,
    GOPG0_GOP_CMDQ_INT_0,
    GOPG1_GOP_CMDQ_INT_1,
    GOPG2_GOP_CMDQ_INT_2,
    GOPG3_GOP_CMDQ_INT_3,
    SCALAR_SC2CMDQ_TRIG_0,
    SCALAR_SC2CMDQ_TRIG_1,
    SCALAR_SC2CMDQ_TRIG_2,
    SCALAR_SC2CMDQ_TRIG_3,
    AESDMA_CMDQDMA_FINISHED,
    VOP_DCM2GOP_VDE,
    VOP_DCS2GOP_VDE,
    XC_DIP_CMDQ_INT,
    AESDMA_CMDQDMA_FINISHED_2,
    NONE_USED_SIGNAL, //AESDMA_CMDQDMA_FINISHED_2ND
}EN_CMDQTriggerBus_ID;

#define CMDQ_Write_RIU_Command        'W'
#define CMDQ_Polling_RIU_EQ_Command   'p'
#define CMDQ_Polling_RIU_NEQ_Command  'C'
#define CMDQ_Wait_SIG_Command         'F'

//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local variable
//-----------------------------------------------------------------------------------------------
#define CMDQ_DUMMY_ZERO                0x00000000
#define CMDQ_DUMMY_BIT_0x00            0x00000001
#define CMDQ_DUMMY_BIT_0x01            0x00000002
#define CMDQ_DUMMY_BIT_0x02            0x00000004
#define CMDQ_DUMMY_BIT_0x03            0x00000008
#define CMDQ_DUMMY_BIT_0x04            0x00000010
#define CMDQ_DUMMY_BIT_0x05            0x00000020
#define CMDQ_DUMMY_BIT_0x06            0x00000040
#define CMDQ_DUMMY_BIT_0x07            0x00000080
#define CMDQ_DUMMY_BIT_0x08            0x00000100
#define CMDQ_DUMMY_BIT_0x09            0x00000200
#define CMDQ_DUMMY_BIT_0x0A            0x00000400
#define CMDQ_DUMMY_BIT_0x0B            0x00000800
#define CMDQ_DUMMY_BIT_0x0C            0x00001000
#define CMDQ_DUMMY_BIT_0x0D            0x00002000
#define CMDQ_DUMMY_BIT_0x0E            0x00004000
#define CMDQ_DUMMY_BIT_0x0F            0x00008000
//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
struct COMMAND_ARRAY_FORMAT
{
    MS_U8   operation;
    MS_U32  destionation_address;     // the writed register address
    MS_U16  destionation_value;       // value writed to register
    MS_U16  mask;                     // value of mask
};

typedef struct COMMAND_ARRAY_FORMAT CAF_Struct;
typedef CAF_Struct *CAF_Struct_Pointer;

struct CMDQ_HEAD
{
    MS_SIZE  Command_Number;
    CAF_Struct_Pointer Pointer_To_CAFArray;
};
typedef struct CMDQ_HEAD CH_Struct;
typedef CH_Struct *CH_Struct_Pointer;


/// CMDQ DDI return value
typedef MS_U32                         DRVCMDQ_RESULT;

/// @name DRVCMDQ_RESULT
/// @ref DRVCMDQ_RESULT
/// return value
/// @{
#define DRVCMDQ_OK                   0x00000000
#define DRVCMDQ_FAIL                 0x00000001
#define DRVCMDQ_INVALID_PARAM        0x00000002
#define DRVCMDQ_FUNC_ERROR           0x00000003
#define DRVCMDQ_MIU_ADDR_ERROR       0x00000004
#define DRVCMDQ_CMDQ_FULL            0x00000005

/// @}

#define CMDQ_DMA_DONE                     0x00010000
#define CMDQ_DMA_PAUSE                    0x00020000
#define CMDQ_STATES_GROUP                 (CMDQ_DMA_DONE  | \
                                             CMDQ_DMA_PAUSE  )


#define CMDQ_VERSION            0x00000001

/*! \defgroup SYS SYS modules
    The CMDQ(COMMAND QUEUE) reads a set of commands from memory and then decodes commands and execute it. There are five actions as below:
    - Null: do nothing but execute next command.
    - Write: write RIU command to XARB.
    - Wait: wait for a specified bit of trigger bus and then execute next command.
    - Polling_eq: read RIU data and check specified bits. If it is equal, next command will be executed.
    - Polling_neq: read RIU data and check specified bits. If it is not equal, next command will be executed.\n
    For more information,see \link SYS_CMDQ Hardware command Q interface (drvCMDQ.h)  \endlink \n

    MMIO maps the RIU to userspace so that drivers are capable to access it directly.\n
    For more information,see \link MMIO_BASIC basic mmio interface  \endlink \n

    MsOS is an OS abstraction layer, which is for utopia drivers
    not to consider different system API usage on each platform.\n
    For more information,see \link MSOS MsOS interface (MsOS.h)  \endlink \n

 *  \defgroup SYS_CMDQ Hardware command Q interface (drvCMDQ.h)
 *  \ingroup SYS

  The CMDQ(COMMAND QUEUE) reads a set of commands from memory and then decodes commands and execute it. There are five actions as below:
    - Null: do nothing but execute next command.
    - Write: write RIU command to XARB.
    - Wait: wait for a specified bit of trigger bus and then execute next command.
    - Polling_eq: read RIU data and check specified bits. If it is equal, next command will be executed.
    - Polling_neq: read RIU data and check specified bits. If it is not equal, next command will be executed.
  These five commands have their own format and each command occupies 64 bits. Therefore, for command queue, a 128-bit MIU interface of this IP, two commands are placed at the same address

  The CMDQ execution mode we use is Ring-Buffer Mode. There are two address pointers, r and w, move around a given memory space, which is limited between start pointer and end pointer.
  Software part tells cmdq where is the current write address pointer(w), and cmdq will do the command sets that software writes.
  Once software finishes storing commands in memory, it needs write current write address pointer(sw_wr_mi_wadr) and generate a trigger(mov_cmd_ptr) to inform cmdq.
  If software wants to know where cmdq reads, the trigger signal, rd_mi_radr_trig, needs to send to cmdq to grab current read address pointer (rd_mi_radr).
  Note that current read address pointer does not represent all data that is behind current read address pointer is sent to cmdq.

  <b> CMDQ Block Diagram: </b> \n
   \image html drvCMDQ_block_diagram.png

  <b> Command Format: </b> \n
   \image html drvCMDQ_command_format.png

  <b> Commands in Memory: </b> \n
   \image html drvCMDQ_command_in_memory.png

  <b> Null Command: </b> \n
   \image html drvCMDQ_null_cmd.png

  <b> Write Command: </b> \n
   \image html drvCMDQ_write_cmd.png

  <b> Wait Command: </b> \n
   \image html drvCMDQ_wait_cmd.png

  <b> Polling eq Command: </b> \n
   \image html drvCMDQ_polling_eq_cmd.png

  <b> Polling neq Command: </b> \n
   \image html drvCMDQ_polling_neq_cmd.png

  <b> Ring-Buffer: </b> \n
   \image html drvCMDQ_ring_buffer.png

 * \defgroup CMDQ_BASIC
 *  \ingroup SYS_CMDQ
 */

/// CMDQ notification function


//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Initialize CMDQ driver setting, including mmio and mutex
/// @ingroup CMDQ_BASIC
/// @param	CMDQ_MIU_SELECT
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Init(MS_U32 CMDQ_MIU_SELECT);

//-------------------------------------------------------------------------------------------------
/// Get CMDQ driver lib version
/// @ingroup    CMDQ_BASIC
/// @param	ppVersion
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// Trigger CMDQ hw to do cmd
/// @ingroup    CMDQ_BASIC
/// @param	bStart
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Start(MS_BOOL bStart);
//-------------------------------------------------------------------------------------------------
/// Set the Start and End address of CMDQ DRAM(use miu_offset and miu_select to get phy_addr)
/// @ingroup    CMDQ_BASIC
/// @param	SmallAddr
/// @param  BigAddr
/// @param  miu_select
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Get_Memory_Size(MS_PHY SmallAddr, MS_PHY BigAddr, MS_U32 miu_select);

//-------------------------------------------------------------------------------------------------
/// Set Start and End address to RIU throw hal layer
/// @ingroup    CMDQ_BASIC
/// @param	StartAddr
/// @param  EndAddr
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Set_Buffer(MS_PHY StartAddr, MS_PHY EndAddr);
//-------------------------------------------------------------------------------------------------
/// To be remove
/// @ingroup CMDQ_BASIC
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Reset(void);

//-------------------------------------------------------------------------------------------------
/// To be remove
/// @ingroup CMDQ_BASIC
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Exit(void);

//-------------------------------------------------------------------------------------------------
/// To be remove
/// @ingroup CMDQ_BASIC
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Stop(void);

//-------------------------------------------------------------------------------------------------
/// Set debug_level(for print)
/// @ingroup CMDQ_BASIC
/// @param	DbgLevel
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_SetDbgLevel(DrvCMDQ_DbgLevel DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Receive CMDQ command
/// @ingroup CMDQ_BASIC
/// @param	New_IP_Head
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Receive(CH_Struct_Pointer New_IP_Head);

//-------------------------------------------------------------------------------------------------
/// Insert null command
/// @ingroup CMDQ_BASIC
/// @param	Need_Null_Number
//-------------------------------------------------------------------------------------------------
void Insert_Redundant_Null(MS_U32 Need_Null_Number);

//-------------------------------------------------------------------------------------------------
/// Transfer command to memory
/// @ingroup CMDQ_BASIC
/// @param	Array_Command
/// @param  Number_Array_Command
//-------------------------------------------------------------------------------------------------
void MDrv_CMDQ_Transfer(CAF_Struct_Pointer Array_Command, MS_SIZE Number_Array_Command);

//-------------------------------------------------------------------------------------------------
/// Set dummy register for CMDQ
/// @ingroup CMDQ_BASIC
/// @param	dummyAddr
//-------------------------------------------------------------------------------------------------
void MDrv_CMDQ_Set_dummy_Address(MS_U32 DummyValue);
//-------------------------------------------------------------------------------------------------
/// Set CMD buffer in which MIU
/// @ingroup CMDQ_BASIC
/// @param	miu_select
//-------------------------------------------------------------------------------------------------
void MDrv_CMDQ_Set_MIU_SELECT(MS_U32 miu_select);

//-------------------------------------------------------------------------------------------------
/// Set CMDQ wait command timeout
/// @ingroup CMDQ_BASIC
/// @param	time
/// @param	ratio
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Set_timer_ratio(MS_U32 time,MS_U32 ratio);

//-------------------------------------------------------------------------------------------------
/// Print command of CMDQ¡Afor debug usage
/// @ingroup CMDQ_BASIC
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Printf_Crash_Command(void);

//-------------------------------------------------------------------------------------------------
/// Produce wait_command for some specific HW IP behaviour
/// @ingroup CMDQ_BASIC
/// @param	WaitTrigger_Bus_Command
/// @param	u16Tigger_ID
/// @param	bDetect_Falling_Mode
//-------------------------------------------------------------------------------------------------
void MDrv_CMDQ_Gen_WaitTrigger_Bus_Command(CAF_Struct_Pointer WaitTrigger_Bus_Command, EN_CMDQTriggerBus_ID u16Tigger_ID, MS_BOOL bDetect_Falling_Mode);

//-------------------------------------------------------------------------------------------------
/// Get CMDQ write point
/// @ingroup CMDQ_BASIC
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CMDQ_GetWritePoint(void);

//-------------------------------------------------------------------------------------------------
/// Str_usage
/// @ingroup CMDQ_BASIC
/// @param	u16PowerState
/// @return
/// @return
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CMDQ_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef __cplusplus
}
#endif


#endif // _DRVCMDQ_H_
