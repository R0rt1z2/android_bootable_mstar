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
/// @brief  IR Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_IR IR interface
    \ingroup  G_PERIPHERAL
    
    \brief 

    Commonly, remote controls are Consumer IR devices which send digitally-coded pulses of infrared radiation to control functions such as 
    power, volume, tuning, temperature set point, fan speed, or other features. Remote controls for these devices are usually small wireless 
    handheld objects with an array of buttons for adjusting various settings such as television channel, track number, and volume. 
    For many devices, the remote control contains all the function controls while the controlled device itself has only a handful of essential primary controls.
    
    <b>Features</b>

    - Consumer electronics infrared protocols
    - Infrared, line of sight and operating angle
    - Remote control systems
    - The opto components, and circuits

    <b> IR Block Diagram: </b> \n
    \image html drvIR_pic1.png

    <b> Operation Code Flow: </b> \n
    -# Prepare IR setting for each operation
    -# Set and start IR in command handle
    -# Trigger IR
    
    \defgroup G_IR_INIT Initialization Task relative
    \ingroup  G_IR
    \defgroup G_IR_COMMON Common Task relative
    \ingroup  G_IR
    \defgroup G_IR_CONTROL Control relative
    \ingroup  G_IR
*/

#ifndef _DRV_IR_H_
#define _DRV_IR_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// define IR library version
#define MSIF_IR_LIB_CODE               {'I','R','_','_'}
#define MSIF_IR_LIBVER                 {'0','5'}
#define MSIF_IR_BUILDNUM               {'0','0' }
#define MSIF_IR_CHANGELIST             {'0','0','4','4','3','0','8','6'}

#define IR_DRV_VERSION                  /* Character String for DRV/API version */  \
    MSIF_TAG,                           /* 'MSIF' */  \
    MSIF_CLASS,                         /* '00' */  \
    MSIF_CUS,                           /* 0x0000 */  \
    MSIF_MOD,                           /* 0x0000 */  \
    MSIF_CHIP,                                       \
    MSIF_CPU,                                        \
    MSIF_IR_LIB_CODE,                   /* IP__ */  \
    MSIF_IR_LIBVER,                     /* 0.0 ~ Z.Z */  \
    MSIF_IR_BUILDNUM,                   /* 00 ~ 99 */  \
    MSIF_IR_CHANGELIST,                 /* CL# */  \
    MSIF_OS

/// IR driver
#define IR_VERSION_STRING_MAX       33
#define IR_DUMMY_KEY                0xFF
#define IR_DUMMY_STATUS             0x00

//IR Decode mode list
#define IR_DEC_RC_RC5               0
#define IR_DEC_RC_RC5X              1
#define IR_DEC_RC_RC6               2
#define IR_DEC_FULL                 3
#define IR_DEC_RAW                  4
#define IR_DEC_SW                   5
#define IR_DEC_FMT                  6

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// define IR driver info
typedef struct __attribute__((aligned (4096)))
{
    MS_U8 u8IsDecodeEn : 1;     /// check decode enable (Full/Raw)
    MS_U8 u8IsIntMsked : 1;     /// check int mask (Full/Raw)
    MS_U8 u8IsPolarInv : 1;     /// check signal polarity inverted (Full/Raw)
    MS_U8 u8IsFifoFullEn : 1;   /// Fifo full check enable (Full/Raw)
    MS_U8 u8IsFifoFull : 1;     /// Check Fifo full (Full/Raw)
    MS_U8 u8IsKeyMsbFst : 1;    /// Check Key MSB first (Full/Raw)
    MS_U8 u8IsBitInvEn : 1;     /// Check decode logic bit inverse (Full/Raw)
    MS_U8 u8IsGlhrmEn : 1;      /// glitch removal enable
    MS_U8 u8DecodeMode : 4;     /// decode mode.  0: RC5, 1: RC5X, 2: RC6, 3: Full, 4: Raw, 5: SW, 6:FMT
    MS_U8 u8CCodeBytes : 2;     /// customer code bytes: 1 or 2 bytes (Full only)
    MS_U8 u8Reserved : 2;       /// RFU
    MS_U8 u8CodeBits;           /// Code bits: 0x00~0x7F
    MS_U8 u8FifoDepth;          /// Fifo depth (Full/Raw)
    //for RC
    MS_U8 u8IsRCDecodeEn : 1;   /// check RC decode enable (RC)
    MS_U8 u8IsRCPolarInv : 1;   /// check RC signal polarity inverted (RC)
    MS_U8 u8RCReserved : 6;     /// RFU (RC)
}IR_DrvInfo;

/// define IR key code time & bounds
typedef struct
{
    MS_S16 s16Time;   ///key code time
    MS_S8 s8UpBnd;    ///upper bound
    MS_S8 s8LoBnd;    ///low bound
} IR_TimeCfg;

/// define IR time parameters
typedef struct __attribute__((aligned (4096)))
{
    IR_TimeCfg tHdr;       /// header code time
    IR_TimeCfg tOff;       /// off code time
    IR_TimeCfg tOffRpt;    /// off code repeat time
    IR_TimeCfg tLg01Hg;    /// logical 0/1 high time
    IR_TimeCfg tLg0;       /// logical 0 time
    IR_TimeCfg tLg1;       /// logical 1 time

} IR_TimeBnd;

/// define IR configuration parameters
typedef struct __attribute__((aligned (4096)))
{
    //MS_U8 u8ModeSel;          /// IR mode selection
    MS_U8 u8Ctrl0;          /// IR enable control 0
    MS_U8 u8Ctrl1;          /// IR enable control 1
    MS_U8 u8Clk_mhz;        /// IR required clock
    MS_U8 u8HdrCode0;       /// IR Header code 0
    MS_U8 u8HdrCode1;       /// IR Header code 1
    MS_U8 u8CCodeBytes;     /// Customer codes: 1 or 2 bytes
    MS_U8 u8CodeBits;       /// Code bits: 0x00~0x7F
    MS_U8 u8HdrCode20;      /// 2nd IR Header code 0
    MS_U8 u8HdrCode21;      /// 2nd IR Header code 1

} IR_RegCfg;

/// emurate IR fucntion return result
typedef enum
{
    /// IR result for failure
    E_IR_FAIL =0,
    /// IR result for OK
    E_IR_OK = 1,

} IR_Result;

/// emurate IR status
typedef enum
{
    /// IR Driver Status: Initialization OK
    E_IR_INIT_OK = (1<<0),
    /// IR Driver Status: Callback installed
    E_IR_INST_CLBK = (1<<1),
    /// IR Driver Status: FIFO empty
    E_IR_FIFO_EMPTY = (1<<2),
    /// IR Driver Status: FIFO full
    E_IR_FIFO_FULL = (1<<3),
    /// IR Driver Status: Get key code
    E_IR_KEY_CODE = (1<<4),
    /// IR Driver Status: Key timeout
    E_IR_KEY_TIMEOUT = (1<<5),
    /// IR Driver Status: Key decoded error
    E_IR_KEY_DECERR = (1<<6),
    /// IR Driver Status: IR is enabled
    E_IR_ENABLE = (1<<7),
    /// IR Driver Status: Maximum status
    E_IR_MAX_STATUS = 255
} IR_Status;

//-------------------------------
// Mailbox Class : IR
//-------------------------------
/// emurate IR mailbox commands from cpu to mcu51
typedef enum
{
    /// mbx command for initialization
    E_IR_CPUTo51_CMD_INIT=0x00,
    /// mbx command for configuration
    E_IR_CPUTo51_CMD_CONFIG,
    /// mbx command for key code
    E_IR_CPUTo51_CMD_KEYCODE,
    /// mbx command for set callback
    E_IR_CPUTo51_CMD_SETCLBK,
    /// mbx command for library version
    E_IR_CPUTo51_CMD_LIBVER,
    /// mbx command for status
    E_IR_CPUTo51_CMD_STATUS,
    /// mbx command for enable
    E_IR_CPUTo51_CMD_ENABLE,
    /// mbx command for driver info
    E_IR_CPUTo51_CMD_INFO,
} IR_CPUTo51CmdIdx;

/// emurate IR mailbox commands ack from mcu51 to cpu
typedef enum
{
    //(1) Acknowledge from MCU51
    /// ack mbx command for initialization
    E_IR_51ToCPU_CMD_ACK_INIT=0x00,
    /// ack mbx command for configuration
    E_IR_51ToCPU_CMD_ACK_CONFIG,
    /// ack mbx command for key code
    E_IR_51ToCPU_CMD_ACK_KEYCODE,
    /// ack mbx command for set callback
    E_IR_51ToCPU_CMD_ACK_SETCLBK,
    /// ack mbx command for library version
    E_IR_51ToCPU_CMD_ACK_LIBVER,
    ///ack mbx command for status
    E_IR_51ToCPU_CMD_ACK_STATUS,
    ///ack mbx command for enable
    E_IR_51ToCPU_CMD_ACK_ENABLE,
    ///ack mbx command for driver info
    E_IR_51ToCPU_CMD_ACK_INFO,

    //(2) Notification from MCU51
    ///notification mbx command for key code
    E_IR_51ToCPU_CMD_KEYCODE,

} IR_51ToCPUCmdIdx;

/// emurate ack flags
typedef enum
{
    /// ack flag for null
    E_IR_ACKFLG_NULL         = 0,
    /// ack flag for wait initialization
    E_IR_ACKFLG_WAIT_INIT    = (1<<0),
    /// ack flag for wait configuration
    E_IR_ACKFLG_WAIT_CONFIG  = (1<<1),
    /// ack flag for wait key code
    E_IR_ACKFLG_WAIT_KEYCODE = (1<<2),
    /// ack flag for wait set callback
    E_IR_ACKFLG_WAIT_SETCLBK = (1<<3),
    /// ack flag for wait library version
    E_IR_ACKFLG_WAIT_LIBVER  = (1<<4),
    /// ack flag for wait status
    E_IR_ACKFLG_WAIT_STATUS  = (1<<5),
    /// ack flag for wait enable
    E_IR_ACKFLG_WAIT_ENABLE  = (1<<6),
    /// ack flag for wait driver info
    E_IR_ACKFLG_WAIT_INFO    = (1<<7),
} IR_AckFlags;

/// Callback function which is called in IR ISR.
typedef void ( *IR_Callback ) (MS_U8 u8Key, MS_U8 u8RepeatFlag);

///Define IR debug level
typedef enum _IR_DbgLvl
{
    E_IR_DBGLVL_NONE = 0,      /// no debug message
    E_IR_DBGLVL_WARNING,       /// show warning only
    E_IR_DBGLVL_ERROR,         /// show error only
    E_IR_DBGLVL_INFO,          /// show error & informaiton
    E_IR_DBGLVL_ALL,           /// show error, information & funciton name
}IR_DbgLvl;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// IR Iinitialized function before using IR to receive keys.
/// @ingroup G_IR_INIT
/// @param pIRRegCfg \b IN: pointer to IR configuration structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_Init(IR_RegCfg *pIRRegCfg);

//-------------------------------------------------------------------------------------------------
/// IR configuration function for time bound
/// @ingroup G_IR_COMMON
/// @param pIRTimeBnd \b IN: pointer to IR time bound structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_Config(IR_TimeBnd *pIRTimeBnd);


//-------------------------------------------------------------------------------------------------
/// IR get key code function.
/// @ingroup G_IR_CONTROL
/// @param pu8Key \b OUT: Get IR key code
/// @param pu8Repeat \b OUT: Get IR key repeat status
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat);


//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR key. Support only one callback. If call it twice,
/// the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @ingroup G_IR_CONTROL
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_SetCallback(IR_Callback pCallback);


//-------------------------------------------------------------------------------------------------
/// Get IR callback function which receive IR key.
/// @ingroup G_IR_CONTROL
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_Callback MDrv_IR_GetCallback(void);


//-------------------------------------------------------------------------------------------------
/// Get IR library version function.
/// @ingroup G_IR_COMMON
/// @param ppVersion \b OUT: pointer to library structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_GetLibVer(const MSIF_Version **ppVersion);


//-------------------------------------------------------------------------------------------------
/// Get IR status function.
/// @ingroup G_IR_CONTROL
/// @param pu8IRStatus \b OUT: pointer to status structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_GetStatus(MS_U8 *pu8IRStatus);


//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_IR_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_Enable(MS_BOOL bEnable);


//-------------------------------------------------------------------------------------------------
/// Get IR driver information function.
/// @ingroup G_IR_COMMON
/// @return @ref IR_DrvInfo structure
//-------------------------------------------------------------------------------------------------
const IR_DrvInfo* MDrv_IR_GetInfo(void);

//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @ingroup G_IR_COMMON
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_SetDbgLevel(IR_DbgLvl eLevel);

#define IR_VERSION            0x00000001

//#ifdef MSOS_TYPE_LINUX
#if 1
#define IR_MAX_BUF_DPH  2
#define IR_MAX_BUF_LEN  256
typedef enum
{
    E_IR_DECMODE_HWFULL = 0,
    E_IR_DECMODE_HWRAW,
    E_IR_DECMODE_HWRC5,
    E_IR_DECMODE_HWRC5X,
    E_IR_DECMODE_HWRC6,
    E_IR_DECMODE_SW,
    E_IR_DECMODE_SHOT,
    E_IR_DECMODE_NONE
} IR_DecodeMode;

///IR data sequence format select for NEC-like (PPM modulation) formats.
///Note:
///S = System Code.
///C = Customer Code Bits, (ex: C8= customer code 8bits).
///D = Data (Key) Code Bits, (ex: D8= data code 8bits).
///P = Format with Parity Check (ex: 3th byte and 4th byte of NEC format).
typedef enum
{
    E_IR_XFM_NOTDEF = 0,      ///Format not define
    E_IR_XFM_C16D8D8P=1,      ///ex: NEC, Toshiba format
    E_IR_XFM_C8D8=2,          ///ex: Mitsubushi, Konka format
    E_IR_XFM_C4D8C4D8P=3,     ///ex: RCA format
    E_IR_XFM_C26D8D8P=4,      ///ex: C26D8D8P
    E_IR_XFM_C32D8D8P=5,      ///ex: C32D8D8P
    E_IR_XFM_C5D6C5D6P=6,     ///ex: C5D6C5D6P
    E_IR_XFM_C6D6C6D6P=7,     ///ex: C6D6C6D6P
    E_IR_XFM_D7C6=8,          ///ex: Sony-D7C6
    E_IR_XFM_D7C8=9,          ///ex: Sony-D7C8
    E_IR_XFM_D8C6=10,         ///ex: Sony-D8C6
    E_IR_XFM_D5_only=11,      ///ex: MV500
    E_IR_XFM_S1C4D6=12,       ///ex: IRT1250
    E_IR_XFM_C5D6D4=13,       ///ex: LR3715M
    E_IR_XFM_R1T1C3D6=14,     ///ex: M3004 LAB1-Carrier
    E_IR_XFM_RESERVED=15,     ///Reserved

} IR_ExtFormat;

typedef enum
{
    EN_IR_SHOT_P = 0x01,   /// 2'b01: only pshot edge detect for counter
    EN_IR_SHOT_N = 0x02,   /// 2'b10: only nshot edge detect for counter
    EN_IR_SHOT_PN = 0x03,  /// 2'b11/2'b00: both pshot/nshot edge detect for counter
    EN_IR_SHOT_INVLD,      /// Invalid for value greater than 2'b11

} IR_ShotSel;

/// define IR key code time & bounds
typedef struct
{
    MS_S16 s16Time;   ///key code time
    MS_S8 s8UpBnd;    ///upper bound
    MS_S8 s8LoBnd;    ///low bound
} IR_TimeBndParam;

/// define IR key code time tail
typedef struct
{
    MS_U32 gu32KeyMin;     /// Min Tail Time for key
    MS_U32 gu32KeyMax;     /// Max Tail Time for key
    MS_U32 gu32RptMin;     /// Min Tail Time for Rpt
    MS_U32 gu32RptMax;     /// Max Tail Time for Rpt
} IR_TimeTailParam;

/// define IR time parameters
typedef struct
{
    IR_TimeBndParam tHdr;       /// header code time
    IR_TimeBndParam tOff;       /// off code time
    IR_TimeBndParam tOffRpt;    /// off code repeat time
    IR_TimeBndParam tLg01Hg;    /// logical 0/1 high time
    IR_TimeBndParam tLg0;       /// logical 0 time
    IR_TimeBndParam tLg1;       /// logical 1 time
    IR_TimeBndParam tSepr;      /// Separate time
    MS_U32 u32TimeoutCyc;       /// Timeout cycle count
    MS_U16 u16RCBitTime;        /// RC Bit Time
    IR_TimeTailParam tTail;     /// Tail Time for sw shot mode
} IR_TimeCfgParam;

/// define IR configuration parameters
typedef struct
{
    MS_U8 u8DecMode;        /// IR mode selection
    MS_U8 u8ExtFormat;      /// IR extention format
    MS_U8 u8Ctrl0;          /// IR enable control 0
    MS_U8 u8Ctrl1;          /// IR enable control 1
    MS_U8 u8Clk_mhz;        /// IR required clock
    MS_U8 u8HdrCode0;       /// IR Header code 0
    MS_U8 u8HdrCode1;       /// IR Header code 1
    MS_U8 u8CCodeBytes;     /// Customer codes: 1 or 2 bytes
    MS_U8 u8CodeBits;       /// Code bits: 1~128 bits
    MS_U8 u8KeySelect;      /// IR select Nth key N(1~16)
    MS_U16 u16GlhrmNum;     /// Glitch Remove Number
    IR_ShotSel enShotSel;   /// Shot selection for SW decoder
    MS_BOOL bInvertPolar;   /// Invert the polarity for input IR signal

} IR_InitCfgParam;

/// define Ping-Pong Buffer structure for IR SW shot count
typedef struct
{
    MS_U32 u32Buffer[IR_MAX_BUF_DPH][IR_MAX_BUF_LEN];  ///Ping-Pong Buffer
    MS_U32 u32Length;                     ///Data Length for Read Index buffer
    MS_U8 u8RdIdx;                        ///Read Index
    MS_U8 u8WtIdx;                        ///Write Index
} IR_PulseShotInfo;

/// define HeaderInfo for sw mode change headercode in apps
typedef struct
{
    MS_U8 u8IRHeaderCode0;                        //IRHeaderCode0
    MS_U8 u8IRHeaderCode1;                        //IRHeaderCode1
    MS_U8 u8IR2HeaderCode0;                        //IR2HeaderCode0
    MS_U8 u8IR2HeaderCode1;                        //IR2HeaderCode1
}MS_MultiIR_HeaderInfo;

#define IR_IOC_MAGIC                'u'
#define MDRV_IR_INIT                _IO(IR_IOC_MAGIC, 0)
#define MDRV_IR_SET_DELAYTIME       _IOW(IR_IOC_MAGIC, 1, int)
#define MDRV_IR_GET_KEY             _IOW(IR_IOC_MAGIC, 2, int)
#define MDRV_IR_GET_LASTKEYTIME     _IOW(IR_IOC_MAGIC, 3, int)
#define MDRV_IR_PARSE_KEY           _IOW(IR_IOC_MAGIC, 4, int)
#define MDRV_IR_TEST                _IOW(IR_IOC_MAGIC, 5, int)
#define MDRV_IR_ENABLE_IR           _IOW(IR_IOC_MAGIC, 6, int)
#define MDRV_IR_IS_FANTASY_PROTOCOL_SUPPORTED _IOR(IR_IOC_MAGIC, 7, int)
#define MDRV_IR_ENABLE_FANTASY_DATA_TRANSFER _IOW(IR_IOC_MAGIC, 8, int)
#define MDRV_IR_SET_MASTER_PID       _IOW(IR_IOC_MAGIC, 9, int)
#define MDRV_IR_GET_MASTER_PID       _IOW(IR_IOC_MAGIC, 10, int)
#define MDRV_IR_INITCFG              _IOW(IR_IOC_MAGIC, 11, IR_InitCfgParam)
#define MDRV_IR_TIMECFG              _IOW(IR_IOC_MAGIC, 12, IR_TimeCfgParam)
#define MDRV_IR_GET_SWSHOT_BUF       _IOW(IR_IOC_MAGIC, 13, IR_PulseShotInfo)
#define MDRV_IR_SEND_KEY             _IOW(IR_IOC_MAGIC, 14, int)
#define MDRV_IR_SET_HEADER           _IOW(IR_IOC_MAGIC, 15, MS_MultiIR_HeaderInfo)

#define IR_IOC_MAXNR                 15

//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_IR_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_IR_OpenDevice(void);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_IR_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_IR_InitCfg(IR_InitCfgParam* pInitCfg);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_IR_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_IR_TimeCfg(IR_TimeCfgParam* pTimeCfg);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_IR_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_IR_GetPulseShot(IR_PulseShotInfo* pPulseShotInfo);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_IR_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_IR_SetMultiHeaderCode(MS_MultiIR_HeaderInfo* pMultiHeaderCode);

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register IR module
//--------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @ingroup G_IR_ToBeRemove
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void IRRegisterToUtopia(FUtopiaOpen ModuleType);
//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @ingroup G_IR_ToBeRemove
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_U32 IROpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @ingroup G_IR_ToBeRemove
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_U32 IRClose(void* pInstance);
//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @ingroup G_IR_ToBeRemove
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_U32 IRIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @ingroup G_IR_ToBeRemove
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_IR_SetPowerState(EN_POWER_MODE u16PowerState);


//IR_Private
typedef struct _IR_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}IR_RESOURCE_PRIVATE;

typedef struct _IR_INSTANT_PRIVATE
{
   
}IR_INSTANT_PRIVATE;


#endif

#ifdef __cplusplus
}
#endif


#endif // _DRV_IR_H_

