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
/// @brief  SAR Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SAR SAR interface
   *  \ingroup  G_PERIPHERAL
   \brief
   Successive Approximation Register controller
   <b>Features</b>
    - Keypa
    - SCART control signal detection
    - RF AGC level detection
    - GPIO

    <b>Successive approximation ADC Diagram</b> \n
     \image html drvSAR_pic.png

     \defgroup G_SAR_INIT Initialization Task relative
     \ingroup  G_SAR
     \defgroup G_SAR_COMMON Common Task relative
     \ingroup  G_SAR
     \defgroup G_SAR_CONTROL Control relative
     \ingroup  G_SAR
     *\defgroup G_SAR_OTHER  other relative
     *\ingroup  G_SAR
     *\defgroup G_SAR_ToBeModified SAR api to be modified
     *\ingroup  G_SAR
     \defgroup G_SAR_ToBeRemove SAR api to be removed
     \ingroup  G_SAR
*/

#ifndef _DRV_SAR_H_
#define _DRV_SAR_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// define SAR library version
#define MSIF_SAR_LIB_CODE               {'S','A','R','_'}
#define MSIF_SAR_LIBVER                 {'0','3'}
#define MSIF_SAR_BUILDNUM               {'0','9'}
#define MSIF_SAR_CHANGELIST             {'0','0','4','3','3','2','5','1'}

#define SAR_DRV_VERSION                 /* Character String for DRV/API version  */  \
    MSIF_TAG,                           /* 'MSIF' */  \
    MSIF_CLASS,                         /* '00' */  \
    MSIF_CUS,                           /* 0x0000 */  \
    MSIF_MOD,                           /* 0x0000 */  \
    MSIF_CHIP,                                       \
    MSIF_CPU,                                        \
    MSIF_SAR_LIB_CODE,                  /* IP__ */  \
    MSIF_SAR_LIBVER,                    /* 0.0 ~ Z.Z */  \
    MSIF_SAR_BUILDNUM,                  /* 00 ~ 99 */  \
    MSIF_SAR_CHANGELIST,                /* CL# */  \
    MSIF_OS

/// SAR driver
#define SAR_VERSION_STRING_MAX      33
#define SAR_DUMMY_KEY               0xFF
#define SAR_DUMMY_STATUS            0x00
#define SAR_VERSION            0x00000001

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
//##################################
//
//  Access SAR settings by using Mailbox
//  sent commands to PM
//
//##################################
/// define SAR driver info
typedef struct
{
    MS_U8 u8TrigMode : 1;  /// SAR trigger mode. 0: edge trigger, 1: level trigger
    MS_U8 u8OprMode : 1;   /// SAR operation mode. 0: one-shot, 1: free run
    MS_U8 u8CfgdChs : 3;   /// SAR configured channels
    MS_U8 u8Reserved : 3;  /// reserved for future
}SAR_DrvInfo;

/// define SAR channel bound
typedef struct
{
    MS_U8 u8UpBnd;  ///upper bound
    MS_U8 u8LoBnd;  ///low bound
} SAR_BndCfg;

/// define SAR parameter configuration
typedef struct
{
    MS_U8 u8SARChID;          /// SAR channel ID: from 0
    MS_U8 u8UpBnd;            ///upper bound
    MS_U8 u8LoBnd;            ///low bound
    MS_U8 u8KeyLevelNum;      /// SAR key level number
    MS_U8 u8KeyThreshold[8];  /// SAR key thresholds
    MS_U8 u8KeyCode[8];       /// SAR key code
} SAR_RegCfg;

/// emurate SAR fucntion return result
typedef enum
{
    /// SAR result for failure
    E_SAR_FAIL =0,
    /// SAR result for OK
    E_SAR_OK = 1,

} SAR_Result;

/// emurate SAR Level for 2.2V or 3.3V
typedef enum
{
    /// SAR result for failure
    E_SAR_20V =0,
    /// SAR result for OK
    E_SAR_33V = 1,

} SAR_LEVEL;


/// emurate SAR status
typedef enum
{
    /// SAR Driver Status: Initialization OK
    E_SAR_INIT_OK = (1<<0),
    /// SAR Driver Status: Callback installed
    E_SAR_INST_CLBK = (1<<1),
    /// SAR Driver Status: Get key code
    E_SAR_KEY_CODE = (1<<2),
    /// SAR Driver Status: SAR is enabled
    E_SAR_ENABLE = (1<<3),
    /// SAR Driver Status: Maximum status
    E_SAR_MAX_STATUS = 255
} SAR_Status;

//-------------------------------
// Mailbox Class : SAR
//-------------------------------
/// emurate SAR mailbox commands from cpu to mcu51
typedef enum
{
    /// mbx command for initialization
    E_SAR_CPUTo51_CMD_INIT=0x00,
    /// mbx command for configuration
    E_SAR_CPUTo51_CMD_CONFIG,
    /// mbx command for key code
    E_SAR_CPUTo51_CMD_KEYCODE,
    /// mbx command for set callback
    E_SAR_CPUTo51_CMD_SETCLBK,
    /// mbx command for library version
    E_SAR_CPUTo51_CMD_LIBVER,
    /// mbx command for status
    E_SAR_CPUTo51_CMD_STATUS,
    /// mbx command for enable
    E_SAR_CPUTo51_CMD_ENABLE,
    /// mbx command for driver info
    E_SAR_CPUTo51_CMD_INFO,
} SAR_CPUTo51CmdIdx;

/// emurate SAR mailbox commands ack from mcu51 to cpu
typedef enum
{
    //(1) Acknowledge from MCU51
    /// ack mbx command for initialization
    E_SAR_51ToCPU_CMD_ACK_INIT=0x00,
    /// ack mbx command for configuration
    E_SAR_51ToCPU_CMD_ACK_CONFIG,
    /// ack mbx command for key code
    E_SAR_51ToCPU_CMD_ACK_KEYCODE,
    /// ack mbx command for set callback
    E_SAR_51ToCPU_CMD_ACK_SETCLBK,
    /// ack mbx command for library version
    E_SAR_51ToCPU_CMD_ACK_LIBVER,
    ///ack mbx command for status
    E_SAR_51ToCPU_CMD_ACK_STATUS,
    ///ack mbx command for enable
    E_SAR_51ToCPU_CMD_ACK_ENABLE,
    ///ack mbx command for driver info
    E_SAR_51ToCPU_CMD_ACK_INFO,

    //(2) Notification from MCU51
    ///notification mbx command for key code
    E_SAR_51ToCPU_CMD_KEYCODE,

} SAR_51ToCPUCmdIdx;

/// emurate ack flags
typedef enum
{
    /// ack flag for null
    E_SAR_ACKFLG_NULL         = 0,
    /// ack flag for wait initialization
    E_SAR_ACKFLG_WAIT_INIT    = (1<<0),
    /// ack flag for wait configuration
    E_SAR_ACKFLG_WAIT_CONFIG  = (1<<1),
    /// ack flag for wait key code
    E_SAR_ACKFLG_WAIT_KEYCODE = (1<<2),
    /// ack flag for wait set callback
    E_SAR_ACKFLG_WAIT_SETCLBK = (1<<3),
    /// ack flag for wait library version
    E_SAR_ACKFLG_WAIT_LIBVER  = (1<<4),
    /// ack flag for wait status
    E_SAR_ACKFLG_WAIT_STATUS  = (1<<5),
    /// ack flag for wait enable
    E_SAR_ACKFLG_WAIT_ENABLE  = (1<<6),
    /// ack flag for wait driver info
    E_SAR_ACKFLG_WAIT_INFO    = (1<<7),
} SAR_AckFlags;

/// Callback function which is called in SAR ISR.
typedef void ( *SAR_Callback ) (MS_U8 u8Key, MS_U8 u8RepeatFlag);

///Define SAR debug level
typedef enum _SAR_DbgLvl
{
    E_SAR_DBGLVL_NONE = 0,      /// no debug message
    E_SAR_DBGLVL_WARNING,       /// show warning only
    E_SAR_DBGLVL_ERROR,         /// show error only
    E_SAR_DBGLVL_INFO,          /// show error & informaiton
    E_SAR_DBGLVL_ALL,           /// show error, information & funciton name
}SAR_DbgLvl;



//##################################
//
//  Access SAR settings directly by RIU without
//  using Mailbox sent commands to PM
//
//##################################
/// emurate SAR Kpd fucntion return result
typedef enum
{
    /// SAR result for failure
    E_SAR_KPD_FAIL =0,
    /// SAR result for OK
    E_SAR_KPD_OK = 1,

} SAR_KpdResult;

/// define SAR Kpd Bounds
typedef struct
{
    MS_U8 u8UpBnd;   //upper bound
    MS_U8 u8LoBnd;  //low bound
} SAR_KpdBndCfg;

/// define SAR Kpd Configuration
typedef struct
{
    MS_U8 u8SARChID;
    SAR_KpdBndCfg tSARChBnd;
    MS_U8 u8KeyLevelNum;
    MS_U8 u8KeyThreshold[8];
    MS_U8 u8KeyCode[8];
} SAR_KpdRegCfg;

/// define SAR driver info
typedef struct
{
    MS_U8 u8TrigMode : 1; /// SAR trigger mode. 0: edge trigger, 1: level trigger
    MS_U8 u8SingleChEn : 1; /// SAR single channel enable
    MS_U8 u8DigitOprMode : 1; /// SAR operation mode. 0: one-shot, 1: free run
    MS_U8 u8AtopFreerun : 1; /// SAR atop freerun mode. 0: controlled by digital, 1: free run
    MS_U8 u8DigitPwrDn : 1; /// SAR digital power down
    MS_U8 u8AtopPwrDn : 1; /// SAR atop power down
    MS_U8 u8HighChEn : 1; /// SAR high channel enable
    MS_U8 u8InterruptEn : 1; /// SAR interrupt enable

    MS_U8 u8ConfigUsedChs : 3; /// SAR configured channels
    MS_U8 u8MaxKpdSuppChs : 3; /// SAR IP max keypad channels supported
    MS_U8 u8Reserved: 2;///RFU1
    MS_U8 u8MaxKpdAvlbChs : 3; /// SAR driver max keypad channels provided
    MS_U8 u8MaxKpdAvlbLvls: 5; // /SAR driver max keypad channel levels provided
} SAR_KpdDrvInfo;

/// emurate SAR status
typedef enum
{
    E_SAR_KPD_INVALID=0,
    /// SAR Driver Status: Initialization OK
    E_SAR_KPD_INIT_OK = (1<<0),
    /// SAR Driver Status: Not Supported
    E_SAR_KPD_NOT_SUP = (1<<1),
    /// SAR Driver Status: Maximum status
    E_SAR_KPD_MAX_STATUS = 255
} SAR_KpdStatus;


///Define SAR debug level
typedef enum
{
    E_SAR_KPD_DBGLVL_NONE = 0,      /// no debug message
    E_SAR_KPD_DBGLVL_WARNING,       /// show warning only
    E_SAR_KPD_DBGLVL_ERROR,         /// show error only
    E_SAR_KPD_DBGLVL_INFO,          /// show error & informaiton
    E_SAR_KPD_DBGLVL_ALL,           /// show error, information & funciton name
}SAR_KpdDbgLvl;

//##################
//
//   For SAR ADC functions
//
//##################
/// emurate SAR ADC fucntion return result
typedef enum
{
    /// SAR result for failure
    E_SAR_ADC_FAIL =0,
    /// SAR result for OK
    E_SAR_ADC_OK = 1,

} SAR_AdcResult;

///Define SAR ADC debug level
typedef enum
{
    E_SAR_ADC_DBGLVL_NONE = 0,      /// no debug message
    E_SAR_ADC_DBGLVL_WARNING,       /// show warning only
    E_SAR_ADC_DBGLVL_ERROR,         /// show error only
    E_SAR_ADC_DBGLVL_INFO,          /// show error & informaiton
    E_SAR_ADC_DBGLVL_ALL,           /// show error, information & funciton name
}SAR_AdcDbgLvl;

///Define RGB HSync ADC channel
typedef enum
{
    E_SAR_ADC_HSYNC_CH0 = 0,  /// ADC HSync channel 0
    E_SAR_ADC_HSYNC_CH1,      /// ADC HSync channel 1
    E_SAR_ADC_HSYNC_CH2,      /// ADC HSync channel 2
    E_SAR_ADC_HSYNC_CH3,      /// ADC HSync channel 3
    E_SAR_ADC_HSYNC_CHMAX,    /// ADC HSync channel max
}SAR_AdcHSyncCh;

//##################
//
//   For SAR GPIO functions
//
//##################
/// emurate SAR GPIO fucntion return result
typedef enum
{
    /// SAR result for failure
    E_SAR_GPIO_FAIL =0,
    /// SAR result for OK
    E_SAR_GPIO_OK = 1,

} SAR_GpioResult;

///Define SAR GPIO debug level
typedef enum
{
    E_SAR_GPIO_DBGLVL_NONE = 0,      /// no debug message
    E_SAR_GPIO_DBGLVL_WARNING,       /// show warning only
    E_SAR_GPIO_DBGLVL_ERROR,         /// show error only
    E_SAR_GPIO_DBGLVL_INFO,          /// show error & informaiton
    E_SAR_GPIO_DBGLVL_ALL,           /// show error, information & funciton name
}SAR_GpioDbgLvl;

typedef enum
{
    /// Use GPIO mode
    E_SAR_GPIO_GPIO_MODE = 0,
    /// Use analog input mode
    E_SAR_GPIO_ANALOG_INPUT = 1,
} SAR_GpioPinMode;

//Define SAR GetKey Value Struct  ==========
typedef struct _SAR_KeyValue_Data_Member
{
    MS_U8 u8Channel;
    MS_U8 u8Key;
    MS_U8 u8Repeat;
}SAR_KeyValue_Data_Member;

typedef union
{
    SAR_KeyValue_Data_Member sData[16];
} SAR_KeyValue_Data;

typedef struct _SAR_KeyValue
{
    MS_U32 u32Version;
    SAR_KeyValue_Data unSarKeyData;
    MS_U32 u32Num; //u32Num indicate number of current  really getting SAR key
}SAR_KeyValue;

//##################
//
//   For SAR SMC functions
//
//##################
/// emurate SAR SMC fucntion return result
typedef enum{
    E_SAR_SMC_FAIL =0,
    E_SAR_SMC_OK = 1,
}SAR_SmcResult;

// Define SMC interrupt level
typedef enum{
    E_SAR_SMC_INT_LOW_ACT =0,
    E_SAR_SMC_INT_HIGH_ACT = 1
}SAR_SmcIntLevel;

///Define SAR SMC debug level
typedef enum
{
    E_SAR_SMC_DBGLVL_NONE = 0,      /// no debug message
    E_SAR_SMC_DBGLVL_WARNING,       /// show warning only
    E_SAR_SMC_DBGLVL_ERROR,         /// show error only
    E_SAR_SMC_DBGLVL_INFO,          /// show error & informaiton
    E_SAR_SMC_DBGLVL_ALL,           /// show error, information & funciton name
}SAR_SmcDbgLvl;
//===============================

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//##################################
//
//  Access SAR settings by using Mailbox
//  sent commands to PM
//
//##################################
//-------------------------------------------------------------------------------------------------
/// SAR Iinitialized function before using SAR to receive keys. (This sends command to PM51 using mailbox.)
/// @ingroup G_SAR_INIT
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_Init(void);


//-------------------------------------------------------------------------------------------------
/// SAR parameter configuration function. (This sends command to PM51 using mailbox.)
/// @ingroup G_SAR_COMMON
/// @param pSARRegCfg \b IN: pointer to the SAR configuration structure.
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_Config(SAR_RegCfg *pSARRegCfg);


//-------------------------------------------------------------------------------------------------
/// SAR get key code function. (This sends command to PM51 using mailbox.)
/// @ingroup G_SAR_COMMON
/// @param pu8Key \b OUT: Get SAR key code
/// @param pu8Repeat \b OUT: Get SAR key repeat status
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat);


//-------------------------------------------------------------------------------------------------
/// Set SAR callback function when receive SAR key. Support only one callback. If call it twice,
/// the first callback does not be called. (This sends command to PM51 using mailbox.)
/// Note: The callback runs at interrupt mode.
/// @ingroup G_SAR_COMMON
/// @param pCallback \b IN: Set the callback function when generate SAR interrupt.
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_SetCallback(SAR_Callback pCallback);


//-------------------------------------------------------------------------------------------------
/// Get SAR callback function which receive SAR key.
/// @ingroup G_SAR_COMMON
/// @return the callback function when generate SAR interrupt
//-------------------------------------------------------------------------------------------------
SAR_Callback MDrv_SAR_GetCallback(void);


//-------------------------------------------------------------------------------------------------
/// Get SAR library version function.
/// @ingroup G_SAR_COMMON
/// @param ppVersion \b OUT: pointer to library structure
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_GetLibVer(const MSIF_Version **ppVersion);


//-------------------------------------------------------------------------------------------------
/// Get SAR status function. (This sends command to PM51 using mailbox.)
/// @ingroup G_SAR_COMMON
/// @param pu8SARStatus \b OUT: pointer to status structure
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_GetStatus(MS_U8 *pu8SARStatus);


//-------------------------------------------------------------------------------------------------
/// Set SAR enable function. (This sends command to PM51 using mailbox.)
/// @ingroup G_SAR_COMMON
/// @param bEnable \b IN: TRUE: enable SAR, FALSE: disable SAR
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_Enable(MS_BOOL bEnable);


//-------------------------------------------------------------------------------------------------
/// Get SAR driver information function. (This sends command to PM51 using mailbox.)
/// @ingroup G_SAR_COMMON
/// @return @ref SAR_DrvInfo structure
//-------------------------------------------------------------------------------------------------
const SAR_DrvInfo* MDrv_SAR_GetInfo(void);

//-------------------------------------------------------------------------------------------------
/// Set SAR debug function level.
/// @ingroup G_SAR_COMMON
/// @param eLevel \b IN: E_SAR_DBGLVL_NONE/E_SAR_DBGLVL_WARNING/E_SAR_DBGLVL_ERROR/E_SAR_DBGLVL_INFO/E_SAR_DBGLVL_ALL
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_SetDbgLevel(SAR_DbgLvl eLevel);



//##################################
//
//  Access SAR settings directly by RIU without
//  using Mailbox sent commands to PM
//
//##################################
//-------------------------------------------------------------------------------------------------
/// Set SAR Keypad initialized function.
/// @ingroup G_SAR_INIT
/// @param None:
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_Init(void);

//-------------------------------------------------------------------------------------------------
/// Set SAR Keypad channel information function.
/// @ingroup G_SAR_COMMON
/// @param sarChInfo: sar channel info pointer
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_SetChInfo(SAR_KpdRegCfg *sarChInfo);

//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad key code function.
/// @ingroup G_SAR_COMMON
/// @param pu8Key: key code pointer
/// @param pu8Repeat: key repeat pointer
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat);

//-------------------------------------------------------------------------------------------------
/// Get SAR Multi-Keypad key code function with every SAR channel
/// @ingroup G_SAR_COMMON
/// @param SAR_KeyValue: key code struct pointer
/// @param u32Size: Struct Size
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetMultiKeyCode(SAR_KeyValue *sSarKey, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Get SAR current status function.
/// @ingroup G_SAR_COMMON
/// @param None:
/// @return E_SAR_KPD_INIT_OK: Init OK
/// @return others: other status
//-------------------------------------------------------------------------------------------------
SAR_KpdStatus MDrv_SAR_Kpd_GetStatus(void);

//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad driver information function.
/// @ingroup G_SAR_COMMON
/// @return @ref SAR_KpdDrvInfo structure
//-------------------------------------------------------------------------------------------------
const SAR_KpdDrvInfo* MDrv_SAR_Kpd_GetInfo(void);

//-------------------------------------------------------------------------------------------------
/// Set SAR Keypad debug function level.
/// @ingroup G_SAR_COMMON
/// @param eLevel \b IN: E_SAR_KPD_DBGLVL_NONE/E_SAR_KPD_DBGLVL_WARNING/E_SAR_KPD_DBGLVL_ERROR/E_SAR_KPD_DBGLVL_INFO/E_SAR_KPD_DBGLVL_ALL
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_SetDbgLevel(SAR_KpdDbgLvl eLevel);

//-------------------------------------------------------------------------------------------------
/// Set SAR as ADC channel.
/// @ingroup G_SAR_COMMON
/// @param u8Channel: sar ADC channel 0~7
/// @param bEnable: 1: configured as ADC, 0: configured as GPIO input
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_Config(MS_U8 u8Channel,MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get ADC value function for each SAR channel.
/// @ingroup G_SAR_COMMON
/// @param u8Channel: sar ADC channel 0~7
/// @return MS_U8: ADC value
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_SAR_Adc_GetValue(MS_U8 u8Channel);

//-------------------------------------------------------------------------------------------------
/// Set SAR High channel function for RGB HSync application
/// @ingroup G_SAR_COMMON
/// @param bEnable: 1: enable high channel, 0: disable high channel
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_SetHSyncChEn(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Switch RGB HSync MUX to direct signal to SAR High channel
/// @ingroup G_SAR_COMMON
/// @param u8HsynCh: HSync channel: 0, 1, 2, 3
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_SetHSyncCh(SAR_AdcHSyncCh eHSyncCh);

//-------------------------------------------------------------------------------------------------
/// Set SAR ADC debug function level.
/// @ingroup G_SAR_COMMON
/// @param eLevel \b IN: E_SAR_ADC_DBGLVL_NONE/E_SAR_ADC_DBGLVL_WARNING/E_SAR_ADC_DBGLVL_ERROR/E_SAR_ADC_DBGLVL_INFO/E_SAR_ADC_DBGLVL_ALL
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_SetDbgLevel(SAR_AdcDbgLvl eLevel);

//-------------------------------------------------------------------------------------------------
/// Set SAR as GPIO channel.
/// @ingroup G_SAR_COMMON
/// @param u8Channel: sar GPIO channel 0~7
/// @param u8InOut: 1: Input, 0:output
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_CfgDir(MS_U8 u8Channel,MS_BOOL u8InOut);

//-------------------------------------------------------------------------------------------------
/// Set SAR GPIO channel High/Low
/// @ingroup G_SAR_CONTROL
/// @param u8Channel: sar GPIO channel 0~7
/// @param bHighLow: 1: High, 0:low
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_SetOutput(MS_U8 u8Channel,MS_BOOL bHighLow);

//-------------------------------------------------------------------------------------------------
/// Get SAR GPIO channel High/Low
/// @ingroup G_SAR_COMMON
/// @param u8Channel: sar GPIO channel 0~7
/// @param pbResult: pointer to get Input level
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_GetInput(MS_U8 u8Channel, MS_BOOL* pbResult);

//-------------------------------------------------------------------------------------------------
/// Set SAR GPIO debug function level.
/// @ingroup G_SAR_COMMON
/// @param eLevel \b IN: E_SAR_GPIO_DBGLVL_NONE/E_SAR_GPIO_DBGLVL_WARNING/E_SAR_GPIO_DBGLVL_ERROR/E_SAR_GPIO_DBGLVL_INFO/E_SAR_GPIO_DBGLVL_ALL
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_SetDbgLevel(SAR_GpioDbgLvl eLevel);

//-------------------------------------------------------------------------------------------------
/// Set SAR Interrupt mask
/// @ingroup G_SAR_COMMON
/// @param u8Channel: sar GPIO channel 0~7
/// @param bEnable:  1: enable interrupt, 0: disable interrupt
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_CfgInterrupt(MS_U8 u8Channel, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set SAR Interrupt wakeup
/// @ingroup G_SAR_COMMON
/// @param bEnable:  1: enable interrupt, 0: disable interrupt
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_CfgPMWakeup(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set SAR Clear Interrupt Status
/// @ingroup G_SAR_CONTROL
/// @param u8Channel: sar GPIO channel 0~7
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_SAR_ClearInterrupt(MS_U8 u8Channel);

//-------------------------------------------------------------------------------------------------
/// Get SAR Status
/// @ingroup G_SAR_COMMON
/// @param u8Channel: sar GPIO channel 0~7
/// @return E_SAR_GPIO_OK: trigger interrupt
/// @return E_SAR_GPIO_FAIL or interrrupt do not trigger
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_GetInterruptStatus(MS_U8 u8Channel);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SAR_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_MMIO_Init(void); 

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SAR_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_CfgChannelBound(MS_U8 u8Channel, MS_U16 u16Upper, MS_U16 u16Lower);

SAR_KpdResult MDrv_SAR_Kpd_SetADCSamplingDuration(MS_U8 u8Duration);

//-------------------------------------------------------------------------------------------------
/// Set SAR 2.0V / 3.3V Level
/// @ingroup G_SAR_COMMON
/// @param eSarLevel:  E_SAR_33V: 3.3V, E_SAR_20V: 2.0V
/// @return Void
/// @return Void
//-------------------------------------------------------------------------------------------------
void MDrv_SAR_SetLevel(SAR_LEVEL eSarLevel);


MS_U32 MDrv_SAR_SetPowerState(EN_POWER_MODE u16PowerState);

//-------------------------------------------------------------------------------------------------
/// Set SAR Smart Card power detect configure function.
/// @param u8SarSmcIntChID: The sar channel ID of smart card power good interrupt
/// @param eSmcIntLevel \b IN: E_SAR_SMC_INT_LOW_ACT/E_SAR_SMC_INT_HIGH_ACT
/// @param u32BndVal: The sar channel bound value: 0x00(0V) ~ 0x3FF(3.3V)
/// @return E_SAR_SMC_OK: Success
/// @return E_SAR_SMC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_SmcResult MDrv_SAR_Smc_PwrDetCfg(MS_U8 u8SarSmcIntChID, SAR_SmcIntLevel eSmcIntLevel, MS_U32 u32BndVal);

//-------------------------------------------------------------------------------------------------
/// Set SAR SMC debug function level.
/// @param eLevel \b IN: E_SAR_SMC_DBGLVL_NONE/E_SAR_SMC_DBGLVL_WARNING/E_SAR_SMC_DBGLVL_ERROR/E_SAR_SMC_DBGLVL_INFO/E_SAR_SMC_DBGLVL_ALL
/// @return E_SAR_SMC_OK: Success
/// @return E_SAR_SMC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_SmcResult MDrv_SAR_Smc_SetDbgLevel(SAR_SmcDbgLvl eLevel);

//-------------------------------------------------------------------------------------------------
/// Get SAR GPIO channel Analog Input/GPIO mode
/// @param u8Channel: sar GPIO channel 0~7
/// @return E_SAR_GPIO_ANALOG_INPUT: is Analog Input
/// @return E_SAR_GPIO_GPIO_MODE: is GPIO
//-------------------------------------------------------------------------------------------------
SAR_GpioPinMode MDrv_SAR_Gpio_GetAnalogInput(MS_U8 u8Channel);

//T-Sensor
typedef void (*TSENSOR_CB_FUNC)(void);

MS_BOOL MDrv_TSensor_GetTemperatureRange(MS_S16 *s16UpBound_C, MS_S16 *s16LowBound_C);
MS_S16 MDrv_TSensor_GetTemperature(void);
MS_BOOL MDrv_TSensor_Init(void);
MS_BOOL MDrv_TSensor_SetTemperatureMointerRange (MS_S16 s16UpBound_C, MS_S16 s16LowBound_C, TSENSOR_CB_FUNC pTSensor);
MS_BOOL MDrv_TSensorIRQClear(void);
#ifdef __cplusplus
}
#endif


#endif // _DRV_SAR_H_

